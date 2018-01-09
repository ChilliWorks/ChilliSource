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

#include <CSBackend/Platform/RPi/Core/Base/DispmanWindow.h>
#include <CSBackend/Platform/RPi/Core/Base/SystemInfoFactory.h>
#include <CSBackend/Platform/RPi/Core/Base/EGLConfigChooser.h>

#include <ChilliSource/Core/Base/StandardMacros.h>
#include <ChilliSource/Core/Base/AppConfig.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/LifecycleManager.h>
#include <ChilliSource/Core/Base/SystemInfo.h>
#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Rendering/Base/SurfaceFormat.h>
#include <ChilliSource/Core/String/StringParser.h>

#include <X11/Xatom.h>
#include <X11/XKBlib.h>

namespace CSBackend
{
	namespace RPi
	{
		namespace
		{
			/// Reads the app.config file root into a Json object
			///
			/// @return App config as json object
			///
			inline Json::Value ReadAppConfig() noexcept
			{
				// Get current executable directory
				char arg1[20];
				char exepath[PATH_MAX + 1] = {0};

				sprintf(arg1, "/proc/%d/exe", getpid());
				readlink(arg1, exepath, 1024 );

				std::string path = std::string(exepath);
				std::string::size_type pos = path.find_last_of("/");
				std::string workingDir = ChilliSource::StringUtils::StandardiseDirectoryPath(path.substr(0, pos));

				//open the file
				std::ifstream file(workingDir + "assets/AppResources/App.config");

				Json::Value root;

				if (file.good() == true)
				{
					std::string contents((std::istreambuf_iterator<s8>(file)), std::istreambuf_iterator<s8>());

					//parse the json
					Json::Reader jReader;
					if (!jReader.parse(contents, root))
					{
						printf("[ChilliSource] Could not parse App.config: %s \n", jReader.getFormattedErrorMessages().c_str());
					}
				}
				else
				{
					printf("[ChilliSource] Error reading App.config.");
				}

				return root;
			}

			/// Reads the surface format from the App.config file.
			///
			/// @param root
			///		App config Json root
			///
			/// @return The surface format.
			///
			inline ChilliSource::SurfaceFormat ReadSurfaceFormat(const Json::Value& root) noexcept
			{
				const std::string k_defaultFormat = "rgb565_depth24";
				std::string formatString = root.get("PreferredSurfaceFormat", k_defaultFormat).asString();

				const Json::Value& rpi = root["RPi"];
				if (rpi.isNull() == false && rpi.isMember("PreferredSurfaceFormat"))
				{
					formatString = rpi["PreferredSurfaceFormat"].asString();
				}

				return ChilliSource::ParseSurfaceFormat(formatString);
			}

			/// Reads the multisample format from the App.config file.
			///
			/// @param root
			///		App config Json root
			///
			/// @return Number times multisample 0x, 2x, 4x.
			///
			inline u32 ReadMultisampleFormat(const Json::Value& root) noexcept
			{
				std::string stringFormat = root.get("Multisample", "None").asString();

				const Json::Value& rpi = root["RPi"];
				if (rpi.isNull() == false && rpi.isMember("Multisample"))
				{
					stringFormat = rpi["Multisample"].asString();
				}

				ChilliSource::StringUtils::ToLowerCase(stringFormat);

				if (stringFormat == "none")
				{
					printf("[ChilliSource] WARNING: Multisampling is not yet supported on Raspberry Pi\n");
					return 0;
				}
				else if (stringFormat == "2x")
				{
					printf("[ChilliSource] WARNING: Multisampling is not yet supported on Raspberry Pi\n");
					return 2;
				}
				else if (stringFormat == "4x")
				{
					return 4;
				}
				else
				{
					printf("[ChilliSource] Unknown multisample format : %s .Options are None, 2x or 4x\n", stringFormat.c_str());
				}

				return 0;
			}


