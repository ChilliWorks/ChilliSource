/*
 *  FMODAudioManager.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 10/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Audio/FMOD/Base/FMODAudioManager.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODAudioResource.h>

namespace moFlo
{
	namespace iOSPlatform
	{
		//-----------------------------------------------------------------
		/// Create Audio Resource
		///
		/// @return Concrete audio resource based on the audio system
		//-----------------------------------------------------------------
		Audio::AudioResourcePtr CFMODAudioManager::CreateAudioResource() const
		{
			return Audio::AudioResourcePtr(new CFMODAudioResource());
		}
	}
}

