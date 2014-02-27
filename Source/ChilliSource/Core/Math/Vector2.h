#ifndef _MO_FLO_CORE_VECTOR_2_H_
#define _MO_FLO_CORE_VECTOR_2_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector4.h>

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
		class Vector2
		{
		public:
			//---------------------------------
			// Constructors & Destructor
			//---------------------------------
			Vector2(void);
			Vector2(const Vector2 &Vec);
			explicit Vector2(const Vector3 &Vec);
			explicit Vector2(const Vector4 &Vec);
			Vector2(const f32 fX, const f32 fY);

			//---------------------------------
			// Operators
			//---------------------------------
			Vector2& operator=(const Vector2 &Vec){x = Vec.x; y = Vec.y; return *this;};

			Vector2 operator+(const Vector2 &Vec) const
			{Vector2 Result; Result.x = this->x + Vec.x; Result.y = this->y + Vec.y; return Result;}

			Vector2 operator+(const f32 Scale) const
			{Vector2 Result; Result.x = this->x + Scale; Result.y = this->y + Scale; return Result;}

			Vector2 operator-(const Vector2 &Vec) const
			{Vector2 Result; Result.x = this->x - Vec.x; Result.y = this->y - Vec.y; return Result;}

			Vector2 operator-() const
			{Vector2 Result; Result.x = -this->x; Result.y = -this->y; return Result;}
	
			Vector2 operator-(const f32 Scale) const
			{Vector2 Result; Result.x = this->x - Scale; Result.y = this->y - Scale; return Result;}

			Vector2 operator*(const Vector2 &Vec) const
			{Vector2 Result; Result.x = this->x * Vec.x; Result.y = this->y * Vec.y; return Result;}

			Vector2 operator*(const f32 Scale) const
			{Vector2 Result; Result.x = this->x * Scale; Result.y = this->y * Scale; return Result;}
			
			Vector2 operator/(const Vector2 &Vec) const
			{Vector2 Result; Result.x = this->x / Vec.x; Result.y = this->y / Vec.y; return Result;}
			
			Vector2 operator/(const f32 Scale) const
			{Vector2 Result; Result.x = this->x / Scale; Result.y = this->y / Scale; return Result;}

			const Vector2& operator+=(const Vector2 &Vec)
            {this->x += Vec.x; this->y += Vec.y; return *this;}

			const Vector2& operator-=(const Vector2 &Vec)
            {this->x -= Vec.x; this->y -= Vec.y; return *this;}

			const Vector2& operator*=(const Vector2 &Vec)
            {this->x *= Vec.x; this->y *= Vec.y; return *this;}
			
			const Vector2& operator/=(const Vector2 &Vec)
            {this->x /= Vec.x; this->y /= Vec.y; return *this;}

			const Vector2& operator+=(const f32 Scale)
            {this->x += Scale; this->y += Scale; return *this;}

			const Vector2& operator-=(const f32 Scale)
            {this->x -= Scale; this->y -= Scale; return *this;}

			const Vector2& operator*=(const f32 Scale)
            {this->x *= Scale; this->y *= Scale; return *this;}
			
			const Vector2& operator/=(const f32 Scale)
            {this->x /= Scale; this->y /= Scale; return *this;}

			bool operator==(const Vector2 &Vec) const
            {return (Vec.x == x && Vec.y == y);}

			bool operator!=(const Vector2 &Vec) const
            {return (Vec.x != x || Vec.y != y);}

			bool operator>(const Vector2 &Vec) const
            {return x > Vec.x && y > Vec.y;}

			bool operator<(const Vector2 &Vec) const
            {return x < Vec.x && y < Vec.y;}

			bool operator>=(const Vector2 &Vec) const
            {return x >= Vec.x && y >= Vec.y;}

			bool operator<=(const Vector2 &Vec) const
            {return x <= Vec.x && y <= Vec.y;}

			//---------------------------------
			// Methods
			//---------------------------------
			Vector2 & Normalise();
			Vector2 NormalisedCopy() const;
            Vector2 & Inverse();
            Vector2 InversedCopy() const;
			f32 DotProduct(const Vector2 &inOtherVec) const;
			f32 Length() const;
			f32 LengthSquared() const;
			f32 Angle(const Vector2 &inOtherVec) const;
            
            static void Multiply(const Vector2* inpVec, const Vector2* inpVec2, Vector2* outpVec);

			static const Vector2 ZERO;
			static const Vector2 ONE;
			static const Vector2 X_UNIT_POSITIVE;
			static const Vector2 X_UNIT_NEGATIVE;
			static const Vector2 Y_UNIT_POSITIVE;
			static const Vector2 Y_UNIT_NEGATIVE;
		public:

			f32 x, y;
		};
		
		inline Vector2 operator*(const f32 Scale, const Vector2 & inVector) 
		{Vector2 Result; Result.x = inVector.x * Scale; Result.y = inVector.y * Scale; return Result;}
	}
}

#endif