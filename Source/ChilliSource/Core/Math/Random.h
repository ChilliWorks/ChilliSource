//
//  Random.h
//  Chilli Source
//  Created by Ian Copland on 24/10/2014.
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

#ifndef _CHILLISOURCE_MATH_RANDOM_H_
#define _CHILLISOURCE_MATH_RANDOM_H_

#include <ChilliSource/ChilliSource.h>

#include <limits>
#include <random>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------------------
		/// A collection of thread-safe methods which can be used to generate 
		/// pseudo-random numbers.
		///
		/// @author Ian Copland
		//-----------------------------------------------------------------------
		namespace Random
		{
			//----------------------------------------------------------------
			/// Generates a psuedo-random integer number in the given range. 
			/// By default this will return a value between the numeric limits
			/// of the type. This will only work for integer types, i.e u32,
			/// s64 and s8.
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @param [Optional] The minimum value, inclusive.
			/// @param [Optional] The maximum value, inclusive.
			///
			/// @return A value within the range.
			//----------------------------------------------------------------
			template <typename TType> TType GenerateInt(TType in_min = std::numeric_limits<TType>::min(), TType in_max = std::numeric_limits<TType>::max());
			//----------------------------------------------------------------
			/// Generates a psuedo-random real number in the given range. By
			/// default the range is between 0.0 and 1.0. This will only work
			/// for real types, i.e f32 and f64.
			///
			/// This is thread safe.
			///
			/// @author Ian Copland
			///
			/// @param [Optional] The minimum value, inclusive.
			/// @param [Optional] The maximum value, inclusive.
			///
			/// @return A value within the range.
			//----------------------------------------------------------------
			template <typename TType> TType GenerateReal(TType in_min = TType(0), TType in_max = TType(1));
			//----------------------------------------------------------------
			/// Generates a psuedo-random direction vector in 2 dimensions with
			/// uniform distribution. 
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return A direction vector.
			//----------------------------------------------------------------
			template <typename TType> GenericVector2<TType> GenerateDirection2D();
			//----------------------------------------------------------------
			/// Generates a psuedo-random direction vector in 3 dimensions with
			/// uniform distribution. 
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return A direction vector.
			//----------------------------------------------------------------
			template <typename TType> GenericVector3<TType> GenerateDirection3D();
			//----------------------------------------------------------------
			/// Generates a psuedo-random direction vector in 4 dimensions with
			/// uniform distribution. 
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return A direction vector.
			//----------------------------------------------------------------
			template <typename TType> GenericVector4<TType> GenerateDirection4D();
			//----------------------------------------------------------------
			/// Generates a psuedo-random direction vector in 4 dimensions with
			/// uniform distribution. 
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return A direction vector.
			//----------------------------------------------------------------
			template <typename TType> GenericVector4<TType> GenerateDirection4D();
			//----------------------------------------------------------------
			/// Generates a psuedo-random value between the two given values. 
			/// This will work for any type that can be reasonably be multiplied 
			/// by a real number.
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @param The first value.
			/// @param The second value.
			///
			/// @return A direction vector.
			//----------------------------------------------------------------
			template <typename TType> TType GenerateInRange(TType in_first, TType in_second);
			//----------------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return Returns the random number generator for the current
			/// thread.
			//----------------------------------------------------------------
			std::mt19937& GetRandomNumberGenerator();

			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> TType GenerateInt(TType in_min, TType in_max)
			{
				std::uniform_int_distribution<TType> distribution(in_min, in_max);
				return distribution(GetRandomNumberGenerator());
			}
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> TType GenerateReal(TType in_min, TType in_max)
			{
				std::uniform_real_distribution<TType> distribution(in_min, in_max);
				return distribution(GetRandomNumberGenerator());
			}
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> GenericVector2<TType> GenerateDirection2D()
			{
				std::uniform_real_distribution<TType> distribution(-1, 1);
				std::mt19937& rng = GetRandomNumberGenerator();

				GenericVector2<TType> vector(distribution(rng), distribution(rng));
				vector.Normalise();
				return vector;
			}
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> GenericVector3<TType> GenerateDirection3D()
			{
				std::uniform_real_distribution<TType> distribution(-1, 1);
				std::mt19937& rng = GetRandomNumberGenerator();

				GenericVector3<TType> vector(distribution(rng), distribution(rng), distribution(rng));
				vector.Normalise();
				return vector;
			}
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> GenericVector4<TType> GenerateDirection4D()
			{
				std::uniform_real_distribution<TType> distribution(-1, 1);
				std::mt19937& rng = GetRandomNumberGenerator();

				GenericVector4<TType> vector(distribution(rng), distribution(rng), distribution(rng), distribution(rng));
				vector.Normalise();
				return vector;
			}
			//----------------------------------------------------------------
			//----------------------------------------------------------------
			template <typename TType> TType GenerateInRange(TType in_first, TType in_second)
			{
				return in_first + (in_second - in_first) * GenerateReal<f32>();
			}
		}
	}
}

#endif