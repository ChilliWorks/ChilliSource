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

#include <moFlo/Core/ComponentFactory.h>

#include <moFlo/Audio/ForwardDeclarations.h>
#include <moFlo/Audio/AudioComponent.h>
#include <moFlo/Audio/AudioListenerComponent.h>
#include <moFlo/Core/FileIO/FileSystem.h>

namespace moFlo
{
	namespace Audio
	{
		class IAudioComponentFactory : public Core::IComponentFactory
		{
		public:
			IAudioComponentFactory(IAudioSystem* inpAudioSystem, IAudioManager* inpAudioMgr);
            virtual ~IAudioComponentFactory(){}
            
			DECLARE_NAMED_INTERFACE(IAudioComponentFactory);
            //--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
			//--------------------------------------------------------
			virtual AudioComponentPtr CreateAudioComponent() = 0;
			//--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
			/// @param Audio sample
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			virtual AudioComponentPtr CreateAudioComponent(const AudioResourcePtr& inpAudio, bool inbShouldLoop = false) = 0;
			//--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
            /// @param The storage location to load from
			/// @param Audio file
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			virtual AudioComponentPtr CreateAudioComponent(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop = false) = 0;
			//--------------------------------------------------------
			/// Create 3D Audio Component
			///
			/// Instantiates a 3D audio component
			/// @param Audio sample
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			virtual AudioComponentPtr Create3DAudioComponent(const AudioResourcePtr& inpAudio, bool inbShouldLoop = false) = 0;
			//--------------------------------------------------------
			/// Create 3D Audio Component
			///
			/// Instantiates a 3D audio component
            /// @param The storage location to load from
			/// @param Audio file
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			virtual AudioComponentPtr Create3DAudioComponent(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop = false) = 0;
			//--------------------------------------------------------
			/// Create Listener Component
			///
			/// Instantiates a 3D listner which is usually attached
			/// to the camera
			/// @return Listener component
			//--------------------------------------------------------
			virtual AudioListenerComponentPtr CreateListenerComponent() = 0;
			
		protected:
			
			IAudioManager* mpAudioManager;
            IAudioSystem* mpAudioSystem;
		};
	}
}

#endif