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

#include <ChilliSource/Audio/3D/AudioListener.h>
#include <ChilliSource/Backend/Audio/FMOD/Base/fmod.hpp>
#include <ChilliSource/Backend/Audio/FMOD/Base/fmod_errors.h>

namespace moFlo
{
	namespace iOSPlatform
	{
		class CFMODAudioListener : public Audio::IAudioListener
		{
		public:
			CFMODAudioListener(FMOD::System* inpFMODSystem);
			virtual ~CFMODAudioListener(){}
			
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
			void Set3DLocation(Core::CVector3& invPos, Core::CVector3& invVel, Core::CVector3& invForward, Core::CVector3& invUp);
			
		private:
			
			FMOD::System* mpFMODSystem;
		};
		
		typedef SHARED_PTR<CFMODAudioListener> FMODAudioListenerPtr;
		typedef WEAK_PTR<CFMODAudioListener> FMODAudioListenerWeakPtr;
	}
}

#endif