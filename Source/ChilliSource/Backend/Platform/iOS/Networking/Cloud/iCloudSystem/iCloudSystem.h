//
//  ICloudSystem.h
//  MMCoHMoFlow
//
//  Created by Hugh McLaughlin on 13/07/2012.
//  Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

#ifndef _MOFLO_PLATFORM_IOS_ICLOUDSYSTEM_H
#define _MOFLO_PLATFORM_IOS_ICLOUDSYSTEM_H

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/iOS/Networking/Cloud/iCloudSystem/MoFlowUIDocument.h>
#include <ChilliSource/Networking/Cloud/CloudStorageSystem.h>
#include <ChilliSource/Networking/Http/HttpConnectionSystem.h>

#import <UIKit/UIKit.h>

#include <map>

namespace ChilliSource
{
	namespace iOS
    {
        class iCloudSystem : public Networking::CloudStorageSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(iCloudSystem);
            
            typedef std::function<void(MoFlowUIDocument*, bool)> OnOpenCloudFileCompletedDelegate;
            
            iCloudSystem(ChilliSource::Networking::HttpConnectionSystem* inpcHttpConnectionSystem);
            
            //----------------------------------------------------------
            /// Is A
            ///
            /// Returns if it is of the type given
            /// @param Comparison Type
            /// @return Whether the class matches the comparison type
            //----------------------------------------------------------
            bool IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const override;
            
            //Returns if the client meets the minimum OS version to use this feature
            static bool IsSupported();
            
            //Returns if iCloud has been enabled on the client device
            bool IsCloudStorageEnabled() const override;
            
            //Performs any initialisation required
            void Initialise() override;

            //If cloud service is enabled returns the path to the storage on device
            std::string GetCloudStoragePath() const override;
            
            //Manually Refresh the cache of files stored in the cloud
            void RefreshCloudContent();

            //-------------------------------------------------------------
            /// GetHttpConnectionSystem
            ///
            /// Returns a pointer to the http connection system as used here.
            /// @return a pointer to the http connection system as used here
            ///
            //-------------------------------------------------------------            
            ChilliSource::Networking::HttpConnectionSystem* GetHttpConnectionSystem() const { return mpcHttpConnectionSystem; }
            
            //-------------------------------------------------------------
            /// GetAppendedFilePathToStorageArea
            ///Syncs a file on a storage location to its matching folder on Cloud storage, returns false if file to sync doesnt exist
            /// @param StorageLocation - area where the local file is stored, 
            /// @param FilePath - Path to file from StorageLocation
            /// @param SyncFileCompletedDelegate - Delegate to call when sync is complete
            /// @param SyncConflictDelegate - Delegate to call when conflict between local and cloud is detected (Note:: if none is provided the sync to cloud will be cancelled automatically)
            /// @return False if iCloud not enabled on system or < iOS 5.0
            //-------------------------------------------------------------
            bool SyncFileToCloud(ChilliSource::Core::StorageLocation ineStorageLocation, const std::string& instrFilePath, CloudStorageSystem::OnSyncFileCompletedDelegate ineSyncCompleteDelegate = nullptr, CloudStorageSystem::OnSyncConflictDelegate ineSyncConflictDelegate = nullptr);
            
            //////////////////////
            //  File Queries    //
            //////////////////////
            
            //Querys for files in a Storage area in the cloud, will invoke OnQueryFilesCompletedEvent when results are retrieved
            void QueryForAllCloudFiles() override;

        private:
            
            //Create Directory in Cloud
            void CreateDirectory(CloudStorageArea ineStorageArea, const std::string& instrDir) override;
            
            //Create file in cloud only, use SyncFileToCloud to maintain both locations
            void CreateFile(CloudStorageArea ineStorageArea, const std::string& instrFileName, const std::string& instrData) override;
            
            //Delete File from cloud only
            void DeleteFile(CloudStorageArea ineStorageArea, const std::string& instrFileName) override;
            
