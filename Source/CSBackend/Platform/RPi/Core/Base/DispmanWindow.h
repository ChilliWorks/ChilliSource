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

#include <vector>

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
			/// Create and run the application; this will update and render the app.
			///
			void Run() noexcept;

			/// @return Current resolution of the dispman window
			///
			ChilliSource::Integer2 GetWindowSize() const noexcept { return m_windowSize; }

			/// @return List of the resolutions supported by the Raspberry Pi's video drivers
			///
			std::vector<ChilliSource::Integer2> GetSupportedResolutions() const noexcept;

			//TODO: Events for when the window is resized post switch to X11.
			//TODO: Handle setting of window mode - fullscreen/windowed, etc.
			//TODO: Add event for window mode changing

            /// Destructor; makes sure that the BCM interface is
            /// properly un-set.
            ///
            ~DispmanWindow() noexcept;

	    private:

			/// Terminates the application loop gracefully.
			///
			void Quit() noexcept;

		private:

			ChilliSource::Integer2 m_windowSize;

			/// EGL Objects needed for dispmanx.
			EGLDisplay m_eglDisplay;
			EGLConfig m_eglConfig;
			EGLSurface m_eglSurface;
			EGLContext m_eglContext;
			EGLint m_eglConfigNum;

			/// Videocom blitting rects
			VC_RECT_T m_dstRect;
			VC_RECT_T m_srcRect;

			/// Dispmanx handles
			DISPMANX_ELEMENT_HANDLE_T m_displayManagerElement;
			DISPMANX_DISPLAY_HANDLE_T m_displayManagerDisplay;
			DISPMANX_UPDATE_HANDLE_T m_displayManagerUpdate;

			/// Dispmanx window
			EGL_DISPMANX_WINDOW_T m_nativeWindow;

			/// Program state stuff
			bool m_bcmInitialised = false;
			bool m_isRunning = false;
			bool m_quitScheduled = false;

			// CS Lifecycle Manager
			ChilliSource::LifecycleManagerUPtr m_lifecycleManager;
		};
	}

}

#endif
#endif
