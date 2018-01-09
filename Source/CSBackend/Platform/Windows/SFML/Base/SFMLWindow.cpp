//
//  SFMLWindow.cpp
//  ChilliSource
//  Created by Scott Downie on 11/03/2014.
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

#ifdef CS_TARGETPLATFORM_WINDOWS

#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>

#include <CSBackend/Platform/Windows/Core/Base/SystemInfoFactory.h>
#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <ChilliSource/Core/Base/AppConfig.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/LifecycleManager.h>
#include <ChilliSource/Core/Base/SystemInfo.h>
#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Base/SurfaceFormat.h>

#include <GL/glew.h>
#include <json/json.h>
#include <SFML/OpenGL.hpp>

#include <fstream>
#include <array>
#include <mutex>

#include <windows.h>


namespace CSBackend
{
	namespace Windows
	{
		namespace
		{
			//-------------------------------------------------------------
			/// Reads the app.config file root into a Json object
			///
			/// @author Ian Copland
			///
			/// @return Json root
			//-------------------------------------------------------------
			Json::Value ReadAppConfig() noexcept
			{
				//get the path to here
				wchar_t pathChars[MAX_PATH];
				GetModuleFileName(nullptr, pathChars, MAX_PATH);
				std::string path = WindowsStringUtils::ConvertWindowsFilePathToStandard(std::wstring(pathChars));
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
						OutputDebugString(CSBackend::Windows::WindowsStringUtils::UTF8ToUTF16("[ChilliSource] Could not parse App.config: " + jReader.getFormattedErrorMessages() + "\n").c_str());
					}
				}

				return root;
			}
			//-------------------------------------------------------------
			/// Reads the surface format from the App.config file.
			///
			/// @author S Downie
			///
			/// @param App config Json root
			///
			/// @return The surface format.
			//-------------------------------------------------------------
			ChilliSource::SurfaceFormat ReadSurfaceFormat(const Json::Value& in_root) noexcept
			{
				const std::string k_defaultFormat = "rgb565_depth24";
				std::string formatString = in_root.get("PreferredSurfaceFormat", k_defaultFormat).asString();

				const Json::Value& windows = in_root["Windows"];
				if (windows.isNull() == false && windows.isMember("PreferredSurfaceFormat"))
				{
					formatString = windows["PreferredSurfaceFormat"].asString();
				}

				return ChilliSource::ParseSurfaceFormat(formatString);
			}
			//-------------------------------------------------------------
			/// Reads the multisample format from the App.config file.
			///
			/// @author S Downie
			///
			/// @param App config Json root
			///
			/// @return Number times multisample 0x, 2x, 4x.
			//-------------------------------------------------------------
			u32 ReadMultisampleFormat(const Json::Value& in_root) noexcept
			{
				std::string stringFormat = in_root.get("Multisample", "None").asString();

				const Json::Value& windows = in_root["Windows"];
				if (windows.isNull() == false && windows.isMember("Multisample"))
				{
					stringFormat = windows["Multisample"].asString();
				}

				ChilliSource::StringUtils::ToLowerCase(stringFormat);

				if (stringFormat == "none")
				{
					return 0;
				}
				else if (stringFormat == "2x")
				{
					return 2;
				}
				else if (stringFormat == "4x")
				{
					return 4;
				}
				else
				{
					OutputDebugString(CSBackend::Windows::WindowsStringUtils::UTF8ToUTF16("[ChilliSource] Unknown multisample format : " + stringFormat + ".Options are None, 2x or 4x\n").c_str());
				}

				return 0;
			}
			//-------------------------------------------------------------
			/// Builds an SFML context based on the surface format
			/// NOTE: SFML doesn't support setting of RGBA values
			///
			/// @author S Downie
			///
			/// @param Surface format
			/// @param Number of samples for MSAA
			///
			/// @return Context settings
			//-------------------------------------------------------------
			sf::ContextSettings CreateContextSettings(ChilliSource::SurfaceFormat in_format, u32 in_multiSampleFormat) noexcept
			{
				sf::ContextSettings glSettings;
				glSettings.majorVersion = 2;
				glSettings.minorVersion = 0;
				glSettings.antialiasingLevel = in_multiSampleFormat;
				glSettings.stencilBits = 0;

				switch (in_format)
				{
				case ChilliSource::SurfaceFormat::k_rgb565_depth24:
				case ChilliSource::SurfaceFormat::k_rgb888_depth24:
				default:
					glSettings.depthBits = 24;
					break;
				case ChilliSource::SurfaceFormat::k_rgb565_depth32:
				case ChilliSource::SurfaceFormat::k_rgb888_depth32:
					glSettings.depthBits = 32;
					break;
				}

				return glSettings;
			}
			//-------------------------------------------------------------
			/// Read the RGBA bit depth based on the surface format
			///
			/// @author S Downie
			///
			/// @param Surface format
			///
			/// @return Pixel bit depth
			//-------------------------------------------------------------
			u32 ReadRGBAPixelDepth(ChilliSource::SurfaceFormat in_format) noexcept
			{
				u32 depth = 32;

				switch (in_format)
				{
				case ChilliSource::SurfaceFormat::k_rgb565_depth24:
				case ChilliSource::SurfaceFormat::k_rgb565_depth32:
				default:
					depth = 16;
					break;
				case ChilliSource::SurfaceFormat::k_rgb888_depth24:
				case ChilliSource::SurfaceFormat::k_rgb888_depth32:
					depth = 32;
					break;
				}

				return depth;
			}

