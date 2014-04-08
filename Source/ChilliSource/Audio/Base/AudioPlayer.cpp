//
//  SoundCache.cpp
//  moFloTest
//
//  Created by Scott Downie on 06/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//

#include <ChilliSource/Audio/Base/AudioPlayer.h>
#include <ChilliSource/Audio/3D/AudioComponent.h>
#include <ChilliSource/Audio/3D/AudioComponentFactory.h>
#include <ChilliSource/Audio/Base/AudioManager.h>

#include <ChilliSource/Core/Resource/ResourceManagerDispenser.h>
#include <ChilliSource/Core/Entity/ComponentFactoryDispenser.h>

namespace ChilliSource
{
    namespace Audio
    {
        u32  AudioPlayer::mudwCapacity = kudwNumConcurrentSoundEffects;
        f32 AudioPlayer::mfEffectVolume = 1.0f;
        
        AudioManager* AudioPlayer::mpAudioManager = nullptr; 
        AudioComponentFactory* AudioPlayer::mpAudioComponentFactory = nullptr;
        
        std::vector<AudioComponentSPtr> AudioPlayer::mAudioComponentCache;
        std::vector<u32> AudioPlayer::maudwComponentPauseCount;
        std::vector<bool> AudioPlayer::mabComponentLooping;
        
        AudioComponentSPtr AudioPlayer::mpMusicComponent;
        
        AudioPlayer::MapNameToResource AudioPlayer::mmapNamesToResources;
        
