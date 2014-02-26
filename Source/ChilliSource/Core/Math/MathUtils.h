/*
 *  MathUtils.h
 *  moFlo
 *
 *  Created by Stuart McGaw on 22/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_MATH_MATH_HELPER_H_
#define _MO_FLO_CORE_MATH_MATH_HELPER_H_

#include <ChilliSource/ChilliSource.h>

#include <cmath>
#include <vector>

namespace ChilliSource
{
	namespace Core 
	{
        const f32 kPI = 3.14159265358979323846264338327950288f;
        const f32 kApproxZero = 0.0001192092896f; 
        
		///////////////////////////////////////////////////////////////////
		/// Description:
		///
		/// Common math utilities
		///////////////////////////////////////////////////////////////////
		class CMathUtils
		{
		public:
			
            //---------------------------------------------------------
            /// Next Power Of Two
            ///
            /// @param Non-power of two value
            /// @return The closest higher power of two
            //---------------------------------------------------------
			static u32 NextPowerOfTwo(u32 inX);
            //---------------------------------------------------------
            /// Degrees To Radians
            ///
            /// @param Angle in degrees
            /// @return Angle in radians
            //---------------------------------------------------------
			static f32 DegToRad(f32 infAngle);
            //---------------------------------------------------------
            /// Radians To Degrees
            ///
            /// @param Angle in radians
            /// @return Angle in degrees
            //---------------------------------------------------------
			static f32 RadToDeg(f32 infAngle);
            //---------------------------------------------------------
            /// fRand
            ///
            /// @return Normalised random number between 0 and 1
            //---------------------------------------------------------
			static f32 FRand();
            //---------------------------------------------------------
            /// Random In Range
            ///
            /// @param The minimum number that can be returned
            /// @param The maximum number that can be returned
            /// @return Random number within the range
            //---------------------------------------------------------
            template <typename T> inline static T RandomInRange(T inMin, T inMax)
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
            template <typename T> inline static T CentralLimit(T inMin, T inMax, u32 inudwPasses = 20)
            {
				MOFLOW_ASSERT(inudwPasses != 0, "Must have at least one pass");
				
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
			static void SeedRandOnTime();            
            //---------------------------------------------------------
            /// Get next number
            ///
            /// pass in seed
            //---------------------------------------------------------
            static f32 GetPseudoRandom(u32 inudwSeed);
			//---------------------------------------------------------
            /// Lerp
            ///
            /// @param Factor to interpolate by
            /// @param Min value in range
            /// @param Max value in range
            /// @return Linearly interpolated value
            //---------------------------------------------------------
			template <typename T> inline static T Lerp(f32 infFactor, T inV1, T inV2)
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
            template <typename T> inline static T Step(T inValue, T inEdge, T inMin, T inMax)
            {
                return (inValue < inEdge) ? inMin : inMax;
            }
            //---------------------------------------------------------
            /// Smooth Step
            ///
            /// @param Factor to interpolate by
            /// @param Min value in range
            /// @param Max value in range
            /// @return Cubicly interpolated value
            //---------------------------------------------------------
            template <typename T> inline static T SmoothStep(f32 infFactor, T inV1, T inV2)
            {
                T t = CMathUtils::Clamp((inV1 * -1.0f + infFactor)/(inV2 - inV1), 0.0f, 1.0f);
                
                return t*t*( t * -2.0f  + 3.0f);
            }
			//---------------------------------------------------------
			/// Log2f
			///
			/// A cross compatible alternative to log2f() in math.h.
			///
			/// @param The same value as log2f();
			/// @return The same value as log2f();
			//---------------------------------------------------------
			static f32 Log2f(f32 infValue);
			//---------------------------------------------------------
			/// Sign
			///
			/// @param Value to determine sign of
			/// @return Sign +1 if positive -1 if negative
			//---------------------------------------------------------
			template<typename T> inline static T Sign(T inValue)
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
			template<typename T> inline static T Clamp(T inValue, T inMin, T inMax)
			{
				return std::max(inMin, std::min(inValue, inMax));
			}
            //----------------------------------------------------------
            /// Is Approx Zero
            ///
            /// @param Value
            /// @return If the value is within epsilon of zero
            //----------------------------------------------------------
            static bool IsApproxZero(f32 infValue);
			//----------------------------------------------------------
			/// Round
			///
			/// Rounds the floating point number to the nearest 
			/// integer. This is a symmetrical round
			///
			/// @param Floating point to round
			/// @return Rounded integer
			//----------------------------------------------------------
			static s32 Round(f32 infValue);
			//----------------------------------------------------------
			/// Min
			///
			/// @param Value of type T
			/// @param Value of type T
			/// @return Minimum of two values
			//----------------------------------------------------------
			template<typename T> inline static T Min(T inValue1, T inValue2)
			{
				return inValue1 < inValue2 ? inValue1 : inValue2;
			}
			//----------------------------------------------------------
			/// Max
			///
			/// @param Value of type T
			/// @param Value of type T
			/// @return Maximum of two values
			//----------------------------------------------------------
			template<typename T> inline static T Max(T inValue1, T inValue2)
			{
				return inValue2 < inValue1 ? inValue1 : inValue2;
			}
            //----------------------------------------------------
            /// Mode 
            ///
            /// Calculate the most common value from the given
            /// data set
            ///
            /// @param Data set of type T
            /// @return Most common value of type T
            //----------------------------------------------------
            template <typename T> static T Mode(std::vector<T>& inDataSet)
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
            template <typename T> static T Mean(std::vector<T>& inDataSet)
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
            template <typename T> static T Median(std::vector<T>& inDataSet)
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
            template <typename T> inline static T Wrap(T inValue, T inMin, T inMax)
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
            template <typename T> inline static T NormalisedRange(T inValue, T inMin, T inMax)
            {
                if(inMax == inMin)
                    return 1.0f;
                
                return Clamp( (inValue - inMin) / (inMax - inMin), 0.0f, 1.0f);
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
            template <typename T> inline static T ConvertRange(T inValue, T inOldMin, T inOldMax, T inNewMin, T inNewMax)
            {
                return (((inValue - inOldMin) * (inNewMax - inNewMin)) / (inOldMax - inOldMin)) + inNewMin;
            }
		};
	}
}

#endif
