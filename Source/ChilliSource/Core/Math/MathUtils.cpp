/*
 *  MathUtils.cpp
 *  moFlo
 *
 *  Created by Stuart McGaw on 22/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Math/MathUtils.h>
#include <ctime>

namespace moFlo
{
	namespace Core 
	{
		//---------------------------------------------------------
		/// Next Power Of Two
		///
		/// @param Non-power of two value
		/// @return The closest higher power of two
		//---------------------------------------------------------
		u32 CMathUtils::NextPowerOfTwo(u32 inX)
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
		f32 CMathUtils::DegToRad(f32 infAngle)
		{
			return infAngle * (kPI/180.0f);
		}
		//---------------------------------------------------------
		/// Radians To Degrees
		///
		/// @param Angle in radians
		/// @return Angle in degrees
		//---------------------------------------------------------
		f32 CMathUtils::RadToDeg(f32 infAngle)
		{
			return infAngle * (180.0f/kPI);
		}
		//---------------------------------------------------------
		/// fRand
		///
		/// @return Normalised random number between 0 and 1
		//---------------------------------------------------------
		f32 CMathUtils::FRand()
		{
			return (f32)rand()/(f32)RAND_MAX;
		}
		//---------------------------------------------------------
		/// Seed Rand On Time
		///
		/// Seed the random number generator based on the
		/// system clock.
		//---------------------------------------------------------
		void CMathUtils::SeedRandOnTime()
		{
			srand(clock());
		}
        //---------------------------------------------------------
        /// Get next number
        ///
        /// pass in seed
        //---------------------------------------------------------
        f32 CMathUtils::GetPseudoRandom(u32 inudwSeed)
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
		f32 CMathUtils::Log2f(f32 infValue)
		{
			return (f32)log(infValue)/(f32)log(2.0f);
		}
        //----------------------------------------------------------
        /// Is Approx Zero
        ///
        /// @param Value
        /// @return If the value is within epsilon of zero
        //----------------------------------------------------------
        bool CMathUtils::IsApproxZero(f32 infValue)
        {
            return (infValue > -kApproxZero) && (infValue < kApproxZero); 
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
		s32 CMathUtils::Round(f32 infValue)
		{
			return (s32)((infValue > 0.0f) ? std::floor(infValue + 0.5f) : std::ceil(infValue - 0.5f));
		}
	}
}