        //------------------------------------------------
        /// Init
        //------------------------------------------------
        void AudioPlayer::Init()
        {
            mpAudioComponentFactory = (AudioComponentFactory*)Core::ComponentFactoryDispenser::GetSingletonPtr()->GetFactoryProducing(AudioComponent::InterfaceID);
            
            if(mpAudioComponentFactory == nullptr)
                return;
            
            mAudioComponentCache.reserve(mudwCapacity);

            for(u32 udwComp = 0; udwComp < mudwCapacity; ++udwComp)
            {
                mAudioComponentCache.push_back(mpAudioComponentFactory->CreateAudioComponent());
                maudwComponentPauseCount.push_back(0);
                mabComponentLooping.push_back(false);
            }
        }
        //------------------------------------------------
        /// Add Effect
        //------------------------------------------------
        void AudioPlayer::AddEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffect)
        {
            MapNameToResource::iterator pIt = mmapNamesToResources.find(instrEffect);
            
            if(pIt == mmapNamesToResources.end())
            {
                if(!mpAudioManager)
                {
                    mpAudioManager = (AudioManager*)Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(AudioResource::InterfaceID);
                }
                
                AudioResourceSPtr pResource = mpAudioManager->GetSoundFromFile(ineStorageLocation, instrEffect);
                if(pResource)
                {
                    pResource->SetStreamed(false);
                    mmapNamesToResources.insert(std::make_pair(instrEffect, pResource));	
                }
            }
        }
        //------------------------------------------------
        /// Remove Effect
        //------------------------------------------------
        void AudioPlayer::RemoveEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffect)
        {
            MapNameToResource::iterator pIt = mmapNamesToResources.find(instrEffect);
            
            if(mmapNamesToResources.end() != pIt)
            {
                if(!mpAudioManager)
                {
                    mpAudioManager = (AudioManager*)Core::ResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(AudioResource::InterfaceID);
                }
                
                // Make sure our effect is stopped
                for(u32 i = 0; i < mAudioComponentCache.size(); i++)
                {
                    AudioComponentSPtr pComp = mAudioComponentCache[i];
                    if(pComp->GetAudioSource() == pIt->second)
                    {
                        StopEffect(i);
                        break;
                    }
                }
                
                mpAudioManager->Destroy(pIt->second);
                pIt->second.reset();
                mmapNamesToResources.erase(pIt);
            }
        }
        //------------------------------------------------
        /// Play Effect
        //------------------------------------------------
        u32 AudioPlayer::PlayEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffectFile, bool inbLooping)
        {
            MapNameToResource::iterator pIt = mmapNamesToResources.find(instrEffectFile);
            u32 udwIndex = 0;
            
            if(pIt != mmapNamesToResources.end())
            {
                AudioResourceSPtr pResource = pIt->second;
                udwIndex = FindFreeComponentIndex();
                AudioComponentSPtr pComp = mAudioComponentCache[udwIndex];
                pComp->SetAudioSource(pResource);
                pComp->SetLooping(inbLooping);
                pComp->Play();
                pComp->SetVolume(mfEffectVolume);
                maudwComponentPauseCount[udwIndex] = 0;
                mabComponentLooping[udwIndex] = inbLooping;
            }
            else
            {
                CS_LOG_ERROR("Sound effect: " + instrEffectFile + " not found in cache");
            }
            
            return udwIndex;
        }
        //------------------------------------------------
        /// Play Music
        //------------------------------------------------
        void AudioPlayer::PlayMusic(Core::StorageLocation ineStorageLocation, const std::string& instrEffectFile, bool inbLooping)
        {
            mpMusicComponent = mpAudioComponentFactory->CreateAudioComponent(ineStorageLocation, instrEffectFile, true, inbLooping);
            mpMusicComponent->Play();
        }
        //------------------------------------------------
        /// Resume Effect
        //------------------------------------------------
        void AudioPlayer::ResumeEffect(u32 inudwEffectID)
        {
            if(inudwEffectID < mudwCapacity)
            {
                maudwComponentPauseCount[inudwEffectID]--;
                
                if(maudwComponentPauseCount[inudwEffectID] == 0)
                {
                    mAudioComponentCache[inudwEffectID]->Play();
                }
            }
        }
        //------------------------------------------------
        /// Resume Music
        //------------------------------------------------
        void AudioPlayer::ResumeMusic()
        {
            if(mpMusicComponent)
            {
                mpMusicComponent->Play();
            }
        }
        //------------------------------------------------
        /// Resume All Effects
        //------------------------------------------------
        void AudioPlayer::ResumeAllEffects()
        {
            for(u32 udwComp = 0; udwComp < mAudioComponentCache.size(); udwComp++)
            {
                ResumeEffect(udwComp);
            }
        }
        //------------------------------------------------
        /// Pause Music
        //------------------------------------------------
        void AudioPlayer::PauseMusic()
        {
            if(mpMusicComponent)
            {
                mpMusicComponent->Pause();
            }
        }
        //------------------------------------------------
        /// Pause Effect
        //------------------------------------------------
        void AudioPlayer::PauseEffect(u32 inudwEffectID)
        {
            if(inudwEffectID < mudwCapacity)
            {
                mAudioComponentCache[inudwEffectID]->Pause();
                maudwComponentPauseCount[inudwEffectID]++;
            }
        }
        //------------------------------------------------
        /// Pause All Effects
        //------------------------------------------------
        void AudioPlayer::PauseAllEffects()
        {
            for(u32 udwComp = 0; udwComp < mAudioComponentCache.size(); udwComp++)
            {
                PauseEffect(udwComp);
            }
        }
        //------------------------------------------------
        /// Stop Effect
        //------------------------------------------------
        void AudioPlayer::StopEffect(u32 inudwEffectID)
        {
            if(inudwEffectID < mudwCapacity)
            {
                mAudioComponentCache[inudwEffectID]->Stop();
                mAudioComponentCache[inudwEffectID]->SetAudioSource(AudioResourceSPtr());
                maudwComponentPauseCount[inudwEffectID] = 0;
                mabComponentLooping[inudwEffectID] = false;
            }
        }
        //------------------------------------------------
        /// Stop Music
        //------------------------------------------------
        void AudioPlayer::StopMusic()
        {
            if(nullptr != mpMusicComponent)
            {
                mpMusicComponent->Stop();
                CS_LOG_VERBOSE("Music stopped");
            }
            else
            {
            	CS_LOG_ERROR("Music component is nullptr");
            }
        }
        //------------------------------------------------
        /// Stop All Effects
        //------------------------------------------------
        void AudioPlayer::StopAllEffects()
        {
            for(u32 udwComp = 0; udwComp < mAudioComponentCache.size(); udwComp++)
            {
                StopEffect(udwComp);
            }
        }
        //------------------------------------------------
        /// Is Effect Playing
        //------------------------------------------------
        bool AudioPlayer::IsEffectPlaying(u32 inudwEffectID)
        {
            if(inudwEffectID < mudwCapacity)
            {
                return mAudioComponentCache[inudwEffectID]->IsPlaying();
            }
            
            return false;
        }
        //------------------------------------------------
        /// Is Music Playing
        //------------------------------------------------
        bool AudioPlayer::IsMusicPlaying()
        {
            if(nullptr != mpMusicComponent)
            {
                return mpMusicComponent->IsPlaying();
            }
            
            return false;
        }
        //------------------------------------------------
        /// Get Cache Size
        //------------------------------------------------
        u32 AudioPlayer::GetCacheSize()
        {
            return mAudioComponentCache.size();
        }
        //------------------------------------------------
        /// Set Effect Volume
        //------------------------------------------------
        void AudioPlayer::SetEffectVolume(f32 infVolume)
        {
            mfEffectVolume = infVolume;
        }
        //------------------------------------------------
        /// Set Music Volume
        //------------------------------------------------
        void AudioPlayer::SetMusicVolume(f32 infVolume)
        {
            if(mpMusicComponent)
            {
                mpMusicComponent->SetVolume(infVolume);
            }
        }
        //------------------------------------------------
        /// Clear Cache
        //------------------------------------------------
        void AudioPlayer::ClearCache()
        {
            mmapNamesToResources.clear();
            
            if(mpMusicComponent && !mpMusicComponent->IsPlaying())
            {
                mpMusicComponent.reset();
            }
        }
        //------------------------------------------------
        /// Get Audio Component At Index
        //------------------------------------------------
        AudioComponentSPtr AudioPlayer::GetAudioComponentAtIndex(const u32 inudwIndex)
        {
            if(inudwIndex < mAudioComponentCache.size())
            {
                return mAudioComponentCache[inudwIndex];
            }
            
            return AudioComponentSPtr();
        }
        //------------------------------------------------
        /// Find Free Component Index
        //------------------------------------------------
        u32 AudioPlayer::FindFreeComponentIndex() 
        {
            //If there is no free audio components then it will
            //override the last audio component in the list
            for(u32 udwComp = 0; udwComp < mAudioComponentCache.size(); ++udwComp)
            {
                if((mAudioComponentCache[udwComp]->GetAudioSource() == nullptr) || (!mAudioComponentCache[udwComp]->IsPlaying() && !mabComponentLooping[udwComp]))
                {
                    return udwComp;
                }
            }
            StopEffect(mudwCapacity-1);
            return mudwCapacity - 1;
        }
    }
}
