//
//  Vector3.h
//  Chilli Source
//  Created by Ian Copland on 20/04/2014.
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

#ifndef _CHILLLISOURCE_CORE_MATH_VECTOR3_H_
#define _CHILLLISOURCE_CORE_MATH_VECTOR3_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------------------
		/// A generic three dimensional mathematical vector. Typically
		/// the type specific typedefs of this class would be used
		/// over direct use. For example:
		///		Vector3 -> GenericVector3<f32>
		///		Float3 -> GenericVector3<f32>
		///		Integer3 -> GenericVector3<s32>
		///
		/// @author Ian Copland
		//-------------------------------------------------------------------
		template <typename TType> class GenericVector3 final
		{
		public:
			//-----------------------------------------------------
			/// Constants
			//-----------------------------------------------------
			static const GenericVector3<TType> k_zero;
			static const GenericVector3<TType> k_one;
			static const GenericVector3<TType> k_unitPositiveX;
			static const GenericVector3<TType> k_unitNegativeX;
			static const GenericVector3<TType> k_unitPositiveY;
			static const GenericVector3<TType> k_unitNegativeY;
			static const GenericVector3<TType> k_unitPositiveZ;
			static const GenericVector3<TType> k_unitNegativeZ;
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A vector.
			///
			/// @return a normalised copy of the vector.
			//-----------------------------------------------------
			static GenericVector3<TType> Normalise(GenericVector3<TType> in_a);
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A vector.
			///
			/// @return An inversed copy of the vector.
			//-----------------------------------------------------
			static GenericVector3<TType> Inverse(GenericVector3<TType> in_a);
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A vector.
			///
			/// @return an absolute copy of the vector.
			//-----------------------------------------------------
			static GenericVector3<TType> Abs(GenericVector3<TType> in_a);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return A vector containing the minimum value for
			/// each component in the vectors.
			//-----------------------------------------------------
			static GenericVector3<TType> Min(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return A vector containing the maximum value for
			/// each component in the vectors.
			//-----------------------------------------------------
			static GenericVector3<TType> Max(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A vector.
			/// @param A minimum vector.
			/// @param A maximum vector.
			///
			/// @return A vector in which each component is clamped
			/// between the equivalent in the min and max vectors.
			//-----------------------------------------------------
			static GenericVector3<TType> Clamp(GenericVector3<TType> in_value, const GenericVector3<TType>& in_min, const GenericVector3<TType>& in_max);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			/// @param the interpolation factor.
			///
			/// @return The interpolated vector.
			//-----------------------------------------------------
			static GenericVector3<TType> Lerp(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b, f32 in_t);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return the dot product of these two vectors.
			//-----------------------------------------------------
			static TType DotProduct(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return the cross product of these two vectors
			//-----------------------------------------------------
			static GenericVector3<TType> CrossProduct(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author S Downie
			///
			/// @param  A vector.
			/// @param  Another vector.
			///
			/// @return The angle between the two vectors in
			/// radians.
			//-----------------------------------------------------
			static TType Angle(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// Rotates the vector by the given quaternion.
			///
			/// @author Ian Copland
			///
			/// @param The vector.
			/// @param The quaternion.
			///
			/// @return The new vector after rotation.
			//-----------------------------------------------------
			static GenericVector3<TType> Rotate(GenericVector3<TType> in_a, const GenericQuaternion<TType>& in_b);
            //-----------------------------------------------------
            /// calculates the result of transforming the vector by
            /// the given regular transform matrix. This is more
            /// efficient than standard matrix multiplication but
            /// will only work for trasform matrices where the point
            /// transformation is in the form:
            ///
            ///  Result = |x y z 1|| a b c 0 |
            ///                    | d e f 0 |
            ///                    | g h i 0 |
            ///                    | j k l 1 |
            ///
            /// This is the case for all non-projective transforms.
            ///
			/// @author Ian Copland
			///
			/// @param The vector.
			/// @param The transform matrix.
			//-----------------------------------------------------
			static GenericVector3<TType> Transform3x4(const GenericVector3<TType>& in_a, const GenericMatrix4<TType>& in_transform);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			//-----------------------------------------------------
			GenericVector3();
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param the x component.
			/// @param the y component.
			/// @param the z component.
			//-----------------------------------------------------
			explicit GenericVector3(TType in_x, TType in_y, TType in_z);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param A two component vector of the same type.
			/// @param the z component.
			//-----------------------------------------------------
			explicit GenericVector3(const GenericVector2<TType>& in_xy, TType in_z);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return A copy of the XY components of this vector
			/// as a Vector2.
			//-----------------------------------------------------
			GenericVector2<TType> XY() const;
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return the length of the vector
			//-----------------------------------------------------
			TType Length() const;
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return the length of the vector squared
			//-----------------------------------------------------
			TType LengthSquared() const;
			//-----------------------------------------------------
			/// Normalises the contents of this vector.
			///
			/// @author Ian Copland
			//-----------------------------------------------------
			void Normalise();
			//-----------------------------------------------------
			/// Sets this contents of this vector to its inverse.
			///
			/// @author Ian Copland
			//-----------------------------------------------------
			void Inverse();
			//-----------------------------------------------------
			/// Sets this vector to it's absolute value.
			///
			/// @author Ian Copland
			//-----------------------------------------------------
			void Abs();
            //-----------------------------------------------------
            /// Sets each component in this vector to which ever is
            /// smaller, itself or the equivelent in the given
            /// vector.
            ///
			/// @author Ian Copland
			///
			/// @param The other vector.
			//-----------------------------------------------------
			void Min(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
            /// Sets each component in this vector to which ever is
            /// greater, itself or the equivelent in the given
            /// vector.
            ///
			/// @author Ian Copland
			///
			/// @param The other vector.
			//-----------------------------------------------------
			void Max(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
            /// Clamps each component in this vector between the
            /// values described by the min and max vectors.
            ///
			/// @author Ian Copland
			///
			/// @param A minimum vector.
			/// @param A maximum vector.
			//-----------------------------------------------------
			void Clamp(const GenericVector3<TType>& in_min, const GenericVector3<TType>& in_max);
			//-----------------------------------------------------
            /// Sets the contents of the vector to the result of
            /// interpolating between this and the given vector
            /// with the given interpolation factor.
            ///
			/// @author Ian Copland
			///
			/// @param The other vector.
			/// @param the interpolation factor.
			//-----------------------------------------------------
			void Lerp(const GenericVector3<TType>& in_b, f32 in_t);
            //-----------------------------------------------------
            /// Sets this vector to the result of the cross product
            /// between itself and the given vector.
            ///
			/// @author Ian Copland
			///
			/// @param The other vector.
			//-----------------------------------------------------
			void CrossProduct(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// Rotates this vector by the given quaternion.
			///
			/// @author Ian Copland
			///
			/// @param The quaternion to rotate by.
			//-----------------------------------------------------
			void Rotate(const GenericQuaternion<TType>& in_b);
            //-----------------------------------------------------
            /// Sets this vector to the result of transforming by
            /// the given regular transform matrix. This is more
            /// efficient than standard matrix multiplication but
            /// will only work for trasform matrices where the point
            /// transformation is in the form:
            ///
            ///  Result = |x y z 1|| a b c 0 |
            ///                    | d e f 0 |
            ///                    | g h i 0 |
            ///                    | j k l 1 |
            ///
            /// This is the case for all non-projective transforms.
            ///
			/// @author Ian Copland
			///
			/// @param The transform matrix.
			//-----------------------------------------------------
			void Transform3x4(const GenericMatrix4<TType>& in_transform);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after the given vector has been
			/// added to it.
			//-----------------------------------------------------
			GenericVector3<TType>& operator+=(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after the given vector has been
			/// subtracted from it.
			//-----------------------------------------------------
			GenericVector3<TType>& operator-=(const GenericVector3<TType>& in_b);
            //-----------------------------------------------------
            /// @author S Downie
			///
			/// @return This vector after it has been negated
            //-----------------------------------------------------
            GenericVector3<TType>& operator-();
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after it has been multiplied
			/// by the given vector.
			//-----------------------------------------------------
			GenericVector3<TType>& operator*=(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after it has been divided
			/// by the given vector.
			//-----------------------------------------------------
			GenericVector3<TType>& operator/=(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A matrix.
			///
			/// @return This vector after it has been multiplied
			/// by the given matrix.
			//-----------------------------------------------------
			GenericVector3<TType>& operator*=(const GenericMatrix3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A matrix.
			///
			/// @return This vector after it has been multiplied
			/// by the given matrix.
			//-----------------------------------------------------
			GenericVector3<TType>& operator*=(const GenericMatrix4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar
			///
			/// @return This vector after it has been multiplied
			/// by the given scalar.
			//-----------------------------------------------------
			GenericVector3<TType>& operator*=(TType in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar.
			///
			/// @return This vector after it has been divided
			/// by the given scalar.
			//-----------------------------------------------------
			GenericVector3<TType>& operator/=(TType in_b);
            
			TType x;
			TType y;
			TType z;
		};
        
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A + B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator+(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A - B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator-(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A / B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator/(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The matrix B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& in_a, const GenericMatrix3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The matrix B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& in_a, const GenericMatrix4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The scalar B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(GenericVector3<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The scalar A.
		/// @param The vector B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(TType in_a, GenericVector3<TType> in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The scalar B.
		///
		/// @return The result of A / B.
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator/(GenericVector3<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return Whether or not the two vectors are equal.
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return Whether or not the two vectors are in-equal.
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
	}
}

//----------------------------------------------------
// These are included here to avoid circular inclusion
// issues. At this stage the class has been defined
// which is enough for the classes included to use it.
//----------------------------------------------------
#include <ChilliSource/Core/Math/Matrix3.h>
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector2.h>

#include <algorithm>
#include <cmath>

namespace ChilliSource
{
	namespace Core
	{
		template <typename TType> const GenericVector3<TType> GenericVector3<TType>::k_zero(0, 0, 0);
		template <typename TType> const GenericVector3<TType> GenericVector3<TType>::k_one(1, 1, 1);
		template <typename TType> const GenericVector3<TType> GenericVector3<TType>::k_unitPositiveX(1, 0, 0);
		template <typename TType> const GenericVector3<TType> GenericVector3<TType>::k_unitNegativeX(-1, 0, 0);
		template <typename TType> const GenericVector3<TType> GenericVector3<TType>::k_unitPositiveY(0, 1, 0);
		template <typename TType> const GenericVector3<TType> GenericVector3<TType>::k_unitNegativeY(0, -1, 0);
		template <typename TType> const GenericVector3<TType> GenericVector3<TType>::k_unitPositiveZ(0, 0, 1);
		template <typename TType> const GenericVector3<TType> GenericVector3<TType>::k_unitNegativeZ(0, 0, -1);
        
        //-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Normalise(GenericVector3<TType> in_a)
		{
			in_a.Normalise();
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Inverse(GenericVector3<TType> in_a)
		{
			in_a.Inverse();
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Abs(GenericVector3<TType> in_a)
		{
			in_a.Abs();
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Min(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b)
		{
			in_a.Min(in_b);
            return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Max(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b)
		{
			in_a.Max(in_b);
            return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Clamp(GenericVector3<TType> in_value, const GenericVector3<TType>& in_min, const GenericVector3<TType>& in_max)
		{
			in_value.Clamp(in_min, in_max);
            return in_value;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Lerp(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b, f32 in_t)
		{
			in_a.Lerp(in_b, in_t);
            return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector3<TType>::DotProduct(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b)
		{
			return (in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::CrossProduct(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b)
		{
			GenericVector3<TType> c;
			c.x = in_a.y * in_b.z - in_a.z * in_b.y;
			c.y = -in_a.x * in_b.z + in_a.z * in_b.x;
			c.z = in_a.x * in_b.y - in_a.y * in_b.x;
			return c;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector3<TType>::Angle(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b)
		{
			TType dot = DotProduct(in_a, in_b);
			TType totalMagnitude = in_a.Length() * in_b.Length();
            
			if (totalMagnitude == 0)
			{
				totalMagnitude = 1;
			}
            
			//This just caps the quotient because in practical use, sometimes dot and totalMagnitude yield a very small number above 1.0,
			//e.g. 1.00003 when angles tend towards 0 degrees
			f32 ratio = dot / totalMagnitude;
			if (ratio > 1)
			{
				ratio = 1;
			}
			else if (ratio < -1)
			{
				ratio = -1;
			}
            
			return std::acos(ratio);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Rotate(GenericVector3<TType> in_a, const GenericQuaternion<TType>& in_b)
		{
			in_a.Rotate(in_b);
            return in_a;
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericVector3<TType> GenericVector3<TType>::Transform3x4(const GenericVector3<TType>& in_a, const GenericMatrix4<TType>& in_transform)
        {
            GenericVector3<TType> b;
			b.x = in_a.x * in_transform.m[0] + in_a.y * in_transform.m[4] + in_a.z * in_transform.m[8] + in_transform.m[12];
			b.y = in_a.x * in_transform.m[1] + in_a.y * in_transform.m[5] + in_a.z * in_transform.m[9] + in_transform.m[13];
			b.z = in_a.x * in_transform.m[2] + in_a.y * in_transform.m[6] + in_a.z * in_transform.m[10] + in_transform.m[14];
            return b;
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>::GenericVector3()
        : x(0), y(0), z(0)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>::GenericVector3(TType in_x, TType in_y, TType in_z)
        : x(in_x), y(in_y), z(in_z)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>::GenericVector3(const GenericVector2<TType>& in_xy, TType in_z)
        : x(in_xy.x), y(in_xy.y), z(in_z)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector3<TType>::XY() const
		{
			return GenericVector2<TType>(x, y);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector3<TType>::LengthSquared() const
		{
			return (x * x + y * y + z * z);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector3<TType>::Length() const
		{
			return (std::sqrt(x * x + y * y + z * z));
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector3<TType>::Normalise()
		{
            TType length = Length();
            if (length != 0)
            {
                TType invLength = 1 / length;
                x *= invLength;
                y *= invLength;
                z *= invLength;
            }
		}
        
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector3<TType>::Inverse()
		{
			if (x != 0)
			{
				x = 1 / x;
			}
            
			if (y != 0)
			{
				y = 1 / y;
			}
            
			if (z != 0)
			{
				z = 1 / z;
			}
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector3<TType>::Abs()
		{
			x = std::abs(x);
			y = std::abs(y);
			z = std::abs(z);
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector3<TType>::Min(const GenericVector3<TType>& in_b)
        {
            x = std::min(x, in_b.x);
			y = std::min(y, in_b.y);
			z = std::min(z, in_b.z);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector3<TType>::Max(const GenericVector3<TType>& in_b)
        {
            x = std::max(x, in_b.x);
			y = std::max(y, in_b.y);
			z = std::max(z, in_b.z);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector3<TType>::Clamp(const GenericVector3<TType>& in_min, const GenericVector3<TType>& in_max)
        {
            x = std::min(std::max(x, in_min.x), in_max.x);
			y = std::min(std::max(y, in_min.y), in_max.y);
			z = std::min(std::max(z, in_min.z), in_max.z);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector3<TType>::Lerp(const GenericVector3<TType>& in_b, f32 in_t)
        {
            f32 t = std::min(std::max(in_t, 0.0f), 1.0f);
			*this = (*this + t * (in_b - *this));
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector3<TType>::CrossProduct(const GenericVector3<TType>& in_b)
        {
            GenericVector3<TType> c = *this;
			x = c.y * in_b.z - c.z * in_b.y;
			y = -c.x * in_b.z + c.z * in_b.x;
			z = c.x * in_b.y - c.y * in_b.x;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector3<TType>::Rotate(const GenericQuaternion<TType>& in_b)
        {
            GenericVector3<TType> xyzA(in_b.x, in_b.y, in_b.z);
			GenericVector3<TType> t = ((TType)2) * CrossProduct(xyzA, *this);
			*this = *this + in_b.w * t + CrossProduct(xyzA, t);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector3<TType>::Transform3x4(const GenericMatrix4<TType>& in_transform)
        {
            GenericVector3<TType> b = *this;
			x = b.x * in_transform.m[0] + b.y * in_transform.m[4] + b.z * in_transform.m[8] + in_transform.m[12];
			y = b.x * in_transform.m[1] + b.y * in_transform.m[5] + b.z * in_transform.m[9] + in_transform.m[13];
			z = b.x * in_transform.m[2] + b.y * in_transform.m[6] + b.z * in_transform.m[10] + in_transform.m[14];
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator+=(const GenericVector3<TType>& in_b)
		{
			x += in_b.x;
			y += in_b.y;
			z += in_b.z;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator-=(const GenericVector3<TType>& in_b)
		{
			x -= in_b.x;
			y -= in_b.y;
			z -= in_b.z;
			return *this;
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator-()
        {
            x = -x;
			y = -y;
			z = -z;
			return *this;
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator*=(const GenericVector3<TType>& in_b)
		{
			x *= in_b.x;
			y *= in_b.y;
			z *= in_b.z;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator/=(const GenericVector3<TType>& in_b)
		{
			x /= in_b.x;
			y /= in_b.y;
			z /= in_b.z;
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator*=(const GenericMatrix3<TType>& in_b)
		{
			GenericVector3<TType> c = *this;
			x = c.x * in_b.m[0] + c.y * in_b.m[3] + c.z * in_b.m[6];
			y = c.x * in_b.m[1] + c.y * in_b.m[4] + c.z * in_b.m[7];
			z = c.x * in_b.m[2] + c.y * in_b.m[5] + c.z * in_b.m[8];
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator*=(const GenericMatrix4<TType>& in_b)
		{
			GenericVector3<TType> c = *this;
			x = c.x * in_b.m[0] + c.y * in_b.m[4] + c.z * in_b.m[8] + in_b.m[12];
			y = c.x * in_b.m[1] + c.y * in_b.m[5] + c.z * in_b.m[9] + in_b.m[13];
			z = c.x * in_b.m[2] + c.y * in_b.m[6] + c.z * in_b.m[10] + in_b.m[14];
			TType oneOverW = 1 / (c.x * in_b.m[3] + c.y * in_b.m[7] + c.z * in_b.m[11] + in_b.m[15]);
			*this *= oneOverW;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator*=(TType in_b)
		{
			x *= in_b;
			y *= in_b;
			z *= in_b;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator/=(TType in_b)
		{
			x /= in_b;
			y /= in_b;
			z /= in_b;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator+(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b)
		{
			return (in_a += in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator-(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b)
		{
			return (in_a -= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b)
		{
			return (in_a *= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator/(GenericVector3<TType> in_a, const GenericVector3<TType>& in_b)
		{
			return (in_a /= in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& in_a, const GenericMatrix3<TType>& in_b)
		{
			GenericVector3<TType> c;
			c.x = in_a.x * in_b.m[0] + in_a.y * in_b.m[3] + in_a.z * in_b.m[6];
			c.y = in_a.x * in_b.m[1] + in_a.y * in_b.m[4] + in_a.z * in_b.m[7];
			c.z = in_a.x * in_b.m[2] + in_a.y * in_b.m[5] + in_a.z * in_b.m[8];
			return c;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			GenericVector3<TType> c;
			c.x = in_a.x * in_b.m[0] + in_a.y * in_b.m[4] + in_a.z * in_b.m[8] + in_b.m[12];
			c.y = in_a.x * in_b.m[1] + in_a.y * in_b.m[5] + in_a.z * in_b.m[9] + in_b.m[13];
			c.z = in_a.x * in_b.m[2] + in_a.y * in_b.m[6] + in_a.z * in_b.m[10] + in_b.m[14];
			TType oneOverW = 1 / (in_a.x * in_b.m[3] + in_a.y * in_b.m[7] + in_a.z * in_b.m[11] + in_b.m[15]);
			c *= oneOverW;
			return c;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(GenericVector3<TType> in_a, TType in_b)
		{
			return (in_a *= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(TType in_a, GenericVector3<TType> in_b)
		{
			return (in_b *= in_a);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> operator/(GenericVector3<TType> in_a, TType in_b)
		{
			return (in_a /= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b)
		{
			return (in_a.x == in_b.x && in_a.y == in_b.y && in_a.z == in_b.z);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b)
		{
			return !(in_a == in_b);
		}
	}
}

#endif
