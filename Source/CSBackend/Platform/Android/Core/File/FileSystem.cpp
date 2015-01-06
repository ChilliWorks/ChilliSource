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

#include <CSBackend/Platform/Android/Core/File/FileSystem.h>

#include <CSBackend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <CSBackend/Platform/Android/Core/File/FileStreamAPK.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
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

namespace CSBackend
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
			/// @author Ian Copland
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
            /// @author Ian Copland
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
			/// @author Ian Copland
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
			/// @author Ian Copland
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
			/// @author Ian Copland
			///
			/// @param The path to check.
			///
			/// @return Whether or not the given path is to a file.
			//--------------------------------------------------------------
			bool IsFilePathString(const std::string& in_path)
			{
				return (CSCore::StringUtils::Match(in_path, "*.*") == true);
			}
            //--------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The path to check.
			///
			/// @return Whether or not the given path is to a directory.
			//--------------------------------------------------------------
			bool IsDirectoryPathString(const std::string& in_path)
			{
				return (CSCore::StringUtils::Match(in_path, "*.*") == false);
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
            /// @author Ian Copland
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
			/// @author Ian Copland
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
				CSCore::FileStreamSPtr sourceStream = CSCore::FileStreamSPtr(new CSCore::FileStream());
				sourceStream->Open(in_sourceFilePath, CSCore::FileMode::k_readBinary);
				if (sourceStream.get() == nullptr || sourceStream->IsBad() == true || sourceStream->IsOpen() == false)
				{
					return false;
				}

				//open the destination file
				CSCore::FileStreamSPtr destinationStream = CSCore::FileStreamSPtr(new CSCore::FileStream());
				destinationStream->Open(in_destinationFilePath, CSCore::FileMode::k_writeBinary);
				if (destinationStream.get() == nullptr || destinationStream->IsBad() == true || destinationStream->IsOpen() == false)
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

				//cleanup
				sourceStream->Close();
				destinationStream->Close();

				return true;
			}
			//--------------------------------------------------------------
			/// Deletes a directory and all its contents.
			///
			/// @author Ian Copland
			///
			/// @param The directory path.
			///
			/// @return Whether or not the directory was successfully deleted.
			//--------------------------------------------------------------
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

		const std::string FileSystem::k_packageAPKDir = "AppResources/";
		const std::string FileSystem::k_csAPKDir = "CSResources/";

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
			externalStorage = CSCore::StringUtils::StandardiseDirectoryPath(externalStorage);
			m_storagePath = externalStorage + "Android/data/" + coreJI->GetPackageName() + "/";

			CSBackend::Android::CreateDirectory(externalStorage + "Android/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/files/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/cache/");

			CreateAPKManifest();

			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_saveData));
			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_cache));
			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC));
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		bool FileSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (CSCore::FileSystem::InterfaceID == in_interfaceId || FileSystem::InterfaceID == in_interfaceId);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		CSCore::FileStreamUPtr FileSystem::CreateFileStream(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath, CSCore::FileMode in_fileMode) const
		{
			if (IsWriteMode(in_fileMode) == true)
			{
				CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");

				std::string filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
				CSCore::FileStreamUPtr fileStream = CSCore::FileStreamUPtr(new CSCore::FileStream());
				fileStream->Open(filePath, in_fileMode);
                return fileStream;
			}
			else
			{
				//if trying to read from the package or from DLC when the file is not in the cache DLC, open a APK file stream. Otherwise open a standard file stream.
				switch(in_storageLocation)
				{
					case CSCore::StorageLocation::k_package:
					case CSCore::StorageLocation::k_chilliSource:
					{
						std::string absoluteFilePath = GetAbsolutePathToFile(in_storageLocation, in_filePath);
						return CreateFileStreamInAPK(in_storageLocation, absoluteFilePath, in_fileMode);
					}
					case CSCore::StorageLocation::k_DLC:
					{
						if(DoesFileExistInCachedDLC(in_filePath) == false)
						{
							std::string absoluteFilePath = GetAbsolutePathToFile(CSCore::StorageLocation::k_DLC, in_filePath);
							return CreateFileStreamInAPK(CSCore::StorageLocation::k_package, absoluteFilePath, in_fileMode);
						}
					}
					default:
					{
						std::string filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
						CSCore::FileStreamUPtr fileStream = CSCore::FileStreamUPtr(new CSCore::FileStream());
						fileStream->Open(filePath, in_fileMode);
						return fileStream;
					}
				}
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CreateDirectoryPath(CSCore::StorageLocation in_storageLocation, const std::string& in_directory) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");

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
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyFile(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
								  CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
		{
			std::vector<std::string> sourcePaths = {in_sourceFilePath};
			std::vector<std::string> destPaths = {in_destinationFilePath};
			return CopyFiles(in_sourceStorageLocation, sourcePaths, in_destinationStorageLocation, destPaths);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyFiles(CSCore::StorageLocation in_sourceStorageLocation, const std::vector<std::string>& in_sourceFilePaths,
								  CSCore::StorageLocation in_destinationStorageLocation, const std::vector<std::string>& in_destinationFilePaths) const
		{
			CS_ASSERT(in_sourceFilePaths.size() == in_destinationFilePaths.size(), "File System: Mismatch number of source paths and dest paths when copying files");
			CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");

			//check if we're loading from DLC, and ensure that the file exists in the dlc cache. if it does not, fall back on package.
			if (in_sourceStorageLocation == CSCore::StorageLocation::k_package)
			{
				std::vector<std::string> filePaths;
				filePaths.reserve(in_sourceFilePaths.size());
				for(const auto& path : in_sourceFilePaths)
				{
					filePaths.push_back(GetAbsolutePathToFile(CSCore::StorageLocation::k_package, path));
					if(filePaths.back().empty())
					{
						CS_LOG_ERROR("File System: Trying to copy file '" + path + "' but it does not exist.");
						return false;
					}
				}

				return CopyFilesFromAPK(in_sourceStorageLocation, filePaths, in_destinationStorageLocation, in_destinationFilePaths);
			}
			else if (in_sourceStorageLocation == CSCore::StorageLocation::k_DLC)
			{
				std::vector<std::string> apkFilePaths;
				apkFilePaths.reserve(in_sourceFilePaths.size());

				std::vector<std::string> cachedDLCFilePaths;
				cachedDLCFilePaths.reserve(in_sourceFilePaths.size());

				for(const auto& path : in_sourceFilePaths)
				{
					if(DoesFileExistInCachedDLC(path) == false)
					{
						apkFilePaths.push_back(GetAbsolutePathToFile(CSCore::StorageLocation::k_package, path));
						if(apkFilePaths.back().empty())
						{
							CS_LOG_ERROR("File System: Trying to copy file '" + path + "' but it does not exist.");
							return false;
						}
					}
					else
					{
						cachedDLCFilePaths.push_back(GetAbsolutePathToFile(in_sourceStorageLocation, path));
					}
				}

				if(CopyFilesFromAPK(CSCore::StorageLocation::k_package, apkFilePaths, in_destinationStorageLocation, in_destinationFilePaths) == false)
				{
					return false;
				}

				if(CopyNonAPKFiles(in_sourceStorageLocation, cachedDLCFilePaths, in_destinationStorageLocation, in_destinationFilePaths) == false)
				{
					return false;
				}

				return true;
			}
			else
			{
				return CopyNonAPKFiles(in_sourceStorageLocation, in_sourceFilePaths, in_destinationStorageLocation, in_destinationFilePaths);
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyNonAPKFiles(CSCore::StorageLocation in_sourceStorageLocation, const std::vector<std::string>& in_sourceFilePaths,
				CSCore::StorageLocation in_destinationStorageLocation, const std::vector<std::string>& in_destinationFilePaths) const
		{
			for(u32 i=0; i<in_sourceFilePaths.size(); ++i)
			{
				std::string sourceAbsolutePath = GetAbsolutePathToFile(in_sourceStorageLocation, in_sourceFilePaths[i]);
				std::string destinationAbsolutePath = GetAbsolutePathToStorageLocation(in_destinationStorageLocation) + in_destinationFilePaths[i];

				//check the source file exists
				if(CSBackend::Android::DoesFileExist(sourceAbsolutePath) == false)
				{
					CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePaths[i] + "' but it does not exist.");
					return false;
				}

				//get the path to the file
				std::string path, name;
				CSCore::StringUtils::SplitFilename(in_destinationFilePaths[i], name, path);

				//create the output directory
				CreateDirectoryPath(in_destinationStorageLocation, path);

				//try and copy the file
				CSBackend::Android::CopyFile(sourceAbsolutePath, destinationAbsolutePath);

				if(CSBackend::Android::DoesFileExist(destinationAbsolutePath) == false)
				{
					return false;
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::CopyDirectory(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
						   CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const
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
				return true;
			}

			std::string sourcePath = CSCore::StringUtils::StandardiseDirectoryPath(in_sourceDirectoryPath);
			std::string destPath = CSCore::StringUtils::StandardiseDirectoryPath(in_destinationDirectoryPath);

			std::vector<std::string> sourcePaths;
			sourcePaths.reserve(filenames.size());

			std::vector<std::string> destPaths;
			destPaths.reserve(filenames.size());

			for (const std::string& filename : filenames)
			{
				sourcePaths.push_back(sourcePath + filename);
				destPaths.push_back(destPath + filename);
			}

			return CopyFiles(in_sourceStorageLocation, sourcePaths, in_destinationStorageLocation, destPaths);
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DeleteFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const
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
		bool FileSystem::DeleteDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");

			std::string directoryPath = GetAbsolutePathToDirectory(in_storageLocation, in_directoryPath);
			if (directoryPath != "")
			{
				CSBackend::Android::DeleteDirectory(directoryPath);
				return true;
			}

			return false;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetFilePaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
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
		std::vector<std::string> FileSystem::GetDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const
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
		bool FileSystem::DoesFileExist(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			switch(in_storageLocation)
			{
				case CSCore::StorageLocation::k_package:
				case CSCore::StorageLocation::k_chilliSource:
				{
					return DoesFileExistInAPK(in_storageLocation, GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath);
				}
				case CSCore::StorageLocation::k_DLC:
				{
					if (DoesFileExistInCachedDLC(in_filePath) == true)
					{
						return true;
					}

					return DoesFileExist(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
				}
				default:
				{
					return CSBackend::Android::DoesFileExist(CSCore::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath));
				}
			}
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& in_filePath) const
		{
			return CSBackend::Android::DoesFileExist(CSCore::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_filePath));
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& in_filePath) const
		{
			return DoesFileExist(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
            if (in_storageLocation == CSCore::StorageLocation::k_package || in_storageLocation == CSCore::StorageLocation::k_chilliSource)
            {
            	return DoesDirectoryExistInAPK(in_storageLocation, in_directoryPath);
            }
            else if (in_storageLocation == CSCore::StorageLocation::k_DLC)
			{
				if (CSBackend::Android::DoesDirectoryExist(CSCore::StringUtils::StandardiseDirectoryPath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_directoryPath)) == true)
				{
					return true;
				}

				return DoesDirectoryExist(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
			}
            else
            {
            	return CSBackend::Android::DoesDirectoryExist(CSCore::StringUtils::StandardiseDirectoryPath(GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath));
            }
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToStorageLocation(CSCore::StorageLocation in_storageLocation) const
		{
			//get the storage location path
			std::string storageLocationPath;
			switch (in_storageLocation)
			{
				case CSCore::StorageLocation::k_package:
					storageLocationPath = "";
					break;
				case CSCore::StorageLocation::k_chilliSource:
					storageLocationPath = "";
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
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			if (DoesFileExist(in_storageLocation, in_filePath) == true)
			{
				switch (in_storageLocation)
				{
					case CSCore::StorageLocation::k_DLC:
					{
						std::string filePath = CSCore::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_filePath);
						if(CSBackend::Android::DoesFileExist(filePath) == true)
						{
							return filePath;
						}

						return GetAbsolutePathToFile(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
					}
					default:
					{
						return GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
					}
				}
			}

			CS_LOG_WARNING("File does not exist: " + in_filePath);
			return "";
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			if (DoesDirectoryExist(in_storageLocation, in_directoryPath) == true)
			{
				switch (in_storageLocation)
				{
					case CSCore::StorageLocation::k_DLC:
					{
						std::string directoryPath = CSCore::StringUtils::StandardiseDirectoryPath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_directoryPath);
						if(CSBackend::Android::DoesDirectoryExist(directoryPath) == true)
						{
							return directoryPath;
						}

						return GetAbsolutePathToDirectory(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
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
		void FileSystem::AddItemToManifest(CSCore::StorageLocation in_location, const std::string& in_filePath, unz_file_pos in_zipPos, std::vector<APKManifestItem>& inout_items)
		{
			std::string directoryPath = CSCore::StringUtils::StandardiseDirectoryPath(in_filePath.substr(0, in_filePath.rfind("/") + 1));

			//check to see if this directory has previously been seen. If it has not, add it.
			if (directoryPath.size() != 0 && DoesDirectoryExistInAPK(in_location, directoryPath) == false)
			{
				APKManifestItem item;
				item.m_path = directoryPath;
				item.m_pathHash = CSCore::HashCRC32::GenerateHashCode(item.m_path);
				item.m_isFile = false;
				item.m_apkPosition = in_zipPos;
				inout_items.push_back(item);
			}

			APKManifestItem item;
			item.m_path = in_filePath;
			item.m_pathHash = CSCore::HashCRC32::GenerateHashCode(item.m_path);
			item.m_isFile = true;
			item.m_apkPosition = in_zipPos;
			inout_items.push_back(item);
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

			std::string appAssetsPath = k_assetsPath + k_packageAPKDir;
			std::string csAssetsPath = k_assetsPath + k_csAPKDir;

			s32 status = unzGoToFirstFile(unzipper);
			while (status == UNZ_OK)
			{
				const u32 k_filePathLength = 1024;

				char filePathBytes[k_filePathLength];
				unz_file_info info;
				unzGetCurrentFileInfo(unzipper, &info, filePathBytes, k_filePathLength, nullptr, 0, nullptr, 0);

				//get the path and filename
				std::string filePath = CSCore::StringUtils::StandardiseFilePath(filePathBytes);

				//if this file is at the same path as requested, then add it to the output
				if (CSCore::StringUtils::StartsWith(filePath, appAssetsPath, false) == true)
				{
					filePath = filePath.erase(0, appAssetsPath.size());
					unz_file_pos filePos;
					unzGetFilePos(unzipper, &filePos);
					AddItemToManifest(CSCore::StorageLocation::k_package, filePath, filePos, m_apkAppManifestItems);
				}
				else if(CSCore::StringUtils::StartsWith(filePath, csAssetsPath, false) == true)
				{
					filePath = filePath.erase(0, csAssetsPath.size());
					unz_file_pos filePos;
					unzGetFilePos(unzipper, &filePos);
					AddItemToManifest(CSCore::StorageLocation::k_chilliSource, filePath, filePos, m_apkCSManifestItems);
				}

				status = unzGoToNextFile(unzipper);
			}
			unzClose(unzipper);

			std::sort(m_apkAppManifestItems.begin(), m_apkAppManifestItems.end(), APKManifestSortPredicate);
			std::sort(m_apkCSManifestItems.begin(), m_apkCSManifestItems.end(), APKManifestSortPredicate);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::TryGetManifestItem(CSCore::StorageLocation in_location, const std::string& in_path, APKManifestItem& out_manifestItem) const
        {
        	CS_ASSERT(in_location == CSCore::StorageLocation::k_package || in_location == CSCore::StorageLocation::k_chilliSource, "APK only covers package and cs locations");

			APKManifestItem searchItem;
			searchItem.m_pathHash = CSCore::HashCRC32::GenerateHashCode(in_path);

			if(in_location == CSCore::StorageLocation::k_package)
			{
				auto it = std::lower_bound(m_apkAppManifestItems.begin(), m_apkAppManifestItems.end(), searchItem, APKManifestSortPredicate);
				if(it !=  m_apkAppManifestItems.end() && it->m_pathHash == searchItem.m_pathHash)
				{
					out_manifestItem = *it;
					return true;
				}
			}
			else
			{
				auto it = std::lower_bound(m_apkCSManifestItems.begin(), m_apkCSManifestItems.end(), searchItem, APKManifestSortPredicate);
				if(it !=  m_apkCSManifestItems.end() && it->m_pathHash == searchItem.m_pathHash)
				{
					out_manifestItem = *it;
					return true;
				}
			}

			return false;
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		CSCore::FileStreamUPtr FileSystem::CreateFileStreamInAPK(CSCore::StorageLocation in_location, const std::string& in_filePath, CSCore::FileMode in_fileMode) const
		{
			CSCore::FileStreamUPtr fileStream = CSCore::FileStreamUPtr(new FileStreamAPK(&m_minizipMutex));

			APKManifestItem manifestItem;
			if (TryGetManifestItem(in_location, CSCore::StringUtils::StandardiseFilePath(in_filePath), manifestItem) == true)
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
		bool FileSystem::CopyFilesFromAPK(CSCore::StorageLocation in_srcLocation, const std::vector<std::string>& in_filePaths, CSCore::StorageLocation in_destinationStorageLocation, const std::vector<std::string>& in_destinationFilePaths) const
		{
			std::unique_lock<std::mutex> lock(m_minizipMutex);

			unzFile unzip = unzOpen(m_apkPath.c_str());
			if (unzip == nullptr)
			{
				CS_LOG_ERROR("File System: Cannot open APK");
				return false;
			}

			bool isSuccess = false;

			for(u32 i=0; i<in_filePaths.size(); ++i)
			{
				APKManifestItem manifestItem;
				if (TryGetManifestItem(in_srcLocation, CSCore::StringUtils::StandardiseFilePath(in_filePaths[i]), manifestItem) == true)
				{
					if (manifestItem.m_isFile == true)
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
								CSCore::StringUtils::SplitFilename(in_destinationFilePaths[i], outputFileName, outputDirectoryPath);
								if (CreateDirectoryPath(in_destinationStorageLocation, outputDirectoryPath) == true)
								{
									char* dataBuffer = new char[info.uncompressed_size];
									unzReadCurrentFile(unzip, (voidp)dataBuffer, info.uncompressed_size);

									if (WriteFile(in_destinationStorageLocation, in_destinationFilePaths[i], dataBuffer, info.uncompressed_size) == true)
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
				}

				if (isSuccess == false)
				{
					CS_LOG_ERROR("File System: Failed to copy file '" + in_filePaths[i] + "' from APK.");
					break;
				}
			}

			unzClose(unzip);

			return isSuccess;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<std::string> FileSystem::GetPathsInAPK(CSCore::StorageLocation in_location, const std::string& in_directoryPath, bool in_recursive) const
		{
			CS_ASSERT(in_location == CSCore::StorageLocation::k_package || in_location == CSCore::StorageLocation::k_chilliSource, "APK only covers package and cs locations");

			std::vector<std::string> output;

			std::string directoryPath = CSCore::StringUtils::StandardiseDirectoryPath(in_directoryPath);

			if(in_location == CSCore::StorageLocation::k_package)
			{
				for (const APKManifestItem& item : m_apkAppManifestItems)
				{
					std::string itemFileName;
					std::string itemDirectoryPath;
					CSCore::StringUtils::SplitFilename(item.m_path, itemFileName, itemDirectoryPath);

					if ((in_recursive == true && CSCore::StringUtils::Match(item.m_path, directoryPath + "*") == true) || (in_recursive == false && directoryPath == itemDirectoryPath))
					{
						output.push_back(item.m_path.substr(directoryPath.length()));
					}
				}
			}
			else
			{
				for (const APKManifestItem& item : m_apkCSManifestItems)
				{
					std::string itemFileName;
					std::string itemDirectoryPath;
					CSCore::StringUtils::SplitFilename(item.m_path, itemFileName, itemDirectoryPath);

					if ((in_recursive == true && CSCore::StringUtils::Match(item.m_path, directoryPath + "*") == true) || (in_recursive == false && directoryPath == itemDirectoryPath))
					{
						output.push_back(item.m_path.substr(directoryPath.length()));
					}
				}
			}

			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInAPK(CSCore::StorageLocation in_location, const std::string& in_filePath) const
		{
			APKManifestItem manifestItem;
			if (TryGetManifestItem(in_location, CSCore::StringUtils::StandardiseFilePath(in_filePath), manifestItem) == true)
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
		bool FileSystem::DoesDirectoryExistInAPK(CSCore::StorageLocation in_location, const std::string& in_directoryPath) const
		{
			APKManifestItem manifestItem;
			if (TryGetManifestItem(in_location, CSCore::StringUtils::StandardiseDirectoryPath(in_directoryPath), manifestItem) == true)
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
			std::string directoryPath = CSCore::StringUtils::StandardiseDirectoryPath(in_directoryPath);
			std::string parentDirectoryPath = CSCore::StringUtils::StandardiseDirectoryPath(in_parentDirectoryPath);

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
			if (CSBackend::Android::DoesDirectoryExist(directoryPath) == false)
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
				struct stat dirStats;
				std::string itemName(directoryItem->d_name);

				//filter out "." and ".."
				if (itemName == "." || itemName == "..")
					continue;

				bool isDir = false;
				std::string itemPath = directoryPath + itemName;
				if (stat(itemPath.c_str(), &dirStats) == 0)
				{
					if (S_ISDIR(dirStats.st_mode) == true)
					{
						itemName = CSCore::StringUtils::StandardiseDirectoryPath(itemName);
						itemPath = CSCore::StringUtils::StandardiseDirectoryPath(itemPath);
						isDir = true;
					}
					else
					{
						itemName = CSCore::StringUtils::StandardiseFilePath(itemName);
						itemPath = CSCore::StringUtils::StandardiseFilePath(itemPath);
						isDir = false;
					}
				}
				else
				{
					CS_LOG_ERROR("Error: Failed to stat path '" + itemPath + "'");
				}

				//add the item to our list
				output.push_back(parentDirectoryPath + itemName);

				//check if this item is a directory, if so, recurse!
				if (isDir == true)
				{
					std::vector<std::string> subDirectoryPaths = GetAllPaths(itemPath, in_recursive, parentDirectoryPath + itemName);
					output.insert(output.end(), subDirectoryPaths.begin(), subDirectoryPaths.end());
				}
			}

			closedir(directory);

			return output;
		}
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		std::vector<FileSystem::PathInfo> FileSystem::GetPossibleAbsoluteDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			std::vector<PathInfo> output;
			switch(in_storageLocation)
			{
				case CSCore::StorageLocation::k_chilliSource:
				case CSCore::StorageLocation::k_package:
				{
					PathInfo info;
					info.m_path = in_directoryPath;
					info.m_storageLocation = in_storageLocation;
					output.push_back(info);
					break;
				}
				case CSCore::StorageLocation::k_DLC:
				{
					{
						PathInfo info;
						info.m_path = CSCore::StringUtils::StandardiseDirectoryPath(GetPackageDLCPath() + in_directoryPath);
						info.m_storageLocation = CSCore::StorageLocation::k_package;
						output.push_back(info);
					}
					{
						PathInfo info;
						info.m_path = GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_directoryPath;
						info.m_storageLocation = CSCore::StorageLocation::k_DLC;
						output.push_back(info);
					}
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
                std::string path = CSCore::StringUtils::StandardiseDirectoryPath(directoryInfo.m_path);

                if(directoryInfo.m_storageLocation == CSCore::StorageLocation::k_package || directoryInfo.m_storageLocation == CSCore::StorageLocation::k_chilliSource)
                {
                	std::vector<std::string> paths = GetPathsInAPK(directoryInfo.m_storageLocation, path, in_recursive);
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
