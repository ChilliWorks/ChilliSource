//
//  ForwardDeclarations.h
//  Chilli Source
//
//  Created by Ian Copland on 03/03/2014
//	Copyright 2011 Tag Games Ltd. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_AUDIO_FMOD_FORWARDDECLARATION_H_
#define _CHILLISOURCE_BACKEND_AUDIO_FMOD_FORWARDDECLARATION_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace FMOD
	{
		//------------------------------------------------------
		/// 3D
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(AudioComponent);
		CS_FORWARDDECLARE_CLASS(AudioComponentFactory);
		CS_FORWARDDECLARE_CLASS(AudioListener);
		//------------------------------------------------------
		/// Base
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(AudioLoader);
		CS_FORWARDDECLARE_CLASS(AudioManager);
		CS_FORWARDDECLARE_CLASS(AudioResource);
		CS_FORWARDDECLARE_CLASS(FMODSystem);
	}
}

#endif