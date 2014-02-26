//
//  Video.h
//  CMMatchDay
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_PLATFORM_IOS_VIDEO_VIDEO_PLAYER_H_
#define _MO_FLO_PLATFORM_IOS_VIDEO_VIDEO_PLAYER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Video/Base/VideoPlayerActivity.h>
#include <ChilliSource/Input/Gestures/Gestures.h>

@class MPMoviePlayerController;
@class CVideoPlayerTapListener;
@class CSubtitlesRenderer;
@class UIView;

namespace ChilliSource
{
    namespace iOS
    {
        class CVideoPlayerActivity : public Video::IVideoPlayerActivity
        {
        public:
            //--------------------------------------------------------------
            /// Constructor
            //--------------------------------------------------------------
            CVideoPlayerActivity();
            ~CVideoPlayerActivity();
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
            /// @param Whether or not the video can be dismissed by tapping.
            /// @param Background colour
            //--------------------------------------------------------------
            void Present(Core::StorageLocation ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const Core::CColour& inBackgroundColour);
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
                                      bool inbCanDismissWithTap, const Core::CColour& inBackgroundColour);
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
            /// @return The position of the video in seconds
            //--------------------------------------------------------------
            f32 GetTime() const;
            //--------------------------------------------------------------
            /// Keep App Running (iOS Specific)
            ///
            /// Sets whether or not the App should keep running while the
            /// video plays. This needs to be set prior to calling Present
            /// in order to have an effect.
            ///
            /// @param whether or not the app should keep running in the
            ///         background.
            //--------------------------------------------------------------
            void KeepAppRunning(bool inbEnabled);
            //--------------------------------------------------------------
            /// Get Video Dimensions (iOS Specific)
            ///
            /// @return the actual dimensions of the playing video.
            //--------------------------------------------------------------
            Core::CVector2 GetVideoDimensions();
        private:
            //---------------------------------------------------------------
            /// On Tapped
            ///
            /// Called when the video is tapped.
            //---------------------------------------------------------------
            void OnTapped();
            //---------------------------------------------------------------
            /// On Load State Changed
            ///
            /// Triggered when the preloading of the video has completed
            /// This will start the playback of the movie
            //---------------------------------------------------------------
            void OnLoadStateChanged();
            //---------------------------------------------------------------
            /// On Playback Finished
            ///
            /// Triggered when the movie playback ends or is stopped
            //---------------------------------------------------------------
            void OnPlaybackFinished();
            //---------------------------------------------------------------
            /// Setup With Movie Player
            ///
            /// Use the post iOS 3 movie player to setup the file
            //---------------------------------------------------------------
            void SetupWithMoviePlayer();
            //---------------------------------------------------------------
            /// Play With Movie Player
            ///
            /// Use the post iOS 3 movie player to play the file
            //---------------------------------------------------------------
            void PlayWithMoviePlayer();
            //---------------------------------------------------------------
            /// Setup Movie View
            ///
            /// Create the movie view frame, size and position
            //---------------------------------------------------------------
            void SetupMovieView();
            //---------------------------------------------------------------
            /// Attach Movie View To Window
            ///
            /// Attach the movie players UIView to the key window
            //---------------------------------------------------------------
            void AttachMovieViewToWindow();
            //---------------------------------------------------------------
            /// Listen For Movie Player Notifications
            ///
            /// Register with the notification centre for the relevant
            /// MPMoviePlayer notifications for starting and stopping
            //---------------------------------------------------------------
            void ListenForMoviePlayerNotifications();
            //---------------------------------------------------------------
            /// Stop Listening For Movie Player Notifications
            ///
            /// Deregister with the notification centre for the relevant
            /// MPMoviePlayer notifications for starting and stopping
            //---------------------------------------------------------------
            void StopListeningForMoviePlayerNotifications();
            //---------------------------------------------------------------
            /// On Resume
            ///
            /// Called when the application is resumed. if a video was playing
            /// when suspend was called, it will be resumed.
            //---------------------------------------------------------------
            void OnResume();
            //---------------------------------------------------------------
            /// Create Video Overlay
            ///
            /// Creates the video overlay view and sets up any active overlay
            /// views.
            //---------------------------------------------------------------
            void CreateVideoOverlay();
            //---------------------------------------------------------------
            /// Delete Video Overlay
            ///
            /// Deletes the video overlay view and cleans up any active
            /// overlay views,
            //---------------------------------------------------------------
            void DeleteVideoOverlay();
            
        private:
            bool mbPlaying;
            MPMoviePlayerController* mpMoviePlayerController;
            
            bool mbCanDismissWithTap;
            Core::StorageLocation meSubtitlesLocation;
            std::string mstrSubtitlesFilename;
            UIView* mpVideoOverlayView;
            CVideoPlayerTapListener* mpTapListener;
            CSubtitlesRenderer* mpSubtitlesRenderer;
            
            Core::CColour mBackgroundColour;
            
            bool mbIsAppSuspended;
            bool mbKeepAppRunning;
        };
    }
}

#endif