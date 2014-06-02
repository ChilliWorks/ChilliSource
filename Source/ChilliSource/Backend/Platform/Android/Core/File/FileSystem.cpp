//
//  FileSystem.cpp
//  Chilli Source
//
//  Created by I Copland on 25/03/2011.
//  Copyright 2011 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_ANDROID

#include <ChilliSource/Backend/Platform/Android/Core/File/FileSystem.h>

#include <ChilliSource/Backend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/Core/File/FileStreamAPK.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <cstdio>
#include <dirent.h>
#include <errno.h>
#include <jni.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace ChilliSource
{
	namespace Android 
	{
		namespace
		{
			const std::string k_assetsPath = "assets/";
			const std::string k_saveDataPath = "files/SaveData/";
			const std::string k_dlcPath = "cache/DLC/";
			const std::string k_cachePath = "cache/Cache/";

            //--------------------------------------------------------------
            /// Returns the error from errno() in string form.
            ///
			/// @author I Copland
			///
			/// @param The error number.
			///
			/// @return The error string.
			//--------------------------------------------------------------
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
			//--------------------------------------------------------------
			/// Creates a new directory at the given directory path.
            ///
            /// @author I Copland
            ///
            /// @param The directory path.
            ///
            /// @return Whether or not the creation succeeded.
			//--------------------------------------------------------------
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
			//--------------------------------------------------------------
			/// returns whether or not a file exists
			///
			/// @author I Copland
			///
			/// @param the filepath.
			///
			/// @return Whether or not it exists.
			//--------------------------------------------------------------
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
			//--------------------------------------------------------------
			/// returns whether or not a directory exists
			///
			/// @author I Copland
			///
			/// @param the directory path.
			///
			/// @return whether or not it exists.
			//--------------------------------------------------------------
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
            //--------------------------------------------------------------
			/// @author I Copland
			///
			/// @param The path to check.
			///
			/// @return Whether or not the given path is to a file.
			//--------------------------------------------------------------
			bool IsFilePathString(const std::string& in_path)
			{
				return (Core::StringUtils::Match(in_path, "*.*") == true);
			}
            //--------------------------------------------------------------
			/// @author I Copland
			///
			/// @param The path to check.
			///
			/// @return Whether or not the given path is to a directory.
			//--------------------------------------------------------------
			bool IsDirectoryPathString(const std::string& in_path)
			{
				return (Core::StringUtils::Match(in_path, "*.*") == false);
			}
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The unfiltered list of file paths.
            ///
            /// @return Filtered file paths.
            //--------------------------------------------------------------
            std::vector<std::string> FilterPathsByFile(const std::vector<std::string>& in_paths)
            {
            	std::vector<std::string> output;
    			for (std::vector<std::string>::const_iterator it = in_paths.begin(); it != in_paths.end(); ++it)
    			{
    				if (IsFilePathString(*it) == true)
    				{
    					output.push_back(*it);
    				}
    			}
    			return output;
            }
            //--------------------------------------------------------------
            /// @author I Copland
            ///
            /// @param The unfiltered list of file paths.
            ///
            /// @return Filtered file paths.
            //--------------------------------------------------------------
            std::vector<std::string> FilterPathsByDirectory(const std::vector<std::string>& in_paths)
            {
            	std::vector<std::string> output;
    			for (std::vector<std::string>::const_iterator it = in_paths.begin(); it != in_paths.end(); ++it)
    			{
    				if (IsDirectoryPathString(*it) == true)
    				{
    					output.push_back(*it);
    				}
    			}
    			return output;
            }
			//--------------------------------------------------------------
			/// Sort by APK path manifest hash lowest to highest.
			///
			/// @author S Downie
			///
			/// @param The left hand side.
			/// @param The right hand side.
			///
			/// @return Whether or not the left hand side is less than the
			/// right hand side.
			//--------------------------------------------------------------
			bool APKManifestSortPredicate(const FileSystem::APKManifestItem& in_lhs, const FileSystem::APKManifestItem& in_rhs)
			{
				return in_lhs.m_pathHash < in_rhs.m_pathHash;
			}
			//--------------------------------------------------------------
			/// Copies a file from one location to another.
			///
			/// @author I Copland
			///
			/// @param The source directory.
			/// @param The destination directory.
			///
			/// @return Whether or not the file was successfully copied.
			//--------------------------------------------------------------
			bool CopyFile(const std::string& in_sourceFilePath, const std::string& in_destinationFilePath)
			{
				const s32 k_chunkSize = 250 * 1024;

				//open the source file
				Core::FileStreamSPtr sourceStream = Core::FileStreamSPtr(new Core::FileStream());
				sourceStream->Open(in_sourceFilePath, Core::FileMode::k_readBinary);
				if (sourceStream.get() == nullptr || sourceStream->IsBad() == true || sourceStream->IsOpen() == false)
				{
					return false;
				}

				//open the destination file
				Core::FileStreamSPtr destinationStream = Core::FileStreamSPtr(new Core::FileStream());
				destinationStream->Open(in_destinationFilePath, Core::FileMode::k_writeBinary);
				if (destinationStream.get() == nullptr || destinationStream->IsBad() == true || destinationStream->IsOpen() == false)
				{
					return false;
				}

				//find the length of the source stream
				sourceStream->SeekG(0, Core::SeekDir::k_end);
				s32 length = sourceStream->TellG();
				sourceStream->SeekG(0, Core::SeekDir::k_beginning);

				s32 progress = 0;
				while (progress < length)
				{
					//get the amount to copy
					s32 copySize = length - progress;
					if (copySize > k_chunkSize)
						copySize = k_chunkSize;

					//copy
					s8 byData[copySize];
					sourceStream->Read(byData, copySize);
					destinationStream->Write(byData, copySize);

					//update progress
					progress += copySize;
				}

				//cleanup
				sourceStream->Close();
				destinationStream->Close();

				return true;
			}
			//--------------------------------------------------------------
			/// Deletes a directory and all its contents.
			///
			/// @author I Copland
			///
			/// @param The directory path.
			///
			/// @return Whether or not the directory was successfully deleted.
			//--------------------------------------------------------------
			bool DeleteDirectory(const std::string& in_directoryPath)
			{
				std::string directoryPath = Core::StringUtils::StandardisePath(in_directoryPath);

				//this has the potential to have a path with a dot in it - make sure that it will always have a "/" on the end.
				if (directoryPath[directoryPath.size() - 1] != '/')
				{
					directoryPath += '/';
				}

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

					//filter out "." and ".."
					if (itemName == "." || itemName == "..")
					{
						continue;
					}

					//check to see if the item is a directory. if it is, then recurse into it. if its not, unlink it.
					struct stat itemStats;
					std::string itemPath = directoryPath + itemName + "\0";
					if (stat(itemPath.c_str(), &itemStats) == 0)
					{
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
					else
					{
						return false;
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
		}

		CS_DEFINE_NAMEDTYPE(FileSystem);
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		FileSystem::FileSystem()
		{
			CoreJavaInterfaceSPtr coreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();

			m_apkPath = coreJI->GetAPKDirectory();

			std::string externalStorage = coreJI->GetExternalStorageDirectory();
			if (externalStorage == "")
			{
				CS_LOG_FATAL("File System: Cannot access External Storage.");
			}

			//create the base directories
			externalStorage = Core::StringUtils::StandardisePath(externalStorage);
			m_storagePath = externalStorage + "Android/data/" + coreJI->GetPackageName() + "/";

			Android::CreateDirectory(externalStorage + "Android/");
			Android::CreateDirectory(externalStorage + "Android/data/");
			Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/");
			Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/files/");
			Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/cache/");

			CreateAPKManifest();

			Android::CreateDirectory(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_saveData));
			Android::CreateDirectory(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_cache));
			Android::CreateDirectory(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC));
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool FileSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (Core::FileSystem::InterfaceID == in_interfaceId || FileSystem::InterfaceID == in_interfaceId);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		Core::FileStreamUPtr FileSystem::CreateFileStream(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Core::FileMode in_fileMode) const
		{
			if (IsWriteMode(in_fileMode) == true)
			{
				CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");

				std::string filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
				Core::FileStreamUPtr fileStream = Core::FileStreamUPtr(new Core::FileStream());
				fileStream->Open(filePath, in_fileMode);
                return fileStream;
			}
			else
			{
				//if trying to read from the package or from DLC when the file is not in the cache DLC, open a APK file stream. Otherwise open a standard file stream.
				if (in_storageLocation == Core::StorageLocation::k_package)
				{
					std::string absoluteFilePath = GetAbsolutePathToFile(Core::StorageLocation::k_package, in_filePath);
					return CreateFileStreamInAPK(absoluteFilePath, in_fileMode);
				}
				else if (in_storageLocation == Core::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_filePath) == false)
				{
					std::string absoluteFilePath = GetAbsolutePathToFile(Core::StorageLocation::k_DLC, in_filePath);
					return CreateFileStreamInAPK(absoluteFilePath, in_fileMode);
				}
				else
				{
					std::string filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
					Core::FileStreamUPtr fileStream = Core::FileStreamUPtr(new Core::FileStream());
					fileStream->Open(filePath, in_fileMode);
					return fileStream;
				}
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CreateDirectoryPath(Core::StorageLocation in_storageLocation, const std::string& in_directory) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");

			std::string path = GetAbsolutePathToStorageLocation(in_storageLocation);

			//get each level of the new directory seperately
			std::string relativePath = Core::StringUtils::StandardisePath(in_directory);
			std::vector<std::string> relativePathSections = Core::StringUtils::Split(relativePath, "/");

			//iterate through each section of the path and try and create it.
			for (const std::string& relativePathSection : relativePathSections)
			{
				path += relativePathSection + "/";

				if (Android::CreateDirectory(path) == false)
				{
					return false;
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyFile(Core::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
								  Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");

			//check if we're loading from DLC, and insure that the file exists in the dlc cache. if it does not, fall back on package.
			if (in_sourceStorageLocation == Core::StorageLocation::k_package)
			{
				std::string filePath = GetAbsolutePathToFile(Core::StorageLocation::k_package, in_sourceFilePath);
				if(filePath.empty())
				{
					CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
					return false;
				}

				return CopyFileFromAPK(filePath, in_destinationStorageLocation, in_destinationFilePath);
			}
			else if (in_sourceStorageLocation == Core::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_sourceFilePath) == false)
			{
				std::string filePath = GetAbsolutePathToFile(Core::StorageLocation::k_package, in_sourceFilePath);
				if(filePath.empty())
				{
					CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
					return false;
				}

				return CopyFileFromAPK(filePath, in_destinationStorageLocation, in_destinationFilePath);
			}
			else
			{
				std::string sourceAbsolutePath = GetAbsolutePathToFile(in_sourceStorageLocation, in_sourceFilePath);
				std::string destinationAbsolutePath = GetAbsolutePathToStorageLocation(in_destinationStorageLocation) + in_destinationFilePath;

				//check the source file exists
				if(Android::DoesFileExist(sourceAbsolutePath) == false)
				{
					CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
					return false;
				}

				//get the path to the file
				std::string path, name;
				Core::StringUtils::SplitFilename(in_destinationFilePath, name, path);

				//create the output directory
				CreateDirectoryPath(in_destinationStorageLocation, path);

				//try and copy the file
				Android::CopyFile(sourceAbsolutePath, destinationAbsolutePath);

				return Android::DoesFileExist(destinationAbsolutePath);
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyDirectory(Core::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
						   Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");

            if (DoesDirectoryExist(in_sourceStorageLocation, in_sourceDirectoryPath) == false)
			{
				CS_LOG_ERROR("File System: Trying to copy directory '" + in_sourceDirectoryPath + "' but it doesn't exist.");
				return false;
			}

			std::vector<std::string> filenames = GetFilePaths(in_sourceStorageLocation, in_sourceDirectoryPath, true);

			//error if there are no files
			if (filenames.size() == 0)
			{
				CreateDirectoryPath(in_destinationStorageLocation, in_destinationDirectoryPath);
			}
			else
			{
				//copy each of these files individually
				std::string sourcePath = Core::StringUtils::StandardisePath(in_sourceDirectoryPath);
				std::string destPath = Core::StringUtils::StandardisePath(in_destinationDirectoryPath);
				for (const std::string& filename : filenames)
				{
					if (CopyFile(in_sourceStorageLocation, sourcePath + filename, in_destinationStorageLocation, destPath + filename) == false)
					{
						return false;
					}
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");

			std::string filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
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
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteDirectory(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");

			std::string directoryPath = GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath);
			if (directoryPath != "")
			{
				Android::DeleteDirectory(directoryPath);
				return true;
			}

			return false;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetFilePaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
		{
            std::vector<PathInfo> directoriesToCheck = GetPossibleAbsoluteDirectoryPaths(in_storageLocation, in_directoryPath);
            std::vector<std::string> paths = GetDirectoryContents(directoriesToCheck, in_recursive);
            std::vector<std::string> output = FilterPathsByFile(paths);
            
            std::sort(output.begin(), output.end());
            std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
            output.resize(it - output.begin());
            return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetDirectoryPaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const
		{
            std::vector<PathInfo> directoriesToCheck = GetPossibleAbsoluteDirectoryPaths(in_storageLocation, in_directoryPath);
            std::vector<std::string> paths = GetDirectoryContents(directoriesToCheck, in_recursive);
            std::vector<std::string> output = FilterPathsByDirectory(paths);
            
            std::sort(output.begin(), output.end());
            std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
            output.resize(it - output.begin());
            return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExist(Core::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
            if(in_storageLocation == Core::StorageLocation::k_package)
            {
            	const std::string* resourceDirectories = GetResourceDirectories();
                for(u32 i = 0; i < 3; ++i)
                {
                	if(DoesFileExistInAPK(resourceDirectories[i] + in_filePath) == true)
                	{
                		return true;
                	}
                }
                
                return false;
            }
            else if (in_storageLocation == Core::StorageLocation::k_DLC)
			{
				if (DoesFileExistInCachedDLC(in_filePath) == true)
				{
					return true;
				}

				return DoesFileExist(Core::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
			}
            else
            {
				return Android::DoesFileExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath));
            }
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& in_filePath) const
		{
			return Android::DoesFileExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_filePath));
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& in_filePath) const
		{
			return DoesFileExist(Core::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
            if (in_storageLocation == Core::StorageLocation::k_package)
            {
            	const std::string* resourceDirectories = GetResourceDirectories();
                for(u32 i = 0; i < 3; ++i)
                {
                    if(DoesDirectoryExistInAPK(resourceDirectories[i] + in_directoryPath) == true)
                    {
                        return true;
                    }
                }

                return false;
            }
            else if (in_storageLocation == Core::StorageLocation::k_DLC)
			{
				if (Android::DoesDirectoryExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_directoryPath)) == true)
				{
					return true;
				}

				return DoesDirectoryExist(Core::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
			}
            else
            {
            	return Android::DoesDirectoryExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath));
            }
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToStorageLocation(Core::StorageLocation in_storageLocation) const
		{
			//get the storage location path
			std::string storageLocationPath;
			switch (in_storageLocation)
			{
				case Core::StorageLocation::k_package:
					storageLocationPath = "";
					break;
				case Core::StorageLocation::k_saveData:
					storageLocationPath = m_storagePath + k_saveDataPath;
					break;
				case Core::StorageLocation::k_cache:
					storageLocationPath = m_storagePath + k_cachePath;
					break;
				case Core::StorageLocation::k_DLC:
					storageLocationPath = m_storagePath + k_dlcPath;
					break;
				case Core::StorageLocation::k_root:
					storageLocationPath = "";
					break;
				default:
					CS_LOG_FATAL("File System: Requested storage location that does not exist on this platform.");
					break;
			}

			return storageLocationPath;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			if (DoesFileExist(in_storageLocation, in_filePath) == true)
			{
				switch (in_storageLocation)
				{
					case Core::StorageLocation::k_package:
					{
						std::string filePath;
						for(u32 i = 0; i < 3; ++i)
						{
							const std::string* resourceDirectories = GetResourceDirectories();
							filePath = Core::StringUtils::StandardisePath(resourceDirectories[i] + in_filePath);
							if(DoesFileExistInAPK(filePath) == true)
							{
								break;
							}
						}

						return filePath;
					}
					case Core::StorageLocation::k_DLC:
					{
						std::string filePath = Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_filePath);
						if(Android::DoesFileExist(filePath) == true)
						{
							return filePath;
						}

						return GetAbsolutePathToFile(Core::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
					}
					default:
					{
						return GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
					}
				}
			}

			return "";
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToDirectory(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			if (DoesDirectoryExist(in_storageLocation, in_directoryPath) == true)
			{
				switch (in_storageLocation)
				{
					case Core::StorageLocation::k_package:
					{
						std::string directoryPath;
						for(u32 i = 0; i < 3; ++i)
						{
							const std::string* resourceDirectories = GetResourceDirectories();
							directoryPath = Core::StringUtils::StandardisePath(resourceDirectories[i] + in_directoryPath);
							if(DoesDirectoryExistInAPK(directoryPath) == true)
							{
								break;
							}
						}

						return directoryPath;
					}
					case Core::StorageLocation::k_DLC:
					{
						std::string directoryPath = Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_directoryPath);
						if(Android::DoesDirectoryExist(directoryPath) == true)
						{
							return directoryPath;
						}

						return GetAbsolutePathToDirectory(Core::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
					}
					default:
					{
						return GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
					}
				}
			}

			return "";
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void FileSystem::CreateAPKManifest()
        {
        	std::unique_lock<std::mutex> lock(m_minizipMutex);

			unzFile unzipper = unzOpen(m_apkPath.c_str());
			if (unzipper == nullptr)
			{
				CS_LOG_FATAL("File System: Failed to open APK.");
			}

			s32 status = unzGoToFirstFile(unzipper);
			while (status == UNZ_OK)
			{
				const u32 k_filePathLength = 1024;

				char filePathBytes[k_filePathLength];
				unz_file_info info;
				unzGetCurrentFileInfo(unzipper, &info, filePathBytes, k_filePathLength, nullptr, 0, nullptr, 0);

				//get the path and filename
				std::string filePath = Core::StringUtils::StandardisePath(filePathBytes);

				//if this file is at the same path as requested, then add it to the output
				if (Core::StringUtils::StartsWith(filePath, k_assetsPath, false) == true)
				{
					filePath = filePath.erase(0, k_assetsPath.size());
					std::string directoryPath = Core::StringUtils::StandardisePath(filePath.substr(0, filePath.rfind("/") + 1));

					//check to see if this directory has previously been seen. If it has not, add it.
					if (directoryPath.size() != 0 && DoesDirectoryExistInAPK(directoryPath) == false)
					{
						APKManifestItem item;
						item.m_path = directoryPath;
						item.m_pathHash = Core::HashCRC32::GenerateHashCode(item.m_path);
						item.m_isFile = false;
						unzGetFilePos(unzipper, &(item.m_apkPosition));
						m_apkManifestItems.push_back(item);
					}

					APKManifestItem item;
					item.m_path = filePath;
					item.m_pathHash = Core::HashCRC32::GenerateHashCode(item.m_path);
					item.m_isFile = true;
					unzGetFilePos(unzipper, &(item.m_apkPosition));
					m_apkManifestItems.push_back(item);
				}
				status = unzGoToNextFile(unzipper);
			}
			unzClose(unzipper);

			std::sort(m_apkManifestItems.begin(), m_apkManifestItems.end(), APKManifestSortPredicate);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::TryGetManifestItem(const std::string& in_path, APKManifestItem& out_manifestItem) const
        {
			APKManifestItem searchItem;
			searchItem.m_pathHash = Core::HashCRC32::GenerateHashCode(Core::StringUtils::StandardisePath(in_path));

			auto it = std::lower_bound(m_apkManifestItems.begin(), m_apkManifestItems.end(), searchItem, APKManifestSortPredicate);
			if(it !=  m_apkManifestItems.end() && it->m_pathHash == searchItem.m_pathHash)
			{
				out_manifestItem = *it;
				return true;
			}

			return false;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		Core::FileStreamUPtr FileSystem::CreateFileStreamInAPK(const std::string& in_filePath, Core::FileMode in_fileMode) const
		{
			Core::FileStreamUPtr fileStream = Core::FileStreamUPtr(new FileStreamAPK(&m_minizipMutex));

			APKManifestItem manifestItem;
			if (TryGetManifestItem(in_filePath, manifestItem) == true)
			{
				if (manifestItem.m_isFile == true)
				{
					FileStreamAPK* fileStreamAPK = static_cast<FileStreamAPK*>(fileStream.get());
					fileStreamAPK->OpenFromAPK(m_apkPath, manifestItem.m_apkPosition, in_fileMode);
				}
			}

			return fileStream;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyFileFromAPK(const std::string& in_filePath, Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
		{
			std::unique_lock<std::mutex> lock(m_minizipMutex);

			bool isSuccess = false;

			APKManifestItem manifestItem;
			if (TryGetManifestItem(in_filePath, manifestItem) == true)
			{
				if (manifestItem.m_isFile == true)
				{
					unzFile unzip = unzOpen(m_apkPath.c_str());
					if (unzip != nullptr)
					{
						if (unzGoToFilePos(unzip, &manifestItem.m_apkPosition) == UNZ_OK)
						{
							if (unzOpenCurrentFile(unzip) == UNZ_OK)
							{
								const u32 k_filePathLength = 1024;

								unz_file_info info;
								char filePathBytes[k_filePathLength];
								unzGetCurrentFileInfo(unzip, &info, filePathBytes, k_filePathLength, nullptr, 0, nullptr, 0);

								std::string outputDirectoryPath, outputFileName;
								Core::StringUtils::SplitFilename(in_destinationFilePath, outputFileName, outputDirectoryPath);
								if (CreateDirectoryPath(in_destinationStorageLocation, outputDirectoryPath) == true)
								{
									char* dataBuffer = new char[info.uncompressed_size];
									unzReadCurrentFile(unzip, (voidp)dataBuffer, info.uncompressed_size);

									if (WriteFile(in_destinationStorageLocation, in_destinationFilePath, dataBuffer, info.uncompressed_size) == true)
									{
										isSuccess = true;
									}

									CS_SAFEDELETE_ARRAY(dataBuffer);
								}

								//close file
								unzCloseCurrentFile(unzip);
							}
						}
					}
					unzClose(unzip);
				}
			}

			if (isSuccess == false)
			{
				CS_LOG_ERROR("File System: Failed to copy file '" + in_filePath + "' from APK.");
			}

			return isSuccess;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetPathsInAPK(const std::string& in_directoryPath, bool in_recursive) const
		{
			std::vector<std::string> output;

			std::string directoryPath = Core::StringUtils::StandardisePath(in_directoryPath);
			for (const APKManifestItem& item : m_apkManifestItems)
			{
				std::string itemFileName;
				std::string itemDirectoryPath;
				Core::StringUtils::SplitFilename(item.m_path, itemFileName, itemDirectoryPath);

				if ((in_recursive == true && Core::StringUtils::Match(item.m_path, directoryPath + "*") == true) || (in_recursive == false && directoryPath == itemDirectoryPath))
				{
					output.push_back(item.m_path.substr(directoryPath.length()));
				}
			}

			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInAPK(const std::string& in_filePath) const
		{
			APKManifestItem manifestItem;
			if (TryGetManifestItem(in_filePath, manifestItem) == true)
			{
				if (manifestItem.m_isFile == true)
				{
					return true;
				}
			}

			return false;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInAPK(const std::string& in_directoryPath) const
		{
			APKManifestItem manifestItem;
			if (TryGetManifestItem(in_directoryPath, manifestItem) == true)
			{
				if (manifestItem.m_isFile == false)
				{
					return true;
				}
			}

			return false;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetAllPaths(const std::string& in_directoryPath, bool in_recursive, const std::string& in_parentDirectoryPath) const
		{
			std::vector<std::string> output;

			//insure the path is in the correct format
			std::string directoryPath = Core::StringUtils::StandardisePath(in_directoryPath);
			std::string parentDirectoryPath = Core::StringUtils::StandardisePath(in_parentDirectoryPath);

			//these have the potential to be paths with a dot in them so make sure that it will always have a "/" on the end regardless.
			if (directoryPath.size() > 0 && directoryPath[directoryPath.size() - 1] != '/')
			{
				directoryPath += '/';
			}
			if (parentDirectoryPath.size() > 0 && parentDirectoryPath[parentDirectoryPath.size() - 1] != '/')
			{
				parentDirectoryPath += '/';
			}

			//check the directory exists
			if (Android::DoesDirectoryExist(directoryPath) == false)
			{
				CS_LOG_ERROR("File System: Directory path doesn't exist '" + directoryPath + "'");
				return output;
			}

			//open up the directory
			DIR* directory = opendir(directoryPath.c_str());
			if(directory == nullptr)
			{
				CS_LOG_ERROR("File System: Error getting paths in directory '" + in_directoryPath + "': " + GetFileErrorString(errno));
				return output;
			}

			//iterate through the directory
			struct dirent* directoryItem;
			while ((directoryItem = readdir(directory)) != nullptr)
			{
				std::string itemName = Core::StringUtils::StandardisePath(directoryItem->d_name);

				//filter out "." and ".."
				if (itemName == "." || itemName == "..")
					continue;

				//add the item to our list
				output.push_back(parentDirectoryPath + itemName);

				//check if this item is a directory, if so, recurse!
				struct stat itemStats;
				std::string itemPath = directoryPath + itemName + "\0";
				if (stat(itemPath.c_str(), &itemStats) == 0)
				{
					if (S_ISDIR(itemStats.st_mode) == true)
					{
						std::vector<std::string> subDirectoryPaths = GetAllPaths(itemPath, in_recursive, parentDirectoryPath + itemName);
						output.insert(output.end(), subDirectoryPaths.begin(), subDirectoryPaths.end());
					}
				}
				else
				{
					CS_LOG_ERROR("Error: Failed to stat path '" + itemPath + "'");
				}

			}

			closedir(directory);

			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<FileSystem::PathInfo> FileSystem::GetPossibleAbsoluteDirectoryPaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			std::vector<PathInfo> output;
			switch(in_storageLocation)
			{
				case Core::StorageLocation::k_package:
				{
					const std::string* resourceDirectories = GetResourceDirectories();
					for(u32 i = 0; i < 3; ++i)
					{
						PathInfo info;
						info.m_path = Core::StringUtils::StandardisePath(resourceDirectories[i] + in_directoryPath);
						info.m_storageLocation = Core::StorageLocation::k_package;
						output.push_back(info);
					}
					break;
				}
				case Core::StorageLocation::k_DLC:
				{
					const std::string* resourceDirectories = GetResourceDirectories();
					for(u32 i = 0; i < 3; ++i)
					{
						PathInfo info;
						info.m_path = Core::StringUtils::StandardisePath(resourceDirectories[i] + GetPackageDLCPath() + in_directoryPath);
						info.m_storageLocation = Core::StorageLocation::k_package;
						output.push_back(info);
					}

					PathInfo info;
					info.m_path = GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_directoryPath;
					info.m_storageLocation = Core::StorageLocation::k_DLC;
					output.push_back(info);
					break;
				}
				default:
				{
					PathInfo info;
					info.m_path = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
					info.m_storageLocation = in_storageLocation;
					output.push_back(info);
					break;
				}
			}
			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetDirectoryContents(const std::vector<PathInfo>& in_directoryInfos, bool in_recursive) const
		{
			std::vector<std::string> output;
            for(const PathInfo& directoryInfo : in_directoryInfos)
            {
                std::string path = ChilliSource::Core::StringUtils::StandardisePath(directoryInfo.m_path);

                if(directoryInfo.m_storageLocation == Core::StorageLocation::k_package)
                {
                	std::vector<std::string> paths = GetPathsInAPK(path, in_recursive);
                	output.insert(output.end(), paths.begin(), paths.end());
                }
                else
                {
                	std::vector<std::string> paths = GetAllPaths(path, in_recursive);
                	output.insert(output.end(), paths.begin(), paths.end());
                }
            }
            return output;
		}
	}
}

#endif