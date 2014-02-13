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
 
#include <moFlo/Platform/Android/FMOD/FMODAudioComponentFactory.h>
#include <moFlo/Platform/Android/FMOD/FMODAudioComponent.h>

#include <moFlo/Audio/AudioManager.h>



namespace moFlo
{
	namespace AndroidPlatform
	{

		DEFINE_NAMED_INTERFACE(CFMODAudioComponentFactory);
		
		//--------------------------------------------------------
		/// Constructor
		///
		/// @param Audio manager instance so we can create audio
		//--------------------------------------------------------
		CFMODAudioComponentFactory::CFMODAudioComponentFactory(Audio::IAudioSystem* inpAudioSystem, Audio::IAudioManager* inAudioMgr) : Audio::IAudioComponentFactory(inpAudioSystem, inAudioMgr)
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
			return inInterfaceID == Audio::IAudioComponentFactory::InterfaceID;
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
			return (Audio::CAudioComponent::InterfaceID == inTypeID) || (Audio::CAudioListenerComponent::InterfaceID == inTypeID);
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
			return (Audio::CAudioComponent::TypeName == incName) || (Audio::CAudioListenerComponent::TypeName == incName);
		}
        //--------------------------------------------------------
        /// Create Audio Component
        ///
        /// Instantiates a 2D audio component
        //--------------------------------------------------------
        Audio::AudioComponentPtr CFMODAudioComponentFactory::CreateAudioComponent()
        {
            return Audio::AudioComponentPtr(new CFMODAudioComponent(mpAudioSystem));
        }
		//--------------------------------------------------------
		/// Create Audio Component
		///
		/// Instantiates a 2D audio component
		/// @param Audio sample
		/// @param Whether the sample loops or not
		/// @return Audio Component
		//--------------------------------------------------------
		Audio::AudioComponentPtr CFMODAudioComponentFactory::CreateAudioComponent(const Audio::AudioResourcePtr& inpAudio, bool inbShouldLoop)
		{
			Audio::AudioComponentPtr pAudioSample(new CFMODAudioComponent(mpAudioSystem));
			pAudioSample->SetAudioSource(inpAudio);
			pAudioSample->SetLooping(inbShouldLoop);
			return pAudioSample;
		}
		//--------------------------------------------------------
		/// Create Audio Component
		///
		/// Instantiates a 2D audio component
		/// @param Audio file
		/// @param Whether the sample loops or not
		/// @return Audio Component
		//--------------------------------------------------------
		Audio::AudioComponentPtr CFMODAudioComponentFactory::CreateAudioComponent(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop)
		{
			//Load the audio with our audio manager
			Audio::AudioResourcePtr pAudioSample;

			if(!inbShouldStream)
			{
				pAudioSample = mpAudioManager->GetSoundFromFile(ineStorageLocation, instrAudioFilePath);
			}
			else
			{
				pAudioSample = mpAudioManager->StreamSoundFromFile(ineStorageLocation, instrAudioFilePath);
			}
			
			pAudioSample->SetStreamed(inbShouldStream);

			Audio::AudioComponentPtr pAudioComponent(new CFMODAudioComponent(mpAudioSystem));
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
		Audio::AudioComponentPtr CFMODAudioComponentFactory::Create3DAudioComponent(const Audio::AudioResourcePtr& inpAudio, bool inbShouldLoop)
		{
			Audio::AudioComponentPtr pAudioSample(new CFMODAudioComponent(mpAudioSystem));
			pAudioSample->SetAudioSource(inpAudio);
			pAudioSample->SetLooping(inbShouldLoop);
			return pAudioSample;
		}
		//--------------------------------------------------------
		/// Create 3D Audio Component
		///
		/// Instantiates a 3D audio component
		/// @param Audio file
		/// @param Whether the sample loops or not
		/// @return Audio Component
		//--------------------------------------------------------
		Audio::AudioComponentPtr CFMODAudioComponentFactory::Create3DAudioComponent(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrAudioFilePath, bool inbShouldStream, bool inbShouldLoop)
		{
			//Load the audio with our audio manager
			Audio::AudioResourcePtr pAudioSample;
			
			if(!inbShouldStream)
			{
				pAudioSample = mpAudioManager->GetSoundFromFile(ineStorageLocation, instrAudioFilePath);
			}
			else
			{
				pAudioSample = mpAudioManager->StreamSoundFromFile(ineStorageLocation, instrAudioFilePath);
			}
			
			pAudioSample->SetStreamed(inbShouldStream);

			Audio::AudioComponentPtr pAudioComponent(new CFMODAudioComponent(mpAudioSystem));
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
		Audio::AudioListenerComponentPtr CFMODAudioComponentFactory::CreateListenerComponent()
		{
			Audio::AudioListenerPtr pListener = mpAudioManager->CreateListener();
			Audio::AudioListenerComponentPtr pListenerComponent(new Audio::CAudioListenerComponent());
			pListenerComponent->SetAudioListener(pListener);
			return pListenerComponent;
		}
	}
}

