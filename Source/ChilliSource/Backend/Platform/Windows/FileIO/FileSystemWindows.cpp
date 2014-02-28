/*
*  FileSystem.cpp
*  iOSTemplate
*
*  Created by Ian Copland on 25/03/2011.
*  Copyright 2011 Tag Games Ltd. All rights reserved.
*
*/

#include <ChilliSource/Backend/Platform/Windows/FileIO/FileSystemWindows.h>

#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/File/FileStream.h>
#include <ChilliSource/Core/String/StringUtils.h>

//This includes windows so needs to come after other engine includes, else it might cause problems with other includes
#include <ChilliSource/Backend/Platform/Windows/FileIO/WindowsFileUtils.h>

#include <iostream>
#include <windows.h>
#include <shlobj.h>

//Stupid windows! Why would you #define function names. Heaven forbid that
//someone else would want to use that function name in their code
#undef CreateDirectory
#undef CreateFile
#undef CopyFile
#undef DeleteFile

namespace ChilliSource
{
	namespace Windows 
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

			this->CreateDirectory(Core::StorageLocation::k_saveData, "");
			this->CreateDirectory(Core::StorageLocation::k_cache, "");
			this->CreateDirectory(Core::StorageLocation::k_DLC, "");
		}
		//--------------------------------------------------------------
		/// Create File Stream
		//--------------------------------------------------------------
		Core::FileStreamSPtr CFileSystem::CreateFileStream(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath, Core::FileMode ineFileMode) const
		{
			//create the file stream
			Core::FileStreamSPtr newFilestream = Core::FileStreamSPtr(new Core::FileStream());

			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available on this platform!");
				return newFilestream;
			}

			//get the filepath
			std::string filepath = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;

			//if this is not a read stream, insure that the storage location is writable.
			if (ineFileMode != Core::FileMode::k_read && ineFileMode != Core::FileMode::k_readBinary)
			{
				if (IsStorageLocationWritable(ineStorageLocation) == false)
				{
					CS_LOG_ERROR("Cannot write to the requested Storage Location!");
					return newFilestream;
				}
			}
			else
			{
                if(ineStorageLocation == Core::StorageLocation::k_package)
                {
                    //Attempt to load the device specific asset first
                    for(u32 i=0; i<3; ++i)
                    {
                        if(DoesFileExist(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + instrFilepath))
                        {
                            filepath = GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + instrFilepath;
                            break;
                        }
                    }
                }
                //if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
                else if (ineStorageLocation == Core::StorageLocation::k_DLC && DoesItemExistInDLCCache(instrFilepath, false) == false)
                {
                    //Attempt to load the device specific asset first
                    for(u32 i=0; i<3; ++i)
                    {
                        if(DoesFileExist(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrFilepath))
                        {
                            filepath = GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrFilepath;
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
		bool CFileSystem::CreateFile(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, s8* inpbyData, u32 inudwDataSize) const
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
		bool CFileSystem::CreateDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available on this platform!");
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Cannot write to the requested Storage Location!");
				return false;
			}

			//create the directory
			return SHCreateDirectoryExA(NULL, (GetStorageLocationDirectory(ineStorageLocation) + instrDirectory).c_str(), NULL) == ERROR_SUCCESS;
		}
		//--------------------------------------------------------------
		/// Copy File
		//--------------------------------------------------------------
		bool CFileSystem::CopyFile(Core::StorageLocation ineSourceStorageLocation, const std::string& instrSourceFilepath, 
			Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationFilepath) const
		{
            //check the requested source storage location is available
            if (IsStorageLocationAvailable(ineSourceStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested source Storage Location is not available on this platform!");
                return false;
            }
            
            //check the requested destination storage location is available
            if (IsStorageLocationAvailable(ineDestinationStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested destination Storage Location is not available on this platform!");
                return false;
            }
            
            //insure that the destination location is writable.
            if (IsStorageLocationWritable(ineDestinationStorageLocation) == false)
            {
                CS_LOG_ERROR("Cannot write to the destination Storage Location!");
                return false;
            }
            
            if(!DoesFileExist(ineSourceStorageLocation, instrSourceFilepath))
            {
                CS_LOG_ERROR("Source file does not exist");
                return false;
            }
            
            std::string strSrcPath;
            if(ineSourceStorageLocation == Core::StorageLocation::k_package)
            {
                //Attempt to load the device specific asset first
                for(u32 i=0; i<3; ++i)
                {
                    if(DoesFileExist(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + instrSourceFilepath))
                    {
                        strSrcPath = GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + instrSourceFilepath;
                        break;
                    }
                }
            }
            else if(ineSourceStorageLocation == Core::StorageLocation::k_DLC && DoesItemExistInDLCCache(instrSourceFilepath, false) == false)
            {
                //Attempt to load the device specific asset first
                for(u32 i=0; i<3; ++i)
                {
                    if(DoesFileExist(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrSourceFilepath))
                    {
                        strSrcPath = GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrSourceFilepath;
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
            ChilliSource::Core::StringUtils::SplitFilename(instrDestinationFilepath, strName, strPath);
            
            //create the output directory
            CreateDirectory(ineDestinationStorageLocation, strPath);
            
            //try and copy the files
			std::string destPath = GetStorageLocationDirectory(ineDestinationStorageLocation) + instrDestinationFilepath;
			if (WindowsFileUtils::WindowsCopyFile(strSrcPath.c_str(), destPath.c_str(), FALSE) == FALSE)
			{
				return false;
			}

			return false;
		}
		//--------------------------------------------------------------
		/// Copy Directory
		//--------------------------------------------------------------
		bool CFileSystem::CopyDirectory(Core::StorageLocation ineSourceStorageLocation, const std::string& instrSourceDirectory, 
			Core::StorageLocation ineDestinationStorageLocation, const std::string& instrDestinationDirectory) const
		{
			//NOTE: This doesnt work.


			//check the requested source storage location is available
			if (IsStorageLocationAvailable(ineSourceStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested source Storage Location is not available on this platform!");
				return false;
			}

			//check the requested destination storage location is available
			if (IsStorageLocationAvailable(ineDestinationStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested destination Storage Location is not available on this platform!");
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
		bool CFileSystem::DeleteFile(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available on this platform!");
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Cannot write to the requested Storage Location!");
				return false;
			}

			//remove the file
			std::string filename = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;
			WindowsFileUtils::WindowsDeleteFile(filename.c_str());

			//return successful
			return true;
		}
		//--------------------------------------------------------------
		/// Delete Directory
		//--------------------------------------------------------------
		bool CFileSystem::DeleteDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const
		{
			//check the requested storage location is available
			if (IsStorageLocationAvailable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available on this platform!");
				return false;
			}

			//insure that the storage location is writable.
			if (IsStorageLocationWritable(ineStorageLocation) == false)
			{
				CS_LOG_ERROR("Cannot write to the requested Storage Location!");
				return false;
			}

			//remove the directory
			std::string directory = GetStorageLocationDirectory(ineStorageLocation) + instrDirectory;
			WindowsFileUtils::WindowsRemoveDirectory(directory.c_str());

			//return successful
			return true;
		}
		//--------------------------------------------------------------
		/// Get File Names With Extension In Directory
		//--------------------------------------------------------------
		void CFileSystem::GetFileNamesWithExtensionInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
			const std::string& instrExtension, std::vector<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<std::string> astrDirectoriesToCheck;
            
            switch(ineStorageLocation)
            {
                case Core::StorageLocation::k_package:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrDirectory);
                    }
                    break;
                case Core::StorageLocation::k_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrDirectory);
                    }
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrDirectory);
                    break;
                default:
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
                    break;
            }
            
            for(std::vector<std::string>::iterator it = astrDirectoriesToCheck.begin(); it != astrDirectoriesToCheck.end(); ++it)
            {
                std::string path = ChilliSource::Core::StringUtils::StandardisePath(*it);
                boost::filesystem::path DstPath(path);
                
                if (inbRecurseIntoSubDirectories == true)
                {
                    boost::filesystem::recursive_directory_iterator it(DstPath);
                    boost::filesystem::recursive_directory_iterator end;
                    
                    while(it != end) 
                    {
                        //Add to list
                        if(boost::filesystem::is_regular_file(it->status()) && Core::StringUtils::EndsWith(it->filename(), instrExtension, false))
                        {
                            if(inbAppendFullPath)
                            {
                                outstrFileNames.push_back(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory) + it->filename());
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
                            if(boost::filesystem::is_regular_file(dir_iter->status()) && Core::StringUtils::EndsWith(dir_iter->filename(), instrExtension, false))
                            {
                                //Add to list
                                if(inbAppendFullPath)
                                {
                                    outstrFileNames.push_back(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory) + dir_iter->filename());
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
		void CFileSystem::GetPathForFilesWithNameInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
			const std::string& instrName, std::vector<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<std::string> astrDirectoriesToCheck;
            
            switch(ineStorageLocation)
            {
                case Core::StorageLocation::k_package:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrDirectory);
                    }
                    break;
                case Core::StorageLocation::k_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrDirectory);
                    }
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrDirectory);
                    break;
                default:
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
                    break;
            }
            
            for(std::vector<std::string>::iterator it = astrDirectoriesToCheck.begin(); it != astrDirectoriesToCheck.end(); ++it)
            {
                std::string path = ChilliSource::Core::StringUtils::StandardisePath(*it);
                boost::filesystem::path DstPath(path);
                
                if (inbRecurseIntoSubDirectories == true)
                {
                    boost::filesystem::recursive_directory_iterator it(DstPath);
                    boost::filesystem::recursive_directory_iterator end;
                    
                    while(it != end) 
                    {
                        //Add to list
                        if(boost::filesystem::is_regular_file(it->status()) && Core::StringUtils::EndsWith(it->filename(), instrName, false))
                        {
                            if(inbAppendFullPath)
                            {
                                outstrFileNames.push_back(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory) + it->filename());
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
                            if(boost::filesystem::is_regular_file(dir_iter->status()) && Core::StringUtils::EndsWith(dir_iter->filename(), instrName, false))
                            {
                                //Add to list
                                if(inbAppendFullPath)
                                {
                                    outstrFileNames.push_back(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory) + dir_iter->filename());
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
		void CFileSystem::GetFileNamesInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories, 
			std::vector<std::string> &outstrFileNames, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<std::string> astrDirectoriesToCheck;
            
            switch(ineStorageLocation)
            {
                case Core::StorageLocation::k_package:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrDirectory);
                    }
                    break;
                case Core::StorageLocation::k_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrDirectory);
                    }
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrDirectory);
                    break;
                default:
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
                    break;
            }
            
            for(std::vector<std::string>::iterator it = astrDirectoriesToCheck.begin(); it != astrDirectoriesToCheck.end(); ++it)
            {
                std::string path = ChilliSource::Core::StringUtils::StandardisePath(*it);
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
                                outstrFileNames.push_back(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory) + it->filename());
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
                                    outstrFileNames.push_back(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory) + dir_iter->filename());
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
		void CFileSystem::GetDirectoriesInDirectory(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory, bool inbRecurseIntoSubDirectories,
			std::vector<std::string> &outstrDirectories, bool inbAppendFullPath) const
		{
            //Check that this storage location is available
            if (IsStorageLocationAvailable(ineStorageLocation) == false)
            {
                CS_LOG_ERROR("Requested Storage Location is not available!");
                return;
            }
            
            std::vector<std::string> astrDirectoriesToCheck;
            
            switch(ineStorageLocation)
            {
                case Core::StorageLocation::k_package:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrDirectory);
                    }
                    break;
                case Core::StorageLocation::k_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrDirectory);
                    }
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrDirectory);
                    break;
                default:
                    astrDirectoriesToCheck.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrDirectory);
                    break;
            }
            
            for(std::vector<std::string>::iterator it = astrDirectoriesToCheck.begin(); it != astrDirectoriesToCheck.end(); ++it)
            {
                std::string path = ChilliSource::Core::StringUtils::StandardisePath(*it);
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
                                outstrDirectories.push_back(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory) + it->filename());
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
                                    outstrDirectories.push_back(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory) + dir_iter->filename());
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
            std::string strPath = ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrFilePath);
            
            if(DoesFileExist(strPath))
            {
                strResult = strPath;
            }
            else
            {
                for(u32 i=0; i<3; ++i)
                {
                    strPath = ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrFilePath);
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
                std::string strPath = ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(Core::StorageLocation::k_package) + mastrResourceDirectory[i] + instrFilePath);
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
		bool CFileSystem::DoesFileExist(Core::StorageLocation ineStorageLocation, const std::string& instrFilepath) const
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
            if (ineStorageLocation == Core::StorageLocation::k_DLC)
            {
                if (DoesItemExistInDLCCache(instrFilepath, false) == true)
                {
                    return true;
                }
                
                return DoesFileExist(Core::StorageLocation::k_package, mstrPackageDLCPath + instrFilepath);
            }
            
            //return whether or not the file exists
			return DoesFileExist(ChilliSource::Core::StringUtils::StandardisePath(path));
		}
		//--------------------------------------------------------------
		/// Does Directory Exist
		//--------------------------------------------------------------
		bool CFileSystem::DoesDirectoryExist(Core::StorageLocation ineStorageLocation, const std::string& instrDirectory) const
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
            if (ineStorageLocation == Core::StorageLocation::k_DLC)
            {
                if (DoesItemExistInDLCCache(instrDirectory, true) == true)
                {
                    return true;
                }
                
                return DoesDirectoryExist(Core::StorageLocation::k_package, mstrPackageDLCPath + instrDirectory);
            }
            
            //return whether or not the dir exists
			return DoesDirectoryExist(ChilliSource::Core::StringUtils::StandardisePath(path));
		}
        //--------------------------------------------------------------
		/// Does File Exist
		//--------------------------------------------------------------
		bool CFileSystem::DoesFileExist(const std::string& instrFilepath) const
		{
			//return whether or not the file exists
			boost::filesystem::path DstPath(ChilliSource::Core::StringUtils::StandardisePath(instrFilepath));
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
			std::string path = GetStorageLocationDirectory(Core::StorageLocation::k_package) + mstrPackageDLCPath + instrFilepath;
			boost::filesystem::path DstPath(ChilliSource::Core::StringUtils::StandardisePath(path));
			return boost::filesystem::exists(DstPath);
		}
		//--------------------------------------------------------------
		/// Does Directory Exist
		//--------------------------------------------------------------
		bool CFileSystem::DoesDirectoryExist(const std::string& instrDirectory) const
		{
			//return whether or not the dir exists
			boost::filesystem::path DstPath(ChilliSource::Core::StringUtils::StandardisePath(instrDirectory));
			return boost::filesystem::exists(DstPath);
		}
		//--------------------------------------------------------------
		/// Is Storage Location Available
		//--------------------------------------------------------------
		bool CFileSystem::IsStorageLocationAvailable(Core::StorageLocation ineStorageLocation) const
		{
			switch (ineStorageLocation) 
			{
			case Core::StorageLocation::k_package:
			case Core::StorageLocation::k_saveData:
			case Core::StorageLocation::k_cache:
			case Core::StorageLocation::k_DLC:
			case Core::SL_ROOT:
				return true;
			default:
				return false;
			}
		}
		//--------------------------------------------------------------
		/// Get Storage Location Directory
		//--------------------------------------------------------------
		std::string CFileSystem::GetStorageLocationDirectory(Core::StorageLocation ineStorageLocation) const
		{
			//get the storage location path
			std::string strStorageLocationPath;
			switch (ineStorageLocation) 
			{
			case Core::StorageLocation::k_package:
				strStorageLocationPath = mstrBundlePath;
				break;
			case Core::StorageLocation::k_saveData:
				strStorageLocationPath = mstrDocumentsPath + kstrSaveDataPath;
				break;
			case Core::StorageLocation::k_cache:
				strStorageLocationPath = mstrLibraryPath + kstrCachePath;
				break;
			case Core::StorageLocation::k_DLC:
				strStorageLocationPath = mstrLibraryPath + kstrDLCPath;
				break;
			case Core::SL_ROOT:
				strStorageLocationPath = "";
				break;
			default:
				CS_LOG_ERROR("Storage Location not available on this platform!");
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
			if (IsStorageLocationAvailable(Core::StorageLocation::k_DLC) == false)
			{
				CS_LOG_ERROR("Requested Storage Location is not available!");
				return false;
			}

			boost::filesystem::path DstPath(ChilliSource::Core::StringUtils::StandardisePath(GetStorageLocationDirectory(Core::StorageLocation::k_DLC) + instrPath));
			return boost::filesystem::exists(DstPath);
		}
	}
}
