//
//  SFMLWindow.h
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

#ifndef _CSBACKEND_PLATFORM_WINDOWS_SFML_BASE_SFMLWINDOW_H_
#define _CSBACKEND_PLATFORM_WINDOWS_SFML_BASE_SFMLWINDOW_H_

#include <CSBackend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Singleton.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>

#include <SFML/Window.hpp>

#include <functional>

namespace CSBackend
{
	namespace Windows
	{
		//---------------------------------------------------
		/// System that allows access to SFML and its window
		///
		/// @author S Downie
		//---------------------------------------------------
		class SFMLWindow final : public ChilliSource::Singleton<SFMLWindow>
		{
		public:
			//-----------------------------------------------------------
			/// Window display modes
			///
			/// @author S Downie
			//-----------------------------------------------------------
			enum class DisplayMode
			{
				k_windowed,
				k_fullscreen
			};
			//-----------------------------------------------------------
			/// A delegate called when the window size changes.
			///
			/// @author S Downie
			///
			/// @param The new window size.
			//-----------------------------------------------------------
			using WindowResizeDelegate = std::function<void(const ChilliSource::Integer2&)>;
			//-----------------------------------------------------------
			/// A delegate called when the window mode changes.
			///
			/// @author S Downie
			///
			/// @param The new window mode.
			//-----------------------------------------------------------
			using WindowDisplayModeDelegate = std::function<void(DisplayMode)>;
			//-----------------------------------------------------------
			/// List of the events that can occur on a mouse button
			///
			/// @author S Downie
			//-----------------------------------------------------------
			enum class MouseButtonEvent
			{
				k_pressed,
				k_released
			};
			//-----------------------------------------------------------
			/// A delegate called when a mouse button event occurs.
			///
			/// @author S Downie
			///
			/// @param Mouse button that the action occurred on
			/// @param The event type (Pressed/Released)
			/// @param Mouse position X
			/// @param Mouse position Y
			//-----------------------------------------------------------
			using MouseButtonDelegate = std::function<void(sf::Mouse::Button, MouseButtonEvent, s32, s32)>;
			//-----------------------------------------------------------
			/// A delegate called when a mouse move event occurs.
			///
			/// @author S Downie
			///
			/// @param Mouse position X
			/// @param Mouse position Y
			//-----------------------------------------------------------
			using MouseMovedDelegate = std::function<void(s32, s32)>;
			//-----------------------------------------------------------
			/// A delegate called when a mouse wheel event occurs.
			///
			/// @author S Downie
			///
			/// @param The number of ticks the wheel has moved in the y-axis
			//-----------------------------------------------------------
			using MouseWheelDelegate = std::function<void(s32)>;
			//-----------------------------------------------------------
			/// A delegate called when a text is entered via user input.
			///
			/// @author S Downie
			///
			/// @param UTF-8 character entered
			//-----------------------------------------------------------
			using TextEnteredEvent = std::function<void(ChilliSource::UTF8Char)>;
			//-------------------------------------------------------
			/// Delegate that receieves events on the key with the
			/// given code when key is pressed
			///
			/// @author S Downie
			///
			/// @param Key code
			/// @param Event
			//-------------------------------------------------------
			using KeyPressedDelegate = std::function<void(sf::Keyboard::Key, const sf::Event::KeyEvent&)>;
			//-------------------------------------------------------
			/// Delegate that receieves events on the key with the
			/// given code when key is released
			///
			/// @author S Downie
			///
			/// @param Key code
			//-------------------------------------------------------
			using KeyReleasedDelegate = std::function<void(sf::Keyboard::Key)>;
			//-------------------------------------------------
			/// Create and begin running the SFML window which in turn
			/// will update and render the app
			///
			/// @author S Downie
			//-------------------------------------------------
			void Run();
			//-------------------------------------------------
			/// Set the desired frame limit for the SFML update
			/// and refresh. This is not completely accurate
			///
			/// @author S Downie
			///
			/// @param Frame per second limit
			//-------------------------------------------------
			void SetPreferredFPS(u32 in_fps);
			//-------------------------------------------------
			/// Enable or disable VSync
			///
			/// @author S Downie
			///
			/// @param Whether to enable or disable
			//-------------------------------------------------
			void SetVSyncEnabled(bool in_enable);
			//-------------------------------------------------
			/// Set the size of the window in pixels.
			///
			/// @author S Downie
			///
			/// @param Size in pixels
			//-------------------------------------------------
			void SetSize(const ChilliSource::Integer2& in_size);
			//-------------------------------------------------
			/// Set the window to fullscreen mode or windowed mode
			/// which will hide or the menu bar
			///
			/// @author S Downie
			///
			/// @param Window mode
			//-------------------------------------------------
			void SetDisplayMode(DisplayMode in_mode);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return A list of resolutions supported by the display
			//----------------------------------------------------------
			std::vector<ChilliSource::Integer2> GetSupportedResolutions() const;
			//-------------------------------------------------
			/// Flush to the display. Should be called at end
			/// if each frame
			///
			/// @author S Downie
			//-------------------------------------------------
			void Display();
			//----------------------------------------------------
			/// Hide the window cursor
			///
			/// @author S Downie
			//----------------------------------------------------
			void HideCursor();
			//----------------------------------------------------
			/// Show the window cursor
			///
			/// @author S Downie
			//----------------------------------------------------
			void ShowCursor();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when the window is resized
			//------------------------------------------------
			ChilliSource::IConnectableEvent<WindowResizeDelegate>& GetWindowResizedEvent();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when the window fullscreen is enabled or disabled
			//-------------------------------------------------
			ChilliSource::IConnectableEvent<WindowDisplayModeDelegate>& GetWindowDisplayModeEvent();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when a mouse button event occurs
			//------------------------------------------------
			ChilliSource::IConnectableEvent<MouseButtonDelegate>& GetMouseButtonEvent();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when the mouse moves
			//------------------------------------------------
			ChilliSource::IConnectableEvent<MouseMovedDelegate>& GetMouseMovedEvent();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when the mouse wheel scrolls
			//------------------------------------------------
			ChilliSource::IConnectableEvent<MouseWheelDelegate>& GetMouseWheelEvent();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when text is entered
			//------------------------------------------------
			ChilliSource::IConnectableEvent<TextEnteredEvent>& GetTextEnteredEvent();
			//-------------------------------------------------------
			/// Get the event that is triggered whenever a key is pressed.
			///
			/// This event is guaranteed and should be used for low
			/// frequency events such as catching a confirm enter press.
			/// The polling "IsDown" method should be used for realtime
			/// events such as moving characters on arrow press, etc.
			///
			/// The event also returns the current state of the modifier
			/// keys (Ctrl, Alt, Shift, etc.)
			///
			/// @author S Downie
			///
			/// @return Event to register for key presses
			//-------------------------------------------------------
			ChilliSource::IConnectableEvent<KeyPressedDelegate>& GetKeyPressedEvent();
			//-------------------------------------------------------
			/// Get the event that is triggered whenever a key is released.
			///
			/// This event is guaranteed and should be used for low
			/// frequency events. The polling "IsUp" method should be
			/// used for realtime events.
			///
			/// @author S Downie
			///
			/// @return Event to register for key releases
			//-------------------------------------------------------
			ChilliSource::IConnectableEvent<KeyReleasedDelegate>& GetKeyReleasedEvent();
			//------------------------------------------------
			/// @author S Downie
			///
			/// @return Current size of the SFML window
			//------------------------------------------------
			ChilliSource::Integer2 GetWindowSize() const;
			//------------------------------------------------
			/// @author S Downie
			///
			/// @return Current handle of the SFML window
			//------------------------------------------------
			sf::WindowHandle GetWindowHandle() const;
			//------------------------------------------------
			/// @author S Downie
			///
			/// @return Current position of mouse relative to Window
			//------------------------------------------------
			ChilliSource::Integer2 GetMousePosition() const;
			//-------------------------------------------------
			/// Stops the update loop causing the application 
			/// to terminate.
			///
			/// @author S Downie
			//-------------------------------------------------
			void Quit();

