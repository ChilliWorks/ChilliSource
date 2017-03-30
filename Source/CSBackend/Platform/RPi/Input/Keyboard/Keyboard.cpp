//
//  The MIT License (MIT)
//
//  Copyright (c) 2017 Tag Games Limited
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

#ifdef CS_TARGETPLATFORM_RPI

#include <CSBackend/Platform/RPi/Input/Keyboard/Keyboard.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Input/Keyboard/KeyCode.h>
#include <ChilliSource/Input/Keyboard/ModifierKeyCode.h>

namespace CSBackend
{
	namespace RPi
	{
		CS_DEFINE_NAMEDTYPE(Keyboard);

		namespace
		{
			const u32 k_x11keyCodesMap[(u32)ChilliSource::KeyCode::k_total] =
			{
				XK_VoidSymbol,
				XK_a,
				XK_b,
				XK_c,
				XK_d,
				XK_e,
				XK_f,
				XK_g,
				XK_h,
				XK_i,
				XK_j,
				XK_k,
				XK_l,
				XK_m,
				XK_n,
				XK_o,
				XK_p,
				XK_q,
				XK_r,
				XK_s,
				XK_t,
				XK_u,
				XK_v,
				XK_w,
				XK_x,
				XK_y,
				XK_z,
				XK_0,
				XK_1,
				XK_2,
				XK_3,
				XK_4,
				XK_5,
				XK_6,
				XK_7,
				XK_8,
				XK_9,
				XK_Escape,
				XK_Control_L,
				XK_Shift_L,
				XK_Alt_L,
				XK_Super_L,
				XK_Control_R,
				XK_Shift_R,
				XK_Alt_R,
				XK_Super_R,
				XK_Menu,
				XK_bracketleft,
				XK_bracketright,
				XK_semicolon,
				XK_comma,
				XK_period,
				XK_apostrophe,
				XK_slash,
				XK_backslash,
				XK_asciitilde,
				XK_equal,
				XK_minus,
				XK_space,
				XK_Return,
				XK_BackSpace,
				XK_Tab,
				XK_Page_Up,
				XK_Page_Down,
				XK_End,
				XK_Home,
				XK_Insert,
				XK_Delete,
				XK_KP_Add,
				XK_KP_Subtract,
				XK_KP_Multiply,
				XK_KP_Divide,
				XK_Left,
				XK_Right,
				XK_Up,
				XK_Down,
				XK_KP_Insert, //XK_KP_0
				XK_KP_End, //XK_KP_1
				XK_KP_Down, //XK_KP_1
				XK_KP_Page_Down, //XK_KP_1
				XK_KP_Left, //XK_KP_1
				XK_KP_Begin, //XK_KP_1
				XK_KP_Right, //XK_KP_1
				XK_KP_Home, //XK_KP_1
				XK_KP_Up, //XK_KP_1
				XK_KP_Page_Up, //XK_KP_1
				XK_F1,
				XK_F2,
				XK_F3,
				XK_F4,
				XK_F5,
				XK_F6,
				XK_F7,
				XK_F8,
				XK_F9,
				XK_F10,
				XK_F11,
				XK_F12,
				XK_F13,
				XK_F14,
				XK_F15,
				XK_Pause
			};

			const ChilliSource::KeyCode k_csKeyCodesMap[(u32)ChilliSource::KeyCode::k_total] =
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
		}

		/// Convert from CS key code to X11 keysym
		///
		/// @param CS key code
		///
		/// @return X11 keysym
		///
		u32 CSKeyCodeToX11KeySym(ChilliSource::KeyCode in_code)
		{
			return k_x11keyCodesMap[(u32)in_code];
		}

