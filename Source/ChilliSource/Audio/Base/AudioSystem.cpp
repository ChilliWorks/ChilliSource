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
        namespace
        {
            const f32 k_defaultAudioVolume = 0.5f;
            const f32 k_defaultDoppler = 1.0f;
            const f32 k_defaultRolloff = 1.0f;
            const f32 k_defaultDistance = 1.0f;
        }
        
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
        : m_masterEffectVolume(k_defaultAudioVolume), m_masterStreamVolume(k_defaultAudioVolume), m_dopplerFactor(k_defaultDoppler), m_rolloffFactor(k_defaultRolloff),
        m_distanceFactor(k_defaultDistance), m_audioComponentFactory(nullptr), m_audioManager(nullptr)
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
        void AudioSystem::SetMasterEffectVolume(f32 in_volume)
        {
            m_masterEffectVolume = Core::MathUtils::Clamp(in_volume, 0.0f, 1.0f);
			
			//Trigger the active sounds OnMasterVolumeChanged delegate
			m_masterEffectVolumeChangedEvent.NotifyConnections();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        void AudioSystem::SetMasterStreamVolume(f32 in_volume)
        {
            m_masterStreamVolume = Core::MathUtils::Clamp(in_volume, 0.0f, 1.0f);
			
			//Trigger the active sounds OnMasterVolumeChanged delegate
			m_masterStreamVolumeChangedEvent.NotifyConnections();
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        f32 AudioSystem::GetMasterEffectVolume() const
        {
            return m_masterEffectVolume;
        }
        //-------------------------------------------------------
        //-------------------------------------------------------
        f32 AudioSystem::GetMasterStreamVolume() const
        {
            return m_masterStreamVolume;
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
		Core::IConnectableEvent<AudioSystem::VolumeEventDelegate>& AudioSystem::GetMasterEffectVolumeChangedEvent()
		{
			return m_masterEffectVolumeChangedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		Core::IConnectableEvent<AudioSystem::VolumeEventDelegate>& AudioSystem::GetMasterStreamVolumeChangedEvent()
		{
			return m_masterStreamVolumeChangedEvent;
		}
	}
}