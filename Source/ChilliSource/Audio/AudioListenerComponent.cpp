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

#include <ChilliSource/Audio/AudioListenerComponent.h>

#include <ChilliSource/Core/Entity.h>

namespace moFlo
{
	namespace Audio
	{
        DEFINE_NAMED_INTERFACE(CAudioListenerComponent);
		//-------------------------------------------------------
		/// Constructor
		///
		/// Default
		//-------------------------------------------------------
		CAudioListenerComponent::CAudioListenerComponent()
		{
		}
		//----------------------------------------------------------
		/// Is A
		///
		/// Returns if it is of the type given
		/// @param Comparison Type
		/// @return Whether the class matches the comparison type
		//----------------------------------------------------------
		bool CAudioListenerComponent::IsA(moFlo::Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == CAudioListenerComponent::InterfaceID);
		}
		//------------------------------------------------------
		/// Set Audio Listener
		///
		/// Set the audio listener for this component
		/// @param Audio resource pointer
		//------------------------------------------------------
		void CAudioListenerComponent::SetAudioListener(const AudioListenerPtr& inpAudioListener)
		{
			mpAudioListener = inpAudioListener;
		}
		//------------------------------------------------------
		/// Get Audio Listener
		///
		/// Get the audio listener for this component
		/// @return Audio resource pointer
		//------------------------------------------------------
		AudioListenerPtr& CAudioListenerComponent::GetAudioListener()
		{
			return mpAudioListener;
		}
		//------------------------------------------------------
		/// Update
		///
		/// @param Time between frames in seconds
		//------------------------------------------------------
		void CAudioListenerComponent::Update(f32 dt)
		{
			if(mpEntityOwner)
			{
				Core::CVector3 vCurrentPos = mpEntityOwner->Transform().GetWorldPosition();
				
				Core::CVector3 vCurrentVel = Core::CVector3::ZERO;
				
				Core::CVector3 vForward;
				Core::CMatrix4x4::Multiply(&Core::CVector3::Z_UNIT_POSITIVE, &mpEntityOwner->Transform().GetWorldTransform(), &vForward);
				
				Core::CVector3 vUp;
				Core::CMatrix4x4::Multiply(&Core::CVector3::Y_UNIT_POSITIVE, &mpEntityOwner->Transform().GetWorldTransform(), &vUp);
				
				mpAudioListener->Set3DLocation(vCurrentPos, vCurrentVel, vForward, vUp);
			}
		}
		//-------------------------------------------------------
		/// Destructor
		///
		//-------------------------------------------------------
		CAudioListenerComponent::~CAudioListenerComponent()
		{
		}
	}
}