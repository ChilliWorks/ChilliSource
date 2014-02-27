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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/Base/AudioManager.h>
#include <ChilliSource/Audio/3D/AudioComponentFactory.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Core/Math/MathUtils.h>
#include <ChilliSource/Core/Event/GenericEvent.h>

namespace ChilliSource
{
	namespace Audio
	{
		const f32 kfDefaultAudioVolume	= 0.5f;
		const f32 kfDefaultDoppler		= 1.0f;
		const f32 kfDefaultRolloff		= 1.0f;
		const f32 kfDefaultDistance		= 1.0f;
		
        typedef std::function<void()> AudioVolumeEventDelegate;
		
		class AudioSystem : public Core::System, public Core::IUpdateable, public Core::IComponentProducer
		{
		public:
			AudioSystem();
			virtual ~AudioSystem();
			
			DECLARE_NAMED_INTERFACE(AudioSystem);

			//-------------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//-------------------------------------------------------
			/// Create Sound
			///
			/// Tell concrete system to create a new sound from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with sound
			//-------------------------------------------------------
			virtual void CreateSound(const std::string& instrFilePath, AudioResource* inpAudio) = 0;
			//-------------------------------------------------------
			/// Create 3D Sound
			///
			/// Tell concrete system to create a new sound from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with sound
			//-------------------------------------------------------
			virtual void Create3DSound(const std::string& instrFilePath, AudioResource* inpAudio) = 0;
			//-------------------------------------------------------
			/// Create Stream
			///
			/// Tell concrete system to stream from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with stream
			//-------------------------------------------------------
			virtual void CreateStream(const std::string& instrFilePath, AudioResource* inpAudio) = 0;
			//-------------------------------------------------------
			/// Create 3D Stream
			///
			/// Tell concrete system to stream from the given
			/// file
			///
			/// @param File path
			/// @param Sound handle to be initialised with stream
			//-------------------------------------------------------
			virtual void Create3DStream(const std::string& instrFilePath, AudioResource* inpAudio) = 0;
			//-------------------------------------------------------
			/// Play Sound
			///
			/// @param Sound handle
			//-------------------------------------------------------
			virtual void PlaySound(AudioComponent* inpAudioComponent) = 0;
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
			virtual AudioListenerSPtr CreateAudioListener() = 0;
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
			const AudioManager& GetAudioManager() const;
			//-------------------------------------------------------
			/// Get Audio Manager Pointer
			///
			/// @param Pointer to audio manager
			//-------------------------------------------------------
			const AudioManager* GetAudioManagerPtr() const;
			//-------------------------------------------------------
			/// Get Master Effect Volume Changed Event
			///
			/// Exposed event that allows sound resources to 
			/// listen for this event
			/// @return Audio event
			//-------------------------------------------------------
			Core::IEvent<AudioVolumeEventDelegate>& GetMasterEffectVolumeChangedEvent();
			//-------------------------------------------------------
			/// Get Master Stream Volume Changed Event
			///
			/// Exposed event that allows sound resources to 
			/// listen for this event
			/// @return Audio event
			//-------------------------------------------------------
			Core::IEvent<AudioVolumeEventDelegate>& GetMasterStreamVolumeChangedEvent();
			//----------------------------------------------------
			/// Get Number Of Component Factories
			///
			/// @return Number of factories in this system
			//----------------------------------------------------
			u32 GetNumComponentFactories() const override;
			//-------------------------------------------------------
			/// Get Component Factory Pointer
			///
			/// @return Audio component factory pointer
			//-------------------------------------------------------
			Core::ComponentFactory* GetComponentFactoryPtr(u32 inudwIndex) override;
			//-------------------------------------------------------
			/// Get Component Factory
			///
			/// @return Audio component factory
			//-------------------------------------------------------
			Core::ComponentFactory& GetComponentFactory(u32 inudwIndex) override;
			
		protected:
			
			Core::CEvent0<AudioVolumeEventDelegate> mOnMasterEffectVolumeChangedEvent;
			Core::CEvent0<AudioVolumeEventDelegate> mOnMasterStreamVolumeChangedEvent;
			
			AudioManager* mpAudioManager;
			AudioComponentFactory* mpAudioComponentFactory;
			
			f32 mfMasterEffectVolume;
			f32 mfMasterStreamVolume;
			f32 mfDopplerFactor;
			f32 mfRolloffFactor;
			f32 mfDistanceFactor;
		};
	}
}

#endif
