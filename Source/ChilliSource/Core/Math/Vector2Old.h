#ifndef _MO_FLO_CORE_VECTOR_2_H_
#define _MO_FLO_CORE_VECTOR_2_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector4Old.h>

#include <math.h>

//=================================================
// Tag - Vector 2
//
// 2-Dimensional Vector
//=================================================

namespace ChilliSource 
{
	namespace Core 
	{
		class Vector2Old
		{
		public:
			//---------------------------------
			// Constructors & Destructor
			//---------------------------------
			Vector2Old(void);
			Vector2Old(const Vector2Old &Vec);
			explicit Vector2Old(const Vector3Old &Vec);
			explicit Vector2Old(const Vector4Old &Vec);
			Vector2Old(const f32 fX, const f32 fY);

			//---------------------------------
			// Operators
			//---------------------------------
			Vector2Old& operator=(const Vector2Old &Vec){x = Vec.x; y = Vec.y; return *this;};

			Vector2Old operator+(const Vector2Old &Vec) const
			{Vector2Old Result; Result.x = this->x + Vec.x; Result.y = this->y + Vec.y; return Result;}

			Vector2Old operator+(const f32 Scale) const
			{Vector2Old Result; Result.x = this->x + Scale; Result.y = this->y + Scale; return Result;}

			Vector2Old operator-(const Vector2Old &Vec) const
			{Vector2Old Result; Result.x = this->x - Vec.x; Result.y = this->y - Vec.y; return Result;}

			Vector2Old operator-() const
			{Vector2Old Result; Result.x = -this->x; Result.y = -this->y; return Result;}
	
			Vector2Old operator-(const f32 Scale) const
			{Vector2Old Result; Result.x = this->x - Scale; Result.y = this->y - Scale; return Result;}

			Vector2Old operator*(const Vector2Old &Vec) const
			{Vector2Old Result; Result.x = this->x * Vec.x; Result.y = this->y * Vec.y; return Result;}

			Vector2Old operator*(const f32 Scale) const
			{Vector2Old Result; Result.x = this->x * Scale; Result.y = this->y * Scale; return Result;}
			
			Vector2Old operator/(const Vector2Old &Vec) const
			{Vector2Old Result; Result.x = this->x / Vec.x; Result.y = this->y / Vec.y; return Result;}
			
			Vector2Old operator/(const f32 Scale) const
			{Vector2Old Result; Result.x = this->x / Scale; Result.y = this->y / Scale; return Result;}

			const Vector2Old& operator+=(const Vector2Old &Vec)
            {this->x += Vec.x; this->y += Vec.y; return *this;}

			const Vector2Old& operator-=(const Vector2Old &Vec)
            {this->x -= Vec.x; this->y -= Vec.y; return *this;}

			const Vector2Old& operator*=(const Vector2Old &Vec)
            {this->x *= Vec.x; this->y *= Vec.y; return *this;}
			
			const Vector2Old& operator/=(const Vector2Old &Vec)
            {this->x /= Vec.x; this->y /= Vec.y; return *this;}

			const Vector2Old& operator+=(const f32 Scale)
            {this->x += Scale; this->y += Scale; return *this;}

			const Vector2Old& operator-=(const f32 Scale)
            {this->x -= Scale; this->y -= Scale; return *this;}

			const Vector2Old& operator*=(const f32 Scale)
            {this->x *= Scale; this->y *= Scale; return *this;}
			
			const Vector2Old& operator/=(const f32 Scale)
            {this->x /= Scale; this->y /= Scale; return *this;}

			bool operator==(const Vector2Old &Vec) const
            {return (Vec.x == x && Vec.y == y);}

			bool operator!=(const Vector2Old &Vec) const
            {return (Vec.x != x || Vec.y != y);}

			bool operator>(const Vector2Old &Vec) const
            {return x > Vec.x && y > Vec.y;}

			bool operator<(const Vector2Old &Vec) const
            {return x < Vec.x && y < Vec.y;}

			bool operator>=(const Vector2Old &Vec) const
            {return x >= Vec.x && y >= Vec.y;}

			bool operator<=(const Vector2Old &Vec) const
            {return x <= Vec.x && y <= Vec.y;}

			//---------------------------------
			// Methods
			//---------------------------------
			Vector2Old & Normalise();
			Vector2Old NormalisedCopy() const;
            Vector2Old & Inverse();
            Vector2Old InversedCopy() const;
			f32 DotProduct(const Vector2Old &inOtherVec) const;
			f32 Length() const;
			f32 LengthSquared() const;
			f32 Angle(const Vector2Old &inOtherVec) const;
            
            static void Multiply(const Vector2Old* inpVec, const Vector2Old* inpVec2, Vector2Old* outpVec);

			static const Vector2Old ZERO;
			static const Vector2Old ONE;
			static const Vector2Old X_UNIT_POSITIVE;
			static const Vector2Old X_UNIT_NEGATIVE;
			static const Vector2Old Y_UNIT_POSITIVE;
			static const Vector2Old Y_UNIT_NEGATIVE;
		public:

			f32 x, y;
		};
		
		inline Vector2Old operator*(const f32 Scale, const Vector2Old & inVector) 
		{Vector2Old Result; Result.x = inVector.x * Scale; Result.y = inVector.y * Scale; return Result;}
	}
}

#endif