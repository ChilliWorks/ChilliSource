/*
 *  FileSystem.cpp
 *  iOSTemplate
 *
 *  Created by Ian Copland on 25/03/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Platform/iOS/FileIO/FileSystem.h>
#include <ChilliSource/Core/File/FileStream.h>

#include <UIKit/UIKit.h>
#include <sys/types.h>
#include <sys/sysctl.h>

#include <ChilliSource/Core/Main/Utils.h>
#include <ChilliSource/Core/String/StringUtils.h>

#include <ChilliSource/Core/boost/filesystem.hpp>

#include <iostream>

namespace moFlo
{
	namespace iOSPlatform 
	{
        //constants
        const std::string kstrSaveDataPath  = "SaveData/";
        const std::string kstrCachePath  = "Caches/Cache/";
        const std::string kstrDLCPath  = "Caches/DLC/";
        
        std::string RetrieveBundlePath();
        std::string RetrieveDocumentsPath();
        std::string RetrieveLibraryPath();
        
		CFileSystem::CFileSystem()
		{
            mstrBundlePath = RetrieveBundlePath();
            mstrDocumentsPath = RetrieveDocumentsPath();
            mstrLibraryPath = RetrieveLibraryPath();
            
            CreateDirectory(Core::SL_SAVEDATA, "");
            CreateDirectory(Core::SL_CACHE, "");
            CreateDirectory(Core::SL_DLC, "");
            
            CreateHashedBundleFileList();
		}
        //--------------------------------------------------------------
        /// Retrieve Bundle Path
        ///
        /// @return The device bundle path as returned by iOS
        //--------------------------------------------------------------
        std::string RetrieveBundlePath()
        {
            CFBundleRef mainBundle = CFBundleGetMainBundle();
			CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
			char path[PATH_MAX];
			if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
			{
				ERROR_LOG("Changing working directory to resource folder");
			}
			CFRelease(resourcesURL);
			return std::string(path)  + "/";
        }
        //--------------------------------------------------------------
        /// Retrieve Documents Path
        ///
        /// @return The device documents path as returned by iOS
        //--------------------------------------------------------------
        std::string RetrieveDocumentsPath()
        {
            NSArray* documentDir = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
			if (documentDir)
			{
                NSString*	nsDocumentPath = nil;
				nsDocumentPath = [documentDir objectAtIndex:0];
                if (nsDocumentPath != nil)
                {
                    const char* pPath = [nsDocumentPath fileSystemRepresentation];
                    return std::string(pPath) + "/";
                }
			}
            
            return "";
        }
        //--------------------------------------------------------------
        /// Retrieve Library Path
        ///
        /// @return The device library path as returned by iOS
        //--------------------------------------------------------------
        std::string RetrieveLibraryPath()
        {
            NSArray* libraryDir = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
			if (libraryDir)
			{
                NSString*	nsLibraryPath = nil;
				nsLibraryPath = [libraryDir objectAtIndex:0];
                if (nsLibraryPath != nil)
                {
                    const char* pPath = [nsLibraryPath fileSystemRepresentation];
                    return std::string(pPath) + "/";
                }
			}
            
            return "";
        }
        //--------------------------------------------------------------
        /// Create Hashed Bundle File List
        ///
        //--------------------------------------------------------------
        void CFileSystem::CreateHashedBundleFileList()
        {
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            NSMutableArray* Contents = [[NSMutableArray alloc] init];
            NSString* Dir = [NSString stringWithCString:mstrBundlePath.c_str() encoding:NSASCIIStringEncoding];
            
            [Contents addObjectsFromArray:[[NSFileManager defaultManager] subpathsOfDirectoryAtPath:Dir error:NULL]];
            
            for(NSString* file in Contents)
            {
                std::string strFile([file UTF8String]);
                mHashedPackageFileNames.push_back(CHashCRC32::GenerateHashCode(strFile));
            }
            
            std::sort(mHashedPackageFileNames.begin(), mHashedPackageFileNames.end());
            
            [Contents release];
            [pPool release];
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
            
            //if this is not a read stream, insure that the storage location is writable.
            if (ineFileMode != Core::FM_READ && ineFileMode != Core::FM_READ_BINARY)
            {
                if(IsStorageLocationWritable(ineStorageLocation) == true)
                {
                    std::string filepath = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;
                    newFilestream->Open(filepath, ineFileMode);
                }
                else
                {
                    ERROR_LOG("Cannot write to the requested Storage Location!");
                }
            }
            else
            {
                std::string filepath;
                GetBestPathToFile(ineStorageLocation, instrFilepath, filepath);
                newFilestream->Open(filepath, ineFileMode);
            }
            
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
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            std::string path = GetStorageLocationDirectory(ineStorageLocation) + instrDirectory;
			NSFileManager *fileManager = [NSFileManager defaultManager];
            if (![fileManager fileExistsAtPath:[NSString stringWithUTF8String:path.c_str()]])
            {
                if (![fileManager createDirectoryAtPath:[NSString stringWithUTF8String:path.c_str()] withIntermediateDirectories:YES attributes:nil error:nil])
                {
                    ERROR_LOG("Error creating directory.");
                    [pPool release];
                    return false;
                }
            }
            [pPool release];
            
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
                ERROR_LOG("Source file does not exist -  " + instrSourceFilepath);
                return false;
            }
            
            std::string strSrcPath;
            GetBestPathToFile(ineSourceStorageLocation, instrSourceFilepath, strSrcPath);
            
            //get the path to the file
            std::string strPath, strName;
            moFlo::Core::CStringUtils::SplitFilename(instrDestinationFilepath, strName, strPath);
            
            //create the output directory
            CreateDirectory(ineDestinationStorageLocation, strPath);
            
            //try and copy the files
            boost::filesystem::path SrcPath(strSrcPath);
            boost::filesystem::path DstPath(GetStorageLocationDirectory(ineDestinationStorageLocation) + instrDestinationFilepath);
            boost::filesystem::copy_file(SrcPath, DstPath, boost::filesystem::copy_option::overwrite_if_exists);
            return DoesFileExist(Core::SL_CACHE, instrDestinationFilepath);
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
            
            //get the filepath
            std::string strPath = GetStorageLocationDirectory(ineStorageLocation) + instrFilepath;
            
            //remove the file
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
			CFStringRef cfStringRef = CFStringCreateWithCString(kCFAllocatorDefault, strPath.c_str(), kCFStringEncodingMacRoman);
            [[NSFileManager defaultManager] removeItemAtPath:(NSString*)cfStringRef error:NULL];
			CFRelease(cfStringRef); 
			[pPool release];
            
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
            
            //get the directory
            std::string strDirectory = GetStorageLocationDirectory(ineStorageLocation) + instrDirectory;
            
            //remove the directory
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
			CFStringRef cfStringRef = CFStringCreateWithCString(kCFAllocatorDefault, strDirectory.c_str(), kCFStringEncodingMacRoman);
            [[NSFileManager defaultManager] removeItemAtPath:(NSString*)cfStringRef error:NULL];
			CFRelease(cfStringRef); 
			[pPool release];
            
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
        void GetDirectoryContents(const DYNAMIC_ARRAY<std::string>& inastrDirs, bool inbRecursive, NSMutableArray* outpContents)
        {
            for(DYNAMIC_ARRAY<std::string>::const_iterator it = inastrDirs.begin(); it != inastrDirs.end(); ++it)
            {
                std::string path = moFlo::Core::CStringUtils::StandardisePath(*it);
                NSString* Dir = [NSString stringWithCString:path.c_str() encoding:NSASCIIStringEncoding];
                
                if (inbRecursive == true)
                {
                    [outpContents addObjectsFromArray:[[NSFileManager defaultManager] subpathsOfDirectoryAtPath:Dir error:NULL]];
                }
                else
                {
                    [outpContents addObjectsFromArray:[[NSFileManager defaultManager] contentsOfDirectoryAtPath:Dir error:NULL]];
                }
            }
        }
        //--------------------------------------------------------------
        /// Filter File Names By Extension
        ///
        /// @param Unfiltered names
        /// @param Extension
        /// @return Filtered names
        //--------------------------------------------------------------
        NSArray* FilterFileNamesByExtension(NSArray* inpFilenames, const std::string& instrExtension)
        {
            //Filter out the files we don't want
            NSString* Extension = [NSString stringWithCString:instrExtension.c_str() encoding:NSASCIIStringEncoding];
            NSString* Predicate = [NSString stringWithFormat:@"self ENDSWITH '.%@'", Extension];
            NSArray* Filtered = [inpFilenames filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:Predicate]];
            return Filtered;
        }
        //--------------------------------------------------------------
        /// Filter File Names By Name
        ///
        /// @param Unfiltered names
        /// @param Name
        /// @return Filtered names
        //--------------------------------------------------------------
        NSArray* FilterFileNamesByName(NSArray* inpFilenames, const std::string& instrName)
        {
            //Filter out the files we don't want
            NSString* Name = [NSString stringWithCString:instrName.c_str() encoding:NSASCIIStringEncoding];
            NSString* Predicate = [NSString stringWithFormat:@"self ENDSWITH '%@'", Name];
            NSArray* Filtered = [inpFilenames filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:Predicate]];
            return Filtered;
        }
        //--------------------------------------------------------------
        /// Filter File Names By File
        ///
        /// @param Unfiltered names
        /// @return Filtered names
        //--------------------------------------------------------------
        NSArray* FilterFileNamesByFile(NSArray* inpFilenames)
        {
            //Filter out the files we don't want
            NSString* Predicate = [NSString stringWithFormat:@"self contains '.'"];
            NSArray* Filtered = [inpFilenames filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:Predicate]];
            return Filtered;
        }
        //--------------------------------------------------------------
        /// Convert ObjC To Path
        ///
        /// @param Filenames ObjC
        /// @param Directory to append
        /// @param Out: Filenames
        //--------------------------------------------------------------
        void ConvertObjCToPath(NSArray* inpFilenames, const std::string& instrDirectory, DYNAMIC_ARRAY<std::string> &outstrFileNames)
        {
            std::string strDir;
            if(instrDirectory.empty() == false)
            {
                strDir = moFlo::Core::CStringUtils::StandardisePath(instrDirectory);
            }
            
            for(NSString* FileNames in inpFilenames)
            {
                outstrFileNames.push_back(strDir + moFlo::Core::CStringUtils::NSStringToString(FileNames));
            }
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
            
            DYNAMIC_ARRAY<std::string> astrDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, astrDirectoriesToCheck);
            
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            NSMutableArray* Contents = [[NSMutableArray alloc] init];
            
            GetDirectoryContents(astrDirectoriesToCheck, inbRecurseIntoSubDirectories, Contents);
            
            if([Contents count] > 0)
            {
                NSArray* Filtered = FilterFileNamesByExtension(Contents, instrExtension);

                if(inbAppendFullPath)
                {
                    ConvertObjCToPath(Filtered, instrDirectory, outstrFileNames);
                }
                else
                {
                    ConvertObjCToPath(Filtered, "", outstrFileNames);
                }
            }
            
            [Contents release];
            [pPool release];
            
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
            
            DYNAMIC_ARRAY<std::string> astrDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, astrDirectoriesToCheck);
            
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            NSMutableArray* Contents = [[NSMutableArray alloc] init];
            
            GetDirectoryContents(astrDirectoriesToCheck, inbRecurseIntoSubDirectories, Contents);
            
            if([Contents count] > 0)
            {
                NSArray* Filtered = FilterFileNamesByName(Contents, instrName);
                
                if(inbAppendFullPath)
                {
                    ConvertObjCToPath(Filtered, instrDirectory, outstrFileNames);
                }
                else
                {
                    ConvertObjCToPath(Filtered, "", outstrFileNames);
                }
            }
            
            [Contents release];
            [pPool release];
            
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
            
            DYNAMIC_ARRAY<std::string> astrDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, astrDirectoriesToCheck);
            
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            NSMutableArray* Contents = [[NSMutableArray alloc] init];
            
            GetDirectoryContents(astrDirectoriesToCheck, inbRecurseIntoSubDirectories, Contents);
            
            if([Contents count] > 0)
            {
                NSArray* Filtered = FilterFileNamesByFile(Contents);
                
                if(inbAppendFullPath)
                {
                    ConvertObjCToPath(Filtered, instrDirectory, outstrFileNames);
                }
                else
                {
                    ConvertObjCToPath(Filtered, "", outstrFileNames);
                }
            }
            
            [Contents release];
            [pPool release];
            
            std::sort(outstrFileNames.begin(), outstrFileNames.end());
            DYNAMIC_ARRAY<std::string>::iterator it = std::unique(outstrFileNames.begin(), outstrFileNames.end());
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
            
            DYNAMIC_ARRAY<std::string> astrDirectoriesToCheck;
            GetPathsForStorageLocation(ineStorageLocation, instrDirectory, astrDirectoriesToCheck);
            
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            NSMutableArray* Contents = [[NSMutableArray alloc] init];
            
            GetDirectoryContents(astrDirectoriesToCheck, inbRecurseIntoSubDirectories, Contents);
            
            if([Contents count] > 0)
            {
                NSArray* Filtered = FilterFileNamesByFile(Contents);
                
                if(inbAppendFullPath)
                {
                    ConvertObjCToPath(Filtered, instrDirectory, outstrDirectories);
                }
                else
                {
                    ConvertObjCToPath(Filtered, "", outstrDirectories);
                }
            }
            
            [Contents release];
            [pPool release];
            
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
        void CFileSystem::GetPathsForStorageLocation(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrFileName, DYNAMIC_ARRAY<std::string>& outaPaths) const
        {
            switch(ineStorageLocation)
            {
                case Core::SL_PACKAGE:
                    for(u32 i=0; i<3; ++i)
                    {
                        outaPaths.push_back(GetStorageLocationDirectory(ineStorageLocation) + mastrResourceDirectory[i] + instrFileName);
                    }
                    break;
                case Core::SL_DLC:
                    for(u32 i=0; i<3; ++i)
                    {
                        outaPaths.push_back(GetStorageLocationDirectory(Core::SL_PACKAGE) + mastrResourceDirectory[i] + mstrPackageDLCPath + instrFileName);
                    }
                    outaPaths.push_back(GetStorageLocationDirectory(Core::SL_DLC) + instrFileName);
                    break;
                default:
                    outaPaths.push_back(GetStorageLocationDirectory(ineStorageLocation) + instrFileName);
                    break;
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
                for(u32 i=0; i<3; ++i)
                {
                    if(DoesFileExistInHashedStore(mastrResourceDirectory[i] + instrFilepath))
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
        /// Does File Exist In Cached DLC
        ///
        /// @param The filepath.
        /// @return Whether or not it is in the cached DLC.
        //--------------------------------------------------------------
        bool CFileSystem::DoesFileExistInCachedDLC(const std::string& instrFilepath) const
        {
            return DoesItemExistInDLCCache(instrFilepath, false);
        }
        //--------------------------------------------------------------
        /// Does File Exist In Package DLC
        ///
        /// @param The filepath.
        /// @return Whether or not it is in the local DLC.
        //--------------------------------------------------------------
        bool CFileSystem::DoesFileExistInPackageDLC(const std::string& instrFilepath) const
        {
            return DoesFileExist(moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_PACKAGE) + mstrPackageDLCPath + instrFilepath));
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
                    if(DoesFolderExist(mstrBundlePath + mastrResourceDirectory[i] + instrDirectory))
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
			return DoesFolderExist(moFlo::Core::CStringUtils::StandardisePath(path));
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
            
            //return whether or not the file exists
            if(inbFolder)
            {
                return DoesFolderExist(moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_DLC) + instrPath));
            }
            else
            {
                return DoesFileExist(moFlo::Core::CStringUtils::StandardisePath(GetStorageLocationDirectory(Core::SL_DLC) + instrPath));
            }
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
                strResult = GetDirectoryForPackageFile(mstrPackageDLCPath + instrFilePath);
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
            std::string strResult = GetStorageLocationDirectory(Core::SL_PACKAGE) + instrFilePath;
            
            for(u32 i=0; i<3; ++i)
            {
                std::string strPath = moFlo::Core::CStringUtils::StandardisePath(mastrResourceDirectory[i] + instrFilePath);
                if(DoesFileExistInHashedStore(strPath))
                {
                    strResult = GetStorageLocationDirectory(Core::SL_PACKAGE) + strPath;
                    break;
                }
            }
            
            return strResult;
        }
        //--------------------------------------------------------------
        /// Does File Exist In Hashed Store
        ///
        /// returns whether the path exists in the stored hashed manifest
        ///
        /// @param the filepath.
        /// @return whether or not it exists.
        //--------------------------------------------------------------
        bool CFileSystem::DoesFileExistInHashedStore(const std::string& instrPath) const
        {
            u32 udwHashedFile = CHashCRC32::GenerateHashCode(instrPath);
            
            DYNAMIC_ARRAY<u32>::const_iterator it = std::lower_bound(mHashedPackageFileNames.begin(), mHashedPackageFileNames.end(), udwHashedFile);
            
            if(it!= mHashedPackageFileNames.end() && *it == udwHashedFile)
                return true;

            return false;
        }
        //--------------------------------------------------------------
        /// Does File Exist
        ///
        /// returns whether the path exists on the filesystem
        ///
        /// @param the filepath.
        /// @return whether or not it exists.
        //--------------------------------------------------------------
        bool CFileSystem::DoesFileExist(const std::string& instrPath) const
        {
            BOOL bDirectory = NO;
            bool bExists = false;
            NSFileManager *fileManager= [NSFileManager defaultManager];
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            
            if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:instrPath.c_str()] isDirectory:&bDirectory])
            {
                bExists = true;
            }
            
            [pPool release];
            return (bExists && !bDirectory);
        }
        //--------------------------------------------------------------
        /// Does Folder Exist
        ///
        /// returns whether the path exists on the filesystem
        ///
        /// @param the filepath.
        /// @return whether or not it exists.
        //--------------------------------------------------------------
        bool CFileSystem::DoesFolderExist(const std::string& instrPath) const
        {
            BOOL bDirectory = NO;
            bool bExists = false;
            NSFileManager *fileManager= [NSFileManager defaultManager];
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            
            if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:instrPath.c_str()] isDirectory:&bDirectory])
            {
                bExists = true;
            }
            
            [pPool release];
            return (bExists && bDirectory);
        }
	}
}
