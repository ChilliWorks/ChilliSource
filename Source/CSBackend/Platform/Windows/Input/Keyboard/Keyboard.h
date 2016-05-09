//
//  Keyboard.h
//  Chilli Source
//  Created by Scott Downie on 09/07/2014
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

#ifndef _CSBACKEND_PLATFORM_WINDOWS_INPUT_KEYBOARD_KEYBOARD_H_
#define _CSBACKEND_PLATFORM_WINDOWS_INPUT_KEYBOARD_KEYBOARD_H_

#include <ChilliSource/Input/Keyboard/Keyboard.h>
#include <ChilliSource/Input/Keyboard/KeyCode.h>
#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>

#include <array>

namespace CSBackend
{
	namespace Windows
	{
        //---------------------------------------------------------------
        /// A system for receiving input from the current keyboard
        ///
        /// @author S Downie
        //---------------------------------------------------------------
		class Keyboard final : public ChilliSource::Keyboard
		{
		public:
            CS_DECLARE_NAMEDTYPE(Keyboard);
            
			//----------------------------------------------------
			/// Queries whether or not this system implements the
			/// interface with the given ID.
			///
			/// @author S Downie
			///
			/// @param The interface Id.
			///
			/// @return Whether or not the interface is implemented.
			//----------------------------------------------------
			bool IsA(ChilliSource::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Check whether the key is currently down. This is
            /// unbuffered so will only check the state of the key
            /// at the moment in time when it is called
            ///
            /// @author S Downie
            ///
            /// @param Key code
            ///
            /// @return Whether the key is down
            //-------------------------------------------------------
			bool IsKeyDown(ChilliSource::KeyCode in_code) const override;
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
			ChilliSource::IConnectableEvent<KeyPressedDelegate>& GetKeyPressedEvent() override;
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
			ChilliSource::IConnectableEvent<KeyReleasedDelegate>& GetKeyReleasedEvent() override;
            
        private:
            
			friend ChilliSource::KeyboardUPtr ChilliSource::Keyboard::Create();
			//-------------------------------------------------------
			/// Constructor
			/// 
			/// @author S Downie
			//-------------------------------------------------------
			Keyboard() = default;
			//-------------------------------------------------------
			/// Called when the system is initialised and registers
			/// for SFML key events
			///
			/// @author S Downie
			//-------------------------------------------------------
			void OnInit() override;
			//-------------------------------------------------------
			/// Triggered by SFML when key is pressed
			///
			/// @param Key code
			/// @param Key event
			///
			/// @author S Downie
			//-------------------------------------------------------
			void OnKeyPressed(sf::Keyboard::Key in_code, const sf::Event::KeyEvent& in_event);
			//-------------------------------------------------------
			/// Triggered by SFML when key is released
			///
			/// @param Key code
			///
			/// @author S Downie
			//-------------------------------------------------------
			void OnKeyReleased(sf::Keyboard::Key in_code);
			//-------------------------------------------------------
			/// Called when the system is destroyed and unsubscribes
			/// from SFML key events
			///
			/// @author S Downie
			//-------------------------------------------------------
			void OnDestroy() override;

		private:

			ChilliSource::Event<KeyPressedDelegate> m_keyPressedEvent;
			ChilliSource::Event<KeyReleasedDelegate> m_keyReleasedEvent;

			ChilliSource::EventConnectionUPtr m_keyPressedConnection;
			ChilliSource::EventConnectionUPtr m_keyReleasedConnection;

			std::array<bool, static_cast<u32>(ChilliSource::KeyCode::k_total)> m_keysDown;
		};
	}
}

#endif

#endif


