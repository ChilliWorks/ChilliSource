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

namespace ChilliSource
{
	namespace Core
	{
		//-------------------------------------------------------------
		/// A generic 4x4 matrix that provides standard matrix math
		/// functionality. The matrix is represented internally in
		/// row major format. Vector multiplication and transformation
		/// matrices are described using row vector format.Typically this 
		/// would not be used directly, instead the f32 typedef Matrix4
		/// should be used.
		///
		/// @author Ian Copland
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
			/// @author Ian Copland
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
			/// @author Ian Copland
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
			/// @author Ian Copland
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
			/// @author Ian Copland
			///
			/// @param the width.
			/// @param the height.
			/// @param the near plane.
			/// @param the far plane.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateOrthographicProjectionRH(TType in_width, TType in_height, TType in_near, TType in_far);
			//------------------------------------------------------
			/// Creates a new orthographic projection matrix with the 
			/// given parameters.  This projection matrix is "Left
			/// Handed" meaning the positive z axis is considered
			/// to point into the screen.
			///
			/// @author Ian Copland
			///
			/// @param the left side.
			/// @param The right side.
			/// @param The bottom.
			/// @param The top.
			/// @param the near.
			/// @param The far.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateOrthographicProjectionLH(TType in_left, f32 in_right, f32 in_bottom, f32 in_top, f32 in_near, f32 in_far);
			//------------------------------------------------------
			/// Creates a new orthographic projection matrix with the 
			/// given parameters.  This projection matrix is "Right
			/// Handed" meaning the positive z axis is considered
			/// to point out of the screen.
			///
			/// @author Ian Copland
			///
			/// @param the left side.
			/// @param The right side.
			/// @param The bottom.
			/// @param The top.
			/// @param the near.
			/// @param The far.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateOrthographicProjectionRH(TType in_left, f32 in_right, f32 in_bottom, f32 in_top, f32 in_near, f32 in_far);
			//------------------------------------------------------
			/// Creates a new "look-at" matrix with the given camera
			/// position, look at and up vector.
			///
			/// @author Ian Copland
			///
			/// @param the position.
			/// @param the look at.
			/// @param the up.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateLookAt(const GenericVector3<TType>& in_position, const GenericVector3<TType>& in_target, const GenericVector3<TType>& in_up);
			//------------------------------------------------------
			/// Create a new translation matrix.
			///
			/// @author Ian Copland
			///
			/// @param The translation.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateTranslation(const GenericVector3<TType>& in_translation);
            //------------------------------------------------------
			/// Create a new translation matrix.
			///
			/// @author Ian Copland
			///
			/// @param The x translation.
            /// @param The y translation.
            /// @param The z translation.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateTranslation(TType in_x, TType in_y, TType in_z);
			//------------------------------------------------------
			/// Creates a new scale matrix.
			///
			/// @author Ian Copland
			///
			/// @param The scale.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateScale(const GenericVector3<TType>& in_scale);
            //------------------------------------------------------
			/// Create a new scale matrix.
			///
			/// @author Ian Copland
			///
			/// @param The x scale.
            /// @param The y scale.
            /// @param The z scale.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateScale(TType in_x, TType in_y, TType in_z);
			//------------------------------------------------------
			/// Create a new matrix that describes a rotation
			/// arround the x axis.
			///
			/// @author Ian Copland
			///
			/// @param The angle of the rotation in radians.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateRotationX(TType in_angle);
			//------------------------------------------------------
			/// Create a new matrix that describes a rotation
			/// arround the y axis.
			///
			/// @author Ian Copland
			///
			/// @param The angle of the rotation in radians.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateRotationY(TType in_angle);
			//------------------------------------------------------
			/// Create a new matrix that describes a rotation
			/// arround the y axis.
			///
			/// @author Ian Copland
			///
			/// @param The angle of the rotation in radians.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateRotationZ(TType in_angle);
			//------------------------------------------------------
			/// Creates a rotation matrix from a quaternion.
			///
			/// @author Ian Copland
			///
			/// @param the quaternion.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateRotation(const GenericQuaternion<TType>& in_rotation);
            //------------------------------------------------------
			/// Creates a rotation matrix arround the given axis
            /// by the given angle.
			///
			/// @author Ian Copland
			///
			/// @param the axis.
            /// @param The angle.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateRotation(const GenericVector3<TType>& in_axis, TType in_angle);
			//------------------------------------------------------
			/// Creates a transform matrix, built out of a translation
			/// scale and rotation.
			///
			/// @author S Downie
			///
			/// @param The translation.
			/// @param The scale.
			/// @param The rotation quaternion.
			///
			/// @return The new transform matrix.
			//------------------------------------------------------
			static GenericMatrix4<TType> CreateTransform(const GenericVector3<TType>& in_translation, const GenericVector3<TType>& in_scale, const GenericQuaternion<TType>& in_rotation);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			///
			/// @return A transposed copy of the matrix.
			//------------------------------------------------------
			static GenericMatrix4<TType> Transpose(const GenericMatrix4<TType>& in_a);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			///
			/// @return An inverted copy of the matrix.
			//------------------------------------------------------
			static GenericMatrix4<TType> Inverse(const GenericMatrix4<TType>& in_a);
            //------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param A translation vector.
            ///
            /// @return A copy of the matrix with the given translation
            /// applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> Translate(const GenericMatrix4<TType>& in_a, const GenericVector3<TType>& in_translation);
            //------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param The x translation.
			/// @param The y translation.
			/// @param The z translation.
            ///
            /// @return A copy of the matrix with the given translation
            /// applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> Translate(const GenericMatrix4<TType>& in_a, TType in_x, TType in_y, TType in_z);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param A scale vector.
            ///
            /// @return A copy of the matrix with the given scale
            /// applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> Scale(const GenericMatrix4<TType>& in_a, const GenericVector3<TType>& in_scale);
            //------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param The x scale.
			/// @param The y scale.
			/// @param The z scale.
            ///
            /// @return A copy of the matrix with the given scale
            /// applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> Scale(const GenericMatrix4<TType>& in_a, TType in_x, TType in_y, TType in_z);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param A rotation angle.
            ///
            /// @return A copy of the matrix with the given X axis
            /// rotation applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> RotateX(const GenericMatrix4<TType>& in_a, TType in_angle);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param A rotation angle.
            ///
            /// @return A copy of the matrix with the given Y axis
            /// rotation applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> RotateY(const GenericMatrix4<TType>& in_a, TType in_angle);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param A rotation angle.
            ///
            /// @return A copy of the matrix with the given Z axis
            /// rotation applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> RotateZ(const GenericMatrix4<TType>& in_a, TType in_angle);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param A orientation quaternion.
            ///
            /// @return A copy of the matrix with the given rotation
            /// applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> Rotate(const GenericMatrix4<TType>& in_a, const GenericQuaternion<TType>& in_orientation);
            //------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			/// @param The axis to rotate around.
            /// @param The angle to rotate through.
            ///
            /// @return A copy of the matrix with the given rotation
            /// applied to it.
			//------------------------------------------------------
			static GenericMatrix4<TType> Rotate(const GenericMatrix4<TType>& in_a, const GenericVector3<TType>& in_axis, TType in_angle);
			//------------------------------------------------------
			/// Constructor. Sets the contents of the matrix to the
			/// identity matrix.
			///
			/// @author Ian Copland
			//------------------------------------------------------
			GenericMatrix4();
			//------------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @params The 16 matrix elements in row major format.
			//------------------------------------------------------
			GenericMatrix4(TType in_a0, TType in_a1, TType in_a2, TType in_a3, TType in_b0, TType in_b1, TType in_b2, TType in_b3, TType in_c0, TType in_c1, TType in_c2, TType in_c3, TType in_d0, TType in_d1, TType in_d2, TType in_d3);
			//------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The determinant of this matrix.
			//------------------------------------------------------
			TType Determinant() const;
			//------------------------------------------------------
			/// Sets this matrix to the identity matrix.
			///
			/// @author Ian Copland
			//------------------------------------------------------
			void Identity();
			//------------------------------------------------------
			/// Sets this matrix to its transpose.
			///
			/// @author Ian Copland
			//------------------------------------------------------
			void Transpose();
			//------------------------------------------------------
			/// Sets this matrix to its inverse.
			///
			/// @author Ian Copland
			//------------------------------------------------------
			void Inverse();
			//------------------------------------------------------
			/// Translate this matrix.
			///
			/// @author Ian Copland
			///
			/// @param the translation vector
			//------------------------------------------------------
			void Translate(const GenericVector3<TType>& in_translation);
            //------------------------------------------------------
			/// Translate this matrix.
			///
			/// @author Ian Copland
			///
			/// @param the x translation.
			/// @param the y translation.
			/// @param the z translation.
			//------------------------------------------------------
			void Translate(TType in_x, TType in_y, TType in_z);
			//------------------------------------------------------
			/// Scales this matrix
			///
			/// @author Ian Copland
			///
			/// @param the scale vector
			//------------------------------------------------------
			void Scale(const GenericVector3<TType>& in_scale);
            //------------------------------------------------------
			/// Scales this matrix
			///
			/// @author Ian Copland
			///
			/// @param the x scale.
			/// @param the y scale.
			/// @param the z scale.
			//------------------------------------------------------
			void Scale(TType in_x, TType in_y, TType in_z);
			//------------------------------------------------------
			/// Rotates arround the x axis.
			///
			/// @author Ian Copland
			///
			/// @param The rotation angle in radians.
			//------------------------------------------------------
			void RotateX(TType in_angle);
			//------------------------------------------------------
			/// Rotates arround the y axis.
			///
			/// @author Ian Copland
			///
			/// @param The rotation angle in radians.
			//------------------------------------------------------
			void RotateY(TType in_angle);
			//------------------------------------------------------
			/// Rotates arround the z axis.
			///
			/// @author Ian Copland
			///
			/// @param The rotation angle in radians.
			//------------------------------------------------------
			void RotateZ(TType in_angle);
			//------------------------------------------------------
			/// Rotates the matrix by the given quaternion rotation.
			///
			/// @author Ian Copland
			///
			/// @param the rotation quaternion.
			//------------------------------------------------------
			void Rotate(const GenericQuaternion<TType>& in_orientation);
            //------------------------------------------------------
			/// Rotates the matrix by the given rotation.
			///
			/// @author Ian Copland
			///
			/// @param The axis to rotate around.
            /// @param The angle to rotate though.
			//------------------------------------------------------
			void Rotate(const GenericVector3<TType>& in_axis, TType in_angle);
			//------------------------------------------------------
			/// @author S Dowie
			///
			/// @param The right vector.
			//------------------------------------------------------
			GenericVector3<TType> GetRight() const;
			//------------------------------------------------------
			/// @author S Dowie
			///
			/// @param The up vector.
			//------------------------------------------------------
			GenericVector3<TType> GetUp() const;
			//------------------------------------------------------
			/// @author S Dowie
			///
			/// @param The forward vector.
			//------------------------------------------------------
			GenericVector3<TType> GetForward() const;
			//------------------------------------------------------
			/// @author S Dowie
			///
			/// @param The translation part of the transform. 
			//------------------------------------------------------
			GenericVector3<TType> GetTranslation() const;
			//------------------------------------------------------
			/// Decompose the matrix into the transform, scale and
			/// rotation it was created with. 
			///
			/// @author S Downie
			///
			/// @param [Out] The translation part of the transform.
			/// @param [Out] The scale part of the transform.
			/// @param [Out] The rotation part of the transform.
			//------------------------------------------------------
			void Decompose(GenericVector3<TType>& out_translation, GenericVector3<TType> & out_scale, GenericQuaternion<TType> & out_orientation) const;
			//-----------------------------------------------------
			/// @author Ian Copland
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
			/// @author Ian Copland
			///
			/// @param The row index.
			/// @param The column index.
			///
			/// @return The value at the given position in the
			/// matrix. Matrices are row major so this equates to
			/// the value at position in_row * 4 + in_column.
			//-----------------------------------------------------
			TType operator()(u32 in_row, u32 in_column) const;
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after adding the given matrix
			/// component-wise.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator+=(const GenericMatrix4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after subtracting the given matrix
			/// component-wise.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator-=(const GenericMatrix4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after multiplying by the given
			/// matrix.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator*=(const GenericMatrix4<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar.
			///
			/// @return This matrix after multiplying by the given
			/// scalar.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator*=(TType in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar.
			///
			/// @return This matrix after dividing by the given
			/// scalar.
			//-----------------------------------------------------
			GenericMatrix4<TType>& operator/=(TType in_b);

