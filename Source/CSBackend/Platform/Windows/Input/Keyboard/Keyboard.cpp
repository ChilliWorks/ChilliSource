//
//  Keyboard.cpp
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

#include <CSBackend/Platform/Windows/Input/Keyboard/Keyboard.h>

#include <ChilliSource/Input/Keyboard/KeyCode.h>
#include <CSBackend/Platform/Windows/SFML/Base/SFMLWindow.h>

namespace CSBackend
{
	namespace Windows
	{
		CS_DEFINE_NAMEDTYPE(Keyboard);

		namespace
		{
			const sf::Keyboard::Key k_sfmlKeyCodesMap[(u32)CSInput::KeyCode::k_total] = 
			{
				sf::Keyboard::Unknown,
				sf::Keyboard::A,
				sf::Keyboard::B,
				sf::Keyboard::C,
				sf::Keyboard::D,
				sf::Keyboard::E,
				sf::Keyboard::F,
				sf::Keyboard::G,
				sf::Keyboard::H,
				sf::Keyboard::I,
				sf::Keyboard::J,
				sf::Keyboard::K,
				sf::Keyboard::L,
				sf::Keyboard::M,
				sf::Keyboard::N,
				sf::Keyboard::O,
				sf::Keyboard::P,
				sf::Keyboard::Q,
				sf::Keyboard::R,
				sf::Keyboard::S,
				sf::Keyboard::T,
				sf::Keyboard::U,
				sf::Keyboard::V,
				sf::Keyboard::W,
				sf::Keyboard::X,
				sf::Keyboard::Y,
				sf::Keyboard::Z,
				sf::Keyboard::Num0,
				sf::Keyboard::Num1,
				sf::Keyboard::Num2,
				sf::Keyboard::Num3,
				sf::Keyboard::Num4,
				sf::Keyboard::Num5,
				sf::Keyboard::Num6,
				sf::Keyboard::Num7,
				sf::Keyboard::Num8,
				sf::Keyboard::Num9,
				sf::Keyboard::Escape,
				sf::Keyboard::LControl,
				sf::Keyboard::LShift,
				sf::Keyboard::LAlt,
				sf::Keyboard::LSystem,
				sf::Keyboard::RControl,
				sf::Keyboard::RShift,
				sf::Keyboard::RAlt,
				sf::Keyboard::RSystem,
				sf::Keyboard::Menu,
				sf::Keyboard::LBracket,
				sf::Keyboard::RBracket,
				sf::Keyboard::SemiColon,
				sf::Keyboard::Comma,
				sf::Keyboard::Period,
				sf::Keyboard::Quote,
				sf::Keyboard::Slash,
				sf::Keyboard::BackSlash,
				sf::Keyboard::Tilde,
				sf::Keyboard::Equal,
				sf::Keyboard::Dash,
				sf::Keyboard::Space,
				sf::Keyboard::Return,
				sf::Keyboard::BackSpace,
				sf::Keyboard::Tab,
				sf::Keyboard::PageUp,
				sf::Keyboard::PageDown,
				sf::Keyboard::End,
				sf::Keyboard::Home,
				sf::Keyboard::Insert,
				sf::Keyboard::Delete,
				sf::Keyboard::Add,
				sf::Keyboard::Subtract,
				sf::Keyboard::Multiply,
				sf::Keyboard::Divide,
				sf::Keyboard::Left,
				sf::Keyboard::Right,
				sf::Keyboard::Up,
				sf::Keyboard::Down,
				sf::Keyboard::Numpad0,
				sf::Keyboard::Numpad1,
				sf::Keyboard::Numpad2,
				sf::Keyboard::Numpad3,
				sf::Keyboard::Numpad4,
				sf::Keyboard::Numpad5,
				sf::Keyboard::Numpad6,
				sf::Keyboard::Numpad7,
				sf::Keyboard::Numpad8,
				sf::Keyboard::Numpad9,
				sf::Keyboard::F1,
				sf::Keyboard::F2,
				sf::Keyboard::F3,
				sf::Keyboard::F4,
				sf::Keyboard::F5,
				sf::Keyboard::F6,
				sf::Keyboard::F7,
				sf::Keyboard::F8,
				sf::Keyboard::F9,
				sf::Keyboard::F10,
				sf::Keyboard::F11,
				sf::Keyboard::F12,
				sf::Keyboard::F13,
				sf::Keyboard::F14,
				sf::Keyboard::F15,
				sf::Keyboard::Pause
			};
			//-------------------------------------------------------
			/// Convert from CS key code to SFML key code
			///
			/// @author S Downie
			///
			/// @param CS key code
			///
			/// @return SFML key code
			//-------------------------------------------------------
			sf::Keyboard::Key CSKeyCodeToSFMLKeyCode(CSInput::KeyCode in_code)
			{
				return k_sfmlKeyCodesMap[(u32)in_code];
			}
		}

		//----------------------------------------------------
		//----------------------------------------------------
		bool Keyboard::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return (CSInput::Keyboard::InterfaceID == in_interfaceId || Keyboard::InterfaceID == in_interfaceId);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool Keyboard::IsKeyDown(CSInput::KeyCode in_code) const
		{
			return sf::Keyboard::isKeyPressed(CSKeyCodeToSFMLKeyCode(in_code)) == true;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool Keyboard::IsKeyUp(CSInput::KeyCode in_code) const
		{
			return sf::Keyboard::isKeyPressed(CSKeyCodeToSFMLKeyCode(in_code)) == false;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<Keyboard::KeyDelegate>& Keyboard::GetKeyPressedEvent()
		{
			return m_keyPressedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<Keyboard::KeyDelegate>& Keyboard::GetKeyReleasedEvent()
		{
			return m_keyReleasedEvent;
		}
	}
}

#endif