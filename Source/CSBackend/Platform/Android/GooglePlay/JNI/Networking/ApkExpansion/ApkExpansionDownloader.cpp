//
//  ApkExpansionDownloader.cpp
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

#include <CSBackend/Platform/Android/GooglePlay/JNI/Networking/ApkExpansion/ApkExpansionDownloader.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClassDef.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>

extern "C"
{
    //------------------------------------------------------------------------------
    /// Called from Java to notify the system that the download state has changed
    /// to Downloading
    ///
    /// @author Ian Copland
    ///
    /// @param in_environment - The JNI Environment.
    /// @param in_this - The calling object.
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedDownloading(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// Called from Java to notify the system that the download state has changed
    /// to Complete
    ///
    /// @author Ian Copland
    ///
    /// @param in_environment - The JNI Environment.
    /// @param in_this - The calling object.
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedComplete(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// Called from Java to notify the system that the download state has changed
    /// to Failed
    ///
    /// @author Ian Copland
    ///
    /// @param in_environment - The JNI Environment.
    /// @param in_this - The calling object.
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedFailed(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// Called from Java to notify the system that the download state has changed
    /// to Failed No Storage
    ///
    /// @author Ian Copland
    ///
    /// @param in_environment - The JNI Environment.
    /// @param in_this - The calling object.
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedFailedNoStorage(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// Called from Java to notify the system that the download state has changed
    /// to Paused
    ///
    /// @author Ian Copland
    ///
    /// @param in_environment - The JNI Environment.
    /// @param in_this - The calling object.
    //------------------------------------------------------------------------------
    void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedPaused(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// Called from Java to notify the system that the download state has changed
    /// to Paused No Wifi
    ///
    /// @author Ian Copland
    ///
    /// @param in_environment - The JNI Environment.
    /// @param in_this - The calling object.
    //------------------------------------------------------------------------------
    void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedPausedNoWiFi(JNIEnv* in_environment, jobject in_this);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedDownloading(JNIEnv* in_environment, jobject in_this)
{
    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask([]
    {
        auto apkExpansionDownloader = CSCore::Application::Get()->GetSystem<CSBackend::Android::ApkExpansionDownloader>();
        CS_ASSERT(apkExpansionDownloader != nullptr, "Cannot get the ApkExpansionDownloader system from Application.");

        apkExpansionDownloader->OnDownloadStateChanged(CSBackend::Android::ApkExpansionDownloader::StateChange::k_downloading);
    });
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedComplete(JNIEnv* in_environment, jobject in_this)
{
    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask([]
    {
        auto apkExpansionDownloader = CSCore::Application::Get()->GetSystem<CSBackend::Android::ApkExpansionDownloader>();
        CS_ASSERT(apkExpansionDownloader != nullptr, "Cannot get the ApkExpansionDownloader system from Application.");

        apkExpansionDownloader->OnDownloadStateChanged(CSBackend::Android::ApkExpansionDownloader::StateChange::k_complete);
    });
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedFailed(JNIEnv* in_environment, jobject in_this)
{
    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask([]
    {
        auto apkExpansionDownloader = CSCore::Application::Get()->GetSystem<CSBackend::Android::ApkExpansionDownloader>();
        CS_ASSERT(apkExpansionDownloader != nullptr, "Cannot get the ApkExpansionDownloader system from Application.");

        apkExpansionDownloader->OnDownloadStateChanged(CSBackend::Android::ApkExpansionDownloader::StateChange::k_failed);
    });
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedFailedNoStorage(JNIEnv* in_environment, jobject in_this)
{
    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask([]
    {
        auto apkExpansionDownloader = CSCore::Application::Get()->GetSystem<CSBackend::Android::ApkExpansionDownloader>();
        CS_ASSERT(apkExpansionDownloader != nullptr, "Cannot get the ApkExpansionDownloader system from Application.");

        apkExpansionDownloader->OnDownloadStateChanged(CSBackend::Android::ApkExpansionDownloader::StateChange::k_failedNoStorage);
    });
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedPaused(JNIEnv* in_environment, jobject in_this)
{
    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask([]
    {
        auto apkExpansionDownloader = CSCore::Application::Get()->GetSystem<CSBackend::Android::ApkExpansionDownloader>();
        CS_ASSERT(apkExpansionDownloader != nullptr, "Cannot get the ApkExpansionDownloader system from Application.");

        apkExpansionDownloader->OnDownloadStateChanged(CSBackend::Android::ApkExpansionDownloader::StateChange::k_paused);
    });
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedPausedNoWiFi(JNIEnv* in_environment, jobject in_this)
{
    CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask([]
    {
        auto apkExpansionDownloader = CSCore::Application::Get()->GetSystem<CSBackend::Android::ApkExpansionDownloader>();
        CS_ASSERT(apkExpansionDownloader != nullptr, "Cannot get the ApkExpansionDownloader system from Application.");

        apkExpansionDownloader->OnDownloadStateChanged(CSBackend::Android::ApkExpansionDownloader::StateChange::k_pausedNoWifi);
    });
}

namespace CSBackend
{
	namespace Android
	{
	    namespace
	    {
	        char k_isDownloadRequiredMethod[] = "isDownloadRequired";
	        char k_startDownloadMethod[] = "startDownload";
	        char k_setDownloadPausedMethod[] = "setDownloadPaused";
	        char k_getDownloadProgressMethod[] = "getDownloadProgress";
	    }

        CS_DEFINE_NAMEDTYPE(ApkExpansionDownloader);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        ApkExpansionDownloaderUPtr ApkExpansionDownloader::Create()
        {
            return ApkExpansionDownloaderUPtr(new ApkExpansionDownloader());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ApkExpansionDownloader::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (ApkExpansionDownloader::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ApkExpansionDownloader::IsExpansionAvailable() const
        {
            CS_ASSERT(m_state != State::k_uninitialised, "Cannot use ApkExpansionDownloader before it has been initialised.");

            return (m_state == State::k_downloaded);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 ApkExpansionDownloader::GetDownloadProgress() const
        {
            CS_ASSERT(m_state != State::k_uninitialised, "Cannot use ApkExpansionDownloader before it has been initialised.");

            if (m_state == State::k_downloading)
            {
                return m_javaSystem->CallFloatMethod(k_getDownloadProgressMethod);
            }
            else
            {
                return 0.0f;
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool ApkExpansionDownloader::IsDownloadPaused() const
        {
            CS_ASSERT(m_state != State::k_uninitialised, "Cannot use ApkExpansionDownloader before it has been initialised.");

            return (m_state == State::k_paused);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ApkExpansionDownloader::SetDownloadPaused(bool in_paused)
        {
            CS_ASSERT(m_state != State::k_uninitialised, "Cannot use ApkExpansionDownloader before it has been initialised.");

            if (m_state == State::k_downloading)
            {
                m_javaSystem->CallVoidMethod(k_setDownloadPausedMethod, in_paused);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CSCore::IConnectableEvent<ApkExpansionDownloader::DownloadedDelegate>& ApkExpansionDownloader::GetDownloadedEvent()
        {
            return m_downloadedEvent;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ApkExpansionDownloader::OnDownloadStateChanged(StateChange in_stateChange)
        {
            CS_ASSERT(m_state == State::k_downloading || m_state == State::k_paused, "Received a state change ("+CSCore::ToString((s32)in_stateChange)+") while in an invalid state.");

            switch (in_stateChange)
            {
                case StateChange::k_downloading:
                {
                    CS_LOG_ERROR("[ApkExpansionDownloader] State -> Downloading...");
                    m_state = State::k_downloading;
                    break;
                }
                case StateChange::k_complete:
                {
                    CS_LOG_ERROR("[ApkExpansionDownloader] State -> Complete!");
                    m_state = State::k_downloaded;
                    m_downloadedEvent.NotifyConnections(Result::k_success);
                    break;
                }
                case StateChange::k_failed:
                {
                    CS_LOG_ERROR("[ApkExpansionDownloader] State -> Failed!");
                    m_state = State::k_failed;
                    m_downloadedEvent.NotifyConnections(Result::k_failed);
                    break;
                }
                case StateChange::k_failedNoStorage:
                {
                    CS_LOG_ERROR("[ApkExpansionDownloader] State -> Out of Storage!");
                    m_state = State::k_failed;
                    m_downloadedEvent.NotifyConnections(Result::k_failedNoStorage);
                    break;
                }
                case StateChange::k_paused:
                {
                    CS_LOG_ERROR("[ApkExpansionDownloader] State -> ...Paused...");
                    CS_ASSERT(m_state == State::k_downloading, "Invalid Apk Expansion Download state change: Paused.");
                    m_state = State::k_paused;
                    break;
                }
                case StateChange::k_pausedNoWifi:
                {
                    CS_LOG_ERROR("[ApkExpansionDownloader] State -> ...No Wifi...");
                    CS_ASSERT(m_state == State::k_downloading, "Invalid Apk Expansion Download state change: Paued No Wifi.");
                    m_state = State::k_paused;
                    break;
                }
                default:
                {
                    CS_LOG_FATAL("Invalid Apk Expansion Download state change: Unknown.");
                    break;
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ApkExpansionDownloader::OnInit()
        {
            jstring lvlPublicKey = JavaUtils::CreateJStringFromSTDString(CSCore::Application::Get()->GetGooglePlayLvlPublicKey());

            //define and create the java class.
            JavaClassDef classDef("com/chilliworks/chillisource/googleplay/networking/ApkExpansionDownloader", "(Ljava/lang/String;)V");
            classDef.AddMethod(k_isDownloadRequiredMethod, "()Z");
            classDef.AddMethod(k_startDownloadMethod, "()V");
            classDef.AddMethod(k_setDownloadPausedMethod, "(Z)V");
            classDef.AddMethod(k_getDownloadProgressMethod, "()F");
            m_javaSystem = JavaSystemUPtr(new JavaSystem(classDef, lvlPublicKey));

            //Start the download if required.
            if (m_javaSystem->CallBoolMethod(k_isDownloadRequiredMethod) == true)
            {
                m_state = State::k_downloading;
                m_javaSystem->CallVoidMethod(k_startDownloadMethod);
            }
            else
            {
                m_state = State::k_downloaded;
            }

            auto environment = JavaVirtualMachine::Get()->GetJNIEnvironment();
            environment->DeleteLocalRef(lvlPublicKey);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void ApkExpansionDownloader::OnDestroy()
        {
            m_javaSystem.reset();
        }
	}
}

#endif

#endif
