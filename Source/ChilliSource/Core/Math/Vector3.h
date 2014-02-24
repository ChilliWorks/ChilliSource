#ifndef _MO_FLO_CORE_VECTOR_3_H_
#define _MO_FLO_CORE_VECTOR_3_H_

#include <math.h>

//=================================================
// Tag - Vector 3
//
// 3-Dimensional Vector
//=================================================

namespace ChilliSource 
{
	namespace Core 
	{
		class CVector2;
		class CVector4;
		
		class CVector3
		{
		public:
			//---------------------------------
			// Constructors & Destructor
			//---------------------------------
			CVector3(void);
			CVector3(const CVector3 &Vec);
			CVector3(const CVector2 &Vec2);
			CVector3(const CVector2 &Vec2, float fZ);
			CVector3(const CVector4 &Vec);
			CVector3(const f32 fX, const f32 fY, const f32 fZ);

			//---------------------------------
			// Operators
			//---------------------------------
			CVector3& operator=(const CVector3 &Vec){x = Vec.x; y = Vec.y; z = Vec.z; return *this;};

			CVector3 operator+(const CVector3 &Vec) const
			{CVector3 Result; Result.x = this->x + Vec.x; Result.y = this->y + Vec.y; Result.z = this->z + Vec.z; return Result;}

			CVector3 operator+(const f32 Scale) const
			{CVector3 Result; Result.x = this->x + Scale; Result.y = this->y + Scale; Result.z = this->z + Scale; return Result;}

			CVector3 operator-(const CVector3 &Vec) const
			{CVector3 Result; Result.x = this->x - Vec.x; Result.y = this->y - Vec.y; Result.z = this->z - Vec.z; return Result;}

			CVector3 operator-(const f32 Scale) const
			{CVector3 Result; Result.x = this->x - Scale; Result.y = this->y - Scale; Result.z = this->z - Scale; return Result;}

			CVector3 operator-() const
			{CVector3 Result; Result.x = -this->x; Result.y = -this->y; Result.z = -this->z; return Result;}
			
			CVector3 operator*(const CVector3 &Vec) const
			{CVector3 Result; Result.x = this->x * Vec.x; Result.y = this->y * Vec.y; Result.z = this->z * Vec.z; return Result;}

			CVector3 operator*(const f32 Scale) const
			{CVector3 Result; Result.x = this->x * Scale; Result.y = this->y * Scale; Result.z = this->z * Scale; return Result;}
			
			CVector3 operator/(const CVector3 &Vec) const
			{CVector3 Result; Result.x = this->x / Vec.x; Result.y = this->y / Vec.y; Result.z = this->z / Vec.z; return Result;}
			
			CVector3 operator/(const f32 Scale) const
			{CVector3 Result; Result.x = this->x / Scale; Result.y = this->y / Scale; Result.z = this->z / Scale; return Result;}

			const CVector3& operator+=(const CVector3 &Vec){this->x += Vec.x; this->y += Vec.y; this->z += Vec.z; return *this;}

			const CVector3& operator-=(const CVector3 &Vec){this->x -= Vec.x; this->y -= Vec.y; this->z -= Vec.z; return *this;}

			const CVector3& operator*=(const CVector3 &Vec){this->x *= Vec.x; this->y *= Vec.y; this->z *= Vec.z; return *this;}
			
			const CVector3& operator/=(const CVector3 &Vec){this->x /= Vec.x; this->y /= Vec.y; this->z /= Vec.z; return *this;}

			const CVector3& operator+=(const f32 Scale){this->x += Scale; this->y += Scale; this->z += Scale; return *this;}

			const CVector3& operator-=(const f32 Scale){this->x -= Scale; this->y -= Scale; this->z -= Scale; return *this;}

			const CVector3& operator*=(const f32 Scale){this->x *= Scale; this->y *= Scale; this->z *= Scale; return *this;}
			
			const CVector3& operator/=(const f32 Scale){this->x /= Scale; this->y /= Scale; this->z /= Scale; return *this;}

			bool operator==(const CVector3 &Vec) const
			{return (Vec.x == x && Vec.y == y && Vec.z == z);}

			bool operator!=(const CVector3 &Vec)const
			{return (Vec.x != x || Vec.y != y || Vec.z != z);}

			bool operator>(const CVector3 &Vec) const
			{return x > Vec.x && y > Vec.y && z > Vec.z;}

			bool operator<(const CVector3 &Vec) const
			{return x < Vec.x && y < Vec.y && z < Vec.z;}

			bool operator>=(const CVector3 &Vec) const
			{return x >= Vec.x && y >= Vec.y && z >= Vec.z;}

			bool operator<=(const CVector3 &Vec) const
			{return x <= Vec.x && y <= Vec.y && z <= Vec.z;}

			//---------------------------------
			// Methods
			//---------------------------------
			void CrossProduct(const CVector3 &Vec, CVector3 &Out);
			CVector3 CrossProduct(const CVector3 &Vec) const;
			CVector3 &Normalise();
            CVector3 & Inverse();
			CVector3 NormalisedCopy() const;
			CVector3 InversedCopy() const;
			f32 DotProduct(const CVector3 &Vec) const;
			f32 Length() const;
			f32 LengthSquared() const;
			f32 Angle(const CVector3 &Vec) const;
            
            static void Multiply(const CVector3* inpVec, const CVector3* inpVec2, CVector3* outpVec);
            static void CrossProduct(const CVector3* inpVec, const CVector3* inpVec2, CVector3* outpVec);
            static f32 DotProduct(const CVector3* inpVec, const CVector3* inpVec2);
			
			
			static const CVector3 ZERO;
            static const CVector3 ONE;
			static const CVector3 X_UNIT_POSITIVE;
			static const CVector3 X_UNIT_NEGATIVE;
			static const CVector3 Y_UNIT_POSITIVE;
			static const CVector3 Y_UNIT_NEGATIVE;
			static const CVector3 Z_UNIT_POSITIVE;
			static const CVector3 Z_UNIT_NEGATIVE;			

		public:

			f32 x, y, z;
		};
		
		CVector3 operator*(const f32 Scale, const CVector3 & Vec);
	}
}

#endif