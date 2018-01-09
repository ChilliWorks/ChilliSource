//
//  SFMLWindow.h
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

#ifndef _CSBACKEND_PLATFORM_WINDOWS_SFML_BASE_SFMLWINDOW_H_
#define _CSBACKEND_PLATFORM_WINDOWS_SFML_BASE_SFMLWINDOW_H_

#include <CSBackend/Platform/Windows/ForwardDeclarations.h>
#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/LifeCycleManager.h>
#include <ChilliSource/Core/Base/Singleton.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>

#include <SFML/Window.hpp>

#include <functional>
#include <mutex>

namespace CSBackend
{
	namespace Windows
	{
		//-----------------------------------------------------------
		/// System that allows access to SFML and its window
        ///
        /// While some of the methods in this class are thread-safe,
        /// most should be called on the system thread.
		///
		/// @author S Downie
		//-----------------------------------------------------------
		class SFMLWindow final : public ChilliSource::Singleton<SFMLWindow>
		{
		public:

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
			using WindowDisplayModeDelegate = std::function<void(ChilliSource::Screen::DisplayMode)>;
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
			using TextEnteredDelegate = std::function<void(ChilliSource::UTF8Char)>;
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

			/// Delegate that receieves events when a new joystick is attached or removed
			///
			/// @param Index of joystick
			///
			using JoystickConnectionChangedDelegate = std::function<void(u32)>;

			/// Delegate that receieves events when a button is pressed or released
			///
			/// @param Index of joystick
			/// @param Index of button
			///
			using JoystickButtonDelegate = std::function<void(u32, u32)>;

			/// Delegate that receieves events when a button is pressed or released
			///
			/// @param Index of joystick
			/// @param Id of axis
			/// @param Position of axis
			///
			using JoystickMovedDelegate = std::function<void(u32, sf::Joystick::Axis, f32)>;

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
			/// @param Window size
			/// @param TRUE to force set even if the same as previous
			//-------------------------------------------------
			void SetDisplayMode(ChilliSource::Screen::DisplayMode in_mode, const ChilliSource::Integer2& size, bool force = false);
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @return A list of resolutions supported by the display
			//----------------------------------------------------------
			std::vector<ChilliSource::Integer2> GetSupportedFullscreenResolutions() const;
			//----------------------------------------------------
			/// Hide the window cursor
			///
			/// @author S Downie
			//----------------------------------------------------
			void HideSystemCursor();
			//----------------------------------------------------
			/// Show the window cursor
			///
			/// @author S Downie
			//----------------------------------------------------
			void ShowSystemCursor();
			//-------------------------------------------------
            /// Set the delegates that relate to window events.
            /// This method will assert if a given delegate is null
            /// or if the delegate has already been set.
            ///
            /// This method is thread-safe.
            ///
            /// @author Jordan Brown
            ///
            /// @param in_windowResizeDelegate - The delegate to be
            /// called when the window is resized.
            /// @param in_windowDisplayModeDelegate - The delegate to be
            /// called when the window's display mode is changed.
            //-------------------------------------------------------
            void SetWindowDelegates(const WindowResizeDelegate& in_windowResizeDelegate, const WindowDisplayModeDelegate& in_windowDisplayModeDelegate) noexcept;
            //-------------------------------------------------------
            /// Set the delegates that relate to mouse events.
            /// This method will assert if a given delegate is null
            /// or if the delegate has already been set.
            ///
            /// This method is thread-safe.
            ///
            /// @author Jordan Brown
            ///
            /// @param in_mouseButtonDelegate - The delegate to be
            /// called when a mouse button event occurs.
            /// @param in_mouseMovedDelegate - The delegate to be
            /// called when the mouse is moved.
            /// @param in_mouseWheelDelegate - The delegate to be
            /// called when the mouse wheel is scrolled.
            //-------------------------------------------------------
            void SetMouseDelegates(const MouseButtonDelegate& in_mouseButtonDelegate, const MouseMovedDelegate& in_mouseMovedDelegate, const MouseWheelDelegate& in_mouseWheelDelegate) noexcept;
            //-------------------------------------------------------
            /// Set the delegate that is called when text is entered.
            /// This method will assert if a given delegate is null
            /// or if the delegate has already been set.
            ///
            /// This method is thread-safe.
            ///
            /// @author Jordan Brown
            ///
            /// @param in_textEnteredDelegate - The delegate to be
            /// called.
            //-------------------------------------------------------
            void SetTextEnteredDelegate(const TextEnteredDelegate& in_textEnteredDelegate) noexcept;
            //-------------------------------------------------------
            /// Set the delegates that relate to keyboard events.
            /// This method will assert if a given delegate is null
            /// or if the delegate has already been set.
            ///
            /// This method is thread-safe.
            ///
            /// @author Jordan Brown
            ///
            /// @param in_keyPressedDelegate - The delegate to be
            /// called when a key is pressed.
            /// @param in_keyReleasedDelegate - The delegate to be
            /// called when a key is released.
            //-------------------------------------------------------
            void SetKeyDelegates(const KeyPressedDelegate& in_keyPressedDelegate, const KeyReleasedDelegate& in_keyReleasedDelegate) noexcept;

