//
//  ContentManagementSystem.h
//  Chilli Source
//
//  Created by S Downie on 04/07/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_NETWORKING_CONTENTDOWNLOAD_CONTENTMANAGEMENTSYSTEM_H_
#define _CHILLISOURCE_NETWORKING_CONTENTDOWNLOAD_CONTENTMANAGEMENTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/XML/XMLUtils.h>
#include <ChilliSource/Networking/ContentDownload/IContentDownloader.h>

namespace ChilliSource
{
    namespace Networking
    {
        //---------------------------------------------------------------
        /// A system for managing the downloading of content to the
        /// DLC storage location.
        ///
        /// @author S Downie
        //---------------------------------------------------------------
        class ContentManagementSystem final : public Core::AppSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(ContentManagementSystem);
            //--------------------------------------------------------
            /// An enum descibing the possible results from a Check
            /// For Updates request.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            enum class CheckForUpdatesResult
            {
                k_available,             //There are updates available to download and we are connected
                k_availableBlocking,     //There are updates available to download and we must download them as the old assets have been purged
                k_checkFailed,           //The downloading of the content manifest has failed
                k_checkFailedBlocking,   //The DLC on the cache has been cleared and a download is required but the manifest has failed
                k_notAvailable,          //There are no updates available
            };
            //--------------------------------------------------------
            /// An enum descibing the possible results from a content
            /// download request.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            enum class Result
            {
                k_succeeded,
                k_failed
            };
            //--------------------------------------------------------
            /// A delegate used for receiving the result of a Check
            /// For Updates request.
            ///
            /// @author S Downie
            ///
            /// @param The result.
            //--------------------------------------------------------
            typedef std::function<void(CheckForUpdatesResult)> CheckForUpdateDelegate;
            //--------------------------------------------------------
            /// A delegate used for receiving the result of a content
            /// download request.
            ///
            /// @author S Downie
            ///
            /// @param The result.
            //--------------------------------------------------------
            typedef std::function<void(Result)> CompleteDelegate;
            //--------------------------------------------------------
            /// Creates a new instance of this system.
            ///
            /// @author I Copland
            ///
            /// @param The content downloader that should be used.
            ///
            /// @return The new system instance.
            //--------------------------------------------------------
			static ContentManagementSystemUPtr Create(IContentDownloader* in_contentDownloader);
            //--------------------------------------------------------
            /// Queries whether or not this implements the interface
            /// with the given Id.
            ///
            /// @author S Downie
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the inteface is implemented.
            //---------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //-----------------------------------------------------------
            /// Get the content manifest from the asset server and
            /// check for any changes with the local copy.
            ///
            /// The location of the asset server is usually the Tag 
            /// server but may be a third party storage solution based on
            /// the SKU (i.e. Google's Asset Store). The URL is taken
            /// from the tweakable constants.
            ///
            /// @author S Downie
            ///
            /// @param Delegate to callback notifying whether an update
            /// is required
            //-----------------------------------------------------------
            void CheckForUpdates(const CheckForUpdateDelegate& in_delegate);
            //-----------------------------------------------------------
            /// Having checked for updates and been notified whether
            /// there are updates pending this function should be
            /// called to begin downloading any out of date content
            ///
            /// Call GetDownloadProgress to get the progress value
            /// to update any progress UI
            ///
            /// @author S Downie
            ///
            /// @param Delegate to call when download is complete
            //-----------------------------------------------------------
            void DownloadUpdates(const CompleteDelegate& in_delegate);
            //-----------------------------------------------------------
            /// Having downloaded the update packages this method
            /// unzips the packages and overwrites any old assets
            ///
            /// @author S Downie
            ///
            /// @param Delegate to call when Install is complete
            //-----------------------------------------------------------
            void InstallUpdates(const CompleteDelegate& in_delegate);
            //-----------------------------------------------------------
            /// Data is cached in memory and only written to disc
            /// when the download is successful; at which point
            /// we can clear the data and delete any temp files
            ///
            /// @author S Downie
            //-----------------------------------------------------------
            void ClearDownloadData();
            //-----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param File name
            /// @return Checksum of file as found in the current manifest
            //-----------------------------------------------------------
            static std::string GetManifestChecksumForFile(const std::string& in_filename);
			//-----------------------------------------------------------
			/// Returns the current running total of the size of data to
            /// download.
            ///
            /// @author S Downie
			///
			/// @return The size of the data needing to be downloaded
			//-----------------------------------------------------------			
			u32 GetRunningTotalToDownload();
			//-----------------------------------------------------------
			/// Returns the current running total of the size of data
            /// downloaded
            ///
            /// @author S Downie
			///
			/// @return The current running total of the size of data
            /// downloaded
			//-----------------------------------------------------------
			u32 GetRunningTotalDownloaded();
			//-----------------------------------------------------------
            /// @author S Downie
			///
			/// @return The current content downloader
			//-----------------------------------------------------------
			IContentDownloader* GetContentDownloader() const;
            
