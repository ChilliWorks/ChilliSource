//
//  CSBinaryInputStream.cpp
//  Chilli Source
//  Created by Ian Copland on 28/10/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Core/File/CSBinaryInputStream.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/CSBinaryChunk.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            //--------------------------------------------------------------
            /// Reads the Chilli Source "Chunked" binary file header.
            ///
            /// @param Ian Copland
            ///
            /// @param The file stream.
            /// @param [Out] The file format Id.
            /// @param [Out] The file format version.
            /// @param [Out] The number of entries in the chunk table.
            ///
            /// @return Whether reading the header was successful or not.
            //--------------------------------------------------------------
            bool ReadHeader(FileStream* in_fileStream, const std::string& in_filePath, u32& out_fileFormatId, u32& out_fileFormatVersion, u32& out_numChunkTableEntries)
            {
                CS_ASSERT(in_fileStream != nullptr, "File stream cannot be null.");
                
                //read the header data
                const u32 k_headerSize = 20;
                std::unique_ptr<u8[]> headerData(new u8[k_headerSize]);
                in_fileStream->Read(reinterpret_cast<s8*>(headerData.get()), k_headerSize);
                
                //test file id.
                if (headerData[0] != 'C' || headerData[1] != 'S' || headerData[2] != 'C' || headerData[3] != 'S')
                {
                    CS_LOG_ERROR("Invalid Chilli Source file identifier in file: " + in_filePath);
                    return false;
                }
                
                //test file endianness.
                const u32 k_endiannessCheckFlagOffset = 4;
                u32 endiannessCheckFlag = *reinterpret_cast<const u32*>(headerData.get() + k_endiannessCheckFlagOffset);
                if (endiannessCheckFlag != 9999)
                {
                    CS_LOG_ERROR("File is big endian, only little endian is currently supported: " + in_filePath);
                    return false;
                }
                
                //read the file type
                const u32 k_fileFormatIdOffset = 8;
                out_fileFormatId = *reinterpret_cast<const u32*>(headerData.get() + k_fileFormatIdOffset);
                
                //read the file version
                const u32 k_fileFormatVersionOffset = 12;
                out_fileFormatVersion = *reinterpret_cast<const u32*>(headerData.get() + k_fileFormatVersionOffset);
                
                //read the number of chunk table entries
                const u32 k_numChunkTableEntriesOffset = 16;
                out_numChunkTableEntries = *reinterpret_cast<const u32*>(headerData.get() + k_numChunkTableEntriesOffset);
                
                return true;
            }
        }
        
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        CSBinaryInputStream::CSBinaryInputStream(StorageLocation in_storageLocation, const std::string& in_filePath)
        {
            FileSystem* fileSystem = Application::Get()->GetFileSystem();
            CS_ASSERT(fileSystem != nullptr, "CSBinaryInputStream missing required system: FileSystem.");
            
            m_fileStream = fileSystem->CreateFileStream(in_storageLocation, in_filePath, FileMode::k_readBinary);
            if (m_fileStream != nullptr && m_fileStream->IsBad() == false && m_fileStream->IsOpen() == true)
            {
                u32 numChunkTableEntries;
                if (ReadHeader(m_fileStream.get(), in_filePath, m_fileFormatId, m_fileFormatVersion, numChunkTableEntries) == false)
                {
                    m_fileStream.reset();
                    return;
                }
                
                ReadChunkTable(numChunkTableEntries);
                
                m_isValid = true;
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool CSBinaryInputStream::IsValid() const
        {
            return m_isValid;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        u32 CSBinaryInputStream::GetFileFormatId() const
        {
            return m_fileFormatId;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        u32 CSBinaryInputStream::GetFileFormatVersion() const
        {
            return m_fileFormatVersion;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        CSBinaryChunkUPtr CSBinaryInputStream::ReadChunk(const std::string& in_chunkId)
        {
            CS_ASSERT(m_isValid == true, "Cannot read chunk from invalid Input stream.");
            
            auto chunkInfoIt = m_chunkInfoMap.find(in_chunkId);
            if (chunkInfoIt != m_chunkInfoMap.end())
            {
                m_fileStream->SeekG(chunkInfoIt->second.m_offset, SeekDir::k_beginning);
                CS_ASSERT(m_fileStream->TellG() == chunkInfoIt->second.m_offset, "Seek failed.");
                
                std::unique_ptr<u8[]> chunkData(new u8[chunkInfoIt->second.m_size]);
                m_fileStream->Read(reinterpret_cast<s8*>(chunkData.get()), chunkInfoIt->second.m_size);
                
                return CSBinaryChunkUPtr(new CSBinaryChunk(std::move(chunkData), chunkInfoIt->second.m_size));
            }
            
            return nullptr;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void CSBinaryInputStream::ReadChunkTable(u32 in_numEntries)
        {
            const u32 k_chunkEntrySize = 12;
            
            //read the chunk table
            const u32 chunkTableDataSize = k_chunkEntrySize * in_numEntries;
            std::unique_ptr<u8[]> chunkTableData(new u8[chunkTableDataSize]);
            m_fileStream->Read(reinterpret_cast<s8*>(chunkTableData.get()), chunkTableDataSize);
            
            //iterate over the chunks
            for (u32 i = 0; i < in_numEntries; ++i)
            {
                const u32 chunkOffset = i * k_chunkEntrySize;
                
                //get the chunk identifier
                const u32 k_chunkIdSize = 4;
                std::string chunkId(reinterpret_cast<const s8*>(chunkTableData.get() + chunkOffset), k_chunkIdSize);
                
                ChunkInfo info;
                
                //get the chunk offset.
                const u32 k_offsetOffset = 4;
                info.m_offset = *reinterpret_cast<const u32*>(chunkTableData.get() + chunkOffset + k_offsetOffset);
                
                //get the chunk size.
                const u32 k_sizeOffset = 8;
                info.m_size = *reinterpret_cast<const u32*>(chunkTableData.get() + chunkOffset + k_sizeOffset);
                
                m_chunkInfoMap.emplace(chunkId, info);
            }
        }
    }
}
