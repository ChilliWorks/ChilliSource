//
//  PlatformSystem.cpp
//  Chilli Source
//  Created by Scott Downie on 24/11/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/Core/Base/PlatformSystem.h>
#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>
#include <CSBackend/Rendering/OpenGL/Shader/GLSLShaderProvider.h>
#include <CSBackend/Rendering/OpenGL/Texture/TextureUnitSystem.h>
#include <ChilliSource/Core/Base/Application.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace CSBackend 
{
	namespace Windows
	{
		namespace
		{
			//This is global as LARGE_INTEGER is defined in windows.h. Including windows.h in 
			//PlatformSystem.h will cause compiler errors in FileSystem.h
			LARGE_INTEGER g_frequency;
		}

		CS_DEFINE_NAMEDTYPE(PlatformSystem);
		//-----------------------------------------
		//-----------------------------------------
		PlatformSystem::PlatformSystem() 
		{
			QueryPerformanceFrequency(&g_frequency);
		}
		//--------------------------------------------------
		//--------------------------------------------------
		bool PlatformSystem::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (ChilliSource::PlatformSystem::InterfaceID == in_interfaceId || PlatformSystem::InterfaceID == in_interfaceId);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(ChilliSource::Application* in_application)
		{
			in_application->CreateSystem<CSBackend::OpenGL::GLSLShaderProvider>();
			in_application->CreateSystem<CSBackend::OpenGL::TextureUnitSystem>();
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::SetPreferredFPS(u32 in_fps)
		{
			SFMLWindow::Get()->SetPreferredFPS(in_fps);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void PlatformSystem::SetVSyncEnabled(bool in_enabled)
		{
			SFMLWindow::Get()->SetVSyncEnabled(in_enabled);
		}
		//--------------------------------------------
		//--------------------------------------------
		void PlatformSystem::Quit()
		{
			SFMLWindow::Get()->Quit();
		}
		//-------------------------------------------------
		//-------------------------------------------------
		std::string PlatformSystem::GetAppVersion() const
		{
			return ""; 
		}
		//--------------------------------------------------
		//--------------------------------------------------
		u64 PlatformSystem::GetSystemTimeMS() const
		{
			LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);
			return (u64)((currentTime.QuadPart) * 1000.0 / g_frequency.QuadPart);
		}
	}
}

#endif
