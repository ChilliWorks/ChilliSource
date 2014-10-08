//
//  VideoPlayerJavaInterface.cpp
//  Chilli Source
//  Created by Ian Copland on 10/08/2012.
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

#include <CSBackend/Platform/Android/Video/Base/VideoPlayerJavaInterface.h>

#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceUtils.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <jni.h>

//------------------------------------------
/// C function declarations
//------------------------------------------
extern "C"
{
	void Java_com_chilliworks_chillisource_video_VideoPlayerNativeInterface_OnVideoComplete(JNIEnv* inpEnv, jobject inThis);
	void Java_com_chilliworks_chillisource_video_VideoPlayerNativeInterface_OnUpdateSubtitles(JNIEnv* inpEnv, jobject inThis);
}
//-------------------------------------------
/// On Video Complete
///
/// Called when a video is stopped
///
/// @param The jni environment.
/// @param The java object calling the function.
//-------------------------------------------
void Java_com_chilliworks_chillisource_video_VideoPlayerNativeInterface_OnVideoComplete(JNIEnv* inpEnv, jobject inThis)
{
	CSBackend::Android::VideoPlayerJavaInterfaceSPtr pVideoPJI = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::VideoPlayerJavaInterface>();

	if (pVideoPJI != nullptr)
	{
		auto task = std::bind(&CSBackend::Android::VideoPlayerJavaInterface::OnVideoComplete, pVideoPJI.get());
		CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
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
void Java_com_chilliworks_chillisource_video_VideoPlayerNativeInterface_OnUpdateSubtitles(JNIEnv* inpEnv, jobject inThis)
{
	CSBackend::Android::VideoPlayerJavaInterfaceSPtr pVideoPJI = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::VideoPlayerJavaInterface>();
	if (pVideoPJI != nullptr)
	{
		pVideoPJI->OnUpdateSubtitles();
	}
}

namespace CSBackend
{
	namespace Android
	{
		CS_DEFINE_NAMEDTYPE(VideoPlayerJavaInterface);
		//--------------------------------------------------------------------------------------
		/// Constructor
		//--------------------------------------------------------------------------------------
		VideoPlayerJavaInterface::VideoPlayerJavaInterface()
		{
			CreateNativeInterface("com/chilliworks/chillisource/video/VideoPlayerNativeInterface");
			CreateMethodReference("Present", "(ZLjava/lang/String;ZZFFFF)V");
			CreateMethodReference("GetTime", "()F");
			CreateMethodReference("CreateSubtitle", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;FFFF)J");
			CreateMethodReference("SetSubtitleColour", "(JFFFF)V");
			CreateMethodReference("RemoveSubtitle", "(J)V");
		}
		//--------------------------------------------------------------------------------------
		/// Is A
		//--------------------------------------------------------------------------------------
		bool VideoPlayerJavaInterface::IsA(CSCore::InterfaceIDType inInterfaceID) const
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
		void VideoPlayerJavaInterface::Present(bool inbInAPK, const std::string& instrFilename, bool inbCanDismissWithTap, const CSCore::Colour& inBackgroundColour, const VideoCompleteDelegate& inVideoCompleteDelegate)
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
