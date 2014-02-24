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

#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
	namespace Audio
	{
        DEFINE_NAMED_INTERFACE(CAudioComponent);
        //---------------------------------------------------------------------
		/// Constructor
		///
		/// Default
		//---------------------------------------------------------------------
		CAudioComponent::CAudioComponent(IAudioSystem* inpOwningSystem) 
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
		bool CAudioComponent::IsA(moFlo::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CAudioComponent::InterfaceID);
		}
		//------------------------------------------------------
		/// Set Audio Source
		///
		/// Set the audio source for this component
		/// @param Audio resource pointer
		//------------------------------------------------------
		void CAudioComponent::SetAudioSource(const AudioResourcePtr& inpAudioSource)
		{
			//Register for master volume changed event
			if(mpAudioSource && mpAudioSource->IsStreamed())
			{
				mpOwningSystem->GetMasterStreamVolumeChangedEvent() -= AudioVolumeEventDelegate(this, &CAudioComponent::OnMasterVolumeChanged);
			}
			else if(mpAudioSource)
			{
				mpOwningSystem->GetMasterEffectVolumeChangedEvent() -= AudioVolumeEventDelegate(this, &CAudioComponent::OnMasterVolumeChanged);
			}
			
			mpAudioSource = inpAudioSource;
			
			if(mpAudioSource && mpAudioSource->IsStreamed())
			{
				mpOwningSystem->GetMasterStreamVolumeChangedEvent() += AudioVolumeEventDelegate(this, &CAudioComponent::OnMasterVolumeChanged);
			}
			else if(mpAudioSource)
			{
				mpOwningSystem->GetMasterEffectVolumeChangedEvent() += AudioVolumeEventDelegate(this, &CAudioComponent::OnMasterVolumeChanged);
			}
		}
		//------------------------------------------------------
		/// Get Audio Source
		///
		/// Get the audio source for this component
		/// @return Audio resource pointer
		//------------------------------------------------------
		AudioResourcePtr& CAudioComponent::GetAudioSource() 
		{
			return mpAudioSource;
		}
        //---------------------------------------------------------------------
        /// Get Length
        ///
        /// @return Sound length in seconds
        //---------------------------------------------------------------------
        f32 CAudioComponent::GetLength() const
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
        IEvent<AudioEventDelegate>& CAudioComponent::GetAudioFinishedEvent()
        {
            return mOnPlaybackCompleteEvent;
        }
        //---------------------------------------------------------------------
        /// On Master Volume Changed
        ///
        /// Triggered when the audio systems master volume is changed
        //---------------------------------------------------------------------
        void CAudioComponent::OnMasterVolumeChanged()
        {
            SetVolume(mfCurrentVolume);
        }
        //------------------------------------------------------
		/// Update
		///
		/// @param Time between frames in seconds
		//------------------------------------------------------
		void CAudioComponent::Update(f32 dt)
		{
			if(mpEntityOwner)
			{
				Core::CVector3 vCurrentPos = mpEntityOwner->Transform().GetWorldPosition();
				Core::CVector3 vCurrentVel = Core::CVector3::ZERO;
				Set3DLocation(vCurrentPos, vCurrentVel);
			}
		}
        //---------------------------------------------------------------------
		/// Destructor
		//---------------------------------------------------------------------
		CAudioComponent::~CAudioComponent() 
		{
            //Register for master volume changed event
			if(mpAudioSource && mpAudioSource->IsStreamed())
			{
				mpOwningSystem->GetMasterStreamVolumeChangedEvent() -= AudioVolumeEventDelegate(this, &CAudioComponent::OnMasterVolumeChanged);
			}
			else if(mpAudioSource)
			{
				mpOwningSystem->GetMasterEffectVolumeChangedEvent() -= AudioVolumeEventDelegate(this, &CAudioComponent::OnMasterVolumeChanged);
			}
		} 
	}
}