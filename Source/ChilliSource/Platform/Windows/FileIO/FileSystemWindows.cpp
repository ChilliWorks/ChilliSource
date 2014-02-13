/*
*  FileSystem.cpp
*  iOSTemplate
*
*  Created by Ian Copland on 25/03/2011.
*  Copyright 2011 Tag Games Ltd. All rights reserved.
*
*/

#include <moFlo/Platform/Windows/FileIO/FileSystemWindows.h>
#include <moFlo/Core/FileIO/FileStream.h>

#include <moFlo/Core/Utils.h>
#include <moFlo/Core/StringUtils.h>

#include <moFlo/Core/boost/filesystem.hpp>

#include <iostream>

#include <windows.h>
#include <shlobj.h>

//Stupid windows! Why would you #define function names. Heaven forbid that
//someone else would want to use that function name in their code
#undef CreateDirectory
#undef CreateFile
#undef CopyFile
#undef DeleteFile

namespace moFlo
{
	namespace WindowsPlatform 
	{
		//constants
		const std::string kstrSaveDataPath  = "SaveData\\";
		const std::string kstrCachePath  = "Caches\\Cache\\";
		const std::string kstrDLCPath  = "Caches\\DLC\\";

		CFileSystem::CFileSystem()
		{
			Initialise();
		}
		//--------------------------------------------------------------------------------------------------
		/// Description:
		///
		/// Initialise the File Manager for use on the current file system.
		//--------------------------------------------------------------------------------------------------
		void CFileSystem::Initialise()
		{
			s8 NPath[MAX_PATH];
			GetModuleFileName(NULL, NPath, MAX_PATH);
			std::string::size_type pos = std::string(NPath).find_last_of( "\\/" );
			std::string strWorkingDir(std::string(NPath).substr(0, pos));

			mstrBundlePath = strWorkingDir + "\\assets\\";
			mstrDocumentsPath = strWorkingDir + "\\Documents\\";
			mstrLibraryPath = strWorkingDir + "\\Library\\";

			this->CreateDirectory(Core::SL_SAVEDATA, "");
			this->CreateDirectory(Core::SL_CACHE, "");
			this->CreateDirectory(Core::SL_DLC, "");
		}
		//--------------------------------------------------------------
		/// Create File Stream
		//--------------------------------------------------------------
		Core::FileStreamPtr CFileSystem::CreateFileStream(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFilepath, Core::FILE_MODE ineFileMode) const
		{
			//create the file stream
			Core::FileStreamPtr newFilestream = Core::FileStreamPtr(new Core::IFileStream());

			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				ERROR_LOG("Requested Storage Location is not available on this platform!");
				return newFilestream;
			}

			//get the filepath
			std::string filepath = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;

			//if this is not a read stream, insure that the storage location is writable.
			if (ineFileMode != Core::FM_READ && ineFileMode != Core::FM_READ_BINARY)
			{
				if (IsStorageLocationWritable(ineStorageLocation) == false)
				{
					ERROR_LOG("Cannot write to the requested Storage Location!");
					return newFilestream;
				}
			}
			else
			{
                if(ineStorageLocation == Core::SL_PACKAGE)
                {
                    //Attempt to load the device specific asset first
                    for(u32 i=0; i<3; ++i)
                    {
                        if(DoesFileExist(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + instrFilepath))
                        {
                            filepath = GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + instrFilepath;
                            break;
                        }
                    }
                }
                //if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
                else if (ineStorageLocation == Core::SL_DLC && DoesItemExistInDLCCache(instrFilepath, false) == false)
                {
                    //Attempt to load the device specific asset first
                    for(u32 i=0; i<3; ++i)
                    {
                        if(DoesFileExist(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrFilepath))
                        {
                            filepath = GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrFilepath;
                            break;
                        }
                    }
                }
			}

			//Load the file
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
				ERROR_LOG("Requested Storage Location is not available on this platform!");
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				ERROR_LOG("Cannot write to the requested Storage Location!");
				return false;
			}

