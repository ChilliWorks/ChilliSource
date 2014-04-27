//
//  Matrix4.h
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

#ifndef _CHILLISOURCE_CORE_MATH_MATRIX4_H_
#define _CHILLISOURCE_CORE_MATH_MATRIX4_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>
#include <ChilliSource/Core/Math/Quaternion.h>

#include <cmath>

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
#include <Accelerate/Accelerate.h>
#endif

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------------
		/// A generic 4x4 matrix that provides standard matrix math
		/// functionality. The matrix is represented internally in
		/// row major format. Vector multiplication and transformation
		/// matrices are described using row vector format.Typically this 
		/// would not be used directly instead the f32 typedef Matrix4 
		/// should be used.
		///
		/// @author I Copland
		//-------------------------------------------------------------
		template <typename TType> class GenericMatrix4 final
		{
		public:
			//------------------------------------------------------
			/// Constants
			//------------------------------------------------------
			static const GenericMatrix4<TType> k_identity;
			//------------------------------------------------------
			/// Creates a new perspective projection matrix with the 
			/// given parameters. This projection matrix is "Left
			/// Handed" meaning the positive z axis is considered
			/// to point into the screen.
			///
			/// @author I Copland
			///
			/// @param the view angle.
			/// @param the aspect ratio.
			/// @param the near plane.
			/// @param the far plane.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreatePerspectiveProjectionLH(TType in_viewAngle, TType in_aspectRatio, TType in_near, TType in_far);
			//------------------------------------------------------
			/// Creates a new perspective projection matrix with the 
			/// given parameters. This projection matrix is "Right
			/// Handed" meaning the positive z axis is considered
			/// to point out of the screen.
			///
			/// @author I Copland
			///
			/// @param the view angle.
			/// @param the aspect ratio.
			/// @param the near plane.
			/// @param the far plane.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreatePerspectiveProjectionRH(TType in_viewAngle, TType in_aspectRatio, TType in_near, TType in_far);
			//------------------------------------------------------
			/// Creates a new orthographic projection matrix with the 
			/// given parameters.  This projection matrix is "Left
			/// Handed" meaning the positive z axis is considered
			/// to point into the screen.
			///
			/// @author I Copland
			///
			/// @param the width.
			/// @param the height.
			/// @param the near plane.
			/// @param the far plane.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateOrthographicProjectionLH(TType in_width, TType in_height, TType in_near, TType in_far);
			//------------------------------------------------------
			/// Creates a new orthographic projection matrix with the 
			/// given parameters.  This projection matrix is "Right
			/// Handed" meaning the positive z axis is considered
			/// to point out of the screen.
			///
			/// @author I Copland
			///
			/// @param the width.
			/// @param the height.
			/// @param the near plane.
			/// @param the far plane.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateOrthographicProjectionRH(TType in_width, TType in_height, TType in_near, TType in_far);
			//------------------------------------------------------
			/// Creates a new "look-at" matrix with the given camera
			/// position, look at and up vector.
			///
			/// @author I Copland
			///
			/// @param the position.
			/// @param the look at.
			/// @param the up.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateLookAt(const GenericVector3<TType>& in_position, const GenericVector3<TType>& in_lookAt, const GenericVector3<TType>& in_up);
			//------------------------------------------------------
			/// Create a new translation matrix.
			///
			/// @author I Copland
			///
			/// @param The translation.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateTranslation(const GenericVector3<TType>& in_translation);
			//------------------------------------------------------
			/// Creates a new scale matrix.
			///
			/// @author I Copland
			///
			/// @param The scale.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateScale(const GenericVector3<TType>& in_scale);
			//------------------------------------------------------
			/// Create a new matrix that describes a rotation
			/// arround the X axis.
			///
			/// @author I Copland
			///
			/// @param The angle of the rotation in radians.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateRotationX(TType in_angle);
			//------------------------------------------------------
			/// Create a new matrix that describes a rotation
			/// arround the Y axis.
			///
			/// @author I Copland
			///
			/// @param The angle of the rotation in radians.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateRotationY(TType in_angle);
			//------------------------------------------------------
			/// Create a new matrix that describes a rotation
			/// arround the Y axis.
			///
			/// @author I Copland
			///
			/// @param The angle of the rotation in radians.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateRotationZ(TType in_angle);
			//------------------------------------------------------
			/// Creates a rotation matrix from a quaternion.
			///
			/// @author I Copland
			///
			/// @param the quaternion.
			//------------------------------------------------------
			//static GenericMatrix4<TType> CreateRotation(const GenericQuaternion<TType>& in_rotation);
			//------------------------------------------------------
			/// Creates a transform matrix, built out of a translation
			/// scale and rotation.
			///
			/// @author I Copland
			///
			/// @param The translation.
			/// @param The scale.
			/// @param The rotation quaternion.
			///
			/// @return The new transform matrix.
			//------------------------------------------------------
			//static GenericMatrix4<TType> CreateTransform(const GenericVector3<TType>& in_position, const GenericVector3<TType>& in_scale, const GenericQuaternion<TType>& in_rotation);
			//------------------------------------------------------
			/// Constructor. Sets the contents of the matrix to the
			/// identity matrix.
			///
			/// @author I Copland
			//------------------------------------------------------
			GenericMatrix4();
			//------------------------------------------------------
			/// Constructor
			///
			/// @author I Copland
			///
			/// @params The 16 matrix elements in row major format.
			//------------------------------------------------------
			GenericMatrix4(TType in_a0, TType in_a1, TType in_a2, TType in_a3, TType in_b0, TType in_b1, TType in_b2, TType in_b3, TType in_c0, TType in_c1, TType in_c2, TType in_c3, TType in_d0, TType in_d1, TType in_d2, TType in_d3);
			//------------------------------------------------------
			/// @author I Copland
			///
			/// @return The determinant of this matrix.
			//------------------------------------------------------
			TType Determinant() const;
			//------------------------------------------------------
			/// Sets this matrix to the identity matrix.
			///
			/// @author I Copland
			//------------------------------------------------------
			void Identity();
			//------------------------------------------------------
			/// Sets this matrix to its transpose.
			///
			/// @author I Copland
			//------------------------------------------------------
			void Transpose();
			//------------------------------------------------------
			/// @author I Copland
			///
			/// @return A transposed copy of this matrix.
			//------------------------------------------------------
			GenericMatrix4<TType> TransposeCopy() const;
			//------------------------------------------------------
			/// Sets this matrix to its inverse.
			///
			/// @author I Copland
			//------------------------------------------------------
			void Inverse();
			//------------------------------------------------------
			/// @author I Copland
			///
			/// @return An inverted copy of this matrix.
			//------------------------------------------------------
			GenericMatrix4<TType> InverseCopy() const;
			//------------------------------------------------------
			/// Translate this matrix.
			///
			/// @author I Copland
			///
			/// @param the translation vector
			//------------------------------------------------------
			void Translate(const GenericVector3<TType>& in_translation);
			//------------------------------------------------------
			/// Scales this matrix
			///
			/// @author I Copland
			///
			/// @param the scale vector
			//------------------------------------------------------
			void Scale(const GenericVector3<TType>& in_scale);
			//------------------------------------------------------
			/// Rotates arround the X axis.
			///
			/// @author I Copland
			///
			/// @param The rotation angle in radians.
			//------------------------------------------------------
			void RotateX(TType in_angle);
			//------------------------------------------------------
			/// Rotates arround the Y axis.
			///
			/// @author I Copland
			///
			/// @param The rotation angle in radians.
			//------------------------------------------------------
			void RotateY(TType in_angle);
			//------------------------------------------------------
			/// Rotates arround the Z axis.
			///
			/// @author I Copland
			///
			/// @param The rotation angle in radians.
			//------------------------------------------------------
			void RotateZ(TType in_angle);
			//------------------------------------------------------
			/// Rotates the matrix by the given quaternion rotation.
			///
			/// @author I Copland
			///
			/// @param the rotation quaternion.
			//------------------------------------------------------
			//void Rotate(const GenericQuaternion<TType>& inOrientation);
			//------------------------------------------------------
			/// Decompose the matrix into the transform, scale and
			/// rotation it was created with. 
			///
			/// @author I Copland
			///
			/// @param [Out] The translation part of the transform.
			/// @param [Out] The scale part of the transform.
			/// @param [Out] The rotation part of the transform.
			//------------------------------------------------------
			//void Decompose(GenericVector3<TType>& out_translation, GenericVector3<TType> & out_scale, GenericQuaternion<TType> & out_orientation);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param The row index.
			/// @param The column index.
			///
			/// @return The value at the given position in the
			/// matrix. Matrices are row major so this equates to
			/// the value at position in_row * 4 + in_column.
			//-----------------------------------------------------
			TType& operator()(u32 in_row, u32 in_column);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after adding the given matrix
			/// component-wise.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator+=(const GenericMatrix4<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after subtracting the given matrix
			/// component-wise.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator-=(const GenericMatrix4<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after multiplying by the given
			/// matrix.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator*=(const GenericMatrix4<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param A scalar.
			///
			/// @return This matrix after multiplying by the given
			/// scalar.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator*=(TType in_b);

			TType m[16];
		};

		template <typename TType> const GenericMatrix4<TType> GenericMatrix4<TType>::k_identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreatePerspectiveProjectionLH(TType in_viewAngle, TType in_aspectRatio, TType in_near, TType in_far)
		{
			TType b1 = 1 / std::tan(in_viewAngle * 0.5);
			TType a0 = b1 / in_aspectRatio;
			TType c2 = (in_far + in_near) / (in_far - in_near);
			TType d2 = -(2 * in_far * in_near) / (in_far - in_near);
			return GenericMatrix4<TType>(a0, 0, 0, 0, 0, b1, 0, 0, 0, 0, c2, 1, 0, 0, d2, 0);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreatePerspectiveProjectionRH(TType in_viewAngle, TType in_aspectRatio, TType in_near, TType in_far)
		{
			TType b1 = 1 / std::tan(in_viewAngle * 0.5);
			TType a0 = b1 / in_aspectRatio;
			TType c2 = (in_near + in_far) / (in_near - in_far);
			TType d2 = (2 * in_near * in_far) / (in_far - in_near);
			return GenericMatrix4<TType>(a0, 0, 0, 0, 0, b1, 0, 0, 0, 0, c2, -1, 0, 0, d2, 0);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateOrthographicProjectionLH(TType in_width, TType in_height, TType in_near, TType in_far)
		{
			TType a0 = 2 / in_width;
			TType b1 = 2 / in_height;
			TType c2 = 2 / (in_far - in_near);
			TType d2 = (in_far + in_near) / (in_near - in_far);
			return Matrix4(a0, 0, 0, 0, 0, b1, 0, 0, 0, 0, c2, 0, 0, 0, d2, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateOrthographicProjectionRH(TType in_width, TType in_height, TType in_near, TType in_far)
		{
			TType a0 = 2 / in_width;
			TType b1 = 2 / in_height;
			TType c2 = -2 / (in_far - in_near);
			TType d2 = -(in_near + in_far) / (in_far - in_near);
			return GenericMatrix4<TType>(a0, 0, 0, 0, 0, b1, 0, 0, 0, 0, c2, 0, 0, 0, d2, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateLookAt(const GenericVector3<TType>& in_position, const GenericVector3<TType>& in_lookAt, const GenericVector3<TType>& in_up)
		{
			GenericVector3<TType> zAxis = in_lookAt - in_position;
			zAxis.Normalise();
			GenericVector3<TType> xAxis = GenericVector3<TType>::CrossProduct(in_up, zAxis);
			xAxis.Normalise();
			GenericVector3<TType> yAxis = GenericVector3<TType>::CrossProduct(zAxis, xAxis);
			TType dotX = GenericVector3<TType>::DotProduct(xAxis, in_position);
			TType dotY = GenericVector3<TType>::DotProduct(yAxis, in_position);
			TType dotZ = GenericVector3<TType>::DotProduct(zAxis, in_position);

			GenericMatrix4<TType> m;
			m.m[0] = xAxis.X;		m.m[1] = yAxis.X;		m.m[2] = zAxis.X;		m.m[3] = 0;
			m.m[4] = xAxis.Y;		m.m[5] = yAxis.Y;		m.m[6] = zAxis.Y;		m.m[7] = 0;
			m.m[8] = xAxis.Z;		m.m[9] = yAxis.Z;		m.m[10] = zAxis.Z;		m.m[11] = 0;
			m.m[12] = -dotX;		m.m[13] = -dotY;		m.m[14] = -dotZ;		m.m[15] = 1;
			return m;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateTranslation(const GenericVector3<TType>& in_translation)
		{
			return GenericMatrix4<TType>(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, in_translation.X, in_translation.Y, in_translation.Z, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateScale(const GenericVector3<TType>& in_scale)
		{
			return GenericMatrix4<TType>(in_scale.X, 0, 0, 0, 0, in_scale.Y, 0, 0, 0, 0, in_scale.Z, 0, 0, 0, 0, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationX(TType in_angle)
		{
			TType sinA = (TType)std::sin(in_angle);
			TType cosA = (TType)std::cos(in_angle);
			return GenericMatrix4<TType>(1, 0, 0, 0, 0, cosA, sinA, 0, 0, -sinA, cosA, 0, 0, 0, 0, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationY(TType in_angle)
		{
			TType sinA = (TType)std::sin(in_angle);
			TType cosA = (TType)std::cos(in_angle);
			return GenericMatrix4<TType>(cosA, 0, -sinA, 0, 0, 1, 0, 0, sinA, 0, cosA, 0, 0, 0, 0, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationZ(TType inRotation)
		{
			TType sinA = (TType)std::sin(in_angle);
			TType cosA = (TType)std::cos(in_angle);
			return GenericMatrix4<TType>(cosA, sinA, 0, 0, -sinA, cosA, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		//template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotation(const GenericQuaternion<TType>& in_rotation)
		//{
		//	GenericQuaternion<TType> q;
		//	if (in_rotation.MagnitudeSquared() == 1)
		//	{
		//		q = inRotation;
		//	}
		//	else if (q.MagnitudeSquared() > 0)
		//	{
		//		q = in_rotation.NormaliseCopy();
		//	}

		//	TType wSquared = q.W * q.W;
		//	TType xSquared = q.X * q.X;
		//	TType ySquared = q.Y * q.Y;
		//	TType zSquared = q.Z * q.Z;

		//	TType a = wSquared + xSquared - ySquared - zSquared;
		//	TType b = 2 * q.X * q.Y + 2 * q.W * q.Z;
		//	TType c = 2 * q.X * q.Z - 2 * q.W * q.Y;
		//	TType d = 0;
		//	TType e = 2 * q.X * q.Y - 2 * q.W * q.Z;
		//	TType f = wSquared - xSquared + ySquared - zSquared;
		//	TType g = 2 * q.Y * q.Z + 2 * q.W * q.X;
		//	TType h = 0;
		//	TType i = 2 * q.X * q.Z + 2 * q.W * q.Y;
		//	TType j = 2 * q.Y * q.Z - 2 * q.W * q.X;
		//	TType k = wSquared - xSquared - ySquared + zSquared;
		//	TType l = 0;
		//	TType m = 0;
		//	TType n = 0;
		//	TType o = 0;
		//	TType p = 1;

		//	return GenericMatrix4<TType>(a, b, c, d,
		//		e, f, g, h,
		//		i, j, k, l,
		//		m, n, o, p);
		//}
		//------------------------------------------------------
		//------------------------------------------------------
		//template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateTransform(const GenericVector3<TType>& in_position, const GenericVector3<TType>& in_scale, const GenericQuaternion<TType>& in_rotation)
		//{
		//	//TODO: Implement
		//}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType>::GenericMatrix4()
		{
			Identity();
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType>::GenericMatrix4(TType in_a0, TType in_a1, TType in_a2, TType in_a3, TType in_b0, TType in_b1, TType in_b2, TType in_b3, TType in_c0, TType in_c1, TType in_c2, TType in_c3, TType in_d0, TType in_d1, TType in_d2, TType in_d3)
		{
			m[0] = in_a0; m[1] = in_a1; m[2] = in_a2; m[3] = in_a3;
			m[4] = in_b0; m[5] = in_b1; m[6] = in_b2; m[7] = in_b3;
			m[8] = in_c0; m[9] = in_c1; m[10] = in_c2; m[11] = in_c3;
			m[12] = in_d0; m[13] = in_d1; m[14] = in_d2; m[15] = in_d3;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Identity()
		{
			m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
			m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
			m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
			m[12] = 0; m[13] = 0; m[14] = 1; m[15] = 0;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Transpose()
		{
			GenericMatrix4<TType> a = *this;
			
#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
			vDSP_mtrans(const_cast<f32*>(a.m), 1, m, 1, 4, 4);
#else 
			m[0] = a.m[0]; m[1] = a.m[4]; m[2] = a.m[8]; m[3] = a.m[12];
			m[4] = a.m[1]; m[5] = a.m[5]; m[6] = a.m[9]; m[7] = a.m[13];
			m[8] = a.m[2]; m[9] = a.m[6]; m[10] = a.m[10]; m[11] = a.m[14];
			m[12] = a.m[3]; m[13] = a.m[7]; m[14] = a.m[11]; m[15] = a.m[15];
#endif
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::TransposeCopy() const
		{
			Matrix4 A = *this;

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
			vDSP_mtrans(const_cast<f32*>(m), 1, a.m, 1, 4, 4);
#else 
			A.m[0] = m[0]; A.m[1] = m[4]; A.m[2] = m[8]; A.m[3] = m[12];
			A.m[4] = m[1]; A.m[5] = m[5]; A.m[6] = m[9]; A.m[7] = m[13];
			A.m[8] = m[2]; A.m[9] = m[6]; A.m[10] = m[10]; A.m[11] = m[14];
			A.m[12] = m[3]; A.m[13] = m[7]; A.m[14] = m[11]; A.m[15] = m[15];
#endif
			return A;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> TType GenericMatrix4<TType>::Determinant() const
		{
			return	m[3] * m[6] * m[9] * m[12] - m[2] * m[7] * m[9] * m[12] - m[3] * m[5] * m[10] * m[12] + m[1] * m[7] * m[10] * m[12] +
				m[2] * m[5] * m[11] * m[12] - m[1] * m[6] * m[11] * m[12] - m[3] * m[6] * m[8] * m[13] + m[2] * m[7] * m[8] * m[13] +
				m[3] * m[4] * m[10] * m[13] - m[0] * m[7] * m[10] * m[13] - m[2] * m[4] * m[11] * m[13] + m[0] * m[6] * m[11] * m[13] +
				m[3] * m[5] * m[8] * m[14] - m[1] * m[7] * m[8] * m[14] - m[3] * m[4] * m[9] * m[14] + m[0] * m[7] * m[9] * m[14] +
				m[1] * m[4] * m[11] * m[14] - m[0] * m[5] * m[11] * m[14] - m[2] * m[5] * m[8] * m[15] + m[1] * m[6] * m[8] * m[15] +
				m[2] * m[4] * m[9] * m[15] - m[0] * m[6] * m[9] * m[15] - m[1] * m[4] * m[10] * m[15] + m[0] * m[5] * m[10] * m[15];
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Inverse()
		{
			*this = InverseCopy();
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::InverseCopy() const
		{
			TType det = Determinant();
			if (det == 0)
			{
				return *this;
			}

			GenericMatrix4<TType> a = *this;
			a.m[0] = (m[6] * m[11] * m[13] - m[7] * m[10] * m[13] + m[7] * m[9] * m[14] - m[5] * m[11] * m[14] - m[6] * m[9] * m[15] + m[5] * m[10] * m[15]) / det;
			a.m[1] = (m[3] * m[10] * m[13] - m[2] * m[11] * m[13] - m[3] * m[9] * m[14] + m[1] * m[11] * m[14] + m[2] * m[9] * m[15] - m[1] * m[10] * m[15]) / det;
			a.m[2] = (m[2] * m[7] * m[13] - m[3] * m[6] * m[13] + m[3] * m[5] * m[14] - m[1] * m[7] * m[14] - m[2] * m[5] * m[15] + m[1] * m[6] * m[15]) / det;
			a.m[3] = (m[3] * m[6] * m[9] - m[2] * m[7] * m[9] - m[3] * m[5] * m[10] + m[1] * m[7] * m[10] + m[2] * m[5] * m[11] - m[1] * m[6] * m[11]) / det;
			a.m[4] = (m[7] * m[10] * m[12] - m[6] * m[11] * m[12] - m[7] * m[8] * m[14] + m[4] * m[11] * m[14] + m[6] * m[8] * m[15] - m[4] * m[10] * m[15]) / det;
			a.m[5] = (m[2] * m[11] * m[12] - m[3] * m[10] * m[12] + m[3] * m[8] * m[14] - m[0] * m[11] * m[14] - m[2] * m[8] * m[15] + m[0] * m[10] * m[15]) / det;
			a.m[6] = (m[3] * m[6] * m[12] - m[2] * m[7] * m[12] - m[3] * m[4] * m[14] + m[0] * m[7] * m[14] + m[2] * m[4] * m[15] - m[0] * m[6] * m[15]) / det;
			a.m[7] = (m[2] * m[7] * m[8] - m[3] * m[6] * m[8] + m[3] * m[4] * m[10] - m[0] * m[7] * m[10] - m[2] * m[4] * m[11] + m[0] * m[6] * m[11]) / det;
			a.m[8] = (m[5] * m[11] * m[12] - m[7] * m[9] * m[12] + m[7] * m[8] * m[13] - m[4] * m[11] * m[13] - m[5] * m[8] * m[15] + m[4] * m[9] * m[15]) / det;
			a.m[9] = (m[3] * m[9] * m[12] - m[1] * m[11] * m[12] - m[3] * m[8] * m[13] + m[0] * m[11] * m[13] + m[1] * m[8] * m[15] - m[0] * m[9] * m[15]) / det;
			a.m[10] = (m[1] * m[7] * m[12] - m[3] * m[5] * m[12] + m[3] * m[4] * m[13] - m[0] * m[7] * m[13] - m[1] * m[4] * m[15] + m[0] * m[5] * m[15]) / det;
			a.m[11] = (m[3] * m[5] * m[8] - m[1] * m[7] * m[8] - m[3] * m[4] * m[9] + m[0] * m[7] * m[9] + m[1] * m[4] * m[11] - m[0] * m[5] * m[11]) / det;
			a.m[12] = (m[6] * m[9] * m[12] - m[5] * m[10] * m[12] - m[6] * m[8] * m[13] + m[4] * m[10] * m[13] + m[5] * m[8] * m[14] - m[4] * m[9] * m[14]) / det;
			a.m[13] = (m[1] * m[10] * m[12] - m[2] * m[9] * m[12] + m[2] * m[8] * m[13] - m[0] * m[10] * m[13] - m[1] * m[8] * m[14] + m[0] * m[9] * m[14]) / det;
			a.m[14] = (m[2] * m[5] * m[12] - m[1] * m[6] * m[12] - m[2] * m[4] * m[13] + m[0] * m[6] * m[13] + m[1] * m[4] * m[14] - m[0] * m[5] * m[14]) / det;
			a.m[15] = (m[1] * m[6] * m[8] - m[2] * m[5] * m[8] + m[2] * m[4] * m[9] - m[0] * m[6] * m[9] - m[1] * m[4] * m[10] + m[0] * m[5] * m[10]) / det;
			return a;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Translate(const GenericVector3<TType>& in_translation)
		{
			*this *= CreateTranslation(in_translation);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Scale(const GenericVector3<TType>& in_scale)
		{
			*this *= CreateScale(in_scale);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::RotateX(TType in_angle)
		{
			*this *= CreateRotationX(in_angle);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::RotateY(TType in_angle)
		{
			*this *= CreateRotationY(in_angle);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::RotateZ(TType in_angle)
		{
			*this *= CreateRotationZ(in_angle);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		//template <typename TType> void GenericMatrix4<TType>::Rotate(const GenericQuaternion<TType>& inOrientation)
		//{
		//	*this *= CreateRotation(inOrientation);
		//}
		//------------------------------------------------------
		//------------------------------------------------------
		//template <typename TType> void GenericMatrix4<TType>::Decompose(GenericVector3<TType>& out_translation, GenericVector3<TType> & out_scale, GenericQuaternion<TType> & out_orientation)
		//{
		//	//TODO: Implement
		//}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator+=(const GenericMatrix4<TType>& in_b)
		{
			m[0] += in_b.m[0]; m[1] += in_b.m[1]; m[2] += in_b.m[2]; m[3] += in_b.m[3];
			m[4] += in_b.m[4]; m[5] += in_b.m[5]; m[6] += in_b.m[6]; m[7] += in_b.m[7];
			m[8] += in_b.m[8]; m[9] += in_b.m[9]; m[10] += in_b.m[10]; m[11] += in_b.m[11];
			m[12] += in_b.m[12]; m[13] += in_b.m[13]; m[14] += in_b.m[14]; m[15] += in_b.m[15];
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator-=(const GenericMatrix4<TType>& in_b)
		{
			m[0] += in_b.m[0]; m[1] += in_b.m[1]; m[2] += in_b.m[2]; m[3] += in_b.m[3];
			m[4] += in_b.m[4]; m[5] += in_b.m[5]; m[6] += in_b.m[6]; m[7] += in_b.m[7];
			m[8] += in_b.m[8]; m[9] += in_b.m[9]; m[10] += in_b.m[10]; m[11] += in_b.m[11];
			m[12] += in_b.m[12]; m[13] += in_b.m[13]; m[14] += in_b.m[14]; m[15] += in_b.m[15];
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator*=(const GenericMatrix4<TType>& in_b)
		{
			GenericMatrix4<TType> c = *this;

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
			vDSP_mmul(const_cast<f32*>(c.m), 1, const_cast<f32*>(in_b.m), 1, m, 1, 4, 4, 4);
#else
			m[0] = c.m[0] * in_b.m[0] + c.m[1] * in_b.m[4] + c.m[2] * in_b.m[8] + c.m[3] * in_b.m[12];
			m[1] = c.m[0] * in_b.m[1] + c.m[1] * in_b.m[5] + c.m[2] * in_b.m[9] + c.m[3] * in_b.m[13];
			m[2] = c.m[0] * in_b.m[2] + c.m[1] * in_b.m[6] + c.m[2] * in_b.m[10] + c.m[3] * in_b.m[14];
			m[3] = c.m[0] * in_b.m[3] + c.m[1] * in_b.m[7] + c.m[2] * in_b.m[11] + c.m[3] * in_b.m[15];

			m[4] = c.m[4] * in_b.m[0] + c.m[5] * in_b.m[4] + c.m[6] * in_b.m[8] + c.m[7] * in_b.m[12];
			m[5] = c.m[4] * in_b.m[1] + c.m[5] * in_b.m[5] + c.m[6] * in_b.m[9] + c.m[7] * in_b.m[13];
			m[6] = c.m[4] * in_b.m[2] + c.m[5] * in_b.m[6] + c.m[6] * in_b.m[10] + c.m[7] * in_b.m[14];
			m[7] = c.m[4] * in_b.m[3] + c.m[5] * in_b.m[7] + c.m[6] * in_b.m[11] + c.m[7] * in_b.m[15];

			m[8] = c.m[8] * in_b.m[0] + c.m[9] * in_b.m[4] + c.m[10] * in_b.m[8] + c.m[11] * in_b.m[12];
			m[9] = c.m[8] * in_b.m[1] + c.m[9] * in_b.m[5] + c.m[10] * in_b.m[9] + c.m[11] * in_b.m[13];
			m[10] = c.m[8] * in_b.m[2] + c.m[9] * in_b.m[6] + c.m[10] * in_b.m[10] + c.m[11] * in_b.m[14];
			m[11] = c.m[8] * in_b.m[3] + c.m[9] * in_b.m[7] + c.m[10] * in_b.m[11] + c.m[11] * in_b.m[15];

			m[12] = c.m[12] * in_b.m[0] + c.m[13] * in_b.m[4] + c.m[14] * in_b.m[8] + c.m[15] * in_b.m[12];
			m[13] = c.m[12] * in_b.m[1] + c.m[13] * in_b.m[5] + c.m[14] * in_b.m[9] + c.m[15] * in_b.m[13];
			m[14] = c.m[12] * in_b.m[2] + c.m[13] * in_b.m[6] + c.m[14] * in_b.m[10] + c.m[15] * in_b.m[14];
			m[15] = c.m[12] * in_b.m[3] + c.m[13] * in_b.m[7] + c.m[14] * in_b.m[11] + c.m[15] * in_b.m[15];
#endif
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator*=(TType in_b)
		{
			m[0] *= in_b; m[1] *= in_b; m[2] *= in_b; m[3] *= in_b;
			m[4] *= in_b; m[5] *= in_b; m[6] *= in_b; m[7] *= in_b;
			m[8] *= in_b; m[9] *= in_b; m[10] *= in_b; m[11] *= in_b;
			m[12] *= in_b; m[13] *= in_b; m[14] *= in_b; m[15] *= in_b;
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator+(GenericMatrix4<TType> in_a, const GenericMatrix4<TType>& in_b)
		{
			return (in_a += in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator-(GenericMatrix4<TType> in_a, const GenericMatrix4<TType>& in_b)
		{
			return (in_a -= in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator*(const GenericMatrix4<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			GenericMatrix4<TType> c;

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
			vDSP_mmul(const_cast<f32*>(in_a.m), 1, const_cast<f32*>(in_b.m), 1, c.m, 1, 4, 4, 4);
#else
			c.m[0] = in_a.m[0] * in_b.m[0] + in_a.m[1] * in_b.m[4] + in_a.m[2] * in_b.m[8] + in_a.m[3] * in_b.m[12];
			c.m[1] = in_a.m[0] * in_b.m[1] + in_a.m[1] * in_b.m[5] + in_a.m[2] * in_b.m[9] + in_a.m[3] * in_b.m[13];
			c.m[2] = in_a.m[0] * in_b.m[2] + in_a.m[1] * in_b.m[6] + in_a.m[2] * in_b.m[10] + in_a.m[3] * in_b.m[14];
			c.m[3] = in_a.m[0] * in_b.m[3] + in_a.m[1] * in_b.m[7] + in_a.m[2] * in_b.m[11] + in_a.m[3] * in_b.m[15];

			c.m[4] = in_a.m[4] * in_b.m[0] + in_a.m[5] * in_b.m[4] + in_a.m[6] * in_b.m[8] + in_a.m[7] * in_b.m[12];
			c.m[5] = in_a.m[4] * in_b.m[1] + in_a.m[5] * in_b.m[5] + in_a.m[6] * in_b.m[9] + in_a.m[7] * in_b.m[13];
			c.m[6] = in_a.m[4] * in_b.m[2] + in_a.m[5] * in_b.m[6] + in_a.m[6] * in_b.m[10] + in_a.m[7] * in_b.m[14];
			c.m[7] = in_a.m[4] * in_b.m[3] + in_a.m[5] * in_b.m[7] + in_a.m[6] * in_b.m[11] + in_a.m[7] * in_b.m[15];

			c.m[8] = in_a.m[8] * in_b.m[0] + in_a.m[9] * in_b.m[4] + in_a.m[10] * in_b.m[8] + in_a.m[11] * in_b.m[12];
			c.m[9] = in_a.m[8] * in_b.m[1] + in_a.m[9] * in_b.m[5] + in_a.m[10] * in_b.m[9] + in_a.m[11] * in_b.m[13];
			c.m[10] = in_a.m[8] * in_b.m[2] + in_a.m[9] * in_b.m[6] + in_a.m[10] * in_b.m[10] + in_a.m[11] * in_b.m[14];
			c.m[11] = in_a.m[8] * in_b.m[3] + in_a.m[9] * in_b.m[7] + in_a.m[10] * in_b.m[11] + in_a.m[11] * in_b.m[15];

			c.m[12] = in_a.m[12] * in_b.m[0] + in_a.m[13] * in_b.m[4] + in_a.m[14] * in_b.m[8] + in_a.m[15] * in_b.m[12];
			c.m[13] = in_a.m[12] * in_b.m[1] + in_a.m[13] * in_b.m[5] + in_a.m[14] * in_b.m[9] + in_a.m[15] * in_b.m[13];
			c.m[14] = in_a.m[12] * in_b.m[2] + in_a.m[13] * in_b.m[6] + in_a.m[14] * in_b.m[10] + in_a.m[15] * in_b.m[14];
			c.m[15] = in_a.m[12] * in_b.m[3] + in_a.m[13] * in_b.m[7] + in_a.m[14] * in_b.m[11] + in_a.m[15] * in_b.m[15];
#endif
			return c;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator*(GenericMatrix4<TType> in_a, TType in_b)
		{
			return (in_a *= in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator*(TType in_a, GenericMatrix4<TType> in_b)
		{
			return (in_b *= in_a);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector4<TType>& operator*=(GenericVector4<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			GenericVector4<TType> c = in_a;
			in_a.X = c.X * in_b.m[0] + c.Y * in_b.m[4] + c.Z * in_b.m[8] + c.W * in_b.m[12];
			in_a.Y = c.X * in_b.m[1] + c.Y * in_b.m[5] + c.Z * in_b.m[9] + c.W * in_b.m[13];
			in_a.Z = c.X * in_b.m[2] + c.Y * in_b.m[6] + c.Z * in_b.m[10] + c.W * in_b.m[14];
			in_a.W = c.X * in_b.m[3] + c.Y * in_b.m[7] + c.Z * in_b.m[11] + c.W * in_b.m[15];
			return in_a;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector4<TType> operator*(const GenericVector4<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			GenericVector4<TType> c;
			c.X = in_a.X * in_b.m[0] + in_a.Y * in_b.m[4] + in_a.Z * in_b.m[8] + in_a.W * in_b.m[12];
			c.Y = in_a.X * in_b.m[1] + in_a.Y * in_b.m[5] + in_a.Z * in_b.m[9] + in_a.W * in_b.m[13];
			c.Z = in_a.X * in_b.m[2] + in_a.Y * in_b.m[6] + in_a.Z * in_b.m[10] + in_a.W * in_b.m[14];
			c.W = in_a.X * in_b.m[3] + in_a.Y * in_b.m[7] + in_a.Z * in_b.m[11] + in_a.W * in_b.m[15];
			return c;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType>& operator*=(GenericVector3<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			GenericVector3<TType> c = in_a;
			in_a.X = c.X * in_b.m[0] + c.Y * in_b.m[4] + c.Z * in_b.m[8] + in_b.m[12];
			in_a.Y = c.X * in_b.m[1] + c.Y * in_b.m[5] + c.Z * in_b.m[9] + in_b.m[13];
			in_a.Z = c.X * in_b.m[2] + c.Y * in_b.m[6] + c.Z * in_b.m[10] + in_b.m[14];
			TType oneOverW = 1 / (c.X * in_b.m[3] + c.Y * in_b.m[7] + c.Z * in_b.m[11] + in_b.m[15]);
			in_a *= oneOverW;
			return in_a;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			GenericVector3<TType> c;
			c.X = in_a.X * in_b.m[0] + in_a.Y * in_b.m[4] + in_a.Z * in_b.m[8] + in_b.m[12];
			c.Y = in_a.X * in_b.m[1] + in_a.Y * in_b.m[5] + in_a.Z * in_b.m[9] + in_b.m[13];
			c.Z = in_a.X * in_b.m[2] + in_a.Y * in_b.m[6] + in_a.Z * in_b.m[10] + in_b.m[14];
			TType oneOverW = 1 / (in_a.X * in_b.m[3] + in_a.Y * in_b.m[7] + in_a.Z * in_b.m[11] + in_b.m[15]);
			c *= oneOverW;
			return c;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> bool operator==(const GenericMatrix4<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			for (u32 i = 0; i < 16; ++i)
			{
				if (in_a.m[i] != in_b.m[i])
					return false;
			}
			return true;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> bool operator!=(const GenericMatrix4<TType>& in_a, const GenericMatrix4<TType>& in_b)
		{
			return !(in_a == in_b);
		}
	}
}

#endif