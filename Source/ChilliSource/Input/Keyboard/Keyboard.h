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

#ifndef _CHILLISOURCE_INPUT_KEYBOARD_KEYBOARD_H_
#define _CHILLISOURCE_INPUT_KEYBOARD_KEYBOARD_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <functional>
#include <vector>

namespace ChilliSource
{
	namespace Input
	{
        //---------------------------------------------------------------
        /// A system for receiving input from the current keyboard
        ///
        /// @author S Downie
        //---------------------------------------------------------------
		class Keyboard : public Core::AppSystem
		{
		public:
            CS_DECLARE_NAMEDTYPE(Keyboard);
            
            //-------------------------------------------------------
            /// Delegate that receieves events on the key with the
            /// given code when key is pressed
            ///
            /// @author S Downie
            ///
            /// @param Key code
            /// @param List of modifier keys that are down
            //-------------------------------------------------------
            using KeyPressedDelegate = std::function<void(KeyCode, const std::vector<ModifierKeyCode>&)>;
            //-------------------------------------------------------
            /// Delegate that receieves events on the key with the
            /// given code when key is released
            ///
            /// @author S Downie
            ///
            /// @param Key code
            //-------------------------------------------------------
            using KeyReleasedDelegate = std::function<void(KeyCode)>;
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
            virtual bool IsKeyDown(KeyCode in_code) const = 0;
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
            virtual Core::IConnectableEvent<KeyPressedDelegate>& GetKeyPressedEvent() = 0;
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
            virtual Core::IConnectableEvent<KeyReleasedDelegate>& GetKeyReleasedEvent() = 0;
			//-------------------------------------------------------
			/// Virtual destructor
			///
			/// @author S Downie
			//-------------------------------------------------------
			virtual ~Keyboard(){}
            
        protected:
            
            friend class Core::Application;
            //-------------------------------------------------------
            /// Factory method from creating a new platform specific
            /// instance of the keyboard system.
            ///
            /// @author Ian Copland
            ///
            /// @return The new instance of the system.
            //-------------------------------------------------------
			static KeyboardUPtr Create();
		};
	}
}

#endif


