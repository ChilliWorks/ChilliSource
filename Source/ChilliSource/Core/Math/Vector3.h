//
//  Vector3.h
//  Chilli Source
//  Created by I Copland on 20/04/2014.
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
#include <ChilliSource/Core/Math/Vector2.h>

#include <math.h>

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
		/// @author I Copland
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
			/// @author I Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return A vector containing the minimum value for
			/// each component in the vectors.
			//-----------------------------------------------------
			static GenericVector3<TType> Min(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return A vector containing the maximum value for
			/// each component in the vectors.
			//-----------------------------------------------------
			static GenericVector3<TType> Max(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param A vector.
			/// @param A minimum vector.
			/// @param A maximum vector.
			///
			/// @return A vector in which each component is clamped
			/// between the equivalent in the min and max vectors.
			//-----------------------------------------------------
			static GenericVector3<TType> Clamp(const GenericVector3<TType>& in_value, const GenericVector3<TType>& in_min, const GenericVector3<TType>& in_max);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param a vector.
			/// @param another vector.
			/// @param the interpolation factor.
			///
			/// @return The interpolated vector.
			//-----------------------------------------------------
			static GenericVector3<TType> Lerp(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b, f32 in_t);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param a vector.
			/// @param another vector.
			///
			/// @return the dot product of these two vectors.
			//-----------------------------------------------------
			static TType DotProduct(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param a vector.
			/// @param another vector.
			/// 
			/// @return the cross product of these two vectors
			//-----------------------------------------------------
			static GenericVector3<TType> CrossProduct(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
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
			/// @author I Copland
			///
			/// @param The quaternion.
			/// @param The vector.
			///
			/// @return The new vector.
			//-----------------------------------------------------
			//TODO: static GenericVector3<TType> Rotate(const GenericQuaternion<TType>& in_a, const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author I Copland
			//-----------------------------------------------------
			GenericVector3();
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author I Copland
			/// 
			/// @param the x component.
			/// @param the y component.
			/// @param the z component.
			//-----------------------------------------------------
			explicit GenericVector3(TType in_x, TType in_y, TType in_z);
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author I Copland
			///
			/// @param A two component vector of the same type.
			/// @param the z component.
			//-----------------------------------------------------
			explicit GenericVector3(const GenericVector2<TType>& in_xy, TType in_z);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @return A copy of the XY components of this vector
			/// as a Vector2.
			//-----------------------------------------------------
			GenericVector2<TType> XY() const;
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @return the length of the vector
			//-----------------------------------------------------
			TType Length() const;
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @return the length of the vector squared
			//-----------------------------------------------------
			TType LengthSquared() const;
			//-----------------------------------------------------
			/// Normalises the contents of this vector.
			///
			/// @author I Copland
			//-----------------------------------------------------
			void Normalise();
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @return a normalised copy of the vector.
			//-----------------------------------------------------
			GenericVector3<TType> NormaliseCopy() const;
			//-----------------------------------------------------
			/// Sets this contents of this vector to its inverse.
			///
			/// @author I Copland
			//-----------------------------------------------------
			void Inverse();
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @return An inversed copy of this vector.
			//-----------------------------------------------------
			GenericVector3<TType> InverseCopy() const;
			//-----------------------------------------------------
			/// Sets this vector to it's absolute value.
			///
			/// @author I Copland
			//-----------------------------------------------------
			void Abs();
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @return an absolute copy of the vector.
			//-----------------------------------------------------
			GenericVector3<TType> AbsCopy() const;
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after the given vector has been
			/// added to it.
			//-----------------------------------------------------
			GenericVector3<TType>& operator+=(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after the given vector has been
			/// subtracted from it.
			//-----------------------------------------------------
			GenericVector3<TType>& operator-=(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after it has been multiplied
			/// by the given vector.
			//-----------------------------------------------------
			GenericVector3<TType>& operator*=(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another vector.
			///
			/// @return This vector after it has been divided
			/// by the given vector.
			//-----------------------------------------------------
			GenericVector3<TType>& operator/=(const GenericVector3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param A scalar
			///
			/// @return This vector after it has been multiplied
			/// by the given scalar.
			//-----------------------------------------------------
			GenericVector3<TType>& operator*=(TType in_b);
			//-----------------------------------------------------
			/// @author I Copland
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
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Min(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b)
		{
			GenericVector3<TType> output;
			output.x = std::min(in_a.x, in_b.x);
			output.y = std::min(in_a.y, in_b.y);
			output.z = std::min(in_a.z, in_b.z);
			return output;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Max(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b)
		{
			GenericVector3<TType> output;
			output.x = std::max(in_a.x, in_b.x);
			output.y = std::max(in_a.y, in_b.y);
			output.z = std::max(in_a.z, in_b.z);
			return output;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Clamp(const GenericVector3<TType>& inValue, const GenericVector3<TType>& inMin, const GenericVector3<TType>& inMax)
		{
			GenericVector3<TType> output;
			output.x = std::min(std::max(inValue.x, inMin.x), inMax.x);
			output.y = std::min(std::max(inValue.y, inMin.y), inMax.y);
			output.z = std::min(std::max(inValue.z, inMin.z), inMax.z);
			return output;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::Lerp(const GenericVector3<TType>& in_a, const GenericVector3<TType>& in_b, f32 in_t)
		{
			if (in_t <= 0.0f)
			{
				return in_a;
			}
			else if (in_t >= 1.0f)
			{
				return in_b;
			}

			return (in_a + in_t * (in_b - in_a));
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
			TType dot = DotProduct(Vec);
			TType totalMagnitude = Length() * Vec.Length();

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

			return acosf(ratio);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		//template <typename TType> GenericVector3<TType> GenericVector3<TType>::Rotate(const GenericQuaternion<TType>& in_a, const GenericVector3<TType>& in_b)
		//{
		//	GenericVector3<TType> xyzA(in_a.x, in_a.y, in_a.z);
		//	GenericVector3<TType> t = static_cast<TType>(2) * CrossProduct(xyzA, in_b);
		//	return in_b + in_a.W * t + CrossProduct(xyzA, t);
		//}
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
			return (sqrt(x * x + y * y + z * z));
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> void GenericVector3<TType>::Normalise()
		{
			TType length = Length();
			x = x / length;
			y = y / length;
			z = z / length;
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::NormaliseCopy() const
		{
			GenericVector3<TType> copy = *this;
			copy.Normalise();
			return copy;
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
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::InverseCopy() const
		{
			GenericVector3<TType> copy = *this;
			copy.Inverse();
			return copy;
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
		template <typename TType> GenericVector3<TType> GenericVector3<TType>::AbsCopy() const
		{
			GenericVector3<TType> copy = *this;
			copy.Abs();
			return copy;
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
			if (in_a.x == in_b.x && in_a.y == in_b.y && in_a.z == in_b.z)
			{
				return true;
			}
			return false;
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
