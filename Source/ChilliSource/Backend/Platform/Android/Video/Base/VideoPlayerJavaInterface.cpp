/*
 *  MediaPlayerJavaInterface.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 10/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/Video/Base/VideoPlayerJavaInterface.h>

#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Colour.h>

#include <jni.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	void Java_com_chillisource_video_VideoPlayerNativeInterface_Dismissed(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_video_VideoPlayerNativeInterface_Stopped(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chillisource_video_VideoPlayerNativeInterface_UpdateSubtitles(JNIEnv* inpEnv, jobject inThis);
}
//-------------------------------------------
/// Dismissed
///
/// Called when a video is dismissed
///
/// @param The jni environment.
/// @param The java object calling the function.
//-------------------------------------------
void Java_com_chillisource_video_VideoPlayerNativeInterface_Dismissed(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::VideoPlayerJavaInterfacePtr pVideoPJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::VideoPlayerJavaInterface>();
	if (pVideoPJI != NULL)
		pVideoPJI->Dismissed();
}
//-------------------------------------------
/// Stopped
///
/// Called when a video is stopped
///
/// @param The jni environment.
/// @param The java object calling the function.
//-------------------------------------------
void Java_com_chillisource_video_VideoPlayerNativeInterface_Stopped(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::VideoPlayerJavaInterfacePtr pVideoPJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::VideoPlayerJavaInterface>();
	if (pVideoPJI != NULL)
		pVideoPJI->Stopped();
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
void Java_com_chillisource_video_VideoPlayerNativeInterface_UpdateSubtitles(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::VideoPlayerJavaInterfacePtr pVideoPJI = ChilliSource::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::VideoPlayerJavaInterface>();
	if (pVideoPJI != NULL)
		pVideoPJI->UpdateSubtitles();
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
			CreateMethodReference("IsPlaying", "()Z");
			CreateMethodReference("GetDuration", "()F");
			CreateMethodReference("GetTime", "()F");
			CreateMethodReference("Dismiss", "()V");
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
		void VideoPlayerJavaInterface::SetUpdateSubtitlesDelegate(OnUpdateSubtitlesDelegate inDelegate)
		{
			mUpdateSubtitlesDelegate = inDelegate;
		}
		//--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::Present(bool inbInAPK, std::string instrFilename, bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour,
												const OnVideoDismissedDelegate& inDismissedDelegate, const OnVideoStoppedDelegate& inStoppedDelegate)
		{
			mVideoDismissedDelegate = inDismissedDelegate;
			mVideoStoppedDelegate = inStoppedDelegate;

			bool bHasSubtitles = false;
			if (mUpdateSubtitlesDelegate != NULL)
			{
				bHasSubtitles = true;
			}

			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrFilename = JavaInterfaceUtils::CreateJStringFromSTDString(instrFilename);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Present"), inbInAPK, jstrFilename, inbCanDismissWithTap, bHasSubtitles, inBackgroundColour.r, inBackgroundColour.g, inBackgroundColour.b, inBackgroundColour.a);
			pEnv->DeleteLocalRef(jstrFilename);
		}
		//--------------------------------------------------------------
		/// Is Playing
		//--------------------------------------------------------------
		bool VideoPlayerJavaInterface::IsPlaying()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsPlaying"));
		}
		//--------------------------------------------------------------
		/// Get Duration
		//--------------------------------------------------------------
		float VideoPlayerJavaInterface::GetDuration()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallFloatMethod(GetJavaObject(), GetMethodID("GetDuration"));
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
		/// Dismiss
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::Dismiss()
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Dismiss"));
		}
		//--------------------------------------------------------------
		/// Dismissed
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::Dismissed()
		{
			if (mVideoDismissedDelegate != NULL)
			{
				mVideoDismissedDelegate();
				mVideoDismissedDelegate = NULL;
			}
		}
		//--------------------------------------------------------------
		/// Stopped
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::Stopped()
		{
			if (mVideoStoppedDelegate != NULL)
			{
				mVideoStoppedDelegate();
				mVideoDismissedDelegate = NULL;
				mVideoStoppedDelegate = NULL;
				mUpdateSubtitlesDelegate = NULL;
			}
		}
		//--------------------------------------------------------------
		/// Create Subtitle
		//--------------------------------------------------------------
		s64 VideoPlayerJavaInterface::CreateSubtitle(const Core::UTF8String& inText, const std::string& instrFontName, u32 inudwFontSize, const std::string& instrAlignment, f32 infX, f32 infY, f32 infWidth, f32 infHeight)
		{
			JNIEnv* pEnv = JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrText = JavaInterfaceUtils::CreateJStringFromUTF8String(inText);
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
		//--------------------------------------------------------------
		/// Update Subtitles
		//--------------------------------------------------------------
		void VideoPlayerJavaInterface::UpdateSubtitles()
		{
			if (mUpdateSubtitlesDelegate != NULL)
			{
				mUpdateSubtitlesDelegate();
			}
		}
	}
}
