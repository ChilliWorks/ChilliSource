

#ifndef _MOFLO_CORE_QUATERNION_H_
#define _MOFLO_CORE_QUATERNION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>
#include <ChilliSource/Core/Math/Vector4.h>

namespace ChilliSource
{
	namespace Core
	{
		class Quaternion
		{
		public:
		
			Quaternion();
			Quaternion(const f32 _x, const f32 _y, const f32 _z, const f32 _w);
			Quaternion(const Vector3 & V, const f32 theta);
			Quaternion(const Matrix4x4& inMat);
			Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis);
			~Quaternion();
			
            f32 Modulus() const;
            f32 ModulusSquared() const;
			Quaternion & NormaliseSelf();
			Quaternion Conjugate() const;
            Quaternion Inverse() const;

			static Quaternion Slerp(const Quaternion &q1, const Quaternion &q2, float t, bool inbShortestPath = true);
            static Quaternion NLerp(const Quaternion &q1, const Quaternion &q2, float t, bool inbShortestPath = true);

			Matrix4x4 ToRotationMatrix4x4(void) const;
			void ToRotationMatrix(Matrix4x4 & inMatrix) const;
			
			f32 GetAngle() const;
			Vector3 GetAxis() const;

			void ToEulerAxes (Vector3& xaxis, Vector3& yaxis, Vector3& zaxis) const;

			void SetAxisAngle(const Vector3 & V, f32 theta);
			void SetAxisAngle(f32 infX, f32 infY, f32 infZ, f32 theta);

			static const Quaternion IDENTITY;
			static const Quaternion ZERO;
			
			bool operator==(const Quaternion &q)const 
			{return (q.x == x && q.y == y && q.z == z && q.w == w);}
			
			bool operator!=(const Quaternion &q)const 
			{return (q.x != x || q.y != y || q.z != z || q.w != w);}
			
			f32 w,x,y,z;
		};
		
		inline Quaternion operator + (const Quaternion &q1, const Quaternion &q2)
		{
			return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
		}
		
		inline Quaternion operator - (const Quaternion &q1, const Quaternion &q2)
		{
			return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
		}
		
		inline Quaternion operator * (const Quaternion &A, const Quaternion &B)
		{
			Quaternion C;
			
			C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
			C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
			C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
			C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
			return C;
		}

		inline Quaternion operator * (const Quaternion &q, const f32 &s)
		{
			return Quaternion(q.x * s, q.y * s, q.z * s, q.w * s);
		}
		
		inline Quaternion operator * (const f32 &s, const Quaternion &q)
		{
			return Quaternion(q.x * s, q.y * s, q.z * s, q.w * s);
		}
        
        inline Vector3 operator * ( const Quaternion &q, const Vector3 &v)
        {
            Vector3 vQuatAxis(q.x,q.y,q.z);
            Vector3 uv(vQuatAxis.CrossProduct(v));
            Vector3 uuv(vQuatAxis.CrossProduct(uv));
            uv *= (2.0f * q.w);
            uuv *= 2.0f;
            
            return v + uv + uuv; 
        }
        
        inline Quaternion operator / (const Quaternion& q, f32 infValue)
        {
            f32 fInverseValue = (1.0f / infValue);
            return Quaternion(q.x * fInverseValue, q.y * fInverseValue, q.z * fInverseValue, q.w * fInverseValue);
        }
	}
}

#endif
