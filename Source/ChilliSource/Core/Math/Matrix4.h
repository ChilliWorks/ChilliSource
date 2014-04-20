// Matrix4.h
// ICEngine
//
// Created by Ian Copland on 20/04/2012
// Copyright Ian Copland 2012. All rights reserved.

#ifndef _ICENGINE_CORE_MATH_MATRIX4_H_
#define _ICENGINE_CORE_MATH_MATRIX4_H_

#include <ICEngine/ICEngine.h>
#include <ICEngine/Core/Math/Vector3.h>
#include <ICEngine/Core/Math/Vector4.h>
#include <ICEngine/Core/Math/Quaternion.h>

namespace ICEngine
{
	//-------------------------------------------------------------
	/// A generic 4x4 matrix that provides standard matrix math
	/// functionality. The matrix is represented internally in
	/// row major format. Vector multiplication and transformation
	/// matrices are described using row vector format.Typically this 
	/// would not be used directly instead the F32 typedef Matrix4 
	/// should be used.
	//-------------------------------------------------------------
	template <typename TType> class GenericMatrix4 final
	{
	public:
		//------------------------------------------------------
		/// Const matrices
		//------------------------------------------------------
		static const GenericMatrix4<TType> kIdentity;
		//------------------------------------------------------
		/// Constructor
		//------------------------------------------------------
		GenericMatrix4();
		//------------------------------------------------------
		/// Constructor
		/// @params The 16 matrix elements in row major format.
		//------------------------------------------------------
		GenericMatrix4(TType inA0, TType inA1, TType inA2, TType inA3, TType inB0, TType inB1, TType inB2, TType inB3, TType inC0, TType inC1, TType inC2, TType inC3, TType inD0, TType inD1, TType inD2, TType inD3);
		//------------------------------------------------------
		/// @return The determinant of this matrix.
		//------------------------------------------------------
		TType Determinant() const;
		//------------------------------------------------------
		/// Sets this matrix to the identity matrix.
		//------------------------------------------------------
		void Identity();
		//------------------------------------------------------
		/// Sets this matrix to its transpose.
		//------------------------------------------------------
		void Transpose();
		//------------------------------------------------------
		/// @return A transposed copy of this matrix.
		//------------------------------------------------------
		GenericMatrix4<TType> TransposeCopy() const;
		//------------------------------------------------------
		/// Sets this matrix to its inverse.
		//------------------------------------------------------
		void Inverse();
		//------------------------------------------------------
		/// @return An inverted copy of this matrix.
		//------------------------------------------------------
		GenericMatrix4<TType> InverseCopy() const;
		//------------------------------------------------------
		/// Translate this matrix.
		/// @param the translation vector
		//------------------------------------------------------
		void Translate(const GenericVector3<TType>& inTranslation);
		//------------------------------------------------------
		/// Scales this matrix
		/// @param the scale vector
		//------------------------------------------------------
		void Scale(const GenericVector3<TType>& inScale);
		//------------------------------------------------------
		/// Rotates arround the X axis.
		/// @param the rotation.
		//------------------------------------------------------
		void RotateX(TType inA);
		//------------------------------------------------------
		/// Rotates arround the Y axis.
		/// @param the rotation.
		//------------------------------------------------------
		void RotateY(TType inA);
		//------------------------------------------------------
		/// Rotates arround the Z axis.
		/// @param the rotation.
		//------------------------------------------------------
		void RotateZ(TType inA);
		//------------------------------------------------------
		/// Rotates the matrix by the given quaternion rotation.
		/// @param the rotation quaternion.
		//------------------------------------------------------
		void Rotate(const GenericQuaternion<TType>& inOrientation);
		//------------------------------------------------------
		/// Creates a new perspective projection matrix with the 
		/// given parameters. This projection matrix uses a
		/// Left Handed coordinate system.
		/// @param the view angle.
		/// @param the aspect ratio.
		/// @param the near plane.
		/// @param the far plane.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreatePerspectiveProjection(TType inViewAngle, TType inAspectRatio, TType inNear, TType inFar);
		//------------------------------------------------------
		/// Creates a new orthographic projection matrix with the 
		/// given parameters. This projection matrix uses a
		/// Left Handed coordinate system.
		/// @param the width.
		/// @param the height.
		/// @param the near plane.
		/// @param the far plane.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreateOrthographicProjection(TType inWidth, TType inHeight, TType inNear, TType inFar);
		//------------------------------------------------------
		/// Creates a new view matrix with the given camera
		/// position, look at and up vector.
		/// @param the position.
		/// @param the look at.
		/// @param the up.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreateView(const GenericVector3<TType>& inPosition, const GenericVector3<TType>& inLookAt, const GenericVector3<TType>& inUp);
		//------------------------------------------------------
		/// Create a matrix that can be used for translation.
		/// @param the translation.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreateTranslation(const GenericVector3<TType>& inTranslation);
		//------------------------------------------------------
		/// Create a matrix that can be used for scaling.
		/// @param the rotation.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreateScale(const GenericVector3<TType>& inScale);
		//------------------------------------------------------
		/// Create a matrix that can be used to rotate around
		/// the X axis.
		/// @param the rotation.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreateRotationX(TType inRotation);
		//------------------------------------------------------
		/// Create a matrix that can be used to rotate around
		/// the Y axis.
		/// @param the rotation.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreateRotationY(TType inRotation);
		//------------------------------------------------------
		/// Create a matrix that that be used to rotate around
		/// the Z axis.
		/// @param the rotation.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreateRotationZ(TType inRotation);
		//------------------------------------------------------
		/// Creates a rotation matrix from a quaternion.
		/// @param the quaternion.
		//------------------------------------------------------
		static GenericMatrix4<TType> CreateRotation(const GenericQuaternion<TType>& inRotation);
		//------------------------------------------------------
		/// operators
		//------------------------------------------------------
		Bool operator==(const GenericMatrix4<TType>& inA);
		Bool operator!=(const GenericMatrix4<TType>& inA);
		GenericMatrix4<TType>& operator+=(const GenericMatrix4<TType>& inB);
		GenericMatrix4<TType>& operator-=(const GenericMatrix4<TType>& inB);
		GenericMatrix4<TType>& operator*=(const GenericMatrix4<TType>& inB);
		GenericMatrix4<TType>& operator*=(TType inB);

		TType M[16];
	};
	template <typename TType> const GenericMatrix4<TType> GenericMatrix4<TType>::kIdentity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType>::GenericMatrix4()
	{
		Identity();
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType>::GenericMatrix4(TType inA0, TType inA1, TType inA2, TType inA3, TType inB0, TType inB1, TType inB2, TType inB3, TType inC0, TType inC1, TType inC2, TType inC3, TType inD0, TType inD1, TType inD2, TType inD3)
	{
		M[0] = inA0; M[1] = inA1; M[2] = inA2; M[3] = inA3;
		M[4] = inB0; M[5] = inB1; M[6] = inB2; M[7] = inB3;
		M[8] = inC0; M[9] = inC1; M[10] = inC2; M[11] = inC3;
		M[12] = inD0; M[13] = inD1; M[14] = inD2; M[15] = inD3;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::Identity()
	{
		*this = kIdentity;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::Transpose()
	{
		GenericMatrix4<TType> a = *this;
		M[0] = a.M[0]; M[1] = a.M[4]; M[2] = a.M[8]; M[3] = a.M[12];
		M[4] = a.M[1]; M[5] = a.M[5]; M[6] = a.M[9]; M[7] = a.M[13];
		M[8] = a.M[2]; M[9] = a.M[6]; M[10] = a.M[10]; M[11] = a.M[14];
		M[12] = a.M[3]; M[13] = a.M[7]; M[14] = a.M[11]; M[15] = a.M[15];
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::TransposeCopy() const
	{
		Matrix4 A = *this;
		A.M[0] = M[0]; A.M[1] = M[4]; A.M[2] = M[8]; A.M[3] = M[12];
		A.M[4] = M[1]; A.M[5] = M[5]; A.M[6] = M[9]; A.M[7] = M[13];
		A.M[8] = M[2]; A.M[9] = M[6]; A.M[10] = M[10]; A.M[11] = M[14];
		A.M[12] = M[3]; A.M[13] = M[7]; A.M[14] = M[11]; A.M[15] = M[15];
		return A;
	}
	//------------------------------------------------------
	template <typename TType> TType GenericMatrix4<TType>::Determinant() const
	{
		return	M[3] * M[6] * M[9] * M[12] - M[2] * M[7] * M[9] * M[12] - M[3] * M[5] * M[10] * M[12] + M[1] * M[7] * M[10] * M[12] +
			M[2] * M[5] * M[11] * M[12] - M[1] * M[6] * M[11] * M[12] - M[3] * M[6] * M[8] * M[13] + M[2] * M[7] * M[8] * M[13] +
			M[3] * M[4] * M[10] * M[13] - M[0] * M[7] * M[10] * M[13] - M[2] * M[4] * M[11] * M[13] + M[0] * M[6] * M[11] * M[13] +
			M[3] * M[5] * M[8] * M[14] - M[1] * M[7] * M[8] * M[14] - M[3] * M[4] * M[9] * M[14] + M[0] * M[7] * M[9] * M[14] +
			M[1] * M[4] * M[11] * M[14] - M[0] * M[5] * M[11] * M[14] - M[2] * M[5] * M[8] * M[15] + M[1] * M[6] * M[8] * M[15] +
			M[2] * M[4] * M[9] * M[15] - M[0] * M[6] * M[9] * M[15] - M[1] * M[4] * M[10] * M[15] + M[0] * M[5] * M[10] * M[15];
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::Inverse()
	{
		*this = InverseCopy();
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::InverseCopy() const
	{
		TType det = Determinant();
		if (det == 0)
		{
			return *this;
		}

		GenericMatrix4<TType> a = *this;
		a.M[0] = (M[6] * M[11] * M[13] - M[7] * M[10] * M[13] + M[7] * M[9] * M[14] - M[5] * M[11] * M[14] - M[6] * M[9] * M[15] + M[5] * M[10] * M[15]) / det;
		a.M[1] = (M[3] * M[10] * M[13] - M[2] * M[11] * M[13] - M[3] * M[9] * M[14] + M[1] * M[11] * M[14] + M[2] * M[9] * M[15] - M[1] * M[10] * M[15]) / det;
		a.M[2] = (M[2] * M[7] * M[13] - M[3] * M[6] * M[13] + M[3] * M[5] * M[14] - M[1] * M[7] * M[14] - M[2] * M[5] * M[15] + M[1] * M[6] * M[15]) / det;
		a.M[3] = (M[3] * M[6] * M[9] - M[2] * M[7] * M[9] - M[3] * M[5] * M[10] + M[1] * M[7] * M[10] + M[2] * M[5] * M[11] - M[1] * M[6] * M[11]) / det;
		a.M[4] = (M[7] * M[10] * M[12] - M[6] * M[11] * M[12] - M[7] * M[8] * M[14] + M[4] * M[11] * M[14] + M[6] * M[8] * M[15] - M[4] * M[10] * M[15]) / det;
		a.M[5] = (M[2] * M[11] * M[12] - M[3] * M[10] * M[12] + M[3] * M[8] * M[14] - M[0] * M[11] * M[14] - M[2] * M[8] * M[15] + M[0] * M[10] * M[15]) / det;
		a.M[6] = (M[3] * M[6] * M[12] - M[2] * M[7] * M[12] - M[3] * M[4] * M[14] + M[0] * M[7] * M[14] + M[2] * M[4] * M[15] - M[0] * M[6] * M[15]) / det;
		a.M[7] = (M[2] * M[7] * M[8] - M[3] * M[6] * M[8] + M[3] * M[4] * M[10] - M[0] * M[7] * M[10] - M[2] * M[4] * M[11] + M[0] * M[6] * M[11]) / det;
		a.M[8] = (M[5] * M[11] * M[12] - M[7] * M[9] * M[12] + M[7] * M[8] * M[13] - M[4] * M[11] * M[13] - M[5] * M[8] * M[15] + M[4] * M[9] * M[15]) / det;
		a.M[9] = (M[3] * M[9] * M[12] - M[1] * M[11] * M[12] - M[3] * M[8] * M[13] + M[0] * M[11] * M[13] + M[1] * M[8] * M[15] - M[0] * M[9] * M[15]) / det;
		a.M[10] = (M[1] * M[7] * M[12] - M[3] * M[5] * M[12] + M[3] * M[4] * M[13] - M[0] * M[7] * M[13] - M[1] * M[4] * M[15] + M[0] * M[5] * M[15]) / det;
		a.M[11] = (M[3] * M[5] * M[8] - M[1] * M[7] * M[8] - M[3] * M[4] * M[9] + M[0] * M[7] * M[9] + M[1] * M[4] * M[11] - M[0] * M[5] * M[11]) / det;
		a.M[12] = (M[6] * M[9] * M[12] - M[5] * M[10] * M[12] - M[6] * M[8] * M[13] + M[4] * M[10] * M[13] + M[5] * M[8] * M[14] - M[4] * M[9] * M[14]) / det;
		a.M[13] = (M[1] * M[10] * M[12] - M[2] * M[9] * M[12] + M[2] * M[8] * M[13] - M[0] * M[10] * M[13] - M[1] * M[8] * M[14] + M[0] * M[9] * M[14]) / det;
		a.M[14] = (M[2] * M[5] * M[12] - M[1] * M[6] * M[12] - M[2] * M[4] * M[13] + M[0] * M[6] * M[13] + M[1] * M[4] * M[14] - M[0] * M[5] * M[14]) / det;
		a.M[15] = (M[1] * M[6] * M[8] - M[2] * M[5] * M[8] + M[2] * M[4] * M[9] - M[0] * M[6] * M[9] - M[1] * M[4] * M[10] + M[0] * M[5] * M[10]) / det;
		return a;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::Translate(const GenericVector3<TType>& inTranslation)
	{
		M[12] += inTranslation.X;
		M[13] += inTranslation.Y;
		M[14] += inTranslation.Z;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::Scale(const GenericVector3<TType>& inScale)
	{
		GenericMatrix4<TType> c = CreateScale(inScale);
		*this *= c;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::RotateX(TType inA)
	{
		GenericMatrix4<TType> c = CreateRotationX(inA);
		*this *= c;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::RotateY(TType inA)
	{
		GenericMatrix4<TType> c = CreateRotationY(inA);
		*this *= c;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::RotateZ(TType inA)
	{
		GenericMatrix4<TType> c = CreateRotationZ(inA);
		*this *= c;
	}
	//------------------------------------------------------
	template <typename TType> void GenericMatrix4<TType>::Rotate(const GenericQuaternion<TType>& inOrientation)
	{
		GenericMatrix4<TType> c = CreateRotation(inOrientation);
		*this *= c;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreatePerspectiveProjection(TType inViewAngle, TType inAspectRatio, TType inNear, TType inFar)
	{
		//calculate the parameters
		TType b1 = 1 / tan(inViewAngle * (TType)0.5);
		TType a0 = b1 / inAspectRatio;
		TType c2 = (inFar + inNear) / (inFar - inNear);
		TType d2 = -((2 * inFar * inNear) / (inFar - inNear));

		return GenericMatrix4<TType>(a0, 0, 0, 0,
			0, b1, 0, 0,
			0, 0, c2, 1,
			0, 0, d2, 0);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateOrthographicProjection(TType inWidth, TType inHeight, TType inNear, TType inFar)
	{
		TType a0 = 2 / inWidth;
		TType b1 = 2 / inHeight;
		TType c2 = 2 / (inFar - inNear);
		TType d2 = (inFar + inNear) / (inNear - inFar);

		return Matrix4(a0, 0, 0, 0,
			0, b1, 0, 0,
			0, 0, c2, 0,
			0, 0, d2, 1);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateView(const GenericVector3<TType>& inPosition, const GenericVector3<TType>& inLookAt, const GenericVector3<TType>& inUp)
	{
		GenericVector3<TType> zAxis = inLookAt - inPosition;
		zAxis.Normalise();
		GenericVector3<TType> xAxis = GenericVector3<TType>::CrossProduct(inUp, zAxis);
		xAxis.Normalise();
		GenericVector3<TType> yAxis = GenericVector3<TType>::CrossProduct(zAxis, xAxis);
		TType dotX = GenericVector3<TType>::DotProduct(xAxis, inPosition);
		TType dotY = GenericVector3<TType>::DotProduct(yAxis, inPosition);
		TType dotZ = GenericVector3<TType>::DotProduct(zAxis, inPosition);

		GenericMatrix4<TType> m;
		m.M[0] = xAxis.X;		m.M[1] = yAxis.X;		m.M[2] = zAxis.X;		m.M[3] = 0;
		m.M[4] = xAxis.Y;		m.M[5] = yAxis.Y;		m.M[6] = zAxis.Y;		m.M[7] = 0;
		m.M[8] = xAxis.Z;		m.M[9] = yAxis.Z;		m.M[10] = zAxis.Z;		m.M[11] = 0;
		m.M[12] = -dotX;		m.M[13] = -dotY;		m.M[14] = -dotZ;		m.M[15] = 1;
		return m;
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateTranslation(const GenericVector3<TType>& inTranslation)
	{
		return GenericMatrix4<TType>(1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, 1, 0,
									inTranslation.X, inTranslation.Y, inTranslation.Z, 1);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateScale(const GenericVector3<TType>& inScale)
	{
		return GenericMatrix4<TType>(inScale.X, 0, 0, 0,
									0, inScale.Y, 0, 0,
									0, 0, inScale.Z, 0,
									0, 0, 0, 1);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationX(TType inRotation)
	{
		TType sinA = (TType)sin(inRotation);
		TType cosA = (TType)cos(inRotation);
		return GenericMatrix4<TType>(1, 0, 0, 0,
									0, cosA, sinA, 0,
									0, -sinA, cosA, 0,
									0, 0, 0, 1);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationY(TType inRotation)
	{
		TType sinA = (TType)sin(inRotation);
		TType cosA = (TType)cos(inRotation);
		return GenericMatrix4<TType>(cosA, 0, -sinA, 0,
									0, 1, 0, 0,
									sinA, 0, cosA, 0,
									0, 0, 0, 1);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotationZ(TType inRotation)
	{
		TType sinA = (TType)sin(inRotation);
		TType cosA = (TType)cos(inRotation);
		return GenericMatrix4<TType>(cosA, sinA, 0, 0,
									-sinA, cosA, 0, 0,
									0, 0, 1, 0,
									0, 0, 0, 1);
	}
	//------------------------------------------------------
	template <typename TType> GenericMatrix4<TType> GenericMatrix4<TType>::CreateRotation(const GenericQuaternion<TType>& inRotation)
	{
		GenericQuaternion<TType> q;
		if (inRotation.MagnitudeSquared() == 1)
		{
			q = inRotation;
		}
		else if (q.MagnitudeSquared() > 0)
		{
			q = inRotation.NormaliseCopy();
		}

		TType wSquared = q.W * q.W;
		TType xSquared = q.X * q.X;
		TType ySquared = q.Y * q.Y;
		TType zSquared = q.Z * q.Z;

		TType a = wSquared + xSquared - ySquared - zSquared;
		TType b = 2 * q.X * q.Y + 2 * q.W * q.Z;
		TType c = 2 * q.X * q.Z - 2 * q.W * q.Y;
		TType d = 0;
		TType e = 2 * q.X * q.Y - 2 * q.W * q.Z;
		TType f = wSquared - xSquared + ySquared - zSquared;
		TType g = 2 * q.Y * q.Z + 2 * q.W * q.X;
		TType h = 0;
		TType i = 2 * q.X * q.Z + 2 * q.W * q.Y;
		TType j = 2 * q.Y * q.Z - 2 * q.W * q.X;
		TType k = wSquared - xSquared - ySquared + zSquared;
		TType l = 0;
		TType m = 0;
		TType n = 0;
		TType o = 0;
		TType p = 1;

		return GenericMatrix4<TType>(a, b, c, d,
									e, f, g, h,
									i, j, k, l,
									m, n, o, p);
	}
	//---------------------------------------------------
	template <typename TType> Bool GenericMatrix4<TType>::operator==(const GenericMatrix4<TType>& inB)
	{
		for (U32 i = 0; i < 16; ++i)
		{
			if (M[i] != inB.M[i])
				return false;
		}
		return true;
	}
	//---------------------------------------------------
	template <typename TType> Bool GenericMatrix4<TType>::operator!=(const GenericMatrix4<TType>& inB)
	{
		return !(*this == inB);
	}
	//---------------------------------------------------
	template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator+=(const GenericMatrix4<TType>& inB)
	{
		M[0] += inB.M[0]; M[1] += inB.M[1]; M[2] += inB.M[2]; M[3] += inB.M[3];
		M[4] += inB.M[4]; M[5] += inB.M[5]; M[6] += inB.M[6]; M[7] += inB.M[7];
		M[8] += inB.M[8]; M[9] += inB.M[9]; M[10] += inB.M[10]; M[11] += inB.M[11];
		M[12] += inB.M[12]; M[13] += inB.M[13]; M[14] += inB.M[14]; M[15] += inB.M[15];
		return *this;
	}
	//---------------------------------------------------
	template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator-=(const GenericMatrix4<TType>& inB)
	{
		M[0] += inB.M[0]; M[1] += inB.M[1]; M[2] += inB.M[2]; M[3] += inB.M[3];
		M[4] += inB.M[4]; M[5] += inB.M[5]; M[6] += inB.M[6]; M[7] += inB.M[7];
		M[8] += inB.M[8]; M[9] += inB.M[9]; M[10] += inB.M[10]; M[11] += inB.M[11];
		M[12] += inB.M[12]; M[13] += inB.M[13]; M[14] += inB.M[14]; M[15] += inB.M[15];
		return *this;
	}
	//---------------------------------------------------
	template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator*=(const GenericMatrix4<TType>& inB)
	{
		*this = (*this * inB);
		return *this;
	}
	//---------------------------------------------------
	template <typename TType> GenericMatrix4<TType>& GenericMatrix4<TType>::operator*=(TType inB)
	{
		M[0] *= inB; M[1] *= inB; M[2] *= inB; M[3] *= inB;
		M[4] *= inB; M[5] *= inB; M[6] *= inB; M[7] *= inB;
		M[8] *= inB; M[9] *= inB; M[10] *= inB; M[11] *= inB;
		M[12] *= inB; M[13] *= inB; M[14] *= inB; M[15] *= inB;
		return *this;
	}
	//------------------------------------------
	template <typename TType> GenericMatrix4<TType> operator+(const GenericMatrix4<TType>& inA, const GenericMatrix4<TType>& inB)
	{
		GenericMatrix4<TType> copy = inA;
		return (copy += inB);
	}
	//------------------------------------------
	template <typename TType> GenericMatrix4<TType> operator-(const GenericMatrix4<TType>& inA, const GenericMatrix4<TType>& inB)
	{
		GenericMatrix4<TType> copy = inA;
		return (copy -= inB);
	}
	//------------------------------------------
	template <typename TType> GenericMatrix4<TType> operator*(const GenericMatrix4<TType>& inA, const GenericMatrix4<TType>& inB)
	{
		GenericMatrix4<TType> c;

		c.M[0] = inA.M[0] * inB.M[0] + inA.M[1] * inB.M[4] + inA.M[2] * inB.M[8] + inA.M[3] * inB.M[12];
		c.M[1] = inA.M[0] * inB.M[1] + inA.M[1] * inB.M[5] + inA.M[2] * inB.M[9] + inA.M[3] * inB.M[13];
		c.M[2] = inA.M[0] * inB.M[2] + inA.M[1] * inB.M[6] + inA.M[2] * inB.M[10] + inA.M[3] * inB.M[14];
		c.M[3] = inA.M[0] * inB.M[3] + inA.M[1] * inB.M[7] + inA.M[2] * inB.M[11] + inA.M[3] * inB.M[15];

		c.M[4] = inA.M[4] * inB.M[0] + inA.M[5] * inB.M[4] + inA.M[6] * inB.M[8] + inA.M[7] * inB.M[12];
		c.M[5] = inA.M[4] * inB.M[1] + inA.M[5] * inB.M[5] + inA.M[6] * inB.M[9] + inA.M[7] * inB.M[13];
		c.M[6] = inA.M[4] * inB.M[2] + inA.M[5] * inB.M[6] + inA.M[6] * inB.M[10] + inA.M[7] * inB.M[14];
		c.M[7] = inA.M[4] * inB.M[3] + inA.M[5] * inB.M[7] + inA.M[6] * inB.M[11] + inA.M[7] * inB.M[15];

		c.M[8] = inA.M[8] * inB.M[0] + inA.M[9] * inB.M[4] + inA.M[10] * inB.M[8] + inA.M[11] * inB.M[12];
		c.M[9] = inA.M[8] * inB.M[1] + inA.M[9] * inB.M[5] + inA.M[10] * inB.M[9] + inA.M[11] * inB.M[13];
		c.M[10] = inA.M[8] * inB.M[2] + inA.M[9] * inB.M[6] + inA.M[10] * inB.M[10] + inA.M[11] * inB.M[14];
		c.M[11] = inA.M[8] * inB.M[3] + inA.M[9] * inB.M[7] + inA.M[10] * inB.M[11] + inA.M[11] * inB.M[15];

		c.M[12] = inA.M[12] * inB.M[0] + inA.M[13] * inB.M[4] + inA.M[14] * inB.M[8] + inA.M[15] * inB.M[12];
		c.M[13] = inA.M[12] * inB.M[1] + inA.M[13] * inB.M[5] + inA.M[14] * inB.M[9] + inA.M[15] * inB.M[13];
		c.M[14] = inA.M[12] * inB.M[2] + inA.M[13] * inB.M[6] + inA.M[14] * inB.M[10] + inA.M[15] * inB.M[14];
		c.M[15] = inA.M[12] * inB.M[3] + inA.M[13] * inB.M[7] + inA.M[14] * inB.M[11] + inA.M[15] * inB.M[15];

		return c;
	}
	//------------------------------------------
	template <typename TType> GenericMatrix4<TType> operator*(const GenericMatrix4<TType>& inA, TType inB)
	{
		GenericMatrix4<TType> copy = inA;
		return (copy *= inB);
	}
	//------------------------------------------
	template <typename TType> GenericMatrix4<TType> operator*(TType inA, const GenericMatrix4<TType>& inB)
	{
		GenericMatrix4<TType> copy = inB;
		return (copy *= inA);
	}
	//------------------------------------------
	template <typename TType> GenericVector4<TType>& operator*=(GenericVector4<TType>& inA, const GenericMatrix4<TType>& inB)
	{
		GenericVector4<TType> c = inA;
		inA.X = c.X * inB.M[0] + c.Y * inB.M[4] + c.Z * inB.M[8] + c.W * inB.M[12];
		inA.Y = c.X * inB.M[1] + c.Y * inB.M[5] + c.Z * inB.M[9] + c.W * inB.M[13];
		inA.Z = c.X * inB.M[2] + c.Y * inB.M[6] + c.Z * inB.M[10] + c.W * inB.M[14];
		inA.W = c.X * inB.M[3] + c.Y * inB.M[7] + c.Z * inB.M[11] + c.W * inB.M[15];
		return inA;
	}
	//------------------------------------------
	template <typename TType> GenericVector4<TType> operator*(const GenericVector4<TType>& inA, const GenericMatrix4<TType>& inB)
	{
		GenericVector4<TType> c;
		c.X = inA.X * inB.M[0] + inA.Y * inB.M[4] + inA.Z * inB.M[8] + inA.W * inB.M[12];
		c.Y = inA.X * inB.M[1] + inA.Y * inB.M[5] + inA.Z * inB.M[9] + inA.W * inB.M[13];
		c.Z = inA.X * inB.M[2] + inA.Y * inB.M[6] + inA.Z * inB.M[10] + inA.W * inB.M[14];
		c.W = inA.X * inB.M[3] + inA.Y * inB.M[7] + inA.Z * inB.M[11] + inA.W * inB.M[15];
		return c;
	}
	//------------------------------------------
	template <typename TType> GenericVector3<TType>& operator*=(GenericVector3<TType>& inA, const GenericMatrix4<TType>& inB)
	{
		GenericVector3<TType> c = inA;
		inA.X = c.X * inB.M[0] + c.Y * inB.M[4] + c.Z * inB.M[8] + inB.M[12];
		inA.Y = c.X * inB.M[1] + c.Y * inB.M[5] + c.Z * inB.M[9] + inB.M[13];
		inA.Z = c.X * inB.M[2] + c.Y * inB.M[6] + c.Z * inB.M[10] + inB.M[14];
		TType oneOverW = 1 / (c.X * inB.M[3] + c.Y * inB.M[7] + c.Z * inB.M[11] + inB.M[15]);
		inA *= oneOverW;
		return inA;
	}
	//------------------------------------------
	template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& inA, const GenericMatrix4<TType>& inB)
	{
		GenericVector3<TType> c;
		c.X = inA.X * inB.M[0] + inA.Y * inB.M[4] + inA.Z * inB.M[8] + inB.M[12];
		c.Y = inA.X * inB.M[1] + inA.Y * inB.M[5] + inA.Z * inB.M[9] + inB.M[13];
		c.Z = inA.X * inB.M[2] + inA.Y * inB.M[6] + inA.Z * inB.M[10] + inB.M[14];
		TType oneOverW = 1 / (inA.X * inB.M[3] + inA.Y * inB.M[7] + inA.Z * inB.M[11] + inB.M[15]);
		c *= oneOverW;
		return c;
	}
}

#endif