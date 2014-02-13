/*
 *  FileSystem.cpp
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Platform/Android/FileIO/FileSystem.h>
#include <ChilliSource/Core/Utils.h>
#include <ChilliSource/Core/FileIO/FileStream.h>
#include <ChilliSource/Platform/Android/FileIO/FileStreamAndroidAPK.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Platform/Android/JavaInterface/CoreJavaInterface.h>
#include <ChilliSource/Core/StringUtils.h>
#include <ChilliSource/Core/Application.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <cstdio>
#include <jni.h>

namespace moFlo
{
	namespace AndroidPlatform 
	{
		const std::string kstrAssetsDir = "assets/";
		const std::string kstrSaveDataDir = "files/SaveData/";
		const std::string kstrDLCDir = "cache/DLC/";
		const std::string kstrCacheDir = "cache/Cache/";

		CFileSystem::CFileSystem()
		{
			CoreJavaInterfacePtr pCoreJI = CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CCoreJavaInterface>();

			//get the path to the bundle dir
			mstrPathToAPK = pCoreJI->GetAPKDirectory();

			//get the storage directory
			std::string externalStorage = pCoreJI->GetExternalStorageDirectory();
			if (externalStorage != "")
			{
				//if external storage is available
				std::string packageName = pCoreJI->GetPackageName();
				mstrStorageDir = Core::CStringUtils::StandardisePath(externalStorage + "/Android/data/" + packageName + "/");

				//Create the directories
				CreateBaseDirectory(GetStorageLocationDirectory(Core::SL_SAVEDATA));
				CreateBaseDirectory(GetStorageLocationDirectory(Core::SL_CACHE));
				CreateBaseDirectory(GetStorageLocationDirectory(Core::SL_DLC));

				mbStorageAvailable = true;
			}
			else
			{
				//if neither is available
				FATAL_LOG("Cannot access external storage!");
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
        void CFileSystem::CreateHashedAPKFileList()
        {
        	DYNAMIC_ARRAY<std::string> aContent;
        	DYNAMIC_ARRAY<unz_file_pos> aZipPos;
        	GetAllItemsInDirectoryInAPK("", true, aContent, aZipPos);

        	APKFileInfo sInfo;
        	u32 i=0;
        	for(DYNAMIC_ARRAY<std::string>::const_iterator it = aContent.begin(); it != aContent.end(); ++it, ++i)
        	{
        		sInfo.udwFileNameHash = CHashCRC32::GenerateHashCode(*it);
        		sInfo.sZipPos = aZipPos[i];
        		mAPKFileInfo.push_back(sInfo);
        	}

            std::sort(mAPKFileInfo.begin(), mAPKFileInfo.end(), FileInfoSortPredicate);
        }
		//--------------------------------------------------------------
		/// Create File Stream
		//--------------------------------------------------------------
		Core::FileStreamPtr CFileSystem::CreateFileStream(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilepath, Core::FILE_MODE ineFileMode) const
		{
			Core::FileStreamPtr newFilestream = Core::FileStreamPtr(new Core::IFileStream());

			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				ERROR_LOG("Requested Storage Location is not available for " + instrFilepath);
				return newFilestream;
			}

			std::string filepath = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;

			//if this is not a read stream, insure that the storage location is writable.
			if (ineFileMode != Core::FM_READ && ineFileMode != Core::FM_READ_BINARY)
			{
                if(IsStorageLocationWritable(ineStorageLocation) == true)
                {
                    newFilestream->Open(filepath, ineFileMode);
                }
                else
                {
                    ERROR_LOG("Cannot write to the requested Storage Location!");
                }

                return newFilestream;
			}

			if (ineStorageLocation == Core::SL_PACKAGE)
			{
				unz_file_pos ZipPos = GetZipPosForPackageFile(instrFilepath);
				return CreateAPKFileStream(ZipPos, ineFileMode);
			}

			//if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
			if (ineStorageLocation == Core::SL_DLC && DoesFileExistInDLCCache(instrFilepath) == false)
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
		bool CFileSystem::CreateFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory, s8* inpbyData, u32 inudwDataSize) const
		{
			Core::FileStreamPtr pFileStream = CreateFileStream(ineStorageLocation, instrDirectory, Core::FM_WRITE_BINARY);

			if (pFileStream.get() == NULL || pFileStream->IsOpen() == false || pFileStream->IsBad() == true)
			{
				ERROR_LOG("Failed to create file: " + instrDirectory);
				return false;
			}

			pFileStream->Write(inpbyData, (s32)inudwDataSize);
			pFileStream->Close();
			return true;
		}
		//--------------------------------------------------------------
		/// Create Directory
		//--------------------------------------------------------------
		bool CFileSystem::CreateDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				ERROR_LOG("Requested Storage Location is not available for " + instrDirectory);
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				ERROR_LOG("Cannot write to the requested Storage Location " + instrDirectory);
				return false;
			}

			//create the directory
			std::string path = GetStorageLocationDirectory(ineStorageLocation);

			//get each level of the new directory seperately
			std::string correctedPath = Core::CStringUtils::StandardisePath(instrDirectory);
			DYNAMIC_ARRAY<std::string> pathSections = Core::CStringUtils::Split(correctedPath, "/");

			//iterate through each section of the path and try and create it.
			for (DYNAMIC_ARRAY<std::string>::iterator it = pathSections.begin(); it != pathSections.end(); ++it)
			{
				path += (*it) + "/";

				s32 error = mkdir(path.c_str(),0777);
				if (error == -1)
				{
					s32 errorType = errno;
					if (errorType != EEXIST)
					{
						ERROR_LOG("Error creating directory: " + path);
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
		bool CFileSystem::CopyFile(Core::STORAGE_LOCATION ineSourceStorageLocation, const std::string& instrSourceFilepath,
					  Core::STORAGE_LOCATION ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const
		{
			//check the requested source storage location is available
			if (IsStorageLocationAvailable(ineSourceStorageLocation) == false)
			{
				ERROR_LOG("Requested source Storage Location is not available for " + instrSourceFilepath);
				return false;
			}

			//check the requested destination storage location is available
			if (IsStorageLocationAvailable(ineDestinationStorageLocation) == false)
			{
				ERROR_LOG("Requested destination Storage Location is not available for " + instrDestinationFilepath);
				return false;
			}

			//insure that the destination location is writable.
			if (IsStorageLocationWritable(ineDestinationStorageLocation) == false)
			{
				ERROR_LOG("Cannot write to the destination Storage Location!");
				return false;
			}

			//check if we're loading from DLC, and insure that the file exists in the dlc cache. if it does not, fall back on package
			if (ineSourceStorageLocation == Core::SL_PACKAGE)
			{
				std::string filePath = GetDirectoryForPackageFile(instrSourceFilepath);
				if(filePath.empty())
				{
					ERROR_LOG("Source filepath does not exist! - " + instrSourceFilepath);
					return false;
				}

				return CopyFileFromAPK(filePath, ineDestinationStorageLocation, instrDestinationFilepath);
			}
			else if(ineSourceStorageLocation == Core::SL_DLC && DoesFileExistInDLCCache(instrSourceFilepath) == false)
			{
				std::string filepath = GetDirectoryForPackageFile(mstrPackageDLCPath + instrSourceFilepath);
				if(filepath.empty())
				{
					ERROR_LOG("Source filepath does not exist! - " + instrSourceFilepath);
					return false;
				}

				return CopyFileFromAPK(filepath, ineDestinationStorageLocation, instrDestinationFilepath);
			}
			else
			{
				//check the source file exists
				if(DoesFileExist(GetStorageLocationDirectory(ineSourceStorageLocation) + instrSourceFilepath) == false)
				{
					ERROR_LOG("Source filepath does not exist!");
					return false;
				}

				//get the path to the file
				std::string strPath, strName;
				moFlo::Core::CStringUtils::SplitFilename(instrDestinationFilepath, strName, strPath);

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
		bool CFileSystem::CopyDirectory(Core::STORAGE_LOCATION ineSourceStorageLocation, const std::string& instrSourceDirectory,
						   Core::STORAGE_LOCATION ineDestinationStorageLocation, const std::string& instrDestinationDirectory) const
		{
			//check the requested source storage location is available
			if (IsStorageLocationAvailable(ineSourceStorageLocation) == false)
			{
				ERROR_LOG("Requested source Storage Location is not available for " + instrSourceDirectory);
				return false;
			}

			//check the requested destination storage location is available
			if (IsStorageLocationAvailable(ineDestinationStorageLocation) == false)
			{
				ERROR_LOG("Requested destination Storage Location is not available for " + instrDestinationDirectory);
				return false;
			}

			//insure that the destination location is writable.
			if (IsStorageLocationWritable(ineDestinationStorageLocation) == false)
			{
				ERROR_LOG("Cannot write to the destination Storage Location!");
				return false;
			}

			//get all the files in the directory
			DYNAMIC_ARRAY<std::string> astrFilenames;
			GetFileNamesInDirectory(ineSourceStorageLocation, instrSourceDirectory, true, astrFilenames);

			//error if there are no files
			if (astrFilenames.size() == 0)
			{
				ERROR_LOG("Cannot copy contents of directory as there are no files: " + instrSourceDirectory);
				return false;
			}

			//copy each of these files individually
			std::string strSourceProperPath = Core::CStringUtils::StandardisePath(instrSourceDirectory);
			std::string strDestProperPath = Core::CStringUtils::StandardisePath(instrDestinationDirectory);
			for (DYNAMIC_ARRAY<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
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
		bool CFileSystem::DeleteFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilepath) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				ERROR_LOG("Requested Storage Location is not available for " + instrFilepath);
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				ERROR_LOG("Cannot write to the requested Storage Location!");
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
					ERROR_LOG("Error deleting: " + instrFilepath);
					return false;
				}
			}

			//return successful
			return true;
		}
		//--------------------------------------------------------------
		/// Delete Directory
		//--------------------------------------------------------------
		bool CFileSystem::DeleteDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				ERROR_LOG("Requested Storage Location is not available for " + instrDirectory);
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				ERROR_LOG("Cannot write to the requested Storage Location!");
				return false;
			}

			//get the directory path
			std::string strPath = Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);

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
        void CFileSystem::GetDirectoryContents(const DYNAMIC_ARRAY<DirInfo>& inaDirs, bool inbRecursive, DYNAMIC_ARRAY<std::string>& outstrContents) const
		{
        	DYNAMIC_ARRAY<unz_file_pos> aZipPos;
            for(DYNAMIC_ARRAY<DirInfo>::const_iterator it = inaDirs.begin(); it != inaDirs.end(); ++it)
            {
            	aZipPos.clear();
                std::string path = moFlo::Core::CStringUtils::StandardisePath(it->strPath);

                if(it->eLocation == Core::SL_PACKAGE)
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
        void FilterFileNamesByExtension(const DYNAMIC_ARRAY<std::string> &instrFileNames, const std::string& instrExtension, const std::string& instrDirectory, DYNAMIC_ARRAY<std::string> &outstrFileNames)
        {
        	std::string strDirectory;
        	if(instrDirectory.empty() == false)
        	{
        		strDirectory = moFlo::Core::CStringUtils::StandardisePath(instrDirectory);
        	}

			for (DYNAMIC_ARRAY<std::string>::const_iterator it = instrFileNames.begin(); it != instrFileNames.end(); ++it)
			{
				if (Core::CStringUtils::EndsWith((*it), "." + instrExtension, true) == true)
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
        void FilterFileNamesByName(const DYNAMIC_ARRAY<std::string> &instrFileNames, const std::string& instrName, const std::string& instrDirectory, DYNAMIC_ARRAY<std::string> &outstrFileNames)
        {
        	std::string strDirectory;
        	if(instrDirectory.empty() == false)
        	{
        		strDirectory = moFlo::Core::CStringUtils::StandardisePath(instrDirectory);
        	}

			for (DYNAMIC_ARRAY<std::string>::const_iterator it = instrFileNames.begin(); it != instrFileNames.end(); ++it)
			{
				if (Core::CStringUtils::EndsWith((*it), instrName, true) == true)
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
        void FilterFileNamesByFile(const DYNAMIC_ARRAY<std::string> &instrFileNames, const std::string& instrDirectory, DYNAMIC_ARRAY<std::string> &outstrFileNames)
        {
        	std::string strDirectory;
        	if(instrDirectory.empty() == false)
        	{
        		strDirectory = moFlo::Core::CStringUtils::StandardisePath(instrDirectory);
        	}

			for (DYNAMIC_ARRAY<std::string>::const_iterator it = instrFileNames.begin(); it != instrFileNames.end(); ++it)
			{
				if (Core::CStringUtils::Match((*it), "*.*") == true)
				{
					outstrFileNames.push_back(strDirectory + *it);
				}
			}
        }
		//--------------------------------------------------------------
		/// Get File Names With Extension In Directory
		//--------------------------------------------------------------
		void CFileSystem::GetFileNamesWithExtensionInDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
												  const std::string& instrExtension, DYNAMIC_ARRAY<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return;
            }
            
            DYNAMIC_ARRAY<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);
            
            DYNAMIC_ARRAY<std::string> allItems;
            GetDirectoryContents(aDirectoriesToCheck, inbRecurseIntoSubDirectories, allItems);
            
            inbAppendFullPath ? FilterFileNamesByExtension(allItems, instrExtension, instrDirectory, outstrFileNames) :
            					FilterFileNamesByExtension(allItems, instrExtension, "", outstrFileNames);
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            DYNAMIC_ARRAY<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
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
        void CFileSystem::GetPathForFilesWithNameInDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
                                                        const std::string& instrName, DYNAMIC_ARRAY<std::string> &outstrFileNames, bool inbAppendFullPath) const
        {
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return;
            }
            
            DYNAMIC_ARRAY<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);
            
            DYNAMIC_ARRAY<std::string> allItems;
            GetDirectoryContents(aDirectoriesToCheck, inbRecurseIntoSubDirectories, allItems);

            inbAppendFullPath ? FilterFileNamesByName(allItems, instrName, instrDirectory, outstrFileNames) :
            					FilterFileNamesByName(allItems, instrName, "", outstrFileNames);
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            DYNAMIC_ARRAY<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
            outstrFileNames.resize(it - outstrFileNames.begin()); 
        }
		//--------------------------------------------------------------
		/// Get File Names In Directory
		//--------------------------------------------------------------
		void CFileSystem::GetFileNamesInDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
											 DYNAMIC_ARRAY<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return;
            }
            
            DYNAMIC_ARRAY<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);
            
            DYNAMIC_ARRAY<std::string> allItems;
            GetDirectoryContents(aDirectoriesToCheck, inbRecurseIntoSubDirectories, allItems);
            
            inbAppendFullPath ? FilterFileNamesByFile(allItems, instrDirectory, outstrFileNames) :
            					FilterFileNamesByFile(allItems, "", outstrFileNames);
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            DYNAMIC_ARRAY<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
            outstrFileNames.resize(it - outstrFileNames.begin()); 
		}
		//--------------------------------------------------------------
		/// Get Directories In Directory
		//--------------------------------------------------------------
		void CFileSystem::GetDirectoriesInDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory,  bool inbRecurseIntoSubDirectories,
											   DYNAMIC_ARRAY<std::string> &outstrDirectories, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return;
            }
            
            DYNAMIC_ARRAY<DirInfo> aDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, aDirectoriesToCheck);

            DYNAMIC_ARRAY<std::string> allItems;
            GetDirectoryContents(aDirectoriesToCheck, inbRecurseIntoSubDirectories, allItems);
            
            inbAppendFullPath ? FilterFileNamesByFile(allItems, instrDirectory, outstrDirectories) :
            					FilterFileNamesByFile(allItems, "", outstrDirectories);
            
            std::sort(outstrDirectories.begin(), outstrDirectories.end());
            DYNAMIC_ARRAY<std::string>::iterator it = std::unique(outstrDirectories.begin(), outstrDirectories.end());
            outstrDirectories.resize(it - outstrDirectories.begin());
		}
        //------------------------------------------------------------
        /// Get Paths For Storage Location
        ///
        /// @param Storage location
        /// @param File name to append
		/// @param Out: All the paths for the given location
		//------------------------------------------------------------
		void CFileSystem::GetPathsForStorageLocation(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFileName, DYNAMIC_ARRAY<DirInfo>& outaPaths) const
		{
			switch(ineStorageLocation)
			{
			case Core::SL_PACKAGE:
				for(u32 i=0; i<3; ++i)
				{
					DirInfo Info;
					Info.strPath = mastrResourceDirectory[i] + instrFileName;
					Info.eLocation = Core::SL_PACKAGE;
					outaPaths.push_back(Info);
				}
				break;
			case Core::SL_DLC:
			{
				for(u32 i=0; i<3; ++i)
				{
					DirInfo Info;
					Info.strPath = mastrResourceDirectory[i] + mstrPackageDLCPath + instrFileName;
					Info.eLocation = Core::SL_PACKAGE;
					outaPaths.push_back(Info);
				}

				DirInfo Info;
				Info.strPath = GetStorageLocationDirectory(Core::SL_DLC) + instrFileName;
				Info.eLocation = Core::SL_DLC;
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
		bool CFileSystem::DoesFileExist(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilepath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return false;
            }
            
            if(ineStorageLocation == Core::SL_PACKAGE)
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
            if (ineStorageLocation == Core::SL_DLC)
            {
                if (DoesFileExistInDLCCache(instrFilepath) == true)
                {
                    return true;
                }
                
                return DoesFileExist(Core::SL_PACKAGE, mstrPackageDLCPath + instrFilepath);
            }
            
            //return whether or not the file exists
			return DoesFileExist(moFlo::Core::CStringUtils::StandardisePath(path));
		}
		//--------------------------------------------------------------
		/// Does File Exist In Cached DLC
		//--------------------------------------------------------------
		bool CFileSystem::DoesFileExistInCachedDLC(const std::string& instrFilepath) const
		{
			return DoesFileExistInDLCCache(instrFilepath);
		}
		//--------------------------------------------------------------
		/// Does File Exist In Package DLC
		//--------------------------------------------------------------
		bool CFileSystem::DoesFileExistInPackageDLC(const std::string& instrFilepath) const
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
		bool CFileSystem::DoesDirectoryExist(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return false;
            }
            
            if(ineStorageLocation == Core::SL_PACKAGE)
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
            if (ineStorageLocation == Core::SL_DLC)
            {
                if (DoesDirectoryExistInDLCCache(instrDirectory) == true)
                {
                    return true;
                }
                
                return DoesDirectoryExist(Core::SL_PACKAGE, mstrPackageDLCPath + instrDirectory);
            }
            
            //return whether or not the dir exists
			return DoesDirectoryExist(moFlo::Core::CStringUtils::StandardisePath(path));
		}
		//--------------------------------------------------------------
		/// Is Storage Location Available
		//--------------------------------------------------------------
		bool CFileSystem::IsStorageLocationAvailable(Core::STORAGE_LOCATION ineStorageLocation) const
		{
			switch (ineStorageLocation)
			{
				case Core::SL_PACKAGE:
					return true;
				case Core::SL_SAVEDATA:
				case Core::SL_CACHE:
				case Core::SL_DLC:
				case Core::SL_ROOT:
					return mbStorageAvailable;
				default:
					return false;
			}
		}
		//--------------------------------------------------------------
		/// Get Storage Location Directory
		//--------------------------------------------------------------
		std::string CFileSystem::GetStorageLocationDirectory(Core::STORAGE_LOCATION ineStorageLocation) const
		{
			//get the storage location path
			std::string strStorageLocationPath;
			switch (ineStorageLocation)
			{
				case Core::SL_PACKAGE:
					strStorageLocationPath = kstrAssetsDir;
					break;
				case Core::SL_SAVEDATA:
					strStorageLocationPath = mstrStorageDir + kstrSaveDataDir;
					break;
				case Core::SL_CACHE:
					strStorageLocationPath = mstrStorageDir + kstrCacheDir;
					break;
				case Core::SL_DLC:
					strStorageLocationPath = mstrStorageDir + kstrDLCDir;
					break;
				case Core::SL_ROOT:
					strStorageLocationPath = "";
					break;
				default:
					ERROR_LOG("Storage Location not available on this platform!");
					break;
			}

			return strStorageLocationPath;
		}
        //--------------------------------------------------------------
		/// Is Storage Available
		//--------------------------------------------------------------
		bool CFileSystem::IsStorageAvailable() const
		{
			return mbStorageAvailable;
		}
		//--------------------------------------------------------------
		/// Create APK File Stream
		//--------------------------------------------------------------
		Core::FileStreamPtr CFileSystem::CreateAPKFileStream(const unz_file_pos& inPos, Core::FILE_MODE ineFileMode) const
		{
			Core::FileStreamPtr newFilestream = Core::FileStreamPtr(new CFileStreamAPK(&mMinizipMutex));
			SHARED_PTR<CFileStreamAPK> apkStream = SHARED_PTR_CAST<CFileStreamAPK>(newFilestream);
			apkStream->OpenFromAPK(mstrPathToAPK, inPos, ineFileMode);

			return newFilestream;
		}
		//--------------------------------------------------------------
		/// Copy File From APK
		//--------------------------------------------------------------
		bool CFileSystem::CopyFileFromAPK(const std::string& instrPath, Core::STORAGE_LOCATION ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const
		{
			CThread::ScopedLock lock(mMinizipMutex);

			bool bSuccess = false;
			std::string strSourceFile = moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_PACKAGE) + instrPath);

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
					std::string strFilepath = moFlo::Core::CStringUtils::StandardisePath(std::string(cfilename));

					//check and see if this file is in the directory we want
					if (moFlo::Core::CStringUtils::Match(strFilepath, strSourceFile))
					{
						//create the directory
						std::string strOutputPath, strOutputFilename;
						Core::CStringUtils::SplitFilename(instrDestinationFilepath, strOutputFilename, strOutputPath);
						if (CreateDirectory(ineDestinationStorageLocation, strOutputPath) == true)
						{
							//get the filename
							std::string strDestFile = moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(ineDestinationStorageLocation) + instrDestinationFilepath);

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
								ERROR_LOG("Failed to create file: " + strDestFile);
							}

							SAFE_DELETE_ARRAY(dataBuffer);
						}
						else
						{
							ERROR_LOG("Could not create directory: " + strOutputPath);
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
		void CFileSystem::GetAllItemsInDirectoryInAPK(const std::string& instrPath, bool inbRecurse, DYNAMIC_ARRAY<std::string>& outItems, DYNAMIC_ARRAY<unz_file_pos>& outPos) const
		{
			CThread::ScopedLock lock(mMinizipMutex);

			//insure the input dir is correctly formatted
			std::string directory = instrPath;
			directory = moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_PACKAGE) + directory);

			//open apk
			unzFile unzipper = unzOpen(mstrPathToAPK.c_str());
			if (unzipper == NULL)
			{
				ERROR_LOG("Cannot get filenames in package: Failed to open APK.");
				return;
			}

			DYNAMIC_ARRAY<std::string> directoriesViewed;

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
				filename = moFlo::Core::CStringUtils::StandardisePath(filename);
				std::string path = filename.substr(0, filename.rfind("/") + 1);

				//if this file is at the same path as requested, then add it to the output
				if ((inbRecurse == false && path == directory) || (inbRecurse == true && Core::CStringUtils::Match(filename, directory + "*") == true))
				{
					filename = filename.erase(0, directory.size());
					path = filename.substr(0, filename.rfind("/") + 1);

					//check to see if this directory has previously been seen. If it has not, add it.
					if (path.size() != 0)
					{
						bool bDirectoryAlreadySeen = false;
						for (DYNAMIC_ARRAY<std::string>::iterator it = directoriesViewed.begin(); it != directoriesViewed.end(); ++it)
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
		bool CFileSystem::DoesFileExistInAPKHashedStore(const std::string& instrPath, unz_file_pos& outZipPos) const
		{
            u32 udwHashedFile = CHashCRC32::GenerateHashCode(instrPath);

            APKFileInfo SearchItem;
            SearchItem.udwFileNameHash = udwHashedFile;

            DYNAMIC_ARRAY<APKFileInfo>::const_iterator it = std::lower_bound(mAPKFileInfo.begin(), mAPKFileInfo.end(), SearchItem, FileInfoSortPredicate);

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
		bool CFileSystem::DoesDirectoryExistInAPK(const std::string& instrPath) const
		{
			CThread::ScopedLock lock(mMinizipMutex);

			bool bResult = false;

			//insure the input dir is correctly formatted
			std::string strDirectory = instrPath;
			strDirectory = moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_PACKAGE) + strDirectory);

			//open apk
			unzFile unzipper = unzOpen(mstrPathToAPK.c_str());
			if (unzipper == NULL)
			{
				ERROR_LOG("Cannot get filenames in package: Failed to open APK.");
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
				strFilename = moFlo::Core::CStringUtils::StandardisePath(strFilename);

				// if this filename contains the path to the requested directory
				if (Core::CStringUtils::Match(strFilename, strDirectory + "*") == true)
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
		bool CFileSystem::CreateBaseDirectory(const std::string& instrDirectory) const
		{
			std::string correctedPath = Core::CStringUtils::StandardisePath(instrDirectory);
			DYNAMIC_ARRAY<std::string> pathSections = Core::CStringUtils::Split(correctedPath, "/");

			//iterate through each section of the path and try and create it.
			std::string path = "";
			for (DYNAMIC_ARRAY<std::string>::iterator it = pathSections.begin(); it != pathSections.end(); ++it)
			{
				path += (*it) + "/";

				s32 error = mkdir(path.c_str(),0777);
				if (error != 0)
				{
					s32 errorType = errno;
					if (errorType != EEXIST)
					{
						ERROR_LOG("Error creating directory: " + path);
						return false;
					}
				}
			}

			return true;
		}
		//--------------------------------------------------------------
		/// Copy File
		//--------------------------------------------------------------
		bool CFileSystem::CopyFile(const std::string& instrSourceFilepath, const std::string& instrDestinationFilepath) const
		{
			const s32 kdwChunkSize = 250 * 1024; //250Kb chunk size

			//open the source file
			Core::FileStreamPtr sourceStream = Core::FileStreamPtr(new Core::IFileStream());
			sourceStream->Open(instrSourceFilepath, Core::FM_READ_BINARY);
			if (sourceStream.get() == NULL || sourceStream->IsBad() == true || sourceStream->IsOpen() == false)
				return false;

			//open the source file
			Core::FileStreamPtr destinationStream = Core::FileStreamPtr(new Core::IFileStream());
			destinationStream->Open(instrDestinationFilepath, Core::FM_WRITE_BINARY);
			if (destinationStream.get() == NULL || destinationStream->IsBad() == true || destinationStream->IsOpen() == false)
				return false;

			//find the length of the source stream
			sourceStream->SeekG(0, Core::SD_END);
			s32 dwLength = sourceStream->TellG();
			sourceStream->SeekG(0, Core::SD_BEGINNING);

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
		bool CFileSystem::DeleteDirectory(const std::string& instrDirectory) const
		{
			//insure the path is in the correct format
			std::string strCorrectedDirectory = Core::CStringUtils::StandardisePath(instrDirectory);

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
				std::string strFullDirectory = strCorrectedDirectory + Core::CStringUtils::StandardisePath(directoryItem->d_name) + "\0";
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
		void CFileSystem::GetAllItemsInDirectory(const std::string& instrPath, bool inbRecurse, const std::string& instrPathSoFar, DYNAMIC_ARRAY<std::string>& outItems) const
		{
			//insure the path is in the correct format
			std::string strCorrectedDirectory = Core::CStringUtils::StandardisePath(instrPath);

			//this has the potential to have a path with a dot in it - make sure that it will always have a "/" on the end.
			if (strCorrectedDirectory[strCorrectedDirectory.size() - 1] != '/')
				strCorrectedDirectory += '/';

			//check the directory exists
			if (DoesDirectoryExist(strCorrectedDirectory) == false)
			{
				//ERROR_LOG("Directory does not exist: " + strCorrectedDirectory);
				return;
			}

			//open up the directory
			DIR* directory = opendir(strCorrectedDirectory.c_str());
			if(directory == NULL)
			{
				s32 errorType = errno;

				if(errorType == EACCES)
					ERROR_LOG("Access Permission Error");
				else if(errorType == ELOOP)
					ERROR_LOG("Symbolic Link Error");
				else if(errorType == ENAMETOOLONG)
					ERROR_LOG("Name Too Long Error");
				else if(errorType == ENOENT)
					ERROR_LOG("Dirname non existing/Dirname Empty String Error");
				else if(errorType == ENOTDIR)
					ERROR_LOG("Dirname non existing Error");
				else if(errorType == EMFILE)
					ERROR_LOG("To many files in Program Opened Error");
				else if(errorType == ENAMETOOLONG)
					ERROR_LOG("Nametolong Error");
				else if(errorType == ENFILE)
					ERROR_LOG("To many files in System Opened Error");

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
				std::string strFullDirectory = strCorrectedDirectory + Core::CStringUtils::StandardisePath(directoryItem->d_name) + "\0";
				if (stat(strFullDirectory.c_str(), &itemStats) == 0)
				{
					if (S_ISDIR(itemStats.st_mode) == true)
						GetAllItemsInDirectory(strFullDirectory, inbRecurse, Core::CStringUtils::StandardisePath(instrPathSoFar) + Core::CStringUtils::StandardisePath(directoryItem->d_name), outItems);
				}
				else
				{
					ERROR_LOG("Error: " + strFullDirectory);
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
        std::string CFileSystem::GetDirectoryForDLCFile(const std::string& instrFilePath) const
        {
        	if(DoesFileExistInCachedDLC(instrFilePath))
        	{
        		return moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_DLC) + instrFilePath);
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
        std::string CFileSystem::GetDirectoryForPackageFile(const std::string& instrFilePath) const
        {
            std::string strResult;
            unz_file_pos sZipPos;
            for(u32 i=0; i<3; ++i)
            {
                std::string strPath = moFlo::Core::CStringUtils::StandardisePath(mastrResourceDirectory[i] + instrFilePath);

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
        unz_file_pos CFileSystem::GetZipPosForPackageFile(const std::string& instrFilePath) const
        {
        	unz_file_pos Result;

            for(u32 i=0; i<3; ++i)
            {
                std::string strPath = moFlo::Core::CStringUtils::StandardisePath(mastrResourceDirectory[i] + instrFilePath);

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
		bool CFileSystem::DoesFileExist(const std::string& instrPath) const
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
		bool CFileSystem::DoesDirectoryExist(const std::string& instrPath) const
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
		bool CFileSystem::DoesFileExistInDLCCache(const std::string& instrPath) const
		{
			//Check that this storage location is available
			if (IsStorageLocationAvailable(Core::SL_DLC) == false)
			{
				ERROR_LOG("DLC Storage Location is not available!");
				return false;
			}
			std::string filepath = GetStorageLocationDirectory(Core::SL_DLC) + instrPath;
			return DoesFileExist(filepath);
		}
		//--------------------------------------------------------------
		/// Does Directory Exist In DLC Cache
		//--------------------------------------------------------------
		bool CFileSystem::DoesDirectoryExistInDLCCache(const std::string& instrPath) const
		{
			//Check that this storage location is available
			if (IsStorageLocationAvailable(Core::SL_DLC) == false)
			{
				ERROR_LOG("DLC Storage Location is not available!");
				return false;
			}
			std::string filepath = GetStorageLocationDirectory(Core::SL_DLC) + instrPath;
			return DoesDirectoryExist(filepath);
		}
	}
}
