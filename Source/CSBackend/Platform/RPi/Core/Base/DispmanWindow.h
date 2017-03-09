//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#ifndef _CSBACKEND_PLATFORM_RPI_CORE_BASE_DISPMANWINDOW_H_
#define _CSBACKEND_PLATFORM_RPI_CORE_BASE_DISPMANWINDOW_H_

#include <CSBackend/Platform/RPi/ForwardDeclarations.h>
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/LifeCycleManager.h>
#include <ChilliSource/Core/Base/Singleton.h>
#include <ChilliSource/Core/Math/Vector2.h>

#include <bcm_host.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <X11/xlib.h>
#include <X11/keysym.h>

#include <vector>
#include <functional>
#include <mutex>

namespace CSBackend
{
	namespace RPi
	{
		/// Class for interfacing with the Raspberry Pi's dispmanx library/video driver.
		///
		/// While some methods in this class are thread-safe, most should be called
		/// on the system thread.
		///
		class DispmanWindow final : public ChilliSource::Singleton<DispmanWindow>
		{
		public:

			/// Window display modes
			///
			enum class DisplayMode
			{
				k_windowed,
				k_fullscreen
			};

			/// List of the events that can occur on a mouse button
			///
			enum class MouseButtonEvent
			{
				k_pressed,
				k_released
			};

			/// List of the events that can occur on a keyboard event
			///
			enum class KeyboardEvent
			{
				k_pressed,
				k_released
			};

			/// @param The new window size.
			///
			using WindowResizeDelegate = std::function<void(const ChilliSource::Integer2&)>;

			/// @param The new window mode.
			///
			using WindowDisplayModeDelegate = std::function<void(DisplayMode)>;

			/// @param Mouse button that the action occurred on
			/// @param The event type (Pressed/Released)
			///
			using MouseButtonDelegate = std::function<void(u32, MouseButtonEvent)>;

			/// @param Mouse position X (in window coords)
			/// @param Mouse position Y (in window coords)
			///
			using MouseMovedDelegate = std::function<void(s32, s32)>;

			/// @param X11 KeySym that the action occured on
			/// @param X11 Key modifier mask
			/// @param The event type (Pressed/Released)
			///
			using KeyboardEventDelegate = std::function<void(u32, u32, KeyboardEvent)>;

			/// Create and run the application; this will update and render the app.
			///
			void Run() noexcept;

			/// Sets the vblank interval on OpenGL buffer swap
			///
			///	@param fps
			///		The maximum frames per second to clamp to.
			///		This should be in multiples of 15 to match the behaviour of other platforms (e.g. 15, 30, 60)
            ///
			void SetPreferredFPS(u32 fps) noexcept;

			/// @return Current resolution of the dispman window
			///
			ChilliSource::Integer2 GetWindowSize() const noexcept { return m_windowSize; }

			/// @param size
			///		Size to make window in pixels
			///
			void SetSize(const ChilliSource::Integer2& size) noexcept;

			/// @return List of the resolutions supported by the Raspberry Pi's video drivers
			///
			std::vector<ChilliSource::Integer2> GetSupportedResolutions() const noexcept;

			/// Set the delegates that are called when the window changes (resized or mode)
			/// This method will assert if a given delegate is null or if the delegate has already been set.
			///
			/// This method is thread-safe.
			///
			/// @param windowResizeDelegate
			///		The delegate called when the window resizes
			/// @param windowDisplayModeDelegate
			///		Called when the window switches from fullscreen to windowed
			///
			void SetWindowDelegates(WindowResizeDelegate windowResizeDelegate, WindowDisplayModeDelegate windowDisplayModeDelegate) noexcept;

			/// Remove the delegates that relate to window events.
			///
			/// This method is thread-safe.
			///
			void RemoveWindowDelegates() noexcept;

			/// Set the delegates that are called on the various mouse events (button, moved, scrolled)
			/// This method will assert if a given delegate is null or if the delegate has already been set.
			///
			/// This method is thread-safe.
			///
			/// @param mouseButtonDelegate
			///		The delegate called when a mouse button event occurs. In X11 the scroll wheel acts like a button getting pressed events for each tick
			/// @param mouseMovedDelegate
			///		The delegate called when the mouse is moved.
			///
			void SetMouseDelegates(MouseButtonDelegate mouseButtonDelegate, MouseMovedDelegate mouseMovedDelegate) noexcept;