        private:
            
            //-------------------------------------------------------------
            /// CloudFileSyncRequest
            /// A struct to wrap up nessesary iCloud request data
            /// @param StorageLocation - Local storage area of file, 
            /// @param LocalFilePath - Path to file 
            /// @param OnSyncConflictDelegate - Function to call if a conflict is detected
            /// @param OnSyncFileCompletedDelegate - Function to call when the file is written succesfully
            //-------------------------------------------------------------
            struct CloudFileSyncRequest 
            {
                std::string mstrLocalFilePath;
                Core::StorageLocation meLocalStorageLocation;
                
                Networking::CloudStorageSystem::OnSyncConflictDelegate mpcSyncConflictDelegate;
                Networking::CloudStorageSystem::OnSyncFileCompletedDelegate mpcSyncCompletedDelegate;

                CloudFileSyncRequest(Core::StorageLocation ineStorageLocation, const std::string& instrLocalFilePath, Networking::CloudStorageSystem::OnSyncConflictDelegate incSyncConflictDelegate = nullptr,
                                     Networking::CloudStorageSystem::OnSyncFileCompletedDelegate incSyncCompletedDelegate = nullptr)
                {
                    mstrLocalFilePath = instrLocalFilePath;
                    meLocalStorageLocation = ineStorageLocation;
                    
                    mpcSyncCompletedDelegate = incSyncCompletedDelegate;
                    mpcSyncConflictDelegate = incSyncConflictDelegate;
                }
            };
            
            //Maps filenames that are to be synced with their delegate callbacks
            typedef std::map<std::string,  CloudFileSyncRequest> FileToSyncDelegateMap;
            

            //-------------------------------------------------------------
            /// GetAppendedFilePathToStorageArea
            ///Constructs a path from a storage area enum and a relative file path
            /// @param Area - area on cloud saving to, 
            /// @param RelativeFilePath - Additional path to file from Area directory
            /// @return Constructed Path
            //-------------------------------------------------------------
            const std::string GetAppendedFilePathToStorageArea(CloudStorageArea ineArea, const std::string& instrRelativeFilePath) const;
            
            //-------------------------------------------------------------
            /// QueryDidFinishGathering
            /// Called when QueryForAllCloudFiles completes
            /// @param invFileList - vector containing urls of all files currently in iCloud
            //-------------------------------------------------------------
            void QueryDidFinishGathering(Networking::CloudStorageSystem::ICloudFileList invFileList);
            
            //-------------------------------------------------------------
            /// OnCloudFileOpened
            /// Called when the ICloudSystemController opens a file
            /// @param incOpenedDoc - the MoFloUIDocument that has been opened 
            /// @param inbJustCreated - true if the file has just been created
            //-------------------------------------------------------------
            void OnCloudFileOpened(MoFlowUIDocument* incOpenedDoc, BOOL inbJustCreated);
            
            //-------------------------------------------------------------
            /// OnConflictResolved
            /// Called by the client when they resolve a conflict
            /// @param ineChoice - whether we want to keep local or cloud files 
            /// @param insFileSyncConflict - Struct containing details on the files in conflict
            /// @param inpSyncCompleteDelegate - delegate to call when the choice has been implemented
            //-------------------------------------------------------------
            void OnConflictResolved(Networking::CloudStorageSystem::FileConflictChoice ineChoice, Networking::CloudStorageSystem::FileSyncConflict* insFileSyncConflict, Networking::CloudStorageSystem::OnSyncFileCompletedDelegate inpSyncCompleteDelegate);
            
            FileToSyncDelegateMap mmFileToSyncDelegateMap;
            
            Networking::CloudStorageSystem::ICloudFileList mvCachedCloudFiles;
            
            NSFileManager *mpcFileManager;
            
            std::string mstrICloudDirectoryPath;
            
            Networking::HttpConnectionSystem* mpcHttpConnectionSystem;
        };
    }
}

#endif
