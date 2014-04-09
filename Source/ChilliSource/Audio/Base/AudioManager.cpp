/*
 * File: AudioManager.cpp
 * Date: 16/11/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Audio/Base/AudioManager.h>
#include <ChilliSource/Audio/Base/AudioLoader.h>

#include <ChilliSource/Core/Resource/ResourceProviderOld.h>

namespace ChilliSource
{
	namespace Audio
	{
		CS_DEFINE_NAMEDTYPE(AudioManager);
		//----------------------------------------------------------------
		/// Is A
		///
		/// Query the interface type
		/// @param The interface to compare
		/// @return Whether the object implements that interface
		//----------------------------------------------------------------
		bool AudioManager::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == AudioManager::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Resource Type
		///
		/// @return The type of resource this manager handles
		//----------------------------------------------------------------
		Core::InterfaceIDType AudioManager::GetResourceType() const
		{
			return AudioResource::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Manages Resource Of Type
		///
		/// @return Whether this object manages the object of type
		//----------------------------------------------------------------
		bool AudioManager::ManagesResourceOfType(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == AudioResource::InterfaceID;
		}
		//----------------------------------------------------------------
		/// Get Provider Type
		///
		/// @return The type of resource it consumes from resource provider
		//----------------------------------------------------------------
		Core::InterfaceIDType AudioManager::GetProviderType() const
		{
			return Audio::AudioResource::InterfaceID;
		}
		//-----------------------------------------------------------------
		/// Get Resource From File
		///
		/// Generic call to get the managers resource
        /// @param The storage location to load from
		/// @param File path to resource
		/// @return Generic pointer to object type
		//-----------------------------------------------------------------
		Core::ResourceOldSPtr AudioManager::GetResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string &instrFilePath)
		{
			return GetSoundFromFile(ineStorageLocation, instrFilePath);
		}
		//----------------------------------------------------------------
		/// Stream Sound From File
		///
		/// Starts streaming audio from file
        ///
        /// @param The storage location to load from
        /// @param the file path
        /// @return The audio resource pointer
		//----------------------------------------------------------------
		AudioResourceSPtr AudioManager::StreamSoundFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath)
		{
			Core::ResourceOldSPtr pResource = CreateAudioResource();
			
			for(u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++) 
			{
                if(mResourceProviderOlds[nProvider]->IsA(AudioLoader::InterfaceID) == true)
                {
                    AudioLoader* audioLoader = static_cast<AudioLoader*>(mResourceProviderOlds[nProvider]);
                    if (audioLoader->StreamResourceFromFile(ineStorageLocation, inFilePath, pResource))
                    {
                        //Add it to the cache
                        CS_LOG_VERBOSE("Streaming sound " + inFilePath);
                        AudioResourceSPtr pAudio = std::static_pointer_cast<AudioResource>(pResource);
                        pAudio->SetName(inFilePath);
                        pAudio->SetOwningResourceManager(this);
                        pAudio->SetLoaded(true);
                        return pAudio;
                    }
                }
			}
		
			//Resource not found
			CS_LOG_ERROR("Cannot find resource for sound with path " + inFilePath);
			return nullptr;
		}
		//----------------------------------------------------------------
		/// Get Sound From File
		///
		/// Creates (lazily loads) a model instance from file. If the 
		/// model it already loaded it returns a handle to the model
        ///
        /// @param The storage location to load from
        /// @param the file path
        /// @return the audio resource pointer
		//----------------------------------------------------------------
		AudioResourceSPtr AudioManager::GetSoundFromFile(Core::StorageLocation ineStorageLocation, const std::string &inFilePath)
		{
			MapStringToSoundEffectPtrItr pExistingResource = mMapFileNamesToSoundEffect.find(inFilePath);
			
			if(pExistingResource == mMapFileNamesToSoundEffect.end()) 
			{
				Core::ResourceOldSPtr pResource = CreateAudioResource();
				
				for(u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++) 
				{
					if(mResourceProviderOlds[nProvider]->CreateResourceFromFile(ineStorageLocation, inFilePath, pResource)) 
					{
						//Add it to the cache
						CS_LOG_VERBOSE("Loading sound " + inFilePath);
						
						AudioResourceSPtr pAudio = std::static_pointer_cast<AudioResource>(pResource);
						mMapFileNamesToSoundEffect.insert(std::make_pair(inFilePath, pAudio));
						pAudio->SetName(inFilePath);
						pAudio->SetOwningResourceManager(this);
						pAudio->SetLoaded(true);
						return pAudio;
					}
				}
			} 
			else 
			{
				return pExistingResource->second;
			}
			
			//Resource not found
			CS_LOG_ERROR("Cannot find resource for sound with path " + inFilePath);
			return nullptr;
		}
		//----------------------------------------------------------------
		/// Create Listener
		///
		/// @return Concrete audio listener
		//----------------------------------------------------------------
		AudioListenerUPtr AudioManager::CreateListener()
		{
			for(u32 nProvider = 0; nProvider < mResourceProviderOlds.size(); nProvider++) 
			{
				AudioListenerUPtr pListener = static_cast<AudioLoader*>(mResourceProviderOlds[nProvider])->CreateAudioListener();
				if(pListener) 
				{
					return pListener;
				}
			}
			
			return nullptr;
		}
		//-----------------------------------------------------------------
		/// Destroy Sound Effect
		///
		/// @param Handle to the sound you want to destroy
		//-----------------------------------------------------------------
		void AudioManager::Destroy(const Core::ResourceOldSPtr& inpSoundEffect)
		{
			AudioResourceSPtr pSound = std::static_pointer_cast<AudioResource>(inpSoundEffect);
			for(MapStringToSoundEffectPtrItr it = mMapFileNamesToSoundEffect.begin(); it != mMapFileNamesToSoundEffect.end(); ++it)
			{
				if(it->second == pSound)
				{
					CS_LOG_VERBOSE("Destroying sound effect " + pSound->GetName());
					mMapFileNamesToSoundEffect.erase(it);
					return;
				}
			}
			
			CS_LOG_ERROR("Destroying sound effect " + pSound->GetName());
		}
		//-----------------------------------------------------------------
		/// Destroy All Sound Effects
		///
		//-----------------------------------------------------------------
		void AudioManager::DestroyAll()
		{
			CS_LOG_VERBOSE("Clearing sound effect cache");
			for(MapStringToSoundEffectPtrItr it = mMapFileNamesToSoundEffect.begin(); it != mMapFileNamesToSoundEffect.end(); ++it)
			{
				//If we are the only person using this then kill it dead
				if(it->second.use_count() == 1)
				{
					it = mMapFileNamesToSoundEffect.erase(it);
				}
			}
		}
	}
}

