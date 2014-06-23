//
//  Matrix3.h
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

#ifndef _CHILLISOURCE_CORE_MATH_MATRIX3_H_
#define _CHILLISOURCE_CORE_MATH_MATRIX3_H_

#include <ChilliSource/ChilliSource.h>

namespace ChilliSource
{
	namespace Core
	{
		//---------------------------------------------------------------
		/// A generic 3x3 matrix that provides standard matrix math
		/// functionality. The matrix is represented internally in
		/// row major format. Vector multiplication and transformation
		/// matrices are described using row vector format.Typically this 
		/// would not be used directly instead the f32 typedef Matrix3 
		/// should be used.
		///
		/// @author Ian Copland
		//---------------------------------------------------------------
		template <typename TType> class GenericMatrix3 final
		{
		public:
			//-----------------------------------------------------
			/// Constants
			//-----------------------------------------------------
			static const GenericMatrix3<TType> k_identity;
			//------------------------------------------------------
			/// Creates a translation matrix.
			///
			/// @author Ian Copland
			///
			/// @param the translation.
			///
			/// @return The new translation matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateTranslation(const GenericVector2<TType>& in_translation);
            //------------------------------------------------------
			/// Creates a translation matrix.
			///
			/// @author Ian Copland
			///
			/// @param the x translation.
            /// @param the y translation.
			///
			/// @return The new translation matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateTranslation(TType in_x, TType in_y);
			//------------------------------------------------------
			/// Creates a scale matrix.
			///
			/// @author Ian Copland
			///
			/// @param the scale.
			///
			/// @return The new scale matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateScale(const GenericVector2<TType>& in_scale);
            //------------------------------------------------------
			/// Creates a scale matrix.
			///
			/// @author Ian Copland
			///
			/// @param the x scale.
			/// @param the y scale.
			///
			/// @return The new scale matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateScale(TType in_x, TType in_y);
			//------------------------------------------------------
			/// Creates a rotation matrix.
			///
			/// @author Ian Copland
			///
			/// @param the rotation.
			///
			/// @return The new rotation matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateRotation(TType in_angle);
			//------------------------------------------------------
			/// Creates a transform matrix, built out of a translation
			/// scale and rotation.
			///
			/// @author S Downie
			///
			/// @param The translation.
			/// @param The scale.
			/// @param The rotation angle.
			///
			/// @return The new transform matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateTransform(const GenericVector2<TType>& in_translation, const GenericVector2<TType>& in_scale, TType in_angle);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			///
			/// @return A transposed copy of the matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> Transpose(const GenericMatrix3<TType>& in_matrix);
			//------------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
			///
			/// @return An inverted copy of the matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> Inverse(const GenericMatrix3<TType>& in_matrix);
            //-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
            /// @param A translation vector.
			///
			/// @return A copy of the matrix with the given translation
            /// applied to it.
			//-----------------------------------------------------
			static GenericMatrix3<TType> Translate(const GenericMatrix3<TType>& in_matrix, const GenericVector2<TType>& in_translation);
            //-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
            /// @param The x translation.
            /// @param The y translation.
			///
			/// @return A copy of the matrix with the given translation
            /// applied to it.
			//-----------------------------------------------------
			static GenericMatrix3<TType> Translate(const GenericMatrix3<TType>& in_matrix, TType in_x, TType in_y);
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
            /// @param A scale vector.
			///
			/// @return A copy of the matrix with the given scale
            /// applied to it.
			//-----------------------------------------------------
			static GenericMatrix3<TType> Scale(const GenericMatrix3<TType>& in_matrix, const GenericVector2<TType>& in_scale);
            //-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
            /// @param The x scale.
            /// @param The y scale.
			///
			/// @return A copy of the matrix with the given scale
            /// applied to it.
			//-----------------------------------------------------
			static GenericMatrix3<TType> Scale(const GenericMatrix3<TType>& in_matrix, TType in_x, TType in_y);
			//-----------------------------------------------------
			/// @author Ian Copland
            ///
            /// @param A matrix.
            /// @param A rotation.
			///
			/// @return A copy of the matrix with the given rotation
            /// applied to it.
			//-----------------------------------------------------
			static GenericMatrix3<TType> Rotate(const GenericMatrix3<TType>& in_matrix, TType in_angle);
			//-----------------------------------------------------
			/// Constructor. Sets the contents to the identity
			/// matrix.
			///
			/// @author Ian Copland.
			//-----------------------------------------------------
			GenericMatrix3();
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author Ian Copland
			///
			/// @param The matrix elements in row major format.
			//-----------------------------------------------------
			GenericMatrix3(TType in_a0, TType in_a1, TType in_a2, TType in_b0, TType in_b1, TType in_b2, TType in_c0, TType in_c1, TType in_c2);
			//------------------------------------------------------
			/// @author Ian Copland
			///
			/// @return The determinant of this matrix.
			//------------------------------------------------------
			TType Determinant() const;
            //------------------------------------------------------
			/// @author S Dowie
			///
			/// @param The translation part of the transform.
			//------------------------------------------------------
			GenericVector2<TType> GetTranslation() const;
			//-----------------------------------------------------
			/// Sets this matrix to the identity matrix.
			///
			/// @author Ian Copland
			//-----------------------------------------------------
			void Identity();
			//-----------------------------------------------------
			/// Sets this matrix to its transpose.
			///
			/// @author Ian Copland
			//-----------------------------------------------------
			void Transpose();
			//------------------------------------------------------
			/// Sets this matrix to its inverse.
			///
			/// @author Ian Copland
			//------------------------------------------------------
			void Inverse();
			//-----------------------------------------------------
			/// Translates this matrix.
			///
			/// @author Ian Copland
			///
			/// @param the translation vector.
			//-----------------------------------------------------
			void Translate(const GenericVector2<TType>& in_translation);
            //-----------------------------------------------------
			/// Translates this matrix.
			///
			/// @author Ian Copland
			///
			/// @param the x translation.
			/// @param the y translation.
			//-----------------------------------------------------
			void Translate(TType in_x, TType in_y);
			//-----------------------------------------------------
			/// Scales this matrix.
			///
			/// @author Ian Copland
			///
			/// @param the scale vector.
			//-----------------------------------------------------
			void Scale(const GenericVector2<TType>& in_scale);
            //-----------------------------------------------------
			/// Scales this matrix.
			///
			/// @author Ian Copland
			///
			/// @param the x scale.
			/// @param the y scale.
			//-----------------------------------------------------
			void Scale(TType in_x, TType in_y);
			//-----------------------------------------------------
			/// Rotates the matrix.
			///
			/// @author Ian Copland
			///
			/// @param the rotation.
			//-----------------------------------------------------
			void Rotate(TType in_angle);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param The row index.
			/// @param The column index.
			///
			/// @return The value at the given position in the
			/// matrix. Matrices are row major so this equates to
			/// the value at position in_row * 3 + in_column.
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
			/// the value at position in_row * 3 + in_column.
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
			GenericMatrix3<TType>& operator+=(const GenericMatrix3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after subtracting the given matrix
			/// component-wise.
			//-----------------------------------------------------
			GenericMatrix3<TType>& operator-=(const GenericMatrix3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after multiplying by the given
			/// matrix.
			//-----------------------------------------------------
			GenericMatrix3<TType>& operator*=(const GenericMatrix3<TType>& in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar
			///
			/// @return This matrix after multiplying by the given 
			/// scalar.
			//-----------------------------------------------------
			GenericMatrix3<TType>& operator*=(TType in_b);
			//-----------------------------------------------------
			/// @author Ian Copland
			///
			/// @param A scalar
			///
			/// @return This matrix after dividing by the given 
			/// scalar.
			//-----------------------------------------------------
			GenericMatrix3<TType>& operator/=(TType in_b);

			TType m[9];
		};
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Matrix A
		/// @param Matrix B
		///
		/// @return The result of the component-wise addition of
		/// A and B.
		//-----------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator+(GenericMatrix3<TType> in_a, const GenericMatrix3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Matrix A
		/// @param Matrix B
		///
		/// @return The result of the component-wise subtraction 
		/// of B from A.
		//-----------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator-(GenericMatrix3<TType> in_a, const GenericMatrix3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Matrix A
		/// @param Matrix B
		///
		/// @return The result of A * B
		//-----------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator*(const GenericMatrix3<TType>& in_a, const GenericMatrix3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Matrix A
		/// @param Scalar B
		///
		/// @return The result of A * B
		//-----------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator*(GenericMatrix3<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Scalar A
		/// @param Matrix B
		///
		/// @return The result of A * B
		//-----------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator*(TType in_a, GenericMatrix3<TType> in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Matrix A
		/// @param Scalar B
		///
		/// @return The result of A / B
		//-----------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator/(GenericMatrix3<TType> in_a, TType in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Matrix A
		/// @param Matrix B
		///
		/// @return Whether or not the matrices are equal.
		//-----------------------------------------------------
		template <typename TType> bool operator==(const GenericMatrix3<TType>& in_a, const GenericMatrix3<TType>& in_b);
		//-----------------------------------------------------
		/// @author Ian Copland
		///
		/// @param Matrix A
		/// @param Matrix B
		///
		/// @return Whether or not the matrices are in-equal.
		//-----------------------------------------------------
		template <typename TType> bool operator!=(const GenericMatrix3<TType>& in_a, const GenericMatrix3<TType>& in_b);
	}
}

//----------------------------------------------------
// These are included here to avoid circular inclusion
// issues. At this stage the class has been defined
// which is enough for the classes included to use it.
//----------------------------------------------------
#include <ChilliSource/Core/Math/Vector2.h>

#include <cmath>

namespace ChilliSource
{
	namespace Core
	{
		template <typename TType> const GenericMatrix3<TType> GenericMatrix3<TType>::k_identity(1, 0, 0, 0, 1, 0, 0, 0, 1);

		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateTranslation(const GenericVector2<TType>& in_translation)
		{
			return GenericMatrix3<TType>(1, 0, 0, 0, 1, 0, in_translation.x, in_translation.y, 1);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateTranslation(TType in_x, TType in_y)
		{
			return CreateTranslation(GenericVector2<TType>(in_x, in_y));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateScale(const GenericVector2<TType>& in_scale)
		{
			return GenericMatrix3<TType>(in_scale.x, 0, 0, 0, in_scale.y, 0, 0, 0, 1);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateScale(TType in_x, TType in_y)
		{
			return CreateScale(GenericVector2<TType>(in_x, in_y));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateRotation(TType in_angle)
		{
			TType sinA = (TType)std::sin(in_angle);
			TType cosA = (TType)std::cos(in_angle);
			return GenericMatrix3<TType>(cosA, sinA, 0, -sinA, cosA, 0, 0, 0, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateTransform(const GenericVector2<TType>& in_translation, const GenericVector2<TType>& in_scale, TType in_angle)
		{
			TType cosA = (TType)std::cos(in_angle);
			TType sinA = (TType)std::sin(in_angle);
			return GenericMatrix3<TType>(cosA * in_scale.x, sinA * in_scale.x, 0, -sinA * in_scale.y, cosA * in_scale.y, 0, in_translation.x, in_translation.y, 1);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::Transpose(const GenericMatrix3<TType>& in_a)
		{
			GenericMatrix3<TType> b;
			b.m[0] = in_a.m[0]; b.m[1] = in_a.m[3]; b.m[2] = in_a.m[6];
			b.m[3] = in_a.m[1]; b.m[4] = in_a.m[4]; b.m[5] = in_a.m[7];
			b.m[6] = in_a.m[2]; b.m[7] = in_a.m[5]; b.m[8] = in_a.m[8];
			return b;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::Inverse(const GenericMatrix3<TType>& in_a)
		{
			TType det = in_a.Determinant();
			if (det == 0)
			{
				return in_a;
			}
			TType oneOverDet = 1 / det;
            
			GenericMatrix3<TType> c;
			c.m[0] = (in_a.m[4] * in_a.m[8] - in_a.m[5] * in_a.m[7]) * oneOverDet;
			c.m[1] = (in_a.m[2] * in_a.m[7] - in_a.m[1] * in_a.m[8]) * oneOverDet;
			c.m[2] = (in_a.m[1] * in_a.m[5] - in_a.m[2] * in_a.m[4]) * oneOverDet;
			c.m[3] = (in_a.m[5] * in_a.m[6] - in_a.m[3] * in_a.m[8]) * oneOverDet;
			c.m[4] = (in_a.m[0] * in_a.m[8] - in_a.m[2] * in_a.m[6]) * oneOverDet;
			c.m[5] = (in_a.m[2] * in_a.m[3] - in_a.m[0] * in_a.m[5]) * oneOverDet;
			c.m[6] = (in_a.m[3] * in_a.m[7] - in_a.m[4] * in_a.m[6]) * oneOverDet;
			c.m[7] = (in_a.m[1] * in_a.m[6] - in_a.m[0] * in_a.m[7]) * oneOverDet;
			c.m[8] = (in_a.m[0] * in_a.m[4] - in_a.m[1] * in_a.m[3]) * oneOverDet;
			return c;
		}
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::Translate(const GenericMatrix3<TType>& in_matrix, const GenericVector2<TType>& in_translation)
        {
            return in_matrix * CreateTranslation(in_translation);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::Translate(const GenericMatrix3<TType>& in_matrix, TType in_x, TType in_y)
        {
            return Translate(in_matrix, GenericVector2<TType>(in_x, in_y));
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::Scale(const GenericMatrix3<TType>& in_matrix, const GenericVector2<TType>& in_scale)
        {
            return in_matrix * CreateScale(in_scale);
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::Scale(const GenericMatrix3<TType>& in_matrix, TType in_x, TType in_y)
        {
            return Scale(in_matrix, GenericVector2<TType>(in_x, in_y));
        }
        //-----------------------------------------------------
        //-----------------------------------------------------
        template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::Rotate(const GenericMatrix3<TType>& in_matrix, TType in_angle)
        {
            return in_matrix * CreateRotation(in_angle);
        }
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType>::GenericMatrix3()
		{
			Identity();
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType>::GenericMatrix3(TType in_a0, TType in_a1, TType in_a2, TType in_b0, TType in_b1, TType in_b2, TType in_c0, TType in_c1, TType in_c2)
		{
			m[0] = in_a0; m[1] = in_a1; m[2] = in_a2;
			m[3] = in_b0; m[4] = in_b1; m[5] = in_b2;
			m[6] = in_c0; m[7] = in_c1; m[8] = in_c2;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> TType GenericMatrix3<TType>::Determinant() const
		{
			return m[0] * (m[4] * m[8] - m[5] * m[7]) - m[1] * (m[3] * m[8] - m[5] * m[6]) + m[2] * (m[3] * m[7] - m[4] * m[6]);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Identity()
		{
			m[0] = 1; m[1] = 0; m[2] = 0;
			m[3] = 0; m[4] = 1; m[5] = 0;
			m[6] = 0; m[7] = 0; m[8] = 1;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Transpose()
		{
			GenericMatrix3<TType> a = *this;
			m[0] = a.m[0]; m[1] = a.m[3]; m[2] = a.m[6];
			m[3] = a.m[1]; m[4] = a.m[4]; m[5] = a.m[7];
			m[6] = a.m[2]; m[7] = a.m[5]; m[8] = a.m[8];
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Inverse()
		{
			*this = Inverse(*this);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Translate(const GenericVector2<TType>& in_translation)
		{
			*this *= CreateTranslation(in_translation);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Translate(TType in_x, TType in_y)
		{
			Translate(GenericVector2<TType>(in_x, in_y));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Scale(const GenericVector2<TType>& in_scale)
		{
			*this *= CreateScale(in_scale);
		}
        //------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Scale(TType in_x, TType in_y)
		{
			Scale(GenericVector2<TType>(in_x, in_y));
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Rotate(TType in_angle)
		{
			*this *= CreateRotation(in_angle);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector2<TType> GenericMatrix3<TType>::GetTranslation() const
		{
			return GenericVector2<TType>(m[6], m[7]);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType& GenericMatrix3<TType>::operator()(u32 in_row, u32 in_column)
		{
			CS_ASSERT(in_row >= 0 && in_row < 3 && in_column >= 0 && in_column < 3, "Trying to access matrix value at [" + ToString(in_row) + ", " + ToString(in_column) + "]");
			return m[in_column + in_row * 3];
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType GenericMatrix3<TType>::operator()(u32 in_row, u32 in_column) const
		{
			CS_ASSERT(in_row >= 0 && in_row < 3 && in_column >= 0 && in_column < 3, "Trying to access matrix value at [" + ToString(in_row) + ", " + ToString(in_column) + "]");
			return m[in_column + in_row * 3];
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator+=(const GenericMatrix3<TType>& in_b)
		{
			m[0] += in_b.m[0]; m[1] += in_b.m[1]; m[2] += in_b.m[2];
			m[3] += in_b.m[3]; m[4] += in_b.m[4]; m[5] += in_b.m[5];
			m[6] += in_b.m[6]; m[7] += in_b.m[7]; m[8] += in_b.m[8];
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator-=(const GenericMatrix3<TType>& in_b)
		{
			m[0] -= in_b.m[0]; m[1] -= in_b.m[1]; m[2] -= in_b.m[2];
			m[3] -= in_b.m[3]; m[4] -= in_b.m[4]; m[5] -= in_b.m[5];
			m[6] -= in_b.m[6]; m[7] -= in_b.m[7]; m[8] -= in_b.m[8];
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator*=(const GenericMatrix3<TType>& in_b)
		{
			Matrix3 c = *this;

			m[0] = c.m[0] * in_b.m[0] + c.m[1] * in_b.m[3] + c.m[2] * in_b.m[6];
			m[1] = c.m[0] * in_b.m[1] + c.m[1] * in_b.m[4] + c.m[2] * in_b.m[7];
			m[2] = c.m[0] * in_b.m[2] + c.m[1] * in_b.m[5] + c.m[2] * in_b.m[8];

			m[3] = c.m[3] * in_b.m[0] + c.m[4] * in_b.m[3] + c.m[5] * in_b.m[6];
			m[4] = c.m[3] * in_b.m[1] + c.m[4] * in_b.m[4] + c.m[5] * in_b.m[7];
			m[5] = c.m[3] * in_b.m[2] + c.m[4] * in_b.m[5] + c.m[5] * in_b.m[8];

			m[6] = c.m[6] * in_b.m[0] + c.m[7] * in_b.m[3] + c.m[8] * in_b.m[6];
			m[7] = c.m[6] * in_b.m[1] + c.m[7] * in_b.m[4] + c.m[8] * in_b.m[7];
			m[8] = c.m[6] * in_b.m[2] + c.m[7] * in_b.m[5] + c.m[8] * in_b.m[8];

			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator*=(TType in_b)
		{
			m[0] *= in_b; m[1] *= in_b; m[2] *= in_b;
			m[3] *= in_b; m[4] *= in_b; m[5] *= in_b;
			m[6] *= in_b; m[7] *= in_b; m[8] *= in_b;
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator/=(TType in_b)
		{
			m[0] /= in_b; m[1] /= in_b; m[2] /= in_b;
			m[3] /= in_b; m[4] /= in_b; m[5] /= in_b;
			m[6] /= in_b; m[7] /= in_b; m[8] /= in_b;
			return *this;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator+(GenericMatrix3<TType> in_a, const GenericMatrix3<TType>& in_b)
		{
			return (in_a += in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator-(GenericMatrix3<TType> in_a, const GenericMatrix3<TType>& in_b)
		{
			return (in_a -= in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator*(const GenericMatrix3<TType>& in_a, const GenericMatrix3<TType>& in_b)
		{
			Matrix3 c;

			c.m[0] = in_a.m[0] * in_b.m[0] + in_a.m[1] * in_b.m[3] + in_a.m[2] * in_b.m[6];
			c.m[1] = in_a.m[0] * in_b.m[1] + in_a.m[1] * in_b.m[4] + in_a.m[2] * in_b.m[7];
			c.m[2] = in_a.m[0] * in_b.m[2] + in_a.m[1] * in_b.m[5] + in_a.m[2] * in_b.m[8];

			c.m[3] = in_a.m[3] * in_b.m[0] + in_a.m[4] * in_b.m[3] + in_a.m[5] * in_b.m[6];
			c.m[4] = in_a.m[3] * in_b.m[1] + in_a.m[4] * in_b.m[4] + in_a.m[5] * in_b.m[7];
			c.m[5] = in_a.m[3] * in_b.m[2] + in_a.m[4] * in_b.m[5] + in_a.m[5] * in_b.m[8];

			c.m[6] = in_a.m[6] * in_b.m[0] + in_a.m[7] * in_b.m[3] + in_a.m[8] * in_b.m[6];
			c.m[7] = in_a.m[6] * in_b.m[1] + in_a.m[7] * in_b.m[4] + in_a.m[8] * in_b.m[7];
			c.m[8] = in_a.m[6] * in_b.m[2] + in_a.m[7] * in_b.m[5] + in_a.m[8] * in_b.m[8];

			return c;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator*(GenericMatrix3<TType> in_a, TType in_b)
		{
			return (in_a *= in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator*(TType in_a, GenericMatrix3<TType> in_b)
		{
			return (in_b *= in_a);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> operator/(GenericMatrix3<TType> in_a, TType in_b)
		{
			return (in_a /= in_b);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> bool operator==(const GenericMatrix3<TType>& in_a, const GenericMatrix3<TType>& in_b)
		{
			for (u32 i = 0; i < 9; ++i)
			{
				if (in_a.m[i] != in_b.m[i])
					return false;
			}
			return true;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> bool operator!=(const GenericMatrix3<TType>& in_a, const GenericMatrix3<TType>& in_b)
		{
			return !(in_a == in_b);
		}
	}
}

#endif