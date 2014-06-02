/*
 *  MediaPlayerJavaInterface.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 10/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifdef CS_TARGETPLATFORM_ANDROID

#include <ChilliSource/Backend/Platform/Android/Video/Base/VideoPlayerJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	void Java_com_chillisource_video_VideoPlayerNativeInterface_OnVideoComplete(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_video_VideoPlayerNativeInterface_OnUpdateSubtitles(JNIEnv* inpEnv, jobject inThis);
}
//-------------------------------------------
/// On Video Complete
///
/// Called when a video is stopped
///
/// @param The jni environment.
/// @param The java object calling the function.
//-------------------------------------------
void Java_com_chillisource_video_VideoPlayerNativeInterface_OnVideoComplete(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::VideoPlayerJavaInterfaceSPtr pVideoPJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::VideoPlayerJavaInterface>();

	if (pVideoPJI != nullptr)
	{
		auto task = std::bind(&ChilliSource::Android::VideoPlayerJavaInterface::OnVideoComplete, pVideoPJI.get());
		ChilliSource::Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
	}
}
//-------------------------------------------
/// Update Subtitles
///
/// Called every frame to update the video
/// subtitles
///
/// @param The jni environment.
/// @param The java object calling the function.
//-------------------------------------------
void Java_com_chillisource_video_VideoPlayerNativeInterface_OnUpdateSubtitles(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::VideoPlayerJavaInterfaceSPtr pVideoPJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::VideoPlayerJavaInterface>();
	if (pVideoPJI != nullptr)
	{
		pVideoPJI->OnUpdateSubtitles();
	}
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(VideoPlayerJavaInterface);
		//--------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------
		VideoPlayerJavaInterface::VideoPlayerJavaInterface()
		{
			CreateNativeInterface("com/chillisource/video/VideoPlayerNativeInterface");
			CreateMethodReference("Present", "(ZLjava/lang/String;ZZFFFF)V");
			CreateMethodReference("GetTime", "()F");
			CreateMethodReference("CreateSubtitle", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;FFFF)J");
			CreateMethodReference("SetSubtitleColour", "(JFFFF)V");
			CreateMethodReference("RemoveSubtitle", "(J)V");
		}
		//--------------------------------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------------------------------
		bool VideoPlayerJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == VideoPlayerJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------
		/// Set Update Subtitles Delegate
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::SetUpdateSubtitlesDelegate(const UpdateSubtitlesDelegate& inDelegate)
		{
			mUpdateSubtitlesDelegate = inDelegate;
		}
		//--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::Present(bool inbInAPK, const std::string& instrFilename, bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour, const VideoCompleteDelegate& inVideoCompleteDelegate)
		{
			mVideoCompleteDelegate = inVideoCompleteDelegate;

			bool bHasSubtitles = false;
			if (mUpdateSubtitlesDelegate != nullptr)
			{
				bHasSubtitles = true;
			}

			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrFilename = JavaInterfaceUtils::CreateJStringFromSTDString(instrFilename);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Present"), inbInAPK, jstrFilename, inbCanDismissWithTap, bHasSubtitles, inBackgroundColour.r, inBackgroundColour.g, inBackgroundColour.b, inBackgroundColour.a);
			pEnv->DeleteLocalRef(jstrFilename);
		}
		//--------------------------------------------------------------
		/// Get Time
		//--------------------------------------------------------------
		float VideoPlayerJavaInterface::GetTime()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallFloatMethod(GetJavaObject(), GetMethodID("GetTime"));
		}
		//--------------------------------------------------------------
		/// Stopped
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::OnVideoComplete()
		{
			if (mVideoCompleteDelegate != nullptr)
			{
				VideoCompleteDelegate delegate = mVideoCompleteDelegate;
				mVideoCompleteDelegate = nullptr;
				delegate();
			}
		}
		//--------------------------------------------------------------
		/// Update Subtitles
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::OnUpdateSubtitles()
		{
			if (mUpdateSubtitlesDelegate != nullptr)
			{
				mUpdateSubtitlesDelegate();
			}
		}
		//--------------------------------------------------------------
		/// Create Subtitle
		//--------------------------------------------------------------
		s64 VideoPlayerJavaInterface::CreateSubtitle(const std::string& inText, const std::string& instrFontName, u32 inudwFontSize, const std::string& instrAlignment, f32 infX, f32 infY, f32 infWidth, f32 infHeight)
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrText = JavaInterfaceUtils::CreateJStringFromSTDString(inText);
			jstring jstrFontName = JavaInterfaceUtils::CreateJStringFromSTDString(instrFontName);
			jstring jstrAlignment = JavaInterfaceUtils::CreateJStringFromSTDString(instrAlignment);
			s64 lwSubtitleID = pEnv->CallLongMethod(GetJavaObject(), GetMethodID("CreateSubtitle"), jstrText, jstrFontName, inudwFontSize, jstrAlignment, infX, infY, infWidth, infHeight);
			pEnv->DeleteLocalRef(jstrText);
			pEnv->DeleteLocalRef(jstrFontName);
			pEnv->DeleteLocalRef(jstrAlignment);
			return lwSubtitleID;
		}
		//--------------------------------------------------------------
		/// Set Subtitle Colour
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::SetSubtitleColour(s64 inlwSubtitleID, f32 infR, f32 infG, f32 infB, f32 infA)
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("SetSubtitleColour"), inlwSubtitleID, infR, infG, infB, infA);
		}
		//--------------------------------------------------------------
		/// Remove Subtitle
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::RemoveSubtitle(s64 inlwSubtitleID)
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("RemoveSubtitle"), inlwSubtitleID);
		}
	}
}

#endif