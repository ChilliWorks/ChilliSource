//
//  SoundCache.h
//  moFloTest
//
//  Created by Scott Downie on 06/06/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_AUDIO_AUDIO_PLAYER_H_
#define _MO_FLO_AUDIO_AUDIO_PLAYER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/3D/AudioComponent.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
    namespace Audio
    {
        const u32 kudwNumConcurrentSoundEffects = 10;
        
        class AudioPlayer
        {
        public:
            
            //------------------------------------------------
            /// Init
            ///
            /// Build the cache
            //------------------------------------------------
            static void Init();
            //------------------------------------------------
            /// Add Effect
            ///
            /// @param The storage location to load from
            /// @param Filename of effect
            /// @param Whether this sound should be streamed
            //------------------------------------------------
            static void AddEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffect);
            //------------------------------------------------
            /// Remove Effect
            ///
            /// @param The storage location to load from
            /// @param Filename of effect
            //------------------------------------------------
            static void RemoveEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffect);
            //------------------------------------------------
            /// Play 
            ///
            /// Begin playback of the given sound effect
            ///
            /// @param The storage location to load from
            /// @param Sound effect file
            /// @param Whether to loop or not
            /// @return ID of file provide access
            //------------------------------------------------
            static u32 PlayEffect(Core::StorageLocation ineStorageLocation, const std::string& instrEffectFile, bool inbLooping);
            //------------------------------------------------
            /// Play 
            ///
            /// Begin playback of the given music
            ///
            /// @param The storage location to load from
            /// @param Sound effect file
            /// @param Whether to loop or not
            //------------------------------------------------
            static void PlayMusic(Core::StorageLocation ineStorageLocation, const std::string& instrEffectFile, bool inbLooping);
            //------------------------------------------------
            /// Resume
            ///
            /// Resume playback of the sound effect after a
            /// pause call
            ///
            /// @param ID of sound effect returned from play
            //------------------------------------------------
            static void ResumeEffect(u32 inudwEffectID);
            //------------------------------------------------
            /// Resume
            ///
            /// Resume playback of the sound effect after a
            /// pause call
            //------------------------------------------------
            static void ResumeMusic();
            //------------------------------------------------
            /// Resume All
            ///
            /// Resume playback all currently pause effects
            //------------------------------------------------
            static void ResumeAllEffects();
            //------------------------------------------------
            /// Pause
            ///
            /// Pause sound effect with given ID returned 
            /// by Play()
            ///
            /// @param Effect ID
            //------------------------------------------------
            static void PauseEffect(u32 inudwEffectID);
            //------------------------------------------------
            /// Pause
            ///
            /// Pause sound effect with given ID returned 
            /// by Play()
            //------------------------------------------------
            static void PauseMusic();
            //------------------------------------------------
            /// Pause All
            ///
            /// Pause all active sound effects
            //------------------------------------------------
            static void PauseAllEffects();
            //------------------------------------------------
            /// Stop Music
            ///
            /// Stop playback of the sound effect with the
            /// given ID
            ///
            //------------------------------------------------
            static void StopMusic();
            //------------------------------------------------
            /// Stop
            ///
            /// Stop playback of the sound effect with the
            /// given ID
            ///
            /// @param ID returned from play
            //------------------------------------------------
            static void StopEffect(u32 inudwEffectID);
            //------------------------------------------------
            /// Stop All
            ///
            /// Stop all active sound effects
            //------------------------------------------------
            static void StopAllEffects();
            //------------------------------------------------
            /// Is Effect Playing
            ///
            /// @return True if effect is playing, false otherwise
            //------------------------------------------------
            static bool IsEffectPlaying(u32 inudwEffectID);
            //------------------------------------------------
            /// Is Music Playing
            ///
            /// @return True if music is playing, false otherwise
            //------------------------------------------------
            static bool IsMusicPlaying();
            //------------------------------------------------
            /// Get Cache Size
            ///
            /// @return The current size of the cache
            //------------------------------------------------
            static u32 GetCacheSize();
            //------------------------------------------------
            /// Clear Cache
            ///
            /// Free up the memory held by the cache
            //------------------------------------------------
            static void ClearCache();
            //------------------------------------------------
            /// Get Audio Component At Index
            ///
            /// Returns the current audio component in the cache that matches
            /// the given index or nullptr if index is invalid
            ///
            /// @param Index of audio component to return
            /// @param Audio component or empty pointer of not found
            //------------------------------------------------
            static AudioComponentSPtr GetAudioComponentAtIndex(const u32 inudwIndex);
            //------------------------------------------------
            /// Set Volume
            ///
            /// @param Volume for all cached effects
            //------------------------------------------------
            static void SetEffectVolume(f32 infVolume);
            //------------------------------------------------
            /// Set Music Volume
            ///
            /// @param Volume for all cached effects
            //------------------------------------------------
            static void SetMusicVolume(f32 infVolume);
            
        private:
            
            //------------------------------------------------
            /// Find Free Component Index
            ///
            /// @return Index of unused audio component
            //------------------------------------------------
            static u32 FindFreeComponentIndex();
            
        private:
        
            static std::vector<AudioComponentSPtr> mAudioComponentCache;
            static std::vector<u32> maudwComponentPauseCount;
            static std::vector<bool> mabComponentLooping;
            
            typedef std::unordered_map<std::string, AudioResourceSPtr> MapNameToResource;
            static MapNameToResource mmapNamesToResources;
            
            static AudioManager* mpAudioManager;
            static AudioComponentFactory* mpAudioComponentFactory;
            
            static AudioComponentSPtr mpMusicComponent;
            
            static f32 mfEffectVolume;
            
            static u32 mudwCapacity;
        };
    }
}

#endif