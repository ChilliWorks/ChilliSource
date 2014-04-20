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
		class Vector4Old;
		
		class Vector3Old
		{
		public:
			//---------------------------------
			// Constructors & Destructor
			//---------------------------------
			Vector3Old(void);
			Vector3Old(const Vector3Old &Vec);
			Vector3Old(const Vector2 &Vec2);
			Vector3Old(const Vector2 &Vec2, float fZ);
			Vector3Old(const Vector4Old &Vec);
			Vector3Old(const f32 fX, const f32 fY, const f32 fZ);

			//---------------------------------
			// Operators
			//---------------------------------
			Vector3Old& operator=(const Vector3Old &Vec){x = Vec.x; y = Vec.y; z = Vec.z; return *this;};

			Vector3Old operator+(const Vector3Old &Vec) const
			{Vector3Old Result; Result.x = this->x + Vec.x; Result.y = this->y + Vec.y; Result.z = this->z + Vec.z; return Result;}

			Vector3Old operator+(const f32 Scale) const
			{Vector3Old Result; Result.x = this->x + Scale; Result.y = this->y + Scale; Result.z = this->z + Scale; return Result;}

			Vector3Old operator-(const Vector3Old &Vec) const
			{Vector3Old Result; Result.x = this->x - Vec.x; Result.y = this->y - Vec.y; Result.z = this->z - Vec.z; return Result;}

			Vector3Old operator-(const f32 Scale) const
			{Vector3Old Result; Result.x = this->x - Scale; Result.y = this->y - Scale; Result.z = this->z - Scale; return Result;}

			Vector3Old operator-() const
			{Vector3Old Result; Result.x = -this->x; Result.y = -this->y; Result.z = -this->z; return Result;}
			
			Vector3Old operator*(const Vector3Old &Vec) const
			{Vector3Old Result; Result.x = this->x * Vec.x; Result.y = this->y * Vec.y; Result.z = this->z * Vec.z; return Result;}

			Vector3Old operator*(const f32 Scale) const
			{Vector3Old Result; Result.x = this->x * Scale; Result.y = this->y * Scale; Result.z = this->z * Scale; return Result;}
			
			Vector3Old operator/(const Vector3Old &Vec) const
			{Vector3Old Result; Result.x = this->x / Vec.x; Result.y = this->y / Vec.y; Result.z = this->z / Vec.z; return Result;}
			
			Vector3Old operator/(const f32 Scale) const
			{Vector3Old Result; Result.x = this->x / Scale; Result.y = this->y / Scale; Result.z = this->z / Scale; return Result;}

			const Vector3Old& operator+=(const Vector3Old &Vec){this->x += Vec.x; this->y += Vec.y; this->z += Vec.z; return *this;}

			const Vector3Old& operator-=(const Vector3Old &Vec){this->x -= Vec.x; this->y -= Vec.y; this->z -= Vec.z; return *this;}

			const Vector3Old& operator*=(const Vector3Old &Vec){this->x *= Vec.x; this->y *= Vec.y; this->z *= Vec.z; return *this;}
			
			const Vector3Old& operator/=(const Vector3Old &Vec){this->x /= Vec.x; this->y /= Vec.y; this->z /= Vec.z; return *this;}

			const Vector3Old& operator+=(const f32 Scale){this->x += Scale; this->y += Scale; this->z += Scale; return *this;}

			const Vector3Old& operator-=(const f32 Scale){this->x -= Scale; this->y -= Scale; this->z -= Scale; return *this;}

			const Vector3Old& operator*=(const f32 Scale){this->x *= Scale; this->y *= Scale; this->z *= Scale; return *this;}
			
			const Vector3Old& operator/=(const f32 Scale){this->x /= Scale; this->y /= Scale; this->z /= Scale; return *this;}

			bool operator==(const Vector3Old &Vec) const
			{return (Vec.x == x && Vec.y == y && Vec.z == z);}

			bool operator!=(const Vector3Old &Vec)const
			{return (Vec.x != x || Vec.y != y || Vec.z != z);}

			bool operator>(const Vector3Old &Vec) const
			{return x > Vec.x && y > Vec.y && z > Vec.z;}

			bool operator<(const Vector3Old &Vec) const
			{return x < Vec.x && y < Vec.y && z < Vec.z;}

			bool operator>=(const Vector3Old &Vec) const
			{return x >= Vec.x && y >= Vec.y && z >= Vec.z;}

			bool operator<=(const Vector3Old &Vec) const
			{return x <= Vec.x && y <= Vec.y && z <= Vec.z;}

			//---------------------------------
			// Methods
			//---------------------------------
			void CrossProduct(const Vector3Old &Vec, Vector3Old &Out);
			Vector3Old CrossProduct(const Vector3Old &Vec) const;
			Vector3Old &Normalise();
            Vector3Old & Inverse();
			Vector3Old NormalisedCopy() const;
			Vector3Old InversedCopy() const;
			f32 DotProduct(const Vector3Old &Vec) const;
			f32 Length() const;
			f32 LengthSquared() const;
			f32 Angle(const Vector3Old &Vec) const;
            
            static void Multiply(const Vector3Old* inpVec, const Vector3Old* inpVec2, Vector3Old* outpVec);
            static void CrossProduct(const Vector3Old* inpVec, const Vector3Old* inpVec2, Vector3Old* outpVec);
            static f32 DotProduct(const Vector3Old* inpVec, const Vector3Old* inpVec2);
			
			
			static const Vector3Old ZERO;
            static const Vector3Old ONE;
			static const Vector3Old X_UNIT_POSITIVE;
			static const Vector3Old X_UNIT_NEGATIVE;
			static const Vector3Old Y_UNIT_POSITIVE;
			static const Vector3Old Y_UNIT_NEGATIVE;
			static const Vector3Old Z_UNIT_POSITIVE;
			static const Vector3Old Z_UNIT_NEGATIVE;			

		public:

			f32 x, y, z;
		};
		
		Vector3Old operator*(const f32 Scale, const Vector3Old & Vec);
	}
}

#endif