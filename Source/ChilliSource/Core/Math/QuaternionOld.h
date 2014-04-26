

#ifndef _MOFLO_CORE_QUATERNION_H_
#define _MOFLO_CORE_QUATERNION_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix4x4Old.h>
#include <ChilliSource/Core/Math/Vector4.h>

namespace ChilliSource
{
	namespace Core
	{
		class QuaternionOld
		{
		public:
		
			QuaternionOld();
			QuaternionOld(const f32 _x, const f32 _y, const f32 _z, const f32 _w);
			QuaternionOld(const Vector3 & V, const f32 theta);
			QuaternionOld(const Matrix4x4Old& inMat);
			QuaternionOld(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis);
			~QuaternionOld();
			
            f32 Modulus() const;
            f32 ModulusSquared() const;
			QuaternionOld & NormaliseSelf();
			QuaternionOld Conjugate() const;
            QuaternionOld Inverse() const;

			static QuaternionOld Slerp(const QuaternionOld &q1, const QuaternionOld &q2, float t, bool inbShortestPath = true);
            static QuaternionOld NLerp(const QuaternionOld &q1, const QuaternionOld &q2, float t, bool inbShortestPath = true);

			Matrix4x4Old ToRotationMatrix4x4Old(void) const;
			void ToRotationMatrix(Matrix4x4Old & inMatrix) const;
			
			f32 GetAngle() const;
			Vector3 GetAxis() const;

			void ToEulerAxes (Vector3& xaxis, Vector3& yaxis, Vector3& zaxis) const;

			void SetAxisAngle(const Vector3 & V, f32 theta);
			void SetAxisAngle(f32 infX, f32 infY, f32 infZ, f32 theta);

			static const QuaternionOld IDENTITY;
			static const QuaternionOld ZERO;
			
			bool operator==(const QuaternionOld &q)const 
			{return (q.x == x && q.y == y && q.z == z && q.w == w);}
			
			bool operator!=(const QuaternionOld &q)const 
			{return (q.x != x || q.y != y || q.z != z || q.w != w);}
			
			f32 w,x,y,z;
		};
		
		inline QuaternionOld operator + (const QuaternionOld &q1, const QuaternionOld &q2)
		{
			return QuaternionOld(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
		}
		
		inline QuaternionOld operator - (const QuaternionOld &q1, const QuaternionOld &q2)
		{
			return QuaternionOld(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
		}
		
		inline QuaternionOld operator * (const QuaternionOld &A, const QuaternionOld &B)
		{
			QuaternionOld C;
			
			C.x = A.w*B.x + A.x*B.w + A.y*B.z - A.z*B.y;
			C.y = A.w*B.y - A.x*B.z + A.y*B.w + A.z*B.x;
			C.z = A.w*B.z + A.x*B.y - A.y*B.x + A.z*B.w;
			C.w = A.w*B.w - A.x*B.x - A.y*B.y - A.z*B.z;
			return C;
		}

		inline QuaternionOld operator * (const QuaternionOld &q, const f32 &s)
		{
			return QuaternionOld(q.x * s, q.y * s, q.z * s, q.w * s);
		}
		
		inline QuaternionOld operator * (const f32 &s, const QuaternionOld &q)
		{
			return QuaternionOld(q.x * s, q.y * s, q.z * s, q.w * s);
		}
        
        inline Vector3 operator * ( const QuaternionOld &q, const Vector3 &v)
        {
            Vector3 vQuatAxis(q.x,q.y,q.z);
            Vector3 uv(Vector3::CrossProduct(vQuatAxis, v));
			Vector3 uuv(Vector3::CrossProduct(vQuatAxis, uv));
            uv *= (2.0f * q.w);
            uuv *= 2.0f;
            
            return v + uv + uuv; 
        }
        
        inline QuaternionOld operator / (const QuaternionOld& q, f32 infValue)
        {
            f32 fInverseValue = (1.0f / infValue);
            return QuaternionOld(q.x * fInverseValue, q.y * fInverseValue, q.z * fInverseValue, q.w * fInverseValue);
        }
	}
}

#endif
