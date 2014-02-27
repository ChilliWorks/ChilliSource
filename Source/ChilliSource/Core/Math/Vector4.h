#ifndef _MO_FLO_CORE_VECTOR_4_H_
#define _MO_FLO_CORE_VECTOR_4_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector3.h>

#include <math.h>

//=================================================
// Tag - Vector 4
//
// 4-Dimensional Vector
//=================================================

namespace ChilliSource 
{
	namespace Core 
	{
		class Vector4
		{
		public:
			//---------------------------------
			// Constructors & Destructor
			//---------------------------------
			Vector4(void);
			Vector4(const Vector4 &Vec);
			Vector4(const Vector3 &Vec, f32 inW = 1.0f);
			Vector4(const f32 fX, const f32 fY, const f32 fZ, const f32 fW);
			
			//---------------------------------
			// Operators
			//---------------------------------
			Vector4& operator=(const Vector4 &Vec){x = Vec.x; y = Vec.y; z = Vec.z; w = Vec.w; return *this;};
			
			Vector4 operator+(const Vector4 &Vec) const
			{Vector4 Result; Result.x = this->x + Vec.x; Result.y = this->y + Vec.y; Result.z = this->z + Vec.z; Result.w = this->w + Vec.w; return Result;}
			
			Vector4 operator+(const f32 Scale) const
			{Vector4 Result; Result.x = this->x + Scale; Result.y = this->y + Scale; Result.z = this->z + Scale; Result.w = this->w + Scale; return Result;}
			
			Vector4 operator-(const Vector4 &Vec) const
			{Vector4 Result; Result.x = this->x - Vec.x; Result.y = this->y - Vec.y; Result.z = this->z - Vec.z; Result.w = this->w - Vec.w; return Result;}
			
			Vector4 operator-(const f32 Scale) const
			{Vector4 Result; Result.x = this->x - Scale; Result.y = this->y - Scale; Result.z = this->z - Scale; Result.w = this->w - Scale; return Result;}
			
			Vector4 operator*(const Vector4 &Vec) const
			{Vector4 Result; Result.x = this->x * Vec.x; Result.y = this->y * Vec.y; Result.z = this->z * Vec.z; Result.w = this->w * Vec.w; return Result;}
			
			Vector4 operator*(const f32 Scale) const
			{Vector4 Result; Result.x = this->x * Scale; Result.y = this->y * Scale; Result.z = this->z * Scale; Result.w = this->w * Scale; return Result;}
			
			Vector4 operator/(const Vector4 &Vec) const
			{Vector4 Result; Result.x = this->x / Vec.x; Result.y = this->y / Vec.y; Result.z = this->z / Vec.z; Result.w = this->w / Vec.w; return Result;}
			
			Vector4 operator/(const f32 Scale) const
			{Vector4 Result; Result.x = this->x / Scale; Result.y = this->y / Scale; Result.z = this->z / Scale; Result.w = this->w / Scale; return Result;}
			
			const Vector4& operator+=(const Vector4 &Vec)
			{this->x += Vec.x; this->y += Vec.y; this->z += Vec.z; this->w += Vec.w; return *this;}
			
			const Vector4& operator-=(const Vector4 &Vec)
			{this->x -= Vec.x; this->y -= Vec.y; this->z -= Vec.z; this->w -= Vec.w; return *this;}
			
			const Vector4& operator*=(const Vector4 &Vec)
			{this->x *= Vec.x; this->y *= Vec.y; this->z *= Vec.z; this->w *= Vec.w; return *this;}
			
			const Vector4& operator/=(const Vector4 &Vec)
			{this->x /= Vec.x; this->y /= Vec.y; this->z /= Vec.z; this->w /= Vec.w; return *this;}
			
			const Vector4& operator+=(const f32 Scale)
			{this->x += Scale; this->y += Scale; this->z += Scale; this->w += Scale; return *this;}
			
			const Vector4& operator-=(const f32 Scale)
			{this->x -= Scale; this->y -= Scale; this->z -= Scale; this->w -= Scale; return *this;}

			Vector4 operator-() const
			{Vector4 Result; Result.x = -this->x; Result.y = -this->y; Result.z = -this->z; Result.w = -this->w; return Result;}
						
			const Vector4& operator*=(const f32 Scale)
			{this->x *= Scale; this->y *= Scale; this->z *= Scale; this->w *= Scale; return *this;}
			
			const Vector4& operator/=(const f32 Scale)
			{this->x /= Scale; this->y /= Scale; this->z /= Scale; this->w /= Scale; return *this;}
			
			bool operator==(const Vector4 &Vec)const 
			{return (Vec.x == x && Vec.y == y && Vec.z == z && Vec.w == w);}
			
			bool operator!=(const Vector4 &Vec)const 
			{return (Vec.x != x || Vec.y != y || Vec.z != z || w != w);}
			
			bool operator>(const Vector4 &Vec)const 
			{return x > Vec.x && y > Vec.y && z > Vec.z && w > Vec.w;}
			
			bool operator<(const Vector4 &Vec)const 
			{return x < Vec.x && y < Vec.y && z < Vec.z && w < Vec.w;}
			
			bool operator>=(const Vector4 &Vec)const 
			{return x >= Vec.x && y >= Vec.y && z >= Vec.z && w >= Vec.w;}
			
			bool operator<=(const Vector4 &Vec)const 
			{return x <= Vec.x && y <= Vec.y && z <= Vec.z && w <= Vec.w;}
			
			//---------------------------------
			// Methods
			//---------------------------------
			void CrossProduct(const Vector4 &Vec, Vector4 &Out);
            Vector4 CrossProduct(const Vector4 &Vec, const Vector4 &Vec2);
            Vector4 & Normalise();
			Vector4 NormalisedCopy() const;
            Vector4 InversedCopy() const;
            Vector4 & Inverse();
			f32 Length() const;
			f32 LengthSquared() const;
			
			static const Vector4 ZERO;
            
            static void Multiply(const Vector4* inpVec, const Vector4* inpVec2, Vector4* outpVec);
            static void CrossProduct(const Vector4* inpVec, const Vector4* inpVec2, Vector4* outpVec);
			
		public:
			
			f32 x, y, z, w;
		};
	}
}

#endif