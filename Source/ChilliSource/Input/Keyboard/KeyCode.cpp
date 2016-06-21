//
//  KeyCode.cpp
//  Chilli Source
//  Created by Jordan Brown on 21/06/2016
//
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
    //----------------------------------------------------------------------
    std::string GetKeyName(const KeyCode& in_key)
    {
        switch (in_key)
        {
            case KeyCode::k_a:
                return "a";
            case KeyCode::k_b:
                return "b";
            case KeyCode::k_c:
                return "c";
            case KeyCode::k_d:
                return "d";
            case KeyCode::k_e:
                return "e";
            case KeyCode::k_f:
                return "f";
            case KeyCode::k_g:
                return "g";
            case KeyCode::k_h:
                return "h";
            case KeyCode::k_i:
                return "i";
            case KeyCode::k_j:
                return "j";
            case KeyCode::k_k:
                return "k";
            case KeyCode::k_l:
                return "l";
            case KeyCode::k_m:
                return "m";
            case KeyCode::k_n:
                return "n";
            case KeyCode::k_o:
                return "o";
            case KeyCode::k_p:
                return "p";
            case KeyCode::k_q:
                return "q";
            case KeyCode::k_r:
                return "r";
            case KeyCode::k_s:
                return "s";
            case KeyCode::k_t:
                return "t";
            case KeyCode::k_u:
                return "u";
            case KeyCode::k_v:
                return "v";
            case KeyCode::k_w:
                return "w";
            case KeyCode::k_x:
                return "x";
            case KeyCode::k_y:
                return "y";
            case KeyCode::k_z:
                return "z";
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
                return "NUMPAD 0";
            case KeyCode::k_numpad1:
                return "NUMPAD 1";
            case KeyCode::k_numpad2:
                return "NUMPAD 2";
            case KeyCode::k_numpad3:
                return "NUMPAD 3";
            case KeyCode::k_numpad4:
                return "NUMPAD 4";
            case KeyCode::k_numpad5:
                return "NUMPAD 5";
            case KeyCode::k_numpad6:
                return "NUMPAD 6";
            case KeyCode::k_numpad7:
                return "NUMPAD 7";
            case KeyCode::k_numpad8:
                return "NUMPAD 8";
            case KeyCode::k_numpad9:
                return "NUMPAD 9";
            case KeyCode::k_space:
                return "SPACE";
            case KeyCode::k_left:
                return "LEFT";
            case KeyCode::k_right:
                return "RIGHT";
            case KeyCode::k_up:
                return "UP";
            case KeyCode::k_down:
                return "DOWN";
            case KeyCode::k_insert:
                return "INSERT";
            case KeyCode::k_delete:
                return "DELETE";
            case KeyCode::k_home:
                return "HOME";
            case KeyCode::k_end:
                return "END";
            case KeyCode::k_pageUp:
                return "PAGE UP";
            case KeyCode::k_pageDown:
                return "PAGE DOWN";
            case KeyCode::k_pause:
                return "PAUSE";
            case KeyCode::k_backSpace:
                return "BACKSPACE";
            case KeyCode::k_return:
                return "RETURN";
            case KeyCode::k_add:
                return "NUMPAD ADD";
            case KeyCode::k_divide:
                return "NUMPAD DIVIDE";
            case KeyCode::k_multiply:
                return "NUMPAD MULTIPLY";
            case KeyCode::k_subtract:
                return "NUMPAD SUBTRACT";
            case KeyCode::k_escape:
                return "ESCAPE";
            case KeyCode::k_backSlash:
                return "BACKSLASH";
            case KeyCode::k_comma:
                return "COMMA";
            case KeyCode::k_period:
                return "PERIOD";
            case KeyCode::k_forwardSlash:
                return "FORWARD SLASH";
            case KeyCode::k_semiColon:
                return "SEMICOLON";
            case KeyCode::k_quote:
                return "QUOTE";
            case KeyCode::k_tilde:
                return "TILDE";
            case KeyCode::k_leftBracket:
                return "LEFT BRACKET";
            case KeyCode::k_rightBracket:
                return "RIGHT BRACKET";
            case KeyCode::k_hyphen:
                return "HYPHEN";
            case KeyCode::k_equals:
                return "EQUALS";
            case KeyCode::k_menu:
                return "MENU";
            case KeyCode::k_leftShift:
                return "LEFT SHIFT";
            case KeyCode::k_rightShift:
                return "RIGHT SHIFT";
            case KeyCode::k_leftCtrl:
                return "LEFT CTRL";
            case KeyCode::k_rightCtrl:
                return "RIGHT CTRL";
            case KeyCode::k_leftAlt:
                return "LEFT ALT";
            case KeyCode::k_rightAlt:
                return "RIGHT ALT";
            case KeyCode::k_leftSystem:
                return "LEFT SYSTEM";
            case KeyCode::k_rightSystem:
                return "RIGHT SYSTEM";
            case KeyCode::k_unknown:
            default:
                return "UNKNOWN";
        }
    }
}