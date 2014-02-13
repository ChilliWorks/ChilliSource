#ifndef _MO_FLO_CORE_VECTOR_2_H_
#define _MO_FLO_CORE_VECTOR_2_H_

#include <math.h>
#include <ChilliSource/Core/Math/Vector4.h>

//=================================================
// Tag - Vector 2
//
// 2-Dimensional Vector
//=================================================

namespace moFlo 
{
	namespace Core 
	{
		class CVector2
		{
		public:
			//---------------------------------
			// Constructors & Destructor
			//---------------------------------
			CVector2(void);
			CVector2(const CVector2 &Vec);
			explicit CVector2(const CVector3 &Vec);
			explicit CVector2(const CVector4 &Vec);
			CVector2(const f32 fX, const f32 fY);

			//---------------------------------
			// Operators
			//---------------------------------
			CVector2& operator=(const CVector2 &Vec){x = Vec.x; y = Vec.y; return *this;};

			CVector2 operator+(const CVector2 &Vec) const
			{CVector2 Result; Result.x = this->x + Vec.x; Result.y = this->y + Vec.y; return Result;}

			CVector2 operator+(const f32 Scale) const
			{CVector2 Result; Result.x = this->x + Scale; Result.y = this->y + Scale; return Result;}

			CVector2 operator-(const CVector2 &Vec) const
			{CVector2 Result; Result.x = this->x - Vec.x; Result.y = this->y - Vec.y; return Result;}

			CVector2 operator-() const
			{CVector2 Result; Result.x = -this->x; Result.y = -this->y; return Result;}
	
			CVector2 operator-(const f32 Scale) const
			{CVector2 Result; Result.x = this->x - Scale; Result.y = this->y - Scale; return Result;}

			CVector2 operator*(const CVector2 &Vec) const
			{CVector2 Result; Result.x = this->x * Vec.x; Result.y = this->y * Vec.y; return Result;}

			CVector2 operator*(const f32 Scale) const
			{CVector2 Result; Result.x = this->x * Scale; Result.y = this->y * Scale; return Result;}
			
			CVector2 operator/(const CVector2 &Vec) const
			{CVector2 Result; Result.x = this->x / Vec.x; Result.y = this->y / Vec.y; return Result;}
			
			CVector2 operator/(const f32 Scale) const
			{CVector2 Result; Result.x = this->x / Scale; Result.y = this->y / Scale; return Result;}

			const CVector2& operator+=(const CVector2 &Vec)
            {this->x += Vec.x; this->y += Vec.y; return *this;}

			const CVector2& operator-=(const CVector2 &Vec)
            {this->x -= Vec.x; this->y -= Vec.y; return *this;}

			const CVector2& operator*=(const CVector2 &Vec)
            {this->x *= Vec.x; this->y *= Vec.y; return *this;}
			
			const CVector2& operator/=(const CVector2 &Vec)
            {this->x /= Vec.x; this->y /= Vec.y; return *this;}

			const CVector2& operator+=(const f32 Scale)
            {this->x += Scale; this->y += Scale; return *this;}

			const CVector2& operator-=(const f32 Scale)
            {this->x -= Scale; this->y -= Scale; return *this;}

			const CVector2& operator*=(const f32 Scale)
            {this->x *= Scale; this->y *= Scale; return *this;}
			
			const CVector2& operator/=(const f32 Scale)
            {this->x /= Scale; this->y /= Scale; return *this;}

			bool operator==(const CVector2 &Vec) const
            {return (Vec.x == x && Vec.y == y);}

			bool operator!=(const CVector2 &Vec) const
            {return (Vec.x != x || Vec.y != y);}

			bool operator>(const CVector2 &Vec) const
            {return x > Vec.x && y > Vec.y;}

			bool operator<(const CVector2 &Vec) const
            {return x < Vec.x && y < Vec.y;}

			bool operator>=(const CVector2 &Vec) const
            {return x >= Vec.x && y >= Vec.y;}

			bool operator<=(const CVector2 &Vec) const
            {return x <= Vec.x && y <= Vec.y;}

			//---------------------------------
			// Methods
			//---------------------------------
			CVector2 & Normalise();
			CVector2 NormalisedCopy() const;
            CVector2 & Inverse();
            CVector2 InversedCopy() const;
			f32 DotProduct(const CVector2 &inOtherVec) const;
			f32 Length() const;
			f32 LengthSquared() const;
			f32 Angle(const CVector2 &inOtherVec) const;
            
            static void Multiply(const CVector2* inpVec, const CVector2* inpVec2, CVector2* outpVec);

			static const CVector2 ZERO;
			static const CVector2 ONE;
			static const CVector2 X_UNIT_POSITIVE;
			static const CVector2 X_UNIT_NEGATIVE;
			static const CVector2 Y_UNIT_POSITIVE;
			static const CVector2 Y_UNIT_NEGATIVE;
		public:

			f32 x, y;
		};
		
		inline CVector2 operator*(const f32 Scale, const CVector2 & inVector) 
		{CVector2 Result; Result.x = inVector.x * Scale; Result.y = inVector.y * Scale; return Result;}
	}
}

#endif