			/// Creates an EGLConfigChooser that will select the closest EGL config that
			/// matches the preferred configuration in the app.config
			///
			/// @param appConfigRoot
			///		App config json root
			///
			/// @return Config chooser that when executed will pick the closest supported config
			///
			inline EGLConfigChooser CreateConfigChooser(const Json::Value& appConfigRoot) noexcept
			{
				ChilliSource::SurfaceFormat surfaceFormat = ReadSurfaceFormat(appConfigRoot);

				switch(surfaceFormat)
				{
					case ChilliSource::SurfaceFormat::k_rgb565_depth24:
						return EGLConfigChooser(5, 6, 5, 0, 16, 24, 0);
						break;
					case ChilliSource::SurfaceFormat::k_rgb565_depth32:
						return EGLConfigChooser(5, 6, 5, 0, 16, 32, 0);
						break;
					case ChilliSource::SurfaceFormat::k_rgb888_depth24:
						return EGLConfigChooser(8, 8, 8, 0, 16, 24, 0);
						break;
					case ChilliSource::SurfaceFormat::k_rgb888_depth32:
						return EGLConfigChooser(8, 8, 8, 0, 16, 32, 0);
						break;
					case ChilliSource::SurfaceFormat::k_rgb565_depth24_stencil8:
						return EGLConfigChooser(5, 6, 5, 0, 16, 24, 8);
						break;
					case ChilliSource::SurfaceFormat::k_rgb565_depth32_stencil8:
						return EGLConfigChooser(5, 6, 5, 0, 16, 32, 8);
						break;
					case ChilliSource::SurfaceFormat::k_rgb888_depth24_stencil8:
						return EGLConfigChooser(8, 8, 8, 0, 16, 24, 8);
						break;
					case ChilliSource::SurfaceFormat::k_rgb888_depth32_stencil8:
						return EGLConfigChooser(8, 8, 8, 0, 16, 32, 8);
						break;
				}

				// Default to k_rgb565_depth24.
				CS_LOG_WARNING("Couldn't get EGLConfig from App.config. Defaulting to RGB565_DEPTH24.");
				return EGLConfigChooser(5, 6, 5, 0, 16, 24, 0);
			}

			/// Reads the desired window title from the AppConfig.
			///
			/// @param appConfigRoot
			///		App config json root
			///
			/// @return Window title
			///
			inline std::string ReadDesiredTitle(const Json::Value& appConfigRoot) noexcept
			{
				std::string titleString = "Application";

				titleString = appConfigRoot.get("DisplayableName", titleString).asString();

				return titleString;
			}

			/// @param appConfigRoot
			///		The app config JSON
			/// @param fallbackResolution
			///		Window res to fallback on if none are specified
			///
			/// @return Inital window resolution as read from the config
			///
			inline ChilliSource::Integer2 ReadInitialWindowSize(const Json::Value& appConfigRoot, const ChilliSource::Integer2& fallbackResolution) noexcept
			{
				ChilliSource::Integer2 res = fallbackResolution;
				const Json::Value& resObj = appConfigRoot["WindowResolution"];
				if(resObj.isNull() == false)
				{
					res = ChilliSource::ParseInteger2(resObj.asString());
				}
				const Json::Value& platformObj = appConfigRoot["RPi"];
				if(platformObj.isNull() == false)
				{
					const Json::Value& resObjPlatform = platformObj["WindowResolution"];
					if(resObjPlatform.isNull() == false)
					{
						res = ChilliSource::ParseInteger2(resObjPlatform.asString());
					}
				}

				return res;
			}

			/// @param appConfigRoot
			///		The app config JSON
			///
			/// @return Inital window mode as read from the config
			///
			inline ChilliSource::Screen::DisplayMode ReadInitialWindowMode(const Json::Value& appConfigRoot) noexcept
			{
				std::string mode = appConfigRoot.get("WindowDisplayMode", "windowed").asString();
				const Json::Value& platformObj = appConfigRoot["RPi"];
				if(platformObj.isNull() == false)
				{
					mode = platformObj.get("WindowDisplayMode", mode.c_str()).asString();
				}

				return ChilliSource::Screen::ParseDisplayMode(mode);
			}
		}
		
