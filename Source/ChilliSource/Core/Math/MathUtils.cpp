//
//  MathUtils.cpp
//  Chilli Source
//  Created by Stuart McGaw on 22/09/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Core/Math/MathUtils.h>

#include <ctime>
#include <random>

namespace ChilliSource
{
	namespace Core 
	{
        namespace MathUtils
        {
            namespace
            {
                const f32 k_degreesToRadians = 0.0174532925f;
                const f32 k_radiansToDegrees = 57.2957795f;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            u32 NextPowerOfTwo(u32 in_value)
            {
                in_value--;
                in_value |= in_value >> 1;  // handle  2 bit numbers
                in_value |= in_value >> 2;  // handle  4 bit numbers
                in_value |= in_value >> 4;  // handle  8 bit numbers
                in_value |= in_value >> 8;  // handle 16 bit numbers
                in_value |= in_value >> 16; // handle 32 bit numbers
                in_value++;
                
                return in_value;	
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            f32 DegToRad(f32 in_angle)
            {
                return in_angle * k_degreesToRadians;
            }
            //---------------------------------------------------------
            //---------------------------------------------------------
            f32 RadToDeg(f32 infAngle)
            {
                return infAngle * k_radiansToDegrees;
            }
        }
	}
}
