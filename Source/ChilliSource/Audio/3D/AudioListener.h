/*
 * File: AudioListener.h
 * Date: 18/11/2010 2010 
 * Description: The ears of the audio system
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_AUDIO_LISTENER_H_
#define _MO_FLO_AUDIO_AUDIO_LISTENER_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Audio
	{
		class IAudioListener
		{
		public:
			virtual ~IAudioListener(){}
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
			virtual void Set3DLocation(Core::CVector3& invPos, Core::CVector3& invVel, Core::CVector3& invForward, Core::CVector3& invUp) = 0;
		};
		
		typedef SHARED_PTR<IAudioListener> AudioListenerPtr;
		typedef WEAK_PTR<IAudioListener> AudioListenerWeakPtr;
	}
}

#endif