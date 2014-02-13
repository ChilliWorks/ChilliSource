//
//  ContentManagementSystem.h
//  moFloTest
//
//  Created by Scott Downie on 04/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <moFlo/Networking/ForwardDeclarations.h>
#include <moFlo/Networking/ContentDownloader.h>

#include <moFlo/Core/System.h>
#include <moFlo/Core/GenericEvent.h>
#include <moFlo/Core/XML/XMLUtils.h>

namespace moFlo
{
    namespace Networking
    {
        class CContentManagementSystem : public Core::ISystem
        {
        public:
            DECLARE_NAMED_INTERFACE(CContentManagementSystem);
            
            enum UpdateResult
            {
                UPDATE_AVAILABLE,               //There are updates available to download and we are connected
                UPDATE_AVAILABLE_BLOCKING,      //There are updates available to download and we must download them as the old assets have been purged
                UPDATE_CHECK_FAILED,            //The downloading of the content manifest has failed
                UPDATE_CHECK_FAILED_BLOCKING,   //The DLC on the cache has been cleared and a download is required but the manifest has failed
                UPDATE_NOT_AVAILABLE,           //There are no updates available
            };
            
            enum Result
            {
                CONTENT_SUCCEEDED,
                CONTENT_FAILED
            };
            
            typedef fastdelegate::FastDelegate1<UpdateResult> CheckForUpdateDelegate;
            typedef fastdelegate::FastDelegate1<Result> CompleteDelegate;
			
            //-----------------------------------------------------------
            /// Constructor
            ///
            /// @param Content downloader
            //-----------------------------------------------------------
            CContentManagementSystem(IContentDownloader* inpContentDownloader);
            //-----------------------------------------------------------
            /// Is A
            ///
            /// @param Interface ID to compare
            /// @return Whether system is of that type
            //-----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //-----------------------------------------------------------
            /// Check For Updates
            ///
            /// Get the content manifest from the asset server and
            /// check for any changes with the local copy
            ///
            /// The location of the asset server is usually the Tag 
            /// server but may be a third party storage solution based on
            /// the SKU (i.e. Google's Asset Store). The URL is taken
            /// from the tweak constants
            ///
            /// @param Delegate to callback notifying whether an update
            /// is required
            //-----------------------------------------------------------
            void CheckForUpdates(const CheckForUpdateDelegate& inDelegate);
            //-----------------------------------------------------------
            /// Download Updates
            ///
            /// Having checked for updates and been notified whether
            /// there are updates pending this function should be
            /// called to begin downloading any out of date content
            ///
            /// Call GetDownloadProgress to get the progress value
            /// to update any progress UI
            ///
            /// @param Delegate to call when download is complete
            //-----------------------------------------------------------
            void DownloadUpdates(const CompleteDelegate& inDelegate);
            //-----------------------------------------------------------
            /// Install Updates
            ///
            /// Having downloaded the update packages this method
            /// unzips the packages and overwrites any old assets
            ///
            /// @param Delegate to call when Install is complete
            //-----------------------------------------------------------
            void InstallUpdates(const CompleteDelegate& inDelegate);
            //-----------------------------------------------------------
            /// Clear Download Data
            ///
            /// Data is cached in memory and only written to disc
            /// when the download is successful; at which point
            /// we can clear the data and delete any temp files
            //-----------------------------------------------------------
            void ClearDownloadData();
            //-----------------------------------------------------------
            /// Get Manifest Checksum for File
            ///
            /// @param File name
            /// @return Checksum of file as found in the current manifest
            //-----------------------------------------------------------
            static std::string GetManifestChecksumForFile(const std::string& instrFilename);
			
			//-----------------------------------------------------------
			/// Get Running Total To Download
			///
			/// Returns the current running total of the size of data to download
			///
			/// @return The size of the data needing to be downloaded
			//-----------------------------------------------------------			
			u32 GetRunningTotalToDownload();
			
			//-----------------------------------------------------------
			/// Get Running Total Downloaded
			///
			/// Returns the current running total of the size of data downloaded
			///
			/// @return The current running total of the size of data downloaded
			//-----------------------------------------------------------
			u32 GetRunningTotalDownloaded();
            
			//-----------------------------------------------------------
			/// Get Content Downloader
			///
			/// @return The current content downloader
			//-----------------------------------------------------------
			inline IContentDownloader* GetContentDownloader() const { return mpContentDownloader; }
            
        private:
            
            struct PackageDetails
            {
                std::string strURL;
                std::string strID;
                std::string strChecksum;
                u32 udwSize;
            };
            
