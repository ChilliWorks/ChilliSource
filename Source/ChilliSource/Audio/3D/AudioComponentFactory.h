/*
 * File: AudioComponentFactory.h
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#ifndef _MO_FLO_AUDIO_AUDIO_COMPONENT_FACTORY_H_
#define _MO_FLO_AUDIO_AUDIO_COMPONENT_FACTORY_H_

#include <ChilliSource/ChilliSource.h>

#include <ChilliSource/Core/Entity/ComponentFactory.h>

#include <ChilliSource/Audio/3D/AudioComponent.h>
#include <ChilliSource/Audio/3D/AudioListenerComponent.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace Audio
	{
		class AudioComponentFactory : public Core::ComponentFactory
		{
		public:
			AudioComponentFactory(AudioSystem* inpAudioSystem, AudioManager* inpAudioMgr);
            virtual ~AudioComponentFactory(){}
            
			CS_DECLARE_NAMEDTYPE(AudioComponentFactory);
            //--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
			//--------------------------------------------------------
			virtual AudioComponentSPtr CreateAudioComponent() = 0;
			//--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
			/// @param Audio sample
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			virtual AudioComponentSPtr CreateAudioComponent(const AudioResourceSPtr& inpAudio, bool inbShouldLoop = false) = 0;
			//--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
            /// @param The storage location to load from
			/// @param Audio file
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			virtual AudioComponentSPtr CreateAudioComponent(Core::StorageLocation ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop = false) = 0;
			//--------------------------------------------------------
			/// Create 3D Audio Component
			///
			/// Instantiates a 3D audio component
			/// @param Audio sample
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			virtual AudioComponentSPtr Create3DAudioComponent(const AudioResourceSPtr& inpAudio, bool inbShouldLoop = false) = 0;
			//--------------------------------------------------------
			/// Create 3D Audio Component
			///
			/// Instantiates a 3D audio component
            /// @param The storage location to load from
			/// @param Audio file
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			virtual AudioComponentSPtr Create3DAudioComponent(Core::StorageLocation ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop = false) = 0;
			//--------------------------------------------------------
			/// Create Listener Component
			///
			/// Instantiates a 3D listner which is usually attached
			/// to the camera
			/// @return Listener component
			//--------------------------------------------------------
			virtual AudioListenerComponentSPtr CreateListenerComponent() = 0;
			
		protected:
			
			AudioManager* mpAudioManager;
            AudioSystem* mpAudioSystem;
		};
	}
}

#endif