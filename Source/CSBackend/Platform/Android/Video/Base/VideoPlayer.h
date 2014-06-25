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
#include <CSBackend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Video/Base/Subtitles.h>
#include <ChilliSource/Video/Base/VideoPlayer.h>

#include <unordered_map>
#include <vector>

namespace CSBackend
{
    namespace Android
    {
    	//--------------------------------------------------------------------
    	/// The Android specific backend for the Video Player.
    	///
    	/// @author Ian Copland
    	//--------------------------------------------------------------------
        class VideoPlayer final : public CSVideo::VideoPlayer
        {
        public:
        	CS_DECLARE_NAMEDTYPE(VideoPlayer);
			//-------------------------------------------------------
			/// Queries whether or not this system implements the
            /// interface with the given Id.
			///
            /// @author Ian Copland
            ///
			/// @param The interface Id.
			/// @param Whether system is of given type.
			//-------------------------------------------------------
			bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Begin streaming the video from file
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the video.
            /// @param The video file name.
            /// @param Connection to complete delegate.
            /// @param [Optional] Whether or not the video can be
            /// dismissed by tapping. Defaults to true.
            /// @param [Optional] The video background colour. Defaults
            /// to black.
            //--------------------------------------------------------
            void Present(CSCore::StorageLocation in_storageLocation, const std::string& in_fileName, VideoCompleteDelegate::Connection&& in_delegateConnection, bool in_dismissWithTap = true,
                         const CSCore::Colour& in_backgroundColour = CSCore::Colour::k_black) override;
            //--------------------------------------------------------
            /// Begin streaming the video from file with subtitles.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the video.
            /// @param The video file name.
            /// @param The subtitles resource.
            /// @param Connection to complete delegate.
            /// @param [Optional] Whether or not the video can be
            /// dismissed by tapping. Defaults to true.
            /// @param [Optional] The video background colour. Defaults
            /// to black.
            //--------------------------------------------------------
            void PresentWithSubtitles(CSCore::StorageLocation in_storageLocation, const std::string& in_fileName, const CSVideo::SubtitlesCSPtr& in_subtitles, VideoCompleteDelegate::Connection&& in_delegateConnection,
                                      bool in_dismissWithTap, const CSCore::Colour& in_backgroundColour = CSCore::Colour::k_black) override;
        private:
            friend CSVideo::VideoPlayerUPtr CSVideo::VideoPlayer::Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            VideoPlayer();
            //-------------------------------------------------------
            /// Called when the owning state is initialised.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            void OnInit() override;
            //-------------------------------------------------------
			/// Called from java when the video finishes.
            ///
            /// @author Ian Copland
			//-------------------------------------------------------
			void OnVideoComplete();
			//-------------------------------------------------------
			/// Updates the subtitles. Note that this is called from
			/// the Android UI thread rather than the main thread.
            ///
            /// @author Ian Copland
			//-------------------------------------------------------
			void OnUpdateSubtitles();
			//-------------------------------------------------------
			/// Updates a single subtitle.
			///
			/// @author Ian Copland
			///
			/// @param The subtitle
			/// @param The subtitle Id
			/// @param The time in milliseconds.
			//-------------------------------------------------------
			void UpdateSubtitle(const CSVideo::Subtitles::Subtitle* in_subtitle, s64 in_subtitleID, TimeIntervalMs in_timeMS);
            //-------------------------------------------------------
            /// Called when the owning state is destroyed.
            ///
            /// @author Ian Copland
            //-------------------------------------------------------
            void OnDestroy() override;

            bool m_isPlaying;
            VideoCompleteDelegate::Connection m_completionDelegateConnection;
            VideoPlayerJavaInterfaceSPtr m_javaInterface;
            CSVideo::SubtitlesCSPtr m_subtitles;
            std::unordered_map<const CSVideo::Subtitles::Subtitle*, s64> m_subtitleMap;
            std::vector<const CSVideo::Subtitles::Subtitle*> m_subtitlesToRemove;
            TimeIntervalMs m_currentSubtitleTimeMS;
        };
    }
}

#endif

#endif
