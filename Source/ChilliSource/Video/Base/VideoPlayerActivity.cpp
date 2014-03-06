//
//  Video.cpp
//  CMMatchDay
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Video/Base/VideoPlayerActivity.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Video/Base/VideoPlayerActivity.h>
#endif

#ifdef CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Video/Base/VideoPlayerActivity.h>
#endif

namespace ChilliSource
{
    namespace Video
    {
		CS_DEFINE_NAMEDTYPE(VideoPlayerActivity);
        //-------------------------------------------------------
        //-------------------------------------------------------
        VideoPlayerActivityUPtr VideoPlayerActivity::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return VideoPlayerActivityUPtr(new iOS::VideoPlayerActivity());
#endif
#ifdef CS_TARGETPLATFORM_ANDROID
            return VideoPlayerActivityUPtr(new Android::VideoPlayerActivity());
#endif
            return nullptr;
        }
        //--------------------------------------------------------------
        /// Get Dismissed Event
        ///
        /// @return Event thats triggered when the video gets dismissed
        ///			by the player.
        //--------------------------------------------------------------
        Core::IConnectableEvent<VideoDismissedEventDelegate>& VideoPlayerActivity::GetDismissedEvent()
        {
            return mOnDismissedEvent;
        }
        //--------------------------------------------------------------
        /// Get Playback Complete Event
        ///
        /// @return Event thats triggered when the video stops
        //--------------------------------------------------------------
        Core::IConnectableEvent<VideoPlaybackEventDelegate>& VideoPlayerActivity::GetPlaybackCompleteEvent()
        {
            return mOnPlaybackCompleteEvent;
        }
    }
}
