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

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/Entity/Component.h>

#include <ChilliSource/Audio/3D/AudioListener.h>

namespace ChilliSource
{
	namespace Audio
	{
		class AudioListenerComponent : public Core::Component
		{
		public:
			CS_DECLARE_NAMEDTYPE(AudioListenerComponent);
			
			AudioListenerComponent();
			virtual ~AudioListenerComponent();
			
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
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
			void SetAudioListener(const AudioListenerSPtr& inpAudioListener);
			//------------------------------------------------------
			/// Get Audio Listener
			///
			/// Get the audio listener for this component
			/// @return Audio resource pointer
			//------------------------------------------------------
			AudioListenerSPtr& GetAudioListener();
			
		private:
			
			AudioListenerSPtr mpAudioListener;
		};
	}
}

#endif