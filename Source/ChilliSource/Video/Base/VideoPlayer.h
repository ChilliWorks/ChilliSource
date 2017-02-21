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

#ifndef _CHILLISOURCE_VIDEO_BASE_VIDEOPLAYER_H_
#define _CHILLISOURCE_VIDEO_BASE_VIDEOPLAYER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Delegate/ConnectableDelegate.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/File/StorageLocation.h>
#include <ChilliSource/Core/System/StateSystem.h>

#include <functional>

namespace ChilliSource
{
    //--------------------------------------------------------------
    /// A state system for cross platform playback of videos.
    ///
    /// @author S Downie
    //--------------------------------------------------------------
    class VideoPlayer : public StateSystem
    {
    public:
        CS_DECLARE_NAMEDTYPE(VideoPlayer);
        //-------------------------------------------------------
        /// A delegate called when the video finishes playback.
        ///
        /// @author S Downie
        //-------------------------------------------------------
        using VideoCompleteDelegate = ConnectableDelegate<void()>;
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
        virtual void Present(StorageLocation in_storageLocation, const std::string& in_fileName, VideoCompleteDelegate::Connection&& in_delegateConnection, bool in_dismissWithTap = true,
                             const Colour& in_backgroundColour = Colour::k_black) = 0;
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
        virtual void PresentWithSubtitles(StorageLocation in_storageLocation, const std::string& in_fileName, const SubtitlesCSPtr& in_subtitles, VideoCompleteDelegate::Connection&& in_delegateConnection,
                                          bool in_dismissWithTap, const Colour& in_backgroundColour = Colour::k_black) = 0;
        //-------------------------------------------------------
        /// Returns whether or not the player is currently
        /// presented.
        ///
        /// @author Jordan Brown
        //-------------------------------------------------------
        virtual bool IsPresented() const noexcept = 0;
        //--------------------------------------------------------
        /// Destructor
        ///
        /// @author S Downie
        //--------------------------------------------------------
        virtual ~VideoPlayer(){}
    protected:
        friend class State;
        //-------------------------------------------------------
        /// Create the platform dependent backend
        ///
        /// @author S Downie
        ///
        /// @return New backend instance
        //-------------------------------------------------------
        static VideoPlayerUPtr Create();
        //-------------------------------------------------------
        /// Private constructor to force use of factory method
        ///
        /// @author S Downie
        //-------------------------------------------------------
        VideoPlayer() = default;
    };
}

#endif
