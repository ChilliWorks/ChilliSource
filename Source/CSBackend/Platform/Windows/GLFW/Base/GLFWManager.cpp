//
//  GLFWManager.cpp
//  Chilli Source
//
//  Created by Scott Downie on 11/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/GLFW/Base/GLFWManager.h>

#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/SurfaceFormat.h>

#include <json/json.h>

#include <fstream>

#include <windows.h>

//This needs to be included after windows.h
#include <Platform/Windows/glfw3.h>

namespace CSBackend
{
	namespace Windows
	{
		namespace
		{
			//-------------------------------------------------------------
			/// Reads the surface format from the App.config file.
			///
			/// @author I Copland
			///
			/// @param The surface format.
			//-------------------------------------------------------------
			CSRendering::SurfaceFormat GetSurfaceFormat()
			{
				const std::string k_defaultFormat = "rgb565_depth24";

				//get the path to here
				wchar_t pathChars[MAX_PATH];
				GetModuleFileName(nullptr, pathChars, MAX_PATH);
				std::string path = WindowsStringUtils::ConvertWindowsPathToStandard(std::wstring(pathChars));
				std::string::size_type pos = path.find_last_of("/");
				std::string workingDir = CSCore::StringUtils::StandardisePath(path.substr(0, pos));

				//open the file
				std::ifstream file(workingDir + "assets/AppResources/Shared/App.config");

				std::string formatString = k_defaultFormat;
				if (file.good() == true)
				{
					std::string contents((std::istreambuf_iterator<s8>(file)), std::istreambuf_iterator<s8>());

					//parse the json
					Json::Reader jReader;
					Json::Value root;
					if (!jReader.parse(contents, root))
					{
						CS_LOG_FATAL("Could not parse App.config: " + jReader.getFormatedErrorMessages());
					}

					formatString = root.get("PreferredSurfaceFormat", k_defaultFormat).asString();
				}

				return CSCore::ParseSurfaceFormat(formatString);
			}
			//-------------------------------------------------------------
			/// Applies the surface format described in the App.config file
			/// to the glfw window.
			///
			/// @author I Copland
			//-------------------------------------------------------------
			void ApplySurfaceFormat()
			{
				CSRendering::SurfaceFormat format = GetSurfaceFormat();

				switch (format)
				{
				case CSRendering::SurfaceFormat::k_rgb565_depth24:
				default:
					glfwWindowHint(GLFW_RED_BITS, 5);
					glfwWindowHint(GLFW_GREEN_BITS, 6);
					glfwWindowHint(GLFW_BLUE_BITS, 5);
					glfwWindowHint(GLFW_ALPHA_BITS, 0);
					glfwWindowHint(GLFW_DEPTH_BITS, 24);
					glfwWindowHint(GLFW_STENCIL_BITS, 0);
					break;
				case CSRendering::SurfaceFormat::k_rgb565_depth32:
					glfwWindowHint(GLFW_RED_BITS, 5);
					glfwWindowHint(GLFW_GREEN_BITS, 6);
					glfwWindowHint(GLFW_BLUE_BITS, 5);
					glfwWindowHint(GLFW_ALPHA_BITS, 0);
					glfwWindowHint(GLFW_DEPTH_BITS, 32);
					glfwWindowHint(GLFW_STENCIL_BITS, 0);
					break;
				case CSRendering::SurfaceFormat::k_rgb888_depth24:
					glfwWindowHint(GLFW_RED_BITS, 8);
					glfwWindowHint(GLFW_GREEN_BITS, 8);
					glfwWindowHint(GLFW_BLUE_BITS, 8);
					glfwWindowHint(GLFW_ALPHA_BITS, 0);
					glfwWindowHint(GLFW_DEPTH_BITS, 24);
					glfwWindowHint(GLFW_STENCIL_BITS, 0);
					break;
				case CSRendering::SurfaceFormat::k_rgb888_depth32:
					glfwWindowHint(GLFW_RED_BITS, 8);
					glfwWindowHint(GLFW_GREEN_BITS, 8);
					glfwWindowHint(GLFW_BLUE_BITS, 8);
					glfwWindowHint(GLFW_ALPHA_BITS, 0);
					glfwWindowHint(GLFW_DEPTH_BITS, 32);
					glfwWindowHint(GLFW_STENCIL_BITS, 0);
					break;
				}
			}
		}
		//---------------------------------------------------
		//---------------------------------------------------
		void GLFWManager::Init(u32 in_width, u32 in_height)
		{
			if (!glfwInit())
			{
				CS_LOG_FATAL("Cannot initialise GLFW");
			}

			ApplySurfaceFormat();

			m_window = glfwCreateWindow(in_width, in_height, "", nullptr, nullptr);
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
		void GLFWManager::SetPreferredFPS(u32 in_fps)
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
		void GLFWManager::SetWindowTitle(const std::string& in_title)
		{
			CS_ASSERT(m_window != nullptr, "Must have created a window");
			glfwSetWindowTitle(m_window, in_title.c_str());
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

#endif
