//
//  CkSound.cpp
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

#include <ChilliSource/Audio/CricketAudio/CkSound.h>

#include <ChilliSource/Audio/CricketAudio/CkBank.h>
#include <ChilliSource/Audio/CricketAudio/CricketAudioSystem.h>
#include <ChilliSource/Core/Base/Application.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Core/File/FileSystem.h>
#endif

#include <ck/ck.h>
#include <ck/sound.h>

namespace ChilliSource 
{
	namespace Audio
	{
        namespace
        {
            const char k_className[] = "CkSound";
            const f32 k_secondsPerMillisecond = 1.0f / 1000.0f;
        }
        
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CkSoundUPtr CkSound::CreateFromBank(const CkBankCSPtr& in_audioBank, const std::string& in_audioName)
        {
            return CkSoundUPtr(new CkSound(in_audioBank, in_audioName));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CkSoundUPtr CkSound::CreateFromStream(Core::StorageLocation in_streamStorageLocation, const std::string& in_streamFilePath)
        {
            return CkSoundUPtr(new CkSound(in_streamStorageLocation, in_streamFilePath));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CkSound::CkSound(const CkBankCSPtr& in_audioBank, const std::string& in_audioName)
        : m_audioBank(in_audioBank)
        {
            CS_ASSERT(m_audioBank != nullptr, "Cannot create " + std::string(k_className) + " with null audio bank.");
            CS_ASSERT(m_audioBank->GetLoadState() == Core::Resource::LoadState::k_loaded, "Cannot create CkSound with an audio bank that hasn't been loaded: " + m_audioBank->GetName());
            
            m_sound = ::CkSound::newBankSound(m_audioBank->GetBank(), in_audioName.c_str());
            CS_ASSERT(m_sound != nullptr, "Could not create " + std::string(k_className) + " because sound '" + in_audioName + "' doesn't exist in the bank '" + m_audioBank->GetName() + "'.");
            
            m_ckSystem = CSCore::Application::Get()->GetSystem<CricketAudioSystem>();
            CS_ASSERT(m_ckSystem != nullptr, std::string(k_className) + " requires missing system: " + CricketAudioSystem::TypeName);
            
            m_ckSystem->Register(this);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CkSound::CkSound(Core::StorageLocation in_streamStorageLocation, const std::string& in_streamFilePath)
        {
#if CS_TARGETPLATFORM_ANDROID
        	switch (in_streamStorageLocation)
			{
				case Core::StorageLocation::k_package:
				{
					m_sound = ::CkSound::newStreamSound((CSBackend::Android::FileSystem::k_packageAPKDir + in_streamFilePath).c_str());
					break;
				}
				case Core::StorageLocation::k_chilliSource:
				{
					m_sound = ::CkSound::newStreamSound((CSBackend::Android::FileSystem::k_csAPKDir + in_streamFilePath).c_str());
					break;
				}
				default:
				{
					std::string locationPath = CSCore::StringUtils::StandardiseDirectoryPath(CSCore::Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(in_streamStorageLocation));
					m_sound = ::CkSound::newStreamSound((locationPath + in_streamFilePath).c_str(), kCkPathType_FileSystem);
					break;
				}
			}
#else
            std::string locationPath = CSCore::StringUtils::StandardiseDirectoryPath(CSCore::Application::Get()->GetFileSystem()->GetAbsolutePathToStorageLocation(in_streamStorageLocation));
            m_sound = ::CkSound::newStreamSound((locationPath + in_streamFilePath).c_str(), kCkPathType_FileSystem);
#endif
            CS_ASSERT(m_sound != nullptr, "Could not create " + std::string(k_className) + " because audio stream '" + in_streamFilePath + "' doesn't exist.");
            
            m_ckSystem = CSCore::Application::Get()->GetSystem<CricketAudioSystem>();
            CS_ASSERT(m_ckSystem != nullptr, std::string(k_className) + " requires missing system: " + CricketAudioSystem::TypeName);
            
            m_ckSystem->Register(this);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 CkSound::GetVolume() const
        {
            return m_sound->getVolume();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 CkSound::GetPitchShift() const
        {
            return m_sound->getPitchShift();
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkSound::PlaybackState CkSound::GetPlaybackState() const
		{
			return m_playbackState;
		}
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 CkSound::GetLength() const
        {
            return m_sound->getLengthMs() * k_secondsPerMillisecond;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 CkSound::GetPlaybackPosition() const
        {
            return m_sound->getPlayPositionMs() * k_secondsPerMillisecond;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkSound::SetVolume(f32 in_volume)
        {
            m_sound->setVolume(in_volume);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkSound::SetPitchShift(f32 in_pitchHalfSteps)
        {
            m_sound->setPitchShift(in_pitchHalfSteps);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkSound::SetPlaybackPosition(f32 in_playbackPosition)
        {
            m_sound->setPlayPositionMs(in_playbackPosition * 1000.0f);
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSound::Play(PlaybackMode in_playbackMode, const FinishedDelegate& in_finishedDelegate)
		{
			CS_ASSERT(m_playbackState == PlaybackState::k_stopped, std::string(k_className) + " is already playing.");

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
		void CkSound::Resume()
		{
			CS_ASSERT(m_playbackState == PlaybackState::k_paused, std::string(k_className) + " cannot be resumed when it is not paused.");

			m_playbackState = PlaybackState::k_playing;
			m_sound->setPaused(false);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSound::Pause()
		{
			CS_ASSERT(m_playbackState == PlaybackState::k_playing, std::string(k_className) + " can only be paused while playing.");

			m_playbackState = PlaybackState::k_paused;
			m_sound->setPaused(true);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSound::Stop()
		{
			CS_ASSERT(m_playbackState != PlaybackState::k_stopped, std::string(k_className) + " is already stopped.");

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
		void CkSound::OnUpdate()
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
		CkSound::~CkSound()
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
