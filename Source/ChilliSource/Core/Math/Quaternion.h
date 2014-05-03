//
//  Quaternion.h
//  Chilli Source
//  Created by Ian Copland on 27/04/2014.
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

#ifndef _CHILLISOURCE_CORE_MATH_QUATERNION_H_
#define _CHILLISOURCE_CORE_MATH_QUATERNION_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------
		/// A generic quaternion class for respresention 3D 
		/// orientations.
		///
		/// @author I Copland
		//-------------------------------------------------------
		template <typename TType> class GenericQuaternion final
		{
		public:
			//-----------------------------------------------
			/// Constants
			//-----------------------------------------------
			static const GenericQuaternion<TType> k_identity;
			//--------------------------------------------
			/// @author I Copland
			///
			/// @param The first quaternion.
			/// @param The second quaternion.
			///
			/// @return The dot product of two quaternions. 
			//--------------------------------------------
			static TType Dot(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b);
			//--------------------------------------------
			/// Spherical Linear Interpolation on two
			/// quaternions.
			///
			/// @author I Copland
			///
			/// @param The first quaternion.
			/// @param The second quaternion.
			/// @param The step between the two values. This
			/// is in the range 0.0 - 1.0 and will be clamped
			/// if outside of it.
			///
			/// @return The interpolated quaternion.
			//--------------------------------------------
			static GenericQuaternion<TType> Slerp(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b, f32 in_t);
			//--------------------------------------------
			/// Normalised Linear Interpolation on two
			/// quaternions.
			///
			/// @author I Copland
			///
			/// @param The first quaternion.
			/// @param The second quaternion.
			/// @param The step between the two values. This
			/// is in the range 0.0 - 1.0 and will be clamped
			/// if outside of it.
			///
			/// @return The interpolated quaternion.
			//--------------------------------------------
			static GenericQuaternion<TType> Nlerp(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b, f32 in_t);
			//-----------------------------------------------
			/// Constructor. Sets the contents to the
			/// Identity quaternion.
			///
			/// @author I Copland
			//-----------------------------------------------
			GenericQuaternion();
			//-----------------------------------------------
			/// Constructor.
			///
			/// @author I Copland
			///
			/// @param x
			/// @param y
			/// @param z
			/// @param w
			//-----------------------------------------------
			explicit GenericQuaternion(TType in_x, TType in_y, TType in_z, TType in_w);
			//-----------------------------------------------
			/// Constructor. Initalises the quaternion to
			/// represent the rotation described by the given
			/// axis and angle.
			///
			/// @author I Copland
			///
			/// @param An axis.
			/// @param An angle.
			//-----------------------------------------------
			explicit GenericQuaternion(const GenericVector3<TType>& in_axis, TType in_angle);
			//-----------------------------------------------
			/// Constructor. Initalises the quaternion to
			/// represent the rotation described by the given
			/// euler axes.
			///
			/// @author S Downie
			///
			/// @param The x axis.
			/// @param The y axis.
			/// @param The z axis.
			//-----------------------------------------------
			explicit GenericQuaternion(const GenericVector3<TType>& in_xAxis, const GenericVector3<TType>& in_yAxis, const GenericVector3<TType>& in_zAxis);
			//-----------------------------------------------
			/// Constructor. Initalises the quaternion to
			/// represent the rotation described by the given
			/// rotation matrix.
			///
			/// @author S Downie
			///
			/// @param The rotation matrix.
			//-----------------------------------------------
			explicit GenericQuaternion(const GenericMatrix4<TType>& in_rotation);
			//-----------------------------------------------
			/// @author I Copland
			///
			/// @return the magnitude of the quaternion.
			//-----------------------------------------------
			TType Magnitude() const;
			//-----------------------------------------------
			/// @author I Copland
			///
			/// @return the magnitude of the quaternion 
			/// squared.
			//-----------------------------------------------
			TType MagnitudeSquared() const;
			//-----------------------------------------------
			/// Normalises this quaternion.
			///
			/// @author I Copland
			//-----------------------------------------------
			void Normalise();
			//-----------------------------------------------
			/// @author I Copland
			///
			/// @return a normalised version of the quaternion.
			//-----------------------------------------------
			GenericQuaternion<TType> NormaliseCopy() const;
			//-----------------------------------------------
			/// Sets this quaternion to its conjugate.
			///
			/// @author I Copland
			//-----------------------------------------------
			void Conjugate();
			//-----------------------------------------------
			/// @author I Copland
			/// 
			/// @return the conjugate of the quaternion.
			//-----------------------------------------------
			GenericQuaternion<TType> ConjugateCopy() const;
			//-----------------------------------------------
			/// Sets this quaternion to its inverse.
			///
			/// @author I Copland
			//-----------------------------------------------
			void Inverese();
			//-----------------------------------------------
			/// @author I Copland
			///
			/// @return the inverse of the quaternion.
			//-----------------------------------------------
			GenericQuaternion<TType> InvereseCopy() const;
			//-----------------------------------------------
			/// Converts the quaternion into the axis and
			/// angle it represents.
			///
			/// @author S Downie
			///
			/// @param [Out] The axis.
			/// @param [Out] The angle.
			//-----------------------------------------------
			void ToAxisAngle(GenericVector3<TType>& out_axis, TType& out_angle) const;
			//-----------------------------------------------
			/// Converts the quaternion into the equivelant
			/// euler axes.
			///
			/// @author S Downie
			///
			/// @param [Out] The x axis.
			/// @param [Out] The y axis.
			/// @param [Out] The z axis.
			//-----------------------------------------------
			void ToEulerAxes(GenericVector3<TType>& out_xAxis, GenericVector3<TType>& out_yAxis, GenericVector3<TType>& out_zAxis) const;
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another quaternion.
			///
			/// @return This quaternion after adding the given
			/// quaternion.
			//-----------------------------------------------------
			GenericQuaternion<TType>& operator+=(const GenericQuaternion<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another quaternion.
			///
			/// @return This quaternion after subtracting the given
			/// quaternion.
			//-----------------------------------------------------
			GenericQuaternion<TType>& operator-=(const GenericQuaternion<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another quaternion.
			///
			/// @return This quaternion after multiplying by the given
			/// quaternion.
			//-----------------------------------------------------
			GenericQuaternion<TType>& operator*=(const GenericQuaternion<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param A scalar.
			///
			/// @return This quaternion after multiplying by the given
			/// scalar.
			//-----------------------------------------------------
			GenericQuaternion<TType>& operator*=(TType in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param A scalar.
			///
			/// @return This quaternion after dividing by the given
			/// scalar.
			//-----------------------------------------------------
			GenericQuaternion<TType>& operator/=(TType in_b);

			TType x;
			TType y;
			TType z;
			TType w;
		};
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The quaternion A.
		/// @param The quaternion B.
		///
		/// @return The result of A + B.
		//-----------------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator+(GenericQuaternion<TType> in_a, const GenericQuaternion<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The quaternion A.
		/// @param The quaternion B.
		///
		/// @return The result of A - B.
		//-----------------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator-(GenericQuaternion<TType> in_a, const GenericQuaternion<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The quaternion A.
		/// @param The quaternion B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator*(GenericQuaternion<TType> in_a, const GenericQuaternion<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The quaternion A.
		/// @param The scalar B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator*(GenericQuaternion<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The scalar A.
		/// @param The quaternion B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator*(TType in_a, GenericQuaternion<TType> in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The quaternion A.
		/// @param The scalar B.
		///
		/// @return The result of A / B.
		//-----------------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator/(GenericQuaternion<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The quaternion A.
		/// @param The quaternion B.
		///
		/// @return Whether or not they are equal.
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param The quaternion A.
		/// @param The quaternion B.
		///
		/// @return Whether or not they are in-equal.
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b);
	}
}

//----------------------------------------------------
// These are included here to avoid circular inclusion
// issues. At this stage the class has been defined
// which is enough for the classes included to use it.
//----------------------------------------------------
#include <ChilliSource/Core/Math/Matrix4.h>
#include <ChilliSource/Core/Math/Vector3.h>

#include <cmath>

namespace ChilliSource
{
	namespace Core
	{
		template <typename TType> const GenericQuaternion<TType> GenericQuaternion<TType>::k_identity(0, 0, 0, 1);

		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> TType GenericQuaternion<TType>::Dot(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b)
		{
			return (in_a.x * in_b.x + in_a.y * in_b.y + in_a.z * in_b.z + in_a.w * in_b.w);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::Slerp(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b, f32 in_t)
		{
			const TType k_epsilon = (TType)0.0001;

			if (in_t <= 0)
			{
				return in_a;
			}
			if (in_t >= 1)
			{
				return in_b;
			}

			GenericQuaternion<TType> B = in_b;
			TType aDotB = Dot(in_a, B);
			if (aDotB < 0)
			{
				B.x = -B.x;
				B.y = -B.y;
				B.z = -B.z;
				B.w = -B.w;
				aDotB = -aDotB;
			}

			if (aDotB > 1 - k_epsilon)
			{
				GenericQuaternion<TType> output;
				output = in_a + in_t * (B - in_a);
				output.Normalise();
				return output;
			}

			TType acosADotB = std::acos(aDotB);
			return (std::sin((1 - in_t) * acosADotB) * in_a + std::sin(in_t * acosADotB) * B) / std::sin(acosADotB);
		}
		//--------------------------------------------
		//--------------------------------------------
		template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::Nlerp(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b, f32 in_t)
		{
			const f32 dot = Dot(in_a, in_b);
			Quaternion output;

			if (dot < 0)
			{
				output = in_a + (in_t * ((in_b * (TType)-1) - in_a));
			}
			else
			{
				output = in_a + (in_t * (in_b - in_a));
			}

			output.Normalise();
			return output;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>::GenericQuaternion()
			: x(0), y(0), z(0), w(1)
		{
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>::GenericQuaternion(TType in_x, TType in_y, TType in_z, TType in_w)
			: x(in_x), y(in_y), z(in_z), w(in_w)
		{
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>::GenericQuaternion(const GenericVector3<TType>& in_axis, TType in_angle)
			: x(0), y(0), z(0), w(0)
		{
			GenericVector3<TType> normalised = in_axis.NormaliseCopy();
			TType halfAngle = in_angle / 2;
			TType sinAngle = (TType)std::sin(halfAngle);

			x = normalised.x * sinAngle;
			y = normalised.y * sinAngle;
			z = normalised.z * sinAngle;
			w = (TType)std::cos(halfAngle);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>::GenericQuaternion(const GenericVector3<TType>& in_xAxis, const GenericVector3<TType>& in_yAxis, const GenericVector3<TType>& in_zAxis)
		{
			GenericMatrix4<TType> rotation;

			rotation.m[0] = in_xAxis.x;
			rotation.m[1] = in_xAxis.y;
			rotation.m[2] = in_xAxis.z;

			rotation.m[4] = in_yAxis.x;
			rotation.m[5] = in_yAxis.y;
			rotation.m[6] = in_yAxis.z;

			rotation.m[8] = in_zAxis.x;
			rotation.m[9] = in_zAxis.y;
			rotation.m[10] = in_zAxis.z;

			(*this) = Quaternion(rotation);
		}
		//-----------------------------------------------
		// Algorithm in Ken Shoemake's article in 1987 
		// SIGGRAPH course notes article "Quaternion 
		// Calculus and Fast Animation".
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>::GenericQuaternion(const GenericMatrix4<TType>& in_rotation)
		{
			TType trace = in_rotation.m[0] + in_rotation.m[5] + in_rotation.m[10];
			TType root = 0;

			if (trace > 0)
			{
				root = std::sqrt(trace + 1); 
				w = (TType)0.5 * root;
				root = (TType)0.5 / root;
				x = (in_rotation.m[6] - in_rotation.m[9]) * root;
				y = (in_rotation.m[8] - in_rotation.m[2]) * root;
				z = (in_rotation.m[1] - in_rotation.m[4]) * root;
			}
			else
			{
				static u32 s_iNext[3] = { 1, 2, 0 };
				u32 i = 0;
				if (in_rotation.m[5] > in_rotation.m[0])
				{
					i = 1;
				}
				if (in_rotation.m[10] > in_rotation(i, i))
				{
					i = 2;
				}
				u32 j = s_iNext[i];
				u32 k = s_iNext[j];

				root = std::sqrt(in_rotation(i, i) - in_rotation(j, j) - in_rotation(k, k) + 1);

				f32* apkQuat[3] = { &x, &y, &z };
				*apkQuat[i] = (TType)0.5 * root;
				root = (TType)0.5 / root;
				w = (in_rotation(j, k) - in_rotation(k, j)) * root;
				*apkQuat[j] = (in_rotation(j, i) + in_rotation(i, j)) * root;
				*apkQuat[k] = (in_rotation(k, i) + in_rotation(i, k)) * root;
			}
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> TType GenericQuaternion<TType>::Magnitude() const
		{
			return (TType)std::sqrt(w*w + x*x + y*y + z*z);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> TType GenericQuaternion<TType>::MagnitudeSquared() const
		{
			return (w*w + x*x + y*y + z*z);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> void GenericQuaternion<TType>::Normalise()
		{
			TType magnitude = Magnitude();
			w = w / magnitude;
			x = x / magnitude;
			y = y / magnitude;
			z = z / magnitude;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::NormaliseCopy() const
		{
			GenericQuaternion<TType> output = *this;
			output.Normalise();
			return output;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> void GenericQuaternion<TType>::Conjugate()
		{
			w = w;
			x = -x;
			y = -y;
			z = -z;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::ConjugateCopy() const
		{
			GenericQuaternion<TType> output = *this;
			output.Conjugate();
			return output;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> void GenericQuaternion<TType>::Inverese()
		{
			TType magnitudeSquared = MagnitudeSquared();
			Conjugate();
			w /= magnitudeSquared;
			x /= magnitudeSquared;
			y /= magnitudeSquared;
			z /= magnitudeSquared;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> void GenericQuaternion<TType>::ToAxisAngle(GenericVector3<TType>& out_axis, TType& out_angle) const
		{
			TType sqrLength = (x * x + y * y + z * z);

			if (sqrLength > 0)
			{
				TType invLength = 1.0f / std::sqrt(sqrLength);
				out_angle = 2 * std::acos(w);
				out_axis = Vector3(x * invLength, y * invLength, z * invLength);
			}
			else
			{
				out_angle = 0;
				out_axis = Vector3::k_unitPositiveX;
			}
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> void GenericQuaternion<TType>::ToEulerAxes(GenericVector3<TType>& out_xAxis, GenericVector3<TType>& out_yAxis, GenericVector3<TType>& out_zAxis) const
		{
			GenericMatrix4<TType> rotation = Matrix4::CreateRotation(*this);

			out_xAxis.x = rotation(0, 0);
			out_xAxis.y = rotation(0, 1);
			out_xAxis.z = rotation(0, 2);

			out_yAxis.x = rotation(1, 0);
			out_yAxis.y = rotation(1, 1);
			out_yAxis.z = rotation(1, 2);

			out_zAxis.x = rotation(2, 0);
			out_zAxis.y = rotation(2, 1);
			out_zAxis.z = rotation(2, 2);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::InvereseCopy() const
		{
			GenericQuaternion<TType> output = *this;
			output.Inverese();
			return output;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator+=(const GenericQuaternion<TType>& in_b)
		{
			x += in_b.x;
			y += in_b.y;
			z += in_b.z;
			w += in_b.w;
			return *this;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator-=(const GenericQuaternion<TType>& in_b)
		{
			x -= in_b.x;
			y -= in_b.y;
			z -= in_b.z;
			w -= in_b.w;
			return *this;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator *= (const GenericQuaternion<TType>& in_b)
		{
			GenericQuaternion<TType> copy = *this;
			w = in_b.w * copy.w - in_b.x *copy.x - in_b.y * copy.y - in_b.z * copy.z;
			x = in_b.w * copy.x + in_b.x *copy.w + in_b.y * copy.z - in_b.z * copy.y;
			y = in_b.w * copy.y - in_b.x *copy.z + in_b.y * copy.w + in_b.z * copy.x;
			z = in_b.w * copy.z + in_b.x *copy.y - in_b.y * copy.x + in_b.z * copy.w;
			return *this;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator*=(TType in_b)
		{
			x *= in_b;
			y *= in_b;
			z *= in_b;
			w *= in_b;
			return *this;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator/=(TType in_b)
		{
			x /= in_b;
			y /= in_b;
			z /= in_b;
			w /= in_b;
			return *this;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator+(GenericQuaternion<TType> in_a, const GenericQuaternion<TType>& in_b)
		{
			return (in_a += in_b);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator-(GenericQuaternion<TType> in_a, const GenericQuaternion<TType>& in_b)
		{
			return (in_a -= in_b);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator*(GenericQuaternion<TType> in_a, const GenericQuaternion<TType>& in_b)
		{
			return (in_a *= in_b);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator*(GenericQuaternion<TType> in_a, TType in_b)
		{
			return (in_a *= in_b);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator*(TType in_a, GenericQuaternion<TType> in_b)
		{
			return (in_b *= in_a);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> GenericQuaternion<TType> operator/(GenericQuaternion<TType> in_a, TType in_b)
		{
			return (in_a /= in_b);
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> bool operator==(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b)
		{
			if (in_a.x != in_b.x || in_a.y != in_b.y || in_a.z != in_b.z || in_a.w != in_b.w)
			{
				return false;
			}
			return true;
		}
		//-----------------------------------------------
		//-----------------------------------------------
		template <typename TType> bool operator!=(const GenericQuaternion<TType>& in_a, const GenericQuaternion<TType>& in_b)
		{
			return !(in_a == in_b);
		}
	}
}

#endif