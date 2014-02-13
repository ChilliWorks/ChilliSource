

#ifndef _MOFLO_CORE_QUATERNION_H_
#define _MOFLO_CORE_QUATERNION_H_

#include <moFlo/Core/Math/Matrix4x4.h>
#include <moFlo/Core/Math/Vector4.h>

namespace moFlo
{
	namespace Core
	{
		class CQuaternion
		{
		public:
		
			CQuaternion();
			CQuaternion(const f32 _x, const f32 _y, const f32 _z, const f32 _w);
			CQuaternion(const CVector3 & V, const f32 theta);
			CQuaternion(const CMatrix4x4& inMat);
			CQuaternion(const CVector3& xaxis, const CVector3& yaxis, const CVector3& zaxis);
			~CQuaternion();
			
            f32 Modulus() const;
            f32 ModulusSquared() const;
			CQuaternion & NormaliseSelf();
			CQuaternion Conjugate() const;
            CQuaternion Inverse() const;

			static CQuaternion Slerp(const CQuaternion &q1, const CQuaternion &q2, float t, bool inbShortestPath = true);
            static CQuaternion NLerp(const CQuaternion &q1, const CQuaternion &q2, float t, bool inbShortestPath = true);

			CMatrix4x4 ToRotationMatrix4x4(void) const;
			void ToRotationMatrix(CMatrix4x4 & inMatrix) const;
			
			f32 GetAngle() const;
			CVector3 GetAxis() const;

			void ToEulerAxes (CVector3& xaxis, CVector3& yaxis, CVector3& zaxis) const;

			void SetAxisAngle(const CVector3 & V, f32 theta);
			void SetAxisAngle(f32 infX, f32 infY, f32 infZ, f32 theta);

			static const CQuaternion IDENTITY;
			static const CQuaternion ZERO;
			
			bool operator==(const CQuaternion &q)const 
			{return (q.x == x && q.y == y && q.z == z && q.w == w);}
			
			bool operator!=(const CQuaternion &q)const 
			{return (q.x != x || q.y != y || q.z != z || q.w != w);}
			
			f32 w,x,y,z;
		};
		
		inline CQuaternion operator + (const CQuaternion &q1, const CQuaternion &q2)
		{
			return CQuaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
		}
		
		inline CQuaternion operator - (const CQuaternion &q1, const CQuaternion &q2)
		{
			return CQuaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
		}
		
		inline CQuaternion operator * (const CQuaternion &A, const CQuaternion &B)
		{
			CQuaternion C;
			
			C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
			C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
			C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
			C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
			return C;
		}

		inline CQuaternion operator * (const CQuaternion &q, const f32 &s)
		{
			return CQuaternion(q.x * s, q.y * s, q.z * s, q.w * s);
		}
		
		inline CQuaternion operator * (const f32 &s, const CQuaternion &q)
		{
			return CQuaternion(q.x * s, q.y * s, q.z * s, q.w * s);
		}
        
        inline CVector3 operator * ( const CQuaternion &q, const CVector3 &v)
        {
            CVector3 vQuatAxis(q.x,q.y,q.z);
            CVector3 uv(vQuatAxis.CrossProduct(v));
            CVector3 uuv(vQuatAxis.CrossProduct(uv));
            uv *= (2.0f * q.w);
            uuv *= 2.0f;
            
            return v + uv + uuv; 
        }
        
        inline CQuaternion operator / (const CQuaternion& q, f32 infValue)
        {
            f32 fInverseValue = (1.0f / infValue);
            return CQuaternion(q.x * fInverseValue, q.y * fInverseValue, q.z * fInverseValue, q.w * fInverseValue);
        }
	}
}

#endif
