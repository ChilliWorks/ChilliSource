//
//  ApkExpansionDownloader.h
//  Chilli Source
//  Created by Ian Copland on 22/04/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifdef CS_ANDROIDFLAVOUR_GOOGLEPLAY

#ifndef _CSBACKEND_PLATFORM_ANDROID_GOOGLEPLAY_JNI_NETWORKING_APKEXPANSION_APKEXPANSIONDOWNLOADER_H_
#define _CSBACKEND_PLATFORM_ANDROID_GOOGLEPLAY_JNI_NETWORKING_APKEXPANSION_APKEXPANSIONDOWNLOADER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaSystem.h>
#include <CSBackend/Platform/Android/GooglePlay/JNI/ForwardDeclarations.h>

#include <functional>

namespace CSBackend
{
	namespace Android
	{
		//------------------------------------------------------------------------------
		/// A system which manages downloading of Google Play APK Expansion files. This
		/// will automatically start the download of expansion files if they are not
		/// present. A manifest of the contents of the files will be generated when they
		/// are first downloaded, this can be used by other systems to find files within
		/// the expansion. "Patch" expansion files are not supported.
		///
		/// The initial state provided in application should not be pushed until the
		/// expansion APK files and the manifest are present.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class ApkExpansionDownloader final : public CSCore::AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(ApkExpansionDownloader);
            //------------------------------------------------------------------------------
            /// An enum describing state changes passed to the system from Java. This is
            /// for internal use.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            enum class StateChange
            {
                k_downloading,
                k_complete,
                k_failed,
                k_failedNoStorage,
                k_paused,
                k_pausedNoWifi
            };
            //------------------------------------------------------------------------------
            /// An enum describing the result of the download.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            enum class Result
            {
                k_success,
                k_failed,
                k_failedNoStorage
            };
            //------------------------------------------------------------------------------
            /// An delegate which will be called when expansion downloading completes.
            ///
            /// @author Ian Copland
            ///
            /// @param in_result - Whether or not the the download was successful.
            //------------------------------------------------------------------------------
            using DownloadedDelegate = std::function<void(Result in_result)>;
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @author Ian Copland
            ///
            /// @param in_interfaceId - The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //------------------------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not the most up to date expansion files are currently
            /// available.
            //------------------------------------------------------------------------------
            bool IsExpansionAvailable() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The current progress of the download.
            //------------------------------------------------------------------------------
            f32 GetDownloadProgress() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return Whether or not the download is currently paused. The download can
            /// be paused manually through calling SetDownloadPaused() or if there is no
            /// available Wifi connection.
            //------------------------------------------------------------------------------
            bool IsDownloadPaused() const;
            //------------------------------------------------------------------------------
            /// Pauses the download if currently downloading. If no download is currently
            /// in progress the app is considered to be in irrecoverable state and will
            /// terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param in_paused - Whether or not the download should be paused.
            //------------------------------------------------------------------------------
            void SetDownloadPaused(bool in_paused);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return An event when is called when the downloader completes.
            //------------------------------------------------------------------------------
            CSCore::IConnectableEvent<DownloadedDelegate>& GetDownloadedEvent();
            //------------------------------------------------------------------------------
            /// Called from the Java system when the downloader state changes. This is for
            /// internal use only and should not be called by the user.
            ///
            /// @author Ian Copland
            ///
            /// @param in_stageChange - The state the downloader as changed to.
            //------------------------------------------------------------------------------
            void OnDownloadStateChanged(StateChange in_stateChange);
		private:
		    friend class CSCore::Application;
            //------------------------------------------------------------------------------
            /// An enum describing the current state of the downloader.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            enum class State
            {
                k_uninitialised,
                k_paused,
                k_downloading,
                k_downloaded,
                k_failed
            };
            //------------------------------------------------------------------------------
            /// A factory method for creating a new instance of the system. Declared private
            /// to ensure this can only be created through Application::CreateSystem().
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static ApkExpansionDownloaderUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the factory method is used.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
			ApkExpansionDownloader() = default;
            //------------------------------------------------------------------------------
            /// Initialises the system during the Application On Init life cycle event.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Cleans up the system during the application On Destroy life cycle event.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;

            JavaSystemUPtr m_javaSystem;
            State m_state = State::k_uninitialised;
            CSCore::Event<DownloadedDelegate> m_downloadedEvent;
		};
	}
}

#endif

#endif

#endif
