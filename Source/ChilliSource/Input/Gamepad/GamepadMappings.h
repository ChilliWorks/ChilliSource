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

#ifndef _CHILLISOURCE_INPUT_GAMEPAD_GAMEPADMAPPINGS_H_
#define _CHILLISOURCE_INPUT_GAMEPAD_GAMEPADMAPPINGS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Input/Gamepad/GamepadAxis.h>

namespace ChilliSource
{
    /// Defines button mappings for some common controllers
    ///
    namespace GamepadMappings
    {
        namespace XBox360
        {
            const u32 k_a = 0;
            const u32 k_b = 1;
            const u32 k_x = 2;
            const u32 k_y = 3;
            const u32 k_lBumper = 4;
            const u32 k_rBumper = 5;
            const u32 k_back = 6;
            const u32 k_start = 7;
            const u32 k_lStick = 8;
            const u32 k_rStick = 9;
            
            const GamepadAxis k_lStickX = GamepadAxis::k_x;
            const GamepadAxis k_lStickY = GamepadAxis::k_y;
            const GamepadAxis k_rStickX = GamepadAxis::k_u;
            const GamepadAxis k_rStickY = GamepadAxis::k_r;
            const GamepadAxis k_dpadX = GamepadAxis::k_povX;
            const GamepadAxis k_dpadY = GamepadAxis::k_povY;
            const GamepadAxis k_triggers = GamepadAxis::k_z; //Strangely the left and right triggers are reported as a single analogue input with +ve meaning RT and -ve meaning LT
        }
        
        namespace PS3
        {
            const u32 k_triangle = 0;
            const u32 k_circle = 1;
            const u32 k_cross = 2;
            const u32 k_square = 3;
            const u32 k_l2 = 4;
            const u32 k_r2 = 5;
            const u32 k_l1 = 6;
            const u32 k_r1 = 7;
            const u32 k_select = 8;
            const u32 k_start = 9;
            const u32 k_l3 = 10;
            const u32 k_r3 = 11;
            //TODO: Stick clicks
            
            const GamepadAxis k_lStickX = GamepadAxis::k_x;
            const GamepadAxis k_lStickY = GamepadAxis::k_y;
            const GamepadAxis k_rStickX = GamepadAxis::k_u;
            const GamepadAxis k_rStickY = GamepadAxis::k_r;
            const GamepadAxis k_dpadX = GamepadAxis::k_povX;
            const GamepadAxis k_dpadY = GamepadAxis::k_povY;
        }
        
        namespace GenericNES
        {
            const u32 k_x = 0;
            const u32 k_a = 1;
            const u32 k_b = 2;
            const u32 k_y = 3;
            const u32 k_l = 4;
            const u32 k_r = 5;
            const u32 k_select = 6;
            const u32 k_start = 7;
            
            const GamepadAxis k_dpadX = GamepadAxis::k_x;
            const GamepadAxis k_dpadY = GamepadAxis::k_y;
        }
    }
}

#endif
