//
//  SFMLWindow.cpp
//  Chilli Source
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

#include <CSBackend/Platform/Windows/Core/String/WindowsStringUtils.h>
#include <ChilliSource/Core/Base/AppConfig.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/SurfaceFormat.h>

#include <glew.h>
#include <json/json.h>
#include <SFML/OpenGL.hpp>

#include <fstream>

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
			Json::Value ReadAppConfig()
			{
				//get the path to here
				wchar_t pathChars[MAX_PATH];
				GetModuleFileName(nullptr, pathChars, MAX_PATH);
				std::string path = WindowsStringUtils::ConvertWindowsFilePathToStandard(std::wstring(pathChars));
				std::string::size_type pos = path.find_last_of("/");
				std::string workingDir = CSCore::StringUtils::StandardiseDirectoryPath(path.substr(0, pos));

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
						CS_LOG_FATAL("Could not parse App.config: " + jReader.getFormatedErrorMessages());
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
			CSRendering::SurfaceFormat ReadSurfaceFormat(const Json::Value& in_root)
			{
				const std::string k_defaultFormat = "rgb565_depth24";
				std::string formatString = in_root.get("PreferredSurfaceFormat", k_defaultFormat).asString();
				return CSCore::ParseSurfaceFormat(formatString);
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
			u32 ReadMultisampleFormat(const Json::Value& in_root)
			{
				const Json::Value& windows = in_root["Windows"];

				if (windows.isNull() == false)
				{
					std::string stringFormat = windows.get("Multisample", "None").asString();
					CSCore::StringUtils::ToLowerCase(stringFormat);

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
						CS_LOG_FATAL("Unknown multisample format: " + stringFormat + ". Options are None, 2x or 4x");
					}
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
			sf::ContextSettings CreateContextSettings(CSRendering::SurfaceFormat in_format, u32 in_multiSampleFormat)
			{
				sf::ContextSettings glSettings;
				glSettings.majorVersion = 2;
				glSettings.minorVersion = 0;
				glSettings.antialiasingLevel = in_multiSampleFormat;
				glSettings.stencilBits = 0;

				switch (in_format)
				{
				case CSRendering::SurfaceFormat::k_rgb565_depth24:
				case CSRendering::SurfaceFormat::k_rgb888_depth24:
				default:
					glSettings.depthBits = 24;
					break;
				case CSRendering::SurfaceFormat::k_rgb565_depth32:
				case CSRendering::SurfaceFormat::k_rgb888_depth32:
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
			u32 ReadRGBAPixelDepth(CSRendering::SurfaceFormat in_format)
			{
				u32 depth = 32;

				switch (in_format)
				{
				case CSRendering::SurfaceFormat::k_rgb565_depth24:
				case CSRendering::SurfaceFormat::k_rgb565_depth32:
				default:
					depth = 16;
					break;
				case CSRendering::SurfaceFormat::k_rgb888_depth24:
				case CSRendering::SurfaceFormat::k_rgb888_depth32:
					depth = 32;
					break;
				}

				return depth;
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetPreferredFPS(u32 in_fps)
		{
			m_window.setFramerateLimit(in_fps);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetSize(u32 in_width, u32 in_height)
		{
			//Clamp to the actual screen size
			in_width = std::min((u32)sf::VideoMode::getDesktopMode().width, in_width);
			in_height = std::min((u32)sf::VideoMode::getDesktopMode().height, in_height);

			if (m_isFullscreen == true)
			{
				m_isFullscreen = false;
				m_window.create(sf::VideoMode(in_width, in_height, sf::VideoMode::getDesktopMode().bitsPerPixel), "", sf::Style::Default, m_contextSettings);
				m_windowStateEvent.NotifyConnections(WindowState::k_windowed);
				m_windowResizeEvent.NotifyConnections(CSCore::Integer2((s32)in_width, (s32)in_height));
			}
			else
			{
				//This will trigger an SFML resize event
				sf::Vector2u size(in_width, in_height);
				m_window.setSize(size);
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetFullscreen()
		{
			if (m_isFullscreen == true)
				return;

			m_isFullscreen = true;

			//Pick the best fit RGBA depth based on the supported depths
			for (auto it = sf::VideoMode::getFullscreenModes().rbegin(); it != sf::VideoMode::getFullscreenModes().rend(); ++it)
			{
				s32 bpp = it->bitsPerPixel;
				if (bpp >= m_preferredRGBADepth)
				{
					while (it != sf::VideoMode::getFullscreenModes().rend() && bpp == it->bitsPerPixel)
					{
						++it;
					}

					m_window.create(*it, "", sf::Style::Fullscreen, m_contextSettings);
					m_windowStateEvent.NotifyConnections(WindowState::k_fullscreen);
					m_windowResizeEvent.NotifyConnections(CSCore::Integer2(it->width, it->height));
					break;
				}
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		CSCore::IConnectableEvent<SFMLWindow::WindowResizeDelegate>& SFMLWindow::GetWindowResizedEvent()
		{
			return m_windowResizeEvent;
		}
		//-------------------------------------------------
		//-------------------------------------------------
		CSCore::IConnectableEvent<SFMLWindow::WindowStateDelegate>& SFMLWindow::GetWindowStateEvent()
		{
			return m_windowStateEvent;
		}
		//-------------------------------------------------
		//------------------------------------------------
		CSCore::IConnectableEvent<SFMLWindow::MouseButtonDelegate>& SFMLWindow::GetMouseButtonEvent()
		{
			return m_mouseButtonEvent;
		}
		//-------------------------------------------------
		//------------------------------------------------
		CSCore::IConnectableEvent<SFMLWindow::MouseMovedDelegate>& SFMLWindow::GetMouseMovedEvent()
		{
			return m_mouseMovedEvent;
		}
		//-------------------------------------------------
		//------------------------------------------------
		CSCore::IConnectableEvent<SFMLWindow::MouseWheelDelegate>& SFMLWindow::GetMouseWheelEvent()
		{
			return m_mouseWheelEvent;
		}
		//-------------------------------------------------
		//------------------------------------------------
		CSCore::IConnectableEvent<SFMLWindow::TextEnteredEvent>& SFMLWindow::GetTextEnteredEvent()
		{
			return m_textEnteredEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<SFMLWindow::KeyPressedDelegate>& SFMLWindow::GetKeyPressedEvent()
		{
			return m_keyPressedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<SFMLWindow::KeyReleasedDelegate>& SFMLWindow::GetKeyReleasedEvent()
		{
			return m_keyReleasedEvent;
		}
		//------------------------------------------------
		//------------------------------------------------
		CSCore::Integer2 SFMLWindow::GetWindowSize() const
		{
			auto size = m_window.getSize();
			return CSCore::Integer2((s32)size.x, (s32)size.y);
		}
		//------------------------------------------------
		//------------------------------------------------
		CSCore::Integer2 SFMLWindow::GetMousePosition() const
		{
			sf::Vector2i pos = sf::Mouse::getPosition(m_window);
			return CSCore::Integer2((s32)pos.x, (s32)pos.y);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void SFMLWindow::HideCursor()
		{
			m_window.setMouseCursorVisible(false);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void SFMLWindow::ShowCursor()
		{
			m_window.setMouseCursorVisible(true);
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::Run()
		{
			Json::Value appConfigRoot = ReadAppConfig();
			CSRendering::SurfaceFormat surfaceFormat = ReadSurfaceFormat(appConfigRoot);
			u32 msaaFormat = ReadMultisampleFormat(appConfigRoot);
			m_contextSettings = CreateContextSettings(surfaceFormat, msaaFormat);
			m_preferredRGBADepth = ReadRGBAPixelDepth(surfaceFormat);
			m_window.create(sf::VideoMode::getDesktopMode(), "", sf::Style::Default, m_contextSettings);

			GLenum glewError = glewInit();
			if (GLEW_OK != glewError)
			{
				CS_LOG_FATAL("Glew Error On Init: " + std::string((const char*)glewGetErrorString(glewError)));
			}

			sf::Clock clock;
			auto appStartTime = clock.getElapsedTime().asSeconds();
			auto appPreviousTime = appStartTime;

			CSCore::Application* app = CreateApplication();
			app->Init();
			app->Resume();
			app->Foreground();

			auto appConfig = CSCore::Application::Get()->GetAppConfig();
			m_window.setFramerateLimit(appConfig->GetPreferredFPS());
			m_window.setTitle(appConfig->GetDisplayableName());

			while (m_isSuspended == false)
			{
				sf::Event event;
				while (m_window.pollEvent(event))
				{
					switch (event.type)
					{
						default:
							break;
						case sf::Event::Closed:
							app->Quit();
							return;
						case sf::Event::Resized:
							m_windowResizeEvent.NotifyConnections(CSCore::Integer2((s32)event.size.width, (s32)event.size.height));
							break;
						case sf::Event::GainedFocus:
							if (m_isFocused == false)
							{
								m_isFocused = true;
								app->Foreground();
							}
							break;
						case sf::Event::LostFocus:
							if (m_isFocused == true)
							{
								m_isFocused = false;
								app->Background();
							}
							break;
						case sf::Event::MouseButtonPressed:
							m_mouseButtonEvent.NotifyConnections(event.mouseButton.button, MouseButtonEvent::k_pressed, event.mouseButton.x, event.mouseButton.y);
							break;
						case sf::Event::MouseButtonReleased:
							m_mouseButtonEvent.NotifyConnections(event.mouseButton.button, MouseButtonEvent::k_released, event.mouseButton.x, event.mouseButton.y);
							break;
						case sf::Event::MouseMoved:
							m_mouseMovedEvent.NotifyConnections(event.mouseMove.x, event.mouseMove.y);
							break;
						case sf::Event::MouseWheelMoved:
							m_mouseWheelEvent.NotifyConnections(event.mouseWheel.delta);
							break;
						case sf::Event::KeyPressed:
							m_keyPressedEvent.NotifyConnections(event.key.code, event.key);
							break;
						case sf::Event::KeyReleased:
							m_keyReleasedEvent.NotifyConnections(event.key.code);
							break;
						case sf::Event::TextEntered:
						{
							CSCore::UTF8Char utf8Char = 0;
							sf::Utf32::toUtf8(&event.text.unicode, (&event.text.unicode) + 1, &utf8Char);
							m_textEnteredEvent.NotifyConnections(utf8Char);
							break;
						}
					}
				}

				auto appCurrentTime = clock.getElapsedTime().asSeconds();

				auto deltaTime = (appCurrentTime - appPreviousTime);
				auto runningTime = (appPreviousTime - appStartTime);
				appPreviousTime = appCurrentTime;

				app->Update(deltaTime, runningTime);
				app->Render();
			}

			delete app;
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::Display()
		{
			m_window.display();
		}
		//--------------------------------------------
		//--------------------------------------------
		void SFMLWindow::Quit()
		{
			if (m_isFocused == true)
			{
				CSCore::Application::Get()->Background();
			}
			if (m_isSuspended == false)
			{
				CSCore::Application::Get()->Suspend();
			}

			CSCore::Application::Get()->Destroy();
		}
	}
}

#endif
