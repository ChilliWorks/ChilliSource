//
//  Vector2.h
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

#ifndef _CHILLISOURCE_CORE_MATH_VECTOR2_H_
#define _CHILLISOURCE_CORE_MATH_VECTOR2_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
		//-----------------------------------------------------------
		/// A generic two dimensional mathematical vector. Typically
		/// the type specific typedefs of this class would be used
		/// over direct use. For example:
		///		Vector2 -> GenericVector2<f32>
		///		Float2 -> GenericVector2<f32>
		///		Integer2 -> GenericVector2<s32>
		///
		/// @author Ian Copland
		//-----------------------------------------------------------
		template <typename TType> class GenericVector2 final
		{
		public:
			//-----------------------------------------------------
			/// Constants
			//-----------------------------------------------------
			static const GenericVector2<TType> k_zero;
			static const GenericVector2<TType> k_one;
			static const GenericVector2<TType> k_unitPositiveX;
			static const GenericVector2<TType> k_unitNegativeX;
			static const GenericVector2<TType> k_unitPositiveY;
			static const GenericVector2<TType> k_unitNegativeY;
            //-----------------------------------------------------
			/// @author Ian Copland
			///
            /// @param A vector.
            ///
			/// @return a normalised copy of the vector.
			//-----------------------------------------------------
			static GenericVector2<TType> Normalise(GenericVector2<TType> in_a);
            //-----------------------------------------------------
			/// @author Ian Copland
			///
            /// @param A vector.
			///
			/// @return An inversed copy of the vector.
			//-----------------------------------------------------
			static GenericVector2<TType> Inverse(GenericVector2<TType> in_a);
            //-----------------------------------------------------
			/// @author Ian Copland
			///
            /// @param A vector.
			///
			/// @return an absolute copy of the vector.
			//-----------------------------------------------------
			static GenericVector2<TType> Abs(GenericVector2<TType> in_a);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return A vector containing the minimum value for
			/// each component in each vector.
			//-----------------------------------------------------
			static GenericVector2<TType> Min(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return A vector containing the maximum value for
			/// each component in each vector.
			//-----------------------------------------------------
			static GenericVector2<TType> Max(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A vector.
			/// @param The minimum vector.
			/// @param The maximum vector.
			///
			/// @return A vector in which each component is clamped
			/// between the equivalent in the min and max vectors.
			//-----------------------------------------------------
			static GenericVector2<TType> Clamp(GenericVector2<TType> in_value, const GenericVector2<TType>& in_min, const GenericVector2<TType>& in_max);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			/// @param The interpolation factor.
			///
			/// @return The interpolated vector.
			//-----------------------------------------------------
			static GenericVector2<TType> Lerp(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b, f32 in_t);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return the dot product of these two vectors.
			//-----------------------------------------------------
			static TType DotProduct(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
			//-----------------------------------------------------
			/// Treats the two vectors as 3D with the z-component
			/// set to zero to calculate the z component of the
			/// cross product.
			///
			/// @author Ian Copland
			///
			/// @param  A vector.
			/// @param  Another vector.
			///
			/// @return The z component of the cross product.
			//-----------------------------------------------------
			static TType CrossProductZ(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
			//-----------------------------------------------------
			/// @author S Downie
			///
			/// @param  A vector.
			/// @param  Another vector.
			///
			/// @return The angle between the two vectors in
			/// radians.
			//-----------------------------------------------------
			static TType Angle(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
            //-----------------------------------------------------
            /// Returns a vector that is the result of rotating the
            /// given vector by the given angle.
            ///
            /// @author Ian Copland
            ///
            /// @param The vector.
            /// @param The angle to rotate through in radians.
            ///
            /// @return The new vector after rotation.
            //-----------------------------------------------------
            static GenericVector2<TType> Rotate(GenericVector2<TType> in_a, TType in_angle);
            //-----------------------------------------------------
            /// calculates the result of transforming the vector by
            /// the given regular transform matrix. This is more
            /// efficient than standard matrix multiplication but
            /// will only work for trasform matrices where the point
            /// transformation is in the form:
            ///
            ///  Result = |x y 1|| a b 0 |
            ///                  | c d 0 |
            ///                  | g h 1 |
            ///
            /// This is the case for all non-projective transforms.
            ///
			/// @author Ian Copland
			///
			/// @param The vector.
			/// @param The transform matrix.
			//-----------------------------------------------------
			static GenericVector2<TType> Transform2x3(const GenericVector2<TType>& in_a, const GenericMatrix3<TType>& in_transform);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			//-----------------------------------------------------
			GenericVector2();
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param the x component.
			/// @param the y component.
			//-----------------------------------------------------
			explicit GenericVector2(TType in_x, TType in_y);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return the length of the vector.
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
			void Min(const GenericVector2<TType>& in_b);
            //-----------------------------------------------------
            /// Sets each component in this vector to which ever is
            /// greater, itself or the equivelent in the given
            /// vector.
            ///
			/// @author Ian Copland
			///
			/// @param The other vector.
			//-----------------------------------------------------
			void Max(const GenericVector2<TType>& in_b);
			//-----------------------------------------------------
            /// Clamps each component in this vector between the
            /// values described by the min and max vectors.
            ///
			/// @author Ian Copland
			///
			/// @param The minimum vector.
			/// @param The maximum vector.
			//-----------------------------------------------------
			void Clamp(const GenericVector2<TType>& in_min, const GenericVector2<TType>& in_max);
			//-----------------------------------------------------
            /// Sets the contents of the vector to the result of
            /// interpolating between this and the given vector
            /// with the given interpolation factor.
            ///
			/// @author Ian Copland
			///
			/// @param The other vector.
			/// @param The interpolation factor.
			//-----------------------------------------------------
			void Lerp(const GenericVector2<TType>& in_b, f32 in_t);
            //-----------------------------------------------------
            /// Rotates this vector by the given angle.
            ///
            /// @author Ian Copland
            ///
            /// @param The angle to rotate though in radians.
            //-----------------------------------------------------
            void Rotate(TType in_angle);
            //-----------------------------------------------------
            /// Sets this vector to the result of transforming by
            /// the given regular transform matrix. This is more
            /// efficient than standard matrix multiplication but
            /// will only work for trasform matrices where the point
            /// transformation is in the form:
            ///
            ///  Result = |x y 1|| a b 0 |
            ///                  | c d 0 |
            ///                  | e f 0 |
            ///
            /// This is the case for all non-projective transforms.
            ///
			/// @author Ian Copland
			///
			/// @param The transform matrix.
			//-----------------------------------------------------
			void Transform2x3(const GenericMatrix3<TType>& in_transform);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after having the given vector
			/// added to it.
			//-----------------------------------------------------
			GenericVector2<TType>& operator+=(const GenericVector2<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after having the given vector
			/// subtracted from it.
			//-----------------------------------------------------
			GenericVector2<TType>& operator-=(const GenericVector2<TType>& in_b);
            //-----------------------------------------------------
            /// @author S Downie
			///
			/// @return This vector after it has been negated
            //-----------------------------------------------------
            GenericVector2<TType>& operator-();
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after being multiplied by the
			/// given vector.
			//-----------------------------------------------------
			GenericVector2<TType>& operator*=(const GenericVector2<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after being divided by the
			/// given vector.
			//-----------------------------------------------------
			GenericVector2<TType>& operator/=(const GenericVector2<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A matrix.
			///
			/// @return This vector after being multiplied by the
			/// given matrix.
			//-----------------------------------------------------
			GenericVector2<TType>& operator*=(const GenericMatrix3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param  A scalar.
			///
			/// @return This vector after being multiplied by the
			/// given scalar.
			//-----------------------------------------------------
			GenericVector2<TType>& operator*=(TType in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar.
			///
			/// @return This vector after being divided by the
			/// given scalar.
			//-----------------------------------------------------
			GenericVector2<TType>& operator/=(TType in_b);
            
			TType x;
			TType y;
		};
        //-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A + B.
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator-(TType in_a, GenericVector2<TType> in_b)
        {
            return GenericVector2<TType>(0.0f, 0.0f);
        }
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Vector B
		///
		/// @return The result of A + B.
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator+(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Vector B
		///
		/// @return The result of A - B.
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator-(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Vector B
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Vector B
		///
		/// @return The result of A / B.
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator/(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Matrix B
		///
		/// @return The result of A * B
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(const GenericVector2<TType>& in_a, const GenericMatrix3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Scalar B
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(GenericVector2<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Scalar A
		/// @param Vector B
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(TType in_a, GenericVector2<TType> in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Scalar B
		///
		/// @return The result of A / B.
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator/(GenericVector2<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Vector B
		///
		/// @return Whether or not the two vectors are equal
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Vector A
		/// @param Vector B
		///
		/// @return Whether or not the two vectors are in-equal
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
	}
}

//----------------------------------------------------
// These are included here to avoid circular inclusion
// issues. At this stage the class has been defined
// which is enough for the classes included to use it.
//----------------------------------------------------
#include <ChilliSource/Core/Math/Matrix3.h>

#include <algorithm>
#include <cmath>

namespace ChilliSource
{
	namespace Core
	{
		template <typename TType> const GenericVector2<TType> GenericVector2<TType>::k_zero(0, 0);
		template <typename TType> const GenericVector2<TType> GenericVector2<TType>::k_one(1, 1);
		template <typename TType> const GenericVector2<TType> GenericVector2<TType>::k_unitPositiveX(1, 0);
		template <typename TType> const GenericVector2<TType> GenericVector2<TType>::k_unitNegativeX(-1, 0);
		template <typename TType> const GenericVector2<TType> GenericVector2<TType>::k_unitPositiveY(0, 1);
		template <typename TType> const GenericVector2<TType> GenericVector2<TType>::k_unitNegativeY(0, -1);
        //-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector2<TType>::Normalise(GenericVector2<TType> in_a)
		{
			in_a.Normalise();
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector2<TType>::Inverse(GenericVector2<TType> in_a)
		{
			in_a.Inverse();
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector2<TType>::Abs(GenericVector2<TType> in_a)
		{
			in_a.Abs();
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector2<TType>::Min(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
		{
			in_a.Min(in_b);
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector2<TType>::Max(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
		{
			in_a.Max(in_b);
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector2<TType>::Clamp(GenericVector2<TType> in_value, const GenericVector2<TType>& in_min, const GenericVector2<TType>& in_max)
		{
            in_value.Clamp(in_min, in_max);
            return in_value;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector2<TType>::Lerp(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b, f32 in_t)
		{
			in_a.Lerp(in_b, in_t);
            return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector2<TType>::DotProduct(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b)
		{
			return (in_a.x * in_b.x + in_a.y * in_b.y);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector2<TType>::CrossProductZ(const GenericVector2<TType> & in_a, const GenericVector2<TType> & in_b)
		{
			return (in_a.x * in_b.y - in_a.y * in_b.x);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector2<TType>::Angle(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b)
		{
			return std::atan2(in_b.y, in_b.x) - std::atan2(in_a.y, in_a.x);
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericVector2<TType> GenericVector2<TType>::Rotate(GenericVector2<TType> in_a, TType in_angle)
        {
            in_a.Rotate(in_angle);
            return in_a;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericVector2<TType> GenericVector2<TType>::Transform2x3(const GenericVector2<TType>& in_a, const GenericMatrix3<TType>& in_transform)
        {
            GenericVector2<TType> c;
			c.x = in_a.x * in_transform.m[0] + in_a.y * in_transform.m[3] + in_transform.m[6];
			c.y = in_a.x * in_transform.m[1] + in_a.y * in_transform.m[4] + in_transform.m[7];
			return c;
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType>::GenericVector2()
        : x(0), y(0)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType>::GenericVector2(TType in_x, TType in_y)
        : x(in_x), y(in_y)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector2<TType>::LengthSquared() const
		{
			return (x * x + y * y);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector2<TType>::Length() const
		{
			return (std::sqrt(x * x + y * y));
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector2<TType>::Normalise()
		{
            TType length = Length();
            if (length != 0)
            {
                TType invLength = 1 / length;
                x *= invLength;
                y *= invLength;
            }
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector2<TType>::Inverse()
		{
			if (x != 0)
			{
				x = 1 / x;
			}
            
			if (y != 0)
			{
				y = 1 / y;
			}
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector2<TType>::Abs()
		{
			x = std::abs(x);
			y = std::abs(y);
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector2<TType>::Min(const GenericVector2<TType>& in_b)
        {
			x = std::min(x, in_b.x);
			y = std::min(y, in_b.y);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector2<TType>::Max(const GenericVector2<TType>& in_b)
        {
            x = std::max(x, in_b.x);
			y = std::max(y, in_b.y);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector2<TType>::Clamp(const GenericVector2<TType>& in_min, const GenericVector2<TType>& in_max)
        {
            x = std::min(std::max(x, in_min.x), in_max.x);
			y = std::min(std::max(y, in_min.y), in_max.y);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector2<TType>::Lerp(const GenericVector2<TType>& in_b, f32 in_t)
        {
            f32 t = std::min(std::max(in_t, 0.0f), 1.0f);
			*this = (*this + t * (in_b - *this));
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector2<TType>::Rotate(TType in_angle)
        {
            GenericVector2<TType> b = *this;
            TType sinA = (TType)std::sin(in_angle);
            TType cosA = (TType)std::cos(in_angle);
            x =  b.x * cosA + b.y * sinA;
            y = -b.x * sinA + b.y * cosA;
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector2<TType>::Transform2x3(const GenericMatrix3<TType>& in_transform)
        {
            GenericVector2<TType> b = *this;
			x = b.x * in_transform.m[0] + b.y * in_transform.m[3] + in_transform.m[6];
			y = b.x * in_transform.m[1] + b.y * in_transform.m[4] + in_transform.m[7];
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator+=(const GenericVector2<TType>& in_b)
		{
			x += in_b.x;
			y += in_b.y;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator-=(const GenericVector2<TType>& in_b)
		{
			x -= in_b.x;
			y -= in_b.y;
			return *this;
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator-()
        {
            x = -x;
			y = -y;
			return *this;
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator*=(const GenericVector2<TType>& in_b)
		{
			x *= in_b.x;
			y *= in_b.y;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator/=(const GenericVector2<TType>& in_b)
		{
			x /= in_b.x;
			y /= in_b.y;
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator*=(const GenericMatrix3<TType>& in_b)
		{
			GenericVector2<TType> c = *this;
			x = c.x * in_b.m[0] + c.y * in_b.m[3] + in_b.m[6];
			y = c.x * in_b.m[1] + c.y * in_b.m[4] + in_b.m[7];
			TType oneOverZ = 1 / (c.x * in_b.m[2] + c.y * in_b.m[5] + in_b.m[8]);
			*this *= oneOverZ;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator*=(TType in_b)
		{
			x *= in_b;
			y *= in_b;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator/=(TType in_b)
		{
			x /= in_b;
			y /= in_b;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator+(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
		{
			return (in_a += in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator-(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
		{
			return (in_a -= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
		{
			return (in_a *= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator/(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
		{
			return (in_a /= in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(const GenericVector2<TType>& in_a, const GenericMatrix3<TType>& in_b)
		{
			GenericVector2<TType> c;
			c.x = in_a.x * in_b.m[0] + in_a.y * in_b.m[3] + in_b.m[6];
			c.y = in_a.x * in_b.m[1] + in_a.y * in_b.m[4] + in_b.m[7];
			TType oneOverZ = 1 / (c.x * in_b.m[2] + c.y * in_b.m[5] + in_b.m[8]);
			c *= oneOverZ;
			return c;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(GenericVector2<TType> in_a, TType in_b)
		{
			return (in_a *= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(TType in_a, GenericVector2<TType> in_b)
		{
			return (in_b *= in_a);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> operator/(GenericVector2<TType> in_a, TType in_b)
		{
			return (in_a /= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b)
		{
			return (in_a.x == in_b.x && in_a.y == in_b.y);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b)
		{
			return !(in_a == in_b);
		}
	}
}

#endif
