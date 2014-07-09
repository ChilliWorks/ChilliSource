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
		class SFMLWindow final : public CSCore::Singleton<SFMLWindow>
		{
		public:
			//-----------------------------------------------------------
			/// A delegate called when the window size changes.
			///
			/// @author S Downie
			///
			/// @param The new window size.
			//-----------------------------------------------------------
			using WindowResizeDelegate = std::function<void(const CSCore::Integer2&)>;
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
			/// A delegate called when a text is entered via user input.
			///
			/// @author S Downie
			///
			/// @param UTF-8 character entered
			//-----------------------------------------------------------
			using TextEnteredEvent = std::function<void(CSCore::UTF8Char)>;
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
			CSCore::IConnectableEvent<WindowResizeDelegate>& GetWindowResizedEvent();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when a mouse button event occurs
			//------------------------------------------------
			CSCore::IConnectableEvent<MouseButtonDelegate>& GetMouseButtonEvent();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when the mouse moves
			//------------------------------------------------
			CSCore::IConnectableEvent<MouseMovedDelegate>& GetMouseMovedEvent();
			//-------------------------------------------------
			/// @author S Downie
			///
			/// @return An event that is called when text is entered
			//------------------------------------------------
			CSCore::IConnectableEvent<TextEnteredEvent>& GetTextEnteredEvent();
			//------------------------------------------------
			/// @author S Downie
			///
			/// @return Current size of the SFML window
			//------------------------------------------------
			CSCore::Integer2 GetWindowSize() const;
			//------------------------------------------------
			/// @author S Downie
			///
			/// @return Current position of mouse relative to Window
			//------------------------------------------------
			CSCore::Integer2 GetMousePosition() const;
			//-------------------------------------------------
			/// Stops the update loop causing the application 
			/// to terminate.
			///
			/// @author S Downie
			//-------------------------------------------------
			void Quit();

		private:

			sf::Window m_window;

			CSCore::Event<WindowResizeDelegate> m_windowResizeEvent;
			CSCore::Event<MouseButtonDelegate> m_mouseButtonEvent;
			CSCore::Event<MouseMovedDelegate> m_mouseMovedEvent;
			CSCore::Event<TextEnteredEvent> m_textEnteredEvent;

			bool m_isSuspended = false;
			bool m_isFocused = true;
		};
	}
}

#endif

#endif
