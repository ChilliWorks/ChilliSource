//
//  VideoPlayer.cpp
//  ChilliSource
//  Created by S Downie on 12/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_IOS

#import <CSBackend/Platform/iOS/Video/Base/VideoPlayer.h>

#import <CSBackend/Platform/iOS/Core/File/FileSystem.h>
#import <CSBackend/Platform/iOS/Core/Notification/NSNotificationAdapter.h>
#import <CSBackend/Platform/iOS/Core/String/NSStringUtils.h>
#import <CSBackend/Platform/iOS/Video/Base/SubtitlesRenderer.h>
#import <CSBackend/Platform/iOS/Video/Base/VideoOverlayView.h>
#import <CSBackend/Platform/iOS/Video/Base/VideoPlayerTapListener.h>
#import <ChilliSource/Core/Base/Application.h>
#import <ChilliSource/Core/Base/Screen.h>
#import <ChilliSource/Core/Delegate/MakeDelegate.h>
#import <ChilliSource/Core/Math/MathUtils.h>
#import <ChilliSource/Core/String/StringUtils.h>
#import <ChilliSource/Core/Threading/TaskScheduler.h>

#import <AudioToolbox/AudioSession.h>
#import <MediaPlayer/MediaPlayer.h>

namespace CSBackend
{
    namespace iOS
    {
        namespace
        {
            //--------------------------------------------------------------
            //--------------------------------------------------------------
            void AudioRouteCallback(void *in_userData, AudioSessionPropertyID in_propertyID, UInt32 in_propertyValueSize, const void *in_propertyValue)
            {
                // Only interested in audio route changes.
                if(in_propertyID != kAudioSessionProperty_AudioRouteChange)
                {
                    return;
                }
                
                const CFDictionaryRef kRouteChangeDictionary = (CFDictionaryRef)in_propertyValue;
                const CFNumberRef     kRouteChangeReasonRef  = (CFNumberRef)CFDictionaryGetValue(kRouteChangeDictionary, CFSTR (kAudioSession_AudioRouteChangeKey_Reason));
                
                SInt32 dwRouteChangeReason(0);
                CFNumberGetValue (kRouteChangeReasonRef, kCFNumberSInt32Type, &dwRouteChangeReason);
                
                // If headphones pulled out or connection to speakers lost.
                if(dwRouteChangeReason == kAudioSessionRouteChangeReason_OldDeviceUnavailable)
                {
                    // Get the movie controller.
                    MPMoviePlayerController *pMovieController = static_cast<MPMoviePlayerController*>(in_userData);
                    
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
        
        CS_DEFINE_NAMEDTYPE(VideoPlayer);
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        VideoPlayer::VideoPlayer()
            : m_moviePlayerController(nil), m_tapListener(nil), m_currentState(State::k_inactive), m_dismissWithTap(false), m_videoOverlayView(nil), m_subtitlesRenderer(nil)
        {
        }
		//--------------------------------------------------------------
		//--------------------------------------------------------------
		bool VideoPlayer::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == ChilliSource::VideoPlayer::InterfaceID || in_interfaceId == VideoPlayer::InterfaceID);
		}
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void VideoPlayer::Present(ChilliSource::StorageLocation in_storageLocation, const std::string& in_fileName, VideoCompleteDelegate::Connection&& in_delegateConnection, bool in_dismissWithTap, const ChilliSource::Colour& in_backgroundColour)
        {
            CS_RELEASE_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot present video on background thread.");
            
            auto previousState = m_currentState.exchange(State::k_loading);
            CS_RELEASE_ASSERT(previousState == State::k_inactive, "Cannot present video while a video is already playing.");
            
            m_completionDelegateConnection = std::move(in_delegateConnection);
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                m_backgroundColour = in_backgroundColour;
                m_dismissWithTap = in_dismissWithTap;

                auto fileSystem = ChilliSource::Application::Get()->GetFileSystem();
                std::string filePath;
                if (in_storageLocation == ChilliSource::StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(in_fileName) == false)
                {
                    filePath = fileSystem->GetAbsolutePathToStorageLocation(ChilliSource::StorageLocation::k_package) + fileSystem->GetPackageDLCPath() + in_fileName;
                }
                else
                {
                    filePath = fileSystem->GetAbsolutePathToStorageLocation(in_storageLocation) + in_fileName;
                }

                NSString* urlString = [NSStringUtils newNSStringWithUTF8String:filePath];
                NSURL* pMovieURL = [NSURL fileURLWithPath:urlString];
                [urlString release];
                
                m_moviePlayerController = [[MPMoviePlayerController alloc] initWithContentURL:pMovieURL];
                
                AudioSessionAddPropertyListener(kAudioSessionProperty_AudioRouteChange, AudioRouteCallback, m_moviePlayerController);
                
                ListenForMoviePlayerNotifications();
                
                Prepare();
            });
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        void VideoPlayer::PresentWithSubtitles(ChilliSource::StorageLocation in_storageLocation, const std::string& in_fileName, const ChilliSource::SubtitlesCSPtr& in_subtitles, VideoCompleteDelegate::Connection&& in_delegateConnection,
                                                     bool in_dismissWithTap, const ChilliSource::Colour& in_backgroundColour)
        {
            CS_RELEASE_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Cannot present video on background thread.");
            m_subtitles = in_subtitles;
            Present(in_storageLocation, in_fileName, std::move(in_delegateConnection), in_dismissWithTap);
        }
        //-------------------------------------------------------------
        //-------------------------------------------------------------
        f32 VideoPlayer::GetCurrentTime() const
        {
            f32 time = 0.0f;
            
            if (m_moviePlayerController != nil)
            {
                time = (f32)[m_moviePlayerController currentPlaybackTime];
            }
            
            return time;
        }
        //--------------------------------------------------------------
        //--------------------------------------------------------------
        ChilliSource::Vector2 VideoPlayer::GetVideoDimensions() const
        {
            return ChilliSource::Vector2(m_moviePlayerController.naturalSize.width, m_moviePlayerController.naturalSize.height);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        bool VideoPlayer::IsPresented() const noexcept
        {
            return (m_currentState != State::k_inactive);
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void VideoPlayer::OnInit()
        {
            m_screen = ChilliSource::Application::Get()->GetSystem<ChilliSource::Screen>();
            m_tapListener = [[CVideoPlayerTapListener alloc] init];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::Prepare()
        {
            auto previousState = m_currentState.exchange(State::k_ready);
            CS_RELEASE_ASSERT(previousState == State::k_loading, "Video player not presented, cannot prepare to play video.");
            
            [m_moviePlayerController setControlStyle:MPMovieControlStyleNone];
            [m_moviePlayerController setFullscreen:YES];
            [m_moviePlayerController setRepeatMode:MPMovieRepeatModeNone];
            [m_moviePlayerController prepareToPlay];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::Play()
        {
            auto previousState = m_currentState.exchange(State::k_playing);
            CS_RELEASE_ASSERT(previousState == State::k_ready, "Video player not prepared to play video yet, cannot play.");
            
            CreateVideoOverlay();
            [m_moviePlayerController play];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::ListenForMoviePlayerNotifications()
        {
            [[NSNotificationAdapter sharedInstance] BeginListeningForMPLoadStateChanged];
            m_moviePlayerLoadStateChangedConnection = [[NSNotificationAdapter sharedInstance] GetMPLoadStateChangeEvent].OpenConnection(ChilliSource::MakeDelegate(this, &VideoPlayer::OnLoadStateChanged));
            [[NSNotificationAdapter sharedInstance] BeginListeningForMPPlaybackDidFinish];
            m_moviePlayerPlaybackFinishedConnection = [[NSNotificationAdapter sharedInstance] GetMPPlaybackDidFinishEvent].OpenConnection(ChilliSource::MakeDelegate(this, &VideoPlayer::OnPlaybackFinished));
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::StopListeningForMoviePlayerNotifications()
        {
            m_moviePlayerLoadStateChangedConnection = nullptr;
            m_moviePlayerPlaybackFinishedConnection = nullptr;
            
            [[NSNotificationAdapter sharedInstance] StopListeningForMPLoadStateChanged];
            [[NSNotificationAdapter sharedInstance] StopListeningForMPPlaybackDidFinish];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::OnTapped()
        {
            if(m_moviePlayerController)
            {
                [m_moviePlayerController stop];
            }
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::OnLoadStateChanged()
        {
            //Unless state is unknown, start playback
            if([m_moviePlayerController loadState] != MPMovieLoadStateUnknown)
            {
                [[NSNotificationAdapter sharedInstance] StopListeningForMPLoadStateChanged];
                m_moviePlayerLoadStateChangedConnection = nullptr;
                
                SetupMovieView();
                AttachMovieViewToWindow();
                Play();
            }
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::SetupMovieView()
        {
            @autoreleasepool
            {
                f32 orientedWidthDensityCorrected = m_screen->GetResolution().x * m_screen->GetInverseDensityScale();
                f32 orientedHeightDensityCorrected = m_screen->GetResolution().y * m_screen->GetInverseDensityScale();
            
                m_moviePlayerController.backgroundView.backgroundColor = [UIColor colorWithRed:m_backgroundColour.r green:m_backgroundColour.g blue:m_backgroundColour.b alpha:m_backgroundColour.a];
                [[m_moviePlayerController view] setFrame:CGRectMake(0, 0, orientedWidthDensityCorrected, orientedHeightDensityCorrected)];
            }
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::AttachMovieViewToWindow()
        {
            [[[[[UIApplication sharedApplication] keyWindow] rootViewController] view] addSubview:m_moviePlayerController.view];
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::OnPlaybackFinished()
        {
            auto previousState = m_currentState.exchange(State::k_inactive);
            CS_RELEASE_ASSERT(previousState == State::k_playing, "Playback finished should only happen once per playing video.");
            
            DeleteVideoOverlay();
            
            if([m_moviePlayerController respondsToSelector:@selector(setFullscreen:animated:)])
            {
                [m_moviePlayerController.view removeFromSuperview];
            }
            
            AudioSessionRemovePropertyListenerWithUserData(kAudioSessionProperty_AudioRouteChange, AudioRouteCallback, m_moviePlayerController);
            
            StopListeningForMoviePlayerNotifications();
            
            [m_moviePlayerController release];
            m_moviePlayerController = nil;
            
            [[NSNotificationAdapter sharedInstance] StopListeningForMPPlaybackDidFinish];
            m_moviePlayerPlaybackFinishedConnection = nullptr;
            
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& taskContext)
            {
                if (m_completionDelegateConnection != nullptr)
                {
                    auto delegateConnection = std::move(m_completionDelegateConnection);
                    m_completionDelegateConnection = nullptr;
                    delegateConnection->Call();
                }
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::OnResume()
        {
            ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_system, [=](const ChilliSource::TaskContext& taskContext)
            {
                if (m_moviePlayerController != nil)
                {
                    if (m_currentState == State::k_playing || m_currentState == State::k_ready)
                    {
                        [m_moviePlayerController play];
                    }
                }
            });
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::CreateVideoOverlay()
        {
            if (m_videoOverlayView == nil)
            {
                //create the overlay
                CGRect rect = CGRectMake(0, 0, m_screen->GetResolution().x * m_screen->GetInverseDensityScale(), m_screen->GetResolution().y * m_screen->GetInverseDensityScale());
                m_videoOverlayView = [[VideoOverlayView alloc] initWithFrame: rect];
                UIView* rootView = [[[[UIApplication sharedApplication] keyWindow] rootViewController] view];
                [rootView addSubview:m_videoOverlayView];
                [rootView bringSubviewToFront:m_videoOverlayView];
                
                //setup the tap gesture if we can dismiss with tap
                if (m_dismissWithTap && m_tapListener != nil)
                {
                    [m_tapListener SetupWithView: m_videoOverlayView AndDelegate:ChilliSource::MakeDelegate(this, &VideoPlayer::OnTapped)];
                }
                
                //create the subtitles renderer
                if (m_subtitles != nullptr && m_subtitlesRenderer == nil)
                {
                    m_subtitlesRenderer = [[CSubtitlesRenderer alloc] initWithVideoPlayer:this view:m_videoOverlayView andSubtitles:m_subtitles];
                }
            }
        }
        //---------------------------------------------------------------
        //---------------------------------------------------------------
        void VideoPlayer::DeleteVideoOverlay()
        {
            if (m_videoOverlayView != nil)
            {
                //cleanup the subtitles renderer
                if (m_subtitlesRenderer != nil)
                {
                    [m_subtitlesRenderer CleanUp];
                    [m_subtitlesRenderer release];
                    m_subtitlesRenderer = nil;
                }
                
                //cleanup the tap listener
                if (m_tapListener != nil)
                {
                    [m_tapListener Reset];
                }
                
                //delete the overlay
                [m_videoOverlayView removeFromSuperview];
                [m_videoOverlayView release];
                m_videoOverlayView = nil;
                
                //reset the overlay options
                m_dismissWithTap = false;
            }
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
        void VideoPlayer::OnDestroy()
        {
            [m_tapListener release];
        }
    }
}

#endif
