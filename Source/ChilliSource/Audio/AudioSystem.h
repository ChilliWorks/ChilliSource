/*
 * File: AudioSystem.h
 * Date: 17/11/2010 2010 
 * Description: Interface for concrete systems such as FMOD
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_AUDIO_SYSTEM_H_
#define _MO_FLO_AUDIO_AUDIO_SYSTEM_H_

#include <moFlo/Audio/AudioManager.h>
#include <moFlo/Audio/AudioComponentFactory.h>

#include <moFlo/Core/System.h>
#include <moFlo/Core/SystemConcepts.h>
#include <moFlo/Core/Math/MathUtils.h>
#include <moFlo/Core/GenericEvent.h>

namespace moFlo
{
	namespace Audio
	{
		const f32 kfDefaultAudioVolume	= 0.5f;
		const f32 kfDefaultDoppler		= 1.0f;
		const f32 kfDefaultRolloff		= 1.0f;
		const f32 kfDefaultDistance		= 1.0f;
		
        typedef fastdelegate::FastDelegate0<> AudioVolumeEventDelegate;
		
		class IAudioSystem : public Core::ISystem, public Core::IUpdateable, public Core::IComponentProducer
		{
		public:
			IAudioSystem();
			virtual ~IAudioSystem();
			
			DECLARE_NAMED_INTERFACE(IAudioSystem);

			//-------------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//-------------------------------------------------------
			/// Create Sound
			///
			/// Tell concrete system to create a new sound from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with sound
			//-------------------------------------------------------
			virtual void CreateSound(const std::string& instrFilePath, IAudioResource* inpAudio) = 0;
			//-------------------------------------------------------
			/// Create 3D Sound
			///
			/// Tell concrete system to create a new sound from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with sound
			//-------------------------------------------------------
			virtual void Create3DSound(const std::string& instrFilePath, IAudioResource* inpAudio) = 0;
			//-------------------------------------------------------
			/// Create Stream
			///
			/// Tell concrete system to stream from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with stream
			//-------------------------------------------------------
			virtual void CreateStream(const std::string& instrFilePath, IAudioResource* inpAudio) = 0;
			//-------------------------------------------------------
			/// Create 3D Stream
			///
			/// Tell concrete system to stream from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with stream
			//-------------------------------------------------------
			virtual void Create3DStream(const std::string& instrFilePath, IAudioResource* inpAudio) = 0;
			//-------------------------------------------------------
			/// Play Sound
			///
			/// @param Sound handle
			//-------------------------------------------------------
			virtual void PlaySound(CAudioComponent* inpAudioComponent) = 0;
			//-------------------------------------------------------
			/// Destroy 
			///
			/// Release the FMOD system
			//-------------------------------------------------------
			virtual void Destroy() = 0;
			//-------------------------------------------------------
			/// Create Audio Listener
			///
			/// @return Audio listener
			//-------------------------------------------------------
			virtual AudioListenerPtr CreateAudioListener() = 0;
			//-------------------------------------------------------
			/// Set Master Effect Volume
			///
			/// Set the volume for the entire audio effect system
			/// @param Normalised volume level [0, 1]
			//-------------------------------------------------------
			void SetMasterEffectVolume(f32 infVolume);
			//-------------------------------------------------------
			/// Set Master Stream Volume
			///
			/// Set the volume for the entire audio stream system
			/// @param Normalised volume level [0, 1]
			//-------------------------------------------------------
			void SetMasterStreamVolume(f32 infVolume);
			//-------------------------------------------------------
			/// Get Master Volume
			///
			/// Get the volume for the entire audio system
			/// @return Normalised volume level [0, 1]
			//-------------------------------------------------------
			f32 GetMasterEffectVolume() const;
			//-------------------------------------------------------
			/// Get Master Stream Volume
			///
			/// Get the volume for the entire audio stream system
			/// @return Normalised volume level [0, 1]
			//-------------------------------------------------------
			f32 GetMasterStreamVolume() const;
			//-------------------------------------------------------
			/// Get Audio Manager
			///
			/// @param Audio manager
			//-------------------------------------------------------
			const IAudioManager& GetAudioManager() const;
			//-------------------------------------------------------
			/// Get Audio Manager Pointer
			///
			/// @param Pointer to audio manager
			//-------------------------------------------------------
			const IAudioManager* GetAudioManagerPtr() const;
			//-------------------------------------------------------
			/// Get Master Effect Volume Changed Event
			///
			/// Exposed event that allows sound resources to 
			/// listen for this event
			/// @return Audio event
			//-------------------------------------------------------
			IEvent<AudioVolumeEventDelegate>& GetMasterEffectVolumeChangedEvent();
			//-------------------------------------------------------
			/// Get Master Stream Volume Changed Event
			///
			/// Exposed event that allows sound resources to 
			/// listen for this event
			/// @return Audio event
			//-------------------------------------------------------
			IEvent<AudioVolumeEventDelegate>& GetMasterStreamVolumeChangedEvent();
			//----------------------------------------------------
			/// Get Number Of Component Factories
			///
			/// @return Number of factories in this system
			//----------------------------------------------------
			u32 GetNumComponentFactories() const;
			//-------------------------------------------------------
			/// Get Component Factory Pointer
			///
			/// @return Audio component factory pointer
			//-------------------------------------------------------
			Core::IComponentFactory* GetComponentFactoryPtr(u32 inudwIndex);
			//-------------------------------------------------------
			/// Get Component Factory
			///
			/// @return Audio component factory
			//-------------------------------------------------------
			Core::IComponentFactory& GetComponentFactory(u32 inudwIndex);
			
		protected:
			
			CEvent0<AudioVolumeEventDelegate> mOnMasterEffectVolumeChangedEvent;
			CEvent0<AudioVolumeEventDelegate> mOnMasterStreamVolumeChangedEvent;
			
			IAudioManager* mpAudioManager;
			IAudioComponentFactory* mpAudioComponentFactory;
			
			f32 mfMasterEffectVolume;
			f32 mfMasterStreamVolume;
			f32 mfDopplerFactor;
			f32 mfRolloffFactor;
			f32 mfDistanceFactor;
		};
	}
}

#endif
