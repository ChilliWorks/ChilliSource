/*
 * File: AudioSystem.cpp
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/Base/AudioSystem.h>

namespace moFlo
{
	namespace Audio
	{
        DEFINE_NAMED_INTERFACE(IAudioSystem);
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		IAudioSystem::IAudioSystem() : mfMasterEffectVolume(kfDefaultAudioVolume), mfMasterStreamVolume(kfDefaultAudioVolume),
		mfDopplerFactor(kfDefaultDoppler), mfRolloffFactor(kfDefaultRolloff), mfDistanceFactor(kfDefaultDistance), mpAudioComponentFactory(NULL), mpAudioManager(NULL)
		{
			//Set the overall system volume
			SetMasterEffectVolume(kfDefaultAudioVolume);
			SetMasterStreamVolume(kfDefaultAudioVolume);
		}
		//-------------------------------------------------------
		/// Is A
		///
		/// @return Whether this object is of given type
		//-------------------------------------------------------
		bool IAudioSystem::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == Core::IUpdateable::InterfaceID || inInterfaceID == Core::IComponentProducer::InterfaceID || inInterfaceID == IAudioSystem::InterfaceID;
		}
		//-------------------------------------------------------
		/// Set Master Effect Volume
		///
		/// Set the volume for the entire audio effect system
		/// @param Normalised volume level [0, 1]
		//-------------------------------------------------------
		void IAudioSystem::SetMasterEffectVolume(f32 infVolume)
		{
			mfMasterEffectVolume = Core::CMathUtils::Clamp(infVolume, 0.0f, 1.0f);
			
			//Trigger the active sounds OnMasterVolumeChanged delegate
			mOnMasterEffectVolumeChangedEvent.Invoke();
		}
		//-------------------------------------------------------
		/// Set Master Stream Volume
		///
		/// Set the volume for the entire audio stream system
		/// @param Normalised volume level [0, 1]
		//-------------------------------------------------------
		void IAudioSystem::SetMasterStreamVolume(f32 infVolume)
		{
			mfMasterStreamVolume = Core::CMathUtils::Clamp(infVolume, 0.0f, 1.0f);
			
			//Trigger the active sounds OnMasterVolumeChanged delegate
			mOnMasterStreamVolumeChangedEvent.Invoke();
		}
		//-------------------------------------------------------
		/// Get Master Effect Volume
		///
		/// Get the volume for the entire audio effect system
		/// @return Normalised volume level [0, 1]
		//-------------------------------------------------------
		f32 IAudioSystem::GetMasterEffectVolume() const
		{
			return mfMasterEffectVolume;
		}
		//-------------------------------------------------------
		/// Get Master Stream Volume
		///
		/// Get the volume for the entire audio stream system
		/// @return Normalised volume level [0, 1]
		//-------------------------------------------------------
		f32 IAudioSystem::GetMasterStreamVolume() const
		{
			return mfMasterStreamVolume;
		}
		//-------------------------------------------------------
		/// Get Audio Manager
		///
		/// @param Audio manager
		//-------------------------------------------------------
		const IAudioManager& IAudioSystem::GetAudioManager() const
		{
			return *mpAudioManager;
		}
		//-------------------------------------------------------
		/// Get Audio Manager Pointer
		///
		/// @param Pointer to audio manager
		//-------------------------------------------------------
		const IAudioManager* IAudioSystem::GetAudioManagerPtr() const
		{
			return mpAudioManager;
		}
		//----------------------------------------------------
		/// Get Number Of Component Factories
		///
		/// @return Number of factories in this system
		//----------------------------------------------------
		u32  IAudioSystem::GetNumComponentFactories() const
		{
			return 1;
		}
		//-------------------------------------------------------
		/// Get Component Factory Pointer
		///
		/// @return Audio component factory pointer
		//-------------------------------------------------------
		Core::IComponentFactory* IAudioSystem::GetComponentFactoryPtr(u32 inudwIndex)
		{
			return mpAudioComponentFactory;
		}
		//-------------------------------------------------------
		/// Get Component Factory
		///
		/// @return Audio component factory
		//-------------------------------------------------------
		Core::IComponentFactory& IAudioSystem::GetComponentFactory(u32 inudwIndex)
		{
			return *mpAudioComponentFactory;
		}
		//-------------------------------------------------------
		/// Get Master Effect Volume Changed Event
		///
		/// Exposed event that allows sound resources to 
		/// listen for this event
		/// @return Audio event
		//-------------------------------------------------------
		IEvent<AudioVolumeEventDelegate>& IAudioSystem::GetMasterEffectVolumeChangedEvent()
		{
			return mOnMasterEffectVolumeChangedEvent;
		}
		//-------------------------------------------------------
		/// Get Master Stream Volume Changed Event
		///
		/// Exposed event that allows sound resources to 
		/// listen for this event
		/// @return Audio event
		//-------------------------------------------------------
		IEvent<AudioVolumeEventDelegate>& IAudioSystem::GetMasterStreamVolumeChangedEvent()
		{
			return mOnMasterStreamVolumeChangedEvent;
		}
		//-------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------
		IAudioSystem::~IAudioSystem()
		{
		}
	}
}