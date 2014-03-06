/*
 * File: FMODAudioListener.h
 * Date: 18/11/2010 2010 
 * Description: Concrete ears of the FMOD audio system
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_FMOD_FMOD_AUDIO_LISTENER_H_
#define _MO_FLO_AUDIO_FMOD_FMOD_AUDIO_LISTENER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/3D/AudioListener.h>
#include <ChilliSource/Backend/Audio/FMOD/ForwardDeclarations.h>

#include <fmod.hpp>
#include <fmod_errors.h>

namespace ChilliSource
{
	namespace FMOD
	{
		class AudioListener : public Audio::AudioListener
		{
		public:
			AudioListener(::FMOD::System* inpFMODSystem);
			virtual ~AudioListener(){}
			
			//------------------------------------------------------------
			/// Set 3D Location
			///
			/// Tell the audio system where the listener is
			/// in world space.
			///
			/// @param 3D position vector
			/// @param 3D velocity vector
			/// @param 3D forward direction vector
			/// @param 3D up direction vector (perpendicular to forward)
			//-------------------------------------------------------------
			void Set3DLocation(Core::Vector3& invPos, Core::Vector3& invVel, Core::Vector3& invForward, Core::Vector3& invUp);
			
		private:
			
			::FMOD::System* mpFMODSystem;
		};
	}
}

#endif