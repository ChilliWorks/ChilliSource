//
//  Video.h
//  CMMatchDay
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _MO_FLO_VIDEO_VIDEO_PLAYER_H_
#define _MO_FLO_VIDEO_VIDEO_PLAYER_H_

#include <ChilliSource/Core/Base/Activity.h>
#include <ChilliSource/Core/Event/GenericEvent.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
    namespace Video
    {
		typedef fastdelegate::FastDelegate0<> VideoDismissedEventDelegate;
		typedef fastdelegate::FastDelegate0<> VideoPlaybackEventDelegate;
        
        class IVideoPlayerActivity : public IActivity
        {
        public:
            DECLARE_NAMED_INTERFACE(IVideoPlayerActivity);
			
            IVideoPlayerActivity(){}
            virtual ~IVideoPlayerActivity(){}
            //--------------------------------------------------------------
            /// Present
            ///
            /// Begin streaming the video from file
            ///
            /// @param Storage location
            /// @param Video filename
            /// @param Whether or not the video can be dismissed by tapping.
            /// @param Background colour
            //--------------------------------------------------------------
            virtual void Present(Core::StorageLocation ineLocation, const std::string& instrFileName, bool inbCanDismissWithTap, const Core::CColour& inBackgroundColour = Core::CColour::BLACK) = 0;
            //--------------------------------------------------------------
            /// Present With Subtitles
            ///
            /// Begin streaming the video from file with subtitles.
            ///
            /// @param Video Storage location
            /// @param Video filename
            /// @param Subtitles storage location.
            /// @param Subtitles filename.
            /// @param Whether or not the video can be dismissed by tapping.
            /// @param Background colour
            //--------------------------------------------------------------
            virtual void PresentWithSubtitles(Core::StorageLocation ineVideoLocation, const std::string& instrVideoFilename,
                                              Core::StorageLocation ineSubtitlesLocation, const std::string& instrSubtitlesFilename,
                                              bool inbCanDismissWithTap, const Core::CColour& inBackgroundColour = Core::CColour::BLACK) = 0;
            //--------------------------------------------------------------
            /// Is Playing
            ///
            /// @return Whether a video is currently playing
            //--------------------------------------------------------------
            virtual bool IsPlaying() const = 0;
            //--------------------------------------------------------------
            /// Dismiss
            ///
            /// End playback of the currently playing video
            //--------------------------------------------------------------
            virtual void Dismiss() = 0;
            //--------------------------------------------------------------
            /// Get Duration
            ///
            /// @return The length of the video in seconds
            //--------------------------------------------------------------
            virtual f32 GetDuration() const = 0;
            //--------------------------------------------------------------
            /// Get Dismissed Event
            ///
            /// @return Event thats triggered when the video gets dismissed
            ///			by the player.
            //--------------------------------------------------------------
            IEvent<VideoDismissedEventDelegate>& GetDismissedEvent();
            //--------------------------------------------------------------
            /// Get Playback Complete Event
            ///
            /// @return Event thats triggered when the video stops
            //--------------------------------------------------------------
            IEvent<VideoPlaybackEventDelegate>& GetPlaybackCompleteEvent();
            //--------------------------------------------------------------
            /// Gets the current time of the video
            ///
            /// @return The elapsed time of the video
            //--------------------------------------------------------------
            virtual f32 GetTime() const = 0;
            
        protected:

            CEvent0<VideoDismissedEventDelegate> mOnDismissedEvent;
            CEvent0<VideoPlaybackEventDelegate> mOnPlaybackCompleteEvent;
        };
    }
}

#endif
