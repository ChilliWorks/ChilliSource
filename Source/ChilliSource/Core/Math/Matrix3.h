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
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
#include <Accelerate/Accelerate.h>
#endif

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
		/// @author I Copland
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
			/// @author I Copland
			///
			/// @param the rotation.
			///
			/// @return The new translation matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateTranslation(const GenericVector2<TType>& in_translation);
			//------------------------------------------------------
			/// Creates a scale matrix.
			///
			/// @author I Copland
			///
			/// @param the scale.
			///
			/// @return The new scale matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateScale(const GenericVector2<TType>& in_scale);
			//------------------------------------------------------
			/// Creates a rotation matrix.
			///
			/// @author I Copland
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
			/// @author I Copland
			///
			/// @param The translation.
			/// @param The scale.
			/// @param The rotation angle.
			///
			/// @return The new transform matrix.
			//------------------------------------------------------
			static GenericMatrix3<TType> CreateTransform(const GenericVector2<TType>& in_translation, const GenericVector2<TType>& in_scale, TType in_angle);
			//-----------------------------------------------------
			/// Constructor. Sets the contents to the identity
			/// matrix.
			///
			/// @author I Copland.
			//-----------------------------------------------------
			GenericMatrix3();
			//-----------------------------------------------------
			/// Constructor
			///
			/// @author I Copland
			///
			/// @param The matrix elements in row major format.
			//-----------------------------------------------------
			GenericMatrix3(TType in_a0, TType in_a1, TType in_a2, TType in_b0, TType in_b1, TType in_b2, TType in_c0, TType in_c1, TType in_c2);
			//------------------------------------------------------
			/// @author I Copland
			///
			/// @return The determinant of this matrix.
			//------------------------------------------------------
			TType Determinant() const;
			//-----------------------------------------------------
			/// Sets this matrix to the identity matrix.
			///
			/// @author I Copland
			//-----------------------------------------------------
			void Identity();
			//-----------------------------------------------------
			/// Sets this matrix to its transpose.
			///
			/// @author I Copland
			//-----------------------------------------------------
			void Transpose();
			//------------------------------------------------------
			/// @author I Copland
			///
			/// @return A transposed copy of this matrix.
			//------------------------------------------------------
			GenericMatrix3<TType> TransposeCopy() const;
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
			GenericMatrix3<TType> InverseCopy() const;
			//-----------------------------------------------------
			/// Translates this matrix.
			///
			/// @author I Copland
			///
			/// @param the translation vector.
			//-----------------------------------------------------
			void Translate(const GenericVector2<TType>& in_translation);
			//-----------------------------------------------------
			/// Scales this matrix.
			///
			/// @author I Copland
			///
			/// @param the scale vector.
			//-----------------------------------------------------
			void Scale(const GenericVector2<TType>& in_scale);
			//-----------------------------------------------------
			/// Rotates the matrix.
			///
			/// @author I Copland
			///
			/// @param the rotation.
			//-----------------------------------------------------
			void Rotate(TType in_angle);
			//-----------------------------------------------------
			/// @author I Copland
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
			/// @author I Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after adding the given matrix
			/// component-wise.
			//-----------------------------------------------------
			GenericMatrix3<TType>& operator+=(const GenericMatrix3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after subtracting the given matrix
			/// component-wise.
			//-----------------------------------------------------
			GenericMatrix3<TType>& operator-=(const GenericMatrix3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after multiplying by the given
			/// matrix.
			//-----------------------------------------------------
			GenericMatrix3<TType>& operator*=(const GenericMatrix3<TType>& in_b);
			//-----------------------------------------------------
			/// @author I Copland
			///
			/// @param Another matrix.
			///
			/// @return This matrix after multiplying by the given 
			/// scalar.
			//-----------------------------------------------------
			GenericMatrix3<TType>& operator*=(TType in_b);

			TType m[9];
		};

		template <typename TType> const GenericMatrix3<TType> GenericMatrix3<TType>::k_identity(1, 0, 0, 0, 1, 0, 0, 0, 1);

		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateTranslation(const GenericVector2<TType>& in_translation)
		{
			return GenericMatrix3<TType>(1, 0, 0, 0, 1, 0, in_translation.x, in_translation.y, 1);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateScale(const GenericVector2<TType>& in_scale)
		{
			return GenericMatrix3<TType>(in_scale.x, 0, 0, 0, in_scale.y, 0, 0, 0, 1);
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
			return GenericMatrix3<TType>(cosA * in_scale.x, -sinA, 0, sinA, cosA * in_scale.y, 0, in_translation.x, in_translation.y, 1);
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
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::TransposeCopy() const
		{
			GenericMatrix3<TType> a = *this;
			a.m[0] = m[0]; a.m[1] = m[3]; a.m[2] = m[6];
			a.m[3] = m[1]; a.m[4] = m[4]; a.m[5] = m[7];
			a.m[6] = m[2]; a.m[7] = m[5]; a.m[8] = m[8];
			return a;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Inverse()
		{
			*this = InverseCopy();
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::InverseCopy() const
		{
			TType det = Determinant();
			if (det == 0)
			{
				return *this;
			}
			TType oneOverDet = 1 / det;

			GenericMatrix3<TType> c;
			c.m[0] = (m[4] * m[8] - m[5] * m[7]) * oneOverDet;
			c.m[1] = (m[2] * m[7] - m[1] * m[8]) * oneOverDet;
			c.m[2] = (m[1] * m[5] - m[2] * m[4]) * oneOverDet;
			c.m[3] = (m[5] * m[6] - m[3] * m[8]) * oneOverDet;
			c.m[4] = (m[0] * m[8] - m[2] * m[6]) * oneOverDet;
			c.m[5] = (m[2] * m[3] - m[0] * m[5]) * oneOverDet;
			c.m[6] = (m[3] * m[7] - m[4] * m[6]) * oneOverDet;
			c.m[7] = (m[1] * m[6] - m[0] * m[7]) * oneOverDet;
			c.m[8] = (m[0] * m[4] - m[1] * m[3]) * oneOverDet;
			return c;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Translate(const GenericVector2<TType>& in_translation)
		{
			*this *= CreateTranslation(in_translation);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Scale(const GenericVector2<TType>& in_scale)
		{
			*this *= CreateScale(in_scale);
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> void GenericMatrix3<TType>::Rotate(TType in_angle)
		{
			*this *= CreateRotation(in_angle);
		}
		//-----------------------------------------------------
		//-----------------------------------------------------
		template <typename TType> TType& GenericMatrix3<TType>::operator()(u32 in_row, u32 in_column)
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

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
			vDSP_mmul(const_cast<f32*>(c.m), 1, const_cast<f32*>(in_b.m), 1, m, 1, 3, 3, 3);
#else
			m[0] = c.m[0] * in_b.m[0] + c.m[1] * in_b.m[3] + c.m[2] * in_b.m[6];
			m[1] = c.m[0] * in_b.m[1] + c.m[1] * in_b.m[4] + c.m[2] * in_b.m[7];
			m[2] = c.m[0] * in_b.m[2] + c.m[1] * in_b.m[5] + c.m[2] * in_b.m[8];

			m[3] = c.m[3] * in_b.m[0] + c.m[4] * in_b.m[3] + c.m[5] * in_b.m[6];
			m[4] = c.m[3] * in_b.m[1] + c.m[4] * in_b.m[4] + c.m[5] * in_b.m[7];
			m[5] = c.m[3] * in_b.m[2] + c.m[4] * in_b.m[5] + c.m[5] * in_b.m[8];

			m[6] = c.m[6] * in_b.m[0] + c.m[7] * in_b.m[3] + c.m[8] * in_b.m[6];
			m[7] = c.m[6] * in_b.m[1] + c.m[7] * in_b.m[4] + c.m[8] * in_b.m[7];
			m[8] = c.m[6] * in_b.m[2] + c.m[7] * in_b.m[5] + c.m[8] * in_b.m[8];
#endif
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

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
			vDSP_mmul(const_cast<f32*>(m), 1, const_cast<f32*>(in_b.m), 1, c.m, 1, 3, 3, 3);
#else
			c.m[0] = in_a.m[0] * in_b.m[0] + in_a.m[1] * in_b.m[3] + in_a.m[2] * in_b.m[6];
			c.m[1] = in_a.m[0] * in_b.m[1] + in_a.m[1] * in_b.m[4] + in_a.m[2] * in_b.m[7];
			c.m[2] = in_a.m[0] * in_b.m[2] + in_a.m[1] * in_b.m[5] + in_a.m[2] * in_b.m[8];

			c.m[3] = in_a.m[3] * in_b.m[0] + in_a.m[4] * in_b.m[3] + in_a.m[5] * in_b.m[6];
			c.m[4] = in_a.m[3] * in_b.m[1] + in_a.m[4] * in_b.m[4] + in_a.m[5] * in_b.m[7];
			c.m[5] = in_a.m[3] * in_b.m[2] + in_a.m[4] * in_b.m[5] + in_a.m[5] * in_b.m[8];

			c.m[6] = in_a.m[6] * in_b.m[0] + in_a.m[7] * in_b.m[3] + in_a.m[8] * in_b.m[6];
			c.m[7] = in_a.m[6] * in_b.m[1] + in_a.m[7] * in_b.m[4] + in_a.m[8] * in_b.m[7];
			c.m[8] = in_a.m[6] * in_b.m[2] + in_a.m[7] * in_b.m[5] + in_a.m[8] * in_b.m[8];
#endif
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
		template <typename TType> GenericVector3<TType>& operator*=(GenericVector3<TType>& in_a, const GenericMatrix3<TType>& in_b)
		{
			GenericVector3<TType> c = in_a;
			in_a.x = c.x * in_b.m[0] + c.y * in_b.m[3] + c.z * in_b.m[6];
			in_a.y = c.x * in_b.m[1] + c.y * in_b.m[4] + c.z * in_b.m[7];
			in_a.z = c.x * in_b.m[2] + c.y * in_b.m[5] + c.z * in_b.m[8];
			return in_a;
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
		template <typename TType> GenericVector2<TType>& operator*=(GenericVector2<TType>& in_a, const GenericMatrix3<TType>& in_b)
		{
			GenericVector2<TType> c = in_a;
			in_a.x = c.x * in_b.m[0] + c.y * in_b.m[3] + in_b.m[6];
			in_a.y = c.x * in_b.m[1] + c.y * in_b.m[4] + in_b.m[7];
			TType oneOverZ = 1 / (c.x * in_b.m[2] + c.y * in_b.m[5] + in_b.m[8]);
			in_a *= oneOverZ;
			return in_a;
		}
		//------------------------------------------------------
		//------------------------------------------------------
		template <typename TType> GenericVector2<TType> operator*(const GenericVector2<TType>& in_a, const GenericMatrix3<TType>& in_b)
		{
			GenericVector3<TType> c;
			c.x = in_a.x * in_b.m[0] + in_a.y * in_b.m[3] + in_b.m[6];
			c.y = in_a.x * in_b.m[1] + in_a.y * in_b.m[4] + in_b.m[7];
			TType oneOverZ = 1 / (c.x * in_b.m[2] + c.y * in_b.m[5] + in_b.m[8]);
			c *= oneOverZ;
			return c;
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