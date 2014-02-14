//
//  FMODAudioComponent.h
//  moFloTest
//
//  Created by Scott Downie on 12/04/2011.
//  Copyright 2011 Tag Games. All rights reserved.
//


#ifndef _MO_FLO_PLATFORM_FMOD_FMOD_AUDIO_COMPONENT_H_
#define _MO_FLO_PLATFORM_FMOD_FMOD_AUDIO_COMPONENT_H_

#include <ChilliSource/Audio/3D/AudioComponent.h>

#include <fmod.hpp>

namespace moFlo
{
    namespace iOSPlatform
    {
        class CFMODAudioComponent : public Audio::CAudioComponent
        {
        public:
            CFMODAudioComponent(Audio::IAudioSystem* inpOwningSystem);
			~CFMODAudioComponent();
            //---------------------------------------------------------------------
			/// Play
			///
			/// Begin playback of the sound
			//---------------------------------------------------------------------
			void Play();
			//---------------------------------------------------------------------
			/// Pause
			///
			/// Suspends playback of the sound
			//---------------------------------------------------------------------
			void Pause();
			//---------------------------------------------------------------------
			/// Stop
			///
			/// Stop playback of the sound
			//---------------------------------------------------------------------
			void Stop();
			//---------------------------------------------------------------------
			/// Set Looping
			///
			/// @param Whether to loop the sound or not
			//---------------------------------------------------------------------
			void SetLooping(bool inbShouldLoop);
			//---------------------------------------------------------------------
			/// Is Playing
			///
			/// @return Whether the sound is playing or not
			//---------------------------------------------------------------------
			bool IsPlaying() const;
			//---------------------------------------------------------------------
			/// Set Volume
			///
			/// @param Volume level in normalised range [0, 1]
			//---------------------------------------------------------------------
			void SetVolume(f32 infVolume);
            //---------------------------------------------------------------------
            /// Set Frequency
            ///
            /// @param Frequency
            //---------------------------------------------------------------------
            void SetFrequency(f32 infFreq);
            //---------------------------------------------------------------------
            /// Get Frequency
            ///
            /// @return Frequency
            //---------------------------------------------------------------------
            f32 GetFrequency() const;
            //---------------------------------------------------------------------
			/// Set 3D Location
			///
			/// Set the position of the audio source in 3D space
			/// @param 3D position vector
			/// @param 3D velocity vector
			//---------------------------------------------------------------------
			void Set3DLocation(Core::CVector3& invPosition, Core::CVector3& invVelocity);
            //---------------------------------------------------------------------
			/// On Playback Complete
			///
			/// Triggered when the channel has finished with us
			//---------------------------------------------------------------------
			void OnPlaybackComplete();
			//---------------------------------------------------------------------
			/// Set Channel
			///
			/// @param A handle to the channel that is playing us.
			//---------------------------------------------------------------------
			void SetChannel(FMOD::Channel* inpChannel);
            
        private:
            
			FMOD::Channel* mpFMODChannel;
            
            friend FMOD_RESULT F_CALLBACK OnChannelAudioCompleteCallback(FMOD_CHANNEL* inpChannel, FMOD_CHANNEL_CALLBACKTYPE type, void *commanddata1, void *commanddata2);
        };
    }
}

#endif