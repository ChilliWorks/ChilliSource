//
//  FileSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 07/07/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Core/File/FileSystem.h>

#include <ChilliSource/Core/Base/ByteBuffer.h>
#include <ChilliSource/Core/Cryptographic/HashMD5.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/String/StringUtils.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Core/File/FileSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Main/JNI/Core/File/FileSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/File/FileSystem.h>
#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#endif

#include <md5/md5.h>

#include <algorithm>

namespace ChilliSource
{
    namespace
    {
        const std::string k_defaultPackageDLCDirectory = "DLC/";
        constexpr u32 k_maxSHA1Length = 80;
        const u32 k_md5ChunkSize = 256;
        const u32 k_sha1ChunkSize = 256;
    }
    CS_DEFINE_NAMEDTYPE(FileSystem);
    
    //-------------------------------------------------------
    //-------------------------------------------------------
    FileSystemUPtr FileSystem::Create()
    {
#ifdef CS_TARGETPLATFORM_IOS
        return FileSystemUPtr(new CSBackend::iOS::FileSystem());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
        return FileSystemUPtr(new CSBackend::Android::FileSystem());
#endif
#ifdef CS_TARGETPLATFORM_WINDOWS
        return FileSystemUPtr(new CSBackend::Windows::FileSystem());
#endif
        return nullptr;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    FileSystem::FileSystem()
        : m_packageDLCPath(k_defaultPackageDLCDirectory)
    {
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    bool FileSystem::ReadFile(StorageLocation in_storageLocation, const std::string& in_directory, std::string& out_contents) const
    {
        ITextInputStreamUPtr fileStream = CreateTextInputStream(in_storageLocation, in_directory);
        if (fileStream == nullptr)
        {
            return false;
        }
        
        out_contents = fileStream->ReadAll();
        
        return true;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    bool FileSystem::WriteFile(StorageLocation in_storageLocation, const std::string& in_directory, const std::string& in_contents) const
    {
        BinaryOutputStreamUPtr fileStream = CreateBinaryOutputStream(in_storageLocation, in_directory);
        if (fileStream.get() == nullptr)
        {
            return false;
        }
        
        fileStream->Write(reinterpret_cast<const u8*>(in_contents.c_str()), in_contents.size());
        
        return true;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    bool FileSystem::WriteFile(StorageLocation in_storageLocation, const std::string& in_directory, const s8* in_data, u32 in_dataSize) const
    {
        BinaryOutputStreamUPtr fileStream = CreateBinaryOutputStream(in_storageLocation, in_directory);
        if (fileStream.get() == nullptr)
        {
            return false;
        }
        
        fileStream->Write(reinterpret_cast<const u8*>(in_data), (s32)in_dataSize);
        
        return true;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    std::vector<std::string> FileSystem::GetFilePathsWithExtension(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, const std::string& in_extension) const
    {
        std::vector<std::string> filePaths = GetFilePaths(in_storageLocation, in_directoryPath, in_recursive);
        
        std::string extension = "." + in_extension;
        auto it = std::remove_if(filePaths.begin(), filePaths.end(), [&] (const std::string& in_path)
        {
            return (StringUtils::EndsWith(in_path, extension, true) == false);
        });
        
        filePaths.resize(it - filePaths.begin());
        
        return filePaths;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    std::vector<std::string> FileSystem::GetFilePathsWithFileName(StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive, const std::string& in_fileName) const
    {
        std::vector<std::string> filePaths = GetFilePaths(in_storageLocation, in_directoryPath, in_recursive);
        
        auto it = std::remove_if(filePaths.begin(), filePaths.end(), [&in_fileName] (const std::string& in_path)
        {
            return (StringUtils::EndsWith(in_path, in_fileName, true) == false);
        });
        
        filePaths.resize(it - filePaths.begin());
        
        return filePaths;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    void FileSystem::SetPackageDLCPath(const std::string& in_directoryPath)
    {
        std::unique_lock<std::mutex> lock(m_packageDLCPathMutex);
        m_packageDLCPath = StringUtils::StandardiseDirectoryPath(in_directoryPath);
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    const std::string& FileSystem::GetPackageDLCPath() const
    {
        std::unique_lock<std::mutex> lock(m_packageDLCPathMutex);
        return m_packageDLCPath;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    std::string FileSystem::GetFileChecksumSHA1(StorageLocation in_storageLocation, const std::string& in_filePath, const CSHA1::REPORT_TYPE in_reportType) const
    {
        auto fileStream = CreateBinaryInputStream(in_storageLocation, in_filePath);
        CS_ASSERT(fileStream, "Could not open file: " + in_filePath);

        u64 currentPosition = fileStream->GetReadPosition();
        u32 length = u32(fileStream->GetLength());
        u8 data[k_sha1ChunkSize];
        CSHA1 Hash;
        Hash.Reset();

        while(length >= k_sha1ChunkSize)
        {
            fileStream->Read(data, k_sha1ChunkSize);
            
            Hash.Update(reinterpret_cast<u8*>(data), k_sha1ChunkSize);
            length -= k_sha1ChunkSize;
        }
        
        // Last chunk
        if(length > 0)
        {
            fileStream->Read(data, length);
            Hash.Update(reinterpret_cast<u8*>(data), length);
        }
        
        fileStream->SetReadPosition(currentPosition);
        
        Hash.Final();
            
#ifdef CS_TARGETPLATFORM_WINDOWS
        TCHAR cHash[k_maxSHA1Length];
        memset(cHash, 0, k_maxSHA1Length);
        Hash.ReportHash(cHash, in_reportType);
        return CSBackend::Windows::WindowsStringUtils::UTF16ToUTF8(std::wstring(cHash));
#else
        char cHash[k_maxSHA1Length];
        memset(cHash, 0, k_maxSHA1Length);
        Hash.ReportHash(cHash, in_reportType);
        return std::string(cHash);
#endif

    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    std::string FileSystem::GetFileChecksumMD5(StorageLocation storageLocation, const std::string& filePath) const
    {
        auto fileStream = CreateBinaryInputStream(storageLocation, filePath);
        CS_ASSERT(fileStream, "Could not open file: " + filePath);
        
        u64 currentPosition = fileStream->GetReadPosition();
        u32 length = u32(fileStream->GetLength());
        u8 data[k_md5ChunkSize];
        MD5 Hash;
        
        while(length >= k_md5ChunkSize)
        {
            fileStream->Read(data, k_md5ChunkSize);
            Hash.update(data, k_md5ChunkSize);
            length -= k_md5ChunkSize;
        }
        
        // Last chunk
        if(length > 0)
        {
            fileStream->Read(data, length);
            Hash.update(data, length);
        }
        
        fileStream->SetReadPosition(currentPosition);
        
        Hash.finalize();
        return Hash.binarydigest();
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    std::string FileSystem::GetDirectoryChecksumMD5(StorageLocation in_storageLocation, const std::string& in_directoryPath) const
    {
        std::vector<std::string> filenames = GetFilePaths(in_storageLocation, in_directoryPath, true);
        
        //get a hash for each of the files.
        std::vector<std::string> hashes;
        for (const std::string& filename : filenames)
        {
            std::string fileHash = GetFileChecksumMD5(in_storageLocation, in_directoryPath + filename);
            std::string pathHash = HashMD5::GenerateBinaryHashCode(filename.c_str(), static_cast<u32>(filename.length()));
            hashes.push_back(fileHash);
            hashes.push_back(pathHash);
        }
        
        //sort the list so that ordering of
        std::sort(hashes.begin(), hashes.end());
        
        //build this into a hashable string
        std::string hashableDirectoryContents;
        for (const std::string& hash : hashes)
        {
            hashableDirectoryContents += hash;
        }
        
        //return the hash of this as the output
        std::string strOutput = 0;
        if (hashableDirectoryContents.length() > 0)
        {
            CS_ASSERT(hashableDirectoryContents.length() < static_cast<std::vector<std::string>::size_type>(std::numeric_limits<u32>::max()), "Hashable directory contents too large. It cannot exceed "
                      + ToString(std::numeric_limits<u32>::max()) + " characters.");
            
            strOutput = HashMD5::GenerateBinaryHashCode(hashableDirectoryContents.c_str(), static_cast<u32>(hashableDirectoryContents.length()));
        }
        
        return strOutput;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    u32 FileSystem::GetFileChecksumCRC32(StorageLocation storageLocation, const std::string& filePath) const
    {
        u32 output = 0;

        auto fileStream = CreateBinaryInputStream(storageLocation, filePath);
        CS_ASSERT(fileStream, "Could not open file: " + filePath);

        auto contents = fileStream->ReadAll();

        //get the hash
        output = HashCRC32::GenerateHashCode(reinterpret_cast<const s8*>(contents->GetData()), contents->GetLength());

        return output;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    u32 FileSystem::GetDirectoryChecksumCRC32(StorageLocation in_storageLocation, const std::string& in_directoryPath) const
    {
        std::vector<std::string> filenames = GetFilePaths(in_storageLocation, in_directoryPath, true);

        //get a hash for each of the files.
        std::vector<u32> hashes;
        for (const std::string& filename : filenames)
        {
            u32 fileHash = GetFileChecksumCRC32(in_storageLocation, in_directoryPath + filename);
            u32 pathHash = HashCRC32::GenerateHashCode(filename.c_str(), static_cast<u32>(filename.length()));
            hashes.push_back(fileHash);
            hashes.push_back(pathHash);
        }

        //sort the list so that ordering of
        std::sort(hashes.begin(), hashes.end());

        //build this into a hashable string
        std::string hashableDirectoryContents;
        for (const u32& hash : hashes)
        {
            hashableDirectoryContents += ToString(hash);
        }

        //return the hash of this as the output
        u32 output = 0;
        if (hashableDirectoryContents.length() > 0)
        {
            CS_ASSERT(hashableDirectoryContents.length() < static_cast<std::vector<std::string>::size_type>(std::numeric_limits<u32>::max()), "Hashable directory contents too large. It cannot exceed "
                      + ToString(std::numeric_limits<u32>::max()) + " characters.");
            
            output = HashCRC32::GenerateHashCode(hashableDirectoryContents.c_str(), static_cast<u32>(hashableDirectoryContents.length()));
        }
        return output;
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    u32 FileSystem::GetFileSize(StorageLocation storageLocation, const std::string& filePath) const
    {
        auto fileStream = CreateBinaryInputStream(storageLocation, filePath);
        CS_ASSERT(fileStream, "Could not open file: " + filePath);
        
        return fileStream->GetLength();
    }
    //--------------------------------------------------------------
    //--------------------------------------------------------------
    u32 FileSystem::GetDirectorySize(StorageLocation in_storageLocation, const std::string& in_directory) const
    {
        std::vector<std::string> filenames = GetFilePaths(in_storageLocation, in_directory, true);

        u32 totalSize = 0;
        for (const std::string& filename : filenames)
        {
            totalSize += GetFileSize(in_storageLocation, in_directory + "/" + filename);
        }

        return totalSize;
    }
    //-------------------------------------------------------
    //-------------------------------------------------------
    bool FileSystem::IsStorageLocationWritable(StorageLocation in_storageLocation) const
    {
        switch (in_storageLocation)
        {
            case StorageLocation::k_saveData:
            case StorageLocation::k_cache:
            case StorageLocation::k_DLC:
            case StorageLocation::k_root:
                return true;
            default:
                return false;
        }
    }
}
