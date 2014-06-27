//
//  GooglePlayExpansionSystem.h
//  Chilli Source
//  Created by Hugh McLaughlin on 25/06/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifdef CS_ANDROIDEXTENSION_GOOGLEPLAY

#ifndef _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_GOOGLEPLAYEXPANSIONSYSTEM_H_
#define _CSBACKEND_PLATFORM_ANDROID_EXTENSIONS_GOOGLEPLAY_GOOGLEPLAYEXPANSIONSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterface.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <json/json.h>

namespace CSBackend
{
    namespace Android
    {
    	//------------------------------------------------------------------------
    	/// A system for using the Google Play Expansion downloader.
    	///
    	/// @author H McLaughlin
    	//------------------------------------------------------------------------
        class GooglePlayExpansionSystem final : public CSCore::AppSystem
        {
        public:
        	CS_DECLARE_NAMEDTYPE(GooglePlayExpansionSystem);
        	//----------------------------------------------------------------
        	/// An enum describing the possible states the download can be in.
        	///
        	/// @author H McLaughlin
        	//----------------------------------------------------------------
        	enum class DownloadStatus
			{
				k_downloading,
				k_installing,
				k_complete,
				k_pausedNoWiFi,
				k_paused,
				k_failed,
				k_failedInsufficientStorage
			};
        	//----------------------------------------------------------------
        	/// A callback for the result of a download attempt.
        	///
        	/// @author H McLaughlin
        	///
        	/// @param The download status.
        	//----------------------------------------------------------------
        	typedef std::function<void(DownloadStatus)> DownloadStatusDelegate;
            //-------------------------------------------------------------
            /// Queries whether or not the system implements the interface
        	/// with the given Id.
        	///
        	/// @author H McLaughlin
            ///
            /// @param The Interface Id.
        	///
            /// @return Whether the system implements the given Id.
            //-------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------------
            /// Check to see if the files have already been downloaded
            /// and if they are complete
            ///
            /// @author H McLaughlin
            ///
            /// @return Whether a download is required
            //-------------------------------------------------------------
            bool IsDownloadRequired();
            //-------------------------------------------------------------
            /// Attempts to download the package and install to device
            /// from the google store.
            ///
            /// @author H McLaughlin
            ///
            /// @param Delegate gets called whenever the download status
            /// changes - such as complete, paused, failed, etc
            //-------------------------------------------------------------
            void Download(const DownloadStatusDelegate& in_delegate);
            //-------------------------------------------------------------
            /// Suspend the currently in-progress download.
            ///
            /// @author H McLaughlin
            //-------------------------------------------------------------
            void Pause();
            //-------------------------------------------------------------
            /// Resume the currently in-progress download.
            ///
            /// @author H McLaughlin
            //-------------------------------------------------------------
            void Resume();
            //-------------------------------------------------------------
            /// @author H McLaughlin
            ///
            /// @return Get the percentage progress of the current download
            //-------------------------------------------------------------
            f32 GetDownloadProgress();
            //-------------------------------------------------------------
            /// @author H McLaughlin
            ///
            /// @return Get the percentage progress of the current install
            //-------------------------------------------------------------
            f32 GetInstallProgress() const;
            //-------------------------------------------------------------
            /// @author H McLaughlin
            ///
            /// @return Storage requirements
            //-------------------------------------------------------------
            u64 GetRequiredStorageSpaceInBytes();
            //-------------------------------------------------------------
            /// This informs the system of changes to the download status.
            /// This is for internal use and should not be called by a
            /// user of the system.
            ///
            /// @author H McLaughlin
            ///
            /// @return Storage requirements
            //-------------------------------------------------------------
            void OnDownloadStatusChanged(DownloadStatus in_Status);
        private:
        	friend class CSCore::Application;
        	//----------------------------------------------------------------
        	/// Creates a new instance of the system.
        	///
        	/// @author H McLaughlin
        	//----------------------------------------------------------------
        	static GooglePlayExpansionSystemUPtr Create();
        	//----------------------------------------------------------------
        	/// Constructor. Declared private to force the use of the factory
        	/// method.
        	///
        	/// @author H McLaughlin
        	//----------------------------------------------------------------
        	GooglePlayExpansionSystem();
            //-------------------------------------------------------------
            /// Called when the application sends the initialise lifecycle
        	/// event.
        	///
        	/// @author H McLaughlin
            //-------------------------------------------------------------
            void OnInit() override;
            //-------------------------------------------------------------
            /// Called when the application sends the resume lifecycle
        	/// event.
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            void OnResume() override;
            //-------------------------------------------------------------
            /// Called when the application sends the suspend lifecycle
            /// event.
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            void OnSuspend() override;
            //-------------------------------------------------------------
            /// Called when the application sends the destroy lifecycle
            /// event.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------------
            void OnDestroy() override;
            //-------------------------------------------------------------
            /// Unzip the expansions into the DLC directory ready for
            /// use by the file system
            ///
            /// @author H McLaughlin
            //-------------------------------------------------------------
            void Install();
            //-------------------------------------------------------------
            /// @author H McLaughlin
            ///
            /// @return Whether the downloaded files require unzipping
            //-------------------------------------------------------------
            bool DoInstalledFilesExist() const;
            //-------------------------------------------------------------
            /// @author H McLaughlin
            ///
            /// @param Path to zip file
            /// @return Unzipped package size
            //-------------------------------------------------------------
            u64 GetUncompressedZipSize(const std::string& in_filePath) const;
            //-------------------------------------------------------------
            /// @author H McLaughlin
            ///
            /// @param Required space in bytes
            /// @return Whether enough space is available
            //-------------------------------------------------------------
            bool HasEnoughStorageSpace(u64 in_required);
            //-------------------------------------------------------------
            /// @author H McLaughlin
            ///
            /// @return Storage requirements for installation
            //-------------------------------------------------------------
            u64 GetRequiredInstallSpaceInBytes();
            //-------------------------------------------------------------
            /// @author H McLaughlin
            ///
            /// @return Storage requirements for download
            //-------------------------------------------------------------
            u64 GetRequiredDownloadSpaceInBytes();
            //-------------------------------------------------------------
            /// A task for unzipping the expansions.
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            void UnzipTask();
            //-------------------------------------------------------------
            /// Unzipping the expansion to a directory.
            ///
            /// @author S Downie
            ///
            /// @param The zip path.
            /// @param [Out] The manifest json
            //-------------------------------------------------------------
            void Unzip(const std::string& in_zipPath, Json::Value& out_manifest);
    		//-------------------------------------------------------------
            /// Save the package version that will allow us to check
            /// whether we have the latest package
            ///
            /// @author S Downie
    		//-------------------------------------------------------------
            void CachePackageDescriptions();
    		//-------------------------------------------------------------
    		/// @author S Downie
            ///
            /// @param Status
    		//-------------------------------------------------------------
            void UnzipCompleteTask(DownloadStatus in_status);
            //-------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Whether the zip file exists on device
            //-------------------------------------------------------------
            bool DoExpansionDownloadFilesExist() const;
            //-------------------------------------------------------------
            /// Attempt to load the manifest and remove all the files
            /// it contains from the installed location.
            ///
            /// @author S Downie
            //-------------------------------------------------------------
            void RemoveInstalledFiles();

        private:
            DownloadStatusDelegate m_downloadStatusDelegate;

            u32 m_numExpansions;
            f32 m_installProgress;

            GooglePlayExpansionJavaInterfaceSPtr m_javaInterface;
        };
    }
}

#endif

#endif

#endif
