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
        u32  CAudioPlayer::mudwCapacity = kudwNumConcurrentSoundEffects;
        f32 CAudioPlayer::mfEffectVolume = 1.0f;
        
        IAudioManager* CAudioPlayer::mpAudioManager = nullptr; 
        IAudioComponentFactory* CAudioPlayer::mpAudioComponentFactory = nullptr;
        
        DYNAMIC_ARRAY<AudioComponentPtr> CAudioPlayer::mAudioComponentCache;
        DYNAMIC_ARRAY<u32> CAudioPlayer::maudwComponentPauseCount;
        DYNAMIC_ARRAY<bool> CAudioPlayer::mabComponentLooping;
        
        AudioComponentPtr CAudioPlayer::mpMusicComponent;
        
        CAudioPlayer::MapNameToResource CAudioPlayer::mmapNamesToResources;
        
        //------------------------------------------------
        /// Init
        //------------------------------------------------
        void CAudioPlayer::Init()
        {
            mAudioComponentCache.reserve(mudwCapacity);
            
            mpAudioComponentFactory = (IAudioComponentFactory*)Core::CComponentFactoryDispenser::GetSingletonPtr()->GetFactoryProducing(CAudioComponent::InterfaceID);
            
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
        void CAudioPlayer::AddEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffect)
        {
            MapNameToResource::iterator pIt = mmapNamesToResources.find(instrEffect);
            
            if(pIt == mmapNamesToResources.end())
            {
                if(!mpAudioManager)
                {
                    mpAudioManager = (IAudioManager*)Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(IAudioResource::InterfaceID);
                }
                
                AudioResourcePtr pResource = mpAudioManager->GetSoundFromFile(ineStorageLocation, instrEffect);
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
        void CAudioPlayer::RemoveEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffect)
        {
            MapNameToResource::iterator pIt = mmapNamesToResources.find(instrEffect);
            
            if(mmapNamesToResources.end() != pIt)
            {
                if(!mpAudioManager)
                {
                    mpAudioManager = (IAudioManager*)Core::CResourceManagerDispenser::GetSingletonPtr()->GetResourceManagerForType(IAudioResource::InterfaceID);
                }
                
                // Make sure our effect is stopped
                for(u32 i = 0; i < mAudioComponentCache.size(); i++)
                {
                    AudioComponentPtr pComp = mAudioComponentCache[i];
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
        u32 CAudioPlayer::PlayEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffectFile,
                                     bool inbLooping, AudioEventDelegate inAudioFinishedEvent)
        {
            MapNameToResource::iterator pIt = mmapNamesToResources.find(instrEffectFile);
            u32 udwIndex = 0;
            
            if(pIt != mmapNamesToResources.end())
            {
                AudioResourcePtr pResource = pIt->second;
                udwIndex = FindFreeComponentIndex();
                AudioComponentPtr pComp = mAudioComponentCache[udwIndex];
                pComp->SetAudioSource(pResource);
                pComp->SetLooping(inbLooping);
                pComp->Play();
                pComp->SetVolume(mfEffectVolume);
                if(nullptr != inAudioFinishedEvent)
                {
                    pComp->GetAudioFinishedEvent() += inAudioFinishedEvent;
                }
                maudwComponentPauseCount[udwIndex] = 0;
                mabComponentLooping[udwIndex] = inbLooping;
            }
            else
            {
                ERROR_LOG("Sound effect: " + instrEffectFile + " not found in cache");
            }
            
            return udwIndex;
        }
        //------------------------------------------------
        /// Play Music
        //------------------------------------------------
        void CAudioPlayer::PlayMusic(Core::StorageLocation ineStorageLocation, const std::string& instrEffectFile, bool inbLooping)
        {
            mpMusicComponent = mpAudioComponentFactory->CreateAudioComponent(ineStorageLocation, instrEffectFile, true, inbLooping);
            mpMusicComponent->Play();
        }
        //------------------------------------------------
        /// Resume Effect
        //------------------------------------------------
        void CAudioPlayer::ResumeEffect(u32 inudwEffectID)
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
        void CAudioPlayer::ResumeMusic()
        {
            if(mpMusicComponent)
            {
                mpMusicComponent->Play();
            }
        }
        //------------------------------------------------
        /// Resume All Effects
        //------------------------------------------------
        void CAudioPlayer::ResumeAllEffects()
        {
            for(u32 udwComp = 0; udwComp < mAudioComponentCache.size(); udwComp++)
            {
                ResumeEffect(udwComp);
            }
        }
        //------------------------------------------------
        /// Pause Music
        //------------------------------------------------
        void CAudioPlayer::PauseMusic()
        {
            if(mpMusicComponent)
            {
                mpMusicComponent->Pause();
            }
        }
        //------------------------------------------------
        /// Pause Effect
        //------------------------------------------------
        void CAudioPlayer::PauseEffect(u32 inudwEffectID)
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
        void CAudioPlayer::PauseAllEffects()
        {
            for(u32 udwComp = 0; udwComp < mAudioComponentCache.size(); udwComp++)
            {
                PauseEffect(udwComp);
            }
        }
        //------------------------------------------------
        /// Stop Effect
        //------------------------------------------------
        void CAudioPlayer::StopEffect(u32 inudwEffectID)
        {
            if(inudwEffectID < mudwCapacity)
            {
                mAudioComponentCache[inudwEffectID]->Stop();
                mAudioComponentCache[inudwEffectID]->SetAudioSource(AudioResourcePtr());
                mAudioComponentCache[inudwEffectID]->GetAudioFinishedEvent().RemoveAllListeners();
                maudwComponentPauseCount[inudwEffectID] = 0;
                mabComponentLooping[inudwEffectID] = false;
            }
        }
        //------------------------------------------------
        /// Stop Music
        //------------------------------------------------
        void CAudioPlayer::StopMusic()
        {
            if(nullptr != mpMusicComponent)
            {
                mpMusicComponent->Stop();
                DEBUG_LOG("Music stopped");
            }
            else
            {
            	ERROR_LOG("Music component is nullptr");
            }
        }
        //------------------------------------------------
        /// Stop All Effects
        //------------------------------------------------
        void CAudioPlayer::StopAllEffects()
        {
            for(u32 udwComp = 0; udwComp < mAudioComponentCache.size(); udwComp++)
            {
                StopEffect(udwComp);
            }
        }
        //------------------------------------------------
        /// Is Effect Playing
        //------------------------------------------------
        bool CAudioPlayer::IsEffectPlaying(u32 inudwEffectID)
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
        bool CAudioPlayer::IsMusicPlaying()
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
        u32 CAudioPlayer::GetCacheSize()
        {
            return mAudioComponentCache.size();
        }
        //------------------------------------------------
        /// Set Effect Volume
        //------------------------------------------------
        void CAudioPlayer::SetEffectVolume(f32 infVolume)
        {
            mfEffectVolume = infVolume;
        }
        //------------------------------------------------
        /// Set Music Volume
        //------------------------------------------------
        void CAudioPlayer::SetMusicVolume(f32 infVolume)
        {
            if(mpMusicComponent)
            {
                mpMusicComponent->SetVolume(infVolume);
            }
        }
        //------------------------------------------------
        /// Clear Cache
        //------------------------------------------------
        void CAudioPlayer::ClearCache()
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
        AudioComponentPtr CAudioPlayer::GetAudioComponentAtIndex(const u32 inudwIndex)
        {
            if(inudwIndex < mAudioComponentCache.size())
            {
                return mAudioComponentCache[inudwIndex];
            }
            
            return AudioComponentPtr();
        }
        //------------------------------------------------
        /// Find Free Component Index
        //------------------------------------------------
        u32 CAudioPlayer::FindFreeComponentIndex() 
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
