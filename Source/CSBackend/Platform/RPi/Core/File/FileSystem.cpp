//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <CSBackend/Platform/RPi/Core/File/FileSystem.h>

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <unistd.h>

namespace CSBackend
{
	namespace RPi
	{
		namespace
		{
			const char k_assetsPath[] = "assets/";
			const char k_saveDataPath[] = "SaveData/";
			const char k_dlcPath[] = "DLC/";
			const char k_cachePath[] = "Cache/";
			const char k_packagePath[] = "AppResources/";
			const char k_chilliSourcePath[] = "CSResources/";

			/// Creates a new directory at the given path
			///
            /// @param directoryPath
			///		The path to the directory to create
            ///
            /// @return Whether or not the creation succeeded.
			///
			bool CreateDirectory(const std::string& directoryPath)
			{
				s32 error = mkdir(directoryPath.c_str(), 0777);
				if (error == -1)
				{
					s32 errorType = errno;
					if (errorType != EEXIST)
					{
						return false;
					}
				}

				return true;
			}

			/// Use the linux file system to copy the file from src to dest.
			///
			/// @param srcPath
			///		Absolute path to copy from
			/// @param dstPath
			///		Absolute path to copy to
			///
			/// @return TRUE if success
			///
			bool CopyFile(const std::string& srcPath, const std::string& dstPath)
			{
				int read_fd;
				int write_fd;
				struct stat stat_buf;
				off_t offset = 0;

				//Open the input file and grab its size
				read_fd = open (srcPath.c_str(), O_RDONLY);
				fstat (read_fd, &stat_buf);

				//Open the output file with the same permissions
				write_fd = open (dstPath.c_str(), O_WRONLY | O_CREAT, stat_buf.st_mode);

				//Send the bytes to the new file
				ssize_t sentBytes = sendfile (write_fd, read_fd, &offset, stat_buf.st_size);

				close (read_fd);
				close (write_fd);

				return sentBytes == stat_buf.st_size;
			}

			/// @param path
			///		The file path to check
			///
			/// @return Whether or not the file at the given path exists.
			///
    		bool DoesFileExist(const std::string& path)
    		{
    			struct stat itemStat;
    			if (stat(path.c_str(), &itemStat) == 0)
    			{
    				if (S_ISDIR(itemStat.st_mode) == false)
    				{
    					return true;
    				}
    			}

    			return false;
    		}

			/// @param path
			///		The directory path to check
			///
			/// @return Whether or not the directory at the given path exists.
			///
    		bool DoesDirectoryExist(const std::string& path)
    		{
    			struct stat itemStats;
    			if (stat(path.c_str(), &itemStats) == 0)
    			{
    				if (S_ISDIR(itemStats.st_mode) == true)
    				{
    					return true;
    				}
    			}

    			return false;
    		}

			/// Deletes a directory and all its contents.
			///
			/// @param dirPath
			///		Path of directory to delete
			///
			/// @return Whether or not the directory was successfully deleted.
			///
			bool DeleteDirectory(const std::string& dirPath)
			{
				std::string directoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(dirPath);
				if (DoesDirectoryExist(directoryPath) == false)
				{
					return false;
				}

				DIR* directory = opendir(directoryPath.c_str());
				if(directory == nullptr)
				{
					return false;
				}

				struct dirent* directoryItem;
				while ((directoryItem = readdir(directory)) != nullptr)
				{
					std::string itemName = directoryItem->d_name;
					if (itemName == "." || itemName == "..")
					{
						continue;
					}

					struct stat itemStats;
					std::string itemPath = directoryPath + itemName + "\0";
					if (stat(itemPath.c_str(), &itemStats) != 0)
					{
					    return false;
					}

                    if (S_ISDIR(itemStats.st_mode) == true)
                    {
                        if (DeleteDirectory(itemPath) == false)
                        {
                            return false;
                        }
                    }
                    else
                    {
                        unlink(itemPath.c_str());
                    }
				}
				closedir(directory);

				//remove the directory
				if (rmdir(directoryPath.c_str()) != 0)
				{
					return false;
				}

				return true;
			}

