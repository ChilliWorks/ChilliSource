/*
 *  FMODAudioManager.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 10/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Backend/Audio/FMOD/Base/AudioManager.h>

#include <ChilliSource/Backend/Audio/FMOD/Base/AudioResource.h>

namespace ChilliSource
{
	namespace FMOD
	{
		//-----------------------------------------------------------------
		/// Create Audio Resource
		///
		/// @return Concrete audio resource based on the audio system
		//-----------------------------------------------------------------
		Audio::AudioResourceUPtr AudioManager::CreateAudioResource() const
		{
			return Audio::AudioResourceUPtr(new AudioResource());
		}
	}
}