            //-----------------------------------------------------------
            /// Load Local Manifest
            ///
            /// Load the client side cached content manifest if one
            /// exists
            ///
            /// @param TiXmlDocument
            //-----------------------------------------------------------
            void LoadLocalManifest(TiXmlDocument* inpCurrentManifest);
            //-----------------------------------------------------------
            /// On Content Manifest Download Complete
            ///
            /// The manifest file has downloaded we can now compare
            /// and contrast to check for outdated files
            ///
            /// @param Request result
            /// @param Request response
            //-----------------------------------------------------------
            void OnContentManifestDownloadComplete(ContentDownloader::Result ineResult, const std::string& instrManifest);
            //-----------------------------------------------------------
            /// On Content Download Complete
            ///
            /// The file has downloaded we can now save it to the the 
            /// cache
            ///
            /// @param Request result
            /// @param Request response
            //-----------------------------------------------------------
            void OnContentDownloadComplete(ContentDownloader::Result ineResult, const std::string& instrData);
            //-----------------------------------------------------------
            /// Build Download List
            ///
            /// Check if an existing content manifest exists and
            /// construct a list of the files that require updating
            /// then overwrite the old manifest with the new one
            ///
            /// @param String containing the server manifest
            //-----------------------------------------------------------
            void BuildDownloadList(const std::string& instrServerManifest);
            //-----------------------------------------------------------
            /// Add To Download List if not in Bundle
            ///
            /// The package may be outdated in documents but are
            /// all the files in bundle up to date
            ///
            /// @param Package element
            //-----------------------------------------------------------
            void AddToDownloadListIfNotInBundle(TiXmlElement* pPackageEl);
            //-----------------------------------------------------------
            /// Save Package To File
            ///
            /// Save the zip file to documents directory
            ///
            /// @param Package details
            /// @param Binary zip
            /// @param Whether the file has finished downloading
            /// @return Success
            //-----------------------------------------------------------
            bool SavePackageToFile(const PackageDetails& insPackageDetails, const std::string& instrZippedPackage, bool inbFullyDownloaded);
            //-----------------------------------------------------------
            /// Extract Files From Package
            ///
            /// Unzip the package and save all the files to the
            /// documents directory
            ///
            /// @param Zipped package
            //-----------------------------------------------------------
            void ExtractFilesFromPackage(const PackageDetails& insPackageDetails) const;
            //-----------------------------------------------------------
			/// Delete Directory
			///
			/// Deletes a directory from the DLC Storage Location.
			///
			/// @return The directory
			//-----------------------------------------------------------
			void DeleteDirectory(const std::string& instrDirectory) const;
            //-----------------------------------------------------------
            /// Does File Exist
            ///
            /// Checks whether the file is within the application and if the
            /// the checksums match
            ///
            /// @param Filename
            /// @param Checksum
            /// @param Whether to check only the bundle
            ///
            /// @return Whether the file exists
            //-----------------------------------------------------------
            static bool DoesFileExist(const std::string& instrFilename, const std::string instrChecksum, bool inbCheckOnlyBundle);
            //-----------------------------------------------------------
            /// Calculate Checksum
            ///
            /// Calculate a checksum for the file. This involves
            /// performing an MD5 hash of the file and converting that
            /// to base 64 encoded and then trimming the trailing '='
            ///
            /// @param File location
            /// @param File path
            /// @return Checksum string
            //-----------------------------------------------------------
            static std::string CalculateChecksum(Core::STORAGE_LOCATION ineLocation, const std::string& instrFilePath);
            //-----------------------------------------------------------
            /// Download Next Package
            ///
            /// Perform the HTTP request for the next DLC package
            //-----------------------------------------------------------
            void DownloadNextPackage();

        private:
            
            DYNAMIC_ARRAY<std::string> mRemovePackageIDs; 
            DYNAMIC_ARRAY<PackageDetails> mPackageDetails;
			
			u32	muRunningToDownloadTotal;
			u32 muRunningDownloadedTotal;
            
            TiXmlDocument* mpServerManifest;
            
            IContentDownloader* mpContentDownloader;
            
            CheckForUpdateDelegate mOnUpdateCheckCompleteDelegate;
            CompleteDelegate mOnDownloadCompleteDelegate;
            
            std::string mstrServerManifestData;
            std::string mstrContentDirectory;
			
            u32 mudwCurrentPackageDownload;
            
            bool mbDLCCachePurged;
        };
    }
}
