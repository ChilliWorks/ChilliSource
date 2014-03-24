//
//  AudioSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 17/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_AUDIO_BASE_AUDIOSYSTEM_H_
#define _CHILLISOURCE_AUDIO_BASE_AUDIOSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/Base/AudioManager.h>
#include <ChilliSource/Audio/3D/AudioComponentFactory.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/System/AppSystem.h>
#include <ChilliSource/Core/System/SystemConcepts.h>
#include <ChilliSource/Core/Math/MathUtils.h>

namespace ChilliSource
{
	namespace Audio
	{
        //-------------------------------------------------------
        /// System that controls the playing of audio via
        /// the concrete audio library implementation
        ///
        /// @author S Downie
        //-------------------------------------------------------
		class AudioSystem : public Core::AppSystem, public Core::IComponentProducer
		{
		public:

            typedef std::function<void()> VolumeEventDelegate;
            
			CS_DECLARE_NAMEDTYPE(AudioSystem);
            
            //-------------------------------------------------------
            /// Create the platform dependent backend
            ///
            /// @author S Downie
            ///
            /// @return New backend instance
            //-------------------------------------------------------
            static AudioSystemUPtr Create();
            
            //-------------------------------------------------------
            /// Virtual destructor
            ///
            /// @author S Downie
            //------------------------------------------------------
			virtual ~AudioSystem(){}

			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @param Audio Component to Play
			//-------------------------------------------------------
			virtual void PlaySound(AudioComponent* in_audioComponent) = 0;
			//-------------------------------------------------------
			/// Set the volume for the entire audio effect channel
            ///
            /// @author S Downie
            ///
			/// @param Normalised volume level [0, 1]
			//-------------------------------------------------------
            void SetMasterEffectVolume(f32 in_volume);
			//-------------------------------------------------------
			/// Set the volume for the entire audio stream channel
            ///
            /// @author S Downie
            ///
			/// @param Normalised volume level [0, 1]
			//-------------------------------------------------------
            void SetMasterStreamVolume(f32 in_volume);
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return Normalised volume level [0, 1] for effect
            /// channel
			//-------------------------------------------------------
            f32 GetMasterEffectVolume() const;
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return Normalised volume level [0, 1] for stream
            /// channel
			//-------------------------------------------------------
            f32 GetMasterStreamVolume() const;
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @param Pointer to audio manager
			//-------------------------------------------------------
			const AudioManager* GetAudioManager() const;
			//-------------------------------------------------------
            /// @author S Downie
            ///
			/// @return Audio event that is triggered on master
            /// effect volume changed
			//-------------------------------------------------------
			Core::IConnectableEvent<VolumeEventDelegate>& GetMasterEffectVolumeChangedEvent();
			//-------------------------------------------------------
            /// @author S Downie
            ///
			/// @return Audio event that is triggered on master
            /// stream volume changed
			//-------------------------------------------------------
			Core::IConnectableEvent<VolumeEventDelegate>& GetMasterStreamVolumeChangedEvent();
			//----------------------------------------------------
			/// @author S Downie
			///
			/// @return Number of factories in this system
			//----------------------------------------------------
			u32 GetNumComponentFactories() const override;
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return Audio component factory pointer
			//-------------------------------------------------------
			Core::ComponentFactory* GetComponentFactoryPtr(u32 inudwIndex) override;
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return Audio component factory
			//-------------------------------------------------------
			Core::ComponentFactory& GetComponentFactory(u32 inudwIndex) override;
            
        protected:
            
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            AudioSystem();
			
		protected:
			
			Core::Event<VolumeEventDelegate> m_masterEffectVolumeChangedEvent;
			Core::Event<VolumeEventDelegate> m_masterStreamVolumeChangedEvent;
			
			AudioManager* m_audioManager;
			AudioComponentFactory* m_audioComponentFactory;
			
			f32 m_masterEffectVolume;
			f32 m_masterStreamVolume;
			f32 m_dopplerFactor;
			f32 m_rolloffFactor;
			f32 m_distanceFactor;
		};
	}
}

#endif
