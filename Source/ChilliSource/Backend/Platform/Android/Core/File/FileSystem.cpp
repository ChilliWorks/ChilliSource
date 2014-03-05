/*
 *  FileSystem.cpp
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

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
		const std::string kstrAssetsDir = "assets/";
		const std::string kstrSaveDataDir = "files/SaveData/";
		const std::string kstrDLCDir = "cache/DLC/";
		const std::string kstrCacheDir = "cache/Cache/";

		FileSystem::FileSystem()
		{
			CoreJavaInterfaceSPtr pCoreJI = JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CoreJavaInterface>();

			//get the path to the bundle dir
			mstrPathToAPK = pCoreJI->GetAPKDirectory();

			//get the storage directory
			std::string externalStorage = pCoreJI->GetExternalStorageDirectory();
			if (externalStorage != "")
			{
				//if external storage is available
				std::string packageName = pCoreJI->GetPackageName();
				mstrStorageDir = Core::StringUtils::StandardisePath(externalStorage + "/Android/data/" + packageName + "/");

				//Create the directories
				CreateBaseDirectory(GetStorageLocationDirectory(Core::StorageLocation::k_saveData));
				CreateBaseDirectory(GetStorageLocationDirectory(Core::StorageLocation::k_cache));
				CreateBaseDirectory(GetStorageLocationDirectory(Core::StorageLocation::k_DLC));

				mbStorageAvailable = true;
			}
			else
			{
				//if neither is available
				CS_LOG_FATAL("Cannot access external storage!");
				mbStorageAvailable = false;
			}

			CreateHashedAPKFileList();
		}
		//--------------------------------------------------------------
		/// File Info Sort Predicate
		///
		/// Sort by file name hash lowest to highest
		//--------------------------------------------------------------
		bool FileInfoSortPredicate(const APKFileInfo& inLHS, const APKFileInfo& inRHS)
		{
			return inLHS.udwFileNameHash < inRHS.udwFileNameHash;
		}
        //--------------------------------------------------------------
        /// Create Hashed APK File List
        ///
        //--------------------------------------------------------------
        void FileSystem::CreateHashedAPKFileList()
        {
        	std::vector<std::string> aContent;
        	std::vector<unz_file_pos> aZipPos;
        	GetAllItemsInDirectoryInAPK("", true, aContent, aZipPos);

        	APKFileInfo sInfo;
        	u32 i=0;
        	for(std::vector<std::string>::const_iterator it = aContent.begin(); it != aContent.end(); ++it, ++i)
        	{
        		sInfo.udwFileNameHash = Core::HashCRC32::GenerateHashCode(*it);
        		sInfo.sZipPos = aZipPos[i];
        		mAPKFileInfo.push_back(sInfo);
        	}

            std::sort(mAPKFileInfo.begin(), mAPKFileInfo.end(), FileInfoSortPredicate);
        }
		//--------------------------------------------------------------
		/// Create File Stream
		//--------------------------------------------------------------
		Core::FileStreamSPtr FileSystem::CreateFileStream(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath, Core::FileMode ineFileMode) const
		{
			Core::FileStreamSPtr newFilestream = Core::FileStreamSPtr(new Core::FileStream());

			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available for " + instrFilepath);
				return newFilestream;
			}

			std::string filepath = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;

			//if this is not a read stream, insure that the storage location is writable.
			if (ineFileMode != Core::FileMode::k_read && ineFileMode != Core::FileMode::k_readBinary)
			{
                if(IsStorageLocationWritable(ineStorageLocation) == true)
                {
                    newFilestream->Open(filepath, ineFileMode);
                }
                else
                {
                    CS_LOG_ERROR("Cannot write to the requested Storage Location!");
                }

                return newFilestream;
			}

			if (ineStorageLocation == Core::StorageLocation::k_package)
			{
				unz_file_pos ZipPos = GetZipPosForPackageFile(instrFilepath);
				return CreateAPKFileStream(ZipPos, ineFileMode);
			}

			//if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
			if (ineStorageLocation == Core::StorageLocation::k_DLC && DoesFileExistInDLCCache(instrFilepath) == false)
			{
				unz_file_pos ZipPos = GetZipPosForPackageFile(mstrPackageDLCPath + instrFilepath);
				return CreateAPKFileStream(ZipPos, ineFileMode);
			}

			newFilestream->Open(filepath, ineFileMode);
			return newFilestream;
		}
		//--------------------------------------------------------------
		/// Create File
		//--------------------------------------------------------------
		bool FileSystem::CreateFile(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, s8* inpbyData, u32 inudwDataSize) const
		{
			Core::FileStreamSPtr pFileStream = CreateFileStream(ineStorageLocation, instrDirectory, Core::FileMode::k_writeBinary);

			if (pFileStream.get() == NULL || pFileStream->IsOpen() == false || pFileStream->IsBad() == true)
			{
				CS_LOG_ERROR("Failed to create file: " + instrDirectory);
				return false;
			}

			pFileStream->Write(inpbyData, (s32)inudwDataSize);
			pFileStream->Close();
			return true;
		}
		//--------------------------------------------------------------
		/// Create Directory
		//--------------------------------------------------------------
		bool FileSystem::CreateDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available for " + instrDirectory);
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Cannot write to the requested Storage Location " + instrDirectory);
				return false;
			}

			//create the directory
			std::string path = GetStorageLocationDirectory(ineStorageLocation);

			//get each level of the new directory seperately
			std::string correctedPath = Core::StringUtils::StandardisePath(instrDirectory);
			std::vector<std::string> pathSections = Core::StringUtils::Split(correctedPath, "/");

			//iterate through each section of the path and try and create it.
			for (std::vector<std::string>::iterator it = pathSections.begin(); it != pathSections.end(); ++it)
			{
				path += (*it) + "/";

				s32 error = mkdir(path.c_str(), 0777);
				if (error == -1)
				{
					s32 errorType = errno;
					if (errorType != EEXIST)
					{
						CS_LOG_ERROR("Error creating directory: " + path);
						return false;
					}
				}
			}

			//return successful
			return true;
		}
		//--------------------------------------------------------------
		/// Copy File
		//--------------------------------------------------------------
		bool FileSystem::CopyFile(Core::StorageLocation ineSourceStorageLocation, const std::string& instrSourceFilepath,
					  Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const
		{
			//check the requested source storage location is available
			if (IsStorageLocationAvailable(ineSourceStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested source Storage Location is not available for " + instrSourceFilepath);
				return false;
			}

			//check the requested destination storage location is available
			if (IsStorageLocationAvailable(ineDestinationStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested destination Storage Location is not available for " + instrDestinationFilepath);
				return false;
			}

			//insure that the destination location is writable.
			if (IsStorageLocationWritable(ineDestinationStorageLocation) == false)
			{
				CS_LOG_ERROR("Cannot write to the destination Storage Location!");
				return false;
			}

			//check if we're loading from DLC, and insure that the file exists in the dlc cache. if it does not, fall back on package
			if (ineSourceStorageLocation == Core::StorageLocation::k_package)
			{
				std::string filePath = GetDirectoryForPackageFile(instrSourceFilepath);
				if(filePath.empty())
				{
					CS_LOG_ERROR("Source filepath does not exist! - " + instrSourceFilepath);
					return false;
				}

				return CopyFileFromAPK(filePath, ineDestinationStorageLocation, instrDestinationFilepath);
			}
			else if(ineSourceStorageLocation == Core::StorageLocation::k_DLC && DoesFileExistInDLCCache(instrSourceFilepath) == false)
			{
				std::string filepath = GetDirectoryForPackageFile(mstrPackageDLCPath + instrSourceFilepath);
				if(filepath.empty())
				{
					CS_LOG_ERROR("Source filepath does not exist! - " + instrSourceFilepath);
					return false;
				}

				return CopyFileFromAPK(filepath, ineDestinationStorageLocation, instrDestinationFilepath);
			}
			else
			{
				//check the source file exists
				if(DoesFileExist(GetStorageLocationDirectory(ineSourceStorageLocation) + instrSourceFilepath) == false)
				{
					CS_LOG_ERROR("Source filepath does not exist!");
					return false;
				}

				//get the path to the file
				std::string strPath, strName;
				ChilliSource::Core::StringUtils::SplitFilename(instrDestinationFilepath, strName, strPath);

				//create the output directory
				CreateDirectory(ineDestinationStorageLocation, strPath);

				//try and copy the file
				CopyFile(GetStorageLocationDirectory(ineSourceStorageLocation) + instrSourceFilepath, GetStorageLocationDirectory(ineDestinationStorageLocation) + instrDestinationFilepath);

				return DoesFileExist(GetStorageLocationDirectory(ineDestinationStorageLocation) + instrDestinationFilepath);
			}
		}
		//--------------------------------------------------------------
		/// Copy Directory
		//--------------------------------------------------------------
		bool FileSystem::CopyDirectory(Core::StorageLocation ineSourceStorageLocation, const std::string& instrSourceDirectory,
						   Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationDirectory) const
		{
			//check the requested source storage location is available
			if (IsStorageLocationAvailable(ineSourceStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested source Storage Location is not available for " + instrSourceDirectory);
				return false;
			}

			//check the requested destination storage location is available
			if (IsStorageLocationAvailable(ineDestinationStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested destination Storage Location is not available for " + instrDestinationDirectory);
				return false;
			}

			//insure that the destination location is writable.
			if (IsStorageLocationWritable(ineDestinationStorageLocation) == false)
			{
				CS_LOG_ERROR("Cannot write to the destination Storage Location!");
				return false;
			}

			//get all the files in the directory
			std::vector<std::string> astrFilenames;
			GetFileNamesInDirectory(ineSourceStorageLocation, instrSourceDirectory, true, astrFilenames);

			//error if there are no files
			if (astrFilenames.size() == 0)
			{
				CS_LOG_ERROR("Cannot copy contents of directory as there are no files: " + instrSourceDirectory);
				return false;
			}

			//copy each of these files individually
			std::string strSourceProperPath = Core::StringUtils::StandardisePath(instrSourceDirectory);
			std::string strDestProperPath = Core::StringUtils::StandardisePath(instrDestinationDirectory);
			for (std::vector<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
			{
				if (CopyFile(ineSourceStorageLocation, strSourceProperPath + *it,
						 ineDestinationStorageLocation, strDestProperPath + *it) == false)
				{
					return false;
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		/// Delete File
		//--------------------------------------------------------------
		bool FileSystem::DeleteFile(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available for " + instrFilepath);
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Cannot write to the requested Storage Location!");
				return false;
			}

			//get the filepath
			std::string strPath = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;

			s32 error = unlink(strPath.c_str());
			if (error != 0)
			{
				s32 errorType = errno;
				if (errorType != ENOENT)
				{
					CS_LOG_ERROR("Error deleting: " + instrFilepath);
					return false;
				}
			}

			//return successful
			return true;
		}
		//--------------------------------------------------------------
		/// Delete Directory
		//--------------------------------------------------------------
		bool FileSystem::DeleteDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available for " + instrDirectory);
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Cannot write to the requested Storage Location!");
				return false;
			}

			//get the directory path
			std::string strPath = Core::StringUtils::StandardisePath(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);

			//remove the directory
			DeleteDirectory(strPath);

			//return successful
			return true;
		}
        //------------------------------------------------------------
        /// Get Directory Contents
        ///
        /// @param Directories
        /// @param Recurse into sub directories
        /// @param Out: Content file names
        //------------------------------------------------------------
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
        /// Filter File Names By Extension
        ///
        /// @param Unfiltered names
        /// @param Extension
        /// @param Out: Filtered names
        //--------------------------------------------------------------
        void FilterFileNamesByExtension(const std::vector<std::string> &instrFileNames, const std::string& instrExtension, const std::string& instrDirectory, std::vector<std::string> &outstrFileNames)
        {
        	std::string strDirectory;
        	if(instrDirectory.empty() == false)
        	{
        		strDirectory = ChilliSource::Core::StringUtils::StandardisePath(instrDirectory);
        	}

			for (std::vector<std::string>::const_iterator it = instrFileNames.begin(); it != instrFileNames.end(); ++it)
			{
				if (Core::StringUtils::EndsWith((*it), "." + instrExtension, true) == true)
				{
					outstrFileNames.push_back(strDirectory + *it);
				}
			}
        }
        //--------------------------------------------------------------
        /// Filter File Names By Name
        ///
        /// @param Unfiltered names
        /// @param Name
        /// @param Out: Filtered names
        //--------------------------------------------------------------
        void FilterFileNamesByName(const std::vector<std::string> &instrFileNames, const std::string& instrName, const std::string& instrDirectory, std::vector<std::string> &outstrFileNames)
        {
        	std::string strDirectory;
        	if(instrDirectory.empty() == false)
        	{
        		strDirectory = ChilliSource::Core::StringUtils::StandardisePath(instrDirectory);
        	}

			for (std::vector<std::string>::const_iterator it = instrFileNames.begin(); it != instrFileNames.end(); ++it)
			{
				if (Core::StringUtils::EndsWith((*it), instrName, true) == true)
				{
					outstrFileNames.push_back(strDirectory + *it);
				}
			}
        }
        //--------------------------------------------------------------
        /// Filter File Names By File
        ///
        /// @param Unfiltered names
        /// @param Append directory
        /// @param Out: Filtered names
        //--------------------------------------------------------------
        void FilterFileNamesByFile(const std::vector<std::string> &instrFileNames, const std::string& instrDirectory, std::vector<std::string> &outstrFileNames)
        {
        	std::string strDirectory;
        	if(instrDirectory.empty() == false)
        	{
        		strDirectory = ChilliSource::Core::StringUtils::StandardisePath(instrDirectory);
        	}

			for (std::vector<std::string>::const_iterator it = instrFileNames.begin(); it != instrFileNames.end(); ++it)
			{
				if (Core::StringUtils::Match((*it), "*.*") == true)
				{
					outstrFileNames.push_back(strDirectory + *it);
				}
			}
        }
		//--------------------------------------------------------------
		/// Get File Names With Extension In Directory
		//--------------------------------------------------------------
		void FileSystem::GetFileNamesWithExtensionInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
												  const std::string& instrExtension, std::vector<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);
            
            std::vector<std::string> allItems;
            GetDirectoryContents(aDirectoriesToCheck, inbRecurseIntoSubDirectories, allItems);
            
            inbAppendFullPath ? FilterFileNamesByExtension(allItems, instrExtension, instrDirectory, outstrFileNames) :
            					FilterFileNamesByExtension(allItems, instrExtension, "", outstrFileNames);
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            std::vector<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
            outstrFileNames.resize(it - outstrFileNames.begin()); 
		}
        //--------------------------------------------------------------
        /// Get Path For Files With Name In Directory
        ///
        /// Creates a dynamic array containing the filenames of each of
        /// each file with the given name in the given
        /// directory.
        ///
        /// @param The Storage Location
        /// @param The directory
        /// @param Flag to determine whether or not to recurse into sub directories
        /// @param The name
        /// @param Output dynamic array containing the filenames.
        //--------------------------------------------------------------
        void FileSystem::GetPathForFilesWithNameInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                        const std::string& instrName, std::vector<std::string> &outstrFileNames, bool inbAppendFullPath) const
        {
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);
            
            std::vector<std::string> allItems;
            GetDirectoryContents(aDirectoriesToCheck, inbRecurseIntoSubDirectories, allItems);

            inbAppendFullPath ? FilterFileNamesByName(allItems, instrName, instrDirectory, outstrFileNames) :
            					FilterFileNamesByName(allItems, instrName, "", outstrFileNames);
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            std::vector<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
            outstrFileNames.resize(it - outstrFileNames.begin()); 
        }
		//--------------------------------------------------------------
		/// Get File Names In Directory
		//--------------------------------------------------------------
		void FileSystem::GetFileNamesInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
											 std::vector<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);
            
            std::vector<std::string> allItems;
            GetDirectoryContents(aDirectoriesToCheck, inbRecurseIntoSubDirectories, allItems);
            
            inbAppendFullPath ? FilterFileNamesByFile(allItems, instrDirectory, outstrFileNames) :
            					FilterFileNamesByFile(allItems, "", outstrFileNames);
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            std::vector<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
            outstrFileNames.resize(it - outstrFileNames.begin()); 
		}
		//--------------------------------------------------------------
		/// Get Directories In Directory
		//--------------------------------------------------------------
		void FileSystem::GetDirectoriesInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
											   std::vector<std::string> &outstrDirectories, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);

            std::vector<std::string> allItems;
            GetDirectoryContents(aDirectoriesToCheck, inbRecurseIntoSubDirectories, allItems);
            
            inbAppendFullPath ? FilterFileNamesByFile(allItems, instrDirectory, outstrDirectories) :
            					FilterFileNamesByFile(allItems, "", outstrDirectories);
            
            std::sort(outstrDirectories.begin(), outstrDirectories.end());
            std::vector<std::string>::iterator it = std::unique(outstrDirectories.begin(), outstrDirectories.end());
            outstrDirectories.resize(it - outstrDirectories.begin());
		}
        //------------------------------------------------------------
        /// Get Paths For Storage Location
        ///
        /// @param Storage location
        /// @param File name to append
		/// @param Out: All the paths for the given location
		//------------------------------------------------------------
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
		//--------------------------------------------------------------
		/// Does File Exist
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExist(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return false;
            }
            
            if(ineStorageLocation == Core::StorageLocation::k_package)
            {
            	unz_file_pos sZipPos;
                for(u32 i=0; i<3; ++i)
                {
                	if(DoesFileExistInAPKHashedStore(mastrResourceDirectory[i] + instrFilepath, sZipPos))
                	{
                		return true;
                	}
                }
                
                return false;
            }
            
            //get the filepath
            std::string path = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;
            
            //if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
            if (ineStorageLocation == Core::StorageLocation::k_DLC)
            {
                if (DoesFileExistInDLCCache(instrFilepath) == true)
                {
                    return true;
                }
                
                return DoesFileExist(Core::StorageLocation::k_package, mstrPackageDLCPath + instrFilepath);
            }
            
            //return whether or not the file exists
			return DoesFileExist(ChilliSource::Core::StringUtils::StandardisePath(path));
		}
		//--------------------------------------------------------------
		/// Does File Exist In Cached DLC
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInCachedDLC(const std::string& instrFilepath) const
		{
			return DoesFileExistInDLCCache(instrFilepath);
		}
		//--------------------------------------------------------------
		/// Does File Exist In Package DLC
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInPackageDLC(const std::string& instrFilepath) const
		{
			unz_file_pos sZipPos;
            for(u32 i=0; i<3; ++i)
            {
            	if(DoesFileExistInAPKHashedStore(mastrResourceDirectory[i] + mstrPackageDLCPath + instrFilepath, sZipPos))
            	{
            		return true;
            	}
            }
			return false;
		}
		//--------------------------------------------------------------
		/// Does Directory Exist
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExist(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return false;
            }
            
            if(ineStorageLocation == Core::StorageLocation::k_package)
            {
                for(u32 i=0; i<3; ++i)
                {
                    if(DoesDirectoryExistInAPK(mastrResourceDirectory[i] + "/" + instrDirectory))
                    {
                        return true;
                    }
                }
                
                return false;
            }
            
            //get the filepath
            std::string path = GetStorageLocationDirectory(ineStorageLocation) + instrDirectory;
            
            //if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
            if (ineStorageLocation == Core::StorageLocation::k_DLC)
            {
                if (DoesDirectoryExistInDLCCache(instrDirectory) == true)
                {
                    return true;
                }
                
                return DoesDirectoryExist(Core::StorageLocation::k_package, mstrPackageDLCPath + instrDirectory);
            }
            
            //return whether or not the dir exists
			return DoesDirectoryExist(ChilliSource::Core::StringUtils::StandardisePath(path));
		}
		//--------------------------------------------------------------
		/// Is Storage Location Available
		//--------------------------------------------------------------
		bool FileSystem::IsStorageLocationAvailable(Core::StorageLocation ineStorageLocation) const
		{
			switch (ineStorageLocation)
			{
				case Core::StorageLocation::k_package:
					return true;
				case Core::StorageLocation::k_saveData:
				case Core::StorageLocation::k_cache:
				case Core::StorageLocation::k_DLC:
				case Core::StorageLocation::k_root:
					return mbStorageAvailable;
				default:
					return false;
			}
		}
		//--------------------------------------------------------------
		/// Get Storage Location Directory
		//--------------------------------------------------------------
		std::string FileSystem::GetStorageLocationDirectory(Core::StorageLocation ineStorageLocation) const
		{
			//get the storage location path
			std::string strStorageLocationPath;
			switch (ineStorageLocation)
			{
				case Core::StorageLocation::k_package:
					strStorageLocationPath = kstrAssetsDir;
					break;
				case Core::StorageLocation::k_saveData:
					strStorageLocationPath = mstrStorageDir + kstrSaveDataDir;
					break;
				case Core::StorageLocation::k_cache:
					strStorageLocationPath = mstrStorageDir + kstrCacheDir;
					break;
				case Core::StorageLocation::k_DLC:
					strStorageLocationPath = mstrStorageDir + kstrDLCDir;
					break;
				case Core::StorageLocation::k_root:
					strStorageLocationPath = "";
					break;
				default:
					CS_LOG_ERROR("Storage Location not available on this platform!");
					break;
			}

			return strStorageLocationPath;
		}
        //--------------------------------------------------------------
		/// Is Storage Available
		//--------------------------------------------------------------
		bool FileSystem::IsStorageAvailable() const
		{
			return mbStorageAvailable;
		}
		//--------------------------------------------------------------
		/// Create APK File Stream
		//--------------------------------------------------------------
		Core::FileStreamSPtr FileSystem::CreateAPKFileStream(const unz_file_pos& inPos, Core::FileMode ineFileMode) const
		{
			Core::FileStreamSPtr newFilestream = Core::FileStreamSPtr(new FileStreamAPK(&mMinizipMutex));
			FileStreamAPKSPtr apkStream = std::static_pointer_cast<FileStreamAPK>(newFilestream);
			apkStream->OpenFromAPK(mstrPathToAPK, inPos, ineFileMode);

			return newFilestream;
		}
		//--------------------------------------------------------------
		/// Copy File From APK
		//--------------------------------------------------------------
		bool FileSystem::CopyFileFromAPK(const std::string& instrPath, Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const
		{
			std::unique_lock<std::mutex> lock(mMinizipMutex);

			bool bSuccess = false;
			std::string strSourceFile = ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(Core::StorageLocation::k_package) + instrPath);

			unzFile unzip = unzOpen(mstrPathToAPK.c_str());
			if (unzip != NULL)
			{
				s32 status = unzGoToFirstFile(unzip);

				while (status == UNZ_OK)
				{
					//open file
					if (unzOpenCurrentFile(unzip) != UNZ_OK)
						break;

					//get file information
					unz_file_info info;
					const u32 dwFilenameLength = 256;
					char cfilename[dwFilenameLength];
					unzGetCurrentFileInfo(unzip, &info, cfilename, dwFilenameLength, NULL, 0, NULL, 0);
					std::string strFilepath = ChilliSource::Core::StringUtils::StandardisePath(std::string(cfilename));

					//check and see if this file is in the directory we want
					if (ChilliSource::Core::StringUtils::Match(strFilepath, strSourceFile))
					{
						//create the directory
						std::string strOutputPath, strOutputFilename;
						Core::StringUtils::SplitFilename(instrDestinationFilepath, strOutputFilename, strOutputPath);
						if (CreateDirectory(ineDestinationStorageLocation, strOutputPath) == true)
						{
							//get the filename
							std::string strDestFile = ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(ineDestinationStorageLocation) + instrDestinationFilepath);

							//load the file into memory
							char * dataBuffer = new char[info.uncompressed_size];
							unzReadCurrentFile(unzip, (voidp)dataBuffer, info.uncompressed_size);

							// write the file in the documents directory
							if (CreateFile(ineDestinationStorageLocation, instrDestinationFilepath, dataBuffer, info.uncompressed_size) == true)
							{
								bSuccess = true;
							}
							else
							{
								CS_LOG_ERROR("Failed to create file: " + strDestFile);
							}

							CS_SAFEDELETE_ARRAY(dataBuffer);
						}
						else
						{
							CS_LOG_ERROR("Could not create directory: " + strOutputPath);
						}
					}

					//close file
					unzCloseCurrentFile(unzip);
					status = unzGoToNextFile(unzip);
				}
			}

			unzClose(unzip);
			return bSuccess;
		}
		//--------------------------------------------------------------
		/// Get All Items In Directory In APK
		//--------------------------------------------------------------
		void FileSystem::GetAllItemsInDirectoryInAPK(const std::string& instrPath, bool inbRecurse, std::vector<std::string>& outItems, std::vector<unz_file_pos>& outPos) const
		{
			std::unique_lock<std::mutex> lock(mMinizipMutex);

			//insure the input dir is correctly formatted
			std::string directory = instrPath;
			directory = ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(Core::StorageLocation::k_package) + directory);

			//open apk
			unzFile unzipper = unzOpen(mstrPathToAPK.c_str());
			if (unzipper == NULL)
			{
				CS_LOG_ERROR("Cannot get filenames in package: Failed to open APK.");
				return;
			}

			std::vector<std::string> directoriesViewed;

			//to the first file!
			s32 status = unzGoToFirstFile(unzipper);
			while (status == UNZ_OK)
			{
				//get file information
				unz_file_info info;
				const u32 udwFilenameLength = 256;
				char cfilename[udwFilenameLength];
				unzGetCurrentFileInfo(unzipper, &info, cfilename, udwFilenameLength, NULL, 0, NULL, 0);

				//get the path and filename
				std::string filename(cfilename);
				filename = ChilliSource::Core::StringUtils::StandardisePath(filename);
				std::string path = filename.substr(0, filename.rfind("/") + 1);

				//if this file is at the same path as requested, then add it to the output
				if ((inbRecurse == false && path == directory) || (inbRecurse == true && Core::StringUtils::Match(filename, directory + "*") == true))
				{
					filename = filename.erase(0, directory.size());
					path = filename.substr(0, filename.rfind("/") + 1);

					//check to see if this directory has previously been seen. If it has not, add it.
					if (path.size() != 0)
					{
						bool bDirectoryAlreadySeen = false;
						for (std::vector<std::string>::iterator it = directoriesViewed.begin(); it != directoriesViewed.end(); ++it)
						{
							if ((*it) == path)
								bDirectoryAlreadySeen = false;
						}

						if (bDirectoryAlreadySeen == false)
						{
							unz_file_pos pos;
							unzGetFilePos(unzipper, &pos);
							outPos.push_back(pos);
							outItems.push_back(path);
							directoriesViewed.push_back(path);
						}
					}

					unz_file_pos pos;
					unzGetFilePos(unzipper, &pos);
					outPos.push_back(pos);
					outItems.push_back(filename);
				}

				//onto the next file!
				status = unzGoToNextFile(unzipper);
			}
			unzClose(unzipper);
		}
		//--------------------------------------------------------------
		/// Does File Exist In APK
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInAPKHashedStore(const std::string& instrPath, unz_file_pos& outZipPos) const
		{
            u32 udwHashedFile = Core::HashCRC32::GenerateHashCode(instrPath);

            APKFileInfo SearchItem;
            SearchItem.udwFileNameHash = udwHashedFile;

            std::vector<APKFileInfo>::const_iterator it = std::lower_bound(mAPKFileInfo.begin(), mAPKFileInfo.end(), SearchItem, FileInfoSortPredicate);

            if(it !=  mAPKFileInfo.end() && it->udwFileNameHash == udwHashedFile)
            {
            	outZipPos = it->sZipPos;
            	return true;
            }

            return false;
		}
		//--------------------------------------------------------------
		/// Does Directory Exist In APK
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInAPK(const std::string& instrPath) const
		{
			std::unique_lock<std::mutex> lock(mMinizipMutex);

			bool bResult = false;

			//insure the input dir is correctly formatted
			std::string strDirectory = instrPath;
			strDirectory = ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(Core::StorageLocation::k_package) + strDirectory);

			//open apk
			unzFile unzipper = unzOpen(mstrPathToAPK.c_str());
			if (unzipper == NULL)
			{
				CS_LOG_ERROR("Cannot get filenames in package: Failed to open APK.");
				return false;
			}

			//to the first file!
			s32 status = unzGoToFirstFile(unzipper);
			while (status == UNZ_OK && bResult == false)
			{
				//get file information
				unz_file_info info;
				const u32 udwFilenameLength = 256;
				char cfilename[udwFilenameLength];
				unzGetCurrentFileInfo(unzipper, &info, cfilename, udwFilenameLength, NULL, 0, NULL, 0);

				//get the path and filename
				std::string strFilename(cfilename);
				strFilename = ChilliSource::Core::StringUtils::StandardisePath(strFilename);

				// if this filename contains the path to the requested directory
				if (Core::StringUtils::Match(strFilename, strDirectory + "*") == true)
				{
					bResult = true;
				}

				//onto the next file!
				status = unzGoToNextFile(unzipper);
			}
			unzClose(unzipper);

			return bResult;
		}
		//--------------------------------------------------------------
		/// Create Base Directory
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
		/// Copy File
		//--------------------------------------------------------------
		bool FileSystem::CopyFile(const std::string& instrSourceFilepath, const std::string& instrDestinationFilepath) const
		{
			const s32 kdwChunkSize = 250 * 1024; //250Kb chunk size

			//open the source file
			Core::FileStreamSPtr sourceStream = Core::FileStreamSPtr(new Core::FileStream());
			sourceStream->Open(instrSourceFilepath, Core::FileMode::k_readBinary);
			if (sourceStream.get() == NULL || sourceStream->IsBad() == true || sourceStream->IsOpen() == false)
				return false;

			//open the source file
			Core::FileStreamSPtr destinationStream = Core::FileStreamSPtr(new Core::FileStream());
			destinationStream->Open(instrDestinationFilepath, Core::FileMode::k_writeBinary);
			if (destinationStream.get() == NULL || destinationStream->IsBad() == true || destinationStream->IsOpen() == false)
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
		/// Delete Directory
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
			if(directory == NULL)
			{
				return false;
			}

			//iterate through the directory
			struct dirent* directoryItem;
			while ((directoryItem = readdir(directory)) != NULL)
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
		/// Get All Items In Directory
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
			if(directory == NULL)
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
			while ((directoryItem = readdir(directory)) != NULL)
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
        /// Get Directory For DLC File
        ///
        /// @param The filename of the DLC asset.
        /// @return The directory to either the package DLC or cache DLC.
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
        /// Get Directory For Package File
        ///
        /// @param The filename of the package asset.
        /// @return The directory to either the correct device package directory.
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
        /// Get Zip Pos For Package File
        ///
        /// @param The filename of the package asset.
        /// @return The zip pos to the package file
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
		/// Does File Exist
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
		/// Does Directory Exist
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
		/// Does File Exist In DLC Cache
		//--------------------------------------------------------------
		bool FileSystem::DoesFileExistInDLCCache(const std::string& instrPath) const
		{
			//Check that this storage location is available
			if (IsStorageLocationAvailable(Core::StorageLocation::k_DLC) == false)
			{
				CS_LOG_ERROR("DLC Storage Location is not available!");
				return false;
			}
			std::string filepath = GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrPath;
			return DoesFileExist(filepath);
		}
		//--------------------------------------------------------------
		/// Does Directory Exist In DLC Cache
		//--------------------------------------------------------------
		bool FileSystem::DoesDirectoryExistInDLCCache(const std::string& instrPath) const
		{
			//Check that this storage location is available
			if (IsStorageLocationAvailable(Core::StorageLocation::k_DLC) == false)
			{
				CS_LOG_ERROR("DLC Storage Location is not available!");
				return false;
			}
			std::string filepath = GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrPath;
			return DoesDirectoryExist(filepath);
		}
	}
}
