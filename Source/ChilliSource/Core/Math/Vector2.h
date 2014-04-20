//
//  Vector2.h
//  Chilli Source
//  Created by I Copland on 20/04/2014.
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

#ifndef _CHILLISOURCE_CORE_MATH_VECTOR2_H_
#define _CHILLISOURCE_CORE_MATH_VECTOR2_H_

#include <ChilliSource/ChilliSource.h>

#include <math.h>

namespace ICEngine
{
	//-----------------------------------------------------------
	/// A generic two dimensional mathematical vector. Typically
	/// the type specific typedefs of this class would be used
	/// over direct use. For example:
	///		Vector2 -> GenericVector2<f32>
	///		Float2 -> GenericVector2<f32>
	///		Integer2 -> GenericVector2<s32>
	///
	/// @author I Copland
	//-----------------------------------------------------------
	template <typename TType> class GenericVector2 final
	{
	public:
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param a vector.
		/// @param another vector.
		///
		/// @return A vector containing the minimum value for
		/// each component in each vector.
		//-----------------------------------------------------
		static GenericVector2<TType> Min(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param a vector.
		/// @param another vector.
		///
		/// @return A vector containing the maximum value for
		/// each component in each vector.
		//-----------------------------------------------------
		static GenericVector2<TType> Max(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param A vector.
		/// @param The minimum vector.
		/// @param The maximum vector.
		///
		/// @return A vector in which each component is clamped
		/// between the equivalent in the min and max vectors.
		//-----------------------------------------------------
		static GenericVector2<TType> Clamp(const GenericVector2<TType>& in_value, const GenericVector2<TType>& in_min, const GenericVector2<TType>& in_max);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param a vector.
		/// @param another vector.
		///
		/// @return The lerped vector.
		//-----------------------------------------------------
		static GenericVector2<TType> Lerp(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b, f32 in_t);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param a vector.
		/// @param another vector.
		///
		/// @return the dot product of these two vectors.
		//-----------------------------------------------------
		static TType DotProduct(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// Treats the two vectors as 3D with the z-component
		/// set to zero to calculate the z component of the
		/// cross product. 
		///
		/// @author I Copland
		///
		/// @param  A vector.
		/// @param  Another vector.
		///
		/// @return The z component of the cross product.
		//-----------------------------------------------------
		static TType CrossProductZ(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param  A vector.
		/// @param  Another vector.
		///
		/// @return The angle between the two vectors in 
		/// radians.
		//-----------------------------------------------------
		static TType Angle(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// Constructor
		///
		/// @author I Copland
		//-----------------------------------------------------
		GenericVector2();
		//-----------------------------------------------------
		/// Constructor
		///
		/// @author I Copland
		/// 
		/// @param the x component.
		/// @param the y component.
		//-----------------------------------------------------
		explicit GenericVector2(TType in_x, TType in_y);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @return the length of the vector.
		//-----------------------------------------------------
		TType Length() const;
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @return the length of the vector squared
		//-----------------------------------------------------
		TType LengthSquared() const;
		//-----------------------------------------------------
		/// Normalises the contents of this vector.
		///
		/// @author I Copland
		//-----------------------------------------------------
		void Normalise();		
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @return a normalised copy of the vector.
		//-----------------------------------------------------
		GenericVector2<TType> NormaliseCopy() const;
		//-----------------------------------------------------
		/// Sets this contents of this vector to its inverse.
		///
		/// @author I Copland
		//-----------------------------------------------------
		void Inverse();
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @return An inversed copy of this vector.
		//-----------------------------------------------------
		GenericVector2<TType> InverseCopy() const;
		//-----------------------------------------------------
		/// Sets this vector to it's absolute value.
		///
		/// @author I Copland
		//-----------------------------------------------------
		void Abs();
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @return an absolute copy of the vector.
		//-----------------------------------------------------
		GenericVector2<TType> AbsCopy() const;
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param Another vector.
		///
		/// @return Whether or not the vectors are equal.
		//-----------------------------------------------------
		bool operator==(const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param Another vector.
		///
		/// @return Whether or not the vectors are different.
		//-----------------------------------------------------
		bool operator!=(const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param Another vector.
		///
		/// @return This vector after having the given vector
		/// added to it.
		//-----------------------------------------------------
		GenericVector2<TType>& operator+=(const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param Another vector.
		///
		/// @return This vector after having the given vector
		/// subtracted from it.
		//-----------------------------------------------------
		GenericVector2<TType>& operator-=(const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param Another vector.
		///
		/// @return This vector after being multiplied by the 
		/// given vector.
		//-----------------------------------------------------
		GenericVector2<TType>& operator*=(const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param Another vector.
		///
		/// @return This vector after being divided by the 
		/// given vector.
		//-----------------------------------------------------
		GenericVector2<TType>& operator/=(const GenericVector2<TType>& in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param Another vector.
		///
		/// @return This vector after being multiplied by the 
		/// given scalar.
		//-----------------------------------------------------
		GenericVector2<TType>& operator*=(TType in_b);
		//-----------------------------------------------------
		/// @author I Copland
		///
		/// @param Another vector.
		///
		/// @return This vector after being divided by the 
		/// given scalar.
		//-----------------------------------------------------
		GenericVector2<TType>& operator/=(TType in_b);

		TType x;
		TType y;
	};
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector2<TType>::Min(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b)
	{
		GenericVector2<TType> output;
		output.x = std::min(in_a.x, in_b.x);
		output.y = std::min(in_a.y, in_b.y);
		return output;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector2<TType>::Max(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b)
	{
		GenericVector2<TType> output;
		output.x = std::max(in_a.x, in_b.x);
		output.y = std::max(in_a.y, in_b.y);
		return output;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector2<TType>::Clamp(const GenericVector2<TType>& in_value, const GenericVector2<TType>& in_min, const GenericVector2<TType>& in_max)
	{
		GenericVector2<TType> output;
		output.x = std::min(std::max(in_value.x, in_min.x), in_max.x);
		output.y = std::min(std::max(in_value.y, in_min.y), in_max.y);
		return output;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector2<TType>::Lerp(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b, F32 in_t)
	{
		if (in_t <= 0.0f)
		{
			return in_a;
		}
		else if (in_t >= 1.0f)
		{
			return in_b;
		}

		return (in_a + in_t * (in_b - in_a));
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> TType GenericVector2<TType>::DotProduct(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b)
	{
		return (in_a.x * in_b.x + in_a.y * in_b.y);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> TType GenericVector2<TType>::CrossProductZ(const GenericVector2<TType> & in_a, const GenericVector2<TType> & in_b)
	{
		return (in_a.x * in_b.y - in_a.y * in_b.x);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> TType GenericVector2<TType>::Angle(const GenericVector2<TType>& in_a, const GenericVector2<TType>& in_b)
	{
		return std::atan2(in_b.y, in_b.x) - std::atan2(in_a.y, in_a.x);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType>::GenericVector2()
		: x(0), y(0)
	{
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType>::GenericVector2(TType in_x, TType in_y)
		: x(in_x), y(in_y)
	{
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> TType GenericVector2<TType>::LengthSquared() const
	{
		return (x * x + y * y);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> TType GenericVector2<TType>::Length() const
	{
		return (sqrt(x * x + y * y));
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> void GenericVector2<TType>::Normalise()
	{
		TType length = Length();
		x = x / length;
		y = y / length;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector2<TType>::NormaliseCopy() const
	{
		GenericVector2<TType> copy = *this;
		copy.Normalise();
		return copy;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> void GenericVector2<TType>::Inverse()
	{
		if (x != 0)
		{
			x = 1 / x;
		}

		if (y != 0)
		{
			y = 1 / y;
		}
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector2<TType>::InverseCopy() const
	{
		GenericVector2<TType> copy = *this;
		copy.Inverse();
		return copy;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> void GenericVector2<TType>::Abs()
	{
		x = std::abs(x);
		y = std::abs(y);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> GenericVector2<TType>::AbsCopy() const
	{
		GenericVector2<TType> copy = *this;
		copy.Abs();
		return copy;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> bool GenericVector2<TType>::operator==(const GenericVector2<TType>& in_b)
	{
		if (x == in_b.x && y == in_b.y)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> bool GenericVector2<TType>::operator!=(const GenericVector2<TType>& in_b)
	{
		return !(*this == in_b);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator+=(const GenericVector2<TType>& in_b)
	{
		x += in_b.x;
		y += in_b.y;
		return *this;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator-=(const GenericVector2<TType>& in_b)
	{
		x -= in_b.x;
		y -= in_b.y;
		return *this;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator*=(const GenericVector2<TType>& in_b)
	{
		x *= in_b.x;
		y *= in_b.y;
		return *this;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator/=(const GenericVector2<TType>& in_b)
	{
		x /= in_b.x;
		y /= in_b.y;
		return *this;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator*=(TType in_b)
	{
		x *= in_b;
		y *= in_b;
		return *this;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType>& GenericVector2<TType>::operator/=(TType in_b)
	{
		x /= in_b;
		y /= in_b;
		return *this;
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> operator+(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
	{
		return (in_a += in_b);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> operator-(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
	{
		return (in_a -= in_b);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> operator*(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
	{
		return (in_a *= in_b);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> operator/(GenericVector2<TType> in_a, const GenericVector2<TType>& in_b)
	{
		return (in_a /= in_b);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> operator*(GenericVector2<TType> in_a, TType in_b)
	{
		return (in_a *= in_b);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> operator*(TType in_a, GenericVector2<TType> in_b)
	{
		return (in_b *= in_a);
	}
	//-----------------------------------------------------
	//-----------------------------------------------------
	template <typename TType> GenericVector2<TType> operator/(GenericVector2<TType> in_a, TType in_b)
	{
		return (in_a /= in_b);
	}
}

#endif
