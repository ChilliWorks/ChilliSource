/*
 * File: AudioComponentFactory.cpp
 * Date: 18/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Backend/Audio/FMOD/3D/FMODAudioComponentFactory.h>
#include <ChilliSource/Backend/Audio/FMOD/3D/FMODAudioComponent.h>

#include <ChilliSource/Audio/Base/AudioManager.h>

namespace ChilliSource
{
	namespace iOS
	{
        CS_DEFINE_NAMEDTYPE(CFMODAudioComponentFactory);

		//--------------------------------------------------------
		/// Constructor
		///
		/// @param Audio manager instance so we can create audio
		//--------------------------------------------------------
		CFMODAudioComponentFactory::CFMODAudioComponentFactory(Audio::AudioSystem* inpAudioSystem, Audio::AudioManager* inpAudioMgr) 
        : Audio::AudioComponentFactory(inpAudioSystem, inpAudioMgr)
		{
			
		}
        //---------------------------------------------------------
        /// Is A
        ///
        /// @param Interface ID
        /// @return Whether the object is of that type
        //---------------------------------------------------------
		bool CFMODAudioComponentFactory::IsA(Core::InterfaceIDType inInterfaceID) const
        {
			return inInterfaceID == AudioComponentFactory::InterfaceID;
		}
		//--------------------------------------------------------
		/// Can Produce Component With Interface
		///
		/// Used to determine if this factory can produce 
		/// component of given type.
		///
		/// @param The ID of the component to create
		/// @return Whether the object can create component of ID
		//--------------------------------------------------------
		bool CFMODAudioComponentFactory::CanProduceComponentWithInterface(Core::InterfaceIDType inTypeID) const
		{
			return (Audio::AudioComponent::InterfaceID == inTypeID) || (Audio::AudioListenerComponent::InterfaceID == inTypeID);
		}
		//--------------------------------------------------------
		/// Can Produce Component With Type Name
		///
		/// Used to determine if this factory can produce 
		/// component of given name.
		///
		/// @param The name of the component to create
		/// @return Whether the object can create component 
		//--------------------------------------------------------
		bool CFMODAudioComponentFactory::CanProduceComponentWithTypeName(const std::string & incName) const
		{
			return (Audio::AudioComponent::TypeName == incName) || (Audio::AudioListenerComponent::TypeName == incName);
		}
        //--------------------------------------------------------
        /// Create Audio Component
        ///
        /// Instantiates a 2D audio component
        //--------------------------------------------------------
        Audio::AudioComponentSPtr CFMODAudioComponentFactory::CreateAudioComponent()
        {
            return Audio::AudioComponentSPtr(new CFMODAudioComponent(mpAudioSystem));
        }
		//--------------------------------------------------------
		/// Create Audio Component
		///
		/// Instantiates a 2D audio component
		/// @param Audio sample
		/// @param Whether the sample loops or not
		/// @return Audio Component
		//--------------------------------------------------------
		Audio::AudioComponentSPtr CFMODAudioComponentFactory::CreateAudioComponent(const Audio::AudioResourceSPtr& inpAudio, bool inbShouldLoop)
		{
			Audio::AudioComponentSPtr pAudioSample(new CFMODAudioComponent(mpAudioSystem));
			pAudioSample->SetAudioSource(inpAudio);
			pAudioSample->SetLooping(inbShouldLoop);
			return pAudioSample;
		}
		//--------------------------------------------------------
		/// Create Audio Component
		///
		/// Instantiates a 2D audio component
        /// @param The storage location to load from
		/// @param Audio file
		/// @param Whether the sample loops or not
		/// @return Audio Component
		//--------------------------------------------------------
		Audio::AudioComponentSPtr CFMODAudioComponentFactory::CreateAudioComponent(Core::StorageLocation ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop)
		{
			//Load the audio with our audio manager
			Audio::AudioResourceSPtr pAudioSample;
			
			if(!inbShouldStream)
			{
				pAudioSample = mpAudioManager->GetSoundFromFile(ineStorageLocation, instrAudioFilePath);
			}
			else
			{
				pAudioSample = mpAudioManager->StreamSoundFromFile(ineStorageLocation, instrAudioFilePath);
			}
			
			pAudioSample->SetStreamed(inbShouldStream);
			
			Audio::AudioComponentSPtr pAudioComponent(new CFMODAudioComponent(mpAudioSystem));
			pAudioComponent->SetAudioSource(pAudioSample);
			pAudioComponent->SetLooping(inbShouldLoop);
			return pAudioComponent;
		}
		//--------------------------------------------------------
		/// Create 3D Audio Component
		///
		/// Instantiates a 3D audio component
		/// @param Audio sample
		/// @param Whether the sample loops or not
		/// @return Audio Component
		//--------------------------------------------------------
		Audio::AudioComponentSPtr CFMODAudioComponentFactory::Create3DAudioComponent(const Audio::AudioResourceSPtr& inpAudio, bool inbShouldLoop)
		{
			Audio::AudioComponentSPtr pAudioSample(new CFMODAudioComponent(mpAudioSystem));
			pAudioSample->SetAudioSource(inpAudio);
			pAudioSample->SetLooping(inbShouldLoop);
			return pAudioSample;
		}
		//--------------------------------------------------------
		/// Create 3D Audio Component
		///
		/// Instantiates a 3D audio component
        /// @param The storage location to load from
		/// @param Audio file
		/// @param Whether the sample loops or not
		/// @return Audio Component
		//--------------------------------------------------------
		Audio::AudioComponentSPtr CFMODAudioComponentFactory::Create3DAudioComponent(Core::StorageLocation ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop)
		{
			//Load the audio with our audio manager
			Audio::AudioResourceSPtr pAudioSample;
			
			if(!inbShouldStream)
			{
				pAudioSample = mpAudioManager->GetSoundFromFile(ineStorageLocation, instrAudioFilePath);
			}
			else
			{
				pAudioSample = mpAudioManager->StreamSoundFromFile(ineStorageLocation, instrAudioFilePath);
			}
			
			pAudioSample->SetStreamed(inbShouldStream);
			
			Audio::AudioComponentSPtr pAudioComponent(new CFMODAudioComponent(mpAudioSystem));
			pAudioComponent->SetAudioSource(pAudioSample);
			pAudioComponent->SetLooping(inbShouldLoop);
			return pAudioComponent;
		}
		//--------------------------------------------------------
		/// Create Listener Component
		///
		/// Instantiates a 3D listner which is usually attached
		/// to the camera
		/// @return Listener component
		//--------------------------------------------------------
		Audio::AudioListenerComponentSPtr CFMODAudioComponentFactory::CreateListenerComponent()
		{
			Audio::AudioListenerSPtr pListener = mpAudioManager->CreateListener();
			Audio::AudioListenerComponentSPtr pListenerComponent(new Audio::AudioListenerComponent());
			pListenerComponent->SetAudioListener(pListener);
			return pListenerComponent;
		}
	}
}

