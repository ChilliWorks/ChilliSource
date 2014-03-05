/*
 * File: FMODAudioListener.cpp
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Backend/Audio/FMOD/3D/AudioListener.h>

namespace ChilliSource
{
	namespace FMOD
	{
		//------------------------------------------------------------
		/// Constructor
		///
		/// @param FMOD system
		//------------------------------------------------------------
		AudioListener::AudioListener(::FMOD::System* inpFMODSystem) : mpFMODSystem(inpFMODSystem)
		{
		}
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
		void AudioListener::Set3DLocation(Core::Vector3& invPos, Core::Vector3& invVel, Core::Vector3& invForward, Core::Vector3& invUp)
		{
			mpFMODSystem->set3DListenerAttributes(0, reinterpret_cast<FMOD_VECTOR*>(&invPos), reinterpret_cast<FMOD_VECTOR*>(&invVel), reinterpret_cast<FMOD_VECTOR*>(&invForward), reinterpret_cast<FMOD_VECTOR*>(&invUp));
		}
	}
}

