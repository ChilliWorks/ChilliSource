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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Core/File/FileSystem.h>

#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>

#import <ChilliSource/Core/String/StringUtils.h>

#import <iostream>
#import <UIKit/UIKit.h>
#import <sys/types.h>
#import <sys/sysctl.h>

namespace CSBackend
{
	namespace iOS 
	{
        namespace
        {
            const std::string k_saveDataPath  = "SaveData/";
            const std::string k_cachePath  = "Caches/Cache/";
            const std::string k_dlcPath  = "Caches/DLC/";
            
            //--------------------------------------------------------------
            /// Returns a string containing the error in the given NSError.
            ///
            /// @author Ian Copland
            ///
            /// @return The error string.
            //--------------------------------------------------------------
            std::string GetErrorString(NSError* in_error)
            {
                if (in_error != nil)
                {
                    return [NSStringUtils newUTF8StringWithNSString:[in_error localizedDescription]];
                }
                
                return nil;
            }
            //--------------------------------------------------------------
            /// @author S Downie
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
                    CS_LOG_ERROR("Changing working directory to resource folder");
                }
                CFRelease(resourcesURL);
                return std::string(path)  + "/";
            }
            //--------------------------------------------------------------
            /// @author S Downie
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
            /// @author S Downie
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
            /// @author S Downie
            ///
            /// @param Unfiltered names
            /// @return Filtered names
            //--------------------------------------------------------------
            NSArray* FilterPathsByFile(NSArray* in_fileNames)
            {
                //Filter out the files we don't want
                NSString* Predicate = [NSString stringWithFormat:@"self contains '.'"];
                NSArray* Filtered = [in_fileNames filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:Predicate]];
                return Filtered;
            }
            //--------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param Unfiltered names
            /// @return Filtered names
            //--------------------------------------------------------------
            NSArray* FilterPathsByDirectory(NSArray* in_fileNames)
            {
                //Filter out the files we don't want
                NSString* Predicate = [NSString stringWithFormat:@"not (self contains '.')"];
                NSArray* Filtered = [in_fileNames filteredArrayUsingPredicate:[NSPredicate predicateWithFormat:Predicate]];
                return Filtered;
            }
            //--------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Filenames ObjC
            ///
            /// @return vector of file paths.
            //--------------------------------------------------------------
            std::vector<std::string> ConvertObjCToPath(NSArray* in_objFilePaths)
            {
                std::vector<std::string> output;
                for(NSString* filePath in in_objFilePaths)
                {
                    output.push_back([NSStringUtils newUTF8StringWithNSString:filePath]);
                }
                return output;
            }
            //--------------------------------------------------------------
			/// returns whether the path exists on the filesystem
            ///
            /// @author S Downie
            ///
            /// @param the filepath.
            ///
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesFileExist(const std::string& in_filePath)
            {
                bool fileExists = false;
                
                @autoreleasepool
                {
                    BOOL bDirectory = NO;
                    bool bExists = false;
                    
                    NSFileManager *fileManager = [NSFileManager defaultManager];
                    if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:in_filePath.c_str()] isDirectory:&bDirectory])
                    {
                        bExists = true;
                    }
                    
                    fileExists = (bExists && !bDirectory);
                }
                
                return fileExists;
            }
            //--------------------------------------------------------------
			/// returns whether the path exists on the filesystem
            ///
            /// @author S Downie
            ///
            /// @param the filepath.
            ///
            /// @return whether or not it exists.
			//--------------------------------------------------------------
            bool DoesDirectoryExist(const std::string& in_directoryPath)
            {
                bool directoryExists = false;
                
                @autoreleasepool
                {
                    BOOL bDirectory = NO;
                    bool bExists = false;
                    
                    NSFileManager *fileManager= [NSFileManager defaultManager];
                    if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:in_directoryPath.c_str()] isDirectory:&bDirectory])
                    {
                        bExists = true;
                    }
                    
                    directoryExists = (bExists && bDirectory);
                }
                
                return directoryExists;
            }
            //------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Directories
            /// @param Recurse into sub directories
            /// @param Out: Content file names
            //------------------------------------------------------------
            void GetDirectoryContents(const std::vector<std::string>& in_directoryPaths, bool in_recursive, NSMutableArray* out_contents)
            {
                @autoreleasepool
                {
                    for(std::vector<std::string>::const_iterator it = in_directoryPaths.begin(); it != in_directoryPaths.end(); ++it)
                    {
                        if (DoesDirectoryExist(*it) == true)
                        {
                            std::string path = CSCore::StringUtils::StandardiseDirectoryPath(*it);
                            NSString* Dir = [NSString stringWithCString:path.c_str() encoding:NSASCIIStringEncoding];
                            NSError* error = nil;
                            
                            if (in_recursive == true)
                            {
                                [out_contents addObjectsFromArray:[[NSFileManager defaultManager] subpathsOfDirectoryAtPath:Dir error:&error]];
                            }
                            else
                            {
                                [out_contents addObjectsFromArray:[[NSFileManager defaultManager] contentsOfDirectoryAtPath:Dir error:&error]];
                            }
                            
                            if (error != nil)
                            {
                                CS_LOG_ERROR("File System: Failed to get contents of directory '" + *it + "' - " + GetErrorString(error));
                                return;
                            }
                        }
                    }
                }
            }
        }
        
        CS_DEFINE_NAMEDTYPE(FileSystem);
        //--------------------------------------------------------------
        //--------------------------------------------------------------
		FileSystem::FileSystem()
		{
            m_bundlePath = RetrieveBundlePath();
            m_documentsPath = RetrieveDocumentsPath();
            m_libraryPath = RetrieveLibraryPath();
            
            CreateDirectoryPath(CSCore::StorageLocation::k_saveData, "");
            CreateDirectoryPath(CSCore::StorageLocation::k_cache, "");
            CreateDirectoryPath(CSCore::StorageLocation::k_DLC, "");
            
            CreatePackageManifest();
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
            CSCore::FileStreamUPtr fileStream = CSCore::FileStreamUPtr(new CSCore::FileStream());
            if (IsWriteMode(in_fileMode) == true)
            {
                CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");
 
                std::string filePath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
                fileStream->Open(filePath, in_fileMode);
            }
            else
            {
                std::string filePath = GetAbsolutePathToFile(in_storageLocation, in_filePath);
                fileStream->Open(filePath, in_fileMode);
            }
            
			return fileStream;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::CreateDirectoryPath(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
        {
            CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");
            
            @autoreleasepool
            {
                //create the directory
                std::string path = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
                NSFileManager* fileManager = [NSFileManager defaultManager];
                NSError* error = nil;
                if (![fileManager fileExistsAtPath:[NSString stringWithUTF8String:path.c_str()]])
                {
                    if (![fileManager createDirectoryAtPath:[NSString stringWithUTF8String:path.c_str()] withIntermediateDirectories:YES attributes:nil error:&error])
                    {
                        CS_LOG_ERROR("File System: Error creating directory '" + in_directoryPath + "' - " + GetErrorString(error));
                        return false;
                    }
                }
            }
            
			return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::CopyFile(CSCore::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
                                   CSCore::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
        {
            CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");
            
            @autoreleasepool
            {
                std::string sourcePath = GetAbsolutePathToFile(in_sourceStorageLocation, in_sourceFilePath);
                if(sourcePath.empty() == true)
                {
                    CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
                    return false;
                }
                
                //get the path to the file
                std::string destinationFileName, destinationDirectoryPath;
                CSCore::StringUtils::SplitFilename(in_destinationFilePath, destinationFileName, destinationDirectoryPath);
                
                //create the output directory
                CreateDirectoryPath(in_destinationStorageLocation, destinationDirectoryPath);
                
                std::string destinationPath = GetAbsolutePathToStorageLocation(in_destinationStorageLocation) + in_destinationFilePath;
                std::string atomicDestinationPath = destinationPath + ".tmp";
                
                NSString* nsSourcePath = [NSString stringWithUTF8String:sourcePath.c_str()];
                NSString* nsAtomicDestinationPath = [NSString stringWithUTF8String:atomicDestinationPath.c_str()];
                
                NSError* pCopyError = nil;
                [[NSFileManager defaultManager] copyItemAtPath:nsSourcePath toPath:nsAtomicDestinationPath error:&pCopyError];
                if (pCopyError != nil)
                {
                    CS_LOG_ERROR("File System: Failed to copy file '" + in_sourceFilePath + "' - " + GetErrorString(pCopyError));
                    return false;
                }

                NSString* nsDestinationURLPath = [NSString stringWithUTF8String:("file://" + destinationPath).c_str()];
                NSString* nsAtomicDestinationURLPath = [NSString stringWithUTF8String:("file://" + atomicDestinationPath).c_str()];
                NSURL* destinationURL = [NSURL URLWithString:[nsDestinationURLPath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
                NSURL* atomicDestinationURL = [NSURL URLWithString:[nsAtomicDestinationURLPath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
                
                NSError* pReplaceError = nil;
                [[NSFileManager defaultManager] replaceItemAtURL:destinationURL withItemAtURL:atomicDestinationURL backupItemName:nil options:NSFileManagerItemReplacementUsingNewMetadataOnly resultingItemURL:nil error:&pReplaceError];
                if (pReplaceError != nil)
                {
                    CS_LOG_ERROR("File System: Failed to copy file '" + in_sourceFilePath + "' - " + GetErrorString(pReplaceError));
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

            //get all the files in the directory
            std::vector<std::string> astrFilenames = GetFilePaths(in_sourceStorageLocation, in_sourceDirectoryPath, true);
            
            //error if there are no files
            if (astrFilenames.size() == 0)
            {
                CreateDirectoryPath(in_destinationStorageLocation, in_destinationDirectoryPath);
            }
            else
            {
                //copy each of these files individually
                std::string sourcePath = CSCore::StringUtils::StandardiseDirectoryPath(in_sourceDirectoryPath);
                std::string destPath = CSCore::StringUtils::StandardiseDirectoryPath(in_destinationDirectoryPath);
                for (std::vector<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
                {
                    if (CopyFile(in_sourceStorageLocation, sourcePath + *it, in_destinationStorageLocation, destPath + *it) == false)
                    {
                        return false;
                    }
                }
            }
            
            return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DeleteFile(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const
        {
            CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");
            
            @autoreleasepool
            {
                //get the filepath
                std::string path = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
                
                NSError* error = nil;
                
                NSString* nsPath = [NSStringUtils newNSStringWithUTF8String:path];
                [[NSFileManager defaultManager] removeItemAtPath:nsPath error:&error];
                [nsPath release];
                
                if (error != nil)
                {
                    CS_LOG_ERROR("File System: Error deleting file '" + in_filePath + "' - " + GetErrorString(error));
                    return false;
                }
            }
            
            //return successful
            return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DeleteDirectory(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
        {
            CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");
            
            @autoreleasepool
            {
                std::string directoryPath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
                
                NSError* error = nil;
                
                NSString* nsPath = [NSStringUtils newNSStringWithUTF8String:directoryPath];
                [[NSFileManager defaultManager] removeItemAtPath:nsPath error:&error];
                [nsPath release];
                
                if (error != nil)
                {
                    CS_LOG_ERROR("File System: Error deleting file '" + in_directoryPath + "' - " + GetErrorString(error));
                    return false;
                }
            }
            
            return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::vector<std::string> FileSystem::GetFilePaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
        {
            std::vector<std::string> output;
            
            @autoreleasepool
            {
                std::vector<std::string> astrDirectoriesToCheck = GetPossibleAbsoluteDirectoryPaths(in_storageLocation, in_directoryPath);
                
                NSMutableArray* Contents = [NSMutableArray array];
                
                GetDirectoryContents(astrDirectoriesToCheck, in_recursive, Contents);
                
                if([Contents count] > 0)
                {
                    NSArray* Filtered = FilterPathsByFile(Contents);
                    output = ConvertObjCToPath(Filtered);
                }
                
                std::sort(output.begin(), output.end());
                std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
                output.resize(it - output.begin());
            }
            
            return output;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::vector<std::string> FileSystem::GetDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
        {
            std::vector<std::string> output;
            
            @autoreleasepool
            {
                std::vector<std::string> astrDirectoriesToCheck = GetPossibleAbsoluteDirectoryPaths(in_storageLocation, in_directoryPath);
                
                NSMutableArray* Contents = [NSMutableArray array];
                
                GetDirectoryContents(astrDirectoriesToCheck, in_recursive, Contents);
                
                if([Contents count] > 0)
                {
                    NSArray* Filtered = FilterPathsByDirectory(Contents);
                    output = ConvertObjCToPath(Filtered);
                }
                
                std::sort(output.begin(), output.end());
                std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
                output.resize(it - output.begin());
            }
            
            return output;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesFileExist(CSCore::StorageLocation in_storageLocation, const std::string& in_filePath) const
        {
            if(in_storageLocation == CSCore::StorageLocation::k_package)
            {
                if(DoesFileExistInPackage(in_filePath))
                {
                    return true;
                }
                
                return false;
            }
            
            //get the filepath
            std::string path = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
            
            //if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
            if (in_storageLocation == CSCore::StorageLocation::k_DLC)
            {
                if (DoesItemExistInDLCCache(in_filePath, false) == true)
                {
                    return true;
                }
                
                return DoesFileExist(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
            }
            
            //return whether or not the file exists
			return CSBackend::iOS::DoesFileExist(CSCore::StringUtils::StandardiseFilePath(path));
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesFileExistInCachedDLC(const std::string& in_filePath) const
        {
            return DoesItemExistInDLCCache(in_filePath, false);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesFileExistInPackageDLC(const std::string& in_filePath) const
        {
            return DoesFileExist(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesDirectoryExist(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
        {
            if(in_storageLocation == CSCore::StorageLocation::k_package)
            {
                if(DoesDirectoryExistInPackage(in_directoryPath))
                {
                    return true;
                }
                
                return false;
            }
            
            //get the filepath
            std::string path = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
            
            //if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
            if (in_storageLocation == CSCore::StorageLocation::k_DLC)
            {
                if (DoesItemExistInDLCCache(in_directoryPath, true) == true)
                {
                    return true;
                }
                
                return DoesDirectoryExist(CSCore::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
            }
            
            //return whether or not the dir exists
			return CSBackend::iOS::DoesDirectoryExist(CSCore::StringUtils::StandardiseDirectoryPath(path));
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::string FileSystem::GetAbsolutePathToStorageLocation(CSCore::StorageLocation in_storageLocation) const
        {
            //get the storage location path
            std::string strStorageLocationPath;
            switch (in_storageLocation)
            {
                case CSCore::StorageLocation::k_package:
                    strStorageLocationPath = m_bundlePath + "AppResources/";
                    break;
                case CSCore::StorageLocation::k_chilliSource:
                    strStorageLocationPath = m_bundlePath + "CSResources/";
                    break;
                case CSCore::StorageLocation::k_saveData:
                    strStorageLocationPath = m_documentsPath + k_saveDataPath;
                    break;
                case CSCore::StorageLocation::k_cache:
                    strStorageLocationPath = m_libraryPath + k_cachePath;
                    break;
                case CSCore::StorageLocation::k_DLC:
                    strStorageLocationPath = m_libraryPath + k_dlcPath;
                    break;
                default:
                    CS_LOG_ERROR("Storage Location not available on this platform!");
                    break;
            }
            
            return strStorageLocationPath;
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
                        if(CSBackend::iOS::DoesFileExist(filePath) == true)
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
                        std::string filePath = CSCore::StringUtils::StandardiseDirectoryPath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_directoryPath);
                        if(CSBackend::iOS::DoesDirectoryExist(filePath) == true)
                        {
                            return filePath;
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
        void FileSystem::CreatePackageManifest()
        {
            @autoreleasepool
            {
                NSMutableArray* contents = [NSMutableArray array];
                NSString* directory = [NSStringUtils newNSStringWithUTF8String:m_bundlePath + "AppResources/"];
                
                NSError* error = nil;
                [contents addObjectsFromArray:[[NSFileManager defaultManager] subpathsOfDirectoryAtPath:directory error:&error]];
                if (error != nil)
                {
                    CS_LOG_FATAL("File System: Failed to create package manfest - " + GetErrorString(error));
                }
                
                for(NSString* nsPath in contents)
                {
                    BOOL isDirectory = NO;
                    NSFileManager* fileManager = [NSFileManager defaultManager];
                    NSString* nsFullPath = [NSString stringWithFormat:@"%@%@", directory, nsPath];
                    
                    if ([fileManager fileExistsAtPath:nsFullPath isDirectory:&isDirectory] == true)
                    {
                        std::string path([nsPath UTF8String]);
                        PackageManifestItem item;
                        if(isDirectory == NO)
                        {
                            item.m_pathHash = CSCore::HashCRC32::GenerateHashCode(CSCore::StringUtils::StandardiseFilePath(path));
                        }
                        else
                        {
                            item.m_pathHash = CSCore::HashCRC32::GenerateHashCode(CSCore::StringUtils::StandardiseDirectoryPath(path));
                        }
                        item.m_isFile = (isDirectory == NO);
                        m_packageManifestItems.push_back(item);
                    }
                }
                
                std::sort(m_packageManifestItems.begin(), m_packageManifestItems.end(), [](const FileSystem::PackageManifestItem& in_lhs, const FileSystem::PackageManifestItem& in_rhs)
                {
                    return in_lhs.m_pathHash < in_rhs.m_pathHash;
                });
                
                [directory release];
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::TryGetPackageManifestItem(const std::string& in_path, PackageManifestItem& out_manifestItem) const
        {
            PackageManifestItem searchItem;
			searchItem.m_pathHash = CSCore::HashCRC32::GenerateHashCode(in_path);
            
			auto it = std::lower_bound(m_packageManifestItems.begin(), m_packageManifestItems.end(), searchItem, [](const FileSystem::PackageManifestItem& in_lhs, const FileSystem::PackageManifestItem& in_rhs)
            {
                return in_lhs.m_pathHash < in_rhs.m_pathHash;
            });
            
			if(it !=  m_packageManifestItems.end() && it->m_pathHash == searchItem.m_pathHash)
			{
				out_manifestItem = *it;
				return true;
			}
            
			return false;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesFileExistInPackage(const std::string& in_filePath) const
        {
            PackageManifestItem item;
            if (TryGetPackageManifestItem(CSCore::StringUtils::StandardiseFilePath(in_filePath), item) == true)
            {
                if (item.m_isFile == true)
                {
                    return true;
                }
            }
            
            return false;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesDirectoryExistInPackage(const std::string& in_directoryPath) const
        {
            PackageManifestItem item;
            if (TryGetPackageManifestItem(CSCore::StringUtils::StandardiseDirectoryPath(in_directoryPath), item) == true)
            {
                if (item.m_isFile == false)
                {
                    return true;
                }
            }
            
            return false;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesItemExistInDLCCache(const std::string& in_path, bool in_isDirectory) const
        {
            if(in_isDirectory == true)
            {
                return CSBackend::iOS::DoesDirectoryExist(CSCore::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_path));
            }
            else
            {
                return CSBackend::iOS::DoesFileExist(CSCore::StringUtils::StandardiseFilePath(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_path));
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::vector<std::string> FileSystem::GetPossibleAbsoluteDirectoryPaths(CSCore::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
        {
            std::vector<std::string> output;
            switch(in_storageLocation)
            {
                case CSCore::StorageLocation::k_package:
                {
                    output.push_back(GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath);
                    break;
                }
                case CSCore::StorageLocation::k_DLC:
                {
                    output.push_back(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_package) + GetPackageDLCPath() + in_directoryPath);
                    output.push_back(GetAbsolutePathToStorageLocation(CSCore::StorageLocation::k_DLC) + in_directoryPath);
                    break;
                }
                default:
                {
                    output.push_back(GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath);
                    break;
                }
            }
            return output;
        }
	}
}

#endif