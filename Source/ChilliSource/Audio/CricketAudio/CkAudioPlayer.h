//
//  CkAudioPlayer.h
//  Chilli Source
//  Created by Ian Copland on 30/12/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKAUDIOPLAYER_H_
#define _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKAUDIOPLAYER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/StateSystem.h>

namespace ChilliSource
{
	namespace Audio
	{
        //------------------------------------------------------------------------------
        /// A state system which simplies playback of basic audio using the Cricket
        /// audio system. This allows playback of throw-away sound effects and a single
        /// music track. Effects can be played and forgotten about since the system
        /// will deal with cleaning them up when finished. Music will continue to loop
        /// until the track is replaced or StopMusic() is called.
        ///
        /// Effects and music played through this system adhere to the State life cycle
        /// and will be paused while the owning state is inactive and cleaned up when
        /// the state is destroyed.
        ///
        /// Cricket Technology has kindly allows us to include the Cricket Audio SDK
        /// in the engine under the free license. For more information see the
        /// documentation for CricketAudioSystem.
        ///
        /// @author Ian Copland
        //------------------------------------------------------------------------------
        class CkAudioPlayer final : public Core::StateSystem
        {
        public:
            CS_DECLARE_NAMEDTYPE(CkAudioPlayer);
            //------------------------------------------------------------------------------
            /// Allows querying of whether or not this system implements the interface
            /// described by the given interface Id. Typically this is not called directly
            /// as the templated equivalent IsA<Interface>() is preferred.
            ///
            /// @author Ian Copland
            ///
            /// @param The interface Id.
            ///
            /// @return Whether or not the interface is implemented.
            //------------------------------------------------------------------------------
            bool IsA(CSCore::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @return The current global volume of effects.
            //------------------------------------------------------------------------------
            f32 GetEffectVolume() const;
            //------------------------------------------------------------------------------
            /// Sets the global volume of effects, affecting currently playing effects.
            /// This takes into account the volume that effects were initially
            /// played with, i.e, if the passed volume is 0.5 and the effect is played
            /// with a volume of 0.5 the actual volume the effect is played at will be 0.25.
            ///
            /// @author Nicolas Tanda
            ///
            /// @param The volume of the effects, between 0.0 and 1.0
            //------------------------------------------------------------------------------
            void SetEffectVolume(f32 in_volume);
            //------------------------------------------------------------------------------
            /// @author Nicolas Tanda
            ///
            /// @return The current volume of the music.
            //------------------------------------------------------------------------------
            f32 GetMusicVolume() const;
            //------------------------------------------------------------------------------
            /// Sets the global volume of music, also affecting currently playing music.
            ///
            /// @author Nicolas Tanda
            ///
            /// @param The volume of the music, between 0.0 and 1.0
            //------------------------------------------------------------------------------
            void SetMusicVolume(f32 in_volume);
            //------------------------------------------------------------------------------
            /// Plays the requested sound effect from the given bank once, with the given
            /// volume. The effect will be automatically cleaned up once finished. If the
            /// requested effect doesn't exist the app is considered to be in an
            /// irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The bank the sound effect is in. Cannot be null.
            /// @param The name of the sound effect.
            /// @param [Optional] The volume of the sound effect. Defaults to 1.0.
            //------------------------------------------------------------------------------
            void PlayEffect(const CkBankCSPtr& in_bank, const std::string& in_effectName, f32 in_volume = 1.0f);
            //------------------------------------------------------------------------------
            /// Plays the requested music stream. This will loop until a new music track
            /// is played or StopMusic() is called. If the requested stream doesn't exist
            /// the app is considered to be in an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the music stream.
            /// @param The file path of the music stream.
            //------------------------------------------------------------------------------
            void PlayMusic(Core::StorageLocation in_storageLocation, const std::string& in_filePath);
            //------------------------------------------------------------------------------
            /// Stops the currently playing music track. Does nothing if there is no track
            /// playing.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void StopMusic();
            
        private:
            friend class CSCore::State;
            //------------------------------------------------------------------------------
            /// A container for information on a playing effect.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            struct EffectInfo
            {
                CkSoundUPtr m_effect;
                f32 m_volume = 0.0f;

				//------------------------------------------------------------------------------
				/// Default constructor
				///
				/// @author Ian Copland
				//------------------------------------------------------------------------------
				EffectInfo() = default;
				//------------------------------------------------------------------------------
				/// Move Constructor. This is required because it is not automatically generated 
				/// in Visual C++ 12.
				///
				/// @author Ian Copland
				///
				/// @param The value to move.
				//------------------------------------------------------------------------------
				EffectInfo(EffectInfo&& in_moveable);
				//------------------------------------------------------------------------------
				/// Move operator. This is required because it is not automatically generated 
				/// in Visual C++ 12.
				///
				/// @author Ian Copland
				///
				/// @param The value to move.
				///
				/// @param A reference to this.
				//------------------------------------------------------------------------------
				EffectInfo& operator=(EffectInfo&& in_moveable);
            };
            //------------------------------------------------------------------------------
            /// A factory method for creating new instances of the system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance.
            //------------------------------------------------------------------------------
            static CkAudioPlayerUPtr Create();
            //------------------------------------------------------------------------------
            /// Default constructor. Declared private to ensure the system is created
            /// through State::CreateSystem<CkAudioPlayer>().
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            CkAudioPlayer() = default;
            //------------------------------------------------------------------------------
            /// Initialises the audio player.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnInit() override;
            //------------------------------------------------------------------------------
            /// Resumes any previously playing effects or music.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnResume() override;
            //------------------------------------------------------------------------------
            /// Cleans up any finished sound effects.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnUpdate(f32 in_deltaTime) override;
            //------------------------------------------------------------------------------
            /// Pauses any currently playing music and effects.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnSuspend() override;
            //------------------------------------------------------------------------------
            /// Cleans up any music and effects.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void OnDestroy() override;
            
            std::vector<EffectInfo> m_effects;
            std::vector<const CkSound*> m_effectsToRemove;
            CkSoundUPtr m_music;
            f32 m_musicVolume = 1.0f;
            f32 m_effectVolume = 1.0f;
        };
	}
}

#endif
