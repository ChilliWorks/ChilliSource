//
//  Video.cpp
//  moFlow
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Backend/Platform/iOS/Video/Base/VideoPlayerActivity.h>
#include <ChilliSource/Backend/Platform/iOS/Video/Base/VideoPlayerTapListener.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Notification/NSNotificationAdapter.h>
#include <ChilliSource/Backend/Platform/iOS/Core/Base/EAGLView.h>
#include <ChilliSource/Backend/Platform/iOS/Core/File/FileSystem.h>
#include <ChilliSource/Backend/Platform/iOS/Video/Base/SubtitlesRenderer.h>

#include <ChilliSource/Core/Base/Application.h>
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
        CVideoPlayerActivity::CVideoPlayerActivity()
        : mpMoviePlayerController(nil), mpTapListener(nil), mbKeepAppRunning(false), mbIsAppSuspended(false), mbPlaying(false), mbCanDismissWithTap(false), mpVideoOverlayView(nil), mpSubtitlesRenderer(nil),
        meSubtitlesLocation(Core::StorageLocation::k_none)
        
        {
            moFlo::Core::CApplicationEvents::GetResumeEvent() += moFlo::Core::ApplicationSystemDelegate(this, &CVideoPlayerActivity::OnResume);
            mpTapListener = [[CVideoPlayerTapListener alloc] init];
        }
		//--------------------------------------------------------------
		/// Is A
		///
		/// @param Interface ID
		/// @param Whether activity is of given type
		//--------------------------------------------------------------
		bool CVideoPlayerActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == Video::IVideoPlayerActivity::InterfaceID;
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
        void CVideoPlayerActivity::Present(Core::StorageLocation ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const moCore::CColour& inBackgroundColour)
        {
            mBackgroundColour = inBackgroundColour;
            
            std::string strPath;
            static_cast<CFileSystem*>(Core::CApplication::GetFileSystemPtr())->GetBestPathToFile(ineLocation, instrFileName, strPath);
            
            NSURL* pMovieURL = [NSURL fileURLWithPath:Core::CStringUtils::StringToNSString(strPath)];
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
        void CVideoPlayerActivity::PresentWithSubtitles(Core::StorageLocation ineVideoLocation, const std::string& instrVideoFilename,
                                                        Core::StorageLocation ineSubtitlesLocation, const std::string& instrSubtitlesFilename,
                                                        bool inbCanDismissWithTap, const moCore::CColour& inBackgroundColour)
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
        void CVideoPlayerActivity::SetupWithMoviePlayer()
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
        void CVideoPlayerActivity::PlayWithMoviePlayer()
        {
            CreateVideoOverlay();
            
            if(mbKeepAppRunning == false)
            {
                mbIsAppSuspended = true;
                Core::CApplication::Suspend();
            }
            
            [mpMoviePlayerController play];
        }
        //---------------------------------------------------------------
        /// Listen For Movie Player Notifications
        ///
        /// Register with the notification centre for the relevant
        /// MPMoviePlayer notifications for starting and stopping
        //---------------------------------------------------------------
        void CVideoPlayerActivity::ListenForMoviePlayerNotifications()
        {
            [[NSNotificationAdapter sharedInstance] BeginListeningForMPLoadStateChanged];
            [[NSNotificationAdapter sharedInstance] GetMPLoadStateChangeEvent].AddListener(NotificationEventDelegate(this, &CVideoPlayerActivity::OnLoadStateChanged));
            [[NSNotificationAdapter sharedInstance] BeginListeningForMPPlaybackDidFinish];
            [[NSNotificationAdapter sharedInstance] GetMPPlaybackDidFinishEvent].AddListener(NotificationEventDelegate(this, &CVideoPlayerActivity::OnPlaybackFinished));
        }
        //---------------------------------------------------------------
        /// Stop Listening For Movie Player Notifications
        ///
        /// Deregister with the notification centre for the relevant
        /// MPMoviePlayer notifications for starting and stopping
        //---------------------------------------------------------------
        void CVideoPlayerActivity::StopListeningForMoviePlayerNotifications()
        {
            [[NSNotificationAdapter sharedInstance] StopListeningForMPLoadStateChanged];
            [[NSNotificationAdapter sharedInstance] GetMPLoadStateChangeEvent].RemoveListener(NotificationEventDelegate(this, &CVideoPlayerActivity::OnLoadStateChanged));
            [[NSNotificationAdapter sharedInstance] StopListeningForMPPlaybackDidFinish];
            [[NSNotificationAdapter sharedInstance] GetMPPlaybackDidFinishEvent].RemoveListener(NotificationEventDelegate(this, &CVideoPlayerActivity::OnPlaybackFinished));
        }
        //--------------------------------------------------------------
        /// Is Playing
        ///
        /// @return Whether a video is currently playing
        //--------------------------------------------------------------
        bool CVideoPlayerActivity::IsPlaying() const
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
        f32 CVideoPlayerActivity::GetDuration() const
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
        void CVideoPlayerActivity::Dismiss()
        {
            if(mpMoviePlayerController)
            {
                mOnDismissedEvent.Invoke();
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
        void CVideoPlayerActivity::KeepAppRunning(bool inbEnabled)
        {
            mbKeepAppRunning = inbEnabled;
        }
        //--------------------------------------------------------------
        /// Get Video Dimensions (iOS Specific)
        ///
        /// @return the dimensions of the video as displayed on screen
        /// minus the black borders.
        //--------------------------------------------------------------
        Core::CVector2 CVideoPlayerActivity::GetVideoDimensions()
        {
            return Core::CVector2(mpMoviePlayerController.naturalSize.width, mpMoviePlayerController.naturalSize.height);
        }
        //---------------------------------------------------------------
        /// On Tapped
        ///
        /// Called when the video is tapped.
        //---------------------------------------------------------------
        void CVideoPlayerActivity::OnTapped()
        {
            Dismiss();
        }
        //---------------------------------------------------------------
        /// On Load State Changed
        ///
        /// Triggered when the preloading of the video has completed
        /// This will start the playback of the movie
        //---------------------------------------------------------------
        void CVideoPlayerActivity::OnLoadStateChanged()
        {
            //Unless state is unknown, start playback
            if([mpMoviePlayerController loadState] != MPMovieLoadStateUnknown)
            {
                [[NSNotificationAdapter sharedInstance] StopListeningForMPLoadStateChanged];
                [[NSNotificationAdapter sharedInstance] GetMPLoadStateChangeEvent].RemoveListener(NotificationEventDelegate(this, &CVideoPlayerActivity::OnLoadStateChanged));
                
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
        void CVideoPlayerActivity::SetupMovieView()
        {
            f32 fOrientedWidthDensityCorrected = Core::CScreen::GetOrientedWidth() * Core::CScreen::GetInverseDensity();
            f32 fOrientedHeightDensityCorrected = Core::CScreen::GetOrientedHeight() * Core::CScreen::GetInverseDensity();
            
            mpMoviePlayerController.backgroundView.backgroundColor = [UIColor colorWithRed:mBackgroundColour.r green:mBackgroundColour.g blue:mBackgroundColour.b alpha:mBackgroundColour.a];
            [[mpMoviePlayerController view] setFrame:CGRectMake(0, 0, fOrientedWidthDensityCorrected, fOrientedHeightDensityCorrected)];
        }
        //---------------------------------------------------------------
        /// Attach Movie View To Window
        ///
        /// Attach the movie players UIView to the key window
        //---------------------------------------------------------------
        void CVideoPlayerActivity::AttachMovieViewToWindow()
        {
            [[[[[UIApplication sharedApplication] keyWindow] rootViewController] view] addSubview:mpMoviePlayerController.view];
            mbPlaying = true;
        }
        //---------------------------------------------------------------
        /// On Playback Finished
        ///
        /// Triggered when the movie playback ends or is stopped
        //---------------------------------------------------------------
        void CVideoPlayerActivity::OnPlaybackFinished()
        {
            mbPlaying = false;
            
            DeleteVideoOverlay();
            
            if(mbIsAppSuspended == true)
            {
                //Resume the application
                Core::CApplication::Resume();
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
            [[NSNotificationAdapter sharedInstance] GetMPPlaybackDidFinishEvent].RemoveListener(NotificationEventDelegate(this, &CVideoPlayerActivity::OnPlaybackFinished));
            
            mOnPlaybackCompleteEvent.Invoke();
        }
        //---------------------------------------------------------------
        /// On Resume
        ///
        /// Called when the application is resumed. if a video was playing
        /// when suspend was called, it will be resumed.
        //---------------------------------------------------------------
        void CVideoPlayerActivity::OnResume()
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
        void CVideoPlayerActivity::CreateVideoOverlay()
        {
            if (mpVideoOverlayView == nil)
            {
                //create the overlay
                CGRect rect = CGRectMake(0, 0, Core::CScreen::GetOrientedWidth() * Core::CScreen::GetInverseDensity(), Core::CScreen::GetOrientedHeight() * Core::CScreen::GetInverseDensity());
                mpVideoOverlayView = [[UIView alloc] initWithFrame: rect];
                UIView* rootView = [[[[UIApplication sharedApplication] keyWindow] rootViewController] view];
                [rootView addSubview:mpVideoOverlayView];
                [rootView bringSubviewToFront:mpVideoOverlayView];
                
                //setup the tap gesture if we can dismiss with tap
                if (mbCanDismissWithTap && mpTapListener != nil)
                {
                    [mpTapListener SetupWithView: mpVideoOverlayView AndDelegate:moFlo::iOSPlatform::VideoPlayerTappedDelegate(this, &CVideoPlayerActivity::OnTapped)];
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
        void CVideoPlayerActivity::DeleteVideoOverlay()
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
        f32 CVideoPlayerActivity::GetTime() const
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
        CVideoPlayerActivity::~CVideoPlayerActivity()
        {
            [mpTapListener release];
            moFlo::Core::CApplicationEvents::GetResumeEvent() -= moFlo::Core::ApplicationSystemDelegate(this, &CVideoPlayerActivity::OnResume);
            
            StopListeningForMoviePlayerNotifications();
        }
    }
}
