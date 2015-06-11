//
//  FileSystem.cpp
//  Chilli Source
//  Created by Ian Copland on 25/03/2011.
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

#ifdef CS_TARGETPLATFORM_ANDROID

#include <CSBackend/Platform/Android/Main/JNI/Core/File/FileSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Base/CoreJavaInterface.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaStaticClass.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <cstdio>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace CSBackend
{
	namespace Android 
	{
		namespace
		{
			const char k_assetsPath[] = "assets/";
			const char k_saveDataPath[] = "files/SaveData/";
			const char k_dlcPath[] = "cache/DLC/";
			const char k_cachePath[] = "cache/Cache/";
			const char k_packagePath[] = "AppResources/";
			const char k_chilliSourcePath[] = "CSResources/";

            //------------------------------------------------------------------------------
            /// Creates a new zipped file system instance. This is dependant on the current
            /// Android flavour: Google Play builds will create a file system which points
            /// to the main Apk Expansion file; Amazon builds will refer to the APK.
            ///
			/// @author Ian Copland
			///
			/// @return The zipped file system.
			//------------------------------------------------------------------------------
			ZippedFileSystemUPtr CreateZippedFileSystem()
			{
#if defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)
                JavaStaticClassDef apkExpansionInfoDef("com/chilliworks/chillisource/core/ApkExpansionInfo");
                apkExpansionInfoDef.AddMethod("getFilePath", "()Ljava/lang/String;");
                JavaStaticClass apkExpansionInfo(apkExpansionInfoDef);

                return ZippedFileSystemUPtr(new ZippedFileSystem(apkExpansionInfo.CallStringMethod("getFilePath")));
#elif defined(CS_ANDROIDFLAVOUR_AMAZON)
                auto* coreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();

                return ZippedFileSystemUPtr(new ZippedFileSystem(coreJI->GetApkDirectory(), "assets/"));
#else
                CS_LOG_FATAL("File System does not support this Android Flavour.");
                return nullptr;
#endif
			}
            //------------------------------------------------------------------------------
            /// Returns the error from errno() in string form.
            ///
			/// @author Ian Copland
			///
			/// @param The error number.
			///
			/// @return The error string.
			//------------------------------------------------------------------------------
            std::string GetFileErrorString(u32 in_errorNumber)
            {
            	switch (in_errorNumber)
            	{
            	case EACCES:
            	  	return "EACCES - Access permission error.";
            	case ELOOP:
            		return "ELOOP - Symbolic link error.";
            	case ENAMETOOLONG:
            		return "ENAMETOOLONG - The path is too long.";
            	case ENOENT:
            		return "ENOENT - Path does not exist.";
            	case ENOTDIR:
            		return "ENOTDIR - Path is not a directory.";
            	case EMFILE:
            		return "EMFILE - To many files in Program Opened Error.";
            	default:
            		return "Unknown error.";
            	}
            }
			//------------------------------------------------------------------------------
			/// Creates a new directory at the given directory path.
            ///
            /// @author Ian Copland
            ///
            /// @param The directory path.
            ///
            /// @return Whether or not the creation succeeded.
			//------------------------------------------------------------------------------
			bool CreateDirectory(const std::string& in_directoryPath)
			{
				s32 error = mkdir(in_directoryPath.c_str(), 0777);
				if (error == -1)
				{
					s32 errorType = errno;
					if (errorType != EEXIST)
					{
						CS_LOG_ERROR("File System: Error creating directory '" + in_directoryPath + "': " + GetFileErrorString(errorType));
						return false;
					}
				}

				return true;
			}
			//------------------------------------------------------------------------------
			/// returns whether or not a file exists
			///
			/// @author Ian Copland
			///
			/// @param the filepath.
			///
			/// @return Whether or not it exists.
			//------------------------------------------------------------------------------
    		bool DoesFileExist(const std::string& in_path)
    		{
    			struct stat itemStat;
    			if (stat(in_path.c_str(), &itemStat) == 0)
    			{
    				if (S_ISDIR(itemStat.st_mode) == false)
    				{
    					return true;
    				}
    			}

    			return false;
    		}
			//------------------------------------------------------------------------------
			/// returns whether or not a directory exists
			///
			/// @author Ian Copland
			///
			/// @param the directory path.
			///
			/// @return whether or not it exists.
			//------------------------------------------------------------------------------
    		bool DoesDirectoryExist(const std::string& in_path)
    		{
    			struct stat itemStats;
    			if (stat(in_path.c_str(), &itemStats) == 0)
    			{
    				if (S_ISDIR(itemStats.st_mode) == true)
    				{
    					return true;
    				}
    			}

    			return false;
    		}
			//------------------------------------------------------------------------------
			/// Deletes a directory and all its contents.
			///
			/// @author Ian Copland
			///
			/// @param The directory path.
			///
			/// @return Whether or not the directory was successfully deleted.
			//------------------------------------------------------------------------------
			bool DeleteDirectory(const std::string& in_directoryPath)
			{
				std::string directoryPath = CSCore::StringUtils::StandardiseDirectoryPath(in_directoryPath);
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
            //------------------------------------------------------------------------------
            /// Builds a list of either file or directories paths in the directory at the
            /// given path, optionally recursing into sub-directories. Whether or not this
            /// searched for files or directories is denoted by the in_searchForDirectories
            /// flag.
            ///
            /// @author Ian Copland
            ///
            /// @param out_directoryPaths - [Output] The output directory path list. This
            /// is not cleared prior to further informating being added, so care needs to
            /// be taken to ensure the list is empty prior to calling.
            /// @param in_directoryPath - The path to search for directories in.
            /// @param in_searchForDirectories - Whether we are looking for directories
            /// or files.
            /// @param in_recursive - Whether or not to recurse into sub-directories.
            /// @param in_relativeParentDirectory - [Optional] The path to be prefixed to
            /// each path found in this directory. This is mostly used for recursion into
            /// sub-directories and shouldn't otherwise be set.
            ///
            /// @return Whether or not this was successful.
            //------------------------------------------------------------------------------
			bool GetPaths(std::vector<std::string>& out_directoryPaths, const std::string& in_directoryPath, bool in_searchForDirectories,
			    bool in_recursive, const std::string& in_relativeParentDirectory = "")
			{
			    std::string directoryPath = CSCore::StringUtils::StandardiseDirectoryPath(in_relativeParentDirectory);
			    std::string relativeParentDirectory = CSCore::StringUtils::StandardiseDirectoryPath(in_relativeParentDirectory);

                DIR* directory = opendir(directoryPath.c_str());
                if(directory == nullptr)
                {
                    CS_LOG_ERROR("File System: Error getting paths in directory '" + in_directoryPath + "': " + GetFileErrorString(errno));
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
                    if (stat(itemPath.c_str(), &dirStats) != 0)
                    {
                        CS_LOG_ERROR("Error: Failed to stat path '" + itemPath + "'");
                        success = false;
                        break;
                    }

                    if (S_ISDIR(dirStats.st_mode) == true)
                    {
                        std::string relativeItemPath = relativeParentDirectory + CSCore::StringUtils::StandardiseDirectoryPath(itemName);
                        if (in_searchForDirectories == true)
                        {
                            out_directoryPaths.push_back(relativeItemPath);
                        }

                        if (in_recursive == true)
                        {
                            fullItemPath = CSCore::StringUtils::StandardiseDirectoryPath(fullItemPath);
                            if (GetDirectoryPaths(out_directoryPaths, fullItemPath, in_searchForDirectories, true, relativeItemPath) == false)
                            {
                                success = false;
                                break;
                            }
                        }
                    }
                    else if (in_searchForDirectories == false)
                    {
                        std::string relativeItemPath = relativeParentDirectory + CSCore::StringUtils::StandardiseFilePath(itemName);
                        out_directoryPaths.push_back(relativeItemPath);
                    }
                }

                closedir(directory);

                return success;
			}
		}

		CS_DEFINE_NAMEDTYPE(FileSystem);
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		FileSystem::FileSystem()
		{
			CoreJavaInterfaceSPtr coreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();

			std::string externalStorage = coreJI->GetExternalStorageDirectory();
			if (externalStorage == "")
			{
				CS_LOG_FATAL("File System: Cannot access External Storage.");
			}

			//create the base directories
			externalStorage = CSCore::StringUtils::StandardiseDirectoryPath(externalStorage);
			m_storagePath = externalStorage + "Android/data/" + coreJI->GetPackageName() + "/";

			CSBackend::Android::CreateDirectory(externalStorage + "Android/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/files/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/cache/");

			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_saveData));
			CS_ASSERT(CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_saveData)) == true, "Could not create SaveData storage location.");

			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_cache));
			CS_ASSERT(CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_cache)) == true, "Could not create Cache storage location.");

			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC));
			CS_ASSERT(CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC)) == true, "Could not create DLC storage location.");

            m_zippedFileSystem = CreateZippedFileSystem();
            if (m_zippedFileSystem->IsValid() == false)
            {
            	CS_LOG_FATAL("File system cannot read Package.");
            }
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (CSCore::FileSystem::InterfaceID == in_interfaceId || FileSystem::InterfaceID == in_interfaceId);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CSCore::FileStreamUPtr FileSystem::CreateFileStream(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, CSCore::FileMode in_fileMode) const
		{
			if (IsWriteMode(in_fileMode) == true)
			{
			    CS_ASSERT(IsStorageLocationWritable(in_storageLocation) == true, "Cannot open write mode file stream in read-only storage location.");

		        CSCore::FileStreamUPtr output(new CSCore::FileStream(GetAbsolutePathToFile(in_storageLocation, in_filePath), in_fileMode));
                if (output->IsValid() == true)
                {
                    return output;
                }
			}
			else
			{
			    if (in_storageLocation == CSCore::StorageLocation::k_package || in_storageLocation == CSCore::StorageLocation::k_chilliSource ||
			        (in_storageLocation == CSCore::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_filePath) == false))
			    {
			        //TODO: Cache DLC file existance race condition.
			        return m_zippedFileSystem->CreateFileStream(GetAbsolutePathToFile(in_storageLocation, in_filePath), in_fileMode);
			    }
			    else
			    {
			        //TODO: Cache DLC file existance race condition.
			        CSCore::FileStreamUPtr output(new CSCore::FileStream(GetAbsolutePathToFile(in_storageLocation, in_filePath), in_fileMode));
                    if (output->IsValid() == true)
                    {
                        return output;
                    }
			    }
			}

			return nullptr;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::CreateDirectoryPath(CSCore::StorageLocation in_storageLocation, const std::string& in_directory) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation) == true, "Cannot create directory in read-only storage location.");

            std::string path = GetAbsolutePathToStorageLocation(in_storageLocation);

            //get each level of the new directory seperately
            std::string relativePath = CSCore::StringUtils::StandardiseDirectoryPath(in_directory);
            std::vector<std::string> relativePathSections = CSCore::StringUtils::Split(relativePath, "/");

            //iterate through each section of the path and try and create it.
            for (const std::string& relativePathSection : relativePathSections)
            {
                path += relativePathSection + "/";

                if (CSBackend::Android::CreateDirectory(path) == false)
                {
                    return false;
                }
            }

            return true;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::CopyFile(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
								  CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
		{
		    CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation) == true, "Cannot copy a file to a read-only storage location.");

            const s32 k_chunkSize = 32 * 1024;

            auto sourceStream = CreateFileStream(in_sourceStorageLocation, in_sourceFilePath, CSCore::FileMode::k_readBinary);
            if (sourceStream == nullptr)
            {
                return false;
            }

            auto destinationStream = CreateFileStream(in_destinationStorageLocation, in_destinationFilePath, CSCore::FileMode::k_writeBinary);
            if (destinationStream == nullptr)
            {
                return false;
            }

            sourceStream->SeekG(0, CSCore::SeekDir::k_end);
            s32 length = sourceStream->TellG();
            sourceStream->SeekG(0, CSCore::SeekDir::k_beginning);

            s32 progress = 0;
            s8 buffer[k_chunkSize];
            while (progress < length)
            {
                s32 copySize = length - progress;
                if (copySize > k_chunkSize)
                {
                    copySize = k_chunkSize;
                }

                sourceStream->Read(buffer, copySize);
                destinationStream->Write(buffer, copySize);

                progress += copySize;
            }

            return true;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::CopyDirectory(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
						   CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation) == true, "Cannot copy a directory to a read-only storage location.");

            std::string sourceDirectoryPath = CSCore::StringUtils::StandardiseDirectoryPath(in_sourceDirectoryPath);
            std::string destinationDirectoryPath = CSCore::StringUtils::StandardiseDirectoryPath(in_destinationDirectoryPath);

			//create all of the required directories.
			CreateDirectoryPath(in_destinationStorageLocation, destinationDirectoryPath);
			std::vector<std::string> subDirectoryPaths = GetDirectoryPaths(in_sourceStorageLocation, in_sourceDirectoryPath);
			for (const auto& subDirectoryPath : subDirectoryPaths)
			{
			    CreateDirectoryPath(in_destinationStorageLocation, destinationDirectoryPath + subDirectoryPaths);
			}

			//Get all of the files that need to be copied.
			std::vector<std::string> filePaths = GetFilePaths(in_sourceStorageLocation, in_sourceDirectoryPath);

			//Copy each of the files to the destination.
			switch (in_sourceStorageLocation)
			{
			    case CSCore::StorageLocation::k_package:
                case CSCore::StorageLocation::k_chilliSource:
                {
                    //This could be handled by simply calling CopyFile() for each of the required
                    //files, however this is pretty inefficient when reading from the zip as it's
                    //repeatedly opened and closed. For the sake of performance we use the batch
                    //copy method when copying from the zip.

                    std::string absSourceDirectoryPath = GetAbsolutePathToStorageLocation(in_sourceStorageLocation) + sourceDirectoryPath + filePath;
                    std::string absDestinationDirectoryPath = GetAbsolutePathToStorageLocation(in_destinationStorageLocation) + destinationDirectoryPath + filePath;

                    std::unordered_map<std::string, std::string> filesToCopy;
                    for (const auto& filePath : filePaths)
                    {
                        std::unordered_map.emplace(absSourceDirectoryPath + filePath, absDestinationDirectoryPath + filePath);
                    }

                    if (m_zippedFileSystem->CopyFiles(filesToCopy) == false)
                    {
                        return false;
                    }

                    break;
                }
                case CSCore::StorageLocation::k_DLC:
                {
                    //TODO: !?
                    break;
                }
                default:
                {
                    for (const auto& filePath : filePaths)
                    {
                        if (CopyFile(in_sourceStorageLocation, sourceDirectoryPath + filePath, in_destinationStorageLocation, destinationDirectoryPath + filePath) == false)
                        {
                            return false;
                        }
                    }

                    break;
                }
			}

			return true;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DeleteFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation) == true, "Cannot delete file from read-only storage location.");

            std::string filePath = GetAbsolutePathToFile(in_storageLocation, in_filePath);
            s32 error = unlink(filePath.c_str());
            if (error != 0)
            {
                s32 errorType = errno;
                if (errorType != ENOENT)
                {
                    CS_LOG_ERROR("File System: Error deleting file '" + in_filePath + "': " + GetFileErrorString(errorType));
                    return false;
                }
            }

            return true;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DeleteDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "Cannot delete directory from read-only storage location.");

            std::string directoryPath = GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath);
            if (directoryPath != "")
            {
                CSBackend::Android::DeleteDirectory(directoryPath);
                return true;
            }

            return false;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetFilePaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
		{
			switch (in_storageLocation)
			{
			    case CSCore::StorageLocation::k_package:
			    case CSCore::StorageLocation::k_chilliSource:
			        return m_zippedFileSystem->GetFilePaths(GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath), in_recursive);
			    case CSCore::StorageLocation::k_DLC:
			        //TODO: !?
			        return std::vector<std::string>();
                default:
                    std::vector<std::string> filePaths;
                    GetPaths(filePaths, GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath), false, in_recursive);
                    return filePaths;
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const
		{
            switch (in_storageLocation)
            {
                case CSCore::StorageLocation::k_package:
                case CSCore::StorageLocation::k_chilliSource:
                    return m_zippedFileSystem->GetDirectoryPaths(GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath), in_recursive);
                case CSCore::StorageLocation::k_DLC:
                    //TODO: !?
                    return std::vector<std::string>();
                default:
                    std::vector<std::string> directoryPaths;
                    GetPaths(directoryPaths, GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath), true, in_recursive);
                    return directoryPaths;
            }
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesFileExist(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
		    if (in_storageLocation == CSCore::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_filePath) == true)
		    {
		        return true;
		    }

		    if (in_storageLocation == CSCore::StorageLocation::k_package || in_storageLocation == CSCore::StorageLocation::k_chilliSource || in_storageLocation == CSCore::StorageLocation::k_DLC)
		    {
		        //TODO: Cached DLC file existance race condition.
		        return m_zippedFileSystem->DoesFileExist(GetAbsolutePathToFile(in_storageLocation, in_filePath));
		    }

		    return CSBackend::Android::DoesFileExist(GetAbsolutePathToFile(in_storageLocation, in_filePath));
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& in_filePath) const
		{
			return CSBackend::Android::DoesFileExist(CSCore::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_filePath));
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& in_filePath) const
		{
			return DoesFileExist(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
            if (in_storageLocation == CSCore::StorageLocation::k_DLC && DoesDirectoryExistInCachedDLC(in_directoryPath) == true)
            {
                return true;
            }

            if (in_storageLocation == CSCore::StorageLocation::k_package || in_storageLocation == CSCore::StorageLocation::k_chilliSource || in_storageLocation == CSCore::StorageLocation::k_DLC)
            {
                //TODO: Cached DLC directory existance race condition.
                return m_zippedFileSystem->DoesDirectoryExist(GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath));
            }

            return CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath));
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInCachedDLC(const std::string& in_directoryPath) const
		{
			return CSBackend::Android::DoesDirectoryExist(CSCore::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_directoryPath));
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInPackageDLC(const std::string& in_directoryPath) const
		{
			return DoesDirectoryExist(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToStorageLocation(CSCore::StorageLocation in_storageLocation) const
		{
			//get the storage location path
			std::string storageLocationPath;
			switch (in_storageLocation)
			{
				case CSCore::StorageLocation::k_package:
					storageLocationPath = k_packagePath;
					break;
				case CSCore::StorageLocation::k_chilliSource:
					storageLocationPath = k_chilliSourcePath;
					break;
				case CSCore::StorageLocation::k_saveData:
					storageLocationPath = m_storagePath + k_saveDataPath;
					break;
				case CSCore::StorageLocation::k_cache:
					storageLocationPath = m_storagePath + k_cachePath;
					break;
				case CSCore::StorageLocation::k_DLC:
					storageLocationPath = m_storagePath + k_dlcPath;
					break;
				case CSCore::StorageLocation::k_root:
					storageLocationPath = "";
					break;
				default:
					CS_LOG_FATAL("File System: Requested storage location that does not exist on this platform.");
					break;
			}

			return storageLocationPath;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
            if (in_storageLocation == CSCore::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_filePath) == false)
            {
                return GetAbsolutePathToFile(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
            }

            return GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
            if (in_storageLocation == CSCore::StorageLocation::k_DLC && DoesDirectoryExistInCachedDLC(in_directoryPath) == false)
            {
                return GetAbsolutePathToDirectory(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
            }

            return GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
		}
	}
}

#endif
