//
//  ParticlePropertyCurveFunctions.cpp
//  Chilli Source
//  Created by Ian Copland on 29/12/2014.
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

#include <ChilliSource/Rendering/Particle/Property/ParticlePropertyCurveFunctions.h>

#include <json/json.h>

namespace ChilliSource
{
    namespace Rendering
    {
        namespace ParticlePropertyCurveFunctions
        {
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            f32 Linear(f32 in_x)
            {
                CS_ASSERT(in_x >= 0.0f && in_x <= 1.0f, "x must always be in the range 0.0 to 1.0 in particle property curve functions.");
                
                return in_x;
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            f32 EaseInQuad(f32 in_x)
            {
                CS_ASSERT(in_x >= 0.0f && in_x <= 1.0f, "x must always be in the range 0.0 to 1.0 in particle property curve functions.");
                
                return in_x * in_x;
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            f32 EaseOutQuad(f32 in_x)
            {
                CS_ASSERT(in_x >= 0.0f && in_x <= 1.0f, "x must always be in the range 0.0 to 1.0 in particle property curve functions.");
                
                return -in_x * (in_x - 2.0f);
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            f32 SmoothStep(f32 in_x)
            {
                CS_ASSERT(in_x >= 0.0f && in_x <= 1.0f, "x must always be in the range 0.0 to 1.0 in particle property curve functions.");
                
                return in_x * in_x * (3.0f - 2.0f * in_x);
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            f32 LinearPingPong(f32 in_x)
            {
                CS_ASSERT(in_x >= 0.0f && in_x <= 1.0f, "x must always be in the range 0.0 to 1.0 in particle property curve functions.");
                
                if (in_x < 0.5f)
                {
                    return Linear(in_x * 2.0f);
                }
                else
                {
                    return Linear(2.0f * (1.0f - in_x));
                }
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            f32 EaseInQuadPingPong(f32 in_x)
            {
                CS_ASSERT(in_x >= 0.0f && in_x <= 1.0f, "x must always be in the range 0.0 to 1.0 in particle property curve functions.");
                
                if (in_x < 0.5f)
                {
                    return EaseInQuad(in_x * 2.0f);
                }
                else
                {
                    return EaseInQuad(2.0f * (1.0f - in_x));
                }
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            f32 EaseOutQuadPingPong(f32 in_x)
            {
                CS_ASSERT(in_x >= 0.0f && in_x <= 1.0f, "x must always be in the range 0.0 to 1.0 in particle property curve functions.");
                
                if (in_x < 0.5f)
                {
                    return EaseOutQuad(in_x * 2.0f);
                }
                else
                {
                    return EaseOutQuad(2.0f * (1.0f - in_x));
                }
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            f32 SmoothStepPingPong(f32 in_x)
            {
                CS_ASSERT(in_x >= 0.0f && in_x <= 1.0f, "x must always be in the range 0.0 to 1.0 in particle property curve functions.");
                
                if (in_x < 0.5f)
                {
                    return SmoothStep(in_x * 2.0f);
                }
                else
                {
                    return SmoothStep(2.0f * (1.0f - in_x));
                }
            }
        }
    }
}