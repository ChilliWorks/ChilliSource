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
		CS_FORWARDDECLARE_CLASS(CFMODAudioComponent);
		CS_FORWARDDECLARE_CLASS(CFMODAudioComponentFactory);
		CS_FORWARDDECLARE_CLASS(CFMODAudioListener);
		//------------------------------------------------------
		/// Base
		//------------------------------------------------------
		CS_FORWARDDECLARE_CLASS(CFMODAudioLoader);
		CS_FORWARDDECLARE_CLASS(CFMODAudioManager);
		CS_FORWARDDECLARE_CLASS(CFMODAudioResource);
		CS_FORWARDDECLARE_CLASS(CFMODSystem);
	}
}

#endif