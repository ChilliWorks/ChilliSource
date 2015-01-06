//
//  RandomImpl.h
//  Chilli Source
//  Created by Ian Copland on 06/01/2015.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2015 Tag Games Limited
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

#ifndef _CHILLISOURCE_CORE_MATH_RANDOMIMPL_H_
#define _CHILLISOURCE_CORE_MATH_RANDOMIMPL_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Random.h>

#include <algorithm>

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
            /// @author Ian Copland
            ///
            /// @return Returns the random number generator for the current
            /// thread.
            //------------------------------------------------------------------------------
            std::mt19937& GetRandomNumberGenerator();
            //------------------------------------------------------------------------------
            /// Specialisation of GenerateComponentwise() for values
            /// of type Vector2.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value, inclusive.
            /// @param The upper value, inclusive.
            ///
            /// @return The value in the given range.
            //------------------------------------------------------------------------------
            template <> Vector2 GenerateComponentwise(Vector2 in_lower, Vector2 in_upper);
            //------------------------------------------------------------------------------
            /// Specialisation of GenerateComponentwise() for values
            /// of type Vector3.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value, inclusive.
            /// @param The upper value, inclusive.
            ///
            /// @return The value in the given range.
            //------------------------------------------------------------------------------
            template <> Vector3 GenerateComponentwise(Vector3 in_lower, Vector3 in_upper);
            //------------------------------------------------------------------------------
            /// Specialisation of GenerateComponentwise() for values
            /// of type Vector4.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value, inclusive.
            /// @param The upper value, inclusive.
            ///
            /// @return The value in the given range.
            //------------------------------------------------------------------------------
            template <> Vector4 GenerateComponentwise(Vector4 in_lower, Vector4 in_upper);
            //------------------------------------------------------------------------------
            /// Specialisation of GenerateComponentwise() for values
            /// of type Matrix3.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value, inclusive.
            /// @param The upper value, inclusive.
            ///
            /// @return The value in the given range.
            //------------------------------------------------------------------------------
            template <> Matrix3 GenerateComponentwise(Matrix3 in_lower, Matrix3 in_upper);
            //------------------------------------------------------------------------------
            /// Specialisation of GenerateComponentwise() for values
            /// of type Matrix4.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value, inclusive.
            /// @param The upper value, inclusive.
            ///
            /// @return The value in the given range.
            //------------------------------------------------------------------------------
            template <> Matrix4 GenerateComponentwise(Matrix4 in_lower, Matrix4 in_upper);
            //------------------------------------------------------------------------------
            /// Specialisation of GenerateComponentwise() for values
            /// of type Quaternion.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value, inclusive.
            /// @param The upper value, inclusive.
            ///
            /// @return The value in the given range.
            //------------------------------------------------------------------------------
            template <> Quaternion GenerateComponentwise(Quaternion in_lower, Quaternion in_upper);
            //------------------------------------------------------------------------------
            /// Specialisation of GenerateComponentwise() for values
            /// of type Colour.
            ///
            /// @author Ian Copland
            ///
            /// @param The lower value, inclusive.
            /// @param The upper value, inclusive.
            ///
            /// @return The value in the given range.
            //------------------------------------------------------------------------------
            template <> Colour GenerateComponentwise(Colour in_lower, Colour in_upper);
            //------------------------------------------------------------------------------
            /// A simple class which uses template specialisation to determine which of the
            /// 3 random number generation types should be used: Integer, Float or Generic.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            template <typename TType, bool = std::is_integral<TType>::value, bool = std::is_floating_point<TType>::value> struct RandomNumberGenerator
            {
                //------------------------------------------------------------------------------
                /// Generates a value in the given range. Unspecialised, this uses a random
                /// float between 0.0 and 1.0 to interpolate between the min and max.
                ///
                /// @author Ian Copland
                ///
                /// @param The lower value.
                /// @param The upper value.
                ///
                /// @return The generated value.
                //------------------------------------------------------------------------------
				static TType Generate(TType in_lower, TType in_upper)
                {
					return in_lower + (in_upper - in_lower) * GenerateNormalised<f32>();
                }
            };
            //------------------------------------------------------------------------------
            /// A specialisation of the random number generator for integer types.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            template <typename TType> struct RandomNumberGenerator<TType, true, false>
            {
                //------------------------------------------------------------------------------
                /// Generates an integer value in the given range. Uses an integer distribution.
                ///
                /// @author Ian Copland
                ///
				/// @param The lower value.
				/// @param The upper value.
                ///
                /// @return The generated value.
                //------------------------------------------------------------------------------
				static TType Generate(TType in_lower, TType in_upper)
                {
					TType lower = std::min(in_lower, in_upper);
					TType upper = std::max(in_lower, in_upper);

					std::uniform_int_distribution<TType> distribution(lower, upper);
					return distribution(GetRandomNumberGenerator());
                }
            };
            //------------------------------------------------------------------------------
            /// A specialisation of the random number generator for floating point types.
            ///
            /// @author Ian Copland
            //------------------------------------------------------------------------------
            template <typename TType> struct RandomNumberGenerator<TType, false, true>
            {
                //------------------------------------------------------------------------------
                /// Generates an floating point value in the given range. Uses a floating point
                //// distribution.
                ///
                /// @author Ian Copland
                ///
                /// @param The minimum value.
                /// @param The maximum value.
                ///
                /// @return The generated value.
                //------------------------------------------------------------------------------
                static TType Generate(TType in_lower, TType in_upper)
                {
					TType lower = std::min(in_lower, in_upper);
					TType upper = std::max(in_lower, in_upper);

					std::uniform_real_distribution<TType> distribution(lower, upper);
					return distribution(GetRandomNumberGenerator());
                }
            };
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> TType Generate(TType in_lower, TType in_upper)
            {
				return RandomNumberGenerator<TType>::Generate(in_lower, in_upper);
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> TType GenerateNormalised()
            {
                return Generate<TType>(TType(0), TType(1));
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> GenericVector2<TType> GenerateDirection2D()
            {
                GenericVector2<TType> vector(Generate(TType(-1), TType(1)), Generate(TType(-1), TType(1)));
                vector.Normalise();
                return vector;
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> GenericVector3<TType> GenerateDirection3D()
            {
                GenericVector3<TType> vector(Generate(TType(-1), TType(1)), Generate(TType(-1), TType(1)), Generate(TType(-1), TType(1)));
                vector.Normalise();
                return vector;
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> GenericVector4<TType> GenerateDirection4D()
            {
                GenericVector4<TType> vector(Generate(TType(-1), TType(1)), Generate(TType(-1), TType(1)), Generate(TType(-1), TType(1)), Generate(TType(-1), TType(1)));
                vector.Normalise();
                return vector;
            }
            //------------------------------------------------------------------------------
            //------------------------------------------------------------------------------
            template <typename TType> TType GenerateComponentwise(TType in_lower, TType in_upper)
            {
                return Generate(in_lower, in_upper);
            }
        }
    }
}

#endif