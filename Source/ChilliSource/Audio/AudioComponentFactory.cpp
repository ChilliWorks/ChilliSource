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

#include <moFlo/Audio/AudioComponentFactory.h>

namespace moFlo
{
	namespace Audio
	{
        DEFINE_NAMED_INTERFACE(IAudioComponentFactory);
		//--------------------------------------------------------
		/// Constructor
		///
		/// @param Audio manager instance so we can create audio
		//--------------------------------------------------------
		IAudioComponentFactory::IAudioComponentFactory(IAudioSystem* inpAudioSystem, IAudioManager* inpAudioMgr) : mpAudioManager(inpAudioMgr), mpAudioSystem(inpAudioSystem)
		{
			
		}
	}
}

