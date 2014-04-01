//
//  FileSystem.cpp
//  Chilli Source
//
//  Created by I Copland on 25/03/2011.
//  Copyright 2011 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>

#include <ChilliSource/Core/String/StringUtils.h>

#include <iostream>
#include <UIKit/UIKit.h>
#include <sys/types.h>
#include <sys/sysctl.h>

namespace ChilliSource
{
	namespace iOS 
	{
        namespace
        {
            const std::string k_saveDataPath  = "SaveData/";
            const std::string k_cachePath  = "Caches/Cache/";
            const std::string k_dlcPath  = "Caches/DLC/";
            
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
            //------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Directories
            /// @param Recurse into sub directories
            /// @param Out: Content file names
            //------------------------------------------------------------
            void GetDirectoryContents(const std::vector<std::string>& in_directoryPaths, bool in_recursive, NSMutableArray* out_contents)
            {
                for(std::vector<std::string>::const_iterator it = in_directoryPaths.begin(); it != in_directoryPaths.end(); ++it)
                {
                    std::string path = ChilliSource::Core::StringUtils::StandardisePath(*it);
                    NSString* Dir = [NSString stringWithCString:path.c_str() encoding:NSASCIIStringEncoding];
                    
                    if (in_recursive == true)
                    {
                        [out_contents addObjectsFromArray:[[NSFileManager defaultManager] subpathsOfDirectoryAtPath:Dir error:nil]];
                    }
                    else
                    {
                        [out_contents addObjectsFromArray:[[NSFileManager defaultManager] contentsOfDirectoryAtPath:Dir error:nil]];
                    }
                }
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
            /// @author I Copland
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
                    output.push_back(ChilliSource::Core::StringUtils::NSStringToString(filePath));
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
                BOOL bDirectory = NO;
                bool bExists = false;
                NSFileManager *fileManager= [NSFileManager defaultManager];
                NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
                
                if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:in_filePath.c_str()] isDirectory:&bDirectory])
                {
                    bExists = true;
                }
                
                [pPool release];
                return (bExists && !bDirectory);
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
                BOOL bDirectory = NO;
                bool bExists = false;
                NSFileManager *fileManager= [NSFileManager defaultManager];
                NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
                
                if ([fileManager fileExistsAtPath:[NSString stringWithUTF8String:in_directoryPath.c_str()] isDirectory:&bDirectory])
                {
                    bExists = true;
                }
                
                [pPool release];
                return (bExists && bDirectory);
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
            
            CreateDirectory(Core::StorageLocation::k_saveData, "");
            CreateDirectory(Core::StorageLocation::k_cache, "");
            CreateDirectory(Core::StorageLocation::k_DLC, "");
            