		//----------------------------------------------------------------------------------
		void DispmanWindow::Run() noexcept
		{
			//The display setup we use mimics Minecraft on the Raspberry Pi. Essentially we use a dispman display
			//to take advantage of hardware acceleration but we layer that on top of an X window which we use to listen
			//for events and to allow the user to control the size of the dispman display.

			//Start interfacing with Raspberry Pi.
			bcm_host_init();

			Json::Value appConfigRoot = ReadAppConfig();

			auto displayMode = ReadInitialWindowMode(appConfigRoot);
			ReadMultisampleFormat(appConfigRoot);

			m_desktopSize = GetSupportedFullscreenResolutions()[0] - (GetSupportedFullscreenResolutions()[0] / 10);
			m_windowSize = ReadInitialWindowSize(appConfigRoot, displayMode == ChilliSource::Screen::DisplayMode::k_windowed ? m_desktopSize : GetSupportedFullscreenResolutions()[0]);
			m_windowPos = ChilliSource::Integer2::k_zero;

			InitXWindow(m_windowPos, m_windowSize, appConfigRoot);
			InitEGLDispmanWindow(m_windowPos, m_windowSize, appConfigRoot);

			//NOTE: We are creating ChilliSource here so no CS calls can be made prior to this including logging.
			ChilliSource::ApplicationUPtr app = ChilliSource::ApplicationUPtr(CreateApplication(SystemInfoFactory::CreateSystemInfo()));
			m_lifecycleManager = ChilliSource::LifecycleManagerUPtr(new ChilliSource::LifecycleManager(app.get()));
			m_lifecycleManager->Resume();

			SetDisplayMode(displayMode);

			m_isRunning = true;
			while(m_isRunning == true)
			{
				Tick();
			}
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::InitXWindow(const ChilliSource::Integer2& windowPos, const ChilliSource::Integer2& windowSize, const Json::Value& appConfigRoot) noexcept
		{
			m_xdisplay = XOpenDisplay(NULL);
			if(m_xdisplay == nullptr)
			{
				printf("[ChilliSource] Failed to create X11 display. Exiting");
				return;
			}

			m_xwindow = XCreateSimpleWindow(m_xdisplay, XDefaultRootWindow(m_xdisplay), windowPos.x, windowPos.y, windowSize.x, windowSize.y, 0, 0, 0);

			m_xinputMethod = XOpenIM(m_xdisplay, 0, 0, 0);
			m_xinputContext = XCreateIC(m_xinputMethod, XNClientWindow, m_xwindow, XNFocusWindow, m_xwindow, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, 0);

			//Capture the window close message
			m_wmDeleteMessage = XInternAtom(m_xdisplay, "WM_DELETE_WINDOW", False);
			XSetWMProtocols(m_xdisplay, m_xwindow, &m_wmDeleteMessage, 1);

			XMapWindow(m_xdisplay, m_xwindow);
			XStoreName(m_xdisplay, m_xwindow, ReadDesiredTitle(appConfigRoot).c_str());

			//All the events we need to listen for
			XSelectInput(m_xdisplay, m_xwindow, PointerMotionMask | ButtonMotionMask | ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask | FocusChangeMask | StructureNotifyMask);

			//Sends all the commands to the X server.
			XFlush(m_xdisplay);
		}

		//-----------------------------------------------------------------------------------------
		//
		// /!\ NOTE: The attribute below is used to instruct GCC not to optimize this function.
		//           This is required as optimizing here prevents the OpenGL context initializing.
		//
		//-----------------------------------------------------------------------------------------
		void __attribute__((optimize("O0"))) DispmanWindow::InitEGLDispmanWindow(const ChilliSource::Integer2& windowPos, const ChilliSource::Integer2& windowSize, const Json::Value& appConfigRoot) noexcept
		{
			// Set up OpenGL context version (OpenGLES 2.0)
			static const EGLint contextAttributeList[] =
			{
				EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL_NONE
			};

			// Setup the EGL display with the framebuffer config and the GL version
			m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			eglInitialize(m_eglDisplay, NULL, NULL);

			// Create ConfigChooser and choose an EGLConfig appropriately.
			EGLConfigChooser eglConfigChooser = CreateConfigChooser(appConfigRoot);
			m_eglConfig = eglConfigChooser.ChooseBestConfig(m_eglDisplay);

			eglBindAPI(EGL_OPENGL_ES_API);
			m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, contextAttributeList);

			// Set up blit rects.
			VC_RECT_T dstRect;
			vc_dispmanx_rect_set(&dstRect, windowPos.x, windowPos.y, windowSize.x, m_windowSize.y);
			VC_RECT_T srcRect;
			vc_dispmanx_rect_set(&srcRect, windowPos.x, windowPos.y, windowSize.x << 16, m_windowSize.y << 16);

			// Set up dispmanx
			m_displayManagerDisplay = vc_dispmanx_display_open(0);
			DISPMANX_UPDATE_HANDLE_T displayManagerUpdate = vc_dispmanx_update_start(0);
			m_displayManagerElement = vc_dispmanx_element_add(displayManagerUpdate, m_displayManagerDisplay, 0, &dstRect, 0, &srcRect, DISPMANX_PROTECTION_NONE, 0, 0, DISPMANX_NO_ROTATE);

			// Set up native window.
			m_nativeWindow.element = m_displayManagerElement;
			m_nativeWindow.width = windowSize.x;
			m_nativeWindow.height = windowSize.y;

			// Instruct VC chip to use this display manager to sync
			vc_dispmanx_update_submit_sync(displayManagerUpdate);

			// Set up EGL surface and connect the context to it
			m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, &m_nativeWindow, NULL);
			eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::Tick() noexcept
		{
			while(XPending(m_xdisplay))
			{
				XEvent event;
				XNextEvent(m_xdisplay, &event);

				switch (event.type)
				{
					case FocusIn:
					{
						XSetICFocus(m_xinputContext);
						if(m_isFocused == false)
						{
							m_isFocused = true;
							m_lifecycleManager->Foreground();
						}
						break;
					}
					case FocusOut:
					{
						XUnsetICFocus(m_xinputContext);
						if(m_isFocused == true)
						{
							m_isFocused = false;
							m_lifecycleManager->Background();
						}
						break;
					}
					case ConfigureNotify:
					{
						bool resized = event.xconfigure.width != m_windowSize.x || event.xconfigure.height != m_windowSize.y;
						if(resized == true)
						{
							m_windowSize.x = event.xconfigure.width;
							m_windowSize.y = event.xconfigure.height;
						}

						//When resizing the window we receive a real event and a synthetic event. The real event has wrong
						//x, y coordinates so we must use the ones from the synthetic event. When moving the window we only
						//get the synthetic event.
						if(event.xconfigure.send_event == true)
						{
							m_windowPos.x = event.xconfigure.x;
							m_windowPos.y = event.xconfigure.y;
						}

						//Update the EGL window to match the xwindow's new size or position
						UpdateEGLWindow();

						std::unique_lock<std::mutex> lock(m_windowMutex);
						if (resized && m_windowResizeDelegate)
						{
							m_windowResizeDelegate(m_windowSize);
						}
						break;
					}
					case MotionNotify:
					{
						std::unique_lock<std::mutex> lock(m_mouseMutex);
						if (m_mouseMovedDelegate)
						{
							m_mouseMovedDelegate(event.xbutton.x, event.xbutton.y);
						}
						break;
					}
					case ButtonPress:
					{
						std::unique_lock<std::mutex> lock(m_mouseMutex);
						if (m_mouseButtonDelegate)
						{
							m_mouseButtonDelegate(event.xbutton.button, MouseButtonEvent::k_pressed);
						}
						break;
					}
					case ButtonRelease:
					{
						std::unique_lock<std::mutex> lock(m_mouseMutex);
						if (m_mouseButtonDelegate)
						{
							m_mouseButtonDelegate(event.xbutton.button, MouseButtonEvent::k_released);
						}
						break;
					}
					case KeyPress:
					{
						std::unique_lock<std::mutex> keyLock(m_keyMutex);
						if(m_keyboardEventDelegate)
						{
							m_keyboardEventDelegate(XkbKeycodeToKeysym(m_xdisplay, event.xkey.keycode, 0, 0), event.xkey.state, KeyboardEvent::k_pressed);
						}
						keyLock.unlock();

						std::unique_lock<std::mutex> textLock(m_textMutex);
						if(m_textEntryEventDelegate)
						{
							if(XFilterEvent(&event, m_xwindow) == false)
							{
								Status status;
								u8 keyBuffer[4] = {0};
								auto length = Xutf8LookupString(m_xinputContext, &event.xkey, (char*)keyBuffer, sizeof(keyBuffer), 0, &status);
								if(length > 0)
								{
									ChilliSource::UTF8Char utf8Char = keyBuffer[0] | (keyBuffer[1] << 8) | (keyBuffer[2] << 16) | (keyBuffer[3] << 24);
									m_textEntryEventDelegate((u32)keyBuffer[0]);
								}
							}
						}
						break;
					}
					case KeyRelease:
					{
						std::unique_lock<std::mutex> lock(m_keyMutex);
						if(m_keyboardEventDelegate)
						{
							m_keyboardEventDelegate(XkbKeycodeToKeysym(m_xdisplay, event.xkey.keycode, 0, 0), event.xkey.state, KeyboardEvent::k_released);
						}
						break;
					}
					case ClientMessage:
					{
            			if (event.xclient.data.l[0] == m_wmDeleteMessage)
						{
							Quit();
							return;
						}
						break;
					}
				}
			}

			//Update, render and then flip display buffer
			m_lifecycleManager->SystemUpdate();
			m_lifecycleManager->Render();

			//NOTE: This enforces the VSync based on eglSwapInterval
			eglSwapBuffers(m_eglDisplay, m_eglSurface);

			if(m_quitScheduled)
			{
				Quit();
			}
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::SetPreferredFPS(u32 fps) noexcept
		{
			eglSwapInterval(m_eglDisplay, 60u/fps);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::SetSize(const ChilliSource::Integer2& size) noexcept
		{
			if(size == m_windowSize)
				return;

			if (m_displayMode == ChilliSource::Screen::DisplayMode::k_fullscreen)
			{
				CS_ASSERT(ChilliSource::VectorUtils::Contains(GetSupportedFullscreenResolutions(), size) == true, "Resolution not supported in fullscreen mode (SetSize).");
			}

			m_windowSize = size;
			XWindowChanges wc;
			wc.x = m_windowPos.x;
			wc.y = m_windowPos.y;
			wc.width = size.x;
			wc.height = size.y;
			XConfigureWindow(m_xdisplay, m_xwindow, CWX|CWY|CWWidth|CWHeight, &wc);
			XSync(m_xdisplay, False);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::SetDisplayMode(ChilliSource::Screen::DisplayMode mode) noexcept
		{
			if (mode == m_displayMode)
				return;

			m_displayMode = mode;

			XEvent fullscreenEvent;
			Atom x11_state_atom	= XInternAtom(m_xdisplay, "_NET_WM_STATE", False);
			Atom x11_fs_atom = XInternAtom(m_xdisplay, "_NET_WM_STATE_FULLSCREEN", False);

			fullscreenEvent.xclient.type = ClientMessage;
			fullscreenEvent.xclient.serial = 0;
			fullscreenEvent.xclient.send_event = True;
			fullscreenEvent.xclient.window = m_xwindow;
			fullscreenEvent.xclient.message_type = x11_state_atom;
			fullscreenEvent.xclient.format = 32;
			fullscreenEvent.xclient.data.l[1] = x11_fs_atom;
			fullscreenEvent.xclient.data.l[2] = 0;

			switch (m_displayMode)
			{
				case ChilliSource::Screen::DisplayMode::k_fullscreen:
				{
					CS_ASSERT(ChilliSource::VectorUtils::Contains(GetSupportedFullscreenResolutions(), m_windowSize) == true, "Current resolution not supported in fullscreen mode.");

					m_windowPosPreFullscreen = m_windowPos;

					fullscreenEvent.xclient.data.l[0] = 1;//_NET_WM_STATE_ADD;

					XSendEvent(m_xdisplay, XDefaultRootWindow(m_xdisplay), False, SubstructureRedirectMask | SubstructureNotifyMask, &fullscreenEvent);
					break;
				}
				case ChilliSource::Screen::DisplayMode::k_windowed:
				{
					m_windowPos = m_windowPosPreFullscreen;

					fullscreenEvent.xclient.data.l[0] = 0;//_NET_WM_STATE_REMOVE;

					XSendEvent(m_xdisplay, XDefaultRootWindow(m_xdisplay), False, SubstructureRedirectMask | SubstructureNotifyMask, &fullscreenEvent);
					SetSize(ChilliSource::Integer2::Min(m_windowSize, m_desktopSize));
					break;
				}
			}
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::UpdateEGLWindow() noexcept
		{
			DISPMANX_UPDATE_HANDLE_T displayManagerUpdate = vc_dispmanx_update_start(0);
			VC_RECT_T dstRect;
			vc_dispmanx_rect_set(&dstRect, m_windowPos.x, m_windowPos.y, m_windowSize.x, m_windowSize.y);
			VC_RECT_T srcRect;
			vc_dispmanx_rect_set(&srcRect, m_windowPos.x, m_windowPos.y, m_windowSize.x << 16, m_windowSize.y << 16);
			vc_dispmanx_element_change_attributes(displayManagerUpdate, m_displayManagerElement, 0, 0, 0, &dstRect, &srcRect, 0, DISPMANX_NO_ROTATE);

			m_nativeWindow.width = m_windowSize.x;
			m_nativeWindow.height = m_windowSize.y;

			vc_dispmanx_update_submit_sync(displayManagerUpdate);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::SetWindowDelegates(WindowResizeDelegate windowResizeDelegate) noexcept
		{
			CS_ASSERT(windowResizeDelegate, "Window resize delegate invalid.");
			CS_ASSERT(!m_windowResizeDelegate, "Window resize delegate already set.");

			std::unique_lock<std::mutex> lock(m_windowMutex);
			m_windowResizeDelegate = std::move(windowResizeDelegate);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::RemoveWindowDelegates() noexcept
		{
			std::unique_lock<std::mutex> lock(m_windowMutex);
			m_windowResizeDelegate = nullptr;
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::SetMouseDelegates(MouseButtonDelegate mouseButtonDelegate, MouseMovedDelegate mouseMovedDelegate) noexcept
		{
			std::unique_lock<std::mutex> lock(m_mouseMutex);

			CS_ASSERT(mouseButtonDelegate, "Mouse button event delegate invalid.");
			CS_ASSERT(mouseMovedDelegate, "Mouse moved delegate invalid.");
			CS_ASSERT(!m_mouseButtonDelegate, "Mouse button event delegate already set.");
			CS_ASSERT(!m_mouseMovedDelegate, "Mouse moved delegate already set.");

			m_mouseButtonDelegate = std::move(mouseButtonDelegate);
			m_mouseMovedDelegate = std::move(mouseMovedDelegate);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::RemoveMouseDelegates() noexcept
		{
			std::unique_lock<std::mutex> lock(m_mouseMutex);
			m_mouseButtonDelegate = nullptr;
			m_mouseMovedDelegate = nullptr;
		}

		//-----------------------------------------------------------------------------------
		ChilliSource::Integer2 DispmanWindow::GetMousePosition() const noexcept
		{
			Window root, child;
			s32 rootX, rootY, winX, winY;
			u32 buttonMask;
			XQueryPointer(m_xdisplay, m_xwindow, &root, &child, &rootX, &rootY, &winX, &winY, &buttonMask);

			return ChilliSource::Integer2(winX, winY);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::SetKeyboardDelegates(KeyboardEventDelegate keyboardEventDelegate) noexcept
		{
			std::unique_lock<std::mutex> lock(m_keyMutex);

			CS_ASSERT(keyboardEventDelegate, "Keyboard event delegate invalid.");
			CS_ASSERT(!m_keyboardEventDelegate, "Keyboard event delegate already set.");

			m_keyboardEventDelegate = std::move(keyboardEventDelegate);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::RemoveKeyboardDelegates() noexcept
		{
			std::unique_lock<std::mutex> lock(m_keyMutex);
			m_keyboardEventDelegate = nullptr;
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::SetTextEntryDelegates(TextEntryEventDelegate textEntryEventDelegate) noexcept
		{
			std::unique_lock<std::mutex> lock(m_textMutex);

			CS_ASSERT(textEntryEventDelegate, "Text entry event delegate invalid.");
			CS_ASSERT(!m_textEntryEventDelegate, "Text entry event delegate already set.");

			m_textEntryEventDelegate = std::move(textEntryEventDelegate);
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::RemoveTextEntryDelegates() noexcept
		{
			std::unique_lock<std::mutex> lock(m_textMutex);
			m_textEntryEventDelegate = nullptr;
		}

		//-----------------------------------------------------------------------------------
		std::vector<ChilliSource::Integer2> DispmanWindow::GetSupportedFullscreenResolutions() const noexcept
		{
			u32 width(0), height(0);
			graphics_get_display_size(0, &width, &height);
			return std::vector<ChilliSource::Integer2> { ChilliSource::Integer2((s32)width, (s32)height) };
		}

		//-----------------------------------------------------------------------------------
		void DispmanWindow::Quit() noexcept
		{
			if (m_isFocused == true)
			{
				m_lifecycleManager->Background();
				m_isFocused = false;
			}

			m_lifecycleManager->Suspend();
			m_lifecycleManager.reset();

			m_isRunning = false;
		}

		//-----------------------------------------------------------------------------------
		DispmanWindow::~DispmanWindow() noexcept
		{
			CS_ASSERT(!m_windowResizeDelegate, "Window resize delegate not removed.");
			CS_ASSERT(!m_mouseButtonDelegate, "Mouse button event delegate not removed.");
			CS_ASSERT(!m_mouseMovedDelegate, "Mouse moved delegate not removed.");
			CS_ASSERT(!m_keyboardEventDelegate, "Keyboard event delegate not removed.");
			CS_ASSERT(!m_textEntryEventDelegate, "Text entry event delegate not removed");

			eglDestroyContext(m_eglDisplay, m_eglContext);
			eglDestroySurface(m_eglDisplay, m_eglSurface);
			eglTerminate(m_eglDisplay);

			if(m_xdisplay)
			{
				XDestroyIC(m_xinputContext);
				XCloseIM(m_xinputMethod);
				XDestroyWindow(m_xdisplay, m_xwindow);
				XCloseDisplay(m_xdisplay);
			}

			bcm_host_deinit();
		}
	}
}

#endif
