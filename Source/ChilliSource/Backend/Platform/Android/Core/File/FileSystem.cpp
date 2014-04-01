//
//  FileSystem.cpp
//  Chilli Source
//
//  Created by I Copland on 25/03/2011.
//  Copyright 2011 Tag Games Ltd. All rights reserved.
//

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
            /// @author I Copland
            ///
            /// @return whether or not the given file mode is a write mode
            //--------------------------------------------------------------
            bool IsWriteMode(Core::FileMode in_fileMode)
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
                    case Core::FileMode::k_read:
                    case Core::FileMode::k_readBinary:
                        return false;

                }
            }
            //--------------------------------------------------------------
			/// @author I Copland
			///
			/// @param The path to check.
			///
			/// @return Whether or not the given path is to a file.
			//--------------------------------------------------------------
			bool IsFilePath(const std::string& in_path)
			{
				return (Core::StringUtils::Match((*it), "*.*") == true);
			}
            //--------------------------------------------------------------
			/// @author I Copland
			///
			/// @param The path to check.
			///
			/// @return Whether or not the given path is to a directory.
			//--------------------------------------------------------------
			bool IsDirectoryPath(const std::string& in_path)
			{
				return (Core::StringUtils::Match((*it), "*.*") == false);
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
    				if (IsFilePath(*it) == true)
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
    				if (IsDirectoryPath(*it) == false)
    				{
    					out_paths.push_back(*it);
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
				return in_lhs.m_pathHash < inRHS.m_pathHash;
			}
		}

		CS_DEFINE_NAMEDTYPE(FileSystem);
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		FileSystem::FileSystem()
		{
			CoreJavaInterfaceSPtr pCoreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();

			//get the path to the bundle dir
			mstrPathToAPK = pCoreJI->GetAPKDirectory();

			//get the storage directory
			std::string externalStorage = pCoreJI->GetExternalStorageDirectory();
			if (externalStorage == "")
			{
				CS_LOG_FATAL("File System: Cannot access External Storage.");
			}

			std::string packageName = pCoreJI->GetPackageName();
			mstrStorageDir = Core::StringUtils::StandardisePath(externalStorage + "/Android/data/" + packageName + "/");

			//Create the directories
			CreateBaseDirectory(GetStorageLocationDirectory(Core::StorageLocation::k_saveData));
			CreateBaseDirectory(GetStorageLocationDirectory(Core::StorageLocation::k_cache));
			CreateBaseDirectory(GetStorageLocationDirectory(Core::StorageLocation::k_DLC));

			CreateHashedAPKFileList();
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
			//if this is not a read stream, insure that the storage location is writable.
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
					return CreateAPKFileStream(in_filePath, in_fileMode);
				}
				else if (ineStorageLocation == Core::StorageLocation::k_DLC && DoesFileExistInCachedDLC(in_filePath) == false)
				{
					return CreateAPKFileStream(GetPackageDLCPath() + in_filePath, in_fileMode);
				}
				else
				{
					std::string filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
					Core::FileStreamUPtr fileStream = Core::FileStreamUPtr(new Core::FileStream());
					newFilestream->Open(filePath, in_fileMode);
					return newFilestream;
				}
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CreateDirectory(Core::StorageLocation in_storageLocation, const std::string& in_directory) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");

			//create the directory
			std::string path = GetAbsolutePathToStorageLocation(in_storageLocation);

			//get each level of the new directory seperately
			std::string relativePath = Core::StringUtils::StandardisePath(in_directory);
			std::vector<std::string> relativePathSections = Core::StringUtils::Split(relativePath, "/");

			//iterate through each section of the path and try and create it.
			for (const std::string& relativePathSection : relativePathSections)
			{
				path += relativePathSection + "/";

				s32 error = mkdir(path.c_str(), 0777);
				if (error == -1)
				{
					s32 errorType = errno;
					if (errorType != EEXIST)
					{
						CS_LOG_ERROR("File System: Error creating directory '" + path + "'");
						return false;
					}
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

			//check if we're loading from DLC, and insure that the file exists in the dlc cache. if it does not, fall back on package
			if (in_sourceStorageLocation == Core::StorageLocation::k_package)
			{
				std::string filePath = GetAbsolutePathForFile(Core::StorageLocation::k_package, in_sourceFilePath);
				if(filePath.empty())
				{
					CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
					return false;
				}

				return CopyFileFromAPK(filePath, in_destinationStorageLocation, in_destinationFilePath);
			}
			else if(in_sourceStorageLocation == Core::StorageLocation::k_DLC && DoesFileExistInCachedDLC(instrSourceFilepath) == false)
			{
				std::string filePath = GetAbsolutePathForFile(Core::StorageLocation::k_package, in_sourceFilePath);
				if(filePath.empty())
				{
					CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
					return false;
				}

				return CopyFileFromAPK(filePath, in_destinationStorageLocation, in_destinationFilePath);
			}
			else
			{
				std::string sourceAbsolutePath = GetAbsolutePathForFile(in_sourceStorageLocation, in_sourceFilePath);
				std::string destinationAbsolutePath = GetAbsolutePathForFile(in_destinationStorageLocation, in_destinationFilePath);

				//check the source file exists
				if(DoesFileExist(sourceAbsolutePath) == false)
				{
					CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
					return false;
				}

				//get the path to the file
				std::string strPath, strName;
				ChilliSource::Core::StringUtils::SplitFilename(in_destinationFilePath, strName, strPath);

				//create the output directory
				CreateDirectory(in_destinationStorageLocation, strPath);

				//try and copy the file
				CopyFile(sourceAbsolutePath, destinationAbsolutePath);

				return DoesFileExist(destinationAbsolutePath);
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyDirectory(Core::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
						   Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");

			//get all the files in the directory
			std::vector<std::string> astrFilenames;
			GetFilePaths(in_sourceStorageLocation, in_sourceDirectoryPath, true, astrFilenames);

			//error if there are no files
			if (astrFilenames.size() == 0)
			{
				CS_LOG_ERROR("File System: Trying to copy directory '" + in_sourceDirectoryPath + "' but it is empty or doesn't exist.");
				return false;
			}

			//copy each of these files individually
			std::string strSourceProperPath = Core::StringUtils::StandardisePath(in_sourceDirectoryPath);
			std::string strDestProperPath = Core::StringUtils::StandardisePath(in_destinationDirectoryPath);
			for (std::vector<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
			{
				if (CopyFile(in_sourceStorageLocation, strSourceProperPath + *it, in_destinationStorageLocation, strDestProperPath + *it) == false)
				{
					return false;
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");

			std::string strPath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
			s32 error = unlink(strPath.c_str());
			if (error != 0)
			{
				s32 errorType = errno;
				if (errorType != ENOENT)
				{
					CS_LOG_ERROR("File System: Error deleting file '" + in_filePath + "'");
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

			std::string strPath = GetAbsolutePathForFile(in_storageLocation, in_directoryPath);
			DeleteDirectory(strPath);
			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetFilePaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
		{
            std::vector<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(in_storageLocation, in_directoryPath, aDirectoriesToCheck);
            
            std::vector<std::string> paths = GetDirectoryContents(aDirectoriesToCheck, in_recursive);
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
            std::vector<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);

            std::vector<std::string> paths = GetDirectoryContents(aDirectoriesToCheck, in_recursive);
            std::vector<std::string> output = FilterFileNamesByDirectory(paths);
            
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
                for(u32 i=0; i<3; ++i)
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
				return DoesFileExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath));
            }
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& in_filePath) const
		{
			return DoesFileExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_filePath));
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& in_filePath) const
		{
			const std::string* resourceDirectories = GetResourceDirectories();
            for (u32 i = 0; i < 3; ++i)
            {
            	if(DoesFileExistInAPK(resourceDirectories[i] + GetPackageDLCPath() + in_filePath) == true)
            	{
            		return true;
            	}
            }
			return false;
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
                    if(DoesDirectoryExistInAPK(resourceDirectories[i] + "/" + in_directoryPath))
                    {
                        return true;
                    }
                }
                
                return false;
            }
            else if (in_storageLocation == Core::StorageLocation::k_DLC)
			{
				if (DoesDirectoryExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_directoryPath)) == true)
				{
					return true;
				}

				return DoesDirectoryExist(Core::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
			}
            else
            {
            	return DoesDirectoryExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(ineStorageLocation) + in_directoryPath));
            }
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToStorageLocation(Core::StorageLocation in_storageLocation) const
		{
			//get the storage location path
			std::string strStorageLocationPath;
			switch (in_storageLocation)
			{
				case Core::StorageLocation::k_package:
					strStorageLocationPath = k_assetsPath;
					break;
				case Core::StorageLocation::k_saveData:
					strStorageLocationPath = m_storagePath + k_saveDataPath;
					break;
				case Core::StorageLocation::k_cache:
					strStorageLocationPath = m_storagePath + k_cachePath;
					break;
				case Core::StorageLocation::k_DLC:
					strStorageLocationPath = m_storagePath + k_dlcPath;
					break;
				case Core::StorageLocation::k_root:
					strStorageLocationPath = "";
					break;
				default:
					CS_LOG_FATAL("File System: Requested storage location that does not exist on this platform.");
					break;
			}

			return strStorageLocationPath;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string GetAbsolutePathToFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			switch (in_storageLocation)
			{
				case Core::StorageLocation::k_package:
				{
					std::string filePath = GetAbsolutePathToStorageLocation(Core::StorageLocation::k_package) + in_filePath;

					for(u32 i = 0; i < 3; ++i)
					{
						const std::string* resourceDirectories = GetResourceDirectories();
						std::string strPath = Core::StringUtils::StandardisePath(resourceDirectories[i] + in_filePath);
						if(DoesFileExistInHashedStore(strPath))
						{
							filePath = GetAbsolutePathToStorageLocation(Core::StorageLocation::k_package) + strPath;
							break;
						}
					}

					return filePath;
				}
				case Core::StorageLocation::k_DLC:
				{
					std::string filePath = Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_filePath);
					if(iOS::DoesFileExist(filePath) == true)
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
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void FileSystem::CreateAPKManifest()
        {
        	std::unique_lock<std::mutex> lock(m_minizipMutex);

			unzFile unzipper = unzOpen(m_pathToAPK.c_str());
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
				if (Core::StringUtils::Match(filePath, k_assetsPath + "*") == true)
				{
					filePath = filePath.erase(0, k_assetsPath.size());
					std::string directoryPath = filePath.substr(0, filePath.rfind("/") + 1);

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
			if(it !=  mAPKFileInfo.end() && it->m_pathHash == searchItem.m_pathHash)
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
					FileStreamAPKSPtr fileStreamAPK = static_cast<FileStreamAPK*>(fileStream.get());
					fileStreamAPK->OpenFromAPK(m_pathToAPK, manifestItem.m_apkPosition, in_fileMode);
				}
			}

			return fileStream;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyFileFromAPK(const std::string& in_filePath, Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
		{
			std::unique_lock<std::mutex> lock(m_minizipMutex);

			APKManifestItem manifestItem;
			if (TryGetManifestItem(in_filePath, manifestItem) == true)
			{
				if (manifestItem.m_isFile == true)
				{
					unzFile unzip = unzOpen(m_pathToAPK.c_str());

					bool bSuccess = false;
					if (unzip != nullptr)
					{
						if (unzGoToFilePos(unzip, manifestItem.m_apkPosition) == UNZ_OK)
						{
							if (unzOpenCurrentFile(unzip) == UNZ_OK)
							{
								unz_file_info info;
								const u32 dwFilePathLength = 256;
								char cFilePath[dwFilePathLength];
								unzGetCurrentFileInfo(unzip, &info, cFilePath, dwFilenameLength, nullptr, 0, nullptr, 0);
								std::string strFilePath = ChilliSource::Core::StringUtils::StandardisePath(std::string(cFilePath));

								//confirm this file is indeed the file we're looking for
								if (Core::StringUtils::Match(strFilePath, manifestItem.m_path))
								{
									std::string strOutputPath, strOutputFilename;
									Core::StringUtils::SplitFilename(in_destinationFilePath, strOutputFilename, strOutputPath);
									if (CreateDirectory(in_destinationStorageLocation, strOutputPath) == true)
									{
										char* dataBuffer = new char[info.uncompressed_size];
										unzReadCurrentFile(unzip, (voidp)dataBuffer, info.uncompressed_size);

										if (WriteFile(in_destinationStorageLocation, in_destinationFilePath, dataBuffer, info.uncompressed_size) == true)
										{
											bSuccess = true;
										}

										CS_SAFEDELETE_ARRAY(dataBuffer);
									}
								}

								//close file
								unzCloseCurrentFile(unzip);
							}
						}
					}
					unzClose(unzip);
				}
			}

			if (bSuccess == false)
			{
				CS_LOG_ERROR("File System: Failed to copy file '" + in_sourceFilePath + "' from APK.");
			}

			return bSuccess;
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
				Core::CStringUtils::SplitFilename(item.m_path, itemFileName, itemDirectoryPath);

				if ((in_recursive == true && Core::StringUtils::Match(item.m_path, directoryPath + "*") == true) || (in_recursive == false && directoryPath == itemDirectoryPath))
				{
					output.push_back(item.m_path);
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
			if (TryGetManifestItem(in_filePath, manifestItem) == true)
			{
				if (manifestItem.m_isFile == true)
				{
					return false;
				}
			}

			return false;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CreateBaseDirectory(const std::string& instrDirectory) const
		{
			std::string correctedPath = Core::StringUtils::StandardisePath(instrDirectory);
			std::vector<std::string> pathSections = Core::StringUtils::Split(correctedPath, "/");

			//iterate through each section of the path and try and create it.
			std::string path = "";
			for (std::vector<std::string>::iterator it = pathSections.begin(); it != pathSections.end(); ++it)
			{
				path += (*it) + "/";

				s32 error = mkdir(path.c_str(),0777);
				if (error != 0)
				{
					s32 errorType = errno;
					if (errorType != EEXIST)
					{
						CS_LOG_ERROR("Error creating directory: " + path);
						return false;
					}
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyFile(const std::string& instrSourceFilepath, const std::string& instrDestinationFilepath) const
		{
			const s32 kdwChunkSize = 250 * 1024; //250Kb chunk size

			//open the source file
			Core::FileStreamSPtr sourceStream = Core::FileStreamSPtr(new Core::FileStream());
			sourceStream->Open(instrSourceFilepath, Core::FileMode::k_readBinary);
			if (sourceStream.get() == nullptr || sourceStream->IsBad() == true || sourceStream->IsOpen() == false)
				return false;

			//open the source file
			Core::FileStreamSPtr destinationStream = Core::FileStreamSPtr(new Core::FileStream());
			destinationStream->Open(instrDestinationFilepath, Core::FileMode::k_writeBinary);
			if (destinationStream.get() == nullptr || destinationStream->IsBad() == true || destinationStream->IsOpen() == false)
				return false;

			//find the length of the source stream
			sourceStream->SeekG(0, Core::SeekDir::k_end);
			s32 dwLength = sourceStream->TellG();
			sourceStream->SeekG(0, Core::SeekDir::k_beginning);

			//iterate through the source file copying to the dest file until there's nothing left to copy
			s32 dwProgress = 0;
			while (dwProgress < dwLength)
			{
				//get the amount to copy
				s32 dwCopySize = dwLength - dwProgress;
				if (dwCopySize > kdwChunkSize)
					dwCopySize = kdwChunkSize;

				//copy
				s8 byData[dwCopySize];
				sourceStream->Read(byData, dwCopySize);
				destinationStream->Write(byData, dwCopySize);

				//update progress
				dwProgress += dwCopySize;
			}

			//cleanup
			sourceStream->Close();
			destinationStream->Close();

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteDirectory(const std::string& instrDirectory) const
		{
			//insure the path is in the correct format
			std::string strCorrectedDirectory = Core::StringUtils::StandardisePath(instrDirectory);

			//this has the potential to have a path with a dot in it - make sure that it will always have a "/" on the end.
			if (strCorrectedDirectory[strCorrectedDirectory.size() - 1] != '/')
				strCorrectedDirectory += '/';

			//check the directory exists
			if (DoesDirectoryExist(strCorrectedDirectory) == false)
			{
				return false;
			}

			//open up the directory
			DIR* directory = opendir(strCorrectedDirectory.c_str());
			if(directory == nullptr)
			{
				return false;
			}

			//iterate through the directory
			struct dirent* directoryItem;
			while ((directoryItem = readdir(directory)) != nullptr)
			{
				//filter out "." and ".."
				if (std::string(directoryItem->d_name) == "." || std::string(directoryItem->d_name) == "..")
					continue;

				//check to see if the item is a directory. if it is, then recurse into it. if its not, unlink it.
				struct stat itemStats;
				std::string strFullDirectory = strCorrectedDirectory + Core::StringUtils::StandardisePath(directoryItem->d_name) + "\0";
				if (stat(strFullDirectory.c_str(), &itemStats) == 0)
				{
					if (S_ISDIR(itemStats.st_mode) == true)
					{
						if (DeleteDirectory(strFullDirectory)== false)
						{
							return false;
						}
					}
					else
					{
						unlink(strFullDirectory.c_str());
					}
				}
				else
				{
					return false;
				}
			}
			closedir(directory);

			//remove the directory
			if (rmdir(strCorrectedDirectory.c_str()) != 0)
			{
				return false;
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		void FileSystem::GetAllItemsInDirectory(const std::string& instrPath, bool inbRecurse, const std::string& instrPathSoFar, std::vector<std::string>& outItems) const
		{
			//insure the path is in the correct format
			std::string strCorrectedDirectory = Core::StringUtils::StandardisePath(instrPath);

			//this has the potential to have a path with a dot in it - make sure that it will always have a "/" on the end.
			if (strCorrectedDirectory[strCorrectedDirectory.size() - 1] != '/')
				strCorrectedDirectory += '/';

			//check the directory exists
			if (DoesDirectoryExist(strCorrectedDirectory) == false)
			{
				//CS_LOG_ERROR("Directory does not exist: " + strCorrectedDirectory);
				return;
			}

			//open up the directory
			DIR* directory = opendir(strCorrectedDirectory.c_str());
			if(directory == nullptr)
			{
				s32 errorType = errno;

				if(errorType == EACCES)
					CS_LOG_ERROR("Access Permission Error");
				else if(errorType == ELOOP)
					CS_LOG_ERROR("Symbolic Link Error");
				else if(errorType == ENAMETOOLONG)
					CS_LOG_ERROR("Name Too Long Error");
				else if(errorType == ENOENT)
					CS_LOG_ERROR("Dirname non existing/Dirname Empty String Error");
				else if(errorType == ENOTDIR)
					CS_LOG_ERROR("Dirname non existing Error");
				else if(errorType == EMFILE)
					CS_LOG_ERROR("To many files in Program Opened Error");
				else if(errorType == ENAMETOOLONG)
					CS_LOG_ERROR("Nametolong Error");
				else if(errorType == ENFILE)
					CS_LOG_ERROR("To many files in System Opened Error");

				return;
			}

			//iterate through the directory
			struct dirent* directoryItem;
			while ((directoryItem = readdir(directory)) != nullptr)
			{
				//filter out "." and ".."
				if (std::string(directoryItem->d_name) == "." || std::string(directoryItem->d_name) == "..")
					continue;

				//add the item to our list
				outItems.push_back(instrPathSoFar + std::string(directoryItem->d_name));

				//check if this item is a directory, if so, recurse!
				struct stat itemStats;
				std::string strFullDirectory = strCorrectedDirectory + Core::StringUtils::StandardisePath(directoryItem->d_name) + "\0";
				if (stat(strFullDirectory.c_str(), &itemStats) == 0)
				{
					if (S_ISDIR(itemStats.st_mode) == true)
						GetAllItemsInDirectory(strFullDirectory, inbRecurse, Core::StringUtils::StandardisePath(instrPathSoFar) + Core::StringUtils::StandardisePath(directoryItem->d_name), outItems);
				}
				else
				{
					CS_LOG_ERROR("Error: " + strFullDirectory);
				}

			}
			closedir(directory);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        std::string FileSystem::GetDirectoryForDLCFile(const std::string& instrFilePath) const
        {
        	if(DoesFileExistInCachedDLC(instrFilePath))
        	{
        		return ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrFilePath);
        	}

        	std::string strPath;
            for(u32 i=0; i<3; ++i)
            {
                unz_file_pos sZipPos;
                strPath = mastrResourceDirectory[i] + mstrPackageDLCPath + instrFilePath;

                if(DoesFileExistInAPKHashedStore(strPath, sZipPos))
                {
                    return strPath;
                }
            }

        	return "";
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        std::string FileSystem::GetDirectoryForPackageFile(const std::string& instrFilePath) const
        {
            std::string strResult;
            unz_file_pos sZipPos;
            for(u32 i=0; i<3; ++i)
            {
                std::string strPath = ChilliSource::Core::StringUtils::StandardisePath(mastrResourceDirectory[i] + instrFilePath);

                if(DoesFileExistInAPKHashedStore(strPath, sZipPos))
                {
                    strResult = strPath;
                    break;
                }
            }
            
            return strResult;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        unz_file_pos FileSystem::GetZipPosForPackageFile(const std::string& instrFilePath) const
        {
        	unz_file_pos Result;

            for(u32 i=0; i<3; ++i)
            {
                std::string strPath = ChilliSource::Core::StringUtils::StandardisePath(mastrResourceDirectory[i] + instrFilePath);

                if(DoesFileExistInAPKHashedStore(strPath, Result))
                {
                    break;
                }
            }

            return Result;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExist(const std::string& instrPath) const
		{
			struct stat fileStats;

			//if it is 0, no error occurred and the item exists
			if (stat(instrPath.c_str(), &fileStats) == 0)
			{
				//is it a file?
				if (S_ISDIR(fileStats.st_mode) == false)
					return true;
			}

			return false;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(const std::string& instrPath) const
		{
			struct stat fileStats;

			//if it is 0, no error occurred and the item exists
			if (stat(instrPath.c_str(), &fileStats) == 0)
			{
				//is it a file?
				if (S_ISDIR(fileStats.st_mode) == true)
					return true;
			}

			return false;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
        void FileSystem::GetDirectoryContents(const std::vector<DirInfo>& inaDirs, bool inbRecursive, std::vector<std::string>& outstrContents) const
		{
        	std::vector<unz_file_pos> aZipPos;
            for(std::vector<DirInfo>::const_iterator it = inaDirs.begin(); it != inaDirs.end(); ++it)
            {
            	aZipPos.clear();
                std::string path = ChilliSource::Core::StringUtils::StandardisePath(it->strPath);

                if(it->eLocation == Core::StorageLocation::k_package)
                {
                    GetAllItemsInDirectoryInAPK(path, inbRecursive, outstrContents, aZipPos);
                }
                else
                {
                    GetAllItemsInDirectory(path, inbRecursive, "", outstrContents);
                }
            }
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		void FileSystem::GetPathsForStorageLocation(Core::StorageLocation ineStorageLocation, const std::string& instrFileName, std::vector<DirInfo>& outaPaths) const
		{
			switch(ineStorageLocation)
			{
			case Core::StorageLocation::k_package:
				for(u32 i=0; i<3; ++i)
				{
					DirInfo Info;
					Info.strPath = mastrResourceDirectory[i] + instrFileName;
					Info.eLocation = Core::StorageLocation::k_package;
					outaPaths.push_back(Info);
				}
				break;
			case Core::StorageLocation::k_DLC:
			{
				for(u32 i=0; i<3; ++i)
				{
					DirInfo Info;
					Info.strPath = mastrResourceDirectory[i] + mstrPackageDLCPath + instrFileName;
					Info.eLocation = Core::StorageLocation::k_package;
					outaPaths.push_back(Info);
				}

				DirInfo Info;
				Info.strPath = GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrFileName;
				Info.eLocation = Core::StorageLocation::k_DLC;
				outaPaths.push_back(Info);
				break;
			}
			default:
			{
				DirInfo Info;
				Info.strPath = GetStorageLocationDirectory(ineStorageLocation) + instrFileName;
				Info.eLocation = ineStorageLocation;
				outaPaths.push_back(Info);
				break;
			}
			}
		}
	}
}
