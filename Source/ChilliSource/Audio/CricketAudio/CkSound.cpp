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
#include <ChilliSource/Core/File/TaggedFilePathResolver.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Main/JNI/Core/File/FileSystem.h>
#endif

#include <ck/ck.h>
#include <ck/sound.h>

namespace ChilliSource
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
    CkSoundUPtr CkSound::CreateFromStream(StorageLocation in_streamStorageLocation, const std::string& in_streamFilePath)
    {
        return CkSoundUPtr(new CkSound(in_streamStorageLocation, in_streamFilePath));
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    CkSound::CkSound(const CkBankCSPtr& in_audioBank, const std::string& in_audioName)
    : m_audioBank(in_audioBank)
    {
        CS_ASSERT(m_audioBank != nullptr, "Cannot create " + std::string(k_className) + " with null audio bank.");
        CS_ASSERT(m_audioBank->GetLoadState() == Resource::LoadState::k_loaded, "Cannot create CkSound with an audio bank that hasn't been loaded: " + m_audioBank->GetName());
        
        m_sound = ::CkSound::newBankSound(m_audioBank->GetBank(), in_audioName.c_str());
        CS_ASSERT(m_sound != nullptr, "Could not create " + std::string(k_className) + " because sound '" + in_audioName + "' doesn't exist in the bank '" + m_audioBank->GetName() + "'.");
        
        m_ckSystem = Application::Get()->GetSystem<CricketAudioSystem>();
        CS_ASSERT(m_ckSystem != nullptr, std::string(k_className) + " requires missing system: " + CricketAudioSystem::TypeName);
        
        m_ckSystem->Register(this);
    }
    //------------------------------------------------------------------------------
    //------------------------------------------------------------------------------
    CkSound::CkSound(StorageLocation in_streamStorageLocation, const std::string& in_streamFilePath)
    {
        auto fileSystem = Application::Get()->GetFileSystem();
        auto taggedFilePath = Application::Get()->GetTaggedFilePathResolver()->ResolveFilePath(in_streamStorageLocation, in_streamFilePath);

#if CS_TARGETPLATFORM_ANDROID
        if (in_streamStorageLocation == StorageLocation::k_package || in_streamStorageLocation == StorageLocation::k_chilliSource ||
        (in_streamStorageLocation == StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(taggedFilePath) == false))
        {
            auto androidFS = fileSystem->Cast<CSBackend::Android::FileSystem>();
            CS_ASSERT(androidFS != nullptr, "Could not cast to Android file system.");

            CSBackend::Android::FileSystem::ZippedFileInfo fileInfo;
            if (androidFS->TryGetZippedFileInfo(in_streamStorageLocation, in_streamFilePath, fileInfo) == true)
            {
                CS_ASSERT(fileInfo.m_isCompressed == false && fileInfo.m_size == fileInfo.m_uncompressedSize, "Cannot stream audio file '" + in_streamFilePath + "' becuase it is compressed inside Apk or Apk expansion file.");
                CS_ASSERT(fileInfo.m_size > 0, "Cannot stream zero size audio file: " + in_streamFilePath);

                m_sound = ::CkSound::newStreamSound(androidFS->GetZipFilePath().c_str(), kCkPathType_FileSystem, fileInfo.m_offset, fileInfo.m_size, in_streamFilePath.c_str());
            }
        }
        else
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(in_streamStorageLocation) + taggedFilePath;
            m_sound = ::CkSound::newStreamSound(absFilePath.c_str(), kCkPathType_FileSystem);
        }
#else
        if (in_streamStorageLocation == StorageLocation::k_DLC && fileSystem->DoesFileExistInCachedDLC(taggedFilePath) == false)
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(StorageLocation::k_package) + fileSystem->GetPackageDLCPath() + taggedFilePath;
            m_sound = ::CkSound::newStreamSound(absFilePath.c_str(), kCkPathType_FileSystem);
        }
        else
        {
            auto absFilePath = fileSystem->GetAbsolutePathToStorageLocation(in_streamStorageLocation) + taggedFilePath;
            m_sound = ::CkSound::newStreamSound(absFilePath.c_str(), kCkPathType_FileSystem);
        }
#endif

        CS_ASSERT(m_sound != nullptr, "Could not create " + std::string(k_className) + " because audio stream '" + in_streamFilePath + "' doesn't exist.");
        
        m_ckSystem = Application::Get()->GetSystem<CricketAudioSystem>();
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
