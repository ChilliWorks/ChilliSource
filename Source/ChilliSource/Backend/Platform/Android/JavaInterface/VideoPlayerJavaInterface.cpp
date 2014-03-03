/*
 *  MediaPlayerJavaInterface.cpp
 *  moFlow
 *
 *  Created by Ian Copland on 10/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Platform/Android/JavaInterface/VideoPlayerJavaInterface.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceManager.h>
#include <ChilliSource/Backend/Platform/Android/JavaInterface/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Colour.h>

#include <jni.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	void Java_com_taggames_moflow_nativeinterface_CVideoPlayerNativeInterface_Dismissed(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CVideoPlayerNativeInterface_Stopped(JNIEnv* inpEnv, jobject inThis);
	void Java_com_taggames_moflow_nativeinterface_CVideoPlayerNativeInterface_UpdateSubtitles(JNIEnv* inpEnv, jobject inThis);
}
//-------------------------------------------
/// Dismissed
///
/// Called when a video is dismissed
///
/// @param The jni environment.
/// @param The java object calling the function.
//-------------------------------------------
void Java_com_taggames_moflow_nativeinterface_CVideoPlayerNativeInterface_Dismissed(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::VideoPlayerJavaInterfacePtr pVideoPJI = ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CVideoPlayerJavaInterface>();
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
void Java_com_taggames_moflow_nativeinterface_CVideoPlayerNativeInterface_Stopped(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::VideoPlayerJavaInterfacePtr pVideoPJI = ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CVideoPlayerJavaInterface>();
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
void Java_com_taggames_moflow_nativeinterface_CVideoPlayerNativeInterface_UpdateSubtitles(JNIEnv* inpEnv, jobject inThis)
{
	ChilliSource::Android::VideoPlayerJavaInterfacePtr pVideoPJI = ChilliSource::Android::CJavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<ChilliSource::Android::CVideoPlayerJavaInterface>();
	if (pVideoPJI != NULL)
		pVideoPJI->UpdateSubtitles();
}

namespace ChilliSource
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(CVideoPlayerJavaInterface);
		//--------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------
		CVideoPlayerJavaInterface::CVideoPlayerJavaInterface()
		{
			CreateNativeInterface("com/taggames/moflow/nativeinterface/CVideoPlayerNativeInterface");
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
		bool CVideoPlayerJavaInterface::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CVideoPlayerJavaInterface::InterfaceID);
		}
		//--------------------------------------------------------------
		/// Set Update Subtitles Delegate
		//--------------------------------------------------------------
		void CVideoPlayerJavaInterface::SetUpdateSubtitlesDelegate(OnUpdateSubtitlesDelegate inDelegate)
		{
			mUpdateSubtitlesDelegate = inDelegate;
		}
		//--------------------------------------------------------------
		/// Present
		//--------------------------------------------------------------
		void CVideoPlayerJavaInterface::Present(bool inbInAPK, std::string instrFilename, bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour,
												const OnVideoDismissedDelegate& inDismissedDelegate, const OnVideoStoppedDelegate& inStoppedDelegate)
		{
			mVideoDismissedDelegate = inDismissedDelegate;
			mVideoStoppedDelegate = inStoppedDelegate;

			bool bHasSubtitles = false;
			if (mUpdateSubtitlesDelegate != NULL)
			{
				bHasSubtitles = true;
			}

			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			jstring jstrFilename = JavaInterfaceUtils::CreateJStringFromSTDString(instrFilename);
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Present"), inbInAPK, jstrFilename, inbCanDismissWithTap, bHasSubtitles, inBackgroundColour.r, inBackgroundColour.g, inBackgroundColour.b, inBackgroundColour.a);
			pEnv->DeleteLocalRef(jstrFilename);
		}
		//--------------------------------------------------------------
		/// Is Playing
		//--------------------------------------------------------------
		bool CVideoPlayerJavaInterface::IsPlaying()
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallBooleanMethod(GetJavaObject(), GetMethodID("IsPlaying"));
		}
		//--------------------------------------------------------------
		/// Get Duration
		//--------------------------------------------------------------
		float CVideoPlayerJavaInterface::GetDuration()
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallFloatMethod(GetJavaObject(), GetMethodID("GetDuration"));
		}
		//--------------------------------------------------------------
		/// Get Time
		//--------------------------------------------------------------
		float CVideoPlayerJavaInterface::GetTime()
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			return pEnv->CallFloatMethod(GetJavaObject(), GetMethodID("GetTime"));
		}
		//--------------------------------------------------------------
		/// Dismiss
		//--------------------------------------------------------------
		void CVideoPlayerJavaInterface::Dismiss()
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("Dismiss"));
		}
		//--------------------------------------------------------------
		/// Dismissed
		//--------------------------------------------------------------
		void CVideoPlayerJavaInterface::Dismissed()
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
		void CVideoPlayerJavaInterface::Stopped()
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
		s64 CVideoPlayerJavaInterface::CreateSubtitle(const Core::UTF8String& inText, const std::string& instrFontName, u32 inudwFontSize, const std::string& instrAlignment, f32 infX, f32 infY, f32 infWidth, f32 infHeight)
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
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
		void CVideoPlayerJavaInterface::SetSubtitleColour(s64 inlwSubtitleID, f32 infR, f32 infG, f32 infB, f32 infA)
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("SetSubtitleColour"), inlwSubtitleID, infR, infG, infB, infA);
		}
		//--------------------------------------------------------------
		/// Remove Subtitle
		//--------------------------------------------------------------
		void CVideoPlayerJavaInterface::RemoveSubtitle(s64 inlwSubtitleID)
		{
			JNIEnv* pEnv = CJavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr();
			pEnv->CallVoidMethod(GetJavaObject(), GetMethodID("RemoveSubtitle"), inlwSubtitleID);
		}
		//--------------------------------------------------------------
		/// Update Subtitles
		//--------------------------------------------------------------
		void CVideoPlayerJavaInterface::UpdateSubtitles()
		{
			if (mUpdateSubtitlesDelegate != NULL)
			{
				mUpdateSubtitlesDelegate();
			}
		}
	}
}
