//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Scott Downie on 12/05/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_VIDEO_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_VIDEO_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
    namespace Video
    {
        //------------------------------------------------------
        /// Base
        //------------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(MoSubtitlesLoader);
        CS_FORWARD_DECLARE_CLASS(Subtitles);
        CS_FORWARD_DECLARE_CLASS(SubtitlesManager);
        CS_FORWARD_DECLARE_CLASS(VideoPlayerActivity);
    }
}

#endif