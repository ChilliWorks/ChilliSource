// Matrix3.h
// ICEngine
//
// Created by Ian Copland on 20/04/2012
// Copyright Ian Copland 2012. All rights reserved.

#ifndef _ICENGINE_CORE_MATH_MATRIX3_H_
#define _ICENGINE_CORE_MATH_MATRIX3_H_

#include <ICEngine/ICEngine.h>
#include <ICEngine/Core/Math/Vector2.h>
#include <ICEngine/Core/Math/Vector3.h>

namespace ICEngine
{
	//---------------------------------------------------------------
	/// A generic 3x3 matrix that provides standard matrix math
	/// functionality. The matrix is represented internally in
	/// row major format. Vector multiplication and transformation
	/// matrices are described using row vector format.Typically this 
	/// would not be used directly instead the F32 typedef Matrix3 
	/// should be used.
	//---------------------------------------------------------------
	template <typename TType> class GenericMatrix3 final
	{
	public:
		//-----------------------------------------------------
		/// Constants
		//-----------------------------------------------------
		static const GenericMatrix3<TType> kIdentity;
		//-----------------------------------------------------
		/// Constructor
		//-----------------------------------------------------
		GenericMatrix3();
		//-----------------------------------------------------
		/// Constructor
		/// @param The matrix elements in row major format.
		//-----------------------------------------------------
		GenericMatrix3(TType inA0, TType inA1, TType inA2, TType inB0, TType inB1, TType inB2, TType inC0, TType inC1, TType inC2);
		//------------------------------------------------------
		/// @return The determinant of this matrix.
		//------------------------------------------------------
		TType Determinant() const;
		//-----------------------------------------------------
		/// Sets this matrix to the identity matrix.
		//-----------------------------------------------------
		void Identity();
		//-----------------------------------------------------
		/// Sets this matrix to its transpose
		//-----------------------------------------------------
		void Transpose();
		//------------------------------------------------------
		/// @return A transposed copy of this matrix.
		//------------------------------------------------------
		GenericMatrix3<TType> TransposeCopy() const;
		//------------------------------------------------------
		/// Sets this matrix to its inverse.
		//------------------------------------------------------
		void Inverse();
		//------------------------------------------------------
		/// @return An inverted copy of this matrix.
		//------------------------------------------------------
		GenericMatrix3<TType> InverseCopy() const;
		//-----------------------------------------------------
		/// Translates the matrix
		/// @param the translation vector.
		//-----------------------------------------------------
		void Translate(const GenericVector2<TType>& inTranslation);
		//-----------------------------------------------------
		/// scales this matrix.
		/// @param scale vector
		//-----------------------------------------------------
		void Scale(const GenericVector2<TType>& inScale);
		//-----------------------------------------------------
		/// Rotates the matrix.
		/// @param the rotation.
		//-----------------------------------------------------
		void Rotate(TType inA);
		//------------------------------------------------------
		/// Create a matrix that will translate.
		/// @param the rotation.
		//------------------------------------------------------
		static GenericMatrix3<TType> CreateTranslation(const GenericVector2<TType>& inTranslation);
		//------------------------------------------------------
		/// Create a matrix that will scale.
		/// @param the rotation.
		//------------------------------------------------------
		static GenericMatrix3<TType> CreateScale(const GenericVector2<TType>& inScale);
		//------------------------------------------------------
		/// Create a matrix that will rotate.
		/// @param the rotation.
		//------------------------------------------------------
		static GenericMatrix3<TType> CreateRotation(TType inA);
		//------------------------------------------------------
		/// Operators
		//------------------------------------------------------
		Bool operator==(const GenericMatrix3<TType>& inB);
		Bool operator!=(const GenericMatrix3<TType>& inB);
		GenericMatrix3<TType>& operator+=(const GenericMatrix3<TType>& inB);
		GenericMatrix3<TType>& operator-=(const GenericMatrix3<TType>& inB);
		GenericMatrix3<TType>& operator*=(const GenericMatrix3<TType>& inB);
		GenericMatrix3<TType>& operator*=(TType inB);

		TType M[9];
	};
	//-----------------------------------------------------
	template <typename TType> const GenericMatrix3<TType> GenericMatrix3<TType>::kIdentity(1, 0, 0, 0, 1, 0, 0, 0, 1);
	//-----------------------------------------------------
	template <typename TType> GenericMatrix3<TType>::GenericMatrix3()
	{
		Identity();
	}
	//-----------------------------------------------------
	template <typename TType> GenericMatrix3<TType>::GenericMatrix3(TType inA0, TType inA1, TType inA2, TType inB0, TType inB1, TType inB2, TType inC0, TType inC1, TType inC2)
	{
		M[0] = inA0; M[1] = inA1; M[2] = inA2;
		M[3] = inB0; M[4] = inB1; M[5] = inB2;
		M[6] = inC0; M[7] = inC1; M[8] = inC2;
	}
	//------------------------------------------------------
	template <typename TType> TType GenericMatrix3<TType>::Determinant() const
	{
		return M[0] * (M[4] * M[8] - M[5] * M[7]) - M[1] * (M[3] * M[8] - M[5] * M[6]) + M[2] * (M[3] * M[7] - M[4] * M[6]);
	}
	//-----------------------------------------------------
	template <typename TType> void GenericMatrix3<TType>::Identity()
	{
		M[0] = 1; M[1] = 0; M[2] = 0;
		M[3] = 0; M[4] = 1; M[5] = 0;
		M[6] = 0; M[7] = 0; M[8] = 1;
	}
	//-----------------------------------------------------
	template <typename TType> void GenericMatrix3<TType>::Transpose()
	{
		GenericMatrix3<TType> a = *this;
		M[0] = a.M[0]; M[1] = a.M[3]; M[2] = a.M[6];
		M[3] = a.M[1]; M[4] = a.M[4]; M[5] = a.M[7];
		M[6] = a.M[2]; M[7] = a.M[5]; M[8] = a.M[8];
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::TransposeCopy() const
	{
		GenericMatrix3<TType> a = *this;
		a.M[0] = M[0]; a.M[1] = M[3]; a.M[2] = M[6];
		a.M[3] = M[1]; a.M[4] = M[4]; a.M[5] = M[7];
		a.M[6] = M[2]; a.M[7] = M[5]; a.M[8] = M[8];
		return a;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix3<TType>::Inverse()
	{
		*this = InverseCopy();
	}
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
		c.M[0] = (M[4] * M[8] - M[5] * M[7]) * oneOverDet;
		c.M[1] = (M[2] * M[7] - M[1] * M[8]) * oneOverDet;
		c.M[2] = (M[1] * M[5] - M[2] * M[4]) * oneOverDet;
		c.M[3] = (M[5] * M[6] - M[3] * M[8]) * oneOverDet;
		c.M[4] = (M[0] * M[8] - M[2] * M[6]) * oneOverDet;
		c.M[5] = (M[2] * M[3] - M[0] * M[5]) * oneOverDet;
		c.M[6] = (M[3] * M[7] - M[4] * M[6]) * oneOverDet;
		c.M[7] = (M[1] * M[6] - M[0] * M[7]) * oneOverDet;
		c.M[8] = (M[0] * M[4] - M[1] * M[3]) * oneOverDet;
		return c;
	}
	//-----------------------------------------------------
	template <typename TType> void GenericMatrix3<TType>::Translate(const GenericVector2<TType>& inTranslation)
	{
		M[6] += inTranslation.X;
		M[7] += inTranslation.Y;
	}
	//-----------------------------------------------------
	template <typename TType> void GenericMatrix3<TType>::Scale(const GenericVector2<TType>& inScale)
	{
		GenericMatrix3<TType> c = CreateScale(inScale);
		*this *= c;
	}
	//-----------------------------------------------------
	template <typename TType> void GenericMatrix3<TType>::Rotate(TType inA)
	{
		GenericMatrix3<TType> c = CreateRotation(inA);
		*this *= c;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateTranslation(const GenericVector2<TType>& inTranslation)
	{
		return GenericMatrix3<TType>(1, 0, 0,
									0, 1, 0,
									inTranslation.X, inTranslation.Y, 1);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateScale(const GenericVector2<TType>& inScale)
	{
		return GenericMatrix3<TType>(inScale.X, 0, 0,
									0., inScale.Y, 0,
									0, 0, 1);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> GenericMatrix3<TType>::CreateRotation(TType inA)
	{
		TType sinA = (TType)sin(inA);
		TType cosA = (TType)cos(inA);
		return GenericMatrix3<TType>(cosA, sinA, 0,
									-sinA, cosA, 0,
									0, 0, 1);
	}
	//------------------------------------------------------
	template <typename TType> Bool GenericMatrix3<TType>::operator==(const GenericMatrix3<TType>& inB)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (inmA.M[i] != inmB.M[i])
				return false;
		}
		return true;
	}
	//------------------------------------------------------
	template <typename TType> Bool GenericMatrix3<TType>::operator!=(const GenericMatrix3<TType>& inB)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (inmA.M[i] != inmB.M[i])
				return true;
		}
		return false;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator+=(const GenericMatrix3<TType>& inB)
	{
		M[0] += inB.M[0]; M[1] += inB.M[1]; M[2] += inB.M[2];
		M[3] += inB.M[3]; M[4] += inB.M[4]; M[5] += inB.M[5];
		M[6] += inB.M[6]; M[7] += inB.M[7]; M[8] += inB.M[8];
		return *this;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator-=(const GenericMatrix3<TType>& inB)
	{
		M[0] -= inB.M[0]; M[1] -= inB.M[1]; M[2] -= inB.M[2];
		M[3] -= inB.M[3]; M[4] -= inB.M[4]; M[5] -= inB.M[5];
		M[6] -= inB.M[6]; M[7] -= inB.M[7]; M[8] -= inB.M[8];
		return *this;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator*=(const GenericMatrix3<TType>& inB)
	{
		*this = (*this * inB);
		return *this;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType>& GenericMatrix3<TType>::operator*=(TType inB)
	{
		M[0] *= inB; M[1] *= inB; M[2] *= inB;
		M[3] *= inB; M[4] *= inB; M[5] *= inB;
		M[6] *= inB; M[7] *= inB; M[8] *= inB;
		return *this;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> operator+(const GenericMatrix3<TType>& inA, const GenericMatrix3<TType>& inB)
	{
		GenericMatrix3<TType> copy = inA;
		return (copy += inB);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> operator-(const GenericMatrix3<TType>& inA, const GenericMatrix3<TType>& inB)
	{
		GenericMatrix3<TType> copy = inA;
		return (copy -= inB);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> operator*(const GenericMatrix3<TType>& inA, const GenericMatrix3<TType>& inB)
	{
		Matrix3 c;

		c.M[0] = inA.M[0] * inB.M[0] + inA.M[1] * inB.M[3] + inA.M[2] * inB.M[6];
		c.M[1] = inA.M[0] * inB.M[1] + inA.M[1] * inB.M[4] + inA.M[2] * inB.M[7];
		c.M[2] = inA.M[0] * inB.M[2] + inA.M[1] * inB.M[5] + inA.M[2] * inB.M[8];

		c.M[3] = inA.M[3] * inB.M[0] + inA.M[4] * inB.M[3] + inA.M[5] * inB.M[6];
		c.M[4] = inA.M[3] * inB.M[1] + inA.M[4] * inB.M[4] + inA.M[5] * inB.M[7];
		c.M[5] = inA.M[3] * inB.M[2] + inA.M[4] * inB.M[5] + inA.M[5] * inB.M[8];

		c.M[6] = inA.M[6] * inB.M[0] + inA.M[7] * inB.M[3] + inA.M[8] * inB.M[6];
		c.M[7] = inA.M[6] * inB.M[1] + inA.M[7] * inB.M[4] + inA.M[8] * inB.M[7];
		c.M[8] = inA.M[6] * inB.M[2] + inA.M[7] * inB.M[5] + inA.M[8] * inB.M[8];

		return c;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> operator*(const GenericMatrix3<TType>& inA, TType inB)
	{
		GenericMatrix3<TType> copy = inA;
		return (copy *= inB);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix3<TType> operator*(TType inA, const GenericMatrix3<TType>& inB)
	{
		GenericMatrix3<TType> copy = inB;
		return (copy *= inA);
	}
	//------------------------------------------------------
	template <typename TType> GenericVector3<TType>& operator*=(GenericVector3<TType>& inA, const GenericMatrix3<TType>& inB)
	{
		GenericVector3<TType> c = inA;
		inA.X = c.X * inB.M[0] + c.Y * inB.M[3] + c.Z * inB.M[6];
		inA.Y = c.X * inB.M[1] + c.Y * inB.M[4] + c.Z * inB.M[7];
		inA.Z = c.X * inB.M[2] + c.Y * inB.M[5] + c.Z * inB.M[8];
		return inA;
	}
	//------------------------------------------------------
	template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& inA, const GenericMatrix3<TType>& inB)
	{
		GenericVector3<TType> c;
		c.X = inA.X * inB.M[0] + inA.Y * inB.M[3] + inA.Z * inB.M[6];
		c.Y = inA.X * inB.M[1] + inA.Y * inB.M[4] + inA.Z * inB.M[7];
		c.Z = inA.X * inB.M[2] + inA.Y * inB.M[5] + inA.Z * inB.M[8];
		return c;
	}
	//------------------------------------------------------
	template <typename TType> GenericVector2<TType>& operator*=(GenericVector2<TType>& inA, const GenericMatrix3<TType>& inB)
	{
		GenericVector2<TType> c = inA;
		inA.X = c.X * inmB.M[0] + c.Y * inmB.M[3] + inmB.M[6];
		inA.Y = c.X * inmB.M[1] + c.Y * inmB.M[4] + inmB.M[7];
		TType oneOverZ = 1 / (c.X * inB.M[2] + c.Y * inB.M[5] + inB.M[8]);
		inA *= oneOverZ;
		return inA;
	}
	//------------------------------------------------------
	template <typename TType> GenericVector2<TType> operator*(const GenericVector2<TType>& invA, const GenericMatrix3<TType>& inB)
	{
		GenericVector3<TType> c;
		c.X = invA.X * inmB.M[0] + invA.Y * inmB.M[3] + inmB.M[6];
		c.Y = invA.X * inmB.M[1] + invA.Y * inmB.M[4] + inmB.M[7];
		TType oneOverZ = 1 / (c.X * inB.M[2] + c.Y * inB.M[5] + inB.M[8]);
		c *= oneOverZ;
		return c;
	}
}

#endif