			TType m[16];
		};
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The matrix A.
		/// @param The matrix B.
		///
		/// @return The result of A + B.
		//-----------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator+(GenericMatrix4<TType> in_a, const GenericMatrix4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The matrix A.
		/// @param The matrix B.
		///
		/// @return The result of A - B.
		//-----------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator-(GenericMatrix4<TType> in_a, const GenericMatrix4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The matrix A.
		/// @param The matrix B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator*(const GenericMatrix4<TType>& in_a, const GenericMatrix4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The matrix A.
		/// @param The scalar B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator*(GenericMatrix4<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The scalar A.
		/// @param The matrix B.
		///
		/// @return The result of A * B.
		//-----------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator*(TType in_a, GenericMatrix4<TType> in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The matrix A.
		/// @param The scalar B.
		///
		/// @return The result of A / B.
		//-----------------------------------------------------
		template <typename TType> GenericMatrix4<TType> operator/(GenericMatrix4<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The matrix A.
		/// @param The matrix B.
		///
		/// @return Whether or not the matrices are equal.
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericMatrix4<TType>& in_a, const GenericMatrix4<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param The matrix A.
		/// @param The matrix B.
		///
		/// @return Whether or not the matrices are in-equal.
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericMatrix4<TType>& in_a, const GenericMatrix4<TType>& in_b);
	}
}

