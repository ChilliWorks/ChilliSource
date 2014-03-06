/*
* File: WindowsPlatformSystem.cpp
* Date: 24/11/2010 2010 
* Description: 
*/

/*
* Author: Scott Downie
* Version: v 1.0
* Copyright ©2010 Tag Games Limited - All rights reserved 
*/

#define WIN32_LEAN_AND_MEAN

#include <ChilliSource/Backend/Platform/Windows/Core/Base/PlatformSystem.h>

#include <windows.h>

//As the opengl classes need to include glfw.h, they need to be included after windows.h to avoid macro redefinitions.
#include <ChilliSource/Backend/Rendering/OpenGL/Base/GLIncludes.h>

namespace ChilliSource 
{
	//This is global as LARGE_INTEGER is defined in windows.h. Including windows.h in PlatformSystemWindows.h will cause compiler errors
	//in FileSystemWindows.h
	LARGE_INTEGER gFrequency;

	namespace Windows
	{
		//-----------------------------------------
		//-----------------------------------------
		PlatformSystem::PlatformSystem() 
			: m_isRunning(true), m_isSuspended(false), m_appStartTime(0), m_appPreviousTime(0.0)
		{
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::Init()
		{
			QueryPerformanceFrequency(&gFrequency);

			if(!glfwInit())
			{
				CS_LOG_FATAL("Cannot initialise GLFW");
			}

			//Set the window based on the retina resolution
			if(glfwOpenWindow(960, 640, 8, 8, 8, 8, 8, 0, GLFW_WINDOW) != GL_TRUE)
			{
				glfwTerminate();
				CS_LOG_FATAL("Cannot create GLFW window");
			}

			glfwSetWindowTitle("MoFlow");

			//Register callbacks
			glfwSetWindowSizeCallback((GLFWwindowsizefun)&PlatformSystem::OnWindowResized);
			glfwSetWindowCloseCallback((GLFWwindowclosefun)&PlatformSystem::OnWindowClosed);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::CreateDefaultSystems(Core::Application* in_application)
		{
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::PostCreateSystems()
		{
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::Run()
		{
			m_appStartTime = (u64)glfwGetTime();

			while (m_isRunning)
			{
				if(!m_isSuspended)
				{
					f64 appCurrentTime = glfwGetTime();

					f32 deltaTime = (f32)(appCurrentTime - m_appPreviousTime);
					u64 uddwAppRunningTime = ((u64)m_appPreviousTime - m_appStartTime);

					//Update event
					Core::Application::Get()->OnUpdate(deltaTime, uddwAppRunningTime);

					m_appPreviousTime = appCurrentTime;
				}
			}
		}
		//-----------------------------------------
		//-----------------------------------------
		void PlatformSystem::SetUpdaterActive(bool inbIsActive)
		{
			m_isSuspended = !inbIsActive;
		}
		//--------------------------------------------
		//--------------------------------------------
		void PlatformSystem::TerminateUpdater()
		{
			m_isRunning = false;
		}
		//---------------------------------------------
		//---------------------------------------------
		Core::Vector2 PlatformSystem::GetScreenDimensions() const
		{
			Core::Vector2 result;

			s32 width, height = 0;
			glfwGetWindowSize(&width, &height);

			result.x = (f32)width;
			result.y = (f32)height;

			return result;
		}
		//------------------------------------------------
		//------------------------------------------------
		std::string PlatformSystem::GetOSVersion() const
		{
			OSVERSIONINFOEX osvi;
			ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX)); osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
			GetVersionEx((OSVERSIONINFO*) &osvi);
			return std::string(Core::ToString((u32)osvi.dwMajorVersion) + "." + Core::ToString((u32)osvi.dwMinorVersion));
		}
		//------------------------------------------------
		//------------------------------------------------
		Core::Locale PlatformSystem::GetLocale() const
		{
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH]={0};

			if(GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
			{
				if(wcscmp(localeName, L"en") == 0)	return Core::Locale("en");
				if(wcscmp(localeName, L"fr") == 0)	return Core::Locale("fr");
				if(wcscmp(localeName, L"it") == 0)  return Core::Locale("it");
				if(wcscmp(localeName, L"de") == 0)  return Core::Locale("de");
				if(wcscmp(localeName, L"es") == 0)  return Core::Locale("es");
				if(wcscmp(localeName, L"jp") == 0)  return Core::Locale("jp");
			}

			//Just default to english
			return Core::Locale("en");
		}
		//------------------------------------------------
		//------------------------------------------------
		std::string PlatformSystem::GetDeviceModelName() const
		{
			return "Windows";
		}
		//------------------------------------------------
		//------------------------------------------------
		std::string PlatformSystem::GetDeviceModelTypeName() const
		{
			return "PC";
		}
		//------------------------------------------------
		//------------------------------------------------
		std::string PlatformSystem::GetDeviceManufacturerName() const
		{
			return "Microsoft";
		}
		//------------------------------------------------
		//------------------------------------------------
		Core::Locale PlatformSystem::GetLanguage() const
		{
			wchar_t localeName[LOCALE_NAME_MAX_LENGTH]={0};

			if(GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH * sizeof(wchar_t)))
			{
				if(wcscmp(localeName, L"en") == 0)	return Core::Locale("en");
				if(wcscmp(localeName, L"fr") == 0)	return Core::Locale("fr");
				if(wcscmp(localeName, L"it") == 0)  return Core::Locale("it");
				if(wcscmp(localeName, L"de") == 0)  return Core::Locale("de");
				if(wcscmp(localeName, L"es") == 0)  return Core::Locale("es");
				if(wcscmp(localeName, L"jp") == 0)  return Core::Locale("jp");
			}

			//Just default to english
			return Core::Locale("en");
		}
		//-------------------------------------------------
		//-------------------------------------------------
		f32 PlatformSystem::GetScreenDensity() const
		{
			return 1.0f;
		}
		//-------------------------------------------------
		//-------------------------------------------------
		std::string PlatformSystem::GetDeviceID()
		{
			CS_LOG_ERROR("PlatformSystem::GetDeviceID() has not been implemented!");
			return "FAKE ID";
		}
		//-------------------------------------------------
		//-------------------------------------------------
		u32 PlatformSystem::GetNumberOfCPUCores() const
		{
			SYSTEM_INFO SysInfo;
			GetSystemInfo(&SysInfo);
			return SysInfo.dwNumberOfProcessors;
		}
		//--------------------------------------------------
		//--------------------------------------------------
		u64 PlatformSystem::GetSystemTimeMS() const
		{
			LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);
            return (u64)((currentTime.QuadPart) * 1000.0 / gFrequency.QuadPart);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		f32 PlatformSystem::GetPhysicalScreenSize()
		{
			CS_LOG_WARNING("GetPhysicalScreenSize() is not implemented on windows!");
			return 0.0f;
		}
		//---GLFW Delegates
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::OnWindowResized(s32 indwWidth, s32 indwHeight)
		{
			Core::Application::Get()->OnScreenResized((u32)indwWidth, (u32)indwHeight);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void PlatformSystem::OnWindowClosed()
		{
			Core::Application::Get()->OnSuspend();
			Core::Application::Get()->Quit();
			glfwTerminate();
		}
		//-----------------------------------------
		//-----------------------------------------
		PlatformSystem::~PlatformSystem()
		{
			glfwTerminate();
		}
	}
}
