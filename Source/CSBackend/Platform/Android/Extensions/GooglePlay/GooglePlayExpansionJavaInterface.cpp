//
//  GooglePlayExpansionSystem.cpp
//  Chilli Source
//
//  Created by I Copland on 09/04/2014.
//  Copyright (c) 2014 Tag Games. All rights reserved.
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
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedInProgress(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedComplete(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailed(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailedNoStorage(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPaused(JNIEnv* inpEnv, jobject inpThis);
	void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPausedNoWiFi(JNIEnv* inpEnv, jobject inpThis);
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedInProgress(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_downloading);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedComplete(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_complete);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailed(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_failed);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedFailedNoStorage(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_failedInsufficientStorage);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPaused(JNIEnv* inpEnv, jobject inpThis)
{
	if(g_expansionSystem)
	{
		g_expansionSystem->OnDownloadStatusChanged(CSBackend::Android::GooglePlayExpansionSystem::DownloadStatus::k_paused);
	}
}

void Java_com_chillisource_googleplay_ExpansionDownloaderNativeInterface_OnDownloadStateChangedPausedNoWiFi(JNIEnv* inpEnv, jobject inpThis)
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
			CreateNativeInterface("com/chillisource/googleplay/ExpansionDownloaderNativeInterface");

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
