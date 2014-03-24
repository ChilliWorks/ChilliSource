//
//  FMODSystem.h
//  Chilli Source
//
//  Created by Scott Downie on 17/11/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_BACKEND_AUDIO_FMOD_BASE_FMODSYSTEM_H_
#define _CHILLISOURCE_BACKEND_AUDIO_FMOD_BASE_FMODSYSTEM_H_

#include <ChilliSource/Audio/Base/AudioSystem.h>
#include <ChilliSource/Backend/Audio/FMOD/ForwardDeclarations.h>
#include <ChilliSource/ChilliSource.h>

#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_event.hpp>

namespace ChilliSource
{
	namespace FMOD
	{
		const u32 k_maxFMODChannels = 32;
		
		class FMODSystem final : public Audio::AudioSystem, public Core::IUpdateable
		{
		public:
            
            CS_DECLARE_NAMEDTYPE(FMODSystem);

            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface ID to test
            ///
            /// @return Whether this object is of given type
            //-------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceID) const override;
            //------------------------------------------------
            /// Called when the system is being created
            ///
            /// @author S Downie
            //------------------------------------------------
            void OnInit() override;
			//-------------------------------------------------------
			/// Play Sound
			///
			/// Tell the FMOD system to play the sound with a free
			/// channel
			///
			/// @param FMOD sound handle
			//-------------------------------------------------------
			void PlaySound(Audio::AudioComponent* in_audioComponent);
			//-------------------------------------------------------
			/// Play Event
			///
			/// Tell the FMOD system to start the sound event
			///
			/// @param name of event
			//-------------------------------------------------------
			::FMOD::Event * PlayEvent(const std::string& instrEventName);
            //-------------------------------------------------------
			/// Set the volume for the entire audio effect channel
            ///
            /// @author S Downie
            ///
			/// @param Normalised volume level [0, 1]
			//-------------------------------------------------------
            void SetMasterEffectVolume(f32 in_volume) override;
			//-------------------------------------------------------
			/// Set the volume for the entire audio stream channel
            ///
            /// @author S Downie
            ///
			/// @param Normalised volume level [0, 1]
			//-------------------------------------------------------
            void SetMasterStreamVolume(f32 in_volume) override;
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return Normalised volume level [0, 1] for effect
            /// channel
			//-------------------------------------------------------
            f32 GetMasterEffectVolume() const override;
			//-------------------------------------------------------
			/// @author S Downie
			///
			/// @return Normalised volume level [0, 1] for stream
            /// channel
			//-------------------------------------------------------
            f32 GetMasterStreamVolume() const override;
			//-------------------------------------------------------
            /// @author S Downie
            ///
			/// @param Time since last updated in seconds
			//-------------------------------------------------------
			void Update(f32 in_timeSinceLastUpdate) override;
            //-------------------------------------------------------
			/// @author A Glass
			///
			/// @return FMOD Event System
			//-------------------------------------------------------
            inline ::FMOD::EventSystem* GetFMODEventSystem()
            {
                return m_FMODEventSystem;
            }
            //-------------------------------------------------------
			/// @author S Downie
			///
			/// @return FMOD System
			//-------------------------------------------------------
            inline ::FMOD::System* GetFMODSystem()
            {
                return m_FMODSystem;
            }
            //-------------------------------------------------------
			/// @author S Downie
			///
			/// @return FMOD event project
			//-------------------------------------------------------
            inline ::FMOD::EventProject* GetFMODEventProject()
            {
                return m_FMODEventProject;
            }
            //------------------------------------------------
            /// Called when the system is being destroyed
            ///
            /// @author S Downie
            //------------------------------------------------
            void OnDestroy() override;
            //------------------------------------------------
            /// Called when the system receieves a memory
            /// warning. This will cause FMOD to free up any
            /// cached resources
            ///
            /// @author S Downie
            //------------------------------------------------
            void OnMemoryWarning() override;
            
		private:
            friend Audio::AudioSystemUPtr Audio::AudioSystem::Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            FMODSystem();
			
		private:
			
			::FMOD::System* m_FMODSystem;
			::FMOD::EventSystem* m_FMODEventSystem;
			::FMOD::EventProject* m_FMODEventProject;
		};
	}
}

#endif