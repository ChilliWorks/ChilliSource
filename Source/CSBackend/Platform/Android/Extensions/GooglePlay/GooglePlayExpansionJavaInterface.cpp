//
//  GooglePlayExpansionJavaInterface.cpp
//  Chilli Source
//  Created by Ian Copland on 09/04/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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

#include <CSBackend/Platform/Android/Extensions/GooglePlay/GooglePlayExpansionJavaInterface.h>

#include <CSBackend/Platform/Android/Extensions/GooglePlay/GooglePlayExpansionSystem.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>

#include <jni.h>

CSBackend::Android::GooglePlayExpansionSystem* g_expansionSystem = nullptr;

extern "C"
{
	void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedInProgress(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedComplete(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailed(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailedNoStorage(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPaused(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPausedNoWiFi(JNIEnv* inpEnv, jobject inpThis);
}

void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedInProgress(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_downloading);
	}
}

void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedComplete(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_complete);
	}
}

void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailed(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_failed);
	}
}

void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailedNoStorage(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_failedInsufficientStorage);
	}
}

void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPaused(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_paused);
	}
}

void Java_com_chilliworks_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPausedNoWiFi(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_pausedNoWiFi);
	}
}

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(GooglePlayExpansionJavaInterface);

		GooglePlayExpansionJavaInterface::GooglePlayExpansionJavaInterface()
		{
			CreateNativeInterface("com/chilliworks/chillisource/googleplay/ExpansionDownloaderNativeInterface");

			CreateMethodReference("Init", "()V");
			CreateMethodReference("Download", "()V");
			CreateMethodReference("PauseDownload", "()V");
			CreateMethodReference("ResumeDownload", "()V");
			CreateMethodReference("GetDownloadProgress", "()F");
			CreateMethodReference("GetExternalFreeStorageInBytes", "()J");
			CreateMethodReference("KeepAppAwake", "()V");
			CreateMethodReference("AllowAppToSleep", "()V");
			CreateMethodReference("GetNumExpansions", "()I");
			CreateMethodReference("GetExpansionVersionCode", "(I)I");
			CreateMethodReference("GetExpansionFileSize", "(I)J");
			CreateMethodReference("GetExpansionPath", "(I)Ljava/lang/String;");
		}

		bool GooglePlayExpansionJavaInterface::IsA(CSCore::InterfaceIDType inInterfaceID) const
		{
			return GooglePlayExpansionJavaInterface::InterfaceID == inInterfaceID;
		}
		void GooglePlayExpansionJavaInterface::SetGooglePlayExpansionSystem(GooglePlayExpansionSystem* in_expansionSystem)
		{
			g_expansionSystem = in_expansionSystem;
		}
		void GooglePlayExpansionJavaInterface::Init()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("Init"));
		}
		void GooglePlayExpansionJavaInterface::Download()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("Download"));
		}
		void GooglePlayExpansionJavaInterface::PauseDownload()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("PauseDownload"));
		}
		void GooglePlayExpansionJavaInterface::ResumeDownload()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("ResumeDownload"));
		}
		f32 GooglePlayExpansionJavaInterface::GetDownloadProgress()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallFloatMethod(GetJavaObject(), GetMethodID("GetDownloadProgress"));
		}
		u64 GooglePlayExpansionJavaInterface::GetExternalFreeStorageInBytes()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallLongMethod(GetJavaObject(), GetMethodID("GetExternalFreeStorageInBytes"));
		}
		void GooglePlayExpansionJavaInterface::KeepAppAwake()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("KeepAppAwake"));
		}
		void GooglePlayExpansionJavaInterface::AllowAppToSleep()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			env->CallVoidMethod(GetJavaObject(), GetMethodID("AllowAppToSleep"));
		}
		u32 GooglePlayExpansionJavaInterface::GetNumExpansions()
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallIntMethod(GetJavaObject(), GetMethodID("GetNumExpansions"));
		}
		u32 GooglePlayExpansionJavaInterface::GetExpansionVersionCode(u32 inudwIndex)
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallIntMethod(GetJavaObject(), GetMethodID("GetExpansionVersionCode"), inudwIndex);
		}
		u64 GooglePlayExpansionJavaInterface::GetExpansionFileSizeInBytes(u32 inudwIndex)
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return env->CallLongMethod(GetJavaObject(), GetMethodID("GetExpansionFileSize"), inudwIndex);
		}
		std::string GooglePlayExpansionJavaInterface::GetExpansionPath(u32 inudwIndex)
		{
			JNIEnv* env = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrZipPath = (jstring) env->CallObjectMethod(GetJavaObject(), GetMethodID("GetExpansionPath"), inudwIndex);
			std::string strZipPath = JavaInterfaceUtils::CreateSTDStringFromJString(jstrZipPath);
			return strZipPath;
		}
	}
}

#endif

#endif
