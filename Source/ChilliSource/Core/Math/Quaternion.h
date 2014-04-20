// Quaternion.h
// ICEngine
//
// Created by Ian Copland on 05/02/2013
// Copyright Ian Copland 2013. All rights reserved.

#ifndef _ICENGINE_CORE_MATH_QUATERNION_H_
#define _ICENGINE_CORE_MATH_QUATERNION_H_

#include <ICEngine/ICEngine.h>
#include <ICEngine/Core/Math/Vector3.h>

namespace ICEngine
{
	//-------------------------------------------------------
	/// A generic quaternion class for respresention 3D 
	/// orientations.
	//-------------------------------------------------------
	template <typename TType> class GenericQuaternion final
	{
	public:
		//-----------------------------------------------
		/// Constructor
		//-----------------------------------------------
		GenericQuaternion();
		//-----------------------------------------------
		/// Constructor
		/// @param X
		/// @param Y
		/// @param Z
		/// @param W
		//-----------------------------------------------
		explicit GenericQuaternion(TType inX, TType inY, TType inZ, TType inW);
		//-----------------------------------------------
		/// Constructor
		/// @param An axis.
		/// @param An angle.
		//-----------------------------------------------
		explicit GenericQuaternion(const GenericVector3<TType>& inAxis, TType inAngle);
		//-----------------------------------------------
		/// @return the magnitude of the quaternion.
		//-----------------------------------------------
		TType Magnitude() const;
		//-----------------------------------------------
		/// @return the magnitude of the quaternion 
		/// squared.
		//-----------------------------------------------
		TType MagnitudeSquared() const;
		//-----------------------------------------------
		/// Normalises this quaternion.
		//-----------------------------------------------
		void Normalise();
		//-----------------------------------------------
		/// @return a normalised version of the quaternion.
		//-----------------------------------------------
		GenericQuaternion<TType> NormaliseCopy() const;
		//-----------------------------------------------
		/// Sets this quaternion to its conjugate.
		//-----------------------------------------------
		void Conjugate();
		//-----------------------------------------------
		/// @return the conjugate of the quaternion.
		//-----------------------------------------------
		GenericQuaternion<TType> ConjugateCopy() const;
		//-----------------------------------------------
		/// Sets this quaternion to its inverse.
		//-----------------------------------------------
		void Inverese();
		//-----------------------------------------------
		/// @return the inverse of the quaternion.
		//-----------------------------------------------
		GenericQuaternion<TType> InvereseCopy() const;
		//--------------------------------------------
		/// @param The first quaternion.
		/// @param The second quaternion.
		/// @return The dot product of two quaternions. 
		//--------------------------------------------
		static F32 Dot(const GenericQuaternion<TType>& inA, const GenericQuaternion<TType>& inB);
		//--------------------------------------------
		/// Spherical Linear Interpolation on two
		/// quaternions.
		/// @param The step between the two values. This
		/// is in the range 0.0 - 1.0 and will be clamped
		/// if outside of it.
		/// @param The first quaternion.
		/// @param The second quaternion.
		/// @return The interpolated quaternion.
		//--------------------------------------------
		static GenericQuaternion<TType> Slerp(const GenericQuaternion<TType>& inA, const GenericQuaternion<TType>& inB, F32 inT);
		//-----------------------------------------------
		/// Overloaded operators
		//-----------------------------------------------
		Bool operator==(const GenericQuaternion<TType>& inB);
		Bool operator!=(const GenericQuaternion<TType>& inB);
		GenericQuaternion<TType>& operator+=(const GenericQuaternion<TType>& inB);
		GenericQuaternion<TType>& operator-=(const GenericQuaternion<TType>& inB);
		GenericQuaternion<TType>& operator*=(const GenericQuaternion<TType>& inB);
		GenericQuaternion<TType>& operator*=(TType inB);
		GenericQuaternion<TType>& operator/=(TType inB);

		TType X;
		TType Y;
		TType Z;
		TType W;
	};
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType>::GenericQuaternion()
		: X(0), Y(0), Z(0), W(1)
	{
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType>::GenericQuaternion(TType inX, TType inY, TType inZ, TType inW)
		: X(inX), Y(inY), Z(inZ), W(inW)
	{
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType>::GenericQuaternion(const GenericVector3<TType>& inAxis, TType inAngle)
		: X(0), Y(0), Z(0), W(0)
	{
		GenericVector3<TType> normalised = inAxis.NormaliseCopy();
		TType halfAngle = inAngle / 2;
		TType sinAngle = (TType)sin(halfAngle);

		X = normalised.X * sinAngle;
		Y = normalised.Y * sinAngle;
		Z = normalised.Z * sinAngle;
		W = (TType)cos(halfAngle);
	}
	//-----------------------------------------------
	template <typename TType> TType GenericQuaternion<TType>::Magnitude() const
	{
		return (TType)sqrt(W*W + X*X + Y*Y + Z*Z);
	}
	//-----------------------------------------------
	template <typename TType> TType GenericQuaternion<TType>::MagnitudeSquared() const
	{
		return (W*W + X*X + Y*Y + Z*Z);
	}
	//-----------------------------------------------
	template <typename TType> void GenericQuaternion<TType>::Normalise()
	{
		float magnitude = Magnitude();
		W = W / magnitude;
		X = X / magnitude;
		Y = Y / magnitude;
		Z = Z / magnitude;
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::NormaliseCopy() const
	{
		GenericQuaternion<TType> output = *this;
		output.Normalise();
		return output;
	}
	//-----------------------------------------------
	template <typename TType> void GenericQuaternion<TType>::Conjugate()
	{
		W = W;
		X = -X;
		Y = -Y;
		Z = -Z;
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::ConjugateCopy() const
	{
		GenericQuaternion<TType> output = *this;
		output.Conjugate();
		return output;
	}
	//-----------------------------------------------
	template <typename TType> void GenericQuaternion<TType>::Inverese()
	{
		TType magnitudeSquared = MagnitudeSquared();
		Conjugate();
		W /= magnitudeSquared;
		X /= magnitudeSquared;
		Y /= magnitudeSquared;
		Z /= magnitudeSquared;
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::InvereseCopy() const
	{
		GenericQuaternion<TType> output = *this;
		output.Inverese();
		return output;
	}
	//--------------------------------------------
	template <typename TType> F32 GenericQuaternion<TType>::Dot(const GenericQuaternion<TType>& inA, const GenericQuaternion<TType>& inB)
	{
		return (inA.X * inB.X + inA.Y * inB.Y + inA.Z * inB.Z + inA.W * inB.W);
	}
	//--------------------------------------------
	template <typename TType> GenericQuaternion<TType> GenericQuaternion<TType>::Slerp(const GenericQuaternion<TType>& inA, const GenericQuaternion<TType>& inB, F32 inT)
	{
		const TType kEpsilon = (TType)0.0001;

		if (inT <= 0)
		{
			return inA;
		}
		if (inT >= 1)
		{
			return inB;
		}

		GenericQuaternion<TType> B = inB;
		TType aDotB = Dot(inA, B);
		if (aDotB < 0)
		{
			B.X = -B.X;
			B.Y = -B.Y;
			B.Z = -B.Z;
			B.W = -B.W;
			aDotB = -aDotB;
		}

		if (aDotB > 1 - kEpsilon)
		{
			GenericQuaternion<TType> output;
			output = inA + inT * (B - inA);
			output.Normalise();
			return output;
		}

		TType acosADotB = acos(aDotB);
		return (sin((1 - inT) * acosADotB) * inA + sin(inT * acosADotB) * B) / sin(acosADotB);
	}
	//-----------------------------------------------
	template <typename TType> Bool GenericQuaternion<TType>::operator==(const GenericQuaternion<TType>& inB)
	{
		if (X != inB.X || Y != inB.Y || Z != inB.Z || W != inB.W)
		{
			return false;
		}
		return true;
	}
	//-----------------------------------------------
	template <typename TType> Bool GenericQuaternion<TType>::operator!=(const GenericQuaternion<TType>& inB)
	{
		return !(*this == inB);
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator+=(const GenericQuaternion<TType>& inB)
	{
		X += inB.X;
		Y += inB.Y;
		Z += inB.Z;
		W += inB.W;
		return *this;
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator-=(const GenericQuaternion<TType>& inB)
	{
		X -= inB.X;
		Y -= inB.Y;
		Z -= inB.Z;
		W -= inB.W;
		return *this;
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator *= (const GenericQuaternion<TType>& inB)
	{
		GenericQuaternion<TType> copy = *this;
		W = inB.W * copy.W - inB.X *copy.X - inB.Y * copy.Y - inB.Z * copy.Z;
		X = inB.W * copy.X + inB.X *copy.W + inB.Y * copy.Z - inB.Z * copy.Y;
		Y = inB.W * copy.Y - inB.X *copy.Z + inB.Y * copy.W + inB.Z * copy.X;
		Z = inB.W * copy.Z + inB.X *copy.Y - inB.Y * copy.X + inB.Z * copy.W;
		return *this;
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator*=(TType inB)
	{
		X *= inB;
		Y *= inB;
		Z *= inB;
		W *= inB;
		return *this;
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType>& GenericQuaternion<TType>::operator/=(TType inB)
	{
		X /= inB;
		Y /= inB;
		Z /= inB;
		W /= inB;
		return *this;
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> operator+(GenericQuaternion<TType> inA, const GenericQuaternion<TType>& inB)
	{
		return (inA += inB);
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> operator-(GenericQuaternion<TType> inA, const GenericQuaternion<TType>& inB)
	{
		return (inA -= inB);
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> operator*(GenericQuaternion<TType> inA, const GenericQuaternion<TType>& inB)
	{
		return (inA *= inB);
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> operator*(GenericQuaternion<TType> inA, TType inB)
	{
		return (inA *= inB);
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> operator*(TType inA, GenericQuaternion<TType> inB)
	{
		return (inB *= inA);
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> operator/(GenericQuaternion<TType> inA, TType inB)
	{
		return (inA /= inB);
	}
	//-----------------------------------------------
	template <typename TType> GenericQuaternion<TType> operator/(TType inA, GenericQuaternion<TType> inB)
	{
		return (inB /= inA);
	}
}

#endif