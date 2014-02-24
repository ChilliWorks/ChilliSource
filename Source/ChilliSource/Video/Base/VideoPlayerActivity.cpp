//
//  Video.cpp
//  CMMatchDay
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Video/Base/VideoPlayerActivity.h>

namespace ChilliSource
{
    namespace Video
    {
		DEFINE_NAMED_INTERFACE(IVideoPlayerActivity);

        //--------------------------------------------------------------
        /// Get Dismissed Event
        ///
        /// @return Event thats triggered when the video gets dismissed
        ///			by the player.
        //--------------------------------------------------------------
        Core::IEvent<VideoDismissedEventDelegate>& IVideoPlayerActivity::GetDismissedEvent()
        {
            return mOnDismissedEvent;
        }
        //--------------------------------------------------------------
        /// Get Playback Complete Event
        ///
        /// @return Event thats triggered when the video stops
        //--------------------------------------------------------------
        Core::IEvent<VideoPlaybackEventDelegate>& IVideoPlayerActivity::GetPlaybackCompleteEvent()
        {
            return mOnPlaybackCompleteEvent;
        }
    }
}
