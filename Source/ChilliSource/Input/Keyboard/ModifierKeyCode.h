//
//  ModifierKeyCode.h
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

#ifndef _CHILLISOURCE_INPUT_KEYBOARD_MODIFIERKEYCODE_H_
#define _CHILLISOURCE_INPUT_KEYBOARD_MODIFIERKEYCODE_H_

namespace ChilliSource
{
	namespace Input
	{
        //---------------------------------------------------------------
        /// List of all the modifier key-codes supported for keyboard events.
        /// This makes no distinction between left and right keys and therefore
        /// is independent of standard key codes
        ///
        /// @author S Downie
        //---------------------------------------------------------------
		enum class ModifierKeyCode
		{
            k_ctrl,
            k_alt,
            k_shift,
            k_system, //Windows, Command, etc.
            
            k_total
        };
	}
}

#endif


