//
//  CSBinaryInputStream.h
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

#ifndef _CHILLISOURCE_CORE_FILE_CSBINARYINPUTSTREAM_H_
#define _CHILLISOURCE_CORE_FILE_CSBINARYINPUTSTREAM_H_

#include <ChilliSource/ChilliSource.h>

#include <unordered_map>

namespace ChilliSource
{
    namespace Core
    {
        //----------------------------------------------------------------------
        /// A file input stream for reading files that use Chilli Source's
        /// "Chunked" binary file format. The input stream wraps around a
        /// standard file stream and provides an easy and efficient API for
        /// loading files.
        ///
        /// The Chilli Source "Chunked" file format has 3 sections. The header,
        /// the chunk table and the chunk data. The header contains basic file
        /// information such as version numbers and the chunk table size. The
        /// chunk table describes the location and size of all file chunks. The
        /// Chunk data section contains each of the chunks described by the
        /// chunk tank. The quantity of chunks and format and contents of a chunk
        /// is determined by the specific file format.
        ///
        /// The file header is a fixed size and contains the following:
        ///
        ///   [4 bytes] The 4 ASCII character Chilli Source file id: "CSCS".
        ///
        ///   [4 bytes] Endianness check flag. This is an unsigned integer that
        ///   should always read 9999. It can be used to determine whether the
        ///   file is big or little endian.
        ///
        ///   [4 bytes] The CS file format Id. An unsigned integer describing which
        ///   of the chilli source file format the file is. For example a CSFont
        ///   file has an Id of 1.
        ///
        ///   [4 bytes] An unsigned integer describing the file format version
        ///   number.
        ///
        ///   [4 bytes] An unsigned integer describing the number of entries in
        ///   chunk table.
        ///
        /// The chunk table simply contains a series of entries, each describing
        /// a chunk. The format of a chunk table entry is as follows:
        ///
        ///   [4 bytes] A 4 ASCII character unique identifier for the chunk.
        ///
        ///   [4 bytes] An unsigned integer describing the offset of the chunk
        ///   from the start of the file in bytes.
        ///
        ///   [4 bytes] An unsigned integer describing the size of the chunk in
        ///   bytes.
        ///
        /// @author Ian Copland
        //----------------------------------------------------------------------
        class CSBinaryInputStream final
        {
        public:
            CS_DECLARE_NOCOPY(CSBinaryInputStream);
            //--------------------------------------------------------------
            /// Constructor. Creates a new binary stream to the given file
            /// path. Before use the IsValid() flag should be checked to
            /// ensure that the stream has successfully be opened.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the file.
            /// @param The file path.
            //--------------------------------------------------------------
            CSBinaryInputStream(StorageLocation in_storageLocation, const std::string& in_filePath);
            //--------------------------------------------------------------
            /// Returns whether or not the stream has successfully been
            /// created and is read for use. This should be checked prior
            /// to any other action on the stream. If it returns false, the
            /// stream should be discarded and no further action performed on
            /// it.
            ///
            /// @author Ian Copland
            ///
            /// @return Whether or not the stream was succesfully created.
            //--------------------------------------------------------------
            bool IsValid() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The CS file format Id.
            //--------------------------------------------------------------
            u32 GetFileFormatId() const;
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The file format version number.
            //--------------------------------------------------------------
            u32 GetFileFormatVersion() const;
            //--------------------------------------------------------------
            /// Reads the contents of the requested chunk into memory and
            /// returns it as a Binary Chunk to allow efficient and easy access
            /// to the chunks contents. If the file doesn't contain the
            /// requested chunk null will be returned.
            ///
            /// @author Ian Copland
            ///
            /// @param The 4 character unique identifier for the chunk.
            ///
            /// @return The chunk or null if it doesn't exist.
            //--------------------------------------------------------------
            CSBinaryChunkUPtr ReadChunk(const std::string& in_chunkId);
        private:
            //--------------------------------------------------------------
            /// A container for information on a single chunk in the chunk
            /// table.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------------
            struct ChunkInfo
            {
                u32 m_offset = 0;
                u32 m_size = 0;
            };
            //--------------------------------------------------------------
            /// Reads the chunk table. This should only be called immediately
            /// after reading the file header.
            ///
            /// @author Ian Copland
            ///
            /// @param The number of entries in the chunk table.
            //--------------------------------------------------------------
            void ReadChunkTable(u32 in_numEntries);
            
            FileStreamUPtr m_fileStream;
            bool m_isValid = false;
            u32 m_fileFormatId = 0;
            u32 m_fileFormatVersion = 0;
            std::unordered_map<std::string, ChunkInfo> m_chunkInfoMap;
        };
    }
}

#endif