//----------------------------------------------------
// These are included here to avoid circular inclusion
// issues. At this stage the class has been defined
// which is enough for the classes included to use it.
//----------------------------------------------------
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>

#include <cmath>

namespace ChilliSource
{
	namespace Core
	{
		template <typename TType> const GenericMatrix4<TType> GenericMatrix4<TType>::k_identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreatePerspectiveProjectionLH(TType in_viewAngle, TType in_aspectRatio, TType in_near, TType in_far)
		{
			TType b1 = 1 / (TType)std::tan(in_viewAngle * 0.5);
			TType a0 = b1 / in_aspectRatio;
			TType c2 = (in_far + in_near) / (in_far - in_near);
			TType d2 = -(2 * in_far * in_near) / (in_far - in_near);
			return GenericMatrix4<TType>(
				a0, 0, 0, 0, 
				0, b1, 0, 0, 
				0, 0, c2, 1, 
				0, 0, d2, 0);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreatePerspectiveProjectionRH(TType in_viewAngle, TType in_aspectRatio, TType in_near, TType in_far)
		{
			TType b1 = 1 / (TType)std::tan(in_viewAngle * 0.5);
			TType a0 = b1 / in_aspectRatio;
			TType c2 = (in_near + in_far) / (in_near - in_far);
			TType d2 = (2 * in_near * in_far) /  (in_near - in_far);
			return GenericMatrix4<TType>(
				a0, 0, 0, 0, 
				0, b1, 0, 0,
				0, 0, c2, -1,
				0, 0, d2, 0);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateOrthographicProjectionLH(TType in_width, TType in_height, TType in_near, TType in_far)
		{
			TType a0 = 2 / in_width;
			TType b1 = 2 / in_height;
			TType c2 = 2 / (in_far - in_near);
			TType d2 = (in_far + in_near) / (in_near - in_far);
			return GenericMatrix4<TType>(
				a0, 0, 0, 0, 
				0, b1, 0, 0, 
				0, 0, c2, 0,
				0, 0, d2, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateOrthographicProjectionRH(TType in_width, TType in_height, TType in_near, TType in_far)
		{
			TType a0 = 2 / in_width;
			TType b1 = 2 / in_height;
			TType c2 = -2 / (in_far - in_near);
			TType d2 = -(in_near + in_far) / (in_far - in_near);
			return GenericMatrix4<TType>(
				a0, 0, 0, 0, 
				0, b1, 0, 0, 
				0, 0, c2, 0, 
				0, 0, d2, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateOrthographicProjectionLH(TType in_left, f32 in_right, f32 in_bottom, f32 in_top, f32 in_near, f32 in_far)
		{
            TType a0 = 2 / (in_right - in_left);
			TType b1 = 2 / (in_top - in_bottom);
			TType c2 = 2 / (in_far - in_near);
			TType d0 = -(in_right + in_left) / (in_right - in_left);
			TType d1 = -(in_top + in_bottom) / (in_top - in_bottom);
			TType d2 = (in_near + in_far) / (in_far - in_near);
			return GenericMatrix4<TType>(
                a0, 0, 0, 0,
                0, b1, 0, 0,
                0, 0, c2, 0,
                d0, d1, d2, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateOrthographicProjectionRH(TType in_left, f32 in_right, f32 in_bottom, f32 in_top, f32 in_near, f32 in_far)
		{
			TType a0 = 2 / (in_right - in_left);
			TType b1 = 2 / (in_top - in_bottom);
			TType c2 = -2 / (in_far - in_near);
			TType d0 = -(in_right + in_left) / (in_right - in_left);
			TType d1 = -(in_top + in_bottom) / (in_top - in_bottom);
			TType d2 = -(in_near + in_far) / (in_far - in_near);
			return GenericMatrix4<TType>(
				a0, 0, 0, 0, 
				0, b1, 0, 0, 
				0, 0, c2, 0, 
				d0, d1, d2, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateLookAt(const GenericVector3<TType>& in_position, const GenericVector3<TType>& in_target, const GenericVector3<TType>& in_up)
		{
			GenericVector3<TType> zAxis = in_target - in_position;
			zAxis.Normalise();
			GenericVector3<TType> xAxis = GenericVector3<TType>::CrossProduct(in_up, zAxis);
			xAxis.Normalise();
			GenericVector3<TType> yAxis = GenericVector3<TType>::CrossProduct(zAxis, xAxis);
			TType dotX = GenericVector3<TType>::DotProduct(xAxis, in_position);
			TType dotY = GenericVector3<TType>::DotProduct(yAxis, in_position);
			TType dotZ = GenericVector3<TType>::DotProduct(zAxis, in_position);

			GenericMatrix4<TType> m;
			m.m[0] = xAxis.x;		m.m[1] = yAxis.x;		m.m[2] = zAxis.x;		m.m[3] = 0;
			m.m[4] = xAxis.y;		m.m[5] = yAxis.y;		m.m[6] = zAxis.y;		m.m[7] = 0;
			m.m[8] = xAxis.z;		m.m[9] = yAxis.z;		m.m[10] = zAxis.z;		m.m[11] = 0;
			m.m[12] = -dotX;		m.m[13] = -dotY;		m.m[14] = -dotZ;		m.m[15] = 1;
			return m;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateTranslation(const GenericVector3<TType>& in_translation)
		{
			return GenericMatrix4<TType>(
				1, 0, 0, 0, 
				0, 1, 0, 0, 
				0, 0, 1, 0, 
				in_translation.x, in_translation.y, in_translation.z, 1);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateTranslation(TType in_x, TType in_y, TType in_z)
		{
			return CreateTranslation(GenericVector3<TType>(in_x, in_y, in_z));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateScale(const GenericVector3<TType>& in_scale)
		{
			return GenericMatrix4<TType>(
				in_scale.x, 0, 0, 0,
				0, in_scale.y, 0, 0, 
				0, 0, in_scale.z, 0, 
				0, 0, 0, 1);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateScale(TType in_x, TType in_y, TType in_z)
		{
			return CreateScale(GenericVector3<TType>(in_x, in_y, in_z));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationX(TType in_angle)
		{
			TType sinA = (TType)std::sin(in_angle);
			TType cosA = (TType)std::cos(in_angle);
			return GenericMatrix4<TType>(
				1, 0, 0, 0, 
				0, cosA, sinA, 0, 
				0, -sinA, cosA, 0, 
				0, 0, 0, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationY(TType in_angle)
		{
			TType sinA = (TType)std::sin(in_angle);
			TType cosA = (TType)std::cos(in_angle);
			return GenericMatrix4<TType>(
				cosA, 0, -sinA, 0, 
				0, 1, 0, 0, 
				sinA, 0, cosA, 0, 
				0, 0, 0, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationZ(TType in_angle)
		{
			TType sinA = (TType)std::sin(in_angle);
			TType cosA = (TType)std::cos(in_angle);
			return GenericMatrix4<TType>(
				cosA, sinA, 0, 0, 
				-sinA, cosA, 0, 0, 
				0, 0, 1, 0, 
				0, 0, 0, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotation(const GenericQuaternion<TType>& in_rotation)
		{
			GenericQuaternion<TType> q = GenericQuaternion<TType>::Normalise(in_rotation);

			TType wSquared = q.w * q.w;
			TType xSquared = q.x * q.x;
			TType ySquared = q.y * q.y;
			TType zSquared = q.z * q.z;

			TType a = wSquared + xSquared - ySquared - zSquared;
			TType b = 2 * q.x * q.y + 2 * q.w * q.z;
			TType c = 2 * q.x * q.z - 2 * q.w * q.y;
			TType d = 0;
			TType e = 2 * q.x * q.y - 2 * q.w * q.z;
			TType f = wSquared - xSquared + ySquared - zSquared;
			TType g = 2 * q.y * q.z + 2 * q.w * q.x;
			TType h = 0;
			TType i = 2 * q.x * q.z + 2 * q.w * q.y;
			TType j = 2 * q.y * q.z - 2 * q.w * q.x;
			TType k = wSquared - xSquared - ySquared + zSquared;
			TType l = 0;
			TType m = 0;
			TType n = 0;
			TType o = 0;
			TType p = 1;

			return GenericMatrix4<TType>(
				a, b, c, d,
				e, f, g, h,
				i, j, k, l,
				m, n, o, p);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotation(const GenericVector3<TType>& in_axis, TType in_angle)
		{
			return CreateRotation(GenericQuaternion<TType>(in_axis, in_angle));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateTransform(const GenericVector3<TType>& in_translation, const GenericVector3<TType>& in_scale, const GenericQuaternion<TType>& in_rotation)
		{
			GenericMatrix4<TType> rotation = CreateRotation(in_rotation);

			return GenericMatrix4<TType>(
				in_scale.x * rotation.m[0], in_scale.x * rotation.m[1], in_scale.x * rotation.m[2], 0,
				in_scale.y * rotation.m[4], in_scale.y * rotation.m[5], in_scale.y * rotation.m[6], 0,
				in_scale.z * rotation.m[8], in_scale.z * rotation.m[9], in_scale.z * rotation.m[10], 0,
				in_translation.x, in_translation.y, in_translation.z, 1);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::Transpose(const GenericMatrix4<TType>& in_a)
		{
			return GenericMatrix4<TType>(
                in_a.m[0], in_a.m[4], in_a.m[8], in_a.m[12],
                in_a.m[1], in_a.m[5], in_a.m[9], in_a.m[13],
                in_a.m[2], in_a.m[6], in_a.m[10], in_a.m[14],
                in_a.m[3], in_a.m[7], in_a.m[11], in_a.m[15]);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::Inverse(const GenericMatrix4<TType>& in_a)
		{
			TType det = in_a.Determinant();
			if (det == 0)
			{
				return in_a;
			}
            
			GenericMatrix4<TType> b;
			b.m[0] = (in_a.m[6] * in_a.m[11] * in_a.m[13] - in_a.m[7] * in_a.m[10] * in_a.m[13] + in_a.m[7] * in_a.m[9] * in_a.m[14] - in_a.m[5] * in_a.m[11] * in_a.m[14] - in_a.m[6] * in_a.m[9] * in_a.m[15] + in_a.m[5] * in_a.m[10] * in_a.m[15]) / det;
			b.m[1] = (in_a.m[3] * in_a.m[10] * in_a.m[13] - in_a.m[2] * in_a.m[11] * in_a.m[13] - in_a.m[3] * in_a.m[9] * in_a.m[14] + in_a.m[1] * in_a.m[11] * in_a.m[14] + in_a.m[2] * in_a.m[9] * in_a.m[15] - in_a.m[1] * in_a.m[10] * in_a.m[15]) / det;
			b.m[2] = (in_a.m[2] * in_a.m[7] * in_a.m[13] - in_a.m[3] * in_a.m[6] * in_a.m[13] + in_a.m[3] * in_a.m[5] * in_a.m[14] - in_a.m[1] * in_a.m[7] * in_a.m[14] - in_a.m[2] * in_a.m[5] * in_a.m[15] + in_a.m[1] * in_a.m[6] * in_a.m[15]) / det;
			b.m[3] = (in_a.m[3] * in_a.m[6] * in_a.m[9] - in_a.m[2] * in_a.m[7] * in_a.m[9] - in_a.m[3] * in_a.m[5] * in_a.m[10] + in_a.m[1] * in_a.m[7] * in_a.m[10] + in_a.m[2] * in_a.m[5] * in_a.m[11] - in_a.m[1] * in_a.m[6] * in_a.m[11]) / det;
			b.m[4] = (in_a.m[7] * in_a.m[10] * in_a.m[12] - in_a.m[6] * in_a.m[11] * in_a.m[12] - in_a.m[7] * in_a.m[8] * in_a.m[14] + in_a.m[4] * in_a.m[11] * in_a.m[14] + in_a.m[6] * in_a.m[8] * in_a.m[15] - in_a.m[4] * in_a.m[10] * in_a.m[15]) / det;
			b.m[5] = (in_a.m[2] * in_a.m[11] * in_a.m[12] - in_a.m[3] * in_a.m[10] * in_a.m[12] + in_a.m[3] * in_a.m[8] * in_a.m[14] - in_a.m[0] * in_a.m[11] * in_a.m[14] - in_a.m[2] * in_a.m[8] * in_a.m[15] + in_a.m[0] * in_a.m[10] * in_a.m[15]) / det;
			b.m[6] = (in_a.m[3] * in_a.m[6] * in_a.m[12] - in_a.m[2] * in_a.m[7] * in_a.m[12] - in_a.m[3] * in_a.m[4] * in_a.m[14] + in_a.m[0] * in_a.m[7] * in_a.m[14] + in_a.m[2] * in_a.m[4] * in_a.m[15] - in_a.m[0] * in_a.m[6] * in_a.m[15]) / det;
			b.m[7] = (in_a.m[2] * in_a.m[7] * in_a.m[8] - in_a.m[3] * in_a.m[6] * in_a.m[8] + in_a.m[3] * in_a.m[4] * in_a.m[10] - in_a.m[0] * in_a.m[7] * in_a.m[10] - in_a.m[2] * in_a.m[4] * in_a.m[11] + in_a.m[0] * in_a.m[6] * in_a.m[11]) / det;
			b.m[8] = (in_a.m[5] * in_a.m[11] * in_a.m[12] - in_a.m[7] * in_a.m[9] * in_a.m[12] + in_a.m[7] * in_a.m[8] * in_a.m[13] - in_a.m[4] * in_a.m[11] * in_a.m[13] - in_a.m[5] * in_a.m[8] * in_a.m[15] + in_a.m[4] * in_a.m[9] * in_a.m[15]) / det;
			b.m[9] = (in_a.m[3] * in_a.m[9] * in_a.m[12] - in_a.m[1] * in_a.m[11] * in_a.m[12] - in_a.m[3] * in_a.m[8] * in_a.m[13] + in_a.m[0] * in_a.m[11] * in_a.m[13] + in_a.m[1] * in_a.m[8] * in_a.m[15] - in_a.m[0] * in_a.m[9] * in_a.m[15]) / det;
			b.m[10] = (in_a.m[1] * in_a.m[7] * in_a.m[12] - in_a.m[3] * in_a.m[5] * in_a.m[12] + in_a.m[3] * in_a.m[4] * in_a.m[13] - in_a.m[0] * in_a.m[7] * in_a.m[13] - in_a.m[1] * in_a.m[4] * in_a.m[15] + in_a.m[0] * in_a.m[5] * in_a.m[15]) / det;
			b.m[11] = (in_a.m[3] * in_a.m[5] * in_a.m[8] - in_a.m[1] * in_a.m[7] * in_a.m[8] - in_a.m[3] * in_a.m[4] * in_a.m[9] + in_a.m[0] * in_a.m[7] * in_a.m[9] + in_a.m[1] * in_a.m[4] * in_a.m[11] - in_a.m[0] * in_a.m[5] * in_a.m[11]) / det;
			b.m[12] = (in_a.m[6] * in_a.m[9] * in_a.m[12] - in_a.m[5] * in_a.m[10] * in_a.m[12] - in_a.m[6] * in_a.m[8] * in_a.m[13] + in_a.m[4] * in_a.m[10] * in_a.m[13] + in_a.m[5] * in_a.m[8] * in_a.m[14] - in_a.m[4] * in_a.m[9] * in_a.m[14]) / det;
			b.m[13] = (in_a.m[1] * in_a.m[10] * in_a.m[12] - in_a.m[2] * in_a.m[9] * in_a.m[12] + in_a.m[2] * in_a.m[8] * in_a.m[13] - in_a.m[0] * in_a.m[10] * in_a.m[13] - in_a.m[1] * in_a.m[8] * in_a.m[14] + in_a.m[0] * in_a.m[9] * in_a.m[14]) / det;
			b.m[14] = (in_a.m[2] * in_a.m[5] * in_a.m[12] - in_a.m[1] * in_a.m[6] * in_a.m[12] - in_a.m[2] * in_a.m[4] * in_a.m[13] + in_a.m[0] * in_a.m[6] * in_a.m[13] + in_a.m[1] * in_a.m[4] * in_a.m[14] - in_a.m[0] * in_a.m[5] * in_a.m[14]) / det;
			b.m[15] = (in_a.m[1] * in_a.m[6] * in_a.m[8] - in_a.m[2] * in_a.m[5] * in_a.m[8] + in_a.m[2] * in_a.m[4] * in_a.m[9] - in_a.m[0] * in_a.m[6] * in_a.m[9] - in_a.m[1] * in_a.m[4] * in_a.m[10] + in_a.m[0] * in_a.m[5] * in_a.m[10]) / det;
			return b;
		}
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::Translate(const GenericMatrix4<TType>& in_a, const GenericVector3<TType>& in_translation)
        {
            return in_a * CreateTranslation(in_translation);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::Translate(const GenericMatrix4<TType>& in_a, TType in_x, TType in_y, TType in_z)
        {
            return Translate(in_a, GenericVector3<TType>(in_x, in_y, in_z));
        }
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::Scale(const GenericMatrix4<TType>& in_a, const GenericVector3<TType>& in_scale)
        {
            return in_a * CreateScale(in_scale);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::Scale(const GenericMatrix4<TType>& in_a, TType in_x, TType in_y, TType in_z)
        {
            return Scale(in_a, GenericVector3<TType>(in_x, in_y, in_z));
        }
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::RotateX(const GenericMatrix4<TType>& in_a, TType in_angle)
        {
            return in_a * CreateRotationX(in_angle);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::RotateY(const GenericMatrix4<TType>& in_a, TType in_angle)
        {
            return in_a * CreateRotationY(in_angle);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::RotateZ(const GenericMatrix4<TType>& in_a, TType in_angle)
        {
            return in_a * CreateRotationZ(in_angle);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::Rotate(const GenericMatrix4<TType>& in_a, const GenericQuaternion<TType>& in_orientation)
        {
            return in_a * CreateRotation(in_orientation);
        }
        //------------------------------------------------------
        //------------------------------------------------------
        template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::Rotate(const GenericMatrix4<TType>& in_a, const GenericVector3<TType>& in_axis, TType in_angle)
        {
            return Rotate(in_a, GenericQuaternion<TType>(in_axis, in_angle));
        }
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
		template <typename TType> void GenericMatrix4<TType>::Identity()
		{
			m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
			m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
			m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
			m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Transpose()
		{
			GenericMatrix4<TType> a = *this;
			
			m[0] = a.m[0]; m[1] = a.m[4]; m[2] = a.m[8]; m[3] = a.m[12];
			m[4] = a.m[1]; m[5] = a.m[5]; m[6] = a.m[9]; m[7] = a.m[13];
			m[8] = a.m[2]; m[9] = a.m[6]; m[10] = a.m[10]; m[11] = a.m[14];
			m[12] = a.m[3]; m[13] = a.m[7]; m[14] = a.m[11]; m[15] = a.m[15];
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Inverse()
		{
			*this = Inverse(*this);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Translate(const GenericVector3<TType>& in_translation)
		{
			*this *= CreateTranslation(in_translation);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Translate(TType in_x, TType in_y, TType in_z)
		{
			Translate(GenericVector3<TType>(in_x, in_y, in_z));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Scale(const GenericVector3<TType>& in_scale)
		{
			*this *= CreateScale(in_scale);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Scale(TType in_x, TType in_y, TType in_z)
		{
			Scale(GenericVector3<TType>(in_x, in_y, in_z));
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
		template <typename TType> void GenericMatrix4<TType>::Rotate(const GenericQuaternion<TType>& in_orientation)
		{
			*this *= CreateRotation(in_orientation);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Rotate(const GenericVector3<TType>& in_axis, TType in_angle)
		{
			Rotate(GenericQuaternion<TType>(in_axis, in_angle));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericMatrix4<TType>::GetRight() const
		{
			return GenericVector3<TType>(m[0], m[1], m[2]);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericMatrix4<TType>::GetUp() const
		{
			return GenericVector3<TType>(m[4], m[5], m[6]);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericMatrix4<TType>::GetForward() const
		{
			return GenericVector3<TType>(m[8], m[9], m[10]);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector3<TType> GenericMatrix4<TType>::GetTranslation() const
		{
			return GenericVector3<TType>(m[12], m[13], m[14]);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix4<TType>::Decompose(GenericVector3<TType>& out_translation, GenericVector3<TType> & out_scale, GenericQuaternion<TType> & out_orientation) const
		{
			out_translation.x = m[12];
			out_translation.y = m[13];
			out_translation.z = m[14];

			GenericMatrix4<TType> p;

			// build orthogonal matrix p
			TType invLength = 1 / std::sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
			p.m[0] = m[0] * invLength;
			p.m[1] = m[1] * invLength;
			p.m[2] = m[2] * invLength;

			TType dot = p.m[0] * m[4] + p.m[1] * m[5] + p.m[2] * m[6];
			p.m[4] = m[4] - dot * p.m[0];
			p.m[5] = m[5] - dot * p.m[1];
			p.m[6] = m[6] - dot * p.m[2];
			invLength = 1 / std::sqrt(p.m[4] * p.m[4] + p.m[5] * p.m[5] + p.m[6] * p.m[6]);
			p.m[4] *= invLength;
			p.m[5] *= invLength;
			p.m[6] *= invLength;

			dot = p.m[0] * m[8] + p.m[1] * m[9] + p.m[2] * m[10];
			p.m[8] = m[8] - dot * p.m[0];
			p.m[9] = m[9] - dot * p.m[1];
			p.m[10] = m[10] - dot * p.m[2];
			dot = p.m[4] * m[8] + p.m[5] * m[9] + p.m[6] * m[10];
			p.m[8] -= dot * p.m[4];
			p.m[9] -= dot * p.m[5];
			p.m[10] -= dot * p.m[6];
			invLength = 1 / std::sqrt(p.m[8] * p.m[8] + p.m[9] * p.m[9] + p.m[10] * p.m[10]);
			p.m[8] *= invLength;
			p.m[9] *= invLength;
			p.m[10] *= invLength;

			// guarantee that orthogonal matrix has determinant 1 (no reflections)
			TType det = p.m[0] * p.m[5] * p.m[10] + p.m[4] * p.m[9] * p.m[2] +
				p.m[8] * p.m[1] * p.m[6] - p.m[8] * p.m[5] * p.m[2] -
				p.m[4] * p.m[1] * p.m[10] - p.m[0] * p.m[9] * p.m[6];

			if (det < 0)
			{
				for (u32 i = 0; i < 16; i++)
				{
					p.m[i] = -p.m[i];
				}
			}

			// build "right" matrix R
			Matrix4 r;
			r.m[0] = p.m[0] * m[0] + p.m[1] * m[1] + p.m[2] * m[2];
			r.m[4] = p.m[0] * m[4] + p.m[1] * m[5] + p.m[2] * m[6];
			r.m[5] = p.m[4] * m[4] + p.m[5] * m[5] + p.m[6] * m[6];
			r.m[8] = p.m[0] * m[8] + p.m[1] * m[9] + p.m[2] * m[10];
			r.m[9] = p.m[4] * m[8] + p.m[5] * m[9] + p.m[6] * m[10];
			r.m[10] = p.m[8] * m[8] + p.m[9] * m[9] + p.m[10] * m[10];

			// the scaling component
			out_scale.x = r.m[0];
			out_scale.y = r.m[5];
			out_scale.z = r.m[10];

			out_orientation = GenericQuaternion<TType>(p);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType& GenericMatrix4<TType>::operator()(u32 in_row, u32 in_column)
		{
			CS_ASSERT(in_row >= 0 && in_row < 4 && in_column >= 0 && in_column < 4, "Trying to access matrix value at [" + ToString(in_row) + ", " + ToString(in_column) + "]");
			return m[in_column + in_row * 4];
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericMatrix4<TType>::operator()(u32 in_row, u32 in_column) const
		{
			CS_ASSERT(in_row >= 0 && in_row < 4 && in_column >= 0 && in_column < 4, "Trying to access matrix value at [" + ToString(in_row) + ", " + ToString(in_column) + "]");
			return m[in_column + in_row * 4];
		}
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
			m[0] -= in_b.m[0]; m[1] -= in_b.m[1]; m[2] -= in_b.m[2]; m[3] -= in_b.m[3];
			m[4] -= in_b.m[4]; m[5] -= in_b.m[5]; m[6] -= in_b.m[6]; m[7] -= in_b.m[7];
			m[8] -= in_b.m[8]; m[9] -= in_b.m[9]; m[10] -= in_b.m[10]; m[11] -= in_b.m[11];
			m[12] -= in_b.m[12]; m[13] -= in_b.m[13]; m[14] -= in_b.m[14]; m[15] -= in_b.m[15];
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator*=(const GenericMatrix4<TType>& in_b)
		{
			GenericMatrix4<TType> c = *this;

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
		template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator/=(TType in_b)
		{
			m[0] /= in_b; m[1] /= in_b; m[2] /= in_b; m[3] /= in_b;
			m[4] /= in_b; m[5] /= in_b; m[6] /= in_b; m[7] /= in_b;
			m[8] /= in_b; m[9] /= in_b; m[10] /= in_b; m[11] /= in_b;
			m[12] /= in_b; m[13] /= in_b; m[14] /= in_b; m[15] /= in_b;
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
		template <typename TType> GenericMatrix4<TType> operator/(GenericMatrix4<TType> in_a, TType in_b)
		{
			return (in_a /= in_b);
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