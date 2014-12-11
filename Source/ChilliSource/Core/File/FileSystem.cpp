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

#include <ChilliSource/Core/Cryptographic/HashMD5.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/String/StringUtils.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <CSBackend/Platform/iOS/Core/File/FileSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Core/File/FileSystem.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/File/FileSystem.h>
#endif

#include <md5/md5.h>

#include <algorithm>

namespace ChilliSource
{
    namespace Core
    {
        namespace
        {
            const std::string k_defaultPackageDLCDirectory = "DLC/";
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
            Core::FileStreamUPtr fileStream = CreateFileStream(in_storageLocation, in_directory, Core::FileMode::k_read);
			
            if (fileStream.get() == nullptr || fileStream->IsOpen() == false || fileStream->IsBad() == true)
            {
                return false;
            }
            
            fileStream->GetAll(out_contents);
            fileStream->Close();
            
            return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::WriteFile(StorageLocation in_storageLocation, const std::string& in_directory, const std::string& in_contents) const
        {
            Core::FileStreamUPtr fileStream = CreateFileStream(in_storageLocation, in_directory, Core::FileMode::k_writeBinary);
			
            if (fileStream.get() == nullptr || fileStream->IsOpen() == false || fileStream->IsBad() == true)
            {
                return false;
            }
            
            fileStream->Write(in_contents);
            fileStream->Close();
            
            return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::WriteFile(StorageLocation in_storageLocation, const std::string& in_directory, const s8* in_data, u32 in_dataSize) const
        {
            Core::FileStreamUPtr fileStream = CreateFileStream(in_storageLocation, in_directory, Core::FileMode::k_writeBinary);
			
            if (fileStream.get() == nullptr || fileStream->IsOpen() == false || fileStream->IsBad() == true)
            {
                return false;
            }
            
            fileStream->Write(in_data, (s32)in_dataSize);
            fileStream->Close();
            
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
                return (Core::StringUtils::EndsWith(in_path, extension, true) == false);
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
                return (Core::StringUtils::EndsWith(in_path, in_fileName, true) == false);
            });
            
            filePaths.resize(it - filePaths.begin());
            
            return filePaths;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void FileSystem::SetPackageDLCPath(const std::string& in_directoryPath)
        {
            m_packageDLCPath = StringUtils::StandardiseDirectoryPath(in_directoryPath);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		const std::string& FileSystem::GetPackageDLCPath() const
		{
			return m_packageDLCPath;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::string FileSystem::GetFileChecksumSHA1(StorageLocation in_storageLocation, const std::string& in_filePath) const
        {
            FileStreamUPtr file = CreateFileStream(in_storageLocation, in_filePath, FileMode::k_readBinary);
            return file->GetSHA1Checksum(CSHA1::REPORT_TYPE::REPORT_HEX_SHORT);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::string FileSystem::GetFileChecksumMD5(StorageLocation in_storageLocation, const std::string& in_filePath) const
        {
            FileStreamUPtr file = CreateFileStream(in_storageLocation, in_filePath, FileMode::k_readBinary);
            return file->GetMD5Checksum();
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
                std::string pathHash = HashMD5::GenerateBinaryHashCode(filename.c_str(), filename.length());
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
				strOutput = HashMD5::GenerateBinaryHashCode(hashableDirectoryContents.c_str(), hashableDirectoryContents.length());
            }
            
			return strOutput;
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		u32 FileSystem::GetFileChecksumCRC32(StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			u32 output = 0;

			//open the file
			FileStreamUPtr file = CreateFileStream(in_storageLocation, in_filePath, FileMode::k_readBinary);
			if (file != nullptr && file->IsOpen() == true && file->IsBad() == false)
			{
				//get the length of the file
				file->SeekG(0, SeekDir::k_end);
				s32 length = file->TellG();
				file->SeekG(0, SeekDir::k_beginning);

				//read contents of file
				s8* contents = new s8[length];
				file->Read(contents, length);

				//get the hash
				output = HashCRC32::GenerateHashCode(contents, length);
				CS_SAFEDELETE_ARRAY(contents);
			}
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
				u32 pathHash = HashCRC32::GenerateHashCode(filename.c_str(), filename.length());
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
				output = HashCRC32::GenerateHashCode(hashableDirectoryContents.c_str(), hashableDirectoryContents.length());
			return output;
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		u32 FileSystem::GetFileSize(StorageLocation in_storageLocation, const std::string& in_filepath) const
		{
			//open the file
			FileStreamUPtr file = CreateFileStream(in_storageLocation, in_filepath, FileMode::k_readBinary);
			if (file->IsOpen() == true && file->IsBad() == false)
			{
				//get the length of the file
				file->SeekG(0, SeekDir::k_end);
				s32 dwLength = file->TellG();
				file->Close();
				return dwLength;
			}

			return 0;
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
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::IsWriteMode(Core::FileMode in_fileMode) const
        {
            switch (in_fileMode)
            {
                case Core::FileMode::k_write:
                case Core::FileMode::k_writeAppend:
                case Core::FileMode::k_writeAtEnd:
                case Core::FileMode::k_writeBinary:
                case Core::FileMode::k_writeBinaryAppend:
                case Core::FileMode::k_writeBinaryAtEnd:
                case Core::FileMode::k_writeBinaryTruncate:
                case Core::FileMode::k_writeTruncate:
                    return true;
                default:
                    return false;
            }
        }
    }
}
