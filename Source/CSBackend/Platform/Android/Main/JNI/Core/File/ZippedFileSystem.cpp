//
//  ZippedFileSystem.cpp
//  ChilliSource
//  Created by Ian Copland on 08/06/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#include <CSBackend/Platform/Android/Main/JNI/Core/File/ZippedFileSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/File/VirtualFileStream.h>

#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

#include <algorithm>

namespace CSBackend
{
    namespace Android
    {
        namespace
        {
            const u32 k_filePathLength = 32 * 1024;
        }

        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        ZippedFileSystem::ZippedFileSystem(const std::string& in_zipFilePath, const std::string& in_rootDirectoryPath)
            : m_filePath(in_zipFilePath)
        {
            BuildManifest(in_rootDirectoryPath);

            CS_LOG_VERBOSE("Manifest for zip file '" + in_zipFilePath + "' with root directory '" + in_rootDirectoryPath + "' is: ");
            for (const auto& item : m_manifestItems)
            {
                if (item.m_isFile)
                {
                    CS_LOG_VERBOSE(" File: '" + item.m_path + "'");
                }
                else
                {
                    CS_LOG_VERBOSE(" Directory: '" + item.m_path + "'");
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::IsValid() const
        {
            return m_isValid;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSCore::FileStreamUPtr ZippedFileSystem::CreateFileStream(const std::string& in_filePath, CSCore::FileMode in_fileMode) const
        {
            CS_ASSERT(IsValid() == true, "Calling into an invalid ZippedFileSystem.");

            CS_LOG_VERBOSE("Creating virtual file stream to '" + in_filePath + "' from inside zip file '" + m_filePath + "'");

            //Get the manifest item describing the location of the file within the zip.
            ManifestItem item;
            if (TryGetManifestItem(CSCore::StringUtils::StandardiseFilePath(in_filePath), item) == false)
            {
                return nullptr;
            }

            //confirm the item is a file, and not a directory.
            if (item.m_isFile == false)
            {
                return nullptr;
            }

            //read the contents of the zip file.
            std::unique_lock<std::mutex> lock(m_mutex);

            CSCore::FileStreamUPtr output;

            unzFile unzipper = unzOpen(m_filePath.c_str());
            s32 result = unzGoToFilePos(unzipper, &item.m_zipPosition);
            if (result == UNZ_OK)
            {
                result = unzOpenCurrentFile(unzipper);
                if (result == UNZ_OK)
                {
                    //get file information
                    unz_file_info info;
                    char filePath[k_filePathLength];
                    unzGetCurrentFileInfo(unzipper, &info, filePath, k_filePathLength, nullptr, 0, nullptr, 0);

                    //load the file into memory
                    std::unique_ptr<u8[]> buffer(new u8[info.uncompressed_size]);
                    unzReadCurrentFile(unzipper, (voidp)buffer.get(), info.uncompressed_size);
                    output = CSCore::FileStreamUPtr(new VirtualFileStream(std::move(buffer), info.uncompressed_size, in_fileMode));
                    if (output->IsValid() == false)
                    {
                        output = nullptr;
                    }
                }
            }

            unzCloseCurrentFile(unzipper);
            unzClose(unzipper);
            return output;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::CopyFile(const std::string& in_sourceFilePath, const std::string& in_destinationFilePath) const
        {
            CS_ASSERT(IsValid() == true, "Calling into an invalid ZippedFileSystem.");

            std::vector<std::pair<std::string, std::string>> filePaths = { std::make_pair(in_sourceFilePath, in_destinationFilePath) };
            return CopyFiles(filePaths);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::CopyFiles(const std::vector<std::pair<std::string, std::string>>& in_filePaths) const
        {
            CS_ASSERT(IsValid() == true, "Calling into an invalid ZippedFileSystem.");

            //TODO: !?
            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::DoesFileExist(const std::string& in_filePath) const
        {
            CS_ASSERT(IsValid() == true, "Calling into an invalid ZippedFileSystem.");

            ManifestItem item;
            if (TryGetManifestItem(CSCore::StringUtils::StandardiseFilePath(in_filePath), item) == true)
            {
                if (item.m_isFile == true)
                {
                    return true;
                }
            }

            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::DoesDirectoryExist(const std::string& in_directoryPath) const
        {
            CS_ASSERT(IsValid() == true, "Calling into an invalid ZippedFileSystem.");

            ManifestItem item;
            if (TryGetManifestItem(CSCore::StringUtils::StandardiseFilePath(in_directoryPath), item) == true)
            {
                if (item.m_isFile == false)
                {
                    return true;
                }
            }

            return false;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        std::vector<std::string> ZippedFileSystem::GetFilePaths(const std::string& in_directoryPath, bool in_recursive) const
        {
            CS_ASSERT(IsValid() == true, "Calling into an invalid ZippedFileSystem.");

            //TODO: !?
            return std::vector<std::string>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        std::vector<std::string> ZippedFileSystem::GetDirectoryPaths(const std::string& in_directoryPath, bool in_recursive) const
        {
            CS_ASSERT(IsValid() == true, "Calling into an invalid ZippedFileSystem.");

            //TODO: !?
            return std::vector<std::string>();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ZippedFileSystem::BuildManifest(const std::string& in_rootDirectoryPath)
        {
            CS_ASSERT(m_isValid == false, "Cannot re-build manifest.");

            CS_LOG_VERBOSE(" >> Opening zip '" + m_filePath + "' << ");

            unzFile unzipper = unzOpen(m_filePath.c_str());
            if (unzipper == nullptr)
            {
                return;
            }

            CS_LOG_VERBOSE(" >> Zip opened << ");

            char filePathBytes[k_filePathLength];
            auto rootDirectoryPath = CSCore::StringUtils::StandardiseFilePath(in_rootDirectoryPath);

            s32 status = unzGoToFirstFile(unzipper);
            while (status == UNZ_OK)
            {
                unz_file_info info;
                unzGetCurrentFileInfo(unzipper, &info, filePathBytes, k_filePathLength, nullptr, 0, nullptr, 0);

                std::string filePath = CSCore::StringUtils::StandardiseFilePath(filePathBytes);

                CS_LOG_VERBOSE(" >> Found file '" + filePath + "' << ");

                if (in_rootDirectoryPath.empty() == true || CSCore::StringUtils::StartsWith(filePath, rootDirectoryPath, false) == true)
                {
                    CS_LOG_VERBOSE(" >> Adding to manifest file << ");

                    if (rootDirectoryPath.empty() == false)
                    {
                        filePath = filePath.erase(0, rootDirectoryPath.size());
                    }

                    unz_file_pos filePos;
                    unzGetFilePos(unzipper, &filePos);
                    AddItemToManifest(filePath, filePos);
                }

                status = unzGoToNextFile(unzipper);
            }
            unzClose(unzipper);

            std::sort(m_manifestItems.begin(), m_manifestItems.end(), [](const ManifestItem& in_lhs, const ManifestItem& in_rhs)
            {
                return in_lhs.m_pathHash < in_rhs.m_pathHash;
            });

            m_isValid = true;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ZippedFileSystem::AddItemToManifest(const std::string& in_filePath, unz_file_pos in_zipPosition)
        {
            std::string filePath = CSCore::StringUtils::StandardiseFilePath(in_filePath);

            std::string directory, fileName;
            CSCore::StringUtils::SplitFilename(filePath, fileName, directory);

            std::vector<std::string> directoriesSections = CSCore::StringUtils::Split(directory, "/");
            std::string directoryPath;
            ManifestItem directoryManifestItem;
            for (const auto& directorySection : directoriesSections)
            {
                directoryPath += CSCore::StringUtils::StandardiseDirectoryPath(directorySection);
                u32 directoryHash = CSCore::HashCRC32::GenerateHashCode(directoryPath);

                //check to see if this directory has previously been seen. The usual
                auto it = std::find_if(m_manifestItems.begin(), m_manifestItems.end(), [=](const ManifestItem& in_item)
                {
                    return (in_item.m_pathHash == directoryHash && in_item.m_path == directoryPath);
                });

                if (directoryPath.size() != 0 && it != m_manifestItems.end())
                {
                    ManifestItem item;
                    item.m_path = directoryPath;
                    item.m_pathHash = directoryHash;
                    item.m_isFile = false;
                    item.m_zipPosition = in_zipPosition;
                    m_manifestItems.push_back(item);
                }
            }

			ManifestItem item;
			item.m_path = filePath;
			item.m_pathHash = CSCore::HashCRC32::GenerateHashCode(item.m_path);
			item.m_isFile = true;
			item.m_zipPosition = in_zipPosition;
			m_manifestItems.push_back(item);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ZippedFileSystem::TryGetManifestItem(const std::string& in_path, ManifestItem& out_manifestItem) const
        {
            CS_ASSERT(IsValid() == true, "Cannot get manifest item before construction finishes as the manifest item list wont have been sorted yet.");

            ManifestItem searchItem;
            searchItem.m_pathHash = CSCore::HashCRC32::GenerateHashCode(in_path);

            auto it = std::lower_bound(m_manifestItems.begin(), m_manifestItems.end(), searchItem, [](const ManifestItem& in_lhs, const ManifestItem& in_rhs)
            {
                return in_lhs.m_pathHash < in_rhs.m_pathHash;
            });

            //Avoid collisions by iterating until we find the item with both the correct hash and correct path.
            while (it != m_manifestItems.end() && it->m_pathHash == searchItem.m_pathHash)
            {
                if (it->m_path == in_path)
                {
                    out_manifestItem = *it;
                    return true;
                }

                ++it;
            }

            return false;
        }
    }
}