/*
 *  Matrix4x4.h
 *  moFlo
 *
 *  Created by Scott Downie on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_MATH_MATRIX_4X4_H_
#define _MO_FLO_CORE_MATH_MATRIX_4X4_H_

#include <ChilliSource/Core/Math/Matrix3x3.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>

#if defined TARGET_OS_IPHONE && defined ENABLE_QUICK_MATH
    #include <Accelerate/Accelerate.h>
#endif

namespace ChilliSource
{
	namespace Core
	{
		
		class CQuaternion;
		//================================================================
		/// Description:
		///
		/// 4x4 matrix container with a wrapper around matrix maths
		/// including transforms and multiplication
		//================================================================
		class CMatrix4x4
		{
		public:
            static const u32 kMatrixWidth = 4;
            static const u32 kMatrixDims  = kMatrixWidth * kMatrixWidth;
            
			CMatrix4x4();
			CMatrix4x4(const f32* paMatrix);
			CMatrix4x4(f32 m00, f32 m01, f32 m02, f32 m03,
                       f32 m10, f32 m11, f32 m12, f32 m13,
                       f32 m20, f32 m21, f32 m22, f32 m23,
                       f32 m30, f32 m31, f32 m32, f32 m33);
            CMatrix4x4(const CMatrix3x3& inmat);
			
			//---Methods
			CMatrix4x4 Inverse() const;
			CMatrix4x4 GetTranspose() const;

			CVector3 Right() const;
            CVector3 Up() const;
            CVector3 Forward() const;
            
			void Translate(f32 inX, f32 inY, f32 inZ);
			void Translate(const CVector3 &inVec);
			void RotateX(f32 infAngleRads);
			void RotateY(f32 infAngleRads);
			void RotateZ(f32 infAngleRads);
			
			void Rotate(f32 inXAxis, f32 inYAxis, f32 inZAxis, f32 infAngleRads);
			void Rotate(const CVector3 &vAxis, f32 infAngleRads);
			
			void Scale(f32 inScale);
			void Scale(f32 inX, f32 inY, f32 inZ);
			void Scale(const CVector3 &Vec);

			void LookAt(const CVector3 &vPos, const CVector3 &vTarget, const CVector3 &vUp);
			
			CVector3 GetTranslation() const;
			void SetTranslation(const CVector3& invTrans); 
			
			void SetTransform(const CVector3 & inTranslate, const CVector3 & inScale, const CQuaternion & inOrientation);
			void DecomposeTransforms(CVector3 & outTranslate, CVector3 & outScale, CQuaternion & outOrientation) const;
			f32 GetTrace();
			
			//---Operators
			f32 operator()(u32 inRow, u32 inColumn) const;

			CMatrix4x4 operator+(const CMatrix4x4 &rhs) const;			
			CMatrix4x4 operator-(const CMatrix4x4 &rhs) const;
			
			const CMatrix4x4& operator+=(const CMatrix4x4 &rhs);
			const CMatrix4x4& operator-=(const CMatrix4x4 &rhs);		
			const CMatrix4x4& operator*=(const CMatrix4x4 &rhs);		
			const CMatrix4x4& operator*=(f32 Scale);
			
			bool operator==(const CMatrix4x4 &rhs) const;
			
			bool operator!=(const CMatrix4x4 &rhs) const;
			
			CMatrix4x4 operator*(const CMatrix4x4 &rhs) const;

			
			static void Multiply(const CMatrix4x4 *  p1, const CMatrix4x4 *   p2, CMatrix4x4 *  pOut);
            static void Multiply(const CVector2* inpVec, const CMatrix4x4* inpMat, CVector2* outpVec);
            static void Multiply(const CVector3* inpVec, const CMatrix4x4* inpMat, CVector3* outpVec);
            static void Multiply(const CVector4* inpVec, const CMatrix4x4* inpMat, CVector4* outpVec);
			
            
            static CMatrix4x4 CreateOrthoMatrix(f32 infWidth, f32 infHeight, f32 infNear, f32 infFar);
            static CMatrix4x4 CreateOrthoMatrixOffset(f32 infLeft, f32 infRight, f32 infBottom, f32 infTop, f32 infNear, f32 infFar);
            
			static const CMatrix4x4 IDENTITY;
			
		public:
			f32 m[kMatrixDims];
		};
		
		inline void CMatrix4x4::Multiply(const CMatrix4x4* inp1, const CMatrix4x4* inp2, CMatrix4x4* outp3)
        {
#if defined TARGET_OS_IPHONE && defined ENABLE_QUICK_MATH
            MOFLOW_ASSERT(inp1 != outp3 && inp2 != outp3, "The input matrices must differ from the output Matrix (see vDSP_mmul difference between iOS6 and iOS7)");
            vDSP_mmul(const_cast<f32*>(inp1->m), 1, const_cast<f32*>(inp2->m), 1, outp3->m, 1, 4, 4, 4);
#else
			const f32* p1 = inp1->m;
			const f32* p2 = inp2->m;
			f32* pOut = outp3->m;
			
			pOut[0] = p1[0] * p2[0] + p1[1] * p2[4] + p1[2] * p2[8] + p1[3] * p2[12];
            pOut[1] = p1[0] * p2[1] + p1[1] * p2[5] + p1[2] * p2[9] + p1[3] * p2[13];
            pOut[2] = p1[0] * p2[2] + p1[1] * p2[6] + p1[2] * p2[10] + p1[3] * p2[14];
            pOut[3] = p1[0] * p2[3] + p1[1] * p2[7] + p1[2] * p2[11] + p1[3] * p2[15];
			
            pOut[4] = p1[4] * p2[0] + p1[5] * p2[4] + p1[6] * p2[8] + p1[7] * p2[12];
            pOut[5] = p1[4] * p2[1] + p1[5] * p2[5] + p1[6] * p2[9] + p1[7] * p2[13];
            pOut[6] = p1[4] * p2[2] + p1[5] * p2[6] + p1[6] * p2[10] + p1[7] * p2[14];
            pOut[7] = p1[4] * p2[3] + p1[5] * p2[7] + p1[6] * p2[11] + p1[7] * p2[15];
			
            pOut[8] = p1[8] * p2[0] + p1[9] * p2[4] + p1[10] * p2[8] + p1[11] * p2[12];
            pOut[9] = p1[8] * p2[1] + p1[9] * p2[5] + p1[10] * p2[9] + p1[11] * p2[13];
            pOut[10] = p1[8] * p2[2] + p1[9] * p2[6] + p1[10] * p2[10] + p1[11] * p2[14];
            pOut[11] = p1[8] * p2[3] + p1[9] * p2[7] + p1[10] * p2[11] + p1[11] * p2[15];
			
            pOut[12] = p1[12] * p2[0] + p1[13] * p2[4] + p1[14] * p2[8] + p1[15] * p2[12];
            pOut[13] = p1[12] * p2[1] + p1[13] * p2[5] + p1[14] * p2[9] + p1[15] * p2[13];
            pOut[14] = p1[12] * p2[2] + p1[13] * p2[6] + p1[14] * p2[10] + p1[15] * p2[14];
            pOut[15] = p1[12] * p2[3] + p1[13] * p2[7] + p1[14] * p2[11] + p1[15] * p2[15];
#endif
		}
        
        inline void CMatrix4x4::Multiply(const CVector2* inpVec, const CMatrix4x4* inpMat, CVector2* outpVec)
        {
            const f32* m = inpMat->m;
            
            outpVec->x = inpVec->x * m[0] + inpVec->y * m[4] + m[12];
			outpVec->y = inpVec->x * m[1] + inpVec->y * m[5] + m[13];
        }
        inline void CMatrix4x4::Multiply(const CVector3* inpVec, const CMatrix4x4* inpMat, CVector3* outpVec)
        {
            const f32* m = inpMat->m;
            
            outpVec->x = inpVec->x * m[0] + inpVec->y * m[4] + inpVec->z * m[8] + m[12];
			outpVec->y = inpVec->x * m[1] + inpVec->y * m[5] + inpVec->z * m[9] + m[13];
			outpVec->z = inpVec->x * m[2] + inpVec->y * m[6] + inpVec->z * m[10] + m[14];
        }
        inline void CMatrix4x4::Multiply(const CVector4* inpVec, const CMatrix4x4* inpMat, CVector4* outpVec)
        {
            const f32* m = inpMat->m;
            
            outpVec->x = inpVec->x * m[0] + inpVec->y * m[4] + inpVec->z * m[8] + inpVec->w * m[12];
			outpVec->y = inpVec->x * m[1] + inpVec->y * m[5] + inpVec->z * m[9] + inpVec->w * m[13];
			outpVec->z = inpVec->x * m[2] + inpVec->y * m[6] + inpVec->z * m[10] + inpVec->w * m[14];
			outpVec->w = inpVec->x * m[3] + inpVec->y * m[7] + inpVec->z * m[11] + inpVec->w * m[15];
        }
		
		CMatrix4x4 operator*(const CMatrix4x4 & inMat, f32 infScale);
		CMatrix4x4 operator*(f32 infScale, const CMatrix4x4 & inMat);

		inline CVector4 operator*(const CVector4& vec, const CMatrix4x4 &mat)
		{
			CVector4 Result; 
			
			Result.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[8] * vec.z + mat.m[12] * vec.w;
			Result.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[9] * vec.z + mat.m[13] * vec.w;
			Result.z = mat.m[2] * vec.x + mat.m[6] * vec.y + mat.m[10] * vec.z + mat.m[14] * vec.w;
			Result.w = mat.m[3] * vec.x + mat.m[7] * vec.y + mat.m[11] * vec.z + mat.m[15] * vec.w;
			
			return Result;
		}
		
		inline CVector3 operator*(const CVector3& vec, const CMatrix4x4 &mat)
		{
			CVector3 Result; 
	
			Result.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[8] * vec.z + mat.m[12];
			Result.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[9] * vec.z + mat.m[13];
			Result.z = mat.m[2] * vec.x + mat.m[6] * vec.y + mat.m[10] * vec.z + mat.m[14];
			
			return Result;
		}
        
        inline CVector2 operator*(const CVector2& vec, const CMatrix4x4 &mat)
		{
			CVector2 Result; 
    
			Result.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[12];
			Result.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[13];
			
			return Result;
		}
	}
}

#endif