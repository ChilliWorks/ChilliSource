//
//  GLFWManager.cpp
//  Chilli Source
//
//  Created by Scott Downie on 11/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#include <ChilliSource/Backend/Platform/Windows/GLFW/Base/GLFWManager.h>

//This needs to be included after windows.h
#include <Platform/Windows/glfw3.h>

namespace ChilliSource
{
	namespace Windows
	{
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::Init(u32 in_width, u32 in_height, const char* in_windowName)
		{
			if (!glfwInit())
			{
				CS_LOG_FATAL("Cannot initialise GLFW");
			}

			//TODO: Expose these
			glfwWindowHint(GLFW_RED_BITS, 32);
			glfwWindowHint(GLFW_GREEN_BITS, 32);
			glfwWindowHint(GLFW_BLUE_BITS, 32);
			glfwWindowHint(GLFW_ALPHA_BITS, 32);
			glfwWindowHint(GLFW_DEPTH_BITS, 32);

			m_window = glfwCreateWindow(in_width, in_height, in_windowName, nullptr, nullptr);
			if (m_window == nullptr)
			{
				glfwTerminate();
				CS_LOG_FATAL("Cannot create GLFW window");
			}

			glfwMakeContextCurrent(m_window);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::GetWindowSize(s32* in_width, s32* in_height)
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			CS_ASSERT(in_width != nullptr && in_height != nullptr, "Must have valid pointers");
			glfwGetWindowSize(m_window, in_width, in_height);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::GetCursorPos(f64* in_xPos, f64* in_yPos)
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			CS_ASSERT(in_xPos != nullptr && in_yPos != nullptr, "Must have valid pointers");
			glfwGetCursorPos(m_window, in_xPos, in_yPos);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		f64 GLFWManager::GetTime()
		{
			return glfwGetTime();
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::PollEvents()
		{
			glfwPollEvents();
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void GLFWManager::SetMaxFPS(u32 in_fps)
		{
			glfwSwapInterval(60 / in_fps);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		bool GLFWManager::IsWindowAlive() const
		{
			if (m_window == nullptr)
				return false;

			return glfwWindowShouldClose(m_window) == false;
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::SetCursorPosDelegate(GLFWcursorposfun in_delegate)
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			glfwSetCursorPosCallback(m_window, in_delegate);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::SetMouseButtonDelegate(GLFWmousebuttonfun in_delegate)
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			glfwSetMouseButtonCallback(m_window, in_delegate);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::SetWindowFocusDelegate(GLFWwindowfocusfun in_delegate)
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			glfwSetWindowFocusCallback(m_window, in_delegate);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::SetWindowSizeDelegate(GLFWwindowsizefun in_delegate)
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			glfwSetWindowSizeCallback(m_window, in_delegate);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::SetWindowCloseDelegate(GLFWwindowclosefun in_delegate)
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			glfwSetWindowCloseCallback(m_window, in_delegate);
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::SwapBuffers()
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			glfwSwapBuffers(m_window);
		}
		//---------------------------------------------------------------
		//---------------------------------------------------------------
		GLFWManager::~GLFWManager()
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}
	}
}