			/// @param appConfigRoot
			///		The app config JSON
			/// @param fallbackResolution
			///		Window res to fallback on if none are specified
			///
			/// @return Inital window resolution as read from the config
			///
			ChilliSource::Integer2 ReadInitialWindowSize(const Json::Value& appConfigRoot, const ChilliSource::Integer2& fallbackResolution) noexcept
			{
				ChilliSource::Integer2 res = fallbackResolution;
				const Json::Value& resObj = appConfigRoot["WindowResolution"];
				if(resObj.isNull() == false)
				{
					res = ChilliSource::ParseInteger2(resObj.asString());
				}
				const Json::Value& platformObj = appConfigRoot["Windows"];
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
			ChilliSource::Screen::DisplayMode ReadInitialWindowMode(const Json::Value& appConfigRoot) noexcept
			{
				std::string mode = appConfigRoot.get("WindowDisplayMode", "windowed").asString();
				const Json::Value& platformObj = appConfigRoot["Windows"];
				if(platformObj.isNull() == false)
				{
					mode = platformObj.get("WindowDisplayMode", mode.c_str()).asString();
				}

				return ChilliSource::Screen::ParseDisplayMode(mode);
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetPreferredFPS(u32 in_fps)
		{
			m_preferredFPS = in_fps;
			m_window.setVerticalSyncEnabled(false);
			m_window.setFramerateLimit(in_fps);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetVSyncEnabled(bool in_enable)
		{
			if (in_enable == true)
			{
				m_window.setFramerateLimit(0);
				m_window.setVerticalSyncEnabled(true);
			}
			else
			{
				m_window.setVerticalSyncEnabled(false);
				m_window.setFramerateLimit(m_preferredFPS);
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetSize(const ChilliSource::Integer2& in_size)
		{
			if(in_size == GetWindowSize())
				return;

			if (m_displayMode == ChilliSource::Screen::DisplayMode::k_fullscreen)
			{
				CS_ASSERT(ChilliSource::VectorUtils::Contains(GetSupportedFullscreenResolutions(), in_size) == true, "Resolution not supported in fullscreen mode.");
			}

			//Clamp to the actual screen size
			auto windowSize = ChilliSource::Integer2::Max(in_size, ChilliSource::Integer2::k_one);

			//This will trigger an SFML resize event
			m_window.setSize(sf::Vector2u(windowSize.x, windowSize.y));

			if (m_displayMode == ChilliSource::Screen::DisplayMode::k_fullscreen)
			{
				SetFullscreen(windowSize);
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetDisplayMode(ChilliSource::Screen::DisplayMode in_mode, const ChilliSource::Integer2& size, bool force)
		{
			if (in_mode == m_displayMode && force == false)
				return;

			m_displayMode = in_mode;

			switch (m_displayMode)
			{
			case ChilliSource::Screen::DisplayMode::k_fullscreen:
				SetFullscreen(size);
				break;
			case ChilliSource::Screen::DisplayMode::k_windowed:
				SetWindowed(ChilliSource::Integer2::Min(size, m_desktopSize));
				break;
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetFullscreen(const ChilliSource::Integer2& size)
		{
			CS_ASSERT(ChilliSource::VectorUtils::Contains(GetSupportedFullscreenResolutions(), size) == true, "Resolution not supported in fullscreen mode.");

			//Pick the best fit RGBA depth based on the supported depths
			for (auto it = sf::VideoMode::getFullscreenModes().rbegin(); it != sf::VideoMode::getFullscreenModes().rend(); ++it)
			{
				u32 bpp = it->bitsPerPixel;
				if (bpp >= m_preferredRGBADepth)
				{
					m_window.create(sf::VideoMode(size.x, size.y, bpp), m_title, sf::Style::Fullscreen, m_contextSettings);

                    std::unique_lock<std::mutex> lock(m_windowMutex);
                    if (m_windowDisplayModeDelegate)
                    {
                        m_windowDisplayModeDelegate(ChilliSource::Screen::DisplayMode::k_fullscreen);
                    }
					break;
				}
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetWindowed(const ChilliSource::Integer2& size)
		{
			bool resized = m_window.isOpen() && GetWindowSize() != size;
			m_window.create(sf::VideoMode((u32)size.x, (u32)size.y, sf::VideoMode::getDesktopMode().bitsPerPixel), m_title, sf::Style::Default, m_contextSettings);

            std::unique_lock<std::mutex> lock(m_windowMutex);
            if (m_windowDisplayModeDelegate)
            {
                m_windowDisplayModeDelegate(ChilliSource::Screen::DisplayMode::k_windowed);
            }

			if (resized && m_windowResizeDelegate)
			{
				m_windowResizeDelegate(size);
			}
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		std::vector<ChilliSource::Integer2> SFMLWindow::GetSupportedFullscreenResolutions() const
		{
			std::vector<ChilliSource::Integer2> result;
			result.reserve(sf::VideoMode::getFullscreenModes().size());

			for (const auto& mode : sf::VideoMode::getFullscreenModes())
			{
				ChilliSource::Integer2 resolution((s32)mode.width, (s32)mode.height);
				if (ChilliSource::VectorUtils::Contains(result, resolution) == false)
				{
					result.push_back(resolution);
				}
			}

			return result;
		}
        //------------------------------------------------
        //------------------------------------------------
        void SFMLWindow::SetWindowDelegates(const WindowResizeDelegate& in_windowResizeDelegate, const WindowDisplayModeDelegate& in_windowDisplayModeDelegate) noexcept
        {
            CS_ASSERT(in_windowResizeDelegate, "Window resize delegate invalid.");
            CS_ASSERT(in_windowDisplayModeDelegate, "Window display mode delegate invalid.");
            CS_ASSERT(!m_windowResizeDelegate, "Window resize delegate already set.");
            CS_ASSERT(!m_windowDisplayModeDelegate, "Window display mode delegate already set.");

            std::unique_lock<std::mutex> lock(m_windowMutex);
            m_windowResizeDelegate = in_windowResizeDelegate;
            m_windowDisplayModeDelegate = in_windowDisplayModeDelegate;
        }
        //------------------------------------------------
        //------------------------------------------------
        void SFMLWindow::SetMouseDelegates(const MouseButtonDelegate& in_mouseButtonDelegate, const MouseMovedDelegate& in_mouseMovedDelegate, const MouseWheelDelegate& in_mouseWheelDelegate) noexcept
        {
            CS_ASSERT(in_mouseButtonDelegate, "Mouse button event delegate invalid.");
            CS_ASSERT(in_mouseMovedDelegate, "Mouse moved delegate invalid.");
            CS_ASSERT(in_mouseWheelDelegate, "Mouse wheel scroll delegate invalid.");
            CS_ASSERT(!m_mouseButtonDelegate, "Mouse button event delegate already set.");
            CS_ASSERT(!m_mouseMovedDelegate, "Mouse moved delegate already set.");
            CS_ASSERT(!m_mouseWheelDelegate, "Mouse wheel scroll delegate already set.");

            std::unique_lock<std::mutex> lock(m_mouseMutex);
            m_mouseButtonDelegate = in_mouseButtonDelegate;
            m_mouseMovedDelegate = in_mouseMovedDelegate;
            m_mouseWheelDelegate = in_mouseWheelDelegate;
        }
        //------------------------------------------------
        //------------------------------------------------
        void SFMLWindow::SetTextEnteredDelegate(const TextEnteredDelegate& in_textEnteredDelegate) noexcept
        {
            CS_ASSERT(in_textEnteredDelegate, "Text entered delegate invalid.");
            CS_ASSERT(!m_textEnteredDelegate, "Text entered delegate already set.");

            std::unique_lock<std::mutex> lock(m_textEntryMutex);
            m_textEnteredDelegate = in_textEnteredDelegate;
        }
        //------------------------------------------------
        //------------------------------------------------
        void SFMLWindow::SetKeyDelegates(const KeyPressedDelegate& in_keyPressedDelegate, const KeyReleasedDelegate& in_keyReleasedDelegate) noexcept
        {
            CS_ASSERT(in_keyPressedDelegate, "Key pressed delegate invalid.");
            CS_ASSERT(in_keyReleasedDelegate, "Key released delegate invalid.");
            CS_ASSERT(!m_keyPressedDelegate, "Key pressed delegate already set.");
            CS_ASSERT(!m_keyReleasedDelegate, "Key released delegate already set.");

            std::unique_lock<std::mutex> lock(m_keyMutex);
            m_keyPressedDelegate = in_keyPressedDelegate;
            m_keyReleasedDelegate = in_keyReleasedDelegate;
        }

		//------------------------------------------------------------------------------
		void SFMLWindow::SetJoystickDelegates(JoystickConnectionChangedDelegate connectedDelegate, JoystickConnectionChangedDelegate disconnectedDelegate,
			JoystickButtonDelegate buttonPressedDelegate, JoystickButtonDelegate buttonReleasedDelegate, JoystickMovedDelegate movedDelegate) noexcept
		{
			CS_ASSERT(connectedDelegate, "Joystick connected delegate invalid.");
			CS_ASSERT(disconnectedDelegate, "Joystick disconnected delegate invalid.");
			CS_ASSERT(buttonPressedDelegate, "Joystick pressed delegate invalid.");
			CS_ASSERT(buttonReleasedDelegate, "Joystick released delegate invalid.");
			CS_ASSERT(movedDelegate, "Joystick moved delegate invalid.");

			CS_ASSERT(!m_joystickConnectedDelegate, "Joystick connected already set.");
			CS_ASSERT(!m_joystickDisconnectedDelegate, "Joystick disconnected already set.");
			CS_ASSERT(!m_joystickButtonPressedDelegate, "Joystick pressed already set.");
			CS_ASSERT(!m_joystickButtonReleasedDelegate, "Joystick released already set.");
			CS_ASSERT(!m_joystickMovedDelegate, "Joystick moved already set.");

			std::unique_lock<std::mutex> lock(m_joystickMutex);
			m_joystickConnectedDelegate = std::move(connectedDelegate);
			m_joystickDisconnectedDelegate = std::move(disconnectedDelegate);
			m_joystickButtonPressedDelegate = std::move(buttonPressedDelegate);
			m_joystickButtonReleasedDelegate = std::move(buttonReleasedDelegate);
			m_joystickMovedDelegate = std::move(movedDelegate);
		}

        //------------------------------------------------
        //------------------------------------------------
        void SFMLWindow::RemoveWindowDelegates() noexcept
        {
            std::unique_lock<std::mutex> lock(m_windowMutex);
            m_windowResizeDelegate = nullptr;
            m_windowDisplayModeDelegate = nullptr;
        }
        //------------------------------------------------
        //------------------------------------------------
        void SFMLWindow::RemoveMouseDelegates() noexcept
        {
            std::unique_lock<std::mutex> lock(m_mouseMutex);
            m_mouseButtonDelegate = nullptr;
            m_mouseMovedDelegate = nullptr;
            m_mouseWheelDelegate = nullptr;
        }
        //------------------------------------------------
        //------------------------------------------------
        void SFMLWindow::RemoveTextEnteredDelegate() noexcept
        {
            std::unique_lock<std::mutex> lock(m_textEntryMutex);
            m_textEnteredDelegate = nullptr;
        }
        //------------------------------------------------
        //------------------------------------------------
        void SFMLWindow::RemoveKeyDelegates() noexcept
        {
            std::unique_lock<std::mutex> lock(m_keyMutex);
            m_keyPressedDelegate = nullptr;
            m_keyReleasedDelegate = nullptr;
        }

		//------------------------------------------------------------------------------
		void SFMLWindow::RemoveJoystickDelegates() noexcept
		{
			std::unique_lock<std::mutex> lock(m_joystickMutex);
			m_joystickConnectedDelegate = nullptr;
			m_joystickDisconnectedDelegate = nullptr;
			m_joystickButtonPressedDelegate = nullptr;
			m_joystickButtonReleasedDelegate = nullptr;
			m_joystickMovedDelegate = nullptr;
		}

		//------------------------------------------------
		//------------------------------------------------
		ChilliSource::Integer2 SFMLWindow::GetWindowSize() const
		{
			auto size = m_window.getSize();
			return ChilliSource::Integer2((s32)size.x, (s32)size.y);
		}
		//------------------------------------------------
		//------------------------------------------------
		sf::WindowHandle SFMLWindow::GetWindowHandle() const
		{
			return m_window.getSystemHandle();
		}
		//------------------------------------------------
		//------------------------------------------------
		ChilliSource::Integer2 SFMLWindow::GetMousePosition() const
		{
			sf::Vector2i pos = sf::Mouse::getPosition(m_window);
			return ChilliSource::Integer2((s32)pos.x, (s32)pos.y);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void SFMLWindow::HideSystemCursor()
		{
			m_window.setMouseCursorVisible(false);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void SFMLWindow::ShowSystemCursor()
		{
			m_window.setMouseCursorVisible(true);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::Run()
		{
			Json::Value appConfigRoot = ReadAppConfig();
			ChilliSource::SurfaceFormat surfaceFormat = ReadSurfaceFormat(appConfigRoot);
			u32 msaaFormat = ReadMultisampleFormat(appConfigRoot);
			m_contextSettings = CreateContextSettings(surfaceFormat, msaaFormat);
			m_preferredRGBADepth = ReadRGBAPixelDepth(surfaceFormat);

			auto displayMode = ReadInitialWindowMode(appConfigRoot);
			m_desktopSize = ChilliSource::Integer2((s32)sf::VideoMode::getDesktopMode().width, (s32)sf::VideoMode::getDesktopMode().height);
			ChilliSource::Integer2 windowSize = ReadInitialWindowSize(appConfigRoot, displayMode == ChilliSource::Screen::DisplayMode::k_windowed ? m_desktopSize : GetSupportedFullscreenResolutions()[0]);
			SetDisplayMode(displayMode, windowSize, true);

			glewExperimental = GL_TRUE;
			GLenum glewError = glewInit();
			if (GLEW_OK != glewError)
			{
				OutputDebugString(CSBackend::Windows::WindowsStringUtils::UTF8ToUTF16("[ChilliSource] Glew Error On Init : " + std::string((const char*)glewGetErrorString(glewError)) + "\n").c_str());
				exit(1);
			}

            // Create SystemInfo here (make into an immutable class)
            // Pass to application
			ChilliSource::ApplicationUPtr app = ChilliSource::ApplicationUPtr(CreateApplication(SystemInfoFactory::CreateSystemInfo()));
            m_lifecycleManager = ChilliSource::LifecycleManagerUPtr(new ChilliSource::LifecycleManager(app.get()));
            m_lifecycleManager->Resume();
            m_lifecycleManager->Foreground();

			m_isRunning = true;
			m_isFocused = true;

			auto appConfig = ChilliSource::Application::Get()->GetAppConfig();
			m_preferredFPS = appConfig->GetPreferredFPS();

			if (appConfig->IsVSyncEnabled())
			{
				SetVSyncEnabled(true);
			}
			else
			{
				m_window.setFramerateLimit(m_preferredFPS);
			}

			m_title = appConfig->GetDisplayableName();
			m_window.setTitle(m_title);

			while (m_isRunning == true)
			{
				sf::Event event;
				while (m_window.pollEvent(event))
				{
					switch (event.type)
					{
						default:
							break;
						case sf::Event::Closed:
							Quit();
							return;
						case sf::Event::Resized:
                        {
                            std::unique_lock<std::mutex> lock(m_windowMutex);
                            if (m_windowResizeDelegate)
                            {
                                m_windowResizeDelegate(ChilliSource::Integer2(s32(event.size.width), s32(event.size.height)));
                            }
                            break;
                        }
						case sf::Event::GainedFocus:
							if (m_isFocused == false)
							{
								m_isFocused = true;
                                m_lifecycleManager->Foreground();
							}
							break;
						case sf::Event::LostFocus:
							if (m_isFocused == true)
							{
								m_isFocused = false;
                                m_lifecycleManager->Background();
							}
							break;
						case sf::Event::MouseButtonPressed:
                        {
                            std::unique_lock<std::mutex> lock(m_mouseMutex);
                            if (m_mouseButtonDelegate)
                            {
                                m_mouseButtonDelegate(event.mouseButton.button, MouseButtonEvent::k_pressed, event.mouseButton.x, event.mouseButton.y);
                            }
                            break;
                        }
						case sf::Event::MouseButtonReleased:
                        {
                            std::unique_lock<std::mutex> lock(m_mouseMutex);
                            if (m_mouseButtonDelegate)
                            {
                                m_mouseButtonDelegate(event.mouseButton.button, MouseButtonEvent::k_released, event.mouseButton.x, event.mouseButton.y);
                            }
                            break;
                        }
						case sf::Event::MouseMoved:
                        {
                            std::unique_lock<std::mutex> lock(m_mouseMutex);
                            if (m_mouseMovedDelegate)
                            {
                                m_mouseMovedDelegate(event.mouseMove.x, event.mouseMove.y);
                            }
                            break;
                        }
						case sf::Event::MouseWheelMoved:
                        {
                            std::unique_lock<std::mutex> lock(m_mouseMutex);
                            if (m_mouseWheelDelegate)
                            {
                                m_mouseWheelDelegate(event.mouseWheel.delta);
                            }
                            break;
                        }
						case sf::Event::KeyPressed:
                        {
                            std::unique_lock<std::mutex> lock(m_keyMutex);
                            if (m_keyPressedDelegate)
                            {
                                m_keyPressedDelegate(event.key.code, event.key);
                            }
                            break;
                        }
						case sf::Event::KeyReleased:
                        {
                            std::unique_lock<std::mutex> lock(m_keyMutex);
                            if (m_keyReleasedDelegate)
                            {
                                m_keyReleasedDelegate(event.key.code);
                            }
                            break;
                        }
						case sf::Event::TextEntered:
						{
                            std::unique_lock<std::mutex> lock(m_textEntryMutex);
                            if (m_textEnteredDelegate)
                            {
                                ChilliSource::UTF8Char utf8Char = event.text.unicode;
                                m_textEnteredDelegate(utf8Char);
                            }
							break;
						}
						case sf::Event::JoystickConnected:
						{
							std::unique_lock<std::mutex> lock(m_joystickMutex);
							if (m_joystickConnectedDelegate)
							{
								m_joystickConnectedDelegate(event.joystickConnect.joystickId);
							}
							break;
						}
						case sf::Event::JoystickDisconnected:
						{
							std::unique_lock<std::mutex> lock(m_joystickMutex);
							if (m_joystickDisconnectedDelegate)
							{
								m_joystickDisconnectedDelegate(event.joystickConnect.joystickId);
							}
							break;
						}
						case sf::Event::JoystickButtonPressed:
						{
							std::unique_lock<std::mutex> lock(m_joystickMutex);
							if (m_joystickButtonPressedDelegate)
							{
								m_joystickButtonPressedDelegate(event.joystickButton.joystickId, event.joystickButton.button);
							}
							break;
						}
						case sf::Event::JoystickButtonReleased:
						{
							std::unique_lock<std::mutex> lock(m_joystickMutex);
							if (m_joystickButtonReleasedDelegate)
							{
								m_joystickButtonReleasedDelegate(event.joystickButton.joystickId, event.joystickButton.button);
							}
							break;
						}
						case sf::Event::JoystickMoved:
						{
							std::unique_lock<std::mutex> lock(m_joystickMutex);
							if (m_joystickMovedDelegate)
							{
								m_joystickMovedDelegate(event.joystickMove.joystickId, event.joystickMove.axis, event.joystickMove.position);
							}
							break;
						}
					}
				}

				if (m_isFocused == true)
				{
                	m_lifecycleManager->SystemUpdate();
                	m_lifecycleManager->Render();
                	m_window.display();
				}

				if (m_quitScheduled)
				{
					Quit();
				}
			}
		}
		//--------------------------------------------
		//--------------------------------------------
		void SFMLWindow::Quit()
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
        //------------------------------------------------
        //------------------------------------------------
        SFMLWindow::~SFMLWindow() noexcept
        {
            CS_ASSERT(!m_windowResizeDelegate, "Window resize delegate not removed.");
            CS_ASSERT(!m_windowDisplayModeDelegate, "Window display mode delegate not removed.");
            CS_ASSERT(!m_mouseButtonDelegate, "Mouse button event delegate not removed.");
            CS_ASSERT(!m_mouseMovedDelegate, "Mouse moved delegate not removed.");
            CS_ASSERT(!m_mouseWheelDelegate, "Mouse wheel scroll delegate not removed.");
            CS_ASSERT(!m_textEnteredDelegate, "Text entry delegate not removed.");
            CS_ASSERT(!m_keyPressedDelegate, "Key press delegate not removed.");
            CS_ASSERT(!m_keyReleasedDelegate, "Key release delegate not removed.");
			CS_ASSERT(!m_joystickConnectedDelegate, "Joystick connected delegate not removed.");
			CS_ASSERT(!m_joystickDisconnectedDelegate, "Joystick disconnected delegate not removed.");
			CS_ASSERT(!m_joystickButtonPressedDelegate, "Joystick pressed delegate not removed.");
			CS_ASSERT(!m_joystickButtonReleasedDelegate, "Joystick released delegate not removed.");
			CS_ASSERT(!m_joystickMovedDelegate, "Joystick connected moved not removed.");
        }
	}
}

#endif