            /// Builds a list of either file or directories paths in the directory at the
            /// given path, optionally recursing into sub-directories. Whether or not this
            /// searched for files or directories is denoted by the searchForDirectories
            /// flag.
    		///
            /// @param out_directoryPaths
			///		[Output] The output directory path list. This is not cleared prior to further informating being added, so care needs to be taken to ensure the list is empty prior to calling.
            /// @param dirPath
			///		The path to search for directories in.
            /// @param searchForDirectories
			///		Whether we are looking for directories or files.
            /// @param recursive
			///		Whether or not to recurse into sub-directories.
            /// @param relativeParentDirectory
			///		[Optional] The path to be prefixed to each path found in this directory. This is mostly used for recursion into sub-directories and shouldn't otherwise be set.
            ///
            /// @return Whether or not this was successful.
            ///
			bool GetPaths(std::vector<std::string>& out_directoryPaths, const std::string& dirPath, bool searchForDirectories, bool recursive, const std::string& relativeParentDir = "")
			{
			    std::string directoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(dirPath);
			    std::string relativeParentDirectory = ChilliSource::StringUtils::StandardiseDirectoryPath(relativeParentDir);

                DIR* directory = opendir(directoryPath.c_str());
                if(directory == nullptr)
                {
                    return false;
                }

                bool success = true;
                struct dirent* item;
                while ((item = readdir(directory)) != nullptr && success == true)
                {
                    std::string itemName(item->d_name);
                    std::string fullItemPath = directoryPath + itemName;

                    if (itemName == "." || itemName == "..")
                    {
                        continue;
                    }

                    struct stat dirStats;
                    if (stat(fullItemPath.c_str(), &dirStats) != 0)
                    {
                        success = false;
                        break;
                    }

                    if (S_ISDIR(dirStats.st_mode) == true)
                    {
                        std::string relativeItemPath = relativeParentDirectory + ChilliSource::StringUtils::StandardiseDirectoryPath(itemName);
                        if (searchForDirectories == true)
                        {
                            out_directoryPaths.push_back(relativeItemPath);
                        }

                        if (recursive == true)
                        {
                            fullItemPath = ChilliSource::StringUtils::StandardiseDirectoryPath(fullItemPath);
                            if (GetPaths(out_directoryPaths, fullItemPath, searchForDirectories, true, relativeItemPath) == false)
                            {
                                success = false;
                                break;
                            }
                        }
                    }
                    else if (searchForDirectories == false)
                    {
                        std::string relativeItemPath = relativeParentDirectory + ChilliSource::StringUtils::StandardiseFilePath(itemName);
                        out_directoryPaths.push_back(relativeItemPath);
                    }
                }

                closedir(directory);

                return success;
			}

			/// @return Working directory of the application
			///
			std::string GetWorkingDirectory()
			{
				char arg1[20];
				char exepath[PATH_MAX + 1] = {0};

				sprintf(arg1, "/proc/%d/exe", getpid());
				readlink(arg1, exepath, 1024 );
				return std::string(exepath);
			}
		}

		CS_DEFINE_NAMEDTYPE(FileSystem);

