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

#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaClassDef.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaUtils.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaVirtualMachine.h>

extern "C"
{
    //------------------------------------------------------------------------------
    /// TODO:
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedDownloading(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// TODO:
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedComplete(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// TODO:
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedFailed(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// TODO:
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedFailedNoStorage(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// TODO:
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedPaused(JNIEnv* in_environment, jobject in_this);
    //------------------------------------------------------------------------------
    /// TODO:
    //------------------------------------------------------------------------------
	void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedPausedNoWiFi(JNIEnv* in_environment, jobject in_this);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onStateChangedDownloading(JNIEnv* in_environment, jobject in_this)
{
    auto apkExpansionSystem = CSCore::Application::Get()->GetSystem<CSBackend::Android::ApkExpansionDownloader>();
    CS_ASSERT(apkExpansionSystem != nullptr, "No expansion system accessible.");

    apkExpansionSystem->TODO
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedComplete(JNIEnv* in_environment, jobject in_this)
{
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedFailed(JNIEnv* in_environment, jobject in_this)
{
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedFailedNoStorage(JNIEnv* in_environment, jobject in_this)
{
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedPaused(JNIEnv* in_environment, jobject in_this)
{
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void Java_com_chilliworks_chillisource_googleplay_networking_ApkExpansionDownloader_onDownloadStateChangedPausedNoWiFi(JNIEnv* in_environment, jobject in_this)
{
}

namespace CSBackend
{
	namespace Android
	{
	    namespace
	    {
	        char[] k_startDownloadIfRequiredMethod = "startDownloadIfRequired";
	        char[] k_setDownloadPausedMethod = "setDownloadPaused";
	        char[] k_getDownloadProgressMethod = "getDownloadProgress";
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
        void ApkExpansionDownloader::OnInit()
        {
            //define and create the java class.
            JavaClassDef classDef("com/chilliworks/chillisource/googleplay/networking/ApkExpansionDownloader");
            classDef.AddMethod(k_startDownloadIfRequiredMethod, "()Z");
            classDef.AddMethod(k_setDownloadPausedMethod, "(Z)V");
            classDef.AddMethod(k_getDownloadProgressMethod, "()F");
            m_javaSystem = JavaSystemUPtr(new JavaSystem(classDef));

            //Start the download if required.
            if (m_javaSystem->CallBoolMethod(k_startDownloadIfRequiredMethod) == true)
            {
                m_state = State::k_downloading;
            }
            else
            {
                m_state = State::k_downloaded;
            }
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
