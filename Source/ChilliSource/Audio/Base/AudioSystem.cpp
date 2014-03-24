//
//  AudioSystem.cpp
//  Chilli Source
//
//  Created by Scott Downie on 17/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Audio/Base/AudioSystem.h>

#include <ChilliSource/Audio/Base/AudioSystem.h>

#ifdef CS_TARGETAUDIO_FMOD
#include <ChilliSource/Backend/Audio/FMOD/Base/FMODSystem.h>
#endif

namespace ChilliSource
{
	namespace Audio
	{
        CS_DEFINE_NAMEDTYPE(AudioSystem);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        AudioSystemUPtr AudioSystem::Create()
        {
#ifdef CS_TARGETAUDIO_FMOD
            return AudioSystemUPtr(new FMOD::FMODSystem());
#endif
            
            return nullptr;
        }
		//-------------------------------------------------------
		//-------------------------------------------------------
		AudioSystem::AudioSystem()
        : m_masterEffectVolume(k_defaultAudioVolume)
        , m_masterStreamVolume(k_defaultAudioVolume)
        , m_dopplerFactor(k_defaultDoppler)
        , m_rolloffFactor(k_defaultRolloff)
        , m_distanceFactor(k_defaultDistance)
        , m_audioComponentFactory(nullptr)
        , m_audioManager(nullptr)
		{
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		const AudioManager* AudioSystem::GetAudioManager() const
		{
			return m_audioManager;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		u32  AudioSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::ComponentFactory* AudioSystem::GetComponentFactoryPtr(u32 inudwIndex)
		{
			return m_audioComponentFactory;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::ComponentFactory& AudioSystem::GetComponentFactory(u32 inudwIndex)
		{
			return *m_audioComponentFactory;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::IConnectableEvent<AudioVolumeEventDelegate>& AudioSystem::GetMasterEffectVolumeChangedEvent()
		{
			return m_masterEffectVolumeChangedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::IConnectableEvent<AudioVolumeEventDelegate>& AudioSystem::GetMasterStreamVolumeChangedEvent()
		{
			return m_masterStreamVolumeChangedEvent;
		}
	}
}