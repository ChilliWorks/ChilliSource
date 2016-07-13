//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
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

#include <ChilliSource/Input/Keyboard/KeyCode.h>

#include <string>

namespace ChilliSource
{
    //----------------------------------------------------------------------
    std::string GetKeyName(const KeyCode& in_key)
    {
        switch (in_key)
        {
            case KeyCode::k_a:
                return "A";
            case KeyCode::k_b:
                return "B";
            case KeyCode::k_c:
                return "C";
            case KeyCode::k_d:
                return "D";
            case KeyCode::k_e:
                return "E";
            case KeyCode::k_f:
                return "F";
            case KeyCode::k_g:
                return "G";
            case KeyCode::k_h:
                return "H";
            case KeyCode::k_i:
                return "I";
            case KeyCode::k_j:
                return "J";
            case KeyCode::k_k:
                return "K";
            case KeyCode::k_l:
                return "L";
            case KeyCode::k_m:
                return "M";
            case KeyCode::k_n:
                return "N";
            case KeyCode::k_o:
                return "O";
            case KeyCode::k_p:
                return "P";
            case KeyCode::k_q:
                return "Q";
            case KeyCode::k_r:
                return "R";
            case KeyCode::k_s:
                return "S";
            case KeyCode::k_t:
                return "T";
            case KeyCode::k_u:
                return "U";
            case KeyCode::k_v:
                return "V";
            case KeyCode::k_w:
                return "W";
            case KeyCode::k_x:
                return "X";
            case KeyCode::k_y:
                return "Y";
            case KeyCode::k_z:
                return "Z";
            case KeyCode::k_num0:
                return "0";
            case KeyCode::k_num1:
                return "1";
            case KeyCode::k_num2:
                return "2";
            case KeyCode::k_num3:
                return "3";
            case KeyCode::k_num4:
                return "4";
            case KeyCode::k_num5:
                return "5";
            case KeyCode::k_num6:
                return "6";
            case KeyCode::k_num7:
                return "7";
            case KeyCode::k_num8:
                return "8";
            case KeyCode::k_num9:
                return "9";
            case KeyCode::k_f1:
                return "F1";
            case KeyCode::k_f2:
                return "F2";
            case KeyCode::k_f3:
                return "F3";
            case KeyCode::k_f4:
                return "F4";
            case KeyCode::k_f5:
                return "F5";
            case KeyCode::k_f6:
                return "F6";
            case KeyCode::k_f7:
                return "F7";
            case KeyCode::k_f8:
                return "F8";
            case KeyCode::k_f9:
                return "F9";
            case KeyCode::k_f10:
                return "F10";
            case KeyCode::k_f11:
                return "F11";
            case KeyCode::k_f12:
                return "F12";
            case KeyCode::k_f13:
                return "F13";
            case KeyCode::k_f14:
                return "F14";
            case KeyCode::k_f15:
                return "F15";
            case KeyCode::k_numpad0:
                return "Numpad 0";
            case KeyCode::k_numpad1:
                return "Numpad 1";
            case KeyCode::k_numpad2:
                return "Numpad 2";
            case KeyCode::k_numpad3:
                return "Numpad 3";
            case KeyCode::k_numpad4:
                return "Numpad 4";
            case KeyCode::k_numpad5:
                return "Numpad 5";
            case KeyCode::k_numpad6:
                return "Numpad 6";
            case KeyCode::k_numpad7:
                return "Numpad 7";
            case KeyCode::k_numpad8:
                return "Numpad 8";
            case KeyCode::k_numpad9:
                return "Numpad 9";
            case KeyCode::k_space:
                return "Space";
            case KeyCode::k_left:
                return "Left";
            case KeyCode::k_right:
                return "Right";
            case KeyCode::k_up:
                return "Up";
            case KeyCode::k_down:
                return "Down";
            case KeyCode::k_insert:
                return "Insert";
            case KeyCode::k_delete:
                return "Delete";
            case KeyCode::k_home:
                return "Home";
            case KeyCode::k_end:
                return "End";
            case KeyCode::k_pageUp:
                return "Page Up";
            case KeyCode::k_pageDown:
                return "Page Down";
            case KeyCode::k_pause:
                return "Pause";
            case KeyCode::k_backSpace:
                return "Backspace";
            case KeyCode::k_return:
                return "Return";
            case KeyCode::k_add:
                return "Numpad Add";
            case KeyCode::k_divide:
                return "Numpad Divide";
            case KeyCode::k_multiply:
                return "Numpad Multiply";
            case KeyCode::k_subtract:
                return "Numpad Subtract";
            case KeyCode::k_escape:
                return "Escape";
            case KeyCode::k_backSlash:
                return "Backslash";
            case KeyCode::k_comma:
                return "Comma";
            case KeyCode::k_period:
                return "Period";
            case KeyCode::k_forwardSlash:
                return "Forward Slash";
            case KeyCode::k_semiColon:
                return "Semicolon";
            case KeyCode::k_quote:
                return "Quote";
            case KeyCode::k_tilde:
                return "Tilde";
            case KeyCode::k_leftBracket:
                return "Left Bracket";
            case KeyCode::k_rightBracket:
                return "Right Bracket";
            case KeyCode::k_hyphen:
                return "Hyphen";
            case KeyCode::k_equals:
                return "Equals";
            case KeyCode::k_menu:
                return "Menu";
            case KeyCode::k_leftShift:
                return "Left Shift";
            case KeyCode::k_rightShift:
                return "Right Shift";
            case KeyCode::k_leftCtrl:
                return "Left Ctrl";
            case KeyCode::k_rightCtrl:
                return "Right Ctrl";
            case KeyCode::k_leftAlt:
                return "Left Alt";
            case KeyCode::k_rightAlt:
                return "Right Alt";
            case KeyCode::k_leftSystem:
                return "Left System";
            case KeyCode::k_rightSystem:
                return "Right System";
            case KeyCode::k_tab:
                return "Tab";
            case KeyCode::k_unknown:
                return "Unknown";
            case KeyCode::k_total:
                break;
        }
        
        CS_LOG_FATAL("Invalid keycode.");
        return "";
    }
}