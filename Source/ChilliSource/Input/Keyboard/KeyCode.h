//
//  KeyCode.h
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

#ifndef _CHILLISOURCE_INPUT_KEYBOARD_KEYCODE_H_
#define _CHILLISOURCE_INPUT_KEYBOARD_KEYCODE_H_

namespace ChilliSource
{
	namespace Input
	{
        //---------------------------------------------------------------
        /// List of all the key-codes supported for keyboard events
        ///
        /// @author S Downie
        //---------------------------------------------------------------
		enum class KeyCode
		{
            k_unknown,
            k_a,
            k_b,
            k_c,
            k_d,
            k_e,
            k_f,
            k_g,
            k_h,
            k_i,
            k_j,
            k_k,
            k_l,
            k_m,
            k_n,
            k_o,
            k_p,
            k_q,
            k_r,
            k_s,
            k_t,
            k_u,
            k_v,
            k_w,
            k_x,
            k_y,
            k_z,
            k_num0,
            k_num1,
            k_num2,
            k_num3,
            k_num4,
            k_num5,
            k_num6,
            k_num7,
            k_num8,
            k_num9,
            k_escape,
            k_leftCtrl,
            k_leftShift,
            k_leftAlt,
            k_leftSystem, //Windows key, Command key, etc
            k_rightCtrl,
            k_rightShift,
            k_rightAlt,
            k_rightSystem, //Windows key, Command key, etc
            k_menu,
            k_leftBracket,
            k_rightBracket,
            k_semiColon,
            k_comma,
            k_period,
            k_quote,
            k_forwardSlash,
            k_backSlash,
            k_tilde,
            k_equals,
            k_hyphen,
            k_space,
            k_return,
            k_backSpace,
            k_tab,
            k_pageUp,
            k_pageDown,
            k_end,
            k_home,
            k_insert,
            k_delete,
            k_add,
            k_subtract,
            k_multiply,
            k_divide,
            k_left,
            k_right,
            k_up,
            k_down,
            k_numpad0,
            k_numpad1,
            k_numpad2,
            k_numpad3,
            k_numpad4,
            k_numpad5,
            k_numpad6,
            k_numpad7,
            k_numpad8,
            k_numpad9,
            k_f1,
            k_f2,
            k_f3,
            k_f4,
            k_f5,
            k_f6,
            k_f7,
            k_f8,
            k_f9,
            k_f10,
            k_f11,
            k_f12,
            k_f13,
            k_f14,
            k_f15,
            k_pause,
            
            k_total
        };
	}
}

#endif