			/// Remove the delegates that relate to mouse events.
			///
			/// This method is thread-safe.
			///
			void RemoveMouseDelegates() noexcept;

			/// @return The current window position of the mouse
			///
			ChilliSource::Integer2 GetMousePosition() const noexcept;

			/// Set the delegates that are called on the various keyboard events (press/release)
			/// This method will assert if a given delegate is null or if the delegate has already been set.
			///
			/// This method is thread-safe.
			///
			/// @param keyboardEventDelegate
			///		The delegate called when a keyboard event occurs.
			///
			void SetKeyboardDelegates(KeyboardEventDelegate keyboardEventDelegate) noexcept;

			/// Remove the delegates that relate to keyboard events.
			///
			/// This method is thread-safe.
			///
			void RemoveKeyboardDelegates() noexcept;

			/// Tell the main loop to shutdown and the windows to close gracefull at the
			/// end of the next update
			///
			void ScheduleQuit() noexcept { m_quitScheduled = true; }

			//TODO: Handle setting of window mode - fullscreen/windowed, etc.
			//TODO: Add event for window mode changing
			//TODO: Handle vsync and FPS setting

            /// Destructor; makes sure that the BCM interface is
            /// properly un-set.
            ///
            ~DispmanWindow() noexcept;

	    private:

			/// Initialise and display the X11 window and register for X events. The X11
			/// window is responsible for the size and position of the GL dispman window and for
			/// receiving mouse and keyboard events
			///
			/// @param windowPos
			///		X, Y position of the window to create
			/// @param windowSize
			///		Size of the window to create
			///
			void InitXWindow(const ChilliSource::Integer2& windowPos, const ChilliSource::Integer2& windowSize) noexcept;

			/// Initialise the dispman window and OpenGLES based on the previously created XWindow.
			///
			/// @param windowPos
			///		X, Y position of the window to create (should be created at the same pos as the X window)
			/// @param windowSize
			///		Size of the window to create (should be created at the same size as the X window)
			///
			void InitEGLDispmanWindow(const ChilliSource::Integer2& windowPos, const ChilliSource::Integer2& windowSize) noexcept;

			/// Single system loop responsible for X server events, rendering and display buffer swaps. The repeating main loop can be
			/// terminated by calling quit. This run loop can be optionally tied to vsync
			///
			void Tick() noexcept;

			/// Update the size and pos of the dispman EGL window
			///
			void UpdateEGLWindow() noexcept;

			/// Terminates the application loop gracefully.
			///
			void Quit() noexcept;

		private:

			ChilliSource::Integer2 m_windowSize;
			ChilliSource::Integer2 m_windowPos;

			/// EGL Objects needed for dispmanx.
			EGLDisplay m_eglDisplay;
			EGLConfig m_eglConfig;
			EGLSurface m_eglSurface;
			EGLContext m_eglContext;
			EGLint m_eglConfigNum;

			/// Dispmanx handles
			DISPMANX_ELEMENT_HANDLE_T m_displayManagerElement;
			DISPMANX_DISPLAY_HANDLE_T m_displayManagerDisplay;

			/// Dispmanx window
			EGL_DISPMANX_WINDOW_T m_nativeWindow;

			/// X11
			Display* m_xdisplay = nullptr;
			Window m_xwindow;

			/// Program state stuff
			bool m_isRunning = false;
			bool m_isFocused = false;
			bool m_quitScheduled = false;

			/// Event delegates
			std::mutex m_mouseMutex;
			MouseButtonDelegate m_mouseButtonDelegate;
			MouseMovedDelegate m_mouseMovedDelegate;

			std::mutex m_windowMutex;
			WindowResizeDelegate m_windowResizeDelegate;
			WindowDisplayModeDelegate m_windowDisplayModeDelegate;

			std::mutex m_keyMutex;
			KeyboardEventDelegate m_keyboardEventDelegate;


			// CS Lifecycle Manager
			ChilliSource::LifecycleManagerUPtr m_lifecycleManager;
		};
	}
}

#endif
#endif
