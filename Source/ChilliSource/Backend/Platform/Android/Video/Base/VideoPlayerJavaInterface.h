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
#include <ChilliSource/Backend/Platform/Android/Core/JNI/JavaInterface.h>

#include <jni.h>

namespace ChilliSource
{
	namespace Android
	{
		//=================================================================
		/// Video Player Java Interface
		///
		/// A Java Interface for communicating with the Android media
		/// player to play videos.
		//=================================================================
		class VideoPlayerJavaInterface : public IJavaInterface
		{
		public:
			CS_DECLARE_NAMEDTYPE();
			//--------------------------------------------------------------
			/// Delegates
			//--------------------------------------------------------------
			typedef std::function<void()> VideoCompleteDelegate;
			typedef std::function<void()> UpdateSubtitlesDelegate;
			//--------------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------------
			VideoPlayerJavaInterface();
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
			void SetUpdateSubtitlesDelegate(const UpdateSubtitlesDelegate& inDelegate);
			//--------------------------------------------------------------
			/// Present
			///
			/// Presents the video in an other activity.
			///
			/// @param whether or not the video is in the APK.
			/// @param the filename of the video.
			/// @param whether or not the video can be dismissed with a tap.
			/// @param Background colour
			/// @param video complete delegate
			//--------------------------------------------------------------
			void Present(bool inbInAPK, std::string instrFilename, bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour, const VideoCompleteDelegate& in_completeDelegate);
			//--------------------------------------------------------------
			/// Get Time
			///
			/// @return the current position through the video.
			//--------------------------------------------------------------
			float GetTime();
			//--------------------------------------------------------------
			/// Called when the video completes. This is for internal use
			/// and should not be called by the user of the interface.
			///
			/// @author I Copland
			//--------------------------------------------------------------
			void OnVideoComplete();
			//--------------------------------------------------------------
			/// Update Subtitles
			///
			/// Called every frame in a video that has subtitles. This will
			/// be called on the UI Thread.
			//--------------------------------------------------------------
			void OnUpdateSubtitles();
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
			s64 CreateSubtitle(const Core::UTF8String& inText, const std::string& instrFontName, u32 inudwFontSize, const std::string& instrAlignment, f32 infX, f32 infY, f32 infWidth, f32 infHeight);
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
		private:

			VideoCompleteDelegate mVideoCompleteDelegate;
			UpdateSubtitlesDelegate mUpdateSubtitlesDelegate;
		};
	}
}

#endif
