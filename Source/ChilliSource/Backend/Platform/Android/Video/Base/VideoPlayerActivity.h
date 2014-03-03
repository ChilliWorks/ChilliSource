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

#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Video/ForwardDeclarations.h>
#include <ChilliSource/Video/Base/Subtitles.h>
#include <ChilliSource/Video/Base/VideoPlayerActivity.h>

#include <unordered_map>
#include <vector>

namespace ChilliSource
{
    namespace Android
    {
    	//==================================================================
    	/// Video Player Activity
    	///
    	/// An activity for displaying the Android video player.
    	//==================================================================
        class CVideoPlayerActivity : public Video::VideoPlayerActivity
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
			bool IsA(Core::InterfaceIDType inID) const override;
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
            void Present(Core::StorageLocation ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour) override;
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
			void PresentWithSubtitles(Core::StorageLocation ineVideoLocation, const std::string& instrVideoFilename,
									  Core::StorageLocation ineSubtitlesLocation, const std::string& instrSubtitlesFilename,
									  bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour) override;
            //--------------------------------------------------------------
            /// Is Playing
            ///
            /// @return Whether a video is currently playing
            //--------------------------------------------------------------
            bool IsPlaying() const override;
            //--------------------------------------------------------------
            /// Dismiss
            ///
            /// End playback of the currently playing video
            //--------------------------------------------------------------
            void Dismiss() override;
            //--------------------------------------------------------------
            /// Get Duration
            ///
            /// @return The length of the video in seconds
            //--------------------------------------------------------------
            f32 GetDuration() const override;
            //--------------------------------------------------------------
            /// Get Time
            ///
            /// @return How far through the video
            //--------------------------------------------------------------
            f32 GetTime() const override;
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
			void UpdateSubtitle(const Video::Subtitles::SubtitlePtr& inpSubtitle, s64 inlwSubtitleID, TimeIntervalMs inTimeMS);
            
        private:
            VideoPlayerJavaInterfacePtr mpVideoPlayerJavaInterface;
            Video::SubtitlesSPtr mpSubtitles;
            std::unordered_map<Video::Subtitles::SubtitlePtr, s64> maSubtitleMap;
            std::vector<Video::Subtitles::SubtitlePtr> maSubtitlesToRemove;
            TimeIntervalMs mCurrentSubtitleTimeMS;
        };
    }
}

#endif
