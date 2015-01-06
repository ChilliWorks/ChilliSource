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

#ifndef _CHILLISOURCE_CORE_MATH_RANDOM_H_
#define _CHILLISOURCE_CORE_MATH_RANDOM_H_

#include <ChilliSource/ChilliSource.h>

#include <limits>
#include <random>

namespace ChilliSource
{
	namespace Core
	{
		//------------------------------------------------------------------------------
		/// A collection of thread-safe methods which can be used to generate pseudo
        /// random numbers.
		///
		/// @author Ian Copland
		//------------------------------------------------------------------------------
		namespace Random
        {
            //------------------------------------------------------------------------------
            /// Generates a psuedo-random value of the requested type within the given range.
            /// Defaults to the maximum possible range for the given type.
            ///
            /// This is thread-safe.
            ///
            /// @author Ian Copland
            ///
            /// @param [Optional] The lower value, inclusive.
            /// @param [Optional] The upper value, inclusive.
            ///
            /// @return A value within the range.
            //------------------------------------------------------------------------------
            template <typename TType> TType Generate(TType in_lower = std::numeric_limits<TType>::lowest(), TType in_upper = std::numeric_limits<TType>::max());
			//------------------------------------------------------------------------------
			/// Generates a psuedo-random number in the range between 0.0 and 1.0 for the
            /// given type.
			///
			/// This is thread safe.
			///
			/// @author Ian Copland
			///
			/// @return A value within the range.
			//------------------------------------------------------------------------------
			template <typename TType> TType GenerateNormalised();
			//------------------------------------------------------------------------------
			/// Generates a psuedo-random direction vector in 2 dimensions with uniform
            /// distribution.
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return A direction vector.
			//------------------------------------------------------------------------------
			template <typename TType> GenericVector2<TType> GenerateDirection2D();
			//------------------------------------------------------------------------------
			/// Generates a psuedo-random direction vector in 3 dimensions with uniform
            /// distribution.
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return A direction vector.
			//------------------------------------------------------------------------------
			template <typename TType> GenericVector3<TType> GenerateDirection3D();
			//------------------------------------------------------------------------------
			/// Generates a psuedo-random direction vector in 4 dimensions with uniform
            /// distribution.
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return A direction vector.
			//------------------------------------------------------------------------------
			template <typename TType> GenericVector4<TType> GenerateDirection4D();
			//------------------------------------------------------------------------------
			/// Generates a psuedo-random direction vector in 4 dimensions with uniform
            /// distribution.
			///
			/// This is thread-safe.
			///
			/// @author Ian Copland
			///
			/// @return A direction vector.
			//------------------------------------------------------------------------------
			template <typename TType> GenericVector4<TType> GenerateDirection4D();
            //------------------------------------------------------------------------------
            /// Generates a psuedo-random value between the two given values. If the value
            /// has multiple components, each will be randomised individually, otherwise
            /// this is identical to Generate().
            ///
            /// This is thread-safe.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value, inclusive.
            /// @param The upper value, inclusive.
            ///
            /// @return The value in the given range.
            //------------------------------------------------------------------------------
            template <typename TType> TType GenerateComponentwise(TType in_lower = std::numeric_limits<TType>::lowest(), TType in_upper = std::numeric_limits<TType>::max());
		}
	}
}

#include <ChilliSource/Core/Math/RandomImpl.h>

#endif