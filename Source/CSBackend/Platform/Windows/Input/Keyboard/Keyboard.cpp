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

#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Input/Keyboard/KeyCode.h>
#include <ChilliSource/Input/Keyboard/ModifierKeyCode.h>

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
			const CSInput::KeyCode k_csKeyCodesMap[(u32)sf::Keyboard::Key::KeyCount] =
			{
				CSInput::KeyCode::k_a,
				CSInput::KeyCode::k_b,
				CSInput::KeyCode::k_c,
				CSInput::KeyCode::k_d,
				CSInput::KeyCode::k_e,
				CSInput::KeyCode::k_f,
				CSInput::KeyCode::k_g,
				CSInput::KeyCode::k_h,
				CSInput::KeyCode::k_i,
				CSInput::KeyCode::k_j,
				CSInput::KeyCode::k_k,
				CSInput::KeyCode::k_l,
				CSInput::KeyCode::k_m,
				CSInput::KeyCode::k_n,
				CSInput::KeyCode::k_o,
				CSInput::KeyCode::k_p,
				CSInput::KeyCode::k_q,
				CSInput::KeyCode::k_r,
				CSInput::KeyCode::k_s,
				CSInput::KeyCode::k_t,
				CSInput::KeyCode::k_u,
				CSInput::KeyCode::k_v,
				CSInput::KeyCode::k_w,
				CSInput::KeyCode::k_x,
				CSInput::KeyCode::k_y,
				CSInput::KeyCode::k_z,
				CSInput::KeyCode::k_num0,
				CSInput::KeyCode::k_num1,
				CSInput::KeyCode::k_num2,
				CSInput::KeyCode::k_num3,
				CSInput::KeyCode::k_num4,
				CSInput::KeyCode::k_num5,
				CSInput::KeyCode::k_num6,
				CSInput::KeyCode::k_num7,
				CSInput::KeyCode::k_num8,
				CSInput::KeyCode::k_num9,
				CSInput::KeyCode::k_escape,
				CSInput::KeyCode::k_leftCtrl,
				CSInput::KeyCode::k_leftShift,
				CSInput::KeyCode::k_leftAlt,
				CSInput::KeyCode::k_leftSystem,
				CSInput::KeyCode::k_rightCtrl,
				CSInput::KeyCode::k_rightShift,
				CSInput::KeyCode::k_rightAlt,
				CSInput::KeyCode::k_rightSystem,
				CSInput::KeyCode::k_menu,
				CSInput::KeyCode::k_leftBracket,
				CSInput::KeyCode::k_rightBracket,
				CSInput::KeyCode::k_semiColon,
				CSInput::KeyCode::k_comma,
				CSInput::KeyCode::k_period,
				CSInput::KeyCode::k_quote,
				CSInput::KeyCode::k_forwardSlash,
				CSInput::KeyCode::k_backSlash,
				CSInput::KeyCode::k_tilde,
				CSInput::KeyCode::k_equals,
				CSInput::KeyCode::k_hyphen,
				CSInput::KeyCode::k_space,
				CSInput::KeyCode::k_return,
				CSInput::KeyCode::k_backSpace,
				CSInput::KeyCode::k_tab,
				CSInput::KeyCode::k_pageUp,
				CSInput::KeyCode::k_pageDown,
				CSInput::KeyCode::k_end,
				CSInput::KeyCode::k_home,
				CSInput::KeyCode::k_insert,
				CSInput::KeyCode::k_delete,
				CSInput::KeyCode::k_add,
				CSInput::KeyCode::k_subtract,
				CSInput::KeyCode::k_multiply,
				CSInput::KeyCode::k_divide,
				CSInput::KeyCode::k_left,
				CSInput::KeyCode::k_right,
				CSInput::KeyCode::k_up,
				CSInput::KeyCode::k_down,
				CSInput::KeyCode::k_numpad0,
				CSInput::KeyCode::k_numpad1,
				CSInput::KeyCode::k_numpad2,
				CSInput::KeyCode::k_numpad3,
				CSInput::KeyCode::k_numpad4,
				CSInput::KeyCode::k_numpad5,
				CSInput::KeyCode::k_numpad6,
				CSInput::KeyCode::k_numpad7,
				CSInput::KeyCode::k_numpad8,
				CSInput::KeyCode::k_numpad9,
				CSInput::KeyCode::k_f1,
				CSInput::KeyCode::k_f2,
				CSInput::KeyCode::k_f3,
				CSInput::KeyCode::k_f4,
				CSInput::KeyCode::k_f5,
				CSInput::KeyCode::k_f6,
				CSInput::KeyCode::k_f7,
				CSInput::KeyCode::k_f8,
				CSInput::KeyCode::k_f9,
				CSInput::KeyCode::k_f10,
				CSInput::KeyCode::k_f11,
				CSInput::KeyCode::k_f12,
				CSInput::KeyCode::k_f13,
				CSInput::KeyCode::k_f14,
				CSInput::KeyCode::k_f15,
				CSInput::KeyCode::k_pause
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
			//-------------------------------------------------------
			/// Convert from SFML key code to CS key code
			///
			/// @author S Downie
			///
			/// @param SFML key code
			///
			/// @return CS key code
			//-------------------------------------------------------
			CSInput::KeyCode SFMLKeyCodeToCSKeyCode(sf::Keyboard::Key in_code)
			{
				if (in_code == sf::Keyboard::Key::Unknown)
				{
					return CSInput::KeyCode::k_unknown;
				}

				return k_csKeyCodesMap[(u32)in_code];
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
		void Keyboard::OnInit()
		{
			m_keyPressedConnection = SFMLWindow::Get()->GetKeyPressedEvent().OpenConnection(CSCore::MakeDelegate(this, &Keyboard::OnKeyPressed));
			m_keyReleasedConnection = SFMLWindow::Get()->GetKeyReleasedEvent().OpenConnection(CSCore::MakeDelegate(this, &Keyboard::OnKeyReleased));
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnKeyPressed(sf::Keyboard::Key in_code, const sf::Event::KeyEvent& in_event)
		{
			std::vector<CSInput::ModifierKeyCode> modifiers;
			modifiers.reserve((u32)CSInput::ModifierKeyCode::k_total);

			if (in_event.alt == true)
			{
				modifiers.push_back(CSInput::ModifierKeyCode::k_alt);
			}
			if (in_event.control == true)
			{
				modifiers.push_back(CSInput::ModifierKeyCode::k_ctrl);
			}
			if (in_event.shift == true)
			{
				modifiers.push_back(CSInput::ModifierKeyCode::k_shift);
			}
			if (in_event.system == true)
			{
				modifiers.push_back(CSInput::ModifierKeyCode::k_system);
			}

			m_keyPressedEvent.NotifyConnections(SFMLKeyCodeToCSKeyCode(in_code), modifiers);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnKeyReleased(sf::Keyboard::Key in_code)
		{
			m_keyReleasedEvent.NotifyConnections(SFMLKeyCodeToCSKeyCode(in_code));
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool Keyboard::IsKeyDown(CSInput::KeyCode in_code) const
		{
			return sf::Keyboard::isKeyPressed(CSKeyCodeToSFMLKeyCode(in_code)) == true;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<Keyboard::KeyPressedDelegate>& Keyboard::GetKeyPressedEvent()
		{
			return m_keyPressedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		CSCore::IConnectableEvent<Keyboard::KeyReleasedDelegate>& Keyboard::GetKeyReleasedEvent()
		{
			return m_keyReleasedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnDestroy()
		{
			m_keyPressedConnection.reset();
			m_keyReleasedConnection.reset();
		}
	}
}

#endif