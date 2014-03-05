//
//  Video.cpp
//  moFlow
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Video/Base/VideoPlayerActivity.h>

#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Notification/NSNotificationAdapter.h>
#include <ChilliSource/Backend/Platform/iOS/Video/Base/SubtitlesRenderer.h>
#include <ChilliSource/Backend/Platform/iOS/Video/Base/VideoPlayerTapListener.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/String/StringUtils.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Base/ApplicationEvents.h>
#include <ChilliSource/Input/Base/InputSystem.h>

#include <MediaPlayer/MediaPlayer.h>
#include <AudioToolbox/AudioSession.h>

namespace ChilliSource
{
    namespace iOS
    {
        namespace
        {
            //--------------------------------------------------------------
            /// AudioRouteCallback
            ///
            /// Catches the audio callbacks from iOS.
            /// Used to restart the movie if it gets stopped by an audio hardware change.
            //--------------------------------------------------------------
            void AudioRouteCallback(void *inpUserData, AudioSessionPropertyID inudwPropertyID, UInt32 inudwPropertyValueSize, const void *inpPropertyValue)
            {
                // Only interested in audio route changes.
                if(inudwPropertyID != kAudioSessionProperty_AudioRouteChange)
                {
                    return;
                }
                
                const CFDictionaryRef kRouteChangeDictionary = (CFDictionaryRef)inpPropertyValue;
                const CFNumberRef     kRouteChangeReasonRef  = (CFNumberRef)CFDictionaryGetValue(kRouteChangeDictionary, CFSTR (kAudioSession_AudioRouteChangeKey_Reason));
                
                SInt32 dwRouteChangeReason(0);
                CFNumberGetValue (kRouteChangeReasonRef, kCFNumberSInt32Type, &dwRouteChangeReason);
                
                // If headphones pulled out or connection to speakers lost.
                if(dwRouteChangeReason == kAudioSessionRouteChangeReason_OldDeviceUnavailable)
                {
                    // Get the movie controller.
                    MPMoviePlayerController *pMovieController = static_cast<MPMoviePlayerController*>(inpUserData);
                    
                    // Restart the movie.
                    if(pMovieController)
                    {
                        // The sleep is required, as sometimes it seems the callback is applied before the video has been paused.
                        usleep(3000);
                        [pMovieController play];
                    }
                }
            }
        }
        
        //--------------------------------------------------------------
        /// Constructor
        ///
        /// Default
        //--------------------------------------------------------------
        VideoPlayerActivity::VideoPlayerActivity()
        : mpMoviePlayerController(nil), mpTapListener(nil), mbKeepAppRunning(false), mbIsAppSuspended(false), mbPlaying(false), mbCanDismissWithTap(false), mpVideoOverlayView(nil), mpSubtitlesRenderer(nil),
        meSubtitlesLocation(Core::StorageLocation::k_none)
        
        {
            m_appResumedConnection = Core::ApplicationEvents::GetResumeEvent().OpenConnection(Core::MakeDelegate(this, &VideoPlayerActivity::OnResume));
            mpTapListener = [[CVideoPlayerTapListener alloc] init];
        }
		//--------------------------------------------------------------
		/// Is A
		///
		/// @param Interface ID
		/// @param Whether activity is of given type
		//--------------------------------------------------------------
		bool VideoPlayerActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == Video::VideoPlayerActivity::InterfaceID;
		}
        //--------------------------------------------------------------
        /// Present
        ///
        /// Begin streaming the video from file
        ///
        /// @param Storage location
        /// @param Video filename
        /// @param Whether to allow dismissing of the video
        /// @param Background colour
        //--------------------------------------------------------------
        void VideoPlayerActivity::Present(Core::StorageLocation ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour)
        {
            mBackgroundColour = inBackgroundColour;
            
            std::string strPath;
            static_cast<FileSystem*>(Core::Application::GetFileSystemPtr())->GetBestPathToFile(ineLocation, instrFileName, strPath);
            
            NSURL* pMovieURL = [NSURL fileURLWithPath:Core::StringUtils::StringToNSString(strPath)];
            mpMoviePlayerController = [[MPMoviePlayerController alloc] initWithContentURL:pMovieURL];
            
            AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, AudioRouteCallback, mpMoviePlayerController);
            
