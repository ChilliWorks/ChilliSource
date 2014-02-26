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
		class CVector4
		{
		public:
			//---------------------------------
			// Constructors & Destructor
			//---------------------------------
			CVector4(void);
			CVector4(const CVector4 &Vec);
			CVector4(const CVector3 &Vec, f32 inW = 1.0f);
			CVector4(const f32 fX, const f32 fY, const f32 fZ, const f32 fW);
			
			//---------------------------------
			// Operators
			//---------------------------------
			CVector4& operator=(const CVector4 &Vec){x = Vec.x; y = Vec.y; z = Vec.z; w = Vec.w; return *this;};
			
			CVector4 operator+(const CVector4 &Vec) const
			{CVector4 Result; Result.x = this->x + Vec.x; Result.y = this->y + Vec.y; Result.z = this->z + Vec.z; Result.w = this->w + Vec.w; return Result;}
			
			CVector4 operator+(const f32 Scale) const
			{CVector4 Result; Result.x = this->x + Scale; Result.y = this->y + Scale; Result.z = this->z + Scale; Result.w = this->w + Scale; return Result;}
			
			CVector4 operator-(const CVector4 &Vec) const
			{CVector4 Result; Result.x = this->x - Vec.x; Result.y = this->y - Vec.y; Result.z = this->z - Vec.z; Result.w = this->w - Vec.w; return Result;}
			
			CVector4 operator-(const f32 Scale) const
			{CVector4 Result; Result.x = this->x - Scale; Result.y = this->y - Scale; Result.z = this->z - Scale; Result.w = this->w - Scale; return Result;}
			
			CVector4 operator*(const CVector4 &Vec) const
			{CVector4 Result; Result.x = this->x * Vec.x; Result.y = this->y * Vec.y; Result.z = this->z * Vec.z; Result.w = this->w * Vec.w; return Result;}
			
			CVector4 operator*(const f32 Scale) const
			{CVector4 Result; Result.x = this->x * Scale; Result.y = this->y * Scale; Result.z = this->z * Scale; Result.w = this->w * Scale; return Result;}
			
			CVector4 operator/(const CVector4 &Vec) const
			{CVector4 Result; Result.x = this->x / Vec.x; Result.y = this->y / Vec.y; Result.z = this->z / Vec.z; Result.w = this->w / Vec.w; return Result;}
			
			CVector4 operator/(const f32 Scale) const
			{CVector4 Result; Result.x = this->x / Scale; Result.y = this->y / Scale; Result.z = this->z / Scale; Result.w = this->w / Scale; return Result;}
			
			const CVector4& operator+=(const CVector4 &Vec)
			{this->x += Vec.x; this->y += Vec.y; this->z += Vec.z; this->w += Vec.w; return *this;}
			
			const CVector4& operator-=(const CVector4 &Vec)
			{this->x -= Vec.x; this->y -= Vec.y; this->z -= Vec.z; this->w -= Vec.w; return *this;}
			
			const CVector4& operator*=(const CVector4 &Vec)
			{this->x *= Vec.x; this->y *= Vec.y; this->z *= Vec.z; this->w *= Vec.w; return *this;}
			
			const CVector4& operator/=(const CVector4 &Vec)
			{this->x /= Vec.x; this->y /= Vec.y; this->z /= Vec.z; this->w /= Vec.w; return *this;}
			
			const CVector4& operator+=(const f32 Scale)
			{this->x += Scale; this->y += Scale; this->z += Scale; this->w += Scale; return *this;}
			
			const CVector4& operator-=(const f32 Scale)
			{this->x -= Scale; this->y -= Scale; this->z -= Scale; this->w -= Scale; return *this;}

			CVector4 operator-() const
			{CVector4 Result; Result.x = -this->x; Result.y = -this->y; Result.z = -this->z; Result.w = -this->w; return Result;}
						
			const CVector4& operator*=(const f32 Scale)
			{this->x *= Scale; this->y *= Scale; this->z *= Scale; this->w *= Scale; return *this;}
			
			const CVector4& operator/=(const f32 Scale)
			{this->x /= Scale; this->y /= Scale; this->z /= Scale; this->w /= Scale; return *this;}
			
			bool operator==(const CVector4 &Vec)const 
			{return (Vec.x == x && Vec.y == y && Vec.z == z && Vec.w == w);}
			
			bool operator!=(const CVector4 &Vec)const 
			{return (Vec.x != x || Vec.y != y || Vec.z != z || w != w);}
			
			bool operator>(const CVector4 &Vec)const 
			{return x > Vec.x && y > Vec.y && z > Vec.z && w > Vec.w;}
			
			bool operator<(const CVector4 &Vec)const 
			{return x < Vec.x && y < Vec.y && z < Vec.z && w < Vec.w;}
			
			bool operator>=(const CVector4 &Vec)const 
			{return x >= Vec.x && y >= Vec.y && z >= Vec.z && w >= Vec.w;}
			
			bool operator<=(const CVector4 &Vec)const 
			{return x <= Vec.x && y <= Vec.y && z <= Vec.z && w <= Vec.w;}
			
			//---------------------------------
			// Methods
			//---------------------------------
			void CrossProduct(const CVector4 &Vec, CVector4 &Out);
            CVector4 CrossProduct(const CVector4 &Vec, const CVector4 &Vec2);
            CVector4 & Normalise();
			CVector4 NormalisedCopy() const;
            CVector4 InversedCopy() const;
            CVector4 & Inverse();
			f32 Length() const;
			f32 LengthSquared() const;
			
			static const CVector4 ZERO;
            
            static void Multiply(const CVector4* inpVec, const CVector4* inpVec2, CVector4* outpVec);
            static void CrossProduct(const CVector4* inpVec, const CVector4* inpVec2, CVector4* outpVec);
			
		public:
			
			f32 x, y, z, w;
		};
	}
}

#endif