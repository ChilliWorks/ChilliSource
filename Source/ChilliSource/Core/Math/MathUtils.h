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

#ifndef _CHILLISOURCE_CORE_MATH_MATH_HELPER_H_
#define _CHILLISOURCE_CORE_MATH_MATH_HELPER_H_

#include <ChilliSource/ChilliSource.h>

#include <algorithm>
#include <cmath>
#include <vector>

namespace ChilliSource
{
	namespace Core 
	{
        ///////////////////////////////////////////////////////////////////
		/// Description:
		///
		/// Common math utilities
		///////////////////////////////////////////////////////////////////
        namespace MathUtils
        {
            const f32 k_pi = 3.14159265358979323846264338327950288f;
            const f32 k_approxZero = 0.0001192092896f;
            //---------------------------------------------------------
            /// Next Power Of Two
            ///
            /// @param Non-power of two value
            /// @return The closest higher power of two
            //---------------------------------------------------------
			u32 NextPowerOfTwo(u32 inX);
            //---------------------------------------------------------
            /// Degrees To Radians
            ///
            /// @param Angle in degrees
            /// @return Angle in radians
            //---------------------------------------------------------
			f32 DegToRad(f32 infAngle);
            //---------------------------------------------------------
            /// Radians To Degrees
            ///
            /// @param Angle in radians
            /// @return Angle in degrees
            //---------------------------------------------------------
			f32 RadToDeg(f32 infAngle);
            //---------------------------------------------------------
            /// fRand
            ///
            /// @return Normalised random number between 0 and 1
            //---------------------------------------------------------
			f32 FRand();
            //---------------------------------------------------------
            /// Random In Range
            ///
            /// @param The minimum number that can be returned
            /// @param The maximum number that can be returned
            /// @return Random number within the range
            //---------------------------------------------------------
            template <typename T> inline T RandomInRange(T inMin, T inMax)
            {
                return inMin + (T)(FRand()*(inMax-inMin));
            }
            //---------------------------------------------------------
            /// Central Limit
            ///
            /// @param The minimum number that can be returned
            /// @param The maximum number that can be returned
            /// @return The average of a certain number of random values,
			/// within the passed range, as define by the
			/// Central Limit Theorem
            //---------------------------------------------------------
            template <typename T> inline T CentralLimit(T inMin, T inMax, u32 inudwPasses = 20)
            {
				CS_ASSERT(inudwPasses != 0, "Must have at least one pass");
				
				T TotalValue = 0;
				for(u32 i = 0; i < inudwPasses; ++i)
					TotalValue += RandomInRange<T>(inMin, inMax);
                return TotalValue /= inudwPasses;
            }
            //---------------------------------------------------------
            /// Seed Rand On Time
            ///
            /// Seed the random number generator based on the
            /// system clock.
            //---------------------------------------------------------
			void SeedRandOnTime();
            //---------------------------------------------------------
            /// Get next number
            ///
            /// pass in seed
            //---------------------------------------------------------
            f32 GetPseudoRandom(u32 inudwSeed);
			//---------------------------------------------------------
            /// Lerp
            ///
            /// @param Factor to interpolate by
            /// @param Min value in range
            /// @param Max value in range
            /// @return Linearly interpolated value
            //---------------------------------------------------------
			template <typename T> inline T Lerp(f32 infFactor, T inV1, T inV2)
			{
				return inV1 * (1.0f-infFactor) + inV2 * infFactor;
			}
            //---------------------------------------------------------
            /// Step
            ///
            /// @param Value
            /// @param Edge
            /// @param Min
            /// @param Max
            /// @return Value >= Edge -> Max Value < Edge -> Min
            //---------------------------------------------------------
            template <typename T> inline T Step(T inValue, T inEdge, T inMin, T inMax)
            {
                return (inValue < inEdge) ? inMin : inMax;
            }
			//---------------------------------------------------------
			/// Log2f
			///
			/// A cross compatible alternative to log2f() in math.h.
			///
			/// @param The same value as log2f();
			/// @return The same value as log2f();
			//---------------------------------------------------------
			f32 Log2f(f32 infValue);
			//---------------------------------------------------------
			/// Sign
			///
			/// @param Value to determine sign of
			/// @return Sign +1 if positive -1 if negative
			//---------------------------------------------------------
			template<typename T> inline T Sign(T inValue)
            {
                return (inValue < 0) ? (T)-1 : (inValue > 0);
            }
			//----------------------------------------------------------
			/// Clamp
			///
			/// @param Value to clamp between range
			/// @param Lower range value
			/// @param Higher range value
			/// @return Value within the given range
			//----------------------------------------------------------
			template<typename T> inline T Clamp(T inValue, T inMin, T inMax)
			{
				return std::max(inMin, std::min(inValue, inMax));
			}
            //---------------------------------------------------------
            /// Smooth Step
            ///
            /// @param Factor to interpolate by
            /// @param Min value in range
            /// @param Max value in range
            /// @return Cubicly interpolated value
            //---------------------------------------------------------
            template <typename T> inline T SmoothStep(f32 infFactor, T inV1, T inV2)
            {
                T t = Clamp((inV1 * -1.0f + infFactor)/(inV2 - inV1), 0.0f, 1.0f);
                
                return t*t*( t * -2.0f  + 3.0f);
            }
            //----------------------------------------------------------
            /// Is Approx Zero
            ///
            /// @param Value
            /// @return If the value is within epsilon of zero
            //----------------------------------------------------------
            bool IsApproxZero(f32 infValue);
			//----------------------------------------------------------
			/// Round
			///
			/// Rounds the floating point number to the nearest 
			/// integer. This is a symmetrical round
			///
			/// @param Floating point to round
			/// @return Rounded integer
			//----------------------------------------------------------
			s32 Round(f32 infValue);
            //----------------------------------------------------
            /// Mode 
            ///
            /// Calculate the most common value from the given
            /// data set
            ///
            /// @param Data set of type T
            /// @return Most common value of type T
            //----------------------------------------------------
            template <typename T> T Mode(std::vector<T>& inDataSet)
            {
                //Sort smallest to largest
                std::sort(inDataSet.begin(), inDataSet.end());
                
                T ModeValue = 0;
                T CurrentValue = 0;
                
                u32 udwGreatestCount = 0;
                u32 udwCurrentCount = 0;
                u32 udwSize = inDataSet.size();
                
                for(u32 i=0; i<udwSize; ++i)
                {
                    //If the height run has ended...
                    if(CurrentValue != inDataSet[i])
                    {
                        //...check if the run exceeds our greatest. 
                        if(udwCurrentCount > udwGreatestCount)
                        {
                            //This is the new mode
                            udwGreatestCount = udwCurrentCount;
                            ModeValue = CurrentValue;
                        }
                        
                        CurrentValue = inDataSet[i];
                        udwCurrentCount = 0;
                    }
                    udwCurrentCount++;				
                }
                
                return ModeValue;
            }
            //----------------------------------------------------
            /// Mean 
            ///
            /// Calculate the avearge value from the given
            /// data set
            ///
            /// @param Data set of type T
            /// @return Average value of type T
            //----------------------------------------------------
            template <typename T> T Mean(std::vector<T>& inDataSet)
            {
                if(inDataSet.empty())
                    return 0;
                
                T MeanValue = 0;
                
                u32 udwSize = inDataSet.size();
                for(u32 i=0; i<udwSize; ++i)
                {
                    MeanValue += inDataSet[i];
                }
                
                return MeanValue/udwSize;
            }
            //----------------------------------------------------
            /// Median 
            ///
            /// Calculate the mid point of the data set
            ///
            /// @param Data set of type T
            /// @return Average value of type T
            //----------------------------------------------------
            template <typename T> T Median(std::vector<T>& inDataSet)
            {
                if(inDataSet.empty())
                    return 0;
                
                //Sort smallest to largest
                std::sort(inDataSet.begin(), inDataSet.end());
                
                u32 udwMidPoint = std::floor(inDataSet.size() * 0.5f);
                
                return inDataSet[udwMidPoint];
            }
            //----------------------------------------------------
            /// Wrap
            ///
            /// Ensure the value is within the given range by
            /// wrapping at both ends
            ///
            /// @param Value
            /// @param Min
            /// @param Max
            /// @return Wrapped value
            //----------------------------------------------------
            template <typename T> inline T Wrap(T inValue, T inMin, T inMax)
            {
                if(inValue > inMax)
                {
                    return inMin;
                }
                else if(inValue < inMin)
                {
                    return inMax;
                }
                else
                {
                    return inValue;
                }
            }
            //----------------------------------------------------
            /// Normalised Range
            ///
            /// @param Value
            /// @param Min
            /// @param Max
            /// @return Value converted to the range 0 - 1
            //----------------------------------------------------
            template <typename T> inline T NormalisedRange(T inValue, T inMin, T inMax)
            {
				CS_ASSERT(inMax != inMin, "Divide by ZERO error");
                
                return (inValue - inMin) / (inMax - inMin);
            }
            //----------------------------------------------------
            /// Convert Range
            ///
            /// @param Value
            /// @param Min Range 1
            /// @param Max Range 1
            /// @param Min Range 2
            /// @param Max Range 2
            /// @return Value converted to range 2
            //----------------------------------------------------
            template <typename T> inline T ConvertRange(T inValue, T inOldMin, T inOldMax, T inNewMin, T inNewMax)
            {
                return (((inValue - inOldMin) * (inNewMax - inNewMin)) / (inOldMax - inOldMin)) + inNewMin;
            }
		};
	}
}

#endif
