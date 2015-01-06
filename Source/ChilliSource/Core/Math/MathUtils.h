//
//  MathUtils.h
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

#ifndef _CHILLISOURCE_CORE_MATH_MATHUTILS_H_
#define _CHILLISOURCE_CORE_MATH_MATHUTILS_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Random.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace ChilliSource
{
	namespace Core 
	{
        //---------------------------------------------------------
        /// A group of miscellaneous mathematical functions
        ///
        /// @author S McGaw
        //---------------------------------------------------------
        namespace MathUtils
        {
            const f32 k_pi = 3.1415926536f;
            
            //---------------------------------------------------------
            /// @author S McGaw
            ///
            /// @param Value
            ///
            /// @return The closest higher power of two
            //---------------------------------------------------------
			u32 NextPowerOfTwo(u32 in_value);
            //---------------------------------------------------------
            /// @author S McGaw
            ///
            /// @param Angle in degrees
            ///
            /// @return Angle in radians
            //---------------------------------------------------------
			f32 DegToRad(f32 in_angle);
            //---------------------------------------------------------
            /// @author S McGaw
            ///
            /// @param Angle in radians
            ///
            /// @return Angle in degrees
            //---------------------------------------------------------
			f32 RadToDeg(f32 in_angle);
            //---------------------------------------------------------
            /// @author A Glass
            ///
            /// @param Value
            /// @param Edge
            /// @param Min
            /// @param Max
            ///
            /// @return Value >= Edge -> Max Value < Edge -> Min
            //---------------------------------------------------------
            template <typename TValueType> TValueType Step(TValueType in_value, TValueType in_edge, TValueType in_min, TValueType in_max)
            {
                return (in_value < in_edge) ? in_min : in_max;
            }
			//---------------------------------------------------------
			/// @author A Glass
			///
			/// @param Value to determine sign of
            ///
			/// @return Sign +1 if positive -1 if negative 0 if zero
			//---------------------------------------------------------
			template<typename TValueType> s32 Sign(TValueType in_value)
            {
                return (0 < in_value) - (in_value < 0);
            }
			//----------------------------------------------------------
			/// @author S Downie
			///
			/// @param Value to clamp between range
			/// @param Lower range value
			/// @param Higher range value
            ///
			/// @return Value within the given range
			//----------------------------------------------------------
			template<typename TValueType> TValueType Clamp(TValueType in_value, TValueType in_min, TValueType in_max)
			{
				return std::max(in_min, std::min(in_value, in_max));
			}
            //---------------------------------------------------------
            /// Linearly interpolate between the min and max values
            /// based on parametric t factor (0 - 1)
            ///
            /// @author S Downie
            ///
            /// @param t factor
            /// @param Min value in range
            /// @param Max value in range
            ///
            /// @return Linearly interpolated value
            //---------------------------------------------------------
			template <typename TValueType> TValueType Lerp(f32 in_factor, TValueType in_min, TValueType in_max)
			{
				return in_min * (1.0f - in_factor) + in_max * in_factor;
			}
            //---------------------------------------------------------
            /// Cubicly interpolate between the min and max values
            /// based on parametric t factor (0 - 1) and the smoothstep
            /// curve
            ///
            /// @author A Glass
            ///
            /// @param t factor
            /// @param Min value in range
            /// @param Max value in range
            ///
            /// @return Cubicly interpolated value
            //---------------------------------------------------------
            template <typename TValueType> TValueType SmoothStep(f32 in_factor, TValueType in_min, TValueType in_max)
            {
                TValueType t = Clamp((in_min * -1.0f + in_factor)/(in_max - in_min), 0.0f, 1.0f);
                return t * t * ( t * -2.0f  + 3.0f);
            }
            //---------------------------------------------------------
            /// Interpolate between the min and max values
            /// based on parametric t factor (0 - 1) and the smootherstep
            /// curve (Ken Perlin)
            ///
            /// @author S Downie
            ///
            /// @param t factor
            /// @param Min value in range
            /// @param Max value in range
            ///
            /// @return Smoother step interpolated value
            //---------------------------------------------------------
            template <typename TValueType> TValueType SmootherStep(f32 in_factor, TValueType in_min, TValueType in_max)
            {
                TValueType t = Clamp((in_min * -1.0f + in_factor)/(in_max - in_min), 0.0f, 1.0f);
                return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
            }
            //----------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Value
            ///
            /// @return If the value is within epsilon of zero
            //----------------------------------------------------------
            template <typename TRealType> bool IsApproxZero(TRealType in_value)
            {
                return std::abs(in_value) < std::numeric_limits<TRealType>::epsilon();
            }
			//----------------------------------------------------------
			/// Rounds the floating point number to the nearest 
			/// whole number. This is a symmetrical round
            ///
            /// @author A Glass
			///
			/// @param Floating point to round
            ///
			/// @return Rounded number
			//----------------------------------------------------------
			template <typename TRealType> TRealType Round(TRealType in_value)
            {
                return ((in_value > 0.0) ? std::floor(in_value + 0.5) : std::ceil(in_value - 0.5));
            }
            //----------------------------------------------------
            /// Calculate the most common value from the given
            /// data set
            ///
            /// @author S Downie
            ///
            /// @param Beginning of data set
            /// @param End of data set
            ///
            /// @return Value that occurs most often
            //----------------------------------------------------
            template <typename TIterType> typename std::iterator_traits<TIterType>::value_type Mode(TIterType in_begin, TIterType in_end)
            {
                typename std::iterator_traits<TIterType>::value_type result = 0;
                size_t largestcount = 0;
                
                for(auto itOuter = in_begin; itOuter != in_end; ++itOuter)
                {
                    size_t count = 0;
                    
                    for(auto itInner = itOuter; itInner != in_end; ++itInner)
                    {
                        if(*itOuter == *itInner)
                        {
                            ++count;
                        }
                    }
                    
                    if(count >= largestcount)
                    {
                        largestcount = count;
                        result = *itOuter;
                    }
                }
                
                return result;
            }
            //----------------------------------------------------
            /// Calculate the avearge value from the given
            /// data set
            ///
            /// @author S Downie
            ///
            /// @param Beginning of data set
            /// @param End of data set
            ///
            /// @return Average value
            //----------------------------------------------------
            template <typename TIterType> typename std::iterator_traits<TIterType>::value_type Mean(TIterType in_begin, TIterType in_end)
            {
                typename std::iterator_traits<TIterType>::value_type result = 0;
                
                size_t size = 0;
                for(auto it = in_begin; it != in_end; ++it)
                {
                    result += (*it);
                    ++size;
                }
                
                CS_ASSERT(size > 0, "MathUtils::Mean: Cannot pass in empty container");
                return result/size;
            }
            //----------------------------------------------------
            /// Calculate the mid point of the data set
            ///
            /// NOTE: This will re-order the array
            ///
            /// @author S Downie
            ///
            /// @param Beginning of data set
            /// @param End of data set
            ///
            /// @return Value at mid point
            //----------------------------------------------------
            template <typename TIterType> typename std::iterator_traits<TIterType>::value_type Median(TIterType in_begin, TIterType in_end)
            {
                std::size_t size = in_end - in_begin;
                
                CS_ASSERT(size > 0, "MathUtils::Median: Cannot pass in empty container");
                
                std::size_t middleIdx = size/2;
                
                TIterType itTarget = in_begin + middleIdx;
                std::nth_element(in_begin, itTarget, in_end);
                
                if(size % 2 != 0)
                {
                    return *itTarget;
                }
                else
                {
                    //Even number of elements
                    typename std::iterator_traits<TIterType>::value_type targetValue = *itTarget;
                    TIterType itTargetNeighbor = std::max_element(in_begin, itTarget);
                    return (targetValue + *itTargetNeighbor)/2.0;
                }
            }
            //---------------------------------------------------------
            /// @author S Downie
            ///
            /// @param The minimum number that can be returned (inclusive)
            /// @param The maximum number that can be returned (inclusive)
            /// @param Number of samples
            ///
            /// @return The average of a certain number of random values,
			/// within the range, as defined by the Central Limit Theorem
            //---------------------------------------------------------
            template <typename TValueType> TValueType CentralLimit(TValueType in_min, TValueType in_max, u32 in_numSamples)
            {
				CS_ASSERT(in_numSamples != 0, "Must have at least one sample");
				
				TValueType result = 0;
                
				for(u32 i=0; i < in_numSamples; ++i)
                {
                    result += Random::Generate(in_min, in_max);
                }
                
                return result /= in_numSamples;
            }
            //----------------------------------------------------
            /// Convert the value in the given range to the range
            /// 0 - 1 using a linear conversion
            ///
            /// @author S McGaw
            ///
            /// @param Value
            /// @param Min
            /// @param Max
            ///
            /// @return Value converted to the range 0 - 1
            //----------------------------------------------------
            template <typename TValueType> TValueType NormalisedRange(TValueType in_value, TValueType in_originalMin, TValueType in_originalMax)
            {
				CS_ASSERT(in_originalMin != in_originalMax, "Divide by ZERO error");
                
                return (in_value - in_originalMin) / (in_originalMax - in_originalMin);
            }
            //----------------------------------------------------
            /// Convert the value in the given range to the new range
            /// using a linear conversion
            ///
            /// @author S McGaw
            ///
            /// @param Value
            /// @param Min Range 1
            /// @param Max Range 1
            /// @param Min Range 2
            /// @param Max Range 2
            ///
            /// @return Value converted to range 2
            //----------------------------------------------------
            template <typename TValueType> TValueType ConvertRange(TValueType in_value, TValueType in_originalMin, TValueType in_originalMax, TValueType in_newMin, TValueType in_newMax)
            {
                CS_ASSERT(in_originalMin != in_originalMax, "Divide by ZERO error");
                
                return (((in_value - in_originalMin) * (in_newMax - in_newMin)) / (in_originalMax - in_originalMin)) + in_newMin;
            }
		};
	}
}

#endif