		//---------------------------------------------------------------------------------
		FileSystem::FileSystem()
		{
			std::string path = GetWorkingDirectory();
			std::string::size_type pos = path.find_last_of("/");
			std::string workingDir = ChilliSource::StringUtils::StandardiseDirectoryPath(path.substr(0, pos));

			m_packagePath = workingDir + "assets/";
			m_documentsPath = workingDir + "Documents/";

			CSBackend::RPi::CreateDirectory(m_documentsPath);
			CS_ASSERT(CSBackend::RPi::DoesDirectoryExist(m_documentsPath), "Could not create Documents directory.");

			CSBackend::RPi::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_saveData));
			CS_ASSERT(CSBackend::RPi::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_saveData)), "Could not create SaveData storage location.");

			CSBackend::RPi::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_cache));
			CS_ASSERT(CSBackend::RPi::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_cache)), "Could not create Cache storage location.");

			CSBackend::RPi::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC));
			CS_ASSERT(CSBackend::RPi::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC)), "Could not create DLC storage location.");
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::IsA(ChilliSource::InterfaceIDType interfaceId) const
		{
			return (ChilliSource::FileSystem::InterfaceID == interfaceId || FileSystem::InterfaceID == interfaceId);
		}

		//---------------------------------------------------------------------------------
        ChilliSource::ITextInputStreamUPtr FileSystem::CreateTextInputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const
        {
        	std::string absFilePath = "";
			if (storageLocation == ChilliSource::StorageLocation::k_DLC && DoesFileExistInCachedDLC(filePath) == false)
			{
				absFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + filePath;
			}
			else
			{
				absFilePath = GetAbsolutePathToStorageLocation(storageLocation) + filePath;
			}

			ChilliSource::ITextInputStreamUPtr output(new ChilliSource::TextInputStream(absFilePath));
            if (output->IsValid() == true)
            {
                return output;
            }
            else
            {
                 return nullptr;
            }
        }

		//---------------------------------------------------------------------------------
		ChilliSource::IBinaryInputStreamUPtr FileSystem::CreateBinaryInputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const
		{
			std::string absFilePath = "";
			if (storageLocation == ChilliSource::StorageLocation::k_DLC && DoesFileExistInCachedDLC(filePath) == false)
			{
				absFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + filePath;
			}
			else
			{
				absFilePath = GetAbsolutePathToStorageLocation(storageLocation) + filePath;
			}

			ChilliSource::IBinaryInputStreamUPtr output(new ChilliSource::BinaryInputStream(absFilePath));
			if (output->IsValid() == true)
			{
				return output;
			}
			else
			{
			 	return nullptr;
			}
		}

		//---------------------------------------------------------------------------------
		ChilliSource::TextOutputStreamUPtr FileSystem::CreateTextOutputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath, ChilliSource::FileWriteMode fileMode) const
		{
			CS_ASSERT(IsStorageLocationWritable(storageLocation), "File System: Trying to write to read only storage location.");

			if (IsStorageLocationWritable(storageLocation))
			{
				std::string absFilePath = GetAbsolutePathToStorageLocation(storageLocation) + filePath;

				ChilliSource::TextOutputStreamUPtr output(new ChilliSource::TextOutputStream(absFilePath, fileMode));
				if (output->IsValid() == true)
				{
					return output;
				}
			}

			return nullptr;
		}

		//---------------------------------------------------------------------------------
		ChilliSource::BinaryOutputStreamUPtr FileSystem::CreateBinaryOutputStream(ChilliSource::StorageLocation storageLocation, const std::string& filePath, ChilliSource::FileWriteMode fileMode) const
		{
			CS_ASSERT(IsStorageLocationWritable(storageLocation), "File System: Trying to write to read only storage location.");

			if (IsStorageLocationWritable(storageLocation))
			{
				std::string absFilePath = GetAbsolutePathToStorageLocation(storageLocation) + filePath;

				ChilliSource::BinaryOutputStreamUPtr output(new ChilliSource::BinaryOutputStream(absFilePath, fileMode));
				if (output->IsValid() == true)
				{
					return output;
				}
			}

			return nullptr;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::CreateDirectoryPath(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(storageLocation), "File System: Trying to write to read only storage location.");

			//get each level of the new directory separately and then iterate though each creating it if it does not already exist.
			auto currentDirectoryPath = GetAbsolutePathToStorageLocation(storageLocation);
			auto relativePathSections = ChilliSource::StringUtils::Split(ChilliSource::StringUtils::StandardiseDirectoryPath(directoryPath), "/");

			for (const auto& relativePathSection : relativePathSections)
			{
				currentDirectoryPath += ChilliSource::StringUtils::StandardiseDirectoryPath(relativePathSection);
				if (!CSBackend::RPi::DoesDirectoryExist(currentDirectoryPath))
				{
					if (!CSBackend::RPi::CreateDirectory(currentDirectoryPath))
					{
						CS_LOG_ERROR("File System: Failed to create directory '" + directoryPath + "'");
						return false;
					}
				}
			}

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::CopyFile(ChilliSource::StorageLocation sourceLocation, const std::string& srcFilePath, ChilliSource::StorageLocation destLocation, const std::string& destFilePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(destLocation), "File System: Trying to write to read only storage location.");

			std::string sourceFilePath;
			if (sourceLocation == ChilliSource::StorageLocation::k_DLC && DoesFileExistInCachedDLC(srcFilePath) == false)
			{
				sourceFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + ChilliSource::StringUtils::StandardiseFilePath(srcFilePath);
			}
			else
			{
				sourceFilePath = GetAbsolutePathToStorageLocation(sourceLocation) + ChilliSource::StringUtils::StandardiseFilePath(srcFilePath);
			}

			if (CSBackend::RPi::DoesFileExist(sourceFilePath) == false)
			{
				CS_LOG_ERROR("File System: Trying to copy file '" + srcFilePath + "' but it does not exist.");
				return false;
			}

            //get the path to the file
			std::string destinationFileName, destinationDirectoryPath;
			ChilliSource::StringUtils::SplitFilename(destFilePath, destinationFileName, destinationDirectoryPath);

            //create the output directory
			CreateDirectoryPath(destLocation, destinationDirectoryPath);

            //try and copy the files
			std::string destPath = GetAbsolutePathToStorageLocation(destLocation) + destFilePath;
			if (CSBackend::RPi::CopyFile(sourceFilePath, destPath) == false)
			{
				CS_LOG_ERROR("File System: Failed to copy file '" + srcFilePath + "'");
				return false;
			}

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::CopyDirectory(ChilliSource::StorageLocation sourceLocation, const std::string& sourceDirPath, ChilliSource::StorageLocation destLocation, const std::string& destDirPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(destLocation), "File System: Trying to write to read only storage location.");

			if (DoesDirectoryExist(sourceLocation, sourceDirPath) == false)
			{
				CS_LOG_ERROR("File System: Trying to copy directory '" + sourceDirPath + "' but it doesn't exist.");
				return false;
			}

			std::vector<std::string> filePaths = GetFilePaths(sourceLocation, sourceDirPath, true);

			//if the source directory is empty, just create the equivelent directory in the destination
			if (filePaths.size() == 0)
			{
				CreateDirectoryPath(destLocation, destDirPath);
			}
			else
			{
				std::string sourceDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(sourceDirPath);
				std::string destinationDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(destDirPath);
				for (const std::string& filePath : filePaths)
				{
					if (CopyFile(sourceLocation, sourceDirectoryPath + filePath, destLocation, destinationDirectoryPath + filePath) == false)
					{
						CS_LOG_ERROR("File System: Failed to copy directory '" + sourceDirPath + "'");
						return false;
					}
				}
			}

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DeleteFile(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(storageLocation) == true, "Cannot delete file from read-only storage location.");

            std::string absFilePath = GetAbsolutePathToStorageLocation(storageLocation) + ChilliSource::StringUtils::StandardiseFilePath(filePath);
            s32 error = unlink(absFilePath.c_str());
            if (error != 0)
            {
                return false;
            }

            return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteDirectory(ChilliSource::StorageLocation storageLocation, const std::string& dirPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(storageLocation), "Cannot delete directory from read-only storage location.");

            std::string directoryPath = GetAbsolutePathToStorageLocation(storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(dirPath);
            return CSBackend::RPi::DeleteDirectory(directoryPath);

			return true;
		}

		//---------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetFilePaths(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath, bool recursive) const
		{
			std::vector<std::string> possibleDirectories = GetPossibleAbsoluteDirectoryPaths(storageLocation, directoryPath);

			std::vector<std::string> output;
			std::vector<std::string> filePaths;
			for (const std::string& possibleDirectory : possibleDirectories)
			{
				filePaths.clear();

				CSBackend::RPi::GetPaths(filePaths, possibleDirectory, false, recursive);
				output.insert(output.end(), filePaths.begin(), filePaths.end());
			}

			std::sort(output.begin(), output.end());
			std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
			output.resize(it - output.begin());
			return output;
		}

		//---------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetDirectoryPaths(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath, bool recursive) const
		{
			std::vector<std::string> possibleDirectories = GetPossibleAbsoluteDirectoryPaths(storageLocation, directoryPath);

			std::vector<std::string> output;
			std::vector<std::string> directoryPaths;
			for (const std::string& possibleDirectory : possibleDirectories)
			{
				directoryPaths.clear();

				CSBackend::RPi::GetPaths(directoryPaths, possibleDirectory, true, recursive);
				output.insert(output.end(), directoryPaths.begin(), directoryPaths.end());
			}

			std::sort(output.begin(), output.end());
			std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
			output.resize(it - output.begin());
			return output;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesFileExist(ChilliSource::StorageLocation storageLocation, const std::string& filePath) const
		{
			switch (storageLocation)
			{
				case ChilliSource::StorageLocation::k_DLC:
				{
					if (DoesItemExistInDLCCache(filePath, false) == true)
					{
						return true;
					}

					return DoesFileExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + filePath);
				}
				default:
				{
					std::string path = ChilliSource::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(storageLocation) + filePath);
					return CSBackend::RPi::DoesFileExist(path);
				}
			}

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& filePath) const
		{
			return CSBackend::RPi::DoesFileExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + ChilliSource::StringUtils::StandardiseFilePath(filePath));
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& filePath) const
		{
			return DoesFileExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + filePath);
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(ChilliSource::StorageLocation storageLocation, const std::string& directoryPath) const
		{
			switch (storageLocation)
			{
				case ChilliSource::StorageLocation::k_DLC:
				{
					if (DoesItemExistInDLCCache(directoryPath, true) == true)
					{
						return true;
					}

					return DoesDirectoryExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + directoryPath);
				}
				default:
				{
					std::string path = ChilliSource::StringUtils::StandardiseDirectoryPath(GetAbsolutePathToStorageLocation(storageLocation) + directoryPath);
					return CSBackend::RPi::DoesDirectoryExist(path);
				}
			}

			return true;
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInCachedDLC(const std::string& directoryPath) const
		{
			return CSBackend::RPi::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + ChilliSource::StringUtils::StandardiseFilePath(directoryPath));
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInPackageDLC(const std::string& directoryPath) const
		{
			return DoesDirectoryExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + directoryPath);
		}

		//---------------------------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation storageLocation) const
		{
			switch (storageLocation)
			{
			case ChilliSource::StorageLocation::k_package:
				return m_packagePath + "AppResources/";
			case ChilliSource::StorageLocation::k_chilliSource:
				return m_packagePath + "CSResources/";
			case ChilliSource::StorageLocation::k_saveData:
				return m_documentsPath + k_saveDataPath;
			case ChilliSource::StorageLocation::k_cache:
				return m_documentsPath + k_cachePath;
			case ChilliSource::StorageLocation::k_DLC:
				return m_documentsPath + k_dlcPath;
			case ChilliSource::StorageLocation::k_root:
				return "";
				break;
			default:
				CS_LOG_ERROR("Storage Location not available on this platform!");
				return "";
			}
		}

		//---------------------------------------------------------------------------------
		bool FileSystem::DoesItemExistInDLCCache(const std::string& path, bool isDirectory) const
		{
			std::string fileOrDirPath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + path;
			if (isDirectory == true)
			{
				return CSBackend::RPi::DoesDirectoryExist(ChilliSource::StringUtils::StandardiseDirectoryPath(fileOrDirPath));
			}
			else
			{
				return CSBackend::RPi::DoesFileExist(ChilliSource::StringUtils::StandardiseFilePath(fileOrDirPath));
			}
			return true;
		}

		//---------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetPossibleAbsoluteDirectoryPaths(ChilliSource::StorageLocation storageLocation, const std::string& path) const
		{
			std::vector<std::string> output;

			switch (storageLocation)
			{
				case ChilliSource::StorageLocation::k_DLC:
				{
					output.push_back(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + path);
					output.push_back(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + path);
					break;
				}
				default:
				{
					output.push_back(GetAbsolutePathToStorageLocation(storageLocation) + path);
					break;
				}
			}

			return output;
		}
	}
}

#endif
