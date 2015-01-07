//
//  CkSound.h
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

#ifndef _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKAUDIO_H_
#define _CHILLISOURCE_AUDIO_CRICKETAUDIO_CKAUDIO_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Audio/CricketAudio/CkForwardDeclarations.h>

#include <functional>

namespace ChilliSource
{
	namespace Audio
	{
		//------------------------------------------------------------------------------
		/// Allows playback of a single peice of audio. This can play both audio streams
		/// and individual peices of audio in an audio bank. Audio played though this
		/// can be paused, stopped, looped and have different effects applied to it. If 
		/// this isn't needed the simpler CkAudioPlayer can be used instead.
		///
		/// Cricket Technology has kindly allows us to include the Cricket Audio SDK
		/// in the engine under the free license. For more information see the
		/// documentation for CricketAudioSystem.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class CkSound final
		{
		public:
			CS_DECLARE_NOCOPY(CkSound);
			//------------------------------------------------------------------------------
			/// An enum that describes the current playback state of the audio.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			enum class PlaybackState
			{
				k_playing,
				k_paused,
				k_stopped
			};
			//------------------------------------------------------------------------------
			/// The playback mode for the audio.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			enum class PlaybackMode
			{
				k_once,
				k_loop
			};
			//------------------------------------------------------------------------------
			/// The delegate that will be called when audio has finished playing. This
			/// is called when audio finished naturally and when manually ended by the user
			/// calling stop.
			///
			/// @author Ian Copland
			///
			/// @param A pointer to the finished audio.
			//------------------------------------------------------------------------------
			using FinishedDelegate = std::function<void(const CkSound* in_ckAudio)>;
            //------------------------------------------------------------------------------
            /// Creates a new CkSound with audio from a bank. CkSound is created in the
            /// stopped state, call Play() to begin playback.
            ///
            /// @author Ian Copland
            ///
            /// @param The audio bank. Cannot be null.
            /// @param The name of the audio within the bank. If this not in the bank the
            /// app is considered to be in an irrecoverable state and will terminate.
            ///
            /// @return The new CkSound instance.
            //------------------------------------------------------------------------------
            static CkSoundUPtr CreateFromBank(const CkBankCSPtr& in_audioBank, const std::string& in_audioName);
            //------------------------------------------------------------------------------
            /// Creates a new CkSound from an audio stream. CkSound is created in the
            /// stopped state, call Play() to begin playback. Declared private to force the
            /// use of the factory methods.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the stream.
            /// @param The file path of the stream. If the stream doesn't exist the app is
            /// considered to be in an irrecoverable state and will terminate.
            //------------------------------------------------------------------------------
            static CkSoundUPtr CreateFromStream(Core::StorageLocation in_streamStorageLocation, const std::string& in_streamFilePath);
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The current volume of the audio.
            //------------------------------------------------------------------------------
            f32 GetVolume() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The current pitch shift of the audio in half-steps.
            //------------------------------------------------------------------------------
            f32 GetPitchShift() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The current playback position of the audio in seconds.
            //------------------------------------------------------------------------------
            f32 GetPlaybackPosition() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The playback length of the audio in seconds. Note that this may
            /// return -1 for streams if the length is unknown or the length has not yet
            /// been calculated.
            //------------------------------------------------------------------------------
            f32 GetLength() const;
            //------------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return The current playback state of the audio.
            //------------------------------------------------------------------------------
            PlaybackState GetPlaybackState() const;
            //------------------------------------------------------------------------------
            /// Sets the current volume of the audio. This can be called both before the
            /// audio is playing and during.
            ///
            /// @author Ian Copland
            ///
            /// @param The volume.
            //------------------------------------------------------------------------------
            void SetVolume(f32 in_volume);
            //------------------------------------------------------------------------------
            /// Sets the pitch shift of the effect in half-steps. This also changes playback
            /// speed. This can be called both before the audio is playing and during. Note
            /// that this doesn't work for streams that are not in cks or ogg format.
            ///
            /// @author Ian Copland
            ///
            /// @param The pitch shift in half-steps.
            //------------------------------------------------------------------------------
            void SetPitchShift(f32 in_pitchHalfSteps);
            //------------------------------------------------------------------------------
            /// Sets the playback position of the audio in seconds. This can be called
            /// before the effect is playing to start at an offset, or during to skip.
            ///
            /// @author Ian Copland
            ///
            /// @param The position through the audio in seconds.
            //------------------------------------------------------------------------------
            void SetPlaybackPosition(f32 in_playbackPosition);
			//------------------------------------------------------------------------------
			/// Plays the audio from the beginning, or from the playback position if one
            /// was set. If this is called when the audio is paused or already playing the
            /// app is considered to be in an irrecoverable state and will terminate.
			///
			/// @author Ian Copland
			///
			/// @param [Optional] The playback mode. Defaults to Once.
			/// @param [Optional] The delegate that will be called when the audio finishes 
			/// playing. This is called even if the audio is manually ended by calling 
			/// Stop(). Can be, and defaults to null.
			//------------------------------------------------------------------------------
			void Play(PlaybackMode in_playbackMode = PlaybackMode::k_once, const FinishedDelegate& in_finishedDelegate = nullptr);
			//------------------------------------------------------------------------------
			/// Resumes the audio if it was paused, otherwise the app is considered to be
			/// in an irrecoverable state and will terminate.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void Resume();
			//------------------------------------------------------------------------------
			/// Pauses the audio if it is currently playing, otherwise the app is considered 
			/// to be in an irrecoverable state and will terminate.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void Pause();
            //------------------------------------------------------------------------------
            /// Stops the audio if it is currently playing or paused, otherwise the app is
            /// considered to be in an irrecoverable state and will terminate.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            void Stop();
			//------------------------------------------------------------------------------
			/// Destructor. Cleans up the underlying CkSound. All CkSound instances must
			/// be destroyed before the end of the application OnDestroy() event, otherwise
			/// Bank and Stream resource references will still be held when destroying 
			/// the resource pool.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			~CkSound();
		private:
			friend class CricketAudioSystem;
            //------------------------------------------------------------------------------
            /// Constructor. Creates this with a peice of audio from an audio bank. Audio
            /// is created in the stopped state, call Play() to begin playback. Declared
            /// private to force the use of the factory methods.
            ///
            /// @author Ian Copland
            ///
            /// @param The audio bank. Cannot be null.
            /// @param The name of the audio within the bank. If this not in the bank the
            /// app is considered to be in an irrecoverable state and will terminate.
            //------------------------------------------------------------------------------
            CkSound(const CkBankCSPtr& in_audioBank, const std::string& in_audioName);
            //------------------------------------------------------------------------------
            /// Constructor. Creates this with a peice of audio from a stream. Audio
            /// is created in the stopped state, call Play() to begin playback. Declared
            /// private to force the use of the factory methods.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the stream.
            /// @param The file path of the stream. If the stream doesn't exist the app is
            /// considered to be in an irrecoverable state and will terminate.
            //------------------------------------------------------------------------------
            CkSound(Core::StorageLocation in_streamStorageLocation, const std::string& in_streamFilePath);
			//------------------------------------------------------------------------------
			/// Updates the playback state of the CkSound based on the current state of the
			/// underlying CkSound. This should only be called by CricketAudioSystem.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void OnUpdate();

			CricketAudioSystem* m_ckSystem = nullptr;
			CkBankCSPtr m_audioBank;
			::CkSound* m_sound = nullptr;

			PlaybackState m_playbackState = PlaybackState::k_stopped;
			FinishedDelegate m_finishedDelegate = nullptr;
		};
	}
}

#endif
