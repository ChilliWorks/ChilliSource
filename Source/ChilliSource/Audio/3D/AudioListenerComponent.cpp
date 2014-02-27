/*
 * File: AudioListenerComponent.cpp
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/3D/AudioListenerComponent.h>

#include <ChilliSource/Core/Entity/Entity.h>

namespace ChilliSource
{
	namespace Audio
	{
        DEFINE_NAMED_INTERFACE(AudioListenerComponent);
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		AudioListenerComponent::AudioListenerComponent()
		{
		}
		//----------------------------------------------------------
		/// Is A
		///
		/// Returns if it is of the type given
		/// @param Comparison Type
		/// @return Whether the class matches the comparison type
		//----------------------------------------------------------
		bool AudioListenerComponent::IsA(ChilliSource::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == AudioListenerComponent::InterfaceID);
		}
		//------------------------------------------------------
		/// Set Audio Listener
		///
		/// Set the audio listener for this component
		/// @param Audio resource pointer
		//------------------------------------------------------
		void AudioListenerComponent::SetAudioListener(const AudioListenerSPtr& inpAudioListener)
		{
			mpAudioListener = inpAudioListener;
		}
		//------------------------------------------------------
		/// Get Audio Listener
		///
		/// Get the audio listener for this component
		/// @return Audio resource pointer
		//------------------------------------------------------
		AudioListenerSPtr& AudioListenerComponent::GetAudioListener()
		{
			return mpAudioListener;
		}
		//------------------------------------------------------
		/// Update
		///
		/// @param Time between frames in seconds
		//------------------------------------------------------
		void AudioListenerComponent::Update(f32 dt)
		{
			if(mpEntityOwner)
			{
				Core::Vector3 vCurrentPos = mpEntityOwner->GetTransform().GetWorldPosition();
				
				Core::Vector3 vCurrentVel = Core::Vector3::ZERO;
				
				Core::Vector3 vForward;
				Core::Matrix4x4::Multiply(&Core::Vector3::Z_UNIT_POSITIVE, &mpEntityOwner->GetTransform().GetWorldTransform(), &vForward);
				
				Core::Vector3 vUp;
				Core::Matrix4x4::Multiply(&Core::Vector3::Y_UNIT_POSITIVE, &mpEntityOwner->GetTransform().GetWorldTransform(), &vUp);
				
				mpAudioListener->Set3DLocation(vCurrentPos, vCurrentVel, vForward, vUp);
			}
		}
		//-------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------
		AudioListenerComponent::~AudioListenerComponent()
		{
		}
	}
}