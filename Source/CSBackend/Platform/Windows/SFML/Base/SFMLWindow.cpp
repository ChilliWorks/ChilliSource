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
#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/String/StringParser.h>
#include <ChilliSource/Rendering/Base/SurfaceFormat.h>

#include <GL/glew.h>
#include <json/json.h>
#include <SFML/OpenGL.hpp>

#include <fstream>
#include <array>

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
						OutputDebugString(CSBackend::Windows::WindowsStringUtils::UTF8ToUTF16("[Chilli Source] Could not parse App.config: " + jReader.getFormattedErrorMessages() + "\n").c_str());
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
			ChilliSource::SurfaceFormat ReadSurfaceFormat(const Json::Value& in_root)
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
			u32 ReadMultisampleFormat(const Json::Value& in_root)
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
					OutputDebugString(CSBackend::Windows::WindowsStringUtils::UTF8ToUTF16("[Chilli Source] Unknown multisample format : " + stringFormat + ".Options are None, 2x or 4x\n").c_str());
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
			sf::ContextSettings CreateContextSettings(ChilliSource::SurfaceFormat in_format, u32 in_multiSampleFormat)
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
			u32 ReadRGBAPixelDepth(ChilliSource::SurfaceFormat in_format)
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
			if (m_displayMode == DisplayMode::k_fullscreen)
			{
				CS_ASSERT(ChilliSource::VectorUtils::Contains(GetSupportedResolutions(), in_size) == true, "Resolution not supported in fullscreen mode.");
			}

			//Clamp to the actual screen size
			auto windowSize = ChilliSource::Integer2::Max(in_size, ChilliSource::Integer2::k_one);

			//This will trigger an SFML resize event
			m_window.setSize(sf::Vector2u(windowSize.x, windowSize.y));

			if (m_displayMode == DisplayMode::k_fullscreen)
			{
				SetFullscreen();
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetDisplayMode(DisplayMode in_mode)
		{
			if (in_mode == m_displayMode)
				return;

			m_displayMode = in_mode;

			switch (m_displayMode)
			{
			case DisplayMode::k_fullscreen:
				SetFullscreen();
				break;
			case DisplayMode::k_windowed:
				SetWindowed();
				break;
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetFullscreen()
		{
			CS_ASSERT(ChilliSource::VectorUtils::Contains(GetSupportedResolutions(), GetWindowSize()) == true, "Resolution not supported in fullscreen mode.");

			//Pick the best fit RGBA depth based on the supported depths
			for (auto it = sf::VideoMode::getFullscreenModes().rbegin(); it != sf::VideoMode::getFullscreenModes().rend(); ++it)
			{
				u32 bpp = it->bitsPerPixel;
				if (bpp >= m_preferredRGBADepth)
				{
					auto windowSize = GetWindowSize();
					m_window.create(sf::VideoMode(windowSize.x, windowSize.y, bpp), m_title, sf::Style::Fullscreen, m_contextSettings);
					m_windowDisplayModeEvent.NotifyConnections(DisplayMode::k_fullscreen);
					break;
				}
			}
		}
		//-------------------------------------------------
		//-------------------------------------------------
		void SFMLWindow::SetWindowed()
		{
			auto windowSize = GetWindowSize();
			m_window.create(sf::VideoMode(windowSize.x, windowSize.y, sf::VideoMode::getDesktopMode().bitsPerPixel), m_title, sf::Style::Default, m_contextSettings);
			m_windowDisplayModeEvent.NotifyConnections(DisplayMode::k_windowed);
		}
		//----------------------------------------------------------
		//----------------------------------------------------------
		std::vector<ChilliSource::Integer2> SFMLWindow::GetSupportedResolutions() const
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
		//-------------------------------------------------
		//-------------------------------------------------
		ChilliSource::IConnectableEvent<SFMLWindow::WindowResizeDelegate>& SFMLWindow::GetWindowResizedEvent()
		{
			return m_windowResizeEvent;
		}
		//-------------------------------------------------
		//-------------------------------------------------
		ChilliSource::IConnectableEvent<SFMLWindow::WindowDisplayModeDelegate>& SFMLWindow::GetWindowDisplayModeEvent()
		{
			return m_windowDisplayModeEvent;
		}
		//-------------------------------------------------
		//------------------------------------------------
		ChilliSource::IConnectableEvent<SFMLWindow::MouseButtonDelegate>& SFMLWindow::GetMouseButtonEvent()
		{
			return m_mouseButtonEvent;
		}
		//-------------------------------------------------
		//------------------------------------------------
		ChilliSource::IConnectableEvent<SFMLWindow::MouseMovedDelegate>& SFMLWindow::GetMouseMovedEvent()
		{
			return m_mouseMovedEvent;
		}
		//-------------------------------------------------
		//------------------------------------------------
		ChilliSource::IConnectableEvent<SFMLWindow::MouseWheelDelegate>& SFMLWindow::GetMouseWheelEvent()
		{
			return m_mouseWheelEvent;
		}
		//-------------------------------------------------
		//------------------------------------------------
		ChilliSource::IConnectableEvent<SFMLWindow::TextEnteredEvent>& SFMLWindow::GetTextEnteredEvent()
		{
			return m_textEnteredEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ChilliSource::IConnectableEvent<SFMLWindow::KeyPressedDelegate>& SFMLWindow::GetKeyPressedEvent()
		{
			return m_keyPressedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ChilliSource::IConnectableEvent<SFMLWindow::KeyReleasedDelegate>& SFMLWindow::GetKeyReleasedEvent()
		{
			return m_keyReleasedEvent;
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
			ChilliSource::SurfaceFormat surfaceFormat = ReadSurfaceFormat(appConfigRoot);
			u32 msaaFormat = ReadMultisampleFormat(appConfigRoot);
			m_contextSettings = CreateContextSettings(surfaceFormat, msaaFormat);
			m_preferredRGBADepth = ReadRGBAPixelDepth(surfaceFormat);

			ChilliSource::Integer2 windowSize(s32(f32(sf::VideoMode::getDesktopMode().width) * 0.8f), s32(f32(sf::VideoMode::getDesktopMode().height) * 0.8f));
			m_window.create(sf::VideoMode((u32)windowSize.x, (u32)windowSize.y, sf::VideoMode::getDesktopMode().bitsPerPixel), "", sf::Style::Default, m_contextSettings);

			GLenum glewError = glewInit();
			if (GLEW_OK != glewError)
			{
				OutputDebugString(CSBackend::Windows::WindowsStringUtils::UTF8ToUTF16("[Chilli Source] Glew Error On Init : " + std::string((const char*)glewGetErrorString(glewError)) + "\n").c_str());
				exit(1);
			}

			sf::Clock clock;
			auto appStartTime = clock.getElapsedTime().asSeconds();
			auto appPreviousTime = appStartTime;

			ChilliSource::Application* app = CreateApplication();
			app->Init();
			app->Resume();
			app->Foreground();

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
							app->Quit();
							return;
						case sf::Event::Resized:
							m_windowResizeEvent.NotifyConnections(ChilliSource::Integer2(s32(event.size.width), s32(event.size.height)));
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
							ChilliSource::UTF8Char utf8Char = event.text.unicode;
							m_textEnteredEvent.NotifyConnections(utf8Char);
							break;
						}
					}
				}

				auto appCurrentTime = clock.getElapsedTime().asSeconds();

				auto deltaTime = (appCurrentTime - appPreviousTime);
				auto runningTime = (appPreviousTime - appStartTime);
				appPreviousTime = appCurrentTime;

				app->Update(deltaTime, TimeIntervalSecs(runningTime));
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
				ChilliSource::Application::Get()->Background();
				m_isFocused = false;
			}

			ChilliSource::Application::Get()->Suspend();
			ChilliSource::Application::Get()->Destroy();

			m_isRunning = false;
		}
	}
}

#endif
