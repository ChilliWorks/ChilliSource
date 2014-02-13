/*
 * File: AudioLoader.h
 * Date: 16/11/2010 2010 
 * Description: Loads sound effects from file and creates resources from them
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_AUDIO_LOADER_H_
#define _MO_FLO_AUDIO_AUDIO_LOADER_H_

#include <ChilliSource/Core/ResourceProvider.h>
#include <ChilliSource/Audio/AudioManager.h>
#include <ChilliSource/Audio/AudioSystem.h>

namespace moFlo
{
	namespace Audio
	{
		class CAudioLoader : public Core::IResourceProvider
		{
		public:
			
			CAudioLoader(IAudioSystem* inpAudioSystem);
			virtual ~CAudioLoader();
			
			//-------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------------------
			/// Can Create Resource of Kind
			///
			/// @param Type to compare
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------------------------------
			/// Create Audio Listener
			///
			/// @return Audio listener
			//----------------------------------------------------------------------------
			virtual AudioListenerPtr CreateAudioListener() = 0;
		protected:
			
			const IAudioManager& mAudioManager;
		};
	}
}

#endif

