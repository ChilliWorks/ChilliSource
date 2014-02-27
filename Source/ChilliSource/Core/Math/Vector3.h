#ifndef _MO_FLO_CORE_VECTOR_3_H_
#define _MO_FLO_CORE_VECTOR_3_H_

#include <ChilliSource/ChilliSource.h>

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
		class Vector2;
		class Vector4;
		
		class Vector3
		{
		public:
			//---------------------------------
			// Constructors & Destructor
			//---------------------------------
			Vector3(void);
			Vector3(const Vector3 &Vec);
			Vector3(const Vector2 &Vec2);
			Vector3(const Vector2 &Vec2, float fZ);
			Vector3(const Vector4 &Vec);
			Vector3(const f32 fX, const f32 fY, const f32 fZ);

			//---------------------------------
			// Operators
			//---------------------------------
			Vector3& operator=(const Vector3 &Vec){x = Vec.x; y = Vec.y; z = Vec.z; return *this;};

			Vector3 operator+(const Vector3 &Vec) const
			{Vector3 Result; Result.x = this->x + Vec.x; Result.y = this->y + Vec.y; Result.z = this->z + Vec.z; return Result;}

			Vector3 operator+(const f32 Scale) const
			{Vector3 Result; Result.x = this->x + Scale; Result.y = this->y + Scale; Result.z = this->z + Scale; return Result;}

			Vector3 operator-(const Vector3 &Vec) const
			{Vector3 Result; Result.x = this->x - Vec.x; Result.y = this->y - Vec.y; Result.z = this->z - Vec.z; return Result;}

			Vector3 operator-(const f32 Scale) const
			{Vector3 Result; Result.x = this->x - Scale; Result.y = this->y - Scale; Result.z = this->z - Scale; return Result;}

			Vector3 operator-() const
			{Vector3 Result; Result.x = -this->x; Result.y = -this->y; Result.z = -this->z; return Result;}
			
			Vector3 operator*(const Vector3 &Vec) const
			{Vector3 Result; Result.x = this->x * Vec.x; Result.y = this->y * Vec.y; Result.z = this->z * Vec.z; return Result;}

			Vector3 operator*(const f32 Scale) const
			{Vector3 Result; Result.x = this->x * Scale; Result.y = this->y * Scale; Result.z = this->z * Scale; return Result;}
			
			Vector3 operator/(const Vector3 &Vec) const
			{Vector3 Result; Result.x = this->x / Vec.x; Result.y = this->y / Vec.y; Result.z = this->z / Vec.z; return Result;}
			
			Vector3 operator/(const f32 Scale) const
			{Vector3 Result; Result.x = this->x / Scale; Result.y = this->y / Scale; Result.z = this->z / Scale; return Result;}

			const Vector3& operator+=(const Vector3 &Vec){this->x += Vec.x; this->y += Vec.y; this->z += Vec.z; return *this;}

			const Vector3& operator-=(const Vector3 &Vec){this->x -= Vec.x; this->y -= Vec.y; this->z -= Vec.z; return *this;}

			const Vector3& operator*=(const Vector3 &Vec){this->x *= Vec.x; this->y *= Vec.y; this->z *= Vec.z; return *this;}
			
			const Vector3& operator/=(const Vector3 &Vec){this->x /= Vec.x; this->y /= Vec.y; this->z /= Vec.z; return *this;}

			const Vector3& operator+=(const f32 Scale){this->x += Scale; this->y += Scale; this->z += Scale; return *this;}

			const Vector3& operator-=(const f32 Scale){this->x -= Scale; this->y -= Scale; this->z -= Scale; return *this;}

			const Vector3& operator*=(const f32 Scale){this->x *= Scale; this->y *= Scale; this->z *= Scale; return *this;}
			
			const Vector3& operator/=(const f32 Scale){this->x /= Scale; this->y /= Scale; this->z /= Scale; return *this;}

			bool operator==(const Vector3 &Vec) const
			{return (Vec.x == x && Vec.y == y && Vec.z == z);}

			bool operator!=(const Vector3 &Vec)const
			{return (Vec.x != x || Vec.y != y || Vec.z != z);}

			bool operator>(const Vector3 &Vec) const
			{return x > Vec.x && y > Vec.y && z > Vec.z;}

			bool operator<(const Vector3 &Vec) const
			{return x < Vec.x && y < Vec.y && z < Vec.z;}

			bool operator>=(const Vector3 &Vec) const
			{return x >= Vec.x && y >= Vec.y && z >= Vec.z;}

			bool operator<=(const Vector3 &Vec) const
			{return x <= Vec.x && y <= Vec.y && z <= Vec.z;}

			//---------------------------------
			// Methods
			//---------------------------------
			void CrossProduct(const Vector3 &Vec, Vector3 &Out);
			Vector3 CrossProduct(const Vector3 &Vec) const;
			Vector3 &Normalise();
            Vector3 & Inverse();
			Vector3 NormalisedCopy() const;
			Vector3 InversedCopy() const;
			f32 DotProduct(const Vector3 &Vec) const;
			f32 Length() const;
			f32 LengthSquared() const;
			f32 Angle(const Vector3 &Vec) const;
            
            static void Multiply(const Vector3* inpVec, const Vector3* inpVec2, Vector3* outpVec);
            static void CrossProduct(const Vector3* inpVec, const Vector3* inpVec2, Vector3* outpVec);
            static f32 DotProduct(const Vector3* inpVec, const Vector3* inpVec2);
			
			
			static const Vector3 ZERO;
            static const Vector3 ONE;
			static const Vector3 X_UNIT_POSITIVE;
			static const Vector3 X_UNIT_NEGATIVE;
			static const Vector3 Y_UNIT_POSITIVE;
			static const Vector3 Y_UNIT_NEGATIVE;
			static const Vector3 Z_UNIT_POSITIVE;
			static const Vector3 Z_UNIT_NEGATIVE;			

		public:

			f32 x, y, z;
		};
		
		Vector3 operator*(const f32 Scale, const Vector3 & Vec);
	}
}

#endif