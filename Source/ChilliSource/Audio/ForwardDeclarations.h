//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Scott Downie on 21/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_AUDIO_FORWARDDECLARATIONS_H_
#define _CHILLISOURCE_AUDIO_FORWARDDECLARATIONS_H_

#include <ChilliSource/Core/Base/StandardMacros.h>

#include <memory>

namespace ChilliSource
{
	namespace Audio
	{
        //--------------------------------------------------
        /// 3D
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(AudioComponent);
        CS_FORWARDDECLARE_CLASS(AudioComponentFactory);
        CS_FORWARDDECLARE_CLASS(AudioListener);
        CS_FORWARDDECLARE_CLASS(AudioListenerComponent);
        //--------------------------------------------------
        /// Base
        //--------------------------------------------------
        CS_FORWARDDECLARE_CLASS(AudioLoader);
        CS_FORWARDDECLARE_CLASS(AudioManager);
        CS_FORWARDDECLARE_CLASS(AudioPlayer);
        CS_FORWARDDECLARE_CLASS(AudioResource);
        CS_FORWARDDECLARE_CLASS(AudioSystem);
        CS_FORWARDDECLARE_CLASS(SoundEvent);
	}
}

#endif