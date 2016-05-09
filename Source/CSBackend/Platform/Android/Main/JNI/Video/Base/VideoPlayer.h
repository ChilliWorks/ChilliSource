//
//  VideoPlayer.h
//  Chilli Source
//  Created by Ian Copland on 10/08/2012.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2012 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_ANDROID

#ifndef _CSBACKEND_PLATFORM_ANDROID_VIDEO_BASE_VIDEOPLAYER_H_
#define _CSBACKEND_PLATFORM_ANDROID_VIDEO_BASE_VIDEOPLAYER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Video/Base/Subtitles.h>
#include <ChilliSource/Video/Base/VideoPlayer.h>

#include <CSBackend/Platform/Android/Main/JNI/ForwardDeclarations.h>
#include <CSBackend/Platform/Android/Main/JNI/Core/Java/JavaSystem.h>

#include <unordered_map>
#include <vector>

namespace CSBackend
{
    namespace Android
    {
    	//------------------------------------------------------------------------------
    	/// The Android specific backend for the Video Player.
    	///
    	/// @author Ian Copland
    	//------------------------------------------------------------------------------
        class VideoPlayer final : public ChilliSource::VideoPlayer
        {
        public:
        	CS_DECLARE_NAMEDTYPE(VideoPlayer);
			//------------------------------------------------------------------------------
			/// Queries whether or not this system implements the interface with the given
			/// Id.
			///
            /// @author Ian Copland
            ///
			/// @param in_interfaceId - The interface Id.
			///
			/// @return Whether system is of given type.
			//------------------------------------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// Begin streaming the video from file
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the video.
            /// @param The video file name.
            /// @param Connection to complete delegate.
            /// @param [Optional] Whether or not the video can be dismissed by tapping.
            /// Defaults to true.
            /// @param [Optional] The video background colour. Defaults to black.
            //------------------------------------------------------------------------------
            void Present(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, VideoCompleteDelegate::Connection&& in_delegateConnection, bool in_dismissWithTap = true,
                         const ChilliSource::Colour& in_backgroundColour = ChilliSource::Colour::k_black) override;
            //------------------------------------------------------------------------------
            /// Begin streaming the video from file with subtitles.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the video.
            /// @param The video file name.
            /// @param The subtitles resource.
            /// @param Connection to complete delegate.
            /// @param [Optional] Whether or not the video can be dismissed by tapping.
            /// Defaults to true.
            /// @param [Optional] The video background colour. Defaults to black.
            //------------------------------------------------------------------------------
            void PresentWithSubtitles(ChilliSource::StorageLocation in_storageLocation, const std::string& in_filePath, const ChilliSource::SubtitlesCSPtr& in_subtitles, VideoCompleteDelegate::Connection&& in_delegateConnection,
                                      bool in_dismissWithTap = true, const ChilliSource::Colour& in_backgroundColour = ChilliSource::Colour::k_black) override;
            //------------------------------------------------------------------------------
            /// Called from java when the video finishes. This is for internal use only
            /// and should not be called by the user.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnVideoComplete();
            //------------------------------------------------------------------------------
            /// Updates the subtitles. Note that this is called from the Android UI thread
            /// rather than the main thread. This is for internal use only and should not
            /// be called by the user.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnUpdateSubtitles();

        private:
            friend ChilliSource::VideoPlayerUPtr ChilliSource::VideoPlayer::Create();
            //------------------------------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //------------------------------------------------------------------------------
            VideoPlayer() = default;
            //------------------------------------------------------------------------------
            /// Called when the owning state is initialised.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
			//------------------------------------------------------------------------------
			/// Updates a single subtitle.
			///
			/// @author Ian Copland
			///
			/// @param The subtitle
			/// @param The subtitle Id
			/// @param The time in milliseconds.
			//------------------------------------------------------------------------------
			void UpdateSubtitle(const ChilliSource::Subtitles::Subtitle* in_subtitle, s64 in_subtitleID, TimeIntervalMs in_timeMS);
            //------------------------------------------------------------------------------
            /// Called when the owning state is destroyed.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;

            bool m_isPlaying = false;
            VideoCompleteDelegate::Connection m_completionDelegateConnection;
            ChilliSource::SubtitlesCSPtr m_subtitles;
            std::unordered_map<const ChilliSource::Subtitles::Subtitle*, s64> m_subtitleMap;
            std::vector<const ChilliSource::Subtitles::Subtitle*> m_subtitlesToRemove;
            TimeIntervalMs m_currentSubtitleTimeMS = 0;

            JavaSystemUPtr m_javaSystem;
        };
    }
}

#endif

#endif