		/// Convert from X11 keysym to CS key code
		///
		/// @param X11 keysym
		///
		/// @return CS key code
		///
		ChilliSource::KeyCode X11KeySymToCSKeyCode(u32 in_keysym)
		{
			u32 foundKey = (u32)ChilliSource::KeyCode::k_total;

			for(u32 keysymIndex = 1; keysymIndex < (u32)ChilliSource::KeyCode::k_total; keysymIndex++)
			{
				if(k_x11keyCodesMap[keysymIndex] == in_keysym)
				{
					foundKey = keysymIndex - 1;
					break;
				}
			}

			if (in_keysym == XK_VoidSymbol || foundKey >= (u32)ChilliSource::KeyCode::k_total)
			{
				return ChilliSource::KeyCode::k_unknown;
			}

			return k_csKeyCodesMap[(u32)foundKey];
		}

		//---------------------------------------------------------------------------------
		bool Keyboard::IsA(ChilliSource::InterfaceIDType interfaceId) const
		{
			return (ChilliSource::Keyboard::InterfaceID == interfaceId || Keyboard::InterfaceID == interfaceId);
		}

		//---------------------------------------------------------------------------------
		ChilliSource::IConnectableEvent<Keyboard::KeyPressedDelegate>& Keyboard::GetKeyPressedEvent()
		{
			return m_keyPressedEvent;
		}

		//---------------------------------------------------------------------------------
		ChilliSource::IConnectableEvent<Keyboard::KeyReleasedDelegate>& Keyboard::GetKeyReleasedEvent()
		{
			return m_keyReleasedEvent;
		}

		//---------------------------------------------------------------------------------
		void Keyboard::OnInit() noexcept
		{
			// Clear all key states
			for(auto& flag : m_keysDown)
			{
				flag = false;
			}

			DispmanWindow::Get()->SetKeyboardDelegates(ChilliSource::MakeDelegate(this, &Keyboard::OnKeyEvent));
		}

		//-------------------------------------------------------
		bool Keyboard::IsKeyDown(ChilliSource::KeyCode in_code) const
		{
			CS_ASSERT(ChilliSource::Application::Get()->GetTaskScheduler()->IsMainThread(), "Attempted to access held keys outside of the main thread.");
			return m_keysDown[static_cast<u32>(in_code)];
		}

		//----------------------------------------------------------------------------------
		void Keyboard::OnKeyEvent(u32 keyCode, u32 keyState, DispmanWindow::KeyboardEvent keyEvent) noexcept
		{
			ChilliSource::Application::Get()->GetTaskScheduler()->ScheduleTask(ChilliSource::TaskType::k_mainThread, [=](const ChilliSource::TaskContext& context)
			{
				auto csKeyCode = X11KeySymToCSKeyCode(keyCode);

				if(keyEvent == DispmanWindow::KeyboardEvent::k_pressed)
				{
					if (IsKeyDown(csKeyCode) == false)
					{
						m_keysDown[static_cast<u32>(csKeyCode)] = true;

						std::vector<ChilliSource::ModifierKeyCode> modifiers;
						modifiers.reserve((u32)ChilliSource::ModifierKeyCode::k_total);

						if (keyState & Mod1Mask)
						{
							modifiers.push_back(ChilliSource::ModifierKeyCode::k_alt);
						}
						if (keyState & ControlMask)
						{
							modifiers.push_back(ChilliSource::ModifierKeyCode::k_ctrl);
						}
						if (keyState & ShiftMask)
						{
							modifiers.push_back(ChilliSource::ModifierKeyCode::k_shift);
						}
						if (keyState & Mod4Mask)
						{
							modifiers.push_back(ChilliSource::ModifierKeyCode::k_system);
						}

						m_keyPressedEvent.NotifyConnections(csKeyCode, std::move(modifiers));
					}
				}
				else if (IsKeyDown(csKeyCode) == true)
				{
					m_keysDown[static_cast<u32>(csKeyCode)] = false;

					m_keyReleasedEvent.NotifyConnections(csKeyCode);
				}
			});
		}

		//---------------------------------------------------------------------------------
		void Keyboard::OnDestroy() noexcept
		{
			DispmanWindow::Get()->RemoveKeyboardDelegates();
		}
	}
}

#endif
