/*
 * File: FMODSoundEffect.h
 * Date: 16/11/2010 2010 
 * Description: Wraps an FMOD sound instance
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_FMOD_FMOD_AUDIO_RESOURCE_H_
#define _MO_FLO_AUDIO_FMOD_FMOD_AUDIO_RESOURCE_H_

#include <ChilliSource/Audio/Main/AudioResource.h>
#include <ChilliSource/Audio/ForwardDeclarations.h>

#include <ChilliSource/Platform/iOS/FMOD/fmod.hpp>
#include <ChilliSource/Platform/iOS/FMOD/fmod_errors.h>

namespace moFlo 
{
	namespace iOSPlatform
	{
		class CFMODAudioResource : public Audio::IAudioResource 
		{
		public:
            CFMODAudioResource();
            ~CFMODAudioResource();
            //----------------------------------------------------------
            /// Is A
            ///
            /// Returns if it is of the type given
            /// @param Comparison Type
            /// @return Whether the class matches the comparison type
            //----------------------------------------------------------
            bool IsA(moFlo::Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------
			/// Get Length
			///
			/// @return Sound length in seconds
			//----------------------------------------------------------
			f32 GetLength();
            //----------------------------------------------------------
            /// Set Looping
            ///
            /// @param Whether to loop the sound or not
            //----------------------------------------------------------
            void SetLooping(bool inbShouldLoop);
            
			FMOD::Sound* mpFMODSound;
		};
	}
}

#endif