			//create the directory
			return SHCreateDirectoryExA(NULL, (GetStorageLocationDirectory(ineStorageLocation) + instrDirectory).c_str(), NULL) == ERROR_SUCCESS;
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
                ERROR_LOG("Requested source Storage Location is not available on this platform!");
                return false;
            }
            
            //check the requested destination storage location is available
            if (IsStorageLocationAvailable(ineDestinationStorageLocation) == false)
            {
                ERROR_LOG("Requested destination Storage Location is not available on this platform!");
                return false;
            }
            
            //insure that the destination location is writable.
            if (IsStorageLocationWritable(ineDestinationStorageLocation) == false)
            {
                ERROR_LOG("Cannot write to the destination Storage Location!");
                return false;
            }
            
            if(!DoesFileExist(ineSourceStorageLocation, instrSourceFilepath))
            {
                ERROR_LOG("Source file does not exist");
                return false;
            }
            
            std::string strSrcPath;
            if(ineSourceStorageLocation == Core::SL_PACKAGE)
            {
                //Attempt to load the device specific asset first
                for(u32 i=0; i<3; ++i)
                {
                    if(DoesFileExist(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + instrSourceFilepath))
                    {
                        strSrcPath = GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + instrSourceFilepath;
                        break;
                    }
                }
            }
            else if(ineSourceStorageLocation == Core::SL_DLC && DoesItemExistInDLCCache(instrSourceFilepath, false) == false)
            {
                //Attempt to load the device specific asset first
                for(u32 i=0; i<3; ++i)
                {
                    if(DoesFileExist(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrSourceFilepath))
                    {
                        strSrcPath = GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrSourceFilepath;
                        break;
                    }
                }
            }
            else
            {
                strSrcPath = GetStorageLocationDirectory(ineSourceStorageLocation) + instrSourceFilepath;
            }
            
            //get the path to the file
            std::string strPath, strName;
            moFlo::Core::CStringUtils::SplitFilename(instrDestinationFilepath, strName, strPath);
            
            //create the output directory
            CreateDirectory(ineDestinationStorageLocation, strPath);
            
            //try and copy the files
            boost::filesystem::path SrcPath(strSrcPath);
            boost::filesystem::path DstPath(GetStorageLocationDirectory(ineDestinationStorageLocation) + instrDestinationFilepath);
            boost::filesystem::copy_file(SrcPath, DstPath, boost::filesystem::copy_option::overwrite_if_exists);
            return boost::filesystem::exists(DstPath);
		}
		//--------------------------------------------------------------
		/// Copy Directory
		//--------------------------------------------------------------
		bool CFileSystem::CopyDirectory(Core::STORAGE_LOCATION ineSourceStorageLocation, const std::string& instrSourceDirectory, 
			Core::STORAGE_LOCATION ineDestinationStorageLocation, const std::string& instrDestinationDirectory) const
		{
			//NOTE: This doesnt work.


			//check the requested source storage location is available
			if (IsStorageLocationAvailable(ineSourceStorageLocation) == false)
			{
				ERROR_LOG("Requested source Storage Location is not available on this platform!");
				return false;
			}

			//check the requested destination storage location is available
			if (IsStorageLocationAvailable(ineDestinationStorageLocation) == false)
			{
				ERROR_LOG("Requested destination Storage Location is not available on this platform!");
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
				ERROR_LOG("Requested Storage Location is not available on this platform!");
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				ERROR_LOG("Cannot write to the requested Storage Location!");
				return false;
			}

			//remove the file
			boost::filesystem::path DstPath(GetStorageLocationDirectory(ineStorageLocation) + instrFilepath);
			boost::filesystem::remove(DstPath);

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
				ERROR_LOG("Requested Storage Location is not available on this platform!");
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				ERROR_LOG("Cannot write to the requested Storage Location!");
				return false;
			}

			//remove the directory
			boost::filesystem::path DstPath(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
			boost::filesystem::remove(DstPath);

			//return successful
			return true;
		}
		//--------------------------------------------------------------
		/// Get File Names With Extension In Directory
		//--------------------------------------------------------------
		void CFileSystem::GetFileNamesWithExtensionInDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
			const std::string& instrExtension, DYNAMIC_ARRAY<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<std::string> astrDirectoriesToCheck;
            
            switch(ineStorageLocation)
            {
                case Core::SL_PACKAGE:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrDirectory);
                    }
                    break;
                case Core::SL_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrDirectory);
                    }
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::SL_DLC) + instrDirectory);
                    break;
                default:
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
                    break;
            }
            
            for(std::vector<std::string>::iterator it = astrDirectoriesToCheck.begin(); it != astrDirectoriesToCheck.end(); ++it)
            {
                std::string path = moFlo::Core::CStringUtils::StandardisePath(*it);
                boost::filesystem::path DstPath(path);
                
                if (inbRecurseIntoSubDirectories == true)
                {
                    boost::filesystem::recursive_directory_iterator it(DstPath);
                    boost::filesystem::recursive_directory_iterator end;
                    
                    while(it != end) 
                    {
                        //Add to list
                        if(boost::filesystem::is_regular_file(it->status()) && Core::CStringUtils::EndsWith(it->filename(), instrExtension, false))
                        {
                            if(inbAppendFullPath)
                            {
                                outstrFileNames.push_back(moFlo::Core::CStringUtils::StandardisePath(instrDirectory) + it->filename());
                            }
                            else
                            {
                                outstrFileNames.push_back(it->filename());
                            }
                        }
                        
                        ++it;
                    }
                }
                else
                {
                    boost::filesystem::directory_iterator end_it;
                    
                    if(boost::filesystem::exists(DstPath) && boost::filesystem::is_directory(DstPath))
                    {
                        for(boost::filesystem::directory_iterator dir_iter(DstPath); dir_iter != end_it ; ++dir_iter)
                        {
                            if(boost::filesystem::is_regular_file(dir_iter->status()) && Core::CStringUtils::EndsWith(dir_iter->filename(), instrExtension, false))
                            {
                                //Add to list
                                if(inbAppendFullPath)
                                {
                                    outstrFileNames.push_back(moFlo::Core::CStringUtils::StandardisePath(instrDirectory) + dir_iter->filename());
                                }
                                else
                                {
                                    outstrFileNames.push_back(dir_iter->filename());
                                }
                            }
                        }
                    }
                }
            }
            
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
		void CFileSystem::GetPathForFilesWithNameInDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
			const std::string& instrName, DYNAMIC_ARRAY<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<std::string> astrDirectoriesToCheck;
            
            switch(ineStorageLocation)
            {
                case Core::SL_PACKAGE:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrDirectory);
                    }
                    break;
                case Core::SL_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrDirectory);
                    }
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::SL_DLC) + instrDirectory);
                    break;
                default:
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
                    break;
            }
            
            for(std::vector<std::string>::iterator it = astrDirectoriesToCheck.begin(); it != astrDirectoriesToCheck.end(); ++it)
            {
                std::string path = moFlo::Core::CStringUtils::StandardisePath(*it);
                boost::filesystem::path DstPath(path);
                
                if (inbRecurseIntoSubDirectories == true)
                {
                    boost::filesystem::recursive_directory_iterator it(DstPath);
                    boost::filesystem::recursive_directory_iterator end;
                    
                    while(it != end) 
                    {
                        //Add to list
                        if(boost::filesystem::is_regular_file(it->status()) && Core::CStringUtils::EndsWith(it->filename(), instrName, false))
                        {
                            if(inbAppendFullPath)
                            {
                                outstrFileNames.push_back(moFlo::Core::CStringUtils::StandardisePath(instrDirectory) + it->filename());
                            }
                            else
                            {
                                outstrFileNames.push_back(it->filename());
                            }
                        }
                        
                        ++it;
                    }
                }
                else
                {
                    boost::filesystem::directory_iterator end_it;
                    
                    if(boost::filesystem::exists(DstPath) && boost::filesystem::is_directory(DstPath))
                    {
                        for(boost::filesystem::directory_iterator dir_iter(DstPath); dir_iter != end_it ; ++dir_iter)
                        {
                            if(boost::filesystem::is_regular_file(dir_iter->status()) && Core::CStringUtils::EndsWith(dir_iter->filename(), instrName, false))
                            {
                                //Add to list
                                if(inbAppendFullPath)
                                {
                                    outstrFileNames.push_back(moFlo::Core::CStringUtils::StandardisePath(instrDirectory) + dir_iter->filename());
                                }
                                else
                                {
                                    outstrFileNames.push_back(dir_iter->filename());
                                }
                            }
                        }
                    }
                }
            }
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            std::vector<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
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
            
            std::vector<std::string> astrDirectoriesToCheck;
            
            switch(ineStorageLocation)
            {
                case Core::SL_PACKAGE:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrDirectory);
                    }
                    break;
                case Core::SL_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrDirectory);
                    }
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::SL_DLC) + instrDirectory);
                    break;
                default:
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
                    break;
            }
            
            for(std::vector<std::string>::iterator it = astrDirectoriesToCheck.begin(); it != astrDirectoriesToCheck.end(); ++it)
            {
                std::string path = moFlo::Core::CStringUtils::StandardisePath(*it);
                boost::filesystem::path DstPath(path);
                
                if (inbRecurseIntoSubDirectories == true)
                {
                    boost::filesystem::recursive_directory_iterator it(DstPath);
                    boost::filesystem::recursive_directory_iterator end;
                    
                    while(it != end) 
                    {
                        //Add to list
                        if(boost::filesystem::is_regular_file(it->status()))
                        {
                            if(inbAppendFullPath)
                            {
                                outstrFileNames.push_back(moFlo::Core::CStringUtils::StandardisePath(instrDirectory) + it->filename());
                            }
                            else
                            {
                                outstrFileNames.push_back(it->filename());
                            }
                        }
                        
                        ++it;
                    }
                }
                else
                {
                    boost::filesystem::directory_iterator end_it;
                    
                    if(boost::filesystem::exists(DstPath) && boost::filesystem::is_directory(DstPath))
                    {
                        for(boost::filesystem::directory_iterator dir_iter(DstPath); dir_iter != end_it ; ++dir_iter)
                        {
                            if(boost::filesystem::is_regular_file(dir_iter->status()))
                            {
                                //Add to list
                                if(inbAppendFullPath)
                                {
                                    outstrFileNames.push_back(moFlo::Core::CStringUtils::StandardisePath(instrDirectory) + dir_iter->filename());
                                }
                                else
                                {
                                    outstrFileNames.push_back(dir_iter->filename());
                                }
                            }
                        }
                    }
                }
            }
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            std::vector<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
            outstrFileNames.resize(it - outstrFileNames.begin()); 
		}
		//--------------------------------------------------------------
		/// Get Directories In Directory
		//--------------------------------------------------------------
		void CFileSystem::GetDirectoriesInDirectory(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
			DYNAMIC_ARRAY<std::string> &outstrDirectories, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                ERROR_LOG("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<std::string> astrDirectoriesToCheck;
            
            switch(ineStorageLocation)
            {
                case Core::SL_PACKAGE:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrDirectory);
                    }
                    break;
                case Core::SL_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrDirectory);
                    }
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::SL_DLC) + instrDirectory);
                    break;
                default:
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
                    break;
            }
            
            for(std::vector<std::string>::iterator it = astrDirectoriesToCheck.begin(); it != astrDirectoriesToCheck.end(); ++it)
            {
                std::string path = moFlo::Core::CStringUtils::StandardisePath(*it);
                boost::filesystem::path DstPath(path);
                
                if (inbRecurseIntoSubDirectories == true)
                {
                    boost::filesystem::recursive_directory_iterator it(DstPath);
                    boost::filesystem::recursive_directory_iterator end;
                    
                    while(it != end) 
                    {
                        //Add to list
                        if(boost::filesystem::is_directory(it->status()))
                        {
                            if(inbAppendFullPath)
                            {
                                outstrDirectories.push_back(moFlo::Core::CStringUtils::StandardisePath(instrDirectory) + it->filename());
                            }
                            else
                            {
                                outstrDirectories.push_back(it->filename());
                            }
                        }
                        
                        ++it;
                    }
                }
                else
                {
                    boost::filesystem::directory_iterator end_it;
                    
                    if(boost::filesystem::exists(DstPath) && boost::filesystem::is_directory(DstPath))
                    {
                        for(boost::filesystem::directory_iterator dir_iter(DstPath); dir_iter != end_it ; ++dir_iter)
                        {
                            if(boost::filesystem::is_directory(dir_iter->status()))
                            {
                                //Add to list
                                if(inbAppendFullPath)
                                {
                                    outstrDirectories.push_back(moFlo::Core::CStringUtils::StandardisePath(instrDirectory) + dir_iter->filename());
                                }
                                else
                                {
                                    outstrDirectories.push_back(dir_iter->filename());
                                }
                            }
                        }
                    }
                }
            }
            
            std::sort(outstrDirectories.begin(), outstrDirectories.end());
            std::vector<std::string>::iterator it = std::unique(outstrDirectories.begin(), outstrDirectories.end());
            outstrDirectories.resize(it - outstrDirectories.begin()); 
		}
        //--------------------------------------------------------------
        /// Get Directory For DLC File
        ///
        /// @param The filename of the DLC asset.
        /// @return The directory to either the package DLC or cache DLC.
        //--------------------------------------------------------------
        std::string CFileSystem::GetDirectoryForDLCFile(const std::string& instrFilePath) const
        {
            std::string strResult;
            std::string strPath = moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_DLC) + instrFilePath);
            
            if(DoesFileExist(strPath))
            {
                strResult = strPath;
            }
            else
            {
                for(u32 i=0; i<3; ++i)
                {
                    strPath = moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrFilePath);
                    if(DoesFileExist(strPath))
                    {
                        strResult = strPath;
                        break;
                    }
                }                                                    
            }
            
            return strResult;
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
            
            for(u32 i=0; i<3; ++i)
            {
                std::string strPath = moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + instrFilePath);
                if(DoesFileExist(strPath))
                {
                    strResult = strPath;
                    break;
                }
            }
            
            return strResult;
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
                for(u32 i=0; i<3; ++i)
                {
                    if(DoesFileExist(mstrBundlePath + mastrResourceDirectory[i] + instrFilepath))
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
                if (DoesItemExistInDLCCache(instrFilepath, false) == true)
                {
                    return true;
                }
                
                return DoesFileExist(Core::SL_PACKAGE, mstrPackageDLCPath + instrFilepath);
            }
            
            //return whether or not the file exists
			return DoesFileExist(moFlo::Core::CStringUtils::StandardisePath(path));
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
                    if(DoesDirectoryExist(mstrBundlePath + mastrResourceDirectory[i] + instrDirectory))
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
                if (DoesItemExistInDLCCache(instrDirectory, true) == true)
                {
                    return true;
                }
                
                return DoesDirectoryExist(Core::SL_PACKAGE, mstrPackageDLCPath + instrDirectory);
            }
            
            //return whether or not the dir exists
			return DoesDirectoryExist(moFlo::Core::CStringUtils::StandardisePath(path));
		}
        //--------------------------------------------------------------
		/// Does File Exist
		//--------------------------------------------------------------
		bool CFileSystem::DoesFileExist(const std::string& instrFilepath) const
		{
			//return whether or not the file exists
			boost::filesystem::path DstPath(moFlo::Core::CStringUtils::StandardisePath(instrFilepath));
			return boost::filesystem::exists(DstPath);
		}
		//--------------------------------------------------------------
		/// Does File Exist In Cached DLC
		//--------------------------------------------------------------
		bool CFileSystem::DoesFileExistInCachedDLC(const std::string& instrFilepath) const
		{
			return DoesItemExistInDLCCache(instrFilepath, false);
		}
		//--------------------------------------------------------------
		/// Does File Exist In Package DLC
		//--------------------------------------------------------------
		bool CFileSystem::DoesFileExistInPackageDLC(const std::string& instrFilepath) const
		{
			std::string path = GetStorageLocationDirectory(Core::SL_PACKAGE) + mstrPackageDLCPath + instrFilepath;
			boost::filesystem::path DstPath(moFlo::Core::CStringUtils::StandardisePath(path));
			return boost::filesystem::exists(DstPath);
		}
		//--------------------------------------------------------------
		/// Does Directory Exist
		//--------------------------------------------------------------
		bool CFileSystem::DoesDirectoryExist(const std::string& instrDirectory) const
		{
			//return whether or not the dir exists
			boost::filesystem::path DstPath(moFlo::Core::CStringUtils::StandardisePath(instrDirectory));
			return boost::filesystem::exists(DstPath);
		}
		//--------------------------------------------------------------
		/// Is Storage Location Available
		//--------------------------------------------------------------
		bool CFileSystem::IsStorageLocationAvailable(Core::STORAGE_LOCATION ineStorageLocation) const
		{
			switch (ineStorageLocation) 
			{
			case Core::SL_PACKAGE:
			case Core::SL_SAVEDATA:
			case Core::SL_CACHE:
			case Core::SL_DLC:
			case Core::SL_ROOT:
				return true;
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
				strStorageLocationPath = mstrBundlePath;
				break;
			case Core::SL_SAVEDATA:
				strStorageLocationPath = mstrDocumentsPath + kstrSaveDataPath;
				break;
			case Core::SL_CACHE:
				strStorageLocationPath = mstrLibraryPath + kstrCachePath;
				break;
			case Core::SL_DLC:
				strStorageLocationPath = mstrLibraryPath + kstrDLCPath;
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
		/// Does Item Exist In DLC Cache
		//--------------------------------------------------------------
		bool CFileSystem::DoesItemExistInDLCCache(const std::string& instrPath, bool inbFolder) const
		{
			//Check that this storage location is available
			if (IsStorageLocationAvailable(Core::SL_DLC) == false)
			{
				ERROR_LOG("Requested Storage Location is not available!");
				return false;
			}

			boost::filesystem::path DstPath(moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_DLC) + instrPath));
			return boost::filesystem::exists(DstPath);
		}
	}
}
