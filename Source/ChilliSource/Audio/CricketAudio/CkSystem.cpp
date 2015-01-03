//
//  CkSystem.cpp
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

#include <ChilliSource/Audio/CricketAudio/CkSystem.h>

#include <ChilliSource/Audio/CricketAudio/CkAudio.h>
#include <ChilliSource/Core/Container/VectorUtils.h>

#include <ck/ck.h>
#include <ck/config.h>

namespace ChilliSource
{
	namespace Audio
	{
		CS_DEFINE_NAMEDTYPE(CkSystem);
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CkSystemUPtr CkSystem::Create()
		{
			return CkSystemUPtr(new CkSystem());
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool CkSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (CkSystem::InterfaceID == in_interfaceId);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSystem::Register(CkAudio* in_ckAudio)
		{
			CS_ASSERT(Core::VectorUtils::Contains(m_ckAudioList, in_ckAudio) == false, "The given CkAudio is already registered with the CkSystem!");

			m_ckAudioList.push_back(in_ckAudio);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSystem::Deregister(CkAudio* in_ckAudio)
		{
			Core::VectorUtils::Remove(m_ckAudioList, in_ckAudio);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSystem::OnInit()
		{
#if CS_TARGETPLATFORM_ANDROID
			//TODO: !?
			//auto activityInfoSystem = CSCore::Application::Get()->GetSystem<BUAndroid::Common::ActivityInfoSystem>();
			//CkConfig config(CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr(), activityInfoSystem->GetActivity());
			//CkInit(&config);
#else 
			CkConfig config;
			CkInit(&config);
#endif
            //TODO: Add license info here.
			CS_LOG_VERBOSE(" ** Add Cricket Audio License here... **");
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSystem::OnResume()
		{
			CkResume();
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSystem::OnUpdate(f32 in_deltaTime)
		{
			CkUpdate();

			for (auto& ckAudio : m_ckAudioList)
			{
				ckAudio->OnUpdate();
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CkSystem::OnSuspend()
		{
			CkSuspend();
		}
		//------------------------------------------------------------------------------
		///-----------------------------------------------------------------------------
		void CkSystem::OnDestroy()
		{
			CkShutdown();
		}
	}
}
