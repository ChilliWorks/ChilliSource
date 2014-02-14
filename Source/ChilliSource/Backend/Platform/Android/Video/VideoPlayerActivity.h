/*
 *  VideoPlayerActivity.h
 *  moFlow
 *
 *  Created by Ian Copland on 10/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MOFLOW_PLATFORM_ANDROID_VIDEO_VIDEOPLAYER_H_
#define _MOFLOW_PLATFORM_ANDROID_VIDEO_VIDEOPLAYER_H_

#include <ChilliSource/Video/VideoPlayerActivity.h>
#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Video/Subtitles.h>

namespace moFlo
{
    namespace AndroidPlatform
    {
    	//==================================================================
    	/// Video Player Activity
    	///
    	/// An activity for displaying the Android video player.
    	//==================================================================
        class CVideoPlayerActivity : public Video::IVideoPlayerActivity
        {
        public:
			//--------------------------------------------------------------
			/// Constructor
			//--------------------------------------------------------------
            CVideoPlayerActivity();
			//--------------------------------------------------------------
			/// Is A
			///
			/// @param Interface ID
			/// @param Whether activity is of given type
			//--------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inID) const;
            //--------------------------------------------------------------
            /// Present
            ///
            /// Begin streaming the video from file
            ///
            /// @param Storage location
            /// @param Video filename
            /// @param whether or not the application can be dismissed with
			/// 		taps.
			/// @param Background colour
            //--------------------------------------------------------------
            void Present(Core::STORAGE_LOCATION ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const moCore::CColour& inBackgroundColour);
            //--------------------------------------------------------------
			/// Present With Subtitles
			///
			/// Begin streaming the video from file with subtitles.
			///
			/// @param Video Storage location
			/// @param Video filename
			/// @param Subtitles storage location.
			/// @param Subtitles filename.
			/// @param Whether or not the video can be dismissed by tapping.
            /// @param Background colour
			//--------------------------------------------------------------
			void PresentWithSubtitles(Core::STORAGE_LOCATION ineVideoLocation, const std::string& instrVideoFilename,
									  Core::STORAGE_LOCATION ineSubtitlesLocation, const std::string& instrSubtitlesFilename,
									  bool inbCanDismissWithTap, const moCore::CColour& inBackgroundColour);
            //--------------------------------------------------------------
            /// Is Playing
            ///
            /// @return Whether a video is currently playing
            //--------------------------------------------------------------
            bool IsPlaying() const;
            //--------------------------------------------------------------
            /// Dismiss
            ///
            /// End playback of the currently playing video
            //--------------------------------------------------------------
            void Dismiss();
            //--------------------------------------------------------------
            /// Get Duration
            ///
            /// @return The length of the video in seconds
            //--------------------------------------------------------------
            f32 GetDuration() const;
            //--------------------------------------------------------------
            /// Get Time
            ///
            /// @return How far through the video
            //--------------------------------------------------------------
            f32 GetTime() const;
        private:
			//---------------------------------------------------------------
			/// On Video Dismissed
			///
			/// Called when the video gets dismissed by the player.
			//---------------------------------------------------------------
			void OnVideoDismissed();
            //---------------------------------------------------------------
			/// On Video Stopped
			///
			/// Called when the video stops.
			//---------------------------------------------------------------
			void OnVideoStopped();
			//---------------------------------------------------------------
			/// Video Dismissed Task
			///
			/// Main thread task for invoking the dismissed event.
			//---------------------------------------------------------------
			void VideoDismissedTask();
			//---------------------------------------------------------------
			/// Video Stopped Task
			///
			/// Main thread task for invoking the playback complete event.
			//---------------------------------------------------------------
			void VideoStoppedTask();
			//---------------------------------------------------------------
			/// Update Subtitles
			///
			/// Updates the subtitles. This will be called on the UI thread
			/// rather than the render thread.
			//---------------------------------------------------------------
			void OnUpdateSubtitles();
			//---------------------------------------------------------------
			/// Update Subtitle
			///
			/// Updates a single subtitle.
			//---------------------------------------------------------------
			void UpdateSubtitle(const moFlo::Video::CSubtitles::SubtitlePtr& inpSubtitle, s64 inlwSubtitleID, TimeIntervalMs inTimeMS);
            
        private:
            VideoPlayerJavaInterfacePtr mpVideoPlayerJavaInterface;
            moFlo::Video::SubtitlesPtr mpSubtitles;
            HASH_MAP<moFlo::Video::CSubtitles::SubtitlePtr, s64> maSubtitleMap;
            DYNAMIC_ARRAY<moFlo::Video::CSubtitles::SubtitlePtr> maSubtitlesToRemove;
            TimeIntervalMs mCurrentSubtitleTimeMS;
        };
    }
}

#endif
