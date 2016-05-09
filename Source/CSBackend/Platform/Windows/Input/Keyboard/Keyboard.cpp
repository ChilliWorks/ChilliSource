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
			const sf::Keyboard::Key k_sfmlKeyCodesMap[(u32)ChilliSource::KeyCode::k_total] = 
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
			const ChilliSource::KeyCode k_csKeyCodesMap[(u32)sf::Keyboard::Key::KeyCount] =
			{
				ChilliSource::KeyCode::k_a,
				ChilliSource::KeyCode::k_b,
				ChilliSource::KeyCode::k_c,
				ChilliSource::KeyCode::k_d,
				ChilliSource::KeyCode::k_e,
				ChilliSource::KeyCode::k_f,
				ChilliSource::KeyCode::k_g,
				ChilliSource::KeyCode::k_h,
				ChilliSource::KeyCode::k_i,
				ChilliSource::KeyCode::k_j,
				ChilliSource::KeyCode::k_k,
				ChilliSource::KeyCode::k_l,
				ChilliSource::KeyCode::k_m,
				ChilliSource::KeyCode::k_n,
				ChilliSource::KeyCode::k_o,
				ChilliSource::KeyCode::k_p,
				ChilliSource::KeyCode::k_q,
				ChilliSource::KeyCode::k_r,
				ChilliSource::KeyCode::k_s,
				ChilliSource::KeyCode::k_t,
				ChilliSource::KeyCode::k_u,
				ChilliSource::KeyCode::k_v,
				ChilliSource::KeyCode::k_w,
				ChilliSource::KeyCode::k_x,
				ChilliSource::KeyCode::k_y,
				ChilliSource::KeyCode::k_z,
				ChilliSource::KeyCode::k_num0,
				ChilliSource::KeyCode::k_num1,
				ChilliSource::KeyCode::k_num2,
				ChilliSource::KeyCode::k_num3,
				ChilliSource::KeyCode::k_num4,
				ChilliSource::KeyCode::k_num5,
				ChilliSource::KeyCode::k_num6,
				ChilliSource::KeyCode::k_num7,
				ChilliSource::KeyCode::k_num8,
				ChilliSource::KeyCode::k_num9,
				ChilliSource::KeyCode::k_escape,
				ChilliSource::KeyCode::k_leftCtrl,
				ChilliSource::KeyCode::k_leftShift,
				ChilliSource::KeyCode::k_leftAlt,
				ChilliSource::KeyCode::k_leftSystem,
				ChilliSource::KeyCode::k_rightCtrl,
				ChilliSource::KeyCode::k_rightShift,
				ChilliSource::KeyCode::k_rightAlt,
				ChilliSource::KeyCode::k_rightSystem,
				ChilliSource::KeyCode::k_menu,
				ChilliSource::KeyCode::k_leftBracket,
				ChilliSource::KeyCode::k_rightBracket,
				ChilliSource::KeyCode::k_semiColon,
				ChilliSource::KeyCode::k_comma,
				ChilliSource::KeyCode::k_period,
				ChilliSource::KeyCode::k_quote,
				ChilliSource::KeyCode::k_forwardSlash,
				ChilliSource::KeyCode::k_backSlash,
				ChilliSource::KeyCode::k_tilde,
				ChilliSource::KeyCode::k_equals,
				ChilliSource::KeyCode::k_hyphen,
				ChilliSource::KeyCode::k_space,
				ChilliSource::KeyCode::k_return,
				ChilliSource::KeyCode::k_backSpace,
				ChilliSource::KeyCode::k_tab,
				ChilliSource::KeyCode::k_pageUp,
				ChilliSource::KeyCode::k_pageDown,
				ChilliSource::KeyCode::k_end,
				ChilliSource::KeyCode::k_home,
				ChilliSource::KeyCode::k_insert,
				ChilliSource::KeyCode::k_delete,
				ChilliSource::KeyCode::k_add,
				ChilliSource::KeyCode::k_subtract,
				ChilliSource::KeyCode::k_multiply,
				ChilliSource::KeyCode::k_divide,
				ChilliSource::KeyCode::k_left,
				ChilliSource::KeyCode::k_right,
				ChilliSource::KeyCode::k_up,
				ChilliSource::KeyCode::k_down,
				ChilliSource::KeyCode::k_numpad0,
				ChilliSource::KeyCode::k_numpad1,
				ChilliSource::KeyCode::k_numpad2,
				ChilliSource::KeyCode::k_numpad3,
				ChilliSource::KeyCode::k_numpad4,
				ChilliSource::KeyCode::k_numpad5,
				ChilliSource::KeyCode::k_numpad6,
				ChilliSource::KeyCode::k_numpad7,
				ChilliSource::KeyCode::k_numpad8,
				ChilliSource::KeyCode::k_numpad9,
				ChilliSource::KeyCode::k_f1,
				ChilliSource::KeyCode::k_f2,
				ChilliSource::KeyCode::k_f3,
				ChilliSource::KeyCode::k_f4,
				ChilliSource::KeyCode::k_f5,
				ChilliSource::KeyCode::k_f6,
				ChilliSource::KeyCode::k_f7,
				ChilliSource::KeyCode::k_f8,
				ChilliSource::KeyCode::k_f9,
				ChilliSource::KeyCode::k_f10,
				ChilliSource::KeyCode::k_f11,
				ChilliSource::KeyCode::k_f12,
				ChilliSource::KeyCode::k_f13,
				ChilliSource::KeyCode::k_f14,
				ChilliSource::KeyCode::k_f15,
				ChilliSource::KeyCode::k_pause
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
			sf::Keyboard::Key CSKeyCodeToSFMLKeyCode(ChilliSource::KeyCode in_code)
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
			ChilliSource::KeyCode SFMLKeyCodeToCSKeyCode(sf::Keyboard::Key in_code)
			{
				if (in_code == sf::Keyboard::Key::Unknown)
				{
					return ChilliSource::KeyCode::k_unknown;
				}

				return k_csKeyCodesMap[(u32)in_code];
			}
		}

		//----------------------------------------------------
		//----------------------------------------------------
		bool Keyboard::IsA(ChilliSource::InterfaceIDType in_interfaceId) const
		{
			return (ChilliSource::Keyboard::InterfaceID == in_interfaceId || Keyboard::InterfaceID == in_interfaceId);
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnInit()
		{
			for (auto& flag : m_keysDown)
			{
				flag = false;
			}

			m_keyPressedConnection = SFMLWindow::Get()->GetKeyPressedEvent().OpenConnection(ChilliSource::MakeDelegate(this, &Keyboard::OnKeyPressed));
			m_keyReleasedConnection = SFMLWindow::Get()->GetKeyReleasedEvent().OpenConnection(ChilliSource::MakeDelegate(this, &Keyboard::OnKeyReleased));
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnKeyPressed(sf::Keyboard::Key in_code, const sf::Event::KeyEvent& in_event)
		{
			auto keyCode = SFMLKeyCodeToCSKeyCode(in_code);

			if (IsKeyDown(keyCode) == false)
			{
				m_keysDown[static_cast<u32>(keyCode)] = true;

				std::vector<ChilliSource::ModifierKeyCode> modifiers;
				modifiers.reserve((u32)ChilliSource::ModifierKeyCode::k_total);

				if (in_event.alt == true)
				{
					modifiers.push_back(ChilliSource::ModifierKeyCode::k_alt);
				}
				if (in_event.control == true)
				{
					modifiers.push_back(ChilliSource::ModifierKeyCode::k_ctrl);
				}
				if (in_event.shift == true)
				{
					modifiers.push_back(ChilliSource::ModifierKeyCode::k_shift);
				}
				if (in_event.system == true)
				{
					modifiers.push_back(ChilliSource::ModifierKeyCode::k_system);
				}

				m_keyPressedEvent.NotifyConnections(keyCode, modifiers);
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnKeyReleased(sf::Keyboard::Key in_code)
		{
			auto keyCode = SFMLKeyCodeToCSKeyCode(in_code);
			if (IsKeyDown(keyCode) == true)
			{
				m_keysDown[static_cast<u32>(keyCode)] = false;

				m_keyReleasedEvent.NotifyConnections(keyCode);
			}
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		bool Keyboard::IsKeyDown(ChilliSource::KeyCode in_code) const
		{
			return m_keysDown[static_cast<u32>(in_code)];
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ChilliSource::IConnectableEvent<Keyboard::KeyPressedDelegate>& Keyboard::GetKeyPressedEvent()
		{
			return m_keyPressedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		ChilliSource::IConnectableEvent<Keyboard::KeyReleasedDelegate>& Keyboard::GetKeyReleasedEvent()
		{
			return m_keyReleasedEvent;
		}
		//-------------------------------------------------------
		//-------------------------------------------------------
		void Keyboard::OnDestroy()
		{
			m_keyPressedConnection.reset();
			m_keyReleasedConnection.reset();

			for (auto& flag : m_keysDown)
			{
				flag = false;
			}
		}
	}
}

#endif