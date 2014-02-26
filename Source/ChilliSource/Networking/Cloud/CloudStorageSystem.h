//
//  CloudStorageSystem.h
//  MMCoHMoFlow
//
//  Created by Hugh McLaughlin on 13/07/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLO_CLOUDSTORAGESYSTEM_H
#define _MOFLO_CLOUDSTORAGESYSTEM_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource 
{
    namespace Networking
    {
        class ICloudStorageSystem : public ChilliSource::Core::ISystem
        {
        public:
            
            enum class CloudStorageArea
            {
                k_sharedDocuments,
                k_sharedSaveData,
                k_sharedCache,
                k_total
            };
            
            struct FileSyncConflict 
            {
                std::string mstrLocalFilePath;
                std::string mstrCloudFilePath;
                
                ChilliSource::Core::StorageLocation meLocalFileLocation;
                CloudStorageArea meCloudStorageLocation;
                 
                std::string mstrLocalContents;
                std::string mstrCloudContents;
                
                FileSyncConflict(ChilliSource::Core::StorageLocation ineLocalStorageLocation, const std::string& instrFile, CloudStorageArea ineCloudStorageLocation, const std::string& instrCloudFile,
                                 const std::string& instrLocalContents, const std::string& instrCloudContents)
                {
                    mstrLocalFilePath = instrFile;
                    mstrCloudFilePath = instrCloudFile;
                    
                    meLocalFileLocation = ineLocalStorageLocation;
                    meCloudStorageLocation = ineCloudStorageLocation;
                    
                    mstrCloudContents = instrCloudContents;
                    mstrLocalContents = instrLocalContents;
                }
                
                bool operator==(FileSyncConflict* insFileConflict)
                {
                    if(this->mstrCloudContents == insFileConflict->mstrCloudContents &&
                       this->mstrCloudFilePath == insFileConflict->mstrCloudFilePath &&
                       this->mstrLocalContents == insFileConflict->mstrLocalContents &&
                       this->mstrLocalFilePath == insFileConflict->mstrLocalFilePath &&
                       this->meLocalFileLocation == insFileConflict->meLocalFileLocation &&
                       this->meCloudStorageLocation == insFileConflict->meCloudStorageLocation)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                
                bool operator!=(FileSyncConflict* insFileConflict)
                {
                    if(this->mstrCloudContents != insFileConflict->mstrCloudContents ||
                       this->mstrCloudFilePath != insFileConflict->mstrCloudFilePath ||
                       this->mstrLocalContents != insFileConflict->mstrLocalContents ||
                       this->mstrLocalFilePath != insFileConflict->mstrLocalFilePath ||
                       this->meLocalFileLocation != insFileConflict->meLocalFileLocation ||
                       this->meCloudStorageLocation != insFileConflict->meCloudStorageLocation)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            };
            
            enum class FileConflictChoice
            {
                k_copyLocalToCloud,
                k_copyCloudToLocal,
                k_doNothing
            };
            
            //--------------------------------------------------------------------------------------------------------//
            //                                  Typedefs                                                              //
            //--------------------------------------------------------------------------------------------------------//
            typedef std::vector<std::string> ICloudFileList;
            
            typedef fastdelegate::FastDelegate1<ICloudFileList> OnQueryFilesCompletedDelegate;
            
            typedef fastdelegate::FastDelegate0<> OnSyncFileCompletedDelegate;
            typedef fastdelegate::FastDelegate3<FileConflictChoice, FileSyncConflict*, OnSyncFileCompletedDelegate> OnConflictResolvedDelegate;
            
            //The system will pass a delegate for the calling class to call when resolved
            typedef fastdelegate::FastDelegate2<OnConflictResolvedDelegate, FileSyncConflict*> OnSyncConflictDelegate;
            
            //--------------------------------------------------------------------------------------------------------//
            
            DECLARE_NAMED_INTERFACE(ICloudStorageSystem);
            
            virtual ~ICloudStorageSystem(){}
            
            //Returns if the client meets the minimum OS version to use this feature
            static bool IsSupported() { return false; }
            
            //Returns if iCloud has been enabled on the client device
            virtual bool IsCloudStorageEnabled() const = 0;
            
            //Performs any initialisation required
            virtual void Initialise() = 0;
            
            //If cloud service is enabled returns the path to the storage on device
            virtual std::string GetCloudStoragePath() const = 0;
            
            //////////////////////
            //  File Queries    //
            //////////////////////
            
            virtual void QueryForAllCloudFiles() = 0;
            
            //////////////////////
            // Delegate Settors //
            //////////////////////

            void SetOnQueryFilesCompletedDelegate(OnQueryFilesCompletedDelegate incDelegate) { mOnQueryFilesCompletedDelegate = incDelegate; }
        
        protected:
            
            
            virtual void CreateDirectory(CloudStorageArea ineStorageArea, const std::string& instrDirectory) = 0;
            
            virtual void CreateFile(CloudStorageArea ineStorageArea, const std::string& instrFileName, const std::string& inpData) = 0;
            
            virtual void DeleteFile(CloudStorageArea ineStorageArea, const std::string& instrFileName) = 0;
            
            //Helper function to return a string version of the enum for use in creating the url
            const std::string GetStringForStorageArea(CloudStorageArea ineArea) const
            {
                switch (ineArea)
                {
                    case CloudStorageArea::k_sharedDocuments:
                        return "SharedDocuments";
                        break;
                    case CloudStorageArea::k_sharedCache:
                        return "SharedCache";
                        break;
                    case CloudStorageArea::k_sharedSaveData:
                        return "SharedSaveData";
                        break;
                    default:
                        return "SharedDocuments";
                        break;
                }
            }
            
            const CloudStorageArea GetCloudStorageAreaForStorageArea(ChilliSource::Core::StorageLocation ineStorageArea)
            {
                switch (ineStorageArea) {
                    case Core::StorageLocation::k_package:
                        return CloudStorageArea::k_sharedDocuments;
                    case Core::StorageLocation::k_saveData:
                        return CloudStorageArea::k_sharedSaveData;
                    case Core::StorageLocation::k_cache:
                        return CloudStorageArea::k_sharedCache;
                    default:
                        return CloudStorageArea::k_sharedDocuments;
                }
            }
            
            
            std::vector<FileSyncConflict*> mvsCachedConflicts;
            
            OnQueryFilesCompletedDelegate mOnQueryFilesCompletedDelegate;
        };
    }
}

#endif
