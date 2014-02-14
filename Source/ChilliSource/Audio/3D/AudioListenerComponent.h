/*
 * File: AudioListenerComponent.h
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_AUDIO_LISTENER_COMPONENT_H_
#define _MO_FLO_AUDIO_AUDIO_LISTENER_COMPONENT_H_

#include <ChilliSource/Core/Entity/Component.h>

#include <ChilliSource/Audio/3D/AudioListener.h>

namespace moFlo
{
	namespace Audio
	{
		class CAudioListenerComponent : public Core::IComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(CAudioListenerComponent);
			
			CAudioListenerComponent();
			virtual ~CAudioListenerComponent();
			
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//------------------------------------------------------
			/// Update
			///
			/// @param Time between frames in seconds
			//------------------------------------------------------
			void Update(f32 dt);
			//------------------------------------------------------
			/// Set Audio Listener
			///
			/// Set the audio listener for this component
			/// @param Audio resource pointer
			//------------------------------------------------------
			void SetAudioListener(const AudioListenerPtr& inpAudioListener);
			//------------------------------------------------------
			/// Get Audio Listener
			///
			/// Get the audio listener for this component
			/// @return Audio resource pointer
			//------------------------------------------------------
			AudioListenerPtr& GetAudioListener();
			
		private:
			
			AudioListenerPtr mpAudioListener;
		};
		
		typedef SHARED_PTR<CAudioListenerComponent> AudioListenerComponentPtr;
		typedef WEAK_PTR<CAudioListenerComponent> AudioListenerComponentWeakPtr;
	}
}

#endif