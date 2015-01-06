//
//  CricketAudioSystem.cpp
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

#include <ChilliSource/Audio/CricketAudio/CricketAudioSystem.h>

#include <ChilliSource/Audio/CricketAudio/CkSound.h>
#include <ChilliSource/Core/Container/VectorUtils.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Core/Base/CoreJavaInterface.h>
#include <CSBackend/Platform/Android/Core/JNI/JavaInterfaceManager.h>
#endif

#include <ck/ck.h>
#include <ck/config.h>

namespace ChilliSource
{
	namespace Audio
	{
		CS_DEFINE_NAMEDTYPE(CricketAudioSystem);
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		CricketAudioSystemUPtr CricketAudioSystem::Create()
		{
			return CricketAudioSystemUPtr(new CricketAudioSystem());
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		bool CricketAudioSystem::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (CricketAudioSystem::InterfaceID == in_interfaceId);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CricketAudioSystem::Register(CkSound* in_ckAudio)
		{
			CS_ASSERT(Core::VectorUtils::Contains(m_ckAudioList, in_ckAudio) == false, "The given CkSound is already registered with the CricketAudioSystem!");

			m_ckAudioList.push_back(in_ckAudio);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CricketAudioSystem::Deregister(CkSound* in_ckAudio)
		{
			Core::VectorUtils::Remove(m_ckAudioList, in_ckAudio);
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CricketAudioSystem::OnInit()
		{
#if CS_TARGETPLATFORM_ANDROID
			auto coreJI = CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJavaInterface<CSBackend::Android::CoreJavaInterface>();
			CS_ASSERT(coreJI != nullptr, "Must have access to the core native interface to setup Cricket Audio.");

			CkConfig config(CSBackend::Android::JavaInterfaceManager::GetSingletonPtr()->GetJNIEnvironmentPtr(), coreJI->GetActivity());
			CkInit(&config);
#else 
			CkConfig config;
			CkInit(&config);
#endif
            //Print the license info.
			CS_LOG_VERBOSE("=================");
            CS_LOG_VERBOSE("= Cricket Audio =");
            CS_LOG_VERBOSE("=================");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE("Chilli Source has obtained special permission to include Cricket Audio as");
            CS_LOG_VERBOSE("part of the engine. See the documentation on CricketAudioSystem for details ");
            CS_LOG_VERBOSE("on how to use Cricket Audio. You are free to use Cricket Audio as part of ");
            CS_LOG_VERBOSE("Chilli Source within your app subject to the Cricket Audio free license");
            CS_LOG_VERBOSE("(http://www.crickettechnology.com/free_license) as outline below:");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE("Cricket Audio free license");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE("Cricket Audio is available FREE in binary form for iOS, Android, Windows");
            CS_LOG_VERBOSE("Phone 8, OS X, Windows, and Linux.");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE("This free license has a few requirements:");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE(" * Include the following message somewhere in your app, visible to users");
            CS_LOG_VERBOSE("   (for example, on a credits screen):");
            CS_LOG_VERBOSE("                     Built with Cricket Audio");
            CS_LOG_VERBOSE("                     www.crickettechnology.com");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE(" * You may optionally include our logo (available in .svg or .png format).");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE(" * Do not distribute our software to anyone, or post it for download on any");
            CS_LOG_VERBOSE("   site; refer them to our website instead. This license does not permit");
            CS_LOG_VERBOSE("   you to distribute Cricket Audio in an SDK, library, or other software");
            CS_LOG_VERBOSE("   development product. Contact us at info@crickettechnology.com for");
            CS_LOG_VERBOSE("   additional licensing options.");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE(" * If you release an app that uses Cricket Audio, you must let Cricket");
            CS_LOG_VERBOSE("   Technology know. Cricket Technology will include a link to it on our");
            CS_LOG_VERBOSE("   customers page.");
            CS_LOG_VERBOSE(" ");
            CS_LOG_VERBOSE("If you do not want to include the credit in your app, or if you do not want");
            CS_LOG_VERBOSE("to notify us of your app's release, or if you want the source code, you can");
            CS_LOG_VERBOSE("purchase a source code license (http://www.crickettechnology.com/source_license).");
            CS_LOG_VERBOSE(" ");
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CricketAudioSystem::OnResume()
		{
			CkResume();
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CricketAudioSystem::OnUpdate(f32 in_deltaTime)
		{
			CkUpdate();

			for (auto& ckAudio : m_ckAudioList)
			{
				ckAudio->OnUpdate();
			}
		}
		//------------------------------------------------------------------------------
		//------------------------------------------------------------------------------
		void CricketAudioSystem::OnSuspend()
		{
			CkSuspend();
		}
		//------------------------------------------------------------------------------
		///-----------------------------------------------------------------------------
		void CricketAudioSystem::OnDestroy()
		{
			CkShutdown();
		}
	}
}
