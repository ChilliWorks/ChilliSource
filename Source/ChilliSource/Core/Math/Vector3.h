// GenericVector3.h
// ICEngine
//
// Created by Ian Copland on 20/04/2012
// Copyright Ian Copland 2012. All rights reserved.

#ifndef _ICENGINE_CORE_MATH_VECTOR3_H_
#define _ICENGINE_CORE_MATH_VECTOR3_H_

#include <ICEngine/ICEngine.h>
#include <ICEngine/Core/Math/Vector2.h>
#include <math.h>

namespace ICEngine
{
	//-------------------------------------------------------------------
	/// A three dimensional vector of any type. Typically this would not
	/// be used directly, rather using the specfic typedefs. For example:
	///		Vector3 -> GenericVector3<F32>
	///		Float3 -> GenericVector3<F32>
	///		Integer3 -> GenericVector3<S32>
	//-------------------------------------------------------------------
	template <typename TType> class GenericVector3 final
	{
	public:
		//-----------------------------------------------------
		/// Constructor
		//-----------------------------------------------------
		GenericVector3();
		//-----------------------------------------------------
		/// Constructor
		/// @param the x component.
		/// @param the y component.
		/// @param the z component.
		//-----------------------------------------------------
		explicit GenericVector3(TType infX, TType infY, TType infZ);
		//-----------------------------------------------------
		/// Constructor
		/// @param A vector 2 of the same type.
		/// @param the z component.
		//-----------------------------------------------------
		explicit GenericVector3(const GenericVector2<TType>& inXY, TType inZ);
		//-----------------------------------------------------
		/// @return A copy of the XY components of this vector
		/// as a Vector2.
		//-----------------------------------------------------
		GenericVector2<TType> XY() const;
		//-----------------------------------------------------
		/// @return the length of the vector
		//-----------------------------------------------------
		TType Length() const;
		//-----------------------------------------------------
		/// @return the length of the vector squared
		//-----------------------------------------------------
		TType LengthSquared() const;
		//-----------------------------------------------------
		/// Normalises the contents of this vector.
		//-----------------------------------------------------
		void Normalise();
		//-----------------------------------------------------
		/// @return a normalised copy of the vector.
		//-----------------------------------------------------
		GenericVector3<TType> NormaliseCopy() const;
		//-----------------------------------------------------
		/// Sets this vector to it's absolute value.
		//-----------------------------------------------------
		void Abs();
		//-----------------------------------------------------
		/// @return an absolute copy of the vector.
		//-----------------------------------------------------
		GenericVector3<TType> AbsCopy() const;
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return A vector containing the minimum value for
		/// each component in each vector.
		//-----------------------------------------------------
		static GenericVector3<TType> Min(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB);
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return A vector containing the maximum value for
		/// each component in each vector.
		//-----------------------------------------------------
		static GenericVector3<TType> Max(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB);
		//-----------------------------------------------------
		/// @param A vector.
		/// @param A minimum vector.
		/// @param A maximum vector.
		/// @return A vector in which each component is clamped
		/// between the equivalent in the min a max vectors.
		//-----------------------------------------------------
		static GenericVector3<TType> Clamp(const GenericVector3<TType>& inValue, const GenericVector3<TType>& inMin, const GenericVector3<TType>& inMax);
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return The lerped vector.
		//-----------------------------------------------------
		static GenericVector3<TType> Lerp(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB, F32 inT);
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return the dot product of these two vectors.
		//-----------------------------------------------------
		static TType DotProduct(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB);
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return the cross product of these two vectors,
		///	as if they were vector3s.
		//-----------------------------------------------------
		static GenericVector3<TType> CrossProduct(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB);
		//-----------------------------------------------------
		/// Rotates the vector by the given quaternion.
		/// @param The quaternion.
		/// @param The vector.
		/// @return The new vector.
		//-----------------------------------------------------
		static GenericVector3<TType> Rotate(const Quaternion& inA, const GenericVector3<TType>& inB);
		//-----------------------------------------------------
		/// overloaded operators
		//-----------------------------------------------------
		Bool operator == (const GenericVector3<TType>& inB);
		Bool operator != (const GenericVector3<TType>& inB);
		GenericVector3<TType>& operator+=(const GenericVector3<TType>& inB);
		GenericVector3<TType>& operator-=(const GenericVector3<TType>& inB);
		GenericVector3<TType>& operator*=(const GenericVector3<TType>& inB);
		GenericVector3<TType>& operator/=(const GenericVector3<TType>& inB);
		GenericVector3<TType>& operator*=(TType inB);
		GenericVector3<TType>& operator/=(TType inB);
		//-----------------------------------------------------
		/// Components
		//-----------------------------------------------------
		TType X;
		TType Y;
		TType Z;
	};
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>::GenericVector3()
		: X(0), Y(0), Z(0)
	{
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>::GenericVector3(TType infX, TType infY, TType infZ)
		: X(infX), Y(infY), Z(infZ)
	{
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>::GenericVector3(const GenericVector2<TType>& inXY, TType inZ)
		: X(inXY.X), Y(inXY.Y), Z(inZ)
	{
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector3<TType>::XY() const
	{
		return GenericVector2<TType>(X, Y);
	}
	//-----------------------------------------------------
	template <typename TType> TType GenericVector3<TType>::LengthSquared() const
	{
		return (X * X + Y * Y + Z * Z);
	}
	//-----------------------------------------------------
	template <typename TType> TType GenericVector3<TType>::Length() const
	{
		return (sqrt(X * X + Y * Y + Z * Z));
	}
	//-----------------------------------------------------
	template <typename TType> void GenericVector3<TType>::Normalise()
	{
		TType length = Length();
		X = X / length;
		Y = Y / length;
		Z = Z / length;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector3<TType>::NormaliseCopy() const
	{
		GenericVector3<TType> copy = *this;
		copy.Normalise();
		return copy;
	}
	//-----------------------------------------------------
	template <typename TType> void GenericVector3<TType>::Abs()
	{
		X = std::abs(X);
		Y = std::abs(Y);
		Z = std::abs(Z);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector3<TType>::AbsCopy() const
	{
		GenericVector3<TType> copy = *this;
		copy.AbsSelf();
		return copy;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector3<TType>::Min(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> output;
		output.X = std::min(inA.X, inB.X);
		output.Y = std::min(inA.Y, inB.Y);
		output.Z = std::min(inA.Z, inB.Z);
		return output;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector3<TType>::Max(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> output;
		output.X = std::max(inA.X, inB.X);
		output.Y = std::max(inA.Y, inB.Y);
		output.Z = std::max(inA.Z, inB.Z);
		return output;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector3<TType>::Clamp(const GenericVector3<TType>& inValue, const GenericVector3<TType>& inMin, const GenericVector3<TType>& inMax)
	{
		GenericVector3<TType> output;
		output.X = std::min(std::max(inValue.X, inMin.X), inMax.X);
		output.Y = std::min(std::max(inValue.Y, inMin.Y), inMax.Y);
		output.Z = std::min(std::max(inValue.Z, inMin.Z), inMax.Z);
		return output;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector3<TType>::Lerp(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB, F32 inT)
	{
		if (inT <= 0.0f)
		{
			return inA;
		}
		else if (inT >= 1.0f)
		{
			return inB;
		}

		return (inA + inT * (inB - inA));
	}
	//-----------------------------------------------------
	template <typename TType> TType GenericVector3<TType>::DotProduct(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB)
	{
		return (inA.X * inB.X + inA.Y * inB.Y + inA.Z * inB.Z);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector3<TType>::CrossProduct(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> C;
		C.X = inA.Y * inB.Z - inA.Z * inB.Y;
		C.Y = -inA.X * inB.Z + inA.Z * inB.X;
		C.Z = inA.X * inB.Y - inA.Y * inB.X;
		return C;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector3<TType>::Rotate(const Quaternion& inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> aXyz(inA.X, inA.Y, inA.Z);
		GenericVector3<TType> t = static_cast<TType>(2) * CrossProduct(aXyz, inB);
		return inB + inA.W * t + CrossProduct(aXyz, t);
	}
	//-----------------------------------------------------
	template <typename TType> Bool GenericVector3<TType>::operator==(const GenericVector3<TType>& inB)
	{
		if (X == inB.X && Y == inB.Y && Z == inB.Z)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------
	template <typename TType> Bool GenericVector3<TType>::operator!=(const GenericVector3<TType>& inB)
	{
		return !(*this == inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator+=(const GenericVector3<TType>& inB)
	{
		X += inB.X;
		Y += inB.Y;
		Z += inB.Z;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator-=(const GenericVector3<TType>& inB)
	{
		X -= inB.X;
		Y -= inB.Y;
		Z -= inB.Z;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator*=(const GenericVector3<TType>& inB)
	{
		X *= inB.X;
		Y *= inB.Y;
		Z *= inB.Z;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator/=(const GenericVector3<TType>& inB)
	{
		X /= inB.X;
		Y /= inB.Y;
		Z /= inB.Z;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator*=(TType inB)
	{
		X *= inB;
		Y *= inB;
		Z *= inB;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType>& GenericVector3<TType>::operator/=(TType inB)
	{
		X /= inB;
		Y /= inB;
		Z /= inB;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> operator+(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> copy = inA;
		return (copy += inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> operator-(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> copy = inA;
		return (copy -= inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> copy = inA;
		return (copy *= inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> operator/(const GenericVector3<TType>& inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> copy = inA;
		return (copy /= inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> operator*(const GenericVector3<TType>& inA, TType inB)
	{
		GenericVector3<TType> copy = inA;
		return (copy *= inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> operator*(TType inA, const GenericVector3<TType>& inB)
	{
		GenericVector3<TType> copy = inB;
		return (copy *= inA);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> operator/(const GenericVector3<TType>& inA, TType inB)
	{
		GenericVector3<TType> copy = inA;
		return (copy /= inB);
	}
}

#endif
