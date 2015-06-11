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
			/// Copies a file from one location to another.
			///
			/// @author Ian Copland
			///
			/// @param The source directory.
			/// @param The destination directory.
			///
			/// @return Whether or not the file was successfully copied.
			//------------------------------------------------------------------------------
			bool CopyFile(const std::string& in_sourceFilePath, const std::string& in_destinationFilePath)
			{
				const s32 k_chunkSize = 32 * 1024;

				//open the source file
				CSCore::FileStreamSPtr sourceStream = CSCore::FileStreamSPtr(new CSCore::FileStream(in_sourceFilePath, CSCore::FileMode::k_readBinary));
				if (sourceStream->IsValid() == false)
				{
					return false;
				}

				//open the destination file
				CSCore::FileStreamSPtr destinationStream = CSCore::FileStreamSPtr(new CSCore::FileStream(in_destinationFilePath, CSCore::FileMode::k_writeBinary));
				if (destinationStream->IsValid() == false)
				{
					return false;
				}

				//find the length of the source stream
				sourceStream->SeekG(0, CSCore::SeekDir::k_end);
				s32 length = sourceStream->TellG();
				sourceStream->SeekG(0, CSCore::SeekDir::k_beginning);

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

				return true;
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
            //TODO: !?
            return false;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::CopyDirectory(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
						   CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const
		{
			//TODO: !?
			return false;
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
                    //TODO: !?
                    return std::vector<std::string>();
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
                    //TODO: !?
                    return std::vector<std::string>();
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
