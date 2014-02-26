//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_AUDIO_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_AUDIO_FORWARDDECLARATIONS_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Audio
	{
        //--------------------------------------------------
        /// 3D
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(AudioComponent);
        CS_FORWARD_DECLARE_CLASS(AudioComponentFactory);
        CS_FORWARD_DECLARE_CLASS(AudioListener);
        CS_FORWARD_DECLARE_CLASS(AudioListenerComponent);
        //--------------------------------------------------
        /// Base
        //--------------------------------------------------
        CS_FORWARD_DECLARE_CLASS(AudioLoader);
        CS_FORWARD_DECLARE_CLASS(AudioManager);
        CS_FORWARD_DECLARE_CLASS(AudioPlayer);
        CS_FORWARD_DECLARE_CLASS(AudioResource);
        CS_FORWARD_DECLARE_CLASS(AudioSystem);
        CS_FORWARD_DECLARE_CLASS(SoundEvent);
	}
}

#endif