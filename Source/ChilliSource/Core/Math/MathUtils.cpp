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

namespace ChilliSource
{
	namespace Core 
	{
        namespace MathUtils
        {
            //---------------------------------------------------------
            /// Next Power Of Two
            ///
            /// @param Non-power of two value
            /// @return The closest higher power of two
            //---------------------------------------------------------
            u32 NextPowerOfTwo(u32 inX)
            {
                inX--;
                inX |= inX >> 1;  // handle  2 bit numbers
                inX |= inX >> 2;  // handle  4 bit numbers
                inX |= inX >> 4;  // handle  8 bit numbers
                inX |= inX >> 8;  // handle 16 bit numbers
                inX |= inX >> 16; // handle 32 bit numbers
                inX++;
                
                return inX;	
            }
            //---------------------------------------------------------
            /// Degrees To Radians
            ///
            /// @param Angle in degrees
            /// @return Angle in radians
            //---------------------------------------------------------
            f32 DegToRad(f32 infAngle)
            {
                return infAngle * (k_pi/180.0f);
            }
            //---------------------------------------------------------
            /// Radians To Degrees
            ///
            /// @param Angle in radians
            /// @return Angle in degrees
            //---------------------------------------------------------
            f32 RadToDeg(f32 infAngle)
            {
                return infAngle * (180.0f/k_pi);
            }
            //---------------------------------------------------------
            /// fRand
            ///
            /// @return Normalised random number between 0 and 1
            //---------------------------------------------------------
            f32 FRand()
            {
                return (f32)rand()/(f32)RAND_MAX;
            }
            //---------------------------------------------------------
            /// Seed Rand On Time
            ///
            /// Seed the random number generator based on the
            /// system clock.
            //---------------------------------------------------------
            void SeedRandOnTime()
            {
                srand(clock());
            }
            //---------------------------------------------------------
            /// Get next number
            ///
            /// pass in seed
            //---------------------------------------------------------
            f32 GetPseudoRandom(u32 inudwSeed)
            {
                u32 udwPseudoRandomSeed = inudwSeed + inudwSeed * 1103515245 + 12345;
                u32 udwResult = (u32)(udwPseudoRandomSeed / 65536) % (100 + 1);
                
                return (f32)udwResult / 100.0f;
            }
            //---------------------------------------------------------
            /// Log2f
            ///
            /// A cross compatible alternative to log2f() in math.h.
            ///
            /// @param The same value as log2f();
            /// @return The same value as log2f();
            //---------------------------------------------------------
            f32 Log2f(f32 infValue)
            {
                return (f32)log(infValue)/(f32)log(2.0f);
            }
            //----------------------------------------------------------
            /// Is Approx Zero
            ///
            /// @param Value
            /// @return If the value is within epsilon of zero
            //----------------------------------------------------------
            bool IsApproxZero(f32 infValue)
            {
                return (infValue > -k_approxZero) && (infValue < k_approxZero); 
            }
            //----------------------------------------------------------
            /// Round
            ///
            /// Rounds the floating point number to the nearest 
            /// integer. This is a symmetrical round
            ///
            /// @param Floating point to round
            /// @return Rounded integer
            //----------------------------------------------------------
            s32 Round(f32 infValue)
            {
                return (s32)((infValue > 0.0f) ? std::floor(infValue + 0.5f) : std::ceil(infValue - 0.5f));
            }
        }
	}
}
