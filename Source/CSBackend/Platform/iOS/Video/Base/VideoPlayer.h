//
//  VideoPlayer.h
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

#import <ChilliSource/ChilliSource.h>
#import <CSBackend/Platform/iOS/ForwardDeclarations.h>
#import <ChilliSource/Video/Base/VideoPlayer.h>

#import <atomic>
#import <mutex>

@class MPMoviePlayerController;
@class CVideoPlayerTapListener;
@class CSubtitlesRenderer;
@class UIView;

namespace CSBackend
{
    namespace iOS
    {
        //--------------------------------------------------------------
        /// The iOS backend for the video player.
        ///
        /// @author S Downie
        //--------------------------------------------------------------
        class VideoPlayer final : public ChilliSource::VideoPlayer
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
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Begin streaming the video from file
            ///
            /// @author S Downie
            ///
            /// @param The storage location of the video.
            /// @param The video file name.
            /// @param Connection to the completion delegate.
            /// @param [Optional] Whether or not the video can be
            /// dismissed by tapping. Defaults to true.
            /// @param [Optional] The video background colour. Defaults
            /// to black.
            //--------------------------------------------------------
            void Present(ChilliSource::StorageLocation in_storageLocation, const std::string& in_fileName, VideoCompleteDelegate::Connection&& in_delegateConnection, bool in_dismissWithTap = true,
                         const ChilliSource::Colour& in_backgroundColour = ChilliSource::Colour::k_black) override;
            //--------------------------------------------------------
            /// Begin streaming the video from file with subtitles.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the video.
            /// @param The video file name.
            /// @param The subtitles resource.
            /// @param Connection to the completion delegate.
            /// @param [Optional] Whether or not the video can be
            /// dismissed by tapping. Defaults to true.
            /// @param [Optional] The video background colour. Defaults
            /// to black.
            //--------------------------------------------------------
            void PresentWithSubtitles(ChilliSource::StorageLocation in_storageLocation, const std::string& in_fileName, const ChilliSource::SubtitlesCSPtr& in_subtitles, VideoCompleteDelegate::Connection&& in_delegateConnection,
                                      bool in_dismissWithTap, const ChilliSource::Colour& in_backgroundColour = ChilliSource::Colour::k_black) override;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// This isn't thread-safe, and should only be called
            /// on the system thread. Used by SubtitlesRenderer.
            ///
            /// @return The current time though the video.
			//-------------------------------------------------------
			f32 GetCurrentTime() const;
            //-------------------------------------------------------
            /// @author Ian Copland
            ///
            /// This isn't thread-safe, and should only be called
            /// on the system thread. Used by SubtitlesRenderer.
            ///
            /// @return the actual dimensions of the playing video.
            //-------------------------------------------------------
            ChilliSource::Vector2 GetVideoDimensions() const;
            //-------------------------------------------------------
            /// Returns whether or not the player is currently
            /// presented.
            ///
            /// @author Jordan Brown
            //-------------------------------------------------------
            bool IsPresented() const noexcept override;
        private:
            friend ChilliSource::VideoPlayerUPtr ChilliSource::VideoPlayer::Create();
            //-------------------------------------------------------
            /// Represents the current state of the video player.
            ///
            /// @author Jordan Brown
            //-------------------------------------------------------
            enum class State
            {
                k_inactive,
                k_loading,
                k_ready,
                k_playing
            };
            //--------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //--------------------------------------------------------
            VideoPlayer();
            //--------------------------------------------------------
            /// Called when the owning state is initialised.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void OnInit() override;
            //--------------------------------------------------------
            /// Called when the application is resumed. if a video was
            /// playing when suspend was called, it will be resumed.
            ///
            /// @author Ian Copland
            //--------------------------------------------------------
            void OnResume() override;
            //--------------------------------------------------------
            /// Called when the video is tapped.
            ///
            /// @author Ian Copland
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
            /// @author Ian Copland
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
            /// @author Ian Copland
            //--------------------------------------------------------
            void OnDestroy() override;
            
        private:
            ChilliSource::Screen* m_screen;
            
            std::atomic<State> m_currentState;
            MPMoviePlayerController* m_moviePlayerController;
            
            bool m_dismissWithTap;
            ChilliSource::SubtitlesCSPtr m_subtitles;
            UIView* m_videoOverlayView;
            CVideoPlayerTapListener* m_tapListener;
            CSubtitlesRenderer* m_subtitlesRenderer;
            
            ChilliSource::Colour m_backgroundColour;
            
            ChilliSource::EventConnectionUPtr m_moviePlayerLoadStateChangedConnection;
            ChilliSource::EventConnectionUPtr m_moviePlayerPlaybackFinishedConnection;
            
            VideoCompleteDelegate::Connection m_completionDelegateConnection;
        };
    }
}

#endif
