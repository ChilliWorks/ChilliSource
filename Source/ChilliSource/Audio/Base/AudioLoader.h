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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/ResourceProvider.h>
#include <ChilliSource/Audio/Base/AudioManager.h>
#include <ChilliSource/Audio/Base/AudioSystem.h>

namespace ChilliSource
{
	namespace Audio
	{
		class AudioLoader : public Core::IResourceProvider
		{
		public:
			
			AudioLoader(AudioSystem* inpAudioSystem);
			virtual ~AudioLoader();
			
			//-------------------------------------------------------------------------
			/// Is A
			///
			/// @param Interface to compare
			/// @return Whether the object implements the given interface
			//-------------------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------------------
			/// Can Create Resource of Kind
			///
			/// @param Type to compare
			/// @return Whether the object can create a resource of given type
			//----------------------------------------------------------------------------
			bool CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const override;
			//----------------------------------------------------------------------------
			/// Create Audio Listener
			///
			/// @return Audio listener
			//----------------------------------------------------------------------------
			virtual AudioListenerSPtr CreateAudioListener() = 0;
		protected:
			
			const AudioManager& mAudioManager;
		};
	}
}

#endif

