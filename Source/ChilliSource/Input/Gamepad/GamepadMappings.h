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
        namespace PS4
        {
            const std::string k_name = "Wireless Controller";
            
            const u32 k_square = 0;
            const u32 k_cross = 1;
            const u32 k_circle = 2;
            const u32 k_triangle = 3;
            const u32 k_l1 = 4;
            const u32 k_r1 = 5;
            const u32 k_l2 = 6;
            const u32 k_r2 = 7;
            const u32 k_share = 8;
            const u32 k_options = 9;
            const u32 k_l3 = 10;
            const u32 k_r3 = 11;
            const u32 k_ps = 12;
            const u32 k_trackpad = 13;
            
            const GamepadAxis k_lStickX = GamepadAxis::k_x;
            const GamepadAxis k_lStickY = GamepadAxis::k_y;
            const GamepadAxis k_rStickX = GamepadAxis::k_z;
            const GamepadAxis k_rStickY = GamepadAxis::k_r;
            const GamepadAxis k_dpadX = GamepadAxis::k_povX;
            const GamepadAxis k_dpadY = GamepadAxis::k_povY;
            
            const bool k_lStickXShouldInvert = false;
            const bool k_lStickYShouldInvert = false;
            const bool k_rStickXShouldInvert = false;
            const bool k_rStickYShouldInvert = false;
            const bool k_dpadXShouldInvert = false;
            const bool k_l2TriggerShouldInvert = false;
            const bool k_r2TriggerShouldInvert = false;
            
            
#ifndef CS_TARGETPLATFORM_RPI
            const GamepadAxis k_l2Trigger = GamepadAxis::k_v;
            const GamepadAxis k_r2Trigger = GamepadAxis::k_u;
            
            const bool k_dpadYShouldInvert = true;
#else
            const GamepadAxis k_l2Trigger = GamepadAxis::k_u;
            const GamepadAxis k_r2Trigger = GamepadAxis::k_v;
            
            const bool k_dpadYShouldInvert = false;
#endif
        }
    }
}

#endif
