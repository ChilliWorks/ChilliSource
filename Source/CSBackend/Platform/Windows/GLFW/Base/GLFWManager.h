//
//  GLFWManager.h
//  Chilli Source
//
//  Created by Scott Downie on 11/03/2014.
//  Copyright (c) 2014 Tag Games Ltd. All rights reserved.
//

#ifdef CS_TARGETPLATFORM_WINDOWS

#ifndef _CSBACKEND_PLATFORM_WINDOWS_GLFW_BASE_GLFWMANAGER_H_
#define _CSBACKEND_PLATFORM_WINDOWS_GLFW_BASE_GLFWMANAGER_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Singleton.h>

struct GLFWwindow;

typedef void(*GLFWmousebuttonfun)(GLFWwindow*, int, int, int);
typedef void(*GLFWcursorposfun)(GLFWwindow*, double, double);
typedef void(*GLFWwindowfocusfun)(GLFWwindow*, int);
typedef void(*GLFWwindowsizefun)(GLFWwindow*, int, int);
typedef void(*GLFWwindowclosefun)(GLFWwindow*);

namespace CSBackend
{
	namespace Windows
	{
		//---------------------------------------------------
		/// Helper singleton for creating and managing
		/// a GLFW window
		//---------------------------------------------------
		class GLFWManager : public CSCore::Singleton<GLFWManager>
		{
		public:

			enum class MouseButtonAction
			{
				k_press = 1,
				k_release = 0
			};
			//---------------------------------------------------------------
			/// Destructor
			///
			/// @author S Downie
			//---------------------------------------------------------------
			~GLFWManager();
			//---------------------------------------------------
			/// Initialise GLFW and create the application 
			/// window
			///
			/// @author S Downie
			///
			/// @param Window width
			/// @param Window height
			//---------------------------------------------------
			void Init(u32 in_windowWidth, u32 in_windowHeight);
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @param [Out] Cursor X Pos
			/// @param [Out] Cursor Y Pos
			//---------------------------------------------------
			void GetCursorPos(f64* in_xPos, f64* in_yPos);
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @param [Out] Width
			/// @param [Out] Height
			//---------------------------------------------------
			void GetWindowSize(s32* in_width, s32* in_height);
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @return Time in seconds since GLFW was initialised
			//---------------------------------------------------
			f64 GetTime();
			//---------------------------------------------------
			/// Swap the double buffered window
			///
			/// @author S Downie
			//---------------------------------------------------
			void SwapBuffers();
			//---------------------------------------------------
			/// Poll for events such as input
			///
			/// @author S Downie
			//---------------------------------------------------
			void PollEvents();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @param The maximum frames per second to clamp 
			/// to. This should be in multiples of 15 
			/// (15, 30, 60)
			//-------------------------------------------------
			void SetPreferredFPS(u32 in_fps);
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return Whether the window exists
			//-------------------------------------------------
			bool IsWindowAlive() const;
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @return GLFW window
			//---------------------------------------------------
			inline GLFWwindow* GetWindow()
			{
				return m_window;
			}
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @param C delegate that is triggered on cursor pos
			/// changed
			//---------------------------------------------------
			void SetCursorPosDelegate(GLFWcursorposfun in_delegate);
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @param C delegate that is triggered on mouse
			/// button events
			//---------------------------------------------------
			void SetMouseButtonDelegate(GLFWmousebuttonfun in_delegate);
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @param C delegate that is triggered when window
			/// focus changes
			//---------------------------------------------------
			void SetWindowFocusDelegate(GLFWwindowfocusfun in_delegate);
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @param C delegate that is triggered when window
			/// is resized
			//---------------------------------------------------
			void SetWindowSizeDelegate(GLFWwindowsizefun in_delegate);
			//---------------------------------------------------
			/// @author S Downie
			///
			/// @param C delegate that is triggered when window
			/// is closed
			//---------------------------------------------------
			void SetWindowCloseDelegate(GLFWwindowclosefun in_delegate);
			//---------------------------------------------------
			/// Sets the title of the window.
			///
			/// @author Ian Copland
			///
			/// @param The new window title.
			//---------------------------------------------------
			void SetWindowTitle(const std::string& in_title);

		private:

			GLFWwindow* m_window = nullptr;
		};
	}
}

#endif

#endif
