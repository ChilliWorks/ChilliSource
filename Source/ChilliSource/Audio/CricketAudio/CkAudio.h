//
//  CkAudio.h
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
		/// documentation for CkSystem.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		class CkAudio final
		{
		public:
			CS_DECLARE_NOCOPY(CkAudio);
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
			using FinishedDelegate = std::function<void(const CkAudio* in_ckAudio)>;
			//------------------------------------------------------------------------------
			/// Constructor. Creates this with a peice of audio from an audio bank. Audio
			/// is created in the stopped state, call Play() to begin playback.
			///
			/// @author Ian Copland
			///
			/// @param The audio bank. Cannot be null.
			/// @param The name of the audio within the bank. If this not in the bank the
			/// app is considered to be in an irrecoverable state and will terminate.
			//------------------------------------------------------------------------------
			CkAudio(const CkAudioBankCSPtr& in_audioBank, const std::string& in_audioName);
			//------------------------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The current playback state of the audio.
			//------------------------------------------------------------------------------
			PlaybackState GetPlaybackState() const;
			//------------------------------------------------------------------------------
			/// Plays the audio from the beginning. If this is called when the audio is 
			/// paused or already playing the app is considered to be in an irrecoverable
			/// state and will terminate.
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
			/// Destructor. Cleans up the underlying CkSound. All CkAudio instances must
			/// be destroyed before the end of the application OnDestroy() event, otherwise
			/// Bank and Stream resource references will still be held when destroying 
			/// the resource pool.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			~CkAudio();
		private:
			friend class CkSystem;
			//------------------------------------------------------------------------------
			/// Updates the playback state of the CkAudio based on the current state of the
			/// underlying CkSound. This should only be called by CkSystem.
			///
			/// @author Ian Copland
			//------------------------------------------------------------------------------
			void OnUpdate();

			CkSystem* m_ckSystem = nullptr;
			CkAudioBankCSPtr m_audioBank;
			CkSound* m_sound = nullptr;

			PlaybackState m_playbackState = PlaybackState::k_stopped;
			FinishedDelegate m_finishedDelegate = nullptr;
		};
	}
}

#endif
