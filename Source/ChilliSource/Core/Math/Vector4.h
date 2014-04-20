// GenericVector4.h
// ICEngine
//
// Created by Ian Copland on 20/04/2012
// Copyright Ian Copland 2012. All rights reserved.

#ifndef _ICENGINE_CORE_MATH_VECTOR4_H_
#define _ICENGINE_CORE_MATH_VECTOR4_H_

#include <ICEngine/ICEngine.h>
#include <ICEngine/Core/Math/Vector2.h>
#include <ICEngine/Core/Math/Vector3.h>
#include <math.h>

namespace ICEngine
{
	//-------------------------------------------------------------------
	/// A four dimensional vector of any type. Typically this would not
	/// be used directly, rather using the specfic typedefs. For example:
	///		Vector4 -> GenericVector4<F32>
	///		Float4 -> GenericVector4<F32>
	///		Integer4 -> GenericVector4<S32>
	//-------------------------------------------------------------------
	template <typename TType> class GenericVector4 final
	{
	public:	
		//-----------------------------------------------------
		/// Constructor
		//-----------------------------------------------------
		GenericVector4();
		//-----------------------------------------------------
		/// Constructor
		/// @param the x component.
		/// @param the y component.
		/// @param the z component.
		/// @param the w component.
		//-----------------------------------------------------
		explicit GenericVector4(TType infX, TType infY, TType infZ, TType infW);
		//-----------------------------------------------------
		/// Constructor
		/// @param A vector2.
		/// @param the z component.
		/// @param the w component.
		//-----------------------------------------------------
		explicit GenericVector4(const GenericVector2<TType>& inXY, TType inZ, TType inW);
		//-----------------------------------------------------
		/// Constructor
		/// @param A vector2 representing the X and Y component
		/// @param A vector2 representing the Z and W component
		//-----------------------------------------------------
		explicit GenericVector4(const GenericVector2<TType>& inXY, const GenericVector2<TType>& inZW);
		//-----------------------------------------------------
		/// Constructor
		/// @param A vector3 representing the X, Y and Z components
		/// @param The w component.
		//-----------------------------------------------------
		explicit GenericVector4(const GenericVector3<TType>& inXYZ, TType inW);
		//-----------------------------------------------------
		/// @return A copy of the XY components of this vector
		/// as a Vector2.
		//-----------------------------------------------------
		GenericVector2<TType> XY() const;
		//-----------------------------------------------------
		/// @return A copy of the ZW components of this vector
		/// as a Vector2.
		//-----------------------------------------------------
		GenericVector2<TType> ZW() const;
		//-----------------------------------------------------
		/// @return A copy of the XYZ components of this vector
		/// as a Vector3.
		//-----------------------------------------------------
		GenericVector3<TType> XYZ() const;
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
		GenericVector4<TType> NormaliseCopy() const;
		//-----------------------------------------------------
		/// Sets this vector to it's absolute value.
		//-----------------------------------------------------
		void Abs();
		//-----------------------------------------------------
		/// @return an absolute copy of the vector.
		//-----------------------------------------------------
		GenericVector4<TType> AbsCopy() const;
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return A vector containing the minimum value for
		/// each component in each vector.
		//-----------------------------------------------------
		static GenericVector4<TType> Min(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB);
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return A vector containing the maximum value for
		/// each component in each vector.
		//-----------------------------------------------------
		static GenericVector4<TType> Max(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB);
		//-----------------------------------------------------
		/// @param A vector.
		/// @param A minimum vector.
		/// @param A maximum vector.
		/// @return A vector in which each component is clamped
		/// between the equivalent in the min a max vectors.
		//-----------------------------------------------------
		static GenericVector4<TType> Clamp(const GenericVector4<TType>& inValue, const GenericVector4<TType>& inMin, const GenericVector4<TType>& inMax);
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return The lerped vector.
		//-----------------------------------------------------
		static GenericVector4<TType> Lerp(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB, F32 inT);
		//-----------------------------------------------------
		/// @param a vector.
		/// @param another vector.
		/// @return the dot product of these two vectors.
		//-----------------------------------------------------
		static TType DotProduct(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB);
		//-----------------------------------------------------
		/// overloaded operators
		//-----------------------------------------------------
		Bool operator == (const GenericVector4<TType>& inB);
		Bool operator != (const GenericVector4<TType>& inB);
		GenericVector4<TType>& operator+=(const GenericVector4<TType>& inB);
		GenericVector4<TType>& operator-=(const GenericVector4<TType>& inB);
		GenericVector4<TType>& operator*=(const GenericVector4<TType>& inB);
		GenericVector4<TType>& operator/=(const GenericVector4<TType>& inB);
		GenericVector4<TType>& operator*=(TType inB);
		GenericVector4<TType>& operator/=(TType inB);
		//-----------------------------------------------------
		/// Components
		//-----------------------------------------------------
		TType X;
		TType Y;
		TType Z;
		TType W;
	};
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>::GenericVector4()
		: X(0), Y(0), Z(0), W(0)
	{
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>::GenericVector4(TType infX, TType infY, TType infZ, TType infW)
		: X(infX), Y(infY), Z(infZ), W(infW)
	{
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>::GenericVector4(const GenericVector2<TType>& inXY, TType inZ, TType inW)
		: X(inXY.X), Y(inXY.Y), Z(inZ), W(inW)
	{
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>::GenericVector4(const GenericVector2<TType>& inXY, const GenericVector2<TType>& inZW)
		: X(inXY.X), Y(inXY.Y), Z(inZW.X), W(inZW.Y)
	{
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>::GenericVector4(const GenericVector3<TType>& inXYZ, TType inW)
		: X(inXYZ.X), Y(inXYZ.Y), Z(inXYZ.Z), W(inW)
	{
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector4<TType>::XY() const
	{
		return GenericVector2<TType>(X, Y);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector4<TType>::ZW() const
	{
		return GenericVector2<TType>(Z, W);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector3<TType> GenericVector4<TType>::XYZ() const
	{
		return GenericVector3<TType>(X, Y, Z);
	}
	//-----------------------------------------------------
	template <typename TType> TType GenericVector4<TType>::LengthSquared() const
	{
		return (X * X + Y * Y + Z * Z + W * W);
	}
	//-----------------------------------------------------
	template <typename TType> TType GenericVector4<TType>::Length() const
	{
		return (sqrt(X * X + Y * Y + Z * Z + W * W));
	}
	//-----------------------------------------------------
	template <typename TType> void GenericVector4<TType>::Normalise()
	{
		TType length = Length();
		X = X / length;
		Y = Y / length;
		Z = Z / length;
		W = W / length;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> GenericVector4<TType>::NormaliseCopy() const
	{
		GenericVector4<TType> copy = inValue;
		copy.NormaliseSelf();
		return copy;
	}
	//-----------------------------------------------------
	template <typename TType> void GenericVector4<TType>::Abs()
	{
		X = std::abs(X);
		Y = std::abs(Y);
		Z = std::abs(Z);
		W = std::abs(W);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> GenericVector4<TType>::AbsCopy() const
	{
		GenericVector4<TType> copy = inValue;
		copy.AbsSelf();
		return copy;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> GenericVector4<TType>::Min(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB)
	{
		GenericVector4<TType> output;
		output.X = std::min(inA.X, inB.X);
		output.Y = std::min(inA.Y, inB.Y);
		output.Z = std::min(inA.Z, inB.Z);
		output.W = std::min(inA.W, inB.W);
		return output;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> GenericVector4<TType>::Max(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB)
	{
		GenericVector4<TType> output;
		output.X = std::max(inA.X, inB.X);
		output.Y = std::max(inA.Y, inB.Y);
		output.Z = std::max(inA.Z, inB.Z);
		output.W = std::max(inA.W, inB.W);
		return output;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> GenericVector4<TType>::Clamp(const GenericVector4<TType>& inValue, const GenericVector4<TType>& inMin, const GenericVector4<TType>& inMax)
	{
		GenericVector4<TType> output;
		output.X = std::min(std::max(inValue.X, inMin.X), inMax.X);
		output.Y = std::min(std::max(inValue.Y, inMin.Y), inMax.Y);
		output.Z = std::min(std::max(inValue.Z, inMin.Z), inMax.Z);
		output.W = std::min(std::max(inValue.W, inMin.W), inMax.W);
		return output;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> GenericVector4<TType>::Lerp(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB, F32 inT)
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
	template <typename TType> TType GenericVector4<TType>::DotProduct(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB)
	{
		return (inA.X * inB.X + inA.Y * inB.Y + inA.Z * inB.Z + inA.W * inB.W);
	}
	//-----------------------------------------------------
	template <typename TType> Bool GenericVector4<TType>::operator==(const GenericVector4<TType>& inB)
	{
		if (X == inB.X && Y == inB.Y && Z == inB.Z && W == inB.W)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------
	template <typename TType> Bool GenericVector4<TType>::operator!=(const GenericVector4<TType>& inB)
	{
		return !(*this == inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator+=(const GenericVector4<TType>& inB)
	{
		X += inB.X;
		Y += inB.Y;
		Z += inB.Z;
		W += inB.W;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator-=(const GenericVector4<TType>& inB)
	{
		X -= inB.X;
		Y -= inB.Y;
		Z -= inB.Z;
		W -= inB.W;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator*=(const GenericVector4<TType>& inB)
	{
		X *= inB.X;
		Y *= inB.Y;
		Z *= inB.Z;
		W *= inB.W;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator/=(const GenericVector4<TType>& inB)
	{
		X /= inB.X;
		Y /= inB.Y;
		Z /= inB.Z;
		W /= inB.W;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator*=(TType inB)
	{
		X *= inB;
		Y *= inB;
		Z *= inB;
		W *= inB;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType>& GenericVector4<TType>::operator/=(TType inB)
	{
		X /= inB;
		Y /= inB;
		Z /= inB;
		W /= inB;
		return *this;
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> operator+(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB)
	{
		GenericVector4<TType> copy = inA;
		return (copy += inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> operator-(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB)
	{
		GenericVector4<TType> copy = inA;
		return (copy -= inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> operator*(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB)
	{
		GenericVector4<TType> copy = inA;
		return (copy *= inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> operator/(const GenericVector4<TType>& inA, const GenericVector4<TType>& inB)
	{
		GenericVector4<TType> copy = inA;
		return (copy /= inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> operator*(const GenericVector4<TType>& inA, TType inB)
	{
		GenericVector4<TType> copy = inA;
		return (copy *= inB);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> operator*(TType inA, const GenericVector4<TType>& inB)
	{
		GenericVector4<TType> copy = inB;
		return (copy *= inA);
	}
	//-----------------------------------------------------
	template <typename TType> GenericVector4<TType> operator/(const GenericVector4<TType>& inA, TType inB)
	{
		GenericVector4<TType> copy = inA;
		return (copy /= inB);
	}
}

#endif
