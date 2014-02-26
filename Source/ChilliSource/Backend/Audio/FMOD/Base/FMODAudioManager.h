/*
 *  FMODAudioManager.h
 *  iOSTemplate
 *
 *  Created by Scott Downie on 10/08/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_PLATFORM_IOS_FMOD_FMOD_AUDIO_MANAGER_H_
#define _MO_FLO_PLATFORM_IOS_FMOD_FMOD_AUDIO_MANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/Base/AudioManager.h>

namespace ChilliSource
{
	namespace iOS
	{
		class CFMODAudioManager : public Audio::AudioManager
		{
		public:
			
			//-----------------------------------------------------------------
			/// Create Audio Resource
			///
			/// @return Concrete audio resource based on the audio system
			//-----------------------------------------------------------------
			Audio::AudioResourceSPtr CreateAudioResource() const;
		};
	}
}

#endif