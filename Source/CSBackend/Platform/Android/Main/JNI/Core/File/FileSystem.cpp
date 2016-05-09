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
			/// @author Ian Copland
			///
			/// @return The file path to the zip. This is dependent on the current Android
			/// Flavour: Google Play builds will refer to the OBB file, while Amazon builds
			/// will refer to the the APK.
			//------------------------------------------------------------------------------
			std::string GetZipFilePath()
			{
#if defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)
                JavaStaticClassDef apkExpansionInfoDef("com/chilliworks/chillisource/core/ApkExpansionInfo");
                apkExpansionInfoDef.AddMethod("getFilePath", "()Ljava/lang/String;");
                JavaStaticClass apkExpansionInfo(apkExpansionInfoDef);
                return apkExpansionInfo.CallStringMethod("getFilePath");
#elif defined(CS_ANDROIDFLAVOUR_AMAZON)
                auto coreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();
                return coreJI->GetAPKDirectory();
#else
                CS_LOG_FATAL("File System does not support this Android Flavour.");
                return "";
#endif
			}
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return the directory within the zip file which will be used as the root.
			/// This is dependent on the Android Flavour: Google Play builds will use the
			/// root of the zip, while Amazon builds will use the assets/ directory.
			//------------------------------------------------------------------------------
			std::string GetZipRootDirectory()
			{
#if defined(CS_ANDROIDFLAVOUR_GOOGLEPLAY)
				return "";
#elif defined(CS_ANDROIDFLAVOUR_AMAZON)
				return "assets/";
#else
				CS_LOG_FATAL("File System does not support this Android Flavour.");
				return "";
#endif
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
				std::string directoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath);
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
			    std::string directoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath);
			    std::string relativeParentDirectory = ChilliSource::StringUtils::StandardiseDirectoryPath(in_relativeParentDirectory);

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
                        if (in_searchForDirectories == true)
                        {
                            out_directoryPaths.push_back(relativeItemPath);
                        }

                        if (in_recursive == true)
                        {
                            fullItemPath = ChilliSource::StringUtils::StandardiseDirectoryPath(fullItemPath);
                            if (GetPaths(out_directoryPaths, fullItemPath, in_searchForDirectories, true, relativeItemPath) == false)
                            {
                                success = false;
                                break;
                            }
                        }
                    }
                    else if (in_searchForDirectories == false)
                    {
                        std::string relativeItemPath = relativeParentDirectory + ChilliSource::StringUtils::StandardiseFilePath(itemName);
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
			externalStorage = ChilliSource::StringUtils::StandardiseDirectoryPath(externalStorage);
			m_storagePath = externalStorage + "Android/data/" + coreJI->GetPackageName() + "/";

			CSBackend::Android::CreateDirectory(externalStorage + "Android/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/files/");
			CSBackend::Android::CreateDirectory(externalStorage + "Android/data/" + coreJI->GetPackageName() + "/cache/");

			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_saveData));
			CS_ASSERT(CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_saveData)) == true, "Could not create SaveData storage location.");

			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_cache));
			CS_ASSERT(CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_cache)) == true, "Could not create Cache storage location.");

			CSBackend::Android::CreateDirectory(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC));
			CS_ASSERT(CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC)) == true, "Could not create DLC storage location.");

			m_zipFilePath = CSBackend::Android::GetZipFilePath();
            m_zippedFileSystem = ZippedFileSystemUPtr(new ZippedFileSystem(m_zipFilePath, GetZipRootDirectory()));
            if (m_zippedFileSystem->IsValid() == false)
            {
            	CS_LOG_FATAL("File system cannot read Package.");
            }
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (ChilliSource::FileSystem::InterfaceID == in_interfaceId || FileSystem::InterfaceID == in_interfaceId);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		ChilliSource::FileStreamUPtr FileSystem::CreateFileStream(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, ChilliSource::FileMode in_fileMode) const
		{
			switch (in_storageLocation)
			{
				case ChilliSource::StorageLocation::k_package:
				case ChilliSource::StorageLocation::k_chilliSource:
				{
					CS_ASSERT(IsStorageLocationWritable(in_storageLocation) == false, "Cannot open write mode file stream in read-only storage location.");

					auto absFilePath = GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_filePath);
					return m_zippedFileSystem->CreateFileStream(absFilePath, in_fileMode);
					break;
				}
				case ChilliSource::StorageLocation::k_DLC:
				{
					if (DoesFileExistInCachedDLC(in_filePath) == true || IsWriteMode(in_fileMode) == true)
					{
						auto absFilePath = GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_filePath);
						ChilliSource::FileStreamUPtr output(new ChilliSource::FileStream(absFilePath, in_fileMode));
						if (output->IsValid() == true)
						{
							return output;
						}
					}
					else
					{
						auto absFilePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + ChilliSource::StringUtils::StandardiseFilePath(in_filePath);
						return m_zippedFileSystem->CreateFileStream(absFilePath, in_fileMode);
					}
					break;
				}
				default:
				{
					auto absFilePath = GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_filePath);
					ChilliSource::FileStreamUPtr output(new ChilliSource::FileStream(absFilePath, in_fileMode));
					if (output->IsValid() == true)
					{
						return output;
					}
					break;
				}
			}

			return nullptr;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::CreateDirectoryPath(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directory) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation) == true, "Cannot create directory in read-only storage location.");

            std::string path = GetAbsolutePathToStorageLocation(in_storageLocation);

            //get each level of the new directory separately
            std::string relativePath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_directory);
            std::vector<std::string> relativePathSections = ChilliSource::StringUtils::Split(relativePath, "/");

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
		bool FileSystem::CopyFile(ChilliSource::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
								  ChilliSource::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
		{
		    CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation) == true, "Cannot copy a file to a read-only storage location.");

            const s32 k_chunkSize = 32 * 1024;

            auto sourceStream = CreateFileStream(in_sourceStorageLocation, in_sourceFilePath, ChilliSource::FileMode::k_readBinary);
            if (sourceStream == nullptr)
            {
                return false;
            }

            auto destinationStream = CreateFileStream(in_destinationStorageLocation, in_destinationFilePath, ChilliSource::FileMode::k_writeBinary);
            if (destinationStream == nullptr)
            {
                return false;
            }

            sourceStream->SeekG(0, ChilliSource::SeekDir::k_end);
            s32 length = sourceStream->TellG();
            sourceStream->SeekG(0, ChilliSource::SeekDir::k_beginning);

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
		bool FileSystem::CopyDirectory(ChilliSource::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
						   ChilliSource::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation) == true, "Cannot copy a directory to a read-only storage location.");

            std::string sourceDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_sourceDirectoryPath);
            std::string destinationDirectoryPath = ChilliSource::StringUtils::StandardiseDirectoryPath(in_destinationDirectoryPath);

			//create all of the required directories.
			CreateDirectoryPath(in_destinationStorageLocation, destinationDirectoryPath);
			std::vector<std::string> subDirectoryPaths = GetDirectoryPaths(in_sourceStorageLocation, sourceDirectoryPath, true);
			for (const auto& subDirectoryPath : subDirectoryPaths)
			{
			    CreateDirectoryPath(in_destinationStorageLocation, destinationDirectoryPath + subDirectoryPath);
			}

			//copy files
			std::vector<std::string> filePaths = GetFilePaths(in_sourceStorageLocation, sourceDirectoryPath, true);
			switch (in_sourceStorageLocation)
			{
			    case ChilliSource::StorageLocation::k_package:
                case ChilliSource::StorageLocation::k_chilliSource:
                {
                    //This could be handled by simply calling CopyFile() for each of the required
                    //files, however this is pretty inefficient when reading from the zip as it's
                    //repeatedly opened and closed. For the sake of performance we use the batch
                    //ExtractFiles() method when copying from the zip.

                    std::string absSourceDirectoryPath = GetAbsolutePathToStorageLocation(in_sourceStorageLocation) + sourceDirectoryPath;
                    std::vector<std::string> sourceFiles, destinationFiles;
                    for (const auto& filePath : filePaths)
                    {
                        sourceFiles.push_back(absSourceDirectoryPath + filePath);
                        destinationFiles.push_back(destinationDirectoryPath + filePath);
                    }

                    u32 index = 0;
					return m_zippedFileSystem->ExtractFiles(sourceFiles, [&](const std::string& in_filePath, std::unique_ptr<const u8[]> in_fileContents, u32 in_fileSize) -> bool
					{
                        const auto& destination = destinationFiles[index++];
                        return WriteFile(in_destinationStorageLocation, destination, reinterpret_cast<const s8*>(in_fileContents.get()), in_fileSize);
					});
                }
                case ChilliSource::StorageLocation::k_DLC:
                {
                	//As with the Package and ChilliSource storage locations this could be handled
                	//by simply calling CopyFile() for each, however it would be inefficient for
                	//files contained in the zip. Instead, the batch ExtractFiles() method is used.

					std::string absPackageDLCPath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath();
                	std::vector<std::string> sourceFiles, destinationFiles;
                    for (const auto& filePath : filePaths)
                    {
                        auto sourceFilePath = sourceDirectoryPath + filePath;
                        auto destinationFilePath = destinationDirectoryPath + filePath;

                    	if (DoesFileExistInCachedDLC(sourceFilePath) == true)
                    	{
                    		if (CopyFile(in_sourceStorageLocation, sourceFilePath, in_destinationStorageLocation, destinationFilePath) == false)
							{
								return false;
							}
                    	}
                    	else
                    	{
							sourceFiles.push_back(absPackageDLCPath + sourceFilePath);
							destinationFiles.push_back(destinationFilePath);
                    	}
                    }

                    if (sourceFiles.empty() == false)
                    {
						u32 index = 0;
						return m_zippedFileSystem->ExtractFiles(sourceFiles, [&](const std::string& in_filePath, std::unique_ptr<const u8[]> in_fileContents, u32 in_fileSize) -> bool
						{
							const auto& destination = destinationFiles[index++];
							return WriteFile(in_destinationStorageLocation, destination, reinterpret_cast<const s8*>(in_fileContents.get()), in_fileSize);
						});
                    }

                    return true;
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

                    return true;
                }
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DeleteFile(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation) == true, "Cannot delete file from read-only storage location.");

            std::string filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_filePath);
            s32 error = unlink(filePath.c_str());
            if (error != 0)
            {
                return false;
            }

            return true;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DeleteDirectory(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "Cannot delete directory from read-only storage location.");

            std::string directoryPath = GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath);
            return CSBackend::Android::DeleteDirectory(directoryPath);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetFilePaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
		{
			switch (in_storageLocation)
			{
			    case ChilliSource::StorageLocation::k_package:
			    case ChilliSource::StorageLocation::k_chilliSource:
			    {
			        return m_zippedFileSystem->GetFilePaths(GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath), in_recursive);
			    }
			    case ChilliSource::StorageLocation::k_DLC:
			    {
                    auto absPackageDirectoryPath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath);
                    auto filePaths = m_zippedFileSystem->GetFilePaths(absPackageDirectoryPath, in_recursive);

                    auto absCacheDirectoryPath = GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath);
                    if (CSBackend::Android::DoesDirectoryExist(absCacheDirectoryPath) == true && GetPaths(filePaths, absCacheDirectoryPath, false, in_recursive) == false)
                    {
                        CS_LOG_ERROR("Failed to get file paths in directory '" + in_directoryPath + "' in storage location '" + ChilliSource::ToString(in_storageLocation) + "'");
                    }

                    std::sort(filePaths.begin(), filePaths.end());
                    auto it = std::unique(filePaths.begin(), filePaths.end());
                    filePaths.resize(it - filePaths.begin());
                    return filePaths;
			    }
                default:
                {
                    std::vector<std::string> filePaths;
                    if (GetPaths(filePaths, GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath), false, in_recursive) == false)
                    {
                        CS_LOG_ERROR("Failed to get file paths in directory '" + in_directoryPath + "' in storage location '" + ChilliSource::ToString(in_storageLocation) + "'");
                    }

                    return filePaths;
                }
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::vector<std::string> FileSystem::GetDirectoryPaths(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath,  bool in_recursive) const
		{
            switch (in_storageLocation)
            {
                case ChilliSource::StorageLocation::k_package:
                case ChilliSource::StorageLocation::k_chilliSource:
                {
                    return m_zippedFileSystem->GetDirectoryPaths(GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath), in_recursive);
                }
                case ChilliSource::StorageLocation::k_DLC:
                {
                    auto absPackageDirectoryPath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath);
                    auto subDirectoryPaths = m_zippedFileSystem->GetDirectoryPaths(absPackageDirectoryPath, in_recursive);

                    auto absCacheDirectoryPath = GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath);
                    if (CSBackend::Android::DoesDirectoryExist(absCacheDirectoryPath) == true && GetPaths(subDirectoryPaths, absCacheDirectoryPath, true, in_recursive) == false)
                    {
                        CS_LOG_ERROR("Failed to get directory paths in directory '" + in_directoryPath + "' in storage location '" + ChilliSource::ToString(in_storageLocation) + "'");
                    }

                    std::sort(subDirectoryPaths.begin(), subDirectoryPaths.end());
                    auto it = std::unique(subDirectoryPaths.begin(), subDirectoryPaths.end());
                    subDirectoryPaths.resize(it - subDirectoryPaths.begin());
                    return subDirectoryPaths;
                }
                default:
                {
                    std::vector<std::string> directoryPaths;
                    if (GetPaths(directoryPaths, GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath), true, in_recursive) == false)
                    {
                        CS_LOG_ERROR("Failed to get directory paths in directory '" + in_directoryPath + "' in storage location '" + ChilliSource::ToString(in_storageLocation) + "'");
                    }

                    return directoryPaths;
                }
            }
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesFileExist(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath) const
		{
			switch (in_storageLocation)
			{
				case ChilliSource::StorageLocation::k_package:
				case ChilliSource::StorageLocation::k_chilliSource:
				{
					return m_zippedFileSystem->DoesFileExist(GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_filePath));
				}
				case ChilliSource::StorageLocation::k_DLC:
				{
					return (DoesFileExistInCachedDLC(in_filePath) || DoesFileExistInPackageDLC(in_filePath));
				}
				default:
				{
					return CSBackend::Android::DoesFileExist(GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_filePath));
				}
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& in_filePath) const
		{
			return CSBackend::Android::DoesFileExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + ChilliSource::StringUtils::StandardiseFilePath(in_filePath));
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& in_filePath) const
		{
			return DoesFileExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(ChilliSource::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
		{
			switch (in_storageLocation)
			{
				case ChilliSource::StorageLocation::k_package:
                case ChilliSource::StorageLocation::k_chilliSource:
                {
                	return m_zippedFileSystem->DoesDirectoryExist(GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath));
                }
                case ChilliSource::StorageLocation::k_DLC:
                {
                	return (DoesDirectoryExistInCachedDLC(in_directoryPath) || DoesDirectoryExistInPackageDLC(in_directoryPath));
                }
                default:
                {
                	return CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseDirectoryPath(in_directoryPath));
                }
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInCachedDLC(const std::string& in_directoryPath) const
		{
			return CSBackend::Android::DoesDirectoryExist(GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_DLC) + ChilliSource::StringUtils::StandardiseFilePath(in_directoryPath));
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInPackageDLC(const std::string& in_directoryPath) const
		{
			return DoesDirectoryExist(ChilliSource::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		std::string FileSystem::GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation in_storageLocation) const
		{
			//get the storage location path
			std::string storageLocationPath;
			switch (in_storageLocation)
			{
				case ChilliSource::StorageLocation::k_package:
					storageLocationPath = k_packagePath;
					break;
				case ChilliSource::StorageLocation::k_chilliSource:
					storageLocationPath = k_chilliSourcePath;
					break;
				case ChilliSource::StorageLocation::k_saveData:
					storageLocationPath = m_storagePath + k_saveDataPath;
					break;
				case ChilliSource::StorageLocation::k_cache:
					storageLocationPath = m_storagePath + k_cachePath;
					break;
				case ChilliSource::StorageLocation::k_DLC:
					storageLocationPath = m_storagePath + k_dlcPath;
					break;
				case ChilliSource::StorageLocation::k_root:
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
		const std::string& FileSystem::GetZipFilePath() const
		{
			return m_zipFilePath;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool FileSystem::TryGetZippedFileInfo(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, ZippedFileInfo& out_zippedFileInfo) const
		{
			CS_ASSERT(in_storageLocation == ChilliSource::StorageLocation::k_package || in_storageLocation == ChilliSource::StorageLocation::k_chilliSource
				|| in_storageLocation == ChilliSource::StorageLocation::k_DLC, "Invalid storage location.");

			std::string filePath;
			if (in_storageLocation == ChilliSource::StorageLocation::k_DLC)
			{
				filePath = GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + GetPackageDLCPath() + ChilliSource::StringUtils::StandardiseFilePath(in_filePath);
			}
			else
			{
				filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + ChilliSource::StringUtils::StandardiseFilePath(in_filePath);
			}

			ZippedFileSystem::FileInfo info;
			if (m_zippedFileSystem->TryGetFileInfo(filePath, info) == false)
			{
				return false;
			}

			out_zippedFileInfo.m_offset = info.m_offset;
			out_zippedFileInfo.m_size = info.m_size;
			out_zippedFileInfo.m_uncompressedSize = info.m_uncompressedSize;
			out_zippedFileInfo.m_isCompressed = info.m_isCompressed;
			return true;
		}

	}
}

#endif