		private:

			//-------------------------------------------------
			/// Recreate the window in fullscreen state
			///
			/// @author S Downie
			//-------------------------------------------------
			void SetFullscreen();
			//-------------------------------------------------
			/// Recreate the window in windowed state
			///
			/// @author S Downie
			//-------------------------------------------------
			void SetWindowed();

		private:

			sf::Window m_window;

			ChilliSource::Event<WindowResizeDelegate> m_windowResizeEvent;
			ChilliSource::Event<WindowDisplayModeDelegate> m_windowDisplayModeEvent;
			ChilliSource::Event<MouseButtonDelegate> m_mouseButtonEvent;
			ChilliSource::Event<MouseMovedDelegate> m_mouseMovedEvent;
			ChilliSource::Event<MouseWheelDelegate> m_mouseWheelEvent;
			ChilliSource::Event<TextEnteredEvent> m_textEnteredEvent;
			ChilliSource::Event<KeyPressedDelegate> m_keyPressedEvent;
			ChilliSource::Event<KeyReleasedDelegate> m_keyReleasedEvent;

			std::string m_title;

			sf::ContextSettings m_contextSettings;

			u32 m_preferredRGBADepth = 32;
			u32 m_preferredFPS = 0;

			bool m_isRunning = true;
			bool m_isFocused = true;
			DisplayMode m_displayMode = DisplayMode::k_windowed;
		};
	}
}

#endif

#endif
