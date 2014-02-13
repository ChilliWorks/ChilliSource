/*
 * File: AudioComponent.h
 * Date: 18/11/2010 2010 
 * Description: Generic audio component from which all other audio components inherit
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */
#ifndef _MO_FLO_AUDIO_AUDIO_COMPONENT_H_
#define _MO_FLO_AUDIO_AUDIO_COMPONENT_H_

#include <ChilliSource/Core/Component.h>
#include <ChilliSource/Core/GenericEvent.h>

#include <ChilliSource/Audio/ForwardDeclarations.h>
#include <ChilliSource/Audio/AudioResource.h>

namespace moFlo
{
	namespace Audio
	{
        typedef fastdelegate::FastDelegate1<CAudioComponent*> AudioEventDelegate;
        
		class CAudioComponent : public Core::IComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(CAudioComponent);
			
			CAudioComponent(IAudioSystem* inpOwningSystem);
			virtual ~CAudioComponent();
            
            //----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
            //---------------------------------------------------------------------
			/// Play
			///
			/// Begin playback of the sound
			//---------------------------------------------------------------------
			virtual void Play() = 0;
			//---------------------------------------------------------------------
			/// Pause
			///
			/// Suspends playback of the sound
			//---------------------------------------------------------------------
			virtual void Pause() = 0;
			//---------------------------------------------------------------------
			/// Stop
			///
			/// Stop playback of the sound
			//---------------------------------------------------------------------
			virtual void Stop() = 0;
			//---------------------------------------------------------------------
			/// Set Looping
			///
			/// @param Whether to loop the sound or not
			//---------------------------------------------------------------------
			virtual void SetLooping(bool inbShouldLoop) = 0;
			//---------------------------------------------------------------------
			/// Is Playing
			///
			/// @return Whether the sound is playing or not
			//---------------------------------------------------------------------
			virtual bool IsPlaying() const = 0;
            //---------------------------------------------------------------------
			/// Set 3D Location
			///
			/// Set the position of the audio source in 3D space
			/// @param 3D position vector
			/// @param 3D velocity vector
			//---------------------------------------------------------------------
			virtual void Set3DLocation(Core::CVector3& invPosition, Core::CVector3& invVelocity) = 0;
            //---------------------------------------------------------------------
			/// Set Volume
			///
			/// @param Volume level in normalised range [0, 1]
			//---------------------------------------------------------------------
			virtual void SetVolume(f32 infVolume) = 0;
            //---------------------------------------------------------------------
            /// Set Frequency
            ///
            /// @param Frequency
            //---------------------------------------------------------------------
            virtual void SetFrequency(f32 infFreq) = 0;
            //---------------------------------------------------------------------
            /// Get Frequency
            ///
            /// @return Frequency
            //---------------------------------------------------------------------
            virtual f32 GetFrequency() const = 0;
			//---------------------------------------------------------------------
			/// Get Length
			///
			/// @return Sound length in seconds
			//---------------------------------------------------------------------
			f32 GetLength() const;
			//---------------------------------------------------------------------
			/// Get Audio Finished Event
			///
			/// Exposed event that listeners can subscribe to for playback 
			/// complete notifications
			/// @return Audio event 
			//---------------------------------------------------------------------
			IEvent<AudioEventDelegate>& GetAudioFinishedEvent();
			//---------------------------------------------------------------------
			/// On Master Volume Changed
			///
			/// Triggered when the audio systems master volume is changed
			//---------------------------------------------------------------------
			void OnMasterVolumeChanged();
			//------------------------------------------------------
			/// Set Audio Source
			///
			/// Set the audio source for this component
			/// @param Audio resource pointer
			//------------------------------------------------------
			void SetAudioSource(const AudioResourcePtr& inpAudioSource);
			//------------------------------------------------------
			/// Get Audio Source
			///
			/// Get the audio source for this component
			/// @return Audio resource pointer
			//------------------------------------------------------
			AudioResourcePtr& GetAudioSource();
            //------------------------------------------------------
			/// Update
			///
			/// @param Time between frames in seconds
			//------------------------------------------------------
			void Update(f32 dt);
			
		protected:
			
            IAudioSystem* mpOwningSystem;
            
			AudioResourcePtr mpAudioSource;
            
			f32 mfCurrentVolume;
            f32 mfCurrentFrequency;
            
			CEvent1<AudioEventDelegate> mOnPlaybackCompleteEvent;
		};
	}
}

#endif