			/// Set the delegates that relate to joystick events.
			/// This method will assert if a given delegate is null
			/// or if the delegate has already been set.
			///
			/// This method is thread-safe.
			///
			/// @param connectedDelegate
			///		Called when a new joystick is attached
			/// @param disconnectedDelegate
			///		Called when an existing joystick is removed
			/// @param buttonPressedDelegate
			///		Called when a button is pressed on a joystick
			/// @param buttonReleasedDelegate
			///		Called when a button is released on a joystick
			/// @param movedDelegate
			///		Called when an analogue stick is moved
			///
			void SetJoystickDelegates(JoystickConnectionChangedDelegate connectedDelegate, JoystickConnectionChangedDelegate disconnectedDelegate,
				JoystickButtonDelegate buttonPressedDelegate, JoystickButtonDelegate buttonReleasedDelegate, JoystickMovedDelegate movedDelegate) noexcept;

            //-------------------------------------------------------
            /// Remove the delegates that relate to window events.
            ///
            /// This method is thread-safe.
            ///
            /// @author Jordan Brown
            //-------------------------------------------------------
            void RemoveWindowDelegates() noexcept;
            //-------------------------------------------------------
            /// Remove the delegates that relate to mouse events.
            ///
            /// This method is thread-safe.
            ///
            /// @author Jordan Brown
            //-------------------------------------------------------
            void RemoveMouseDelegates() noexcept;
            //-------------------------------------------------------
            /// Remove the delegate that is called when text is entered.
            ///
            /// This method is thread-safe.
            ///
            /// @author Jordan Brown
            //-------------------------------------------------------
            void RemoveTextEnteredDelegate() noexcept;
            //-------------------------------------------------------
            /// Remove the delegates that relate to keyboard events.
            ///
            /// This method is thread-safe.
            ///
            /// @author Jordan Brown
            //-------------------------------------------------------
            void RemoveKeyDelegates() noexcept;

			/// Removes the delegates related to joystick events
			///
			/// This method is thread-safe
			///
			void RemoveJoystickDelegates() noexcept;

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
			/// Schedules a quit to occur at the end of the
			/// the current update.
			///
			/// @author HMcLaughlin
			//-------------------------------------------------
			void ScheduleQuit() noexcept { m_quitScheduled = true; }
            //-------------------------------------------------
            /// Destructor; makes sure that delegates have been
            /// properly un-set.
            ///
            /// @author Jordan Brown
            //-------------------------------------------------
            ~SFMLWindow() noexcept;

		private:

			//-------------------------------------------------
			/// Recreate the window in fullscreen state
			///
			/// @author S Downie
			///
			/// @param size of window
			//-------------------------------------------------
			void SetFullscreen(const ChilliSource::Integer2& size);
			//-------------------------------------------------
			/// Recreate the window in windowed state
			///
			/// @author S Downie
			///
			/// @param size of window
			//-------------------------------------------------
			void SetWindowed(const ChilliSource::Integer2& size);
			//-------------------------------------------------
			/// Stops the update loop causing the application
			/// to terminate.
			///
			/// @author S Downie
			//-------------------------------------------------
			void Quit();

		private:

			sf::Window m_window;

            WindowResizeDelegate m_windowResizeDelegate;
            WindowDisplayModeDelegate m_windowDisplayModeDelegate;
            MouseButtonDelegate m_mouseButtonDelegate;
            MouseMovedDelegate m_mouseMovedDelegate;
            MouseWheelDelegate m_mouseWheelDelegate;
            TextEnteredDelegate m_textEnteredDelegate;
            KeyPressedDelegate m_keyPressedDelegate;
            KeyReleasedDelegate m_keyReleasedDelegate;
			JoystickConnectionChangedDelegate m_joystickConnectedDelegate;
			JoystickConnectionChangedDelegate m_joystickDisconnectedDelegate;
			JoystickButtonDelegate m_joystickButtonPressedDelegate;
			JoystickButtonDelegate m_joystickButtonReleasedDelegate;
			JoystickMovedDelegate m_joystickMovedDelegate;

            std::mutex m_windowMutex;
            std::mutex m_mouseMutex;
            std::mutex m_textEntryMutex;
            std::mutex m_keyMutex;
			std::mutex m_joystickMutex;

			std::string m_title;

			sf::ContextSettings m_contextSettings;

			u32 m_preferredRGBADepth = 32;
			u32 m_preferredFPS = 0;
			ChilliSource::Integer2 m_desktopSize;

			bool m_isRunning = true;
			bool m_isFocused = true;
			bool m_quitScheduled = false;

			ChilliSource::Screen::DisplayMode m_displayMode = ChilliSource::Screen::DisplayMode::k_windowed;

            ChilliSource::LifecycleManagerUPtr m_lifecycleManager;
		};
	}
}

#endif

#endif
