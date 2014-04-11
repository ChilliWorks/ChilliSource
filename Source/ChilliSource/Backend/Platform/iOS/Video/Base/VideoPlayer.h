//
//  VideoPlayer.h
//  Chilli Source
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

#ifndef _CHILLISOURCE_BACKEND_PLATFORM_IOS_VIDEO_BASE_VIDEOPLAYERSYSTEM_H_
#define _CHILLISOURCE_BACKEND_PLATFORM_IOS_VIDEO_BASE_VIDEOPLAYERSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Input/Gestures/Gestures.h>
#include <ChilliSource/Video/Base/VideoPlayer.h>


@class MPMoviePlayerController;
@class CVideoPlayerTapListener;
@class CSubtitlesRenderer;
@class UIView;

namespace ChilliSource
{
    namespace iOS
    {
        //--------------------------------------------------------------
        /// The iOS backend for the video player.
        ///
        /// @author S Downie
        //--------------------------------------------------------------
        class VideoPlayer final : public Video::VideoPlayer
        {
        public:
            CS_DECLARE_NAMEDTYPE(VideoPlayer);
			//-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author S Downie
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Begin streaming the video from file
            ///
            /// @author S Downie
            ///
            /// @param The storage location of the video.
            /// @param The video file name.
            /// @param The completion delegate.
            /// @param [Optional] Whether or not the video can be
            /// dismissed by tapping. Defaults to true.
            /// @param [Optional] The video background colour. Defaults
            /// to black.
            //--------------------------------------------------------
            void Present(Core::StorageLocation in_storageLocation, const std::string& in_fileName, const VideoCompleteDelegate& in_delegate, bool in_dismissWithTap = true,
                         const Core::Colour& in_backgroundColour = Core::Colour::k_black) override;
            //--------------------------------------------------------
            /// Begin streaming the video from file with subtitles.
            ///
            /// @author I Copland
            ///
            /// @param The storage location of the video.
            /// @param The video file name.
            /// @param The subtitles resource.
            /// @param The completion delegate.
            /// @param [Optional] Whether or not the video can be
            /// dismissed by tapping. Defaults to true.
            /// @param [Optional] The video background colour. Defaults
            /// to black.
            //--------------------------------------------------------
            void PresentWithSubtitles(Core::StorageLocation in_storageLocation, const std::string& in_fileName, const Video::SubtitlesCSPtr& in_subtitles, const VideoCompleteDelegate& in_delegate,
                                      bool in_dismissWithTap, const Core::Colour& in_backgroundColour = Core::Colour::k_black) override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return The current time though the video.
			//-------------------------------------------------------
			f32 GetCurrentTime() const;
            //-------------------------------------------------------
            /// @author I Copland
            ///
            /// @return the actual dimensions of the playing video.
            //-------------------------------------------------------
            Core::Vector2 GetVideoDimensions() const;
        private:
            
            friend Video::VideoPlayerUPtr Video::VideoPlayer::Create();
            //--------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------
            VideoPlayer();
            //--------------------------------------------------------
            /// Called when the owning state is initialised.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnInit() override;
            //--------------------------------------------------------
            /// Called when the application is resumed. if a video was
            /// playing when suspend was called, it will be resumed.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnResume() override;
            //--------------------------------------------------------
            /// Called when the video is tapped.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnTapped();
            //--------------------------------------------------------
            /// Triggered when the preloading of the video has completed
            /// This will start the playback of the movie.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            void OnLoadStateChanged();
            //--------------------------------------------------------
            /// Triggered when the movie playback ends or is stopped.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            void OnPlaybackFinished();
            //--------------------------------------------------------
            /// Prepares the video for being played.
            ///
            ///  @author S Downie.
            //--------------------------------------------------------
            void Prepare();
            //--------------------------------------------------------
            /// Plays the movie.
            ///
            /// @author S Downie.
            //--------------------------------------------------------
            void Play();
            //--------------------------------------------------------
            /// Create the movie view frame, size and position.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            void SetupMovieView();
            //--------------------------------------------------------
            /// Attach the movie players UIView to the key window.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            void AttachMovieViewToWindow();
            //--------------------------------------------------------
            /// Register with the notification centre for the relevant
            /// MPMoviePlayer notifications for starting and stopping.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            void ListenForMoviePlayerNotifications();
            //--------------------------------------------------------
            /// Deregister with the notification centre for the relevant
            /// MPMoviePlayer notifications for starting and stopping
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void StopListeningForMoviePlayerNotifications();
            //--------------------------------------------------------
            /// Creates the video overlay view and sets up any active
            /// overlay views.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            void CreateVideoOverlay();
            //--------------------------------------------------------
            /// Deletes the video overlay view and cleans up any active
            /// overlay views.
            ///
            /// @author S Downie
            //--------------------------------------------------------
            void DeleteVideoOverlay();
            //--------------------------------------------------------
            /// Called when the owning state is destroyed.
            ///
            /// @author I Copland
            //--------------------------------------------------------
            void OnDestroy() override;
            
        private:
            bool m_playing;
            MPMoviePlayerController* m_moviePlayerController;
            
            bool m_dismissWithTap;
            Video::SubtitlesCSPtr m_subtitles;
            UIView* m_videoOverlayView;
            CVideoPlayerTapListener* m_tapListener;
            CSubtitlesRenderer* m_subtitlesRenderer;
            
            Core::Colour m_backgroundColour;
            
            Core::ConnectionUPtr m_moviePlayerLoadStateChangedConnection;
            Core::ConnectionUPtr m_moviePlayerPlaybackFinishedConnection;

            VideoCompleteDelegate m_completionDelegate;
        };
    }
}

#endif