            CreateHashedBundleFileList();
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
            Core::FileStreamUPtr fileStream = Core::FileStreamUPtr(new Core::FileStream());
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
        bool FileSystem::CreateDirectory(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const //<--- Continue here!
        {
            CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to write to read only storage location.");
            
            //create the directory
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            std::string path = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
			NSFileManager* fileManager = [NSFileManager defaultManager];
            if (![fileManager fileExistsAtPath:[NSString stringWithUTF8String:path.c_str()]])
            {
                if (![fileManager createDirectoryAtPath:[NSString stringWithUTF8String:path.c_str()] withIntermediateDirectories:YES attributes:nil error:nil])
                {
                    CS_LOG_ERROR("File System: Error creating directory '" + in_directoryPath + "'");
                    [pPool release];
                    return false;
                }
            }
            [pPool release];
            
            //return successful
			return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::CopyFile(Core::StorageLocation in_sourceStorageLocation, const std::string& in_sourceFilePath,
                                   Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationFilePath) const
        {
            CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");
            
            if(DoesFileExist(in_sourceStorageLocation, in_sourceFilePath) == false)
            {
                CS_LOG_ERROR("File System: Trying to copy file '" + in_sourceFilePath + "' but it does not exist.");
                return false;
            }
            
            std::string strSrcPath = GetAbsolutePathToFile(in_sourceStorageLocation, in_sourceFilePath);
            
            //get the path to the file
            std::string strPath, strName;
            Core::StringUtils::SplitFilename(in_destinationFilePath, strName, strPath);
            
            //create the output directory
            CreateDirectory(in_destinationStorageLocation, strPath);
            
            std::string strDstPath = GetAbsolutePathToStorageLocation(in_destinationStorageLocation) + in_destinationFilePath;
            std::string strDstAtomicPath = strDstPath + ".tmp";
            
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            
			NSString* pSrcPath = [[NSString alloc] initWithCString:strSrcPath.c_str() encoding:NSUTF8StringEncoding];
            NSString* pDstPath = [[NSString alloc] initWithCString:strDstPath.c_str() encoding:NSUTF8StringEncoding];
            NSString* pDstAtomicPath = [[NSString alloc] initWithCString:strDstAtomicPath.c_str() encoding:NSUTF8StringEncoding];
            
            NSURL* pDstURL = [NSURL URLWithString:[pDstPath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
            NSURL* pDstAtomicURL = [NSURL URLWithString:[pDstAtomicPath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
            
            NSError* pCopyError = nil;
            [[NSFileManager defaultManager] copyItemAtPath:pSrcPath toPath:pDstAtomicPath error:&pCopyError];
    
            NSError* pReplaceError = nil;
            [[NSFileManager defaultManager] replaceItemAtURL:pDstURL withItemAtURL:pDstAtomicURL backupItemName:nil options:NSFileManagerItemReplacementUsingNewMetadataOnly resultingItemURL:nil error:&pReplaceError];
            
            [pSrcPath release];
            [pDstPath release];
            [pDstAtomicPath release];
			[pPool release];
            
            return pCopyError == nil && pReplaceError == nil;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::CopyDirectory(Core::StorageLocation in_sourceStorageLocation, const std::string& in_sourceDirectoryPath,
                                        Core::StorageLocation in_destinationStorageLocation, const std::string& in_destinationDirectoryPath) const
        {
            CS_ASSERT(IsStorageLocationWritable(in_destinationStorageLocation), "File System: Trying to write to read only storage location.");
            
            //get all the files in the directory
            std::vector<std::string> astrFilenames = GetFilePaths(in_sourceStorageLocation, in_sourceDirectoryPath, true);
            
            //error if there are no files
            if (astrFilenames.size() == 0)
            {
                CS_LOG_ERROR("File System: Trying to copy directory '" + in_sourceDirectoryPath + "' but it is empty or doesn't exist.");
                return false;
            }
            
            //copy each of these files individually
            std::string sourcePath = Core::StringUtils::StandardisePath(in_sourceDirectoryPath);
            std::string destPath = Core::StringUtils::StandardisePath(in_destinationDirectoryPath);
            for (std::vector<std::string>::iterator it = astrFilenames.begin(); it != astrFilenames.end(); ++it)
            {
                if (CopyFile(in_sourceStorageLocation, sourcePath + *it, in_destinationStorageLocation, destPath + *it) == false)
                {
                    return false;
                }
            }
            
            return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DeleteFile(Core::StorageLocation in_storageLocation, const std::string& in_FilePath) const
        {
            CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");
            
            //get the filepath
            std::string strPath = GetAbsolutePathToStorageLocation(in_storageLocation) + in_FilePath;
            
            //remove the file
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
			CFStringRef cfStringRef = CFStringCreateWithCString(kCFAllocatorDefault, strPath.c_str(), kCFStringEncodingMacRoman);
            [[NSFileManager defaultManager] removeItemAtPath:(NSString*)cfStringRef error:nil];
			CFRelease(cfStringRef); 
			[pPool release];
            
            //return successful
            return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DeleteDirectory(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
        {
            CS_ASSERT(IsStorageLocationWritable(in_storageLocation), "File System: Trying to delete from a read only storage location.");
            
            //get the directory
            std::string strDirectory = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
            
            //remove the directory
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
			CFStringRef cfStringRef = CFStringCreateWithCString(kCFAllocatorDefault, strDirectory.c_str(), kCFStringEncodingMacRoman);
            [[NSFileManager defaultManager] removeItemAtPath:(NSString*)cfStringRef error:nil];
			CFRelease(cfStringRef); 
			[pPool release];
            
            //return successful
            return true;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::vector<std::string> FileSystem::GetFilePaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
        {
            std::vector<std::string> astrDirectoriesToCheck;
            GetPathsForStorageLocation(in_storageLocation, in_directoryPath, astrDirectoriesToCheck);
            
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            NSMutableArray* Contents = [[NSMutableArray alloc] init];
            
            GetDirectoryContents(astrDirectoriesToCheck, in_recursive, Contents);
            
            std::vector<std::string> output;
            if([Contents count] > 0)
            {
                NSArray* Filtered = FilterPathsByFile(Contents);
                output = ConvertObjCToPath(Filtered);
            }
            
            [Contents release];
            [pPool release];
            
            std::sort(output.begin(), output.end());
            std::vector<std::string>::iterator it = std::unique(output.begin(), output.end());
            output.resize(it - output.begin());
            return output;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        std::vector<std::string> FileSystem::GetDirectoryPaths(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath, bool in_recursive) const
        {
            std::vector<std::string> astrDirectoriesToCheck;
            GetPathsForStorageLocation(in_storageLocation, in_directoryPath, astrDirectoriesToCheck);
            
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            NSMutableArray* Contents = [[NSMutableArray alloc] init];
            
            GetDirectoryContents(astrDirectoriesToCheck, in_recursive, Contents);
            
            std::vector<std::string> output;
            if([Contents count] > 0)
            {
                NSArray* Filtered = FilterPathsByDirectory(Contents);
                output = ConvertObjCToPath(Filtered);
            }
            
            [Contents release];
            [pPool release];
            
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
                for(u32 i = 0; i < 3; ++i)
                {
                    if(DoesFileExistInHashedStore(resourceDirectories[i] + in_filePath))
                    {
                        return true;
                    }
                }
                
                return false;
            }
            
            //get the filepath
            std::string path = GetAbsolutePathToStorageLocation(in_storageLocation) + in_filePath;
            
            //if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
            if (in_storageLocation == Core::StorageLocation::k_DLC)
            {
                if (DoesItemExistInDLCCache(in_filePath, false) == true)
                {
                    return true;
                }
                
                return DoesFileExist(Core::StorageLocation::k_package, GetPackageDLCPath() + in_filePath);
            }
            
            //return whether or not the file exists
			return iOS::DoesFileExist(Core::StringUtils::StandardisePath(path));
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
            return iOS::DoesFileExist(ChilliSource::Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_package) + GetPackageDLCPath() + in_filePath));
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesDirectoryExist(Core::StorageLocation in_storageLocation, const std::string& in_directoryPath) const
        {
            if(in_storageLocation == Core::StorageLocation::k_package)
            {
                const std::string* resourceDirectories = GetResourceDirectories();
                for (u32 i = 0; i < 3; ++i)
                {
                    if(iOS::DoesDirectoryExist(m_bundlePath + resourceDirectories[i] + in_directoryPath))
                    {
                        return true;
                    }
                }
                
                return false;
            }
            
            //get the filepath
            std::string path = GetAbsolutePathToStorageLocation(in_storageLocation) + in_directoryPath;
            
            //if its a DLC stream, make sure that it exists in the DLC cache, if not fall back on the package
            if (in_storageLocation == Core::StorageLocation::k_DLC)
            {
                if (DoesItemExistInDLCCache(in_directoryPath, true) == true)
                {
                    return true;
                }
                
                return DoesDirectoryExist(Core::StorageLocation::k_package, GetPackageDLCPath() + in_directoryPath);
            }
            
            //return whether or not the dir exists
			return iOS::DoesDirectoryExist(Core::StringUtils::StandardisePath(path));
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
                    strStorageLocationPath = m_bundlePath;
                    break;
                case Core::StorageLocation::k_saveData:
                    strStorageLocationPath = m_documentsPath + k_saveDataPath;
                    break;
                case Core::StorageLocation::k_cache:
                    strStorageLocationPath = m_libraryPath + k_cachePath;
                    break;
                case Core::StorageLocation::k_DLC:
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
        std::string FileSystem::GetAbsolutePathToFile(Core::StorageLocation in_storageLocation, const std::string& in_filePath) const
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
        bool FileSystem::DoesItemExistInDLCCache(const std::string& in_path, bool in_isFolder) const
        {
            //return whether or not the file exists
            if(in_isFolder)
            {
                return iOS::DoesDirectoryExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_path));
            }
            else
            {
                return iOS::DoesFileExist(Core::StringUtils::StandardisePath(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_path));
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        bool FileSystem::DoesFileExistInHashedStore(const std::string& in_filePath) const
        {
            u32 udwHashedFile = Core::HashCRC32::GenerateHashCode(in_filePath);
            
            std::vector<u32>::const_iterator it = std::lower_bound(m_hashedPackageFilePaths.begin(), m_hashedPackageFilePaths.end(), udwHashedFile);
            
            if(it!= m_hashedPackageFilePaths.end() && *it == udwHashedFile)
                return true;

            return false;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void FileSystem::GetPathsForStorageLocation(Core::StorageLocation in_storageLocation, const std::string& in_path, std::vector<std::string>& out_paths) const
        {
            switch(in_storageLocation)
            {
                case Core::StorageLocation::k_package:
                {
                    const std::string* resourceDirectories = GetResourceDirectories();
                    for(u32 i = 0; i < 3; ++i)
                    {
                        out_paths.push_back(GetAbsolutePathToStorageLocation(in_storageLocation) + resourceDirectories[i] + in_path);
                    }
                    break;
                }
                case Core::StorageLocation::k_DLC:
                {
                    const std::string* resourceDirectories = GetResourceDirectories();
                    for(u32 i = 0; i < 3; ++i)
                    {
                        out_paths.push_back(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_package) + resourceDirectories[i] + GetPackageDLCPath() + in_path);
                    }
                    out_paths.push_back(GetAbsolutePathToStorageLocation(Core::StorageLocation::k_DLC) + in_path);
                    break;
                }
                default:
                {
                    out_paths.push_back(GetAbsolutePathToStorageLocation(in_storageLocation) + in_path);
                    break;
                }
            }
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void FileSystem::CreateHashedBundleFileList()
        {
            NSAutoreleasePool* pPool = [[NSAutoreleasePool alloc] init];
            NSMutableArray* Contents = [[NSMutableArray alloc] init];
            NSString* Dir = [NSString stringWithCString:m_bundlePath.c_str() encoding:NSASCIIStringEncoding];
            
            [Contents addObjectsFromArray:[[NSFileManager defaultManager] subpathsOfDirectoryAtPath:Dir error:nil]];
            
            for(NSString* file in Contents)
            {
                std::string strFile([file UTF8String]);
                m_hashedPackageFilePaths.push_back(Core::HashCRC32::GenerateHashCode(strFile));
            }
            
            std::sort(m_hashedPackageFilePaths.begin(), m_hashedPackageFilePaths.end());
            
            [Contents release];
            [pPool release];
        }
	}
}
