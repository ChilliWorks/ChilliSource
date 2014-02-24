/*
 *  VideoPlayerJavaInterface.h
 *  moFlow
 *
 *  Created by Ian Copland on 10/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_VIDEOPLAYERJAVAINTERFACE_H_
#define _MOFLOW_PLATFORM_ANDROID_JAVAINTERFACE_VIDEOPLAYERJAVAINTERFACE_H_

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Platform/Android/JavaInterface/JavaInterface.h>
#include <ChilliSource/Core/Main/FastDelegate.h>
#include <jni.h>

namespace ChilliSource
{
	namespace Android
	{
		//=================================================================
		/// Delegates
		//=================================================================
		typedef fastdelegate::FastDelegate0<> OnVideoStoppedDelegate;
		typedef fastdelegate::FastDelegate0<> OnVideoDismissedDelegate;
		typedef fastdelegate::FastDelegate0<> OnUpdateSubtitlesDelegate;
		//=================================================================
		/// Video Player Java Interface
		///
		/// A Java Interface for communicating with the Android media
		/// player to play videos.
		//=================================================================
		class CVideoPlayerJavaInterface : public IJavaInterface
		{
		public:
			DECLARE_NAMED_INTERFACE();
			//--------------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------------
			CVideoPlayerJavaInterface();
			//--------------------------------------------------------------
			/// Is A
			///
			/// @return whether or not this object implements the given interface.
			//--------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//--------------------------------------------------------------
			/// Set Update Subtitles Delegate
			///
			/// @param The delegate.
			//--------------------------------------------------------------
			void SetUpdateSubtitlesDelegate(OnUpdateSubtitlesDelegate inDelegate);
			//--------------------------------------------------------------
			/// Present
			///
			/// Presents the video in an other activity.
			///
			/// @param whether or not the video is in the APK.
			/// @param the filename of the video.
			/// @param whether or not the video can be dismissed with a tap.
			/// @param Background colour
			/// @param video dismissed delegate.
			/// @param video stopped delegate.
			//--------------------------------------------------------------
			void Present(bool inbInAPK, std::string instrFilename, bool inbCanDismissWithTap, const moCore::CColour& inBackgroundColour,
						const OnVideoDismissedDelegate& inDismissedDelegate, const OnVideoStoppedDelegate& inStoppedDelegate);
			//--------------------------------------------------------------
			/// Is Playing
			///
			/// @return whether or not there is currently a video playing.
			//--------------------------------------------------------------
			bool IsPlaying();
			//--------------------------------------------------------------
			/// Get Duration
			///
			/// @return the total length of the currently running video.
			//--------------------------------------------------------------
			float GetDuration();
			//--------------------------------------------------------------
			/// Get Time
			///
			/// @return the current position through the video.
			//--------------------------------------------------------------
			float GetTime();
			//--------------------------------------------------------------
			/// Dismiss
			///
			/// Stops the current running video from playing.
			//--------------------------------------------------------------
			void Dismiss();
			//--------------------------------------------------------------
			/// Stopped
			///
			/// Called when the video stops.
			//--------------------------------------------------------------
			void Stopped();
			//--------------------------------------------------------------
			/// Dismissed
			///
			/// Called when the video is dismissed.
			//--------------------------------------------------------------
			void Dismissed();
			//--------------------------------------------------------------
			/// Create Subtitle
			///
			/// Creates a new subtitle object.
			///
			/// @param The text.
			/// @param The font name.
			/// @param The font size.
			/// @param The alignment
			/// @param X position.
			/// @param Y position.
			/// @param Width.
			/// @param Height.
			/// @return The subtitle id.
			//--------------------------------------------------------------
			s64 CreateSubtitle(const CUTF8String& inText, const std::string& instrFontName, u32 inudwFontSize, const std::string& instrAlignment, f32 infX, f32 infY, f32 infWidth, f32 infHeight);
			//--------------------------------------------------------------
			/// Set Subtitle Colour
			///
			/// Sets the colour of a subtitle.
			///
			/// @param The subtitle id.
			/// @param Red.
			/// @param Green.
			/// @param Blue.
			/// @param Alpha.
			//--------------------------------------------------------------
			void SetSubtitleColour(s64 inlwSubtitleID, f32 infR, f32 infG, f32 infB, f32 infA);
			//--------------------------------------------------------------
			/// Remove Subtitle
			///
			/// Removes the given subtitle object.
			///
			/// @param The subtitle object.
			//--------------------------------------------------------------
			void RemoveSubtitle(s64 inlwSubtitleID);
			//--------------------------------------------------------------
			/// Update Subtitles
			///
			/// Called every frame in a video that has subtitles. This will
			/// be called on the UI Thread.
			//--------------------------------------------------------------
			void UpdateSubtitles();
		private:

			OnVideoStoppedDelegate mVideoStoppedDelegate;
			OnVideoDismissedDelegate mVideoDismissedDelegate;
			OnUpdateSubtitlesDelegate mUpdateSubtitlesDelegate;
		};
	}
}

#endif
