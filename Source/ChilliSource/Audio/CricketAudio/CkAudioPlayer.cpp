//
//  CkAudioPlayer.cpp
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

#include <ChilliSource/Audio/CricketAudio/CkAudioPlayer.h>

#include <ChilliSource/Audio/CricketAudio/CkSound.h>
#include <ChilliSource/Audio/CricketAudio/CricketAudioSystem.h>
#include <ChilliSource/Core/Base/Application.h>

namespace ChilliSource
{
	namespace Audio
	{
        CS_DEFINE_NAMEDTYPE(CkAudioPlayer);
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        CkAudioPlayerUPtr CkAudioPlayer::Create()
        {
            return CkAudioPlayerUPtr(new CkAudioPlayer());
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        bool CkAudioPlayer::IsA(CSCore::InterfaceIDType in_interfaceId) const
        {
            return (CkAudioPlayer::InterfaceID == in_interfaceId);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 CkAudioPlayer::GetEffectVolume() const
        {
            return m_effectVolume;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::SetEffectVolume(f32 in_volume)
        {
            m_effectVolume = in_volume;
            
            for (auto& effectInfo : m_effects)
            {
                if (effectInfo.m_effect->GetPlaybackState() == CkSound::PlaybackState::k_playing)
                {
                    effectInfo.m_effect->SetVolume(m_effectVolume * effectInfo.m_volume);
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        f32 CkAudioPlayer::GetMusicVolume() const
        {
            return m_musicVolume;
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::SetMusicVolume(f32 in_volume)
        {
            m_musicVolume = in_volume;
            
            if (m_music != nullptr && m_music->GetPlaybackState() == CkSound::PlaybackState::k_playing)
            {
                m_music->SetVolume(m_musicVolume);
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::PlayEffect(const CkBankCSPtr& in_bank, const std::string& in_effectName, f32 in_volume)
        {
            EffectInfo effectInfo;
            effectInfo.m_volume = in_volume;
            effectInfo.m_effect = CkSound::CreateFromBank(in_bank, in_effectName);
            
            effectInfo.m_effect->SetVolume(m_effectVolume * effectInfo.m_volume);
            effectInfo.m_effect->Play(CkSound::PlaybackMode::k_once, [=](const CkSound* in_audio)
            {
                m_effectsToRemove.push_back(in_audio);
            });
            
            m_effects.push_back(std::move(effectInfo));
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::PlayMusic(Core::StorageLocation in_storageLocation, const std::string& in_filePath)
        {
            m_music = CkSound::CreateFromStream(in_storageLocation, in_filePath);
            m_music->SetVolume(m_musicVolume);
            m_music->Play(CkSound::PlaybackMode::k_loop);
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::StopMusic()
        {
            if (m_music != nullptr)
            {
                m_music.reset();
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::OnInit()
        {
            CS_ASSERT(Core::Application::Get()->GetSystem<CricketAudioSystem>() != nullptr, "CkAudioPlayer missing required system: CricketAudioSystem.");
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::OnResume()
        {
            for (auto& effectInfo : m_effects)
            {
                if (effectInfo.m_effect->GetPlaybackState() == CkSound::PlaybackState::k_paused)
                {
                    effectInfo.m_effect->Resume();
                }
            }
            
            if (m_music != nullptr && m_music->GetPlaybackState() == CkSound::PlaybackState::k_paused)
            {
                m_music->Resume();
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::OnUpdate(f32 in_deltaTime)
        {
            for (auto& removable : m_effectsToRemove)
            {
                for (auto it = m_effects.begin(); it != m_effects.end();)
                {
                    if (it->m_effect.get() == removable)
                    {
                        it = m_effects.erase(it);
                        break;
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
            m_effectsToRemove.clear();
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::OnSuspend()
        {
            if (m_music != nullptr && m_music->GetPlaybackState() == CkSound::PlaybackState::k_playing)
            {
                m_music->Pause();
            }
            
            for (auto& effectInfo : m_effects)
            {
                if (effectInfo.m_effect->GetPlaybackState() == CkSound::PlaybackState::k_playing)
                {
                    effectInfo.m_effect->Pause();
                }
            }
        }
        //------------------------------------------------------------------------------
        //------------------------------------------------------------------------------
        void CkAudioPlayer::OnDestroy()
        {
            m_effects.clear();
            m_effectsToRemove.clear();
        }
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkAudioPlayer::EffectInfo::EffectInfo(EffectInfo&& in_moveable)
		{
			m_volume = in_moveable.m_volume;
			m_effect = std::move(in_moveable.m_effect);

			in_moveable.m_volume = 0.0f;
			in_moveable.m_effect = nullptr;
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkAudioPlayer::EffectInfo& CkAudioPlayer::EffectInfo::operator=(CkAudioPlayer::EffectInfo&& in_moveable)
		{
			m_volume = in_moveable.m_volume;
			m_effect = std::move(in_moveable.m_effect);

			in_moveable.m_volume = 0.0f;
			in_moveable.m_effect = nullptr;

			return *this;
		}
	}
}