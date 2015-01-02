//
//  CkAudio.cpp
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

#include <ChilliSource/Audio/CricketAudio/CkAudio.h>

#include <ChilliSource/Audio/CricketAudio/CkAudioBank.h>
#include <ChilliSource/Audio/CricketAudio/CkSystem.h>
#include <ChilliSource/Core/Base/Application.h>

#include <ck/ck.h>
#include <ck/sound.h>

namespace ChilliSource 
{
	namespace Audio
	{
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkAudio::CkAudio(const CkAudioBankCSPtr& in_audioBank, const std::string& in_audioName)
			: m_audioBank(in_audioBank)
		{
			CS_ASSERT(m_audioBank != nullptr, "Cannot create CkAudio with null audio bank.");
			CS_ASSERT(m_audioBank->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot create CkAudio with an audio bank that hasn't been loaded: " + m_audioBank->GetName());

			m_sound = CkSound::newBankSound(m_audioBank->GetBank(), in_audioName.c_str());
			CS_ASSERT(m_sound != nullptr, "Could not create CkAudio becuase sound '" + in_audioName + "' doesn't exist in the bank '" + m_audioBank->GetName() + "'");

			m_ckSystem = CSCore::Application::Get()->GetSystem<CkSystem>();
			CS_ASSERT(m_ckSystem != nullptr, "CkAudio requires missing system: CkSystem.");

			m_ckSystem->Register(this);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkAudio::PlaybackState CkAudio::GetPlaybackState() const
		{
			return m_playbackState;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkAudio::Play(PlaybackMode in_playbackMode, const FinishedDelegate& in_finishedDelegate)
		{
			CS_ASSERT(m_playbackState == PlaybackState::k_stopped, "CkAudio is already playing.");

			m_playbackState = PlaybackState::k_playing;
			m_finishedDelegate = in_finishedDelegate;

			switch (in_playbackMode)
			{
			case PlaybackMode::k_once:
				m_sound->setLoopCount(0);
				break;
			case PlaybackMode::k_loop:
				m_sound->setLoopCount(-1);
				break;
			default:
				CS_LOG_FATAL("Invalid playback mode.");
				break;
			}

			m_sound->play();
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkAudio::Resume()
		{
			CS_ASSERT(m_playbackState == PlaybackState::k_paused, "CkAudio cannot be resumed when it is not paused.");

			m_playbackState = PlaybackState::k_playing;
			m_sound->setPaused(false);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkAudio::Pause()
		{
			CS_ASSERT(m_playbackState == PlaybackState::k_playing, "CkAudio can only be paused while playing.");

			m_playbackState = PlaybackState::k_paused;
			m_sound->setPaused(true);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkAudio::Stop()
		{
			CS_ASSERT(m_playbackState != PlaybackState::k_stopped, "CkAudio is already stopped.");

			if (m_sound->isPlaying() == true)
			{
				m_sound->stop();
			}

			m_playbackState = PlaybackState::k_stopped;

			if (m_finishedDelegate != nullptr)
			{
				auto finishedDelegate = m_finishedDelegate;
				m_finishedDelegate = nullptr;
				finishedDelegate(this);
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkAudio::OnUpdate()
		{
			if (m_playbackState == PlaybackState::k_playing)
			{
				if (m_sound->isReady() == true && m_sound->isPlaying() == false)
				{
					Stop();
				}
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkAudio::~CkAudio()
		{
			m_ckSystem->Deregister(this);

			if (m_playbackState != PlaybackState::k_stopped)
			{
				Stop();
			}

			m_sound->destroy();
		}
	}
}