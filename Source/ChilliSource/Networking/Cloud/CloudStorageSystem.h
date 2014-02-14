//
//  CloudStorageSystem.h
//  MMCoHMoFlow
//
//  Created by Hugh McLaughlin on 13/07/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLO_CLOUDSTORAGESYSTEM_H
#define _MOFLO_CLOUDSTORAGESYSTEM_H

#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/Base/FastDelegate.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace moFlo 
{
    class ICloudStorageSystem : public moFlo::Core::ISystem
    {
    public:
        
        enum CloudStorageArea { eSharedDocuments = 0, eSharedSaveData, eSharedCache, eSharedTotal } ;
        
        struct FileSyncConflict 
        {
            std::string mstrLocalFilePath;
            std::string mstrCloudFilePath;
            
            moFlo::Core::STORAGE_LOCATION meLocalFileLocation;
            CloudStorageArea meCloudStorageLocation;
             
            std::string mstrLocalContents;
            std::string mstrCloudContents;
            
            FileSyncConflict(moFlo::Core::STORAGE_LOCATION ineLocalStorageLocation, const std::string& instrFile, CloudStorageArea ineCloudStorageLocation, const std::string& instrCloudFile,
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
        
        enum FileConflictChoice { eCopyLocalToCloud, eCopyCloudToLocal, eDoNothing };
        
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
            switch (ineArea) {
                case eSharedDocuments:
                    return "SharedDocuments";
                    break;
                case eSharedCache:
                    return "SharedCache";
                    break;
                case eSharedSaveData:
                    return "SharedSaveData";
                    break;
                default:
                    return "SharedDocuments";
                    break;
            }
        }
        
        const CloudStorageArea GetCloudStorageAreaForStorageArea(moFlo::Core::STORAGE_LOCATION ineStorageArea)
        {
            switch (ineStorageArea) {
                case moFlo::Core::SL_PACKAGE:
                    return eSharedDocuments;
                case moFlo::Core::SL_SAVEDATA:
                    return eSharedSaveData;
                case moFlo::Core::SL_CACHE:
                    return eSharedCache;
                default:
                    return eSharedDocuments;
            }
        }
        
        
        std::vector<FileSyncConflict*> mvsCachedConflicts;
        
        OnQueryFilesCompletedDelegate mOnQueryFilesCompletedDelegate;
    };
}

#endif
