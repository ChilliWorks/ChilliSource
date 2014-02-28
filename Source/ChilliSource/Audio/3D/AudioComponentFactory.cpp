/*
 * File: AudioComponentFactory.cpp
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/3D/AudioComponentFactory.h>

namespace ChilliSource
{
	namespace Audio
	{
        CS_DEFINE_NAMEDTYPE(AudioComponentFactory);
		//--------------------------------------------------------
		/// Constructor
		///
		/// @param Audio manager instance so we can create audio
		//--------------------------------------------------------
		AudioComponentFactory::AudioComponentFactory(AudioSystem* inpAudioSystem, AudioManager* inpAudioMgr) : mpAudioManager(inpAudioMgr), mpAudioSystem(inpAudioSystem)
		{
			
		}
	}
}

