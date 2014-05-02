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

#include <ChilliSource/Backend/Platform/Windows/Core/Base/PlatformSystem.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Shader/GLSLShaderProvider.h>
#include <ChilliSource/Backend/Rendering/OpenGL/Texture/TextureUnitSystem.h>
#include <ChilliSource/Core/Base/Application.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//As the opengl classes need to include glfw.h, they need to be included after windows.h to avoid macro redefinitions.
#include <ChilliSource/Backend/Platform/Windows/GLFW/Base/GLFWManager.h>

namespace ChilliSource 
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
			: m_isRunning(true), m_isSuspended(false), m_appStartTime(0), m_appPreviousTime(0.0)
		{
			QueryPerformanceFrequency(&g_frequency);

			GLFWManager::Create();

			//TODO: Expose the window name through app.config.
			GLFWManager::Get()->Init(960, 640, "ChilliSource");

			//Register callbacks
			GLFWManager::Get()->SetWindowFocusDelegate((GLFWwindowfocusfun)&PlatformSystem::OnWindowFocusChanged);
			GLFWManager::Get()->SetWindowCloseDelegate((GLFWwindowclosefun)&PlatformSystem::OnWindowClosed);
		}
		//--------------------------------------------------
		//--------------------------------------------------
		bool PlatformSystem::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return (Core::PlatformSystem::InterfaceID == in_interfaceId || PlatformSystem::InterfaceID == in_interfaceId);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(Core::Application* in_application)
		{
			in_application->CreateSystem<OpenGL::GLSLShaderProvider>();
			in_application->CreateSystem<OpenGL::TextureUnitSystem>();
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::Run()
		{
			Core::Application::Get()->Resume();
			Core::Application::Get()->Foreground();

			m_appStartTime = (u64)GLFWManager::Get()->GetTime();

			while (m_isRunning)
			{
				if(!m_isSuspended)
				{
					f64 appCurrentTime = GLFWManager::Get()->GetTime();

					f32 deltaTime = (f32)(appCurrentTime - m_appPreviousTime);
					u64 uddwAppRunningTime = ((u64)m_appPreviousTime - m_appStartTime);

					GLFWManager::Get()->PollEvents();
					Core::Application::Get()->Update(deltaTime, uddwAppRunningTime);

					m_appPreviousTime = appCurrentTime;
				}
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::SetMaxFPS(u32 in_fps)
		{
			GLFWManager::Get()->SetMaxFPS(in_fps);
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::SetUpdaterActive(bool in_isActive)
		{
			m_isSuspended = !in_isActive;
		}
		//--------------------------------------------
		//--------------------------------------------
		void PlatformSystem::TerminateUpdater()
		{
			m_isRunning = false;
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
		//---GLFW Delegates
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::OnWindowClosed(GLFWwindow* in_window)
		{
			Core::Application::Get()->Background();
			Core::Application::Get()->Suspend();
			Core::Application::Get()->Quit();
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::OnWindowFocusChanged(GLFWwindow* in_window, s32 in_isFocused)
		{
			if (in_isFocused)
			{
				Core::Application::Get()->Foreground();
			}
			else
			{
				Core::Application::Get()->Background();
			}
		}
		//-----------------------------------------
		//-----------------------------------------
		PlatformSystem::~PlatformSystem()
		{
			GLFWManager::Destroy();
		}
	}
}
