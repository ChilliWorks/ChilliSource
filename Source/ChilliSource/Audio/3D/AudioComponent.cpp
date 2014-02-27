/*
 * File: AudioComponent.cpp
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/3D/AudioComponent.h>
#include <ChilliSource/Audio/Base/AudioSystem.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>
#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
	namespace Audio
	{
        DEFINE_NAMED_INTERFACE(AudioComponent);
        //---------------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//---------------------------------------------------------------------
		AudioComponent::AudioComponent(AudioSystem* inpOwningSystem) 
        : mfCurrentVolume(0.0f), mpOwningSystem(inpOwningSystem), mfCurrentFrequency(1.0f)
		{

		} 
        //----------------------------------------------------------
		/// Is A
		///
		/// Returns if it is of the type given
		/// @param Comparison Type
		/// @return Whether the class matches the comparison type
		//----------------------------------------------------------
		bool AudioComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == AudioComponent::InterfaceID);
		}
		//------------------------------------------------------
		/// Set Audio Source
		///
		/// Set the audio source for this component
		/// @param Audio resource pointer
		//------------------------------------------------------
		void AudioComponent::SetAudioSource(const AudioResourceSPtr& inpAudioSource)
		{
			//Register for master volume changed event
			if(mpAudioSource && mpAudioSource->IsStreamed())
			{
				mpOwningSystem->GetMasterStreamVolumeChangedEvent() -= Core::MakeDelegate(this, &AudioComponent::OnMasterVolumeChanged);
			}
			else if(mpAudioSource)
			{
				mpOwningSystem->GetMasterEffectVolumeChangedEvent() -= Core::MakeDelegate(this, &AudioComponent::OnMasterVolumeChanged);
			}
			
			mpAudioSource = inpAudioSource;
			
			if(mpAudioSource && mpAudioSource->IsStreamed())
			{
				mpOwningSystem->GetMasterStreamVolumeChangedEvent() += Core::MakeDelegate(this, &AudioComponent::OnMasterVolumeChanged);
			}
			else if(mpAudioSource)
			{
				mpOwningSystem->GetMasterEffectVolumeChangedEvent() += Core::MakeDelegate(this, &AudioComponent::OnMasterVolumeChanged);
			}
		}
		//------------------------------------------------------
		/// Get Audio Source
		///
		/// Get the audio source for this component
		/// @return Audio resource pointer
		//------------------------------------------------------
		AudioResourceSPtr& AudioComponent::GetAudioSource() 
		{
			return mpAudioSource;
		}
        //---------------------------------------------------------------------
        /// Get Length
        ///
        /// @return Sound length in seconds
        //---------------------------------------------------------------------
        f32 AudioComponent::GetLength() const
        {
            return mpAudioSource->GetLength();
        }
        //---------------------------------------------------------------------
        /// Get Audio Finished Event
        ///
        /// Exposed event that listeners can subscribe to for playback 
        /// complete notifications
        /// @return Audio event 
        //---------------------------------------------------------------------
        Core::IEvent<AudioEventDelegate>& AudioComponent::GetAudioFinishedEvent()
        {
            return mOnPlaybackCompleteEvent;
        }
        //---------------------------------------------------------------------
        /// On Master Volume Changed
        ///
        /// Triggered when the audio systems master volume is changed
        //---------------------------------------------------------------------
        void AudioComponent::OnMasterVolumeChanged()
        {
            SetVolume(mfCurrentVolume);
        }
        //------------------------------------------------------
		/// Update
		///
		/// @param Time between frames in seconds
		//------------------------------------------------------
		void AudioComponent::Update(f32 dt)
		{
			if(mpEntityOwner)
			{
				Core::Vector3 vCurrentPos = mpEntityOwner->GetTransform().GetWorldPosition();
				Core::Vector3 vCurrentVel = Core::Vector3::ZERO;
				Set3DLocation(vCurrentPos, vCurrentVel);
			}
		}
        //---------------------------------------------------------------------
		/// Destructor
		//---------------------------------------------------------------------
		AudioComponent::~AudioComponent() 
		{
            //Register for master volume changed event
			if(mpAudioSource && mpAudioSource->IsStreamed())
			{
				mpOwningSystem->GetMasterStreamVolumeChangedEvent() -= Core::MakeDelegate(this, &AudioComponent::OnMasterVolumeChanged);
			}
			else if(mpAudioSource)
			{
				mpOwningSystem->GetMasterEffectVolumeChangedEvent() -= Core::MakeDelegate(this, &AudioComponent::OnMasterVolumeChanged);
			}
		} 
	}
}