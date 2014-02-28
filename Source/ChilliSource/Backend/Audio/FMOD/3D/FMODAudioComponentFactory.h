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

#ifndef _MO_FLO_PLATFORM_IOS_FMOD_FMOD_AUDIO_COMPONENT_FACTORY_H_
#define _MO_FLO_PLATFORM_IOS_FMOD_FMOD_AUDIO_COMPONENT_FACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/3D/AudioComponentFactory.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace iOS
	{
		class CFMODAudioComponentFactory : public Audio::AudioComponentFactory
		{
		public:
			CFMODAudioComponentFactory(Audio::AudioSystem* inpAudioSystem, Audio::AudioManager* inpAudioMgr);
	
			CS_DECLARE_NAMEDTYPE(CFMODAudioComponentFactory);
			
            //-------------------------------------------------------
			/// Is A
			///
			/// @return Whether this object is of given type
			//-------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const override;
			//--------------------------------------------------------
			/// Can Produce Component With Interface
			///
			/// Used to determine if this factory can produce 
			/// component of given type.
			///
			/// @param The ID of the component to create
			/// @return Whether the object can create component of ID
			//--------------------------------------------------------
			bool CanProduceComponentWithInterface(Core::InterfaceIDType inTypeID) const override;
			//--------------------------------------------------------
			/// Can Produce Component With Type Name
			///
			/// Used to determine if this factory can produce 
			/// component of given name.
			///
			/// @param The name of the component to create
			/// @return Whether the object can create component 
			//--------------------------------------------------------
			bool CanProduceComponentWithTypeName(const std::string & incName) const override;

            //--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
			//--------------------------------------------------------
			Audio::AudioComponentSPtr CreateAudioComponent();
			//--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
			/// @param Audio sample
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			Audio::AudioComponentSPtr CreateAudioComponent(const Audio::AudioResourceSPtr& inpAudio, bool inbShouldLoop = false);
			//--------------------------------------------------------
			/// Create Audio Component
			///
			/// Instantiates a 2D audio component
            /// @param The storage location to load from
			/// @param Audio file
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			Audio::AudioComponentSPtr CreateAudioComponent(Core::StorageLocation ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop = false);
			//--------------------------------------------------------
			/// Create 3D Audio Component
			///
			/// Instantiates a 3D audio component
			/// @param Audio sample
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			Audio::AudioComponentSPtr Create3DAudioComponent(const Audio::AudioResourceSPtr& inpAudio, bool inbShouldLoop = false);
			//--------------------------------------------------------
			/// Create 3D Audio Component
			///
			/// Instantiates a 3D audio component
            /// @param The storage location to load from
			/// @param Audio file
			/// @param Whether the sample loops or not
			/// @return Audio Component
			//--------------------------------------------------------
			Audio::AudioComponentSPtr Create3DAudioComponent(Core::StorageLocation ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop = false);
			//--------------------------------------------------------
			/// Create Listener Component
			///
			/// Instantiates a 3D listner which is usually attached
			/// to the camera
			/// @return Listener component
			//--------------------------------------------------------
			Audio::AudioListenerComponentSPtr CreateListenerComponent();
		};
	}
}

#endif