//
//  Vector4.h
//  Chilli Source
//  Created by Ian Copland on 26/04/2014.
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

#ifndef _CHILLISOURCE_CORE_MATH_VECTOR4_H_
#define _CHILLISOURCE_CORE_MATH_VECTOR4_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------------------
		/// A generic four dimensional mathematical vector. Typically
		/// the type specific typedefs of this class would be used
		/// over direct use. For example:
		///		Vector4 -> GenericVector4<f32>
		///		Float4 -> GenericVector4<f32>
		///		Integer4 -> GenericVector4<s32>
		///
		/// @author Ian Copland
		//-------------------------------------------------------------------
		template <typename TType> class GenericVector4 final
		{
		public:
			//-----------------------------------------------------
			/// Constants
			//-----------------------------------------------------
			static const GenericVector4<TType> k_zero;
			static const GenericVector4<TType> k_one;
			static const GenericVector4<TType> k_unitPositiveX;
			static const GenericVector4<TType> k_unitNegativeX;
			static const GenericVector4<TType> k_unitPositiveY;
			static const GenericVector4<TType> k_unitNegativeY;
			static const GenericVector4<TType> k_unitPositiveZ;
			static const GenericVector4<TType> k_unitNegativeZ;
			static const GenericVector4<TType> k_unitPositiveW;
			static const GenericVector4<TType> k_unitNegativeW;
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A vector.
			///
			/// @return a normalised copy of the vector.
			//-----------------------------------------------------
			static GenericVector4<TType> Normalise(GenericVector4<TType> in_a);
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A vector.
			///
			/// @return An inversed copy of the vector.
			//-----------------------------------------------------
			static GenericVector4<TType> Inverse(GenericVector4<TType> in_a);
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A vector.
			///
			/// @return an absolute copy of the vector.
			//-----------------------------------------------------
			static GenericVector4<TType> Abs(GenericVector4<TType> in_a);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return A vector containing the minimum value for
			/// each component in each vector.
			//-----------------------------------------------------
			static GenericVector4<TType> Min(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return A vector containing the maximum value for
			/// each component in each vector.
			//-----------------------------------------------------
			static GenericVector4<TType> Max(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b);
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
			static GenericVector4<TType> Clamp(GenericVector4<TType> in_value, const GenericVector4<TType>& in_min, const GenericVector4<TType>& in_max);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return The interpolated vector.
			//-----------------------------------------------------
			static GenericVector4<TType> Lerp(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b, f32 in_t);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return the dot product of these two vectors.
			//-----------------------------------------------------
			static TType DotProduct(const GenericVector4<TType>& in_a, const GenericVector4<TType>& in_b);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			//-----------------------------------------------------
			GenericVector4();
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param the x component.
			/// @param the y component.
			/// @param the z component.
			/// @param the w component.
			//-----------------------------------------------------
			explicit GenericVector4(TType in_x, TType in_y, TType in_z, TType in_w);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param A vector2.
			/// @param the z component.
			/// @param the w component.
			//-----------------------------------------------------
			explicit GenericVector4(const GenericVector2<TType>& in_xy, TType in_z, TType in_w);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param A vector2 representing the x and y component
			/// @param A vector2 representing the z and w component
			//-----------------------------------------------------
			explicit GenericVector4(const GenericVector2<TType>& in_xy, const GenericVector2<TType>& in_zw);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param A vector3 representing the x, y and z components
			/// @param The w component.
			//-----------------------------------------------------
			explicit GenericVector4(const GenericVector3<TType>& in_xyz, TType in_w);
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
			/// @return A copy of the ZW components of this vector
			/// as a Vector2.
			//-----------------------------------------------------
			GenericVector2<TType> ZW() const;
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @return A copy of the XYZ components of this vector
			/// as a Vector3.
			//-----------------------------------------------------
			GenericVector3<TType> XYZ() const;
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
			void Min(const GenericVector4<TType>& in_b);
			//-----------------------------------------------------
            /// Sets each component in this vector to which ever is
            /// greater, itself or the equivelent in the given
            /// vector.
            ///
			/// @author Ian Copland
			///
			/// @param The other vector.
			//-----------------------------------------------------
			void Max(const GenericVector4<TType>& in_b);
			//-----------------------------------------------------
            /// Clamps each component in this vector between the
            /// values described by the min and max vectors.
            ///
			/// @author Ian Copland
			///
			/// @param The minimum vector.
			/// @param The maximum vector.
			//-----------------------------------------------------
			void Clamp(const GenericVector4<TType>& in_min, const GenericVector4<TType>& in_max);
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
			void Lerp(const GenericVector4<TType>& in_b, f32 in_t);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after the given vector has been
			/// added to it.
			//-----------------------------------------------------
			GenericVector4<TType>& operator+=(const GenericVector4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after the given vector has been
			/// subtracted from it.
			//-----------------------------------------------------
			GenericVector4<TType>& operator-=(const GenericVector4<TType>& in_b);
            //-----------------------------------------------------
			/// @author S Downie
			///
			/// @return This vector after it has been negated.
			//-----------------------------------------------------
			GenericVector4<TType>& operator-();
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after being multiplied by the
			/// given vector.
			//-----------------------------------------------------
			GenericVector4<TType>& operator*=(const GenericVector4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after being divided by the given
			/// vector.
			//-----------------------------------------------------
			GenericVector4<TType>& operator/=(const GenericVector4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A matrix
			///
			/// @return This vector after being multipled by the given
			/// matrix.
			//-----------------------------------------------------
			GenericVector4<TType>& operator*=(const GenericMatrix4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar.
			///
			/// @return This vector after being multiplied by the
			/// given scalar.
			//-----------------------------------------------------
			GenericVector4<TType>& operator*=(TType in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar.
			///
			/// @return This vector after being divided by the
			/// given scalar.
			//-----------------------------------------------------
			GenericVector4<TType>& operator/=(TType in_b);
            
			TType x;
			TType y;
			TType z;
			TType w;
		};
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A + B.
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator+(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A - B.
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator-(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return The result of A / B.
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator/(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The matrix B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(const GenericVector4<TType>& in_a, const GenericMatrix4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The scalar B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(GenericVector4<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The scalar A.
		/// @param The vector B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(TType in_a, GenericVector4<TType> in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The scalar B.
		///
		/// @return The result of A / B.
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator/(GenericVector4<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return Whether or not they are equal.
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericVector4<TType>& in_a, const GenericVector4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The vector A.
		/// @param The vector B.
		///
		/// @return Whether or not they are in-equal.
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericVector4<TType>& in_a, const GenericVector4<TType>& in_b);
	}
}

//----------------------------------------------------
// These are included here to avoid circular inclusion
// issues. At this stage the class has been defined
// which is enough for the classes included to use it.
//----------------------------------------------------
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>

#include <algorithm>
#include <cmath>

namespace ChilliSource
{
	namespace Core
	{
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_zero(0, 0, 0, 0);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_one(1, 1, 1, 1);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_unitPositiveX(1, 0, 0, 0);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_unitNegativeX(-1, 0, 0, 0);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_unitPositiveY(0, 1, 0, 0);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_unitNegativeY(0, -1, 0, 0);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_unitPositiveZ(0, 0, 1, 0);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_unitNegativeZ(0, 0, -1, 0);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_unitPositiveW(0, 0, 0, 1);
		template <typename TType> const GenericVector4<TType> GenericVector4<TType>::k_unitNegativeW(0, 0, 0, -1);
        
        
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> GenericVector4<TType>::Normalise(GenericVector4<TType> in_a)
		{
			in_a.Normalise();
			return in_a;
		}
        
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> GenericVector4<TType>::Inverse(GenericVector4<TType> in_a)
		{
			in_a.Inverse();
			return in_a;
		}
        
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> GenericVector4<TType>::Abs(GenericVector4<TType> in_a)
		{
			in_a.Abs();
			return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> GenericVector4<TType>::Min(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b)
		{
			in_a.Min(in_b);
            return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> GenericVector4<TType>::Max(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b)
		{
			in_a.Max(in_b);
            return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> GenericVector4<TType>::Clamp(GenericVector4<TType> in_value, const GenericVector4<TType>& in_min, const GenericVector4<TType>& in_max)
		{
			in_value.Clamp(in_min, in_max);
            return in_value;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> GenericVector4<TType>::Lerp(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b, f32 in_t)
		{
			in_a.Lerp(in_b, in_t);
            return in_a;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector4<TType>::DotProduct(const GenericVector4<TType>& in_a, const GenericVector4<TType>& in_b)
		{
			return (in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z + in_a.w * in_b.w);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>::GenericVector4()
        : x(0), y(0), z(0), w(0)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>::GenericVector4(TType in_x, TType in_y, TType in_z, TType in_w)
        : x(in_x), y(in_y), z(in_z), w(in_w)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>::GenericVector4(const GenericVector2<TType>& in_xy, TType in_z, TType in_w)
        : x(in_xy.x), y(in_xy.y), z(in_z), w(in_w)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>::GenericVector4(const GenericVector2<TType>& in_xy, const GenericVector2<TType>& in_zw)
        : x(in_xy.x), y(in_xy.y), z(in_zw.x), w(in_zw.y)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>::GenericVector4(const GenericVector3<TType>& in_xyz, TType in_w)
        : x(in_xyz.x), y(in_xyz.y), z(in_xyz.z), w(in_w)
		{
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector4<TType>::XY() const
		{
			return GenericVector2<TType>(x, y);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericVector4<TType>::ZW() const
		{
			return GenericVector2<TType>(z, w);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector4<TType>::XYZ() const
		{
			return GenericVector3<TType>(x, y, z);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector4<TType>::LengthSquared() const
		{
			return (x * x + y * y + z * z + w * w);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericVector4<TType>::Length() const
		{
			return (std::sqrt(x * x + y * y + z * z + w * w));
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector4<TType>::Normalise()
		{
            TType length = Length();
            if (length != 0)
            {
                TType invLength = 1 / length;
                x *= invLength;
                y *= invLength;
                z *= invLength;
                w *= invLength;
            }
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector4<TType>::Inverse()
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
            
			if (w != 0)
			{
				w = 1 / w;
			}
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector4<TType>::Abs()
		{
			x = std::abs(x);
			y = std::abs(y);
			z = std::abs(z);
			w = std::abs(w);
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector4<TType>::Min(const GenericVector4<TType>& in_b)
        {
            x = std::min(x, in_b.x);
			y = std::min(y, in_b.y);
			z = std::min(z, in_b.z);
			w = std::min(w, in_b.w);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector4<TType>::Max(const GenericVector4<TType>& in_b)
        {
            x = std::max(x, in_b.x);
			y = std::max(y, in_b.y);
			z = std::max(z, in_b.z);
			w = std::max(w, in_b.w);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector4<TType>::Clamp(const GenericVector4<TType>& in_min, const GenericVector4<TType>& in_max)
        {
            x = std::min(std::max(x, in_min.x), in_max.x);
			y = std::min(std::max(y, in_min.y), in_max.y);
			z = std::min(std::max(z, in_min.z), in_max.z);
			w = std::min(std::max(w, in_min.w), in_max.w);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> void GenericVector4<TType>::Lerp(const GenericVector4<TType>& in_b, f32 in_t)
        {
            f32 t = std::min(std::max(in_t, 0.0f), 1.0f);
			*this = (*this + t * (in_b - *this));
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator+=(const GenericVector4<TType>& in_b)
		{
			x += in_b.x;
			y += in_b.y;
			z += in_b.z;
			w += in_b.w;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator-=(const GenericVector4<TType>& in_b)
		{
			x -= in_b.x;
			y -= in_b.y;
			z -= in_b.z;
			w -= in_b.w;
			return *this;
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator-()
        {
            x = -x;
			y = -y;
			z = -z;
			w = -w;
			return *this;
        }
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator*=(const GenericVector4<TType>& in_b)
		{
			x *= in_b.x;
			y *= in_b.y;
			z *= in_b.z;
			w *= in_b.w;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator/=(const GenericVector4<TType>& in_b)
		{
			x /= in_b.x;
			y /= in_b.y;
			z /= in_b.z;
			w /= in_b.w;
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator*=(const GenericMatrix4<TType>& in_b)
		{
			GenericVector4<TType> c = *this;
			x = c.x * in_b.m[0] + c.y * in_b.m[4] + c.z * in_b.m[8] + c.w * in_b.m[12];
			y = c.x * in_b.m[1] + c.y * in_b.m[5] + c.z * in_b.m[9] + c.w * in_b.m[13];
			z = c.x * in_b.m[2] + c.y * in_b.m[6] + c.z * in_b.m[10] + c.w * in_b.m[14];
			w = c.x * in_b.m[3] + c.y * in_b.m[7] + c.z * in_b.m[11] + c.w * in_b.m[15];
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator*=(TType in_b)
		{
			x *= in_b;
			y *= in_b;
			z *= in_b;
			w *= in_b;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator/=(TType in_b)
		{
			x /= in_b;
			y /= in_b;
			z /= in_b;
			w /= in_b;
			return *this;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator+(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b)
		{
			return (in_a += in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator-(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b)
		{
			return (in_a -= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b)
		{
			return (in_a *= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator/(GenericVector4<TType> in_a, const GenericVector4<TType>& in_b)
		{
			return (in_a /= in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(const GenericVector4<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			GenericVector4<TType> c;
			c.x = in_a.x * in_b.m[0] + in_a.y * in_b.m[4] + in_a.z * in_b.m[8] + in_a.w * in_b.m[12];
			c.y = in_a.x * in_b.m[1] + in_a.y * in_b.m[5] + in_a.z * in_b.m[9] + in_a.w * in_b.m[13];
			c.z = in_a.x * in_b.m[2] + in_a.y * in_b.m[6] + in_a.z * in_b.m[10] + in_a.w * in_b.m[14];
			c.w = in_a.x * in_b.m[3] + in_a.y * in_b.m[7] + in_a.z * in_b.m[11] + in_a.w * in_b.m[15];
			return c;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(GenericVector4<TType> in_a, TType in_b)
		{
			return (in_a *= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(TType in_a, GenericVector4<TType> in_b)
		{
			return (in_b *= in_a);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector4<TType> operator/(GenericVector4<TType> in_a, TType in_b)
		{
			return (in_a /= in_b);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericVector4<TType>& in_a, const GenericVector4<TType>& in_b)
		{
			return (in_a.x == in_b.x && in_a.y == in_b.y && in_a.z == in_b.z && in_a.w == in_b.w);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericVector4<TType>& in_a, const GenericVector4<TType>& in_b)
		{
			return !(in_a == in_b);
		}
	}
}

#endif