        private:
            //-----------------------------------------------------------
            /// A container for information on a single downloaded
            /// package.
            ///
            /// @author S Downie
			//-----------------------------------------------------------
            struct PackageDetails
            {
                std::string m_url;
                std::string m_id;
                std::string m_checksum;
                u32 m_size;
            };
            //------------------------------------------------------------
            /// Initialisation method called at a time when all App Systems
            /// have been created. System initialisation occurs in the order
            /// they were created.
            ///
            /// @author I Copland
            //-----------------------------------------------------------
            void OnInit() override;
            //-----------------------------------------------------------
            /// Constructor. Declared private to force the use of the
            /// factory method.
            ///
            /// @author S Downie
            ///
            /// @param The content downloader to use.
            //-----------------------------------------------------------
            ContentManagementSystem(IContentDownloader* in_contentDownloader);
            //-----------------------------------------------------------
            /// Load the client side cached content manifest if one
            /// exists
            ///
            /// @author S Downie
            ///
            /// @param TiXmlDocument
            //-----------------------------------------------------------
            void LoadLocalManifest(TiXmlDocument* in_currentManifest);
            //-----------------------------------------------------------
            /// The manifest file has downloaded we can now compare
            /// and contrast to check for outdated files
            ///
            /// @author S Downie
            ///
            /// @param Request result
            /// @param Request response
            //-----------------------------------------------------------
            void OnContentManifestDownloadComplete(IContentDownloader::Result in_result, const std::string& in_manifest);
            //-----------------------------------------------------------
            /// The file has downloaded we can now save it to the the 
            /// cache
            ///
            /// @author S Downie
            ///
            /// @param Request result
            /// @param Request response
            //-----------------------------------------------------------
            void OnContentDownloadComplete(IContentDownloader::Result in_result, const std::string& in_data);
            //-----------------------------------------------------------
            /// Check if an existing content manifest exists and
            /// construct a list of the files that require updating
            /// then overwrite the old manifest with the new one
            ///
            /// @author S Downie
            ///
            /// @param String containing the server manifest
            //-----------------------------------------------------------
            void BuildDownloadList(const std::string& in_serverManifest);
            //-----------------------------------------------------------
            /// The package may be outdated in documents but are
            /// all the files in bundle up to date
            ///
            /// @author S Downie
            ///
            /// @param Package element
            //-----------------------------------------------------------
            void AddToDownloadListIfNotInBundle(TiXmlElement* in_packageEl);
            //-----------------------------------------------------------
            /// Save the zip file to documents directory
            ///
            /// @author S Downie
            ///
            /// @param Package details
            /// @param Binary zip
            /// @param Whether the file has finished downloading
            /// @return Success
            //-----------------------------------------------------------
            bool SavePackageToFile(const PackageDetails& in_packageDetails, const std::string& in_zippedPackage, bool in_fullyDownloaded);
            //-----------------------------------------------------------
            /// Unzip the package and save all the files to the
            /// documents directory
            ///
            /// @author S Downie
            ///
            /// @param Zipped package
            //-----------------------------------------------------------
            void ExtractFilesFromPackage(const PackageDetails& in_packageDetails) const;
            //-----------------------------------------------------------
			/// Deletes a directory from the DLC Storage Location.
            ///
            /// @author S Downie
			///
			/// @return The directory
			//-----------------------------------------------------------
			void DeleteDirectory(const std::string& in_directory) const;
            //-----------------------------------------------------------
            /// Checks whether the file is within the application and if the
            /// the checksums match
            ///
            /// @author S Downie
            ///
            /// @param Filename
            /// @param Checksum
            /// @param Whether to check only the bundle
            ///
            /// @return Whether the file exists
            //-----------------------------------------------------------
            static bool DoesFileExist(const std::string& in_filename, const std::string in_checksum, bool in_checkOnlyBundle);
            //-----------------------------------------------------------
            /// Calculate a checksum for the file. This involves
            /// performing an MD5 hash of the file and converting that
            /// to base 64 encoded and then trimming the trailing '='
            ///
            /// @author S Downie
            ///
            /// @param File location
            /// @param File path
            /// @return Checksum string
            //-----------------------------------------------------------
            static std::string CalculateChecksum(Core::StorageLocation in_location, const std::string& in_filePath);
            //-----------------------------------------------------------
            /// Perform the HTTP request for the next DLC package.
            ///
            /// @author S Downie
            //-----------------------------------------------------------
            void DownloadNextPackage();
        private:
            
            std::vector<std::string> m_removePackageIds;
            std::vector<PackageDetails> m_packageDetails;
			
			u32	m_runningToDownloadTotal;
			u32 m_runningDownloadedTotal;
            
            TiXmlDocument* m_serverManifest;
            
            IContentDownloader* m_contentDownloader;
            
            CheckForUpdateDelegate m_onUpdateCheckCompleteDelegate;
            CompleteDelegate m_onDownloadCompleteDelegate;
            
            std::string m_serverManifestData;
            std::string m_contentDirectory;
			
            u32 m_currentPackageDownload;
            
            bool m_dlcCachePurged;
        };
    }
}

#endif