            SetupWithMoviePlayer();
            
            mbCanDismissWithTap = inbCanDismissWithTap;
            
            ListenForMoviePlayerNotifications();
        }
        //--------------------------------------------------------------
        /// Present
        ///
        /// Begin streaming the video from file with subtitles
        ///
        /// @param Video Storage location
        /// @param Video filename
        /// @param Subtitles storage location.
        /// @param Subtitles filename.
        /// @param Whether or not the video can be dismissed by tapping.
        /// @param Background colour
        //--------------------------------------------------------------
        void VideoPlayerActivity::PresentWithSubtitles(Core::StorageLocation ineVideoLocation, const std::string& instrVideoFilename,
                                                        Core::StorageLocation ineSubtitlesLocation, const std::string& instrSubtitlesFilename,
                                                        bool inbCanDismissWithTap, const Core::Colour& inBackgroundColour)
        {
            //setup the subtitles
            meSubtitlesLocation = ineSubtitlesLocation;
            mstrSubtitlesFilename = instrSubtitlesFilename;
            
            //Present the video
            Present(ineVideoLocation, instrVideoFilename, inbCanDismissWithTap, inBackgroundColour);
        }
        //---------------------------------------------------------------
        /// Setup With Movie Player
        ///
        /// Use the post iOS 3 movie player to setup the file
        //---------------------------------------------------------------
        void VideoPlayerActivity::SetupWithMoviePlayer()
        {
            [mpMoviePlayerController setControlStyle:MPMovieControlStyleNone];
            [mpMoviePlayerController setFullscreen:YES];
            [mpMoviePlayerController setRepeatMode:MPMovieRepeatModeNone];
            
            //May help to reduce latency
            [mpMoviePlayerController prepareToPlay];
        }
        //---------------------------------------------------------------
        /// Play With Movie Player
        ///
        /// Use the post iOS 3 movie player to play the file
        //---------------------------------------------------------------
        void VideoPlayerActivity::PlayWithMoviePlayer()
        {
            CreateVideoOverlay();
            
            if(mbKeepAppRunning == false)
            {
                mbIsAppSuspended = true;
                Core::Application::Suspend();
            }
            
            [mpMoviePlayerController play];
        }
        //---------------------------------------------------------------
        /// Listen For Movie Player Notifications
        ///
        /// Register with the notification centre for the relevant
        /// MPMoviePlayer notifications for starting and stopping
        //---------------------------------------------------------------
        void VideoPlayerActivity::ListenForMoviePlayerNotifications()
        {
            [[NSNotificationAdapter sharedInstance] BeginListeningForMPLoadStateChanged];
            m_moviePlayerLoadStateChangedConnection = [[NSNotificationAdapter sharedInstance] GetMPLoadStateChangeEvent].OpenConnection(Core::MakeDelegate(this, &VideoPlayerActivity::OnLoadStateChanged));
            [[NSNotificationAdapter sharedInstance] BeginListeningForMPPlaybackDidFinish];
            m_moviePlayerPlaybackFinishedConnection = [[NSNotificationAdapter sharedInstance] GetMPPlaybackDidFinishEvent].OpenConnection(Core::MakeDelegate(this, &VideoPlayerActivity::OnPlaybackFinished));
        }
        //---------------------------------------------------------------
        /// Stop Listening For Movie Player Notifications
        ///
        /// Deregister with the notification centre for the relevant
        /// MPMoviePlayer notifications for starting and stopping
        //---------------------------------------------------------------
        void VideoPlayerActivity::StopListeningForMoviePlayerNotifications()
        {
            m_moviePlayerLoadStateChangedConnection = nullptr;
            m_moviePlayerPlaybackFinishedConnection = nullptr;
            
            [[NSNotificationAdapter sharedInstance] StopListeningForMPLoadStateChanged];
            [[NSNotificationAdapter sharedInstance] StopListeningForMPPlaybackDidFinish];
        }
        //--------------------------------------------------------------
        /// Is Playing
        ///
        /// @return Whether a video is currently playing
        //--------------------------------------------------------------
        bool VideoPlayerActivity::IsPlaying() const
        {
            if(!mpMoviePlayerController)
            {
                return false;
            }
            
            return (mpMoviePlayerController.playbackState == MPMoviePlaybackStatePlaying);
        }
        //--------------------------------------------------------------
        /// Get Duration
        ///
        /// @return The length of the video in seconds
        //--------------------------------------------------------------
        f32 VideoPlayerActivity::GetDuration() const
        {
            if(!mpMoviePlayerController)
            {
                return 0.0f;
            }
            
            return (f32)mpMoviePlayerController.duration;
        }
        //--------------------------------------------------------------
        /// Dismiss
        ///
        /// End playback of the currently playing video
        //--------------------------------------------------------------
        void VideoPlayerActivity::Dismiss()
        {
            if(mpMoviePlayerController)
            {
                mOnDismissedEvent.NotifyConnections();
                [mpMoviePlayerController stop];
            }
        }
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
        void VideoPlayerActivity::KeepAppRunning(bool inbEnabled)
        {
            mbKeepAppRunning = inbEnabled;
        }
        //--------------------------------------------------------------
        /// Get Video Dimensions (iOS Specific)
        ///
        /// @return the dimensions of the video as displayed on screen
        /// minus the black borders.
        //--------------------------------------------------------------
        Core::Vector2 VideoPlayerActivity::GetVideoDimensions()
        {
            return Core::Vector2(mpMoviePlayerController.naturalSize.width, mpMoviePlayerController.naturalSize.height);
        }
        //---------------------------------------------------------------
        /// On Tapped
        ///
        /// Called when the video is tapped.
        //---------------------------------------------------------------
        void VideoPlayerActivity::OnTapped()
        {
            Dismiss();
        }
        //---------------------------------------------------------------
        /// On Load State Changed
        ///
        /// Triggered when the preloading of the video has completed
        /// This will start the playback of the movie
        //---------------------------------------------------------------
        void VideoPlayerActivity::OnLoadStateChanged()
        {
            //Unless state is unknown, start playback
            if([mpMoviePlayerController loadState] != MPMovieLoadStateUnknown)
            {
                [[NSNotificationAdapter sharedInstance] StopListeningForMPLoadStateChanged];
                m_moviePlayerLoadStateChangedConnection = nullptr;
                
                SetupMovieView();
                AttachMovieViewToWindow();
                PlayWithMoviePlayer();
            }
        }
        //---------------------------------------------------------------
        /// Setup Movie View
        ///
        /// Create the movie view frame, size and position
        //---------------------------------------------------------------
        void VideoPlayerActivity::SetupMovieView()
        {
            f32 fOrientedWidthDensityCorrected = Core::Screen::GetOrientedWidth() * Core::Screen::GetInverseDensity();
            f32 fOrientedHeightDensityCorrected = Core::Screen::GetOrientedHeight() * Core::Screen::GetInverseDensity();
            
            mpMoviePlayerController.backgroundView.backgroundColor = [UIColor colorWithRed:mBackgroundColour.r green:mBackgroundColour.g blue:mBackgroundColour.b alpha:mBackgroundColour.a];
            [[mpMoviePlayerController view] setFrame:CGRectMake(0, 0, fOrientedWidthDensityCorrected, fOrientedHeightDensityCorrected)];
        }
        //---------------------------------------------------------------
        /// Attach Movie View To Window
        ///
        /// Attach the movie players UIView to the key window
        //---------------------------------------------------------------
        void VideoPlayerActivity::AttachMovieViewToWindow()
        {
            [[[[[UIApplication sharedApplication] keyWindow] rootViewController] view] addSubview:mpMoviePlayerController.view];
            mbPlaying = true;
        }
        //---------------------------------------------------------------
        /// On Playback Finished
        ///
        /// Triggered when the movie playback ends or is stopped
        //---------------------------------------------------------------
        void VideoPlayerActivity::OnPlaybackFinished()
        {
            mbPlaying = false;
            
            DeleteVideoOverlay();
            
            if(mbIsAppSuspended == true)
            {
                //Resume the application
                Core::Application::Resume();
                mbIsAppSuspended = false;
            }
            
            if([mpMoviePlayerController respondsToSelector:@selector(setFullscreen:animated:)]) 
            {
                [mpMoviePlayerController.view removeFromSuperview];
            }
            
            AudioSessionRemovePropertyListenerWithUserData(kAudioSessionProperty_AudioRouteChange, AudioRouteCallback, mpMoviePlayerController);
            
            [mpMoviePlayerController release];
            mpMoviePlayerController = nil;
            
            [[NSNotificationAdapter sharedInstance] StopListeningForMPPlaybackDidFinish];
            m_moviePlayerPlaybackFinishedConnection = nullptr;
            
            mOnPlaybackCompleteEvent.NotifyConnections();
        }
        //---------------------------------------------------------------
        /// On Resume
        ///
        /// Called when the application is resumed. if a video was playing
        /// when suspend was called, it will be resumed.
        //---------------------------------------------------------------
        void VideoPlayerActivity::OnResume()
        {
            if (mpMoviePlayerController != nil)
            {
                if (mbPlaying == true)
                {
                    //Finally we can actually start playback
                    [mpMoviePlayerController play];
                }
            }
        }
        //---------------------------------------------------------------
        /// Create Video Overlay
        ///
        /// Creates the video overlay view and sets up any active overlay
        /// views.
        //---------------------------------------------------------------
        void VideoPlayerActivity::CreateVideoOverlay()
        {
            if (mpVideoOverlayView == nil)
            {
                //create the overlay
                CGRect rect = CGRectMake(0, 0, Core::Screen::GetOrientedWidth() * Core::Screen::GetInverseDensity(), Core::Screen::GetOrientedHeight() * Core::Screen::GetInverseDensity());
                mpVideoOverlayView = [[UIView alloc] initWithFrame: rect];
                UIView* rootView = [[[[UIApplication sharedApplication] keyWindow] rootViewController] view];
                [rootView addSubview:mpVideoOverlayView];
                [rootView bringSubviewToFront:mpVideoOverlayView];
                
                //setup the tap gesture if we can dismiss with tap
                if (mbCanDismissWithTap && mpTapListener != nil)
                {
                    [mpTapListener SetupWithView: mpVideoOverlayView AndDelegate:Core::MakeDelegate(this, &VideoPlayerActivity::OnTapped)];
                }
                
                //create the subtitles renderer
                if (meSubtitlesLocation != Core::StorageLocation::k_none && mstrSubtitlesFilename.size() != 0 && mpSubtitlesRenderer == nil)
                {
                    mpSubtitlesRenderer = [[CSubtitlesRenderer alloc] InitWithVideoPlayer:this View:mpVideoOverlayView StorageLocation:meSubtitlesLocation Filename:mstrSubtitlesFilename];
                }
            }
        }
        //---------------------------------------------------------------
        /// Delete Video Overlay
        ///
        /// Deletes the video overlay view and cleans up any active
        /// overlay views,
        //---------------------------------------------------------------
        void VideoPlayerActivity::DeleteVideoOverlay()
        {
            if (mpVideoOverlayView != nil)
            {
                //cleanup the subtitles renderer
                if (mpSubtitlesRenderer != nil)
                {
                    [mpSubtitlesRenderer CleanUp];
                    [mpSubtitlesRenderer release];
                    mpSubtitlesRenderer = nil;
                }
                
                //cleanup the tap listener
                if (mpTapListener != nil)
                {
                    [mpTapListener Reset];
                }
                
                //delete the overlay
                [mpVideoOverlayView removeFromSuperview];
                [mpVideoOverlayView release];
                mpVideoOverlayView = nil;
                
                //reset the overlay options
                mbCanDismissWithTap = false;
                meSubtitlesLocation = Core::StorageLocation::k_none;
                mstrSubtitlesFilename = "";
            }
        }
        //--------------------------------------------------------------
        /// GetTime
        //--------------------------------------------------------------
        f32 VideoPlayerActivity::GetTime() const
        {
            f32 fTime = 0.0f;
            if (mpMoviePlayerController)
            {
                fTime = (f32)[mpMoviePlayerController currentPlaybackTime];
            }
            
            return fTime;
        }
        //--------------------------------------------------------------
        /// Destructor
        //--------------------------------------------------------------
        VideoPlayerActivity::~VideoPlayerActivity()
        {
            [mpTapListener release];

            StopListeningForMoviePlayerNotifications();
        }
    }
}
