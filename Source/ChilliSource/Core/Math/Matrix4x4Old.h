/*
 *  Matrix4x4Old.h
 *  moFlo
 *
 *  Created by Scott Downie on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_MATH_MATRIX_4X4_H_
#define _MO_FLO_CORE_MATH_MATRIX_4X4_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Matrix3x3Old.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Vector4.h>

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
    #include <Accelerate/Accelerate.h>
#endif

namespace ChilliSource
{
	namespace Core
	{
		
		class QuaternionOld;
		//================================================================
		/// Description:
		///
		/// 4x4 matrix container with a wrapper around matrix maths
		/// including transforms and multiplication
		//================================================================
		class Matrix4x4Old
		{
		public:
            static const u32 kMatrixWidth = 4;
            static const u32 kMatrixDims  = kMatrixWidth * kMatrixWidth;
            
			Matrix4x4Old();
			Matrix4x4Old(const f32* paMatrix);
			Matrix4x4Old(f32 m00, f32 m01, f32 m02, f32 m03,
                       f32 m10, f32 m11, f32 m12, f32 m13,
                       f32 m20, f32 m21, f32 m22, f32 m23,
                       f32 m30, f32 m31, f32 m32, f32 m33);
            Matrix4x4Old(const Matrix3x3Old& inmat);
			
			//---Methods
			Matrix4x4Old Inverse() const;
			Matrix4x4Old GetTranspose() const;

			Vector3 Right() const;
            Vector3 Up() const;
            Vector3 Forward() const;
            
			void Translate(f32 inX, f32 inY, f32 inZ);
			void Translate(const Vector3 &inVec);
			void RotateX(f32 infAngleRads);
			void RotateY(f32 infAngleRads);
			void RotateZ(f32 infAngleRads);
			
			void Rotate(f32 inXAxis, f32 inYAxis, f32 inZAxis, f32 infAngleRads);
			void Rotate(const Vector3 &vAxis, f32 infAngleRads);
			
			void Scale(f32 inScale);
			void Scale(f32 inX, f32 inY, f32 inZ);
			void Scale(const Vector3 &Vec);

			void LookAt(const Vector3 &vPos, const Vector3 &vTarget, const Vector3 &vUp);
			
			Vector3 GetTranslation() const;
			void SetTranslation(const Vector3& invTrans); 
			
			void SetTransform(const Vector3 & inTranslate, const Vector3 & inScale, const QuaternionOld & inOrientation);
			void DecomposeTransforms(Vector3 & outTranslate, Vector3 & outScale, QuaternionOld & outOrientation) const;
			f32 GetTrace();
			
			//---Operators
			f32 operator()(u32 inRow, u32 inColumn) const;

			Matrix4x4Old operator+(const Matrix4x4Old &rhs) const;			
			Matrix4x4Old operator-(const Matrix4x4Old &rhs) const;
			
			const Matrix4x4Old& operator+=(const Matrix4x4Old &rhs);
			const Matrix4x4Old& operator-=(const Matrix4x4Old &rhs);		
			const Matrix4x4Old& operator*=(const Matrix4x4Old &rhs);		
			const Matrix4x4Old& operator*=(f32 Scale);
			
			bool operator==(const Matrix4x4Old &rhs) const;
			
			bool operator!=(const Matrix4x4Old &rhs) const;
			
			Matrix4x4Old operator*(const Matrix4x4Old &rhs) const;

			
			static void Multiply(const Matrix4x4Old *  p1, const Matrix4x4Old *   p2, Matrix4x4Old *  pOut);
            static void Multiply(const Vector2* inpVec, const Matrix4x4Old* inpMat, Vector2* outpVec);
            static void Multiply(const Vector3* inpVec, const Matrix4x4Old* inpMat, Vector3* outpVec);
            static void Multiply(const Vector4* inpVec, const Matrix4x4Old* inpMat, Vector4* outpVec);
			
            
            static Matrix4x4Old CreateOrthoMatrix(f32 infWidth, f32 infHeight, f32 infNear, f32 infFar);
            static Matrix4x4Old CreateOrthoMatrixOffset(f32 infLeft, f32 infRight, f32 infBottom, f32 infTop, f32 infNear, f32 infFar);
            
			static const Matrix4x4Old IDENTITY;
			
		public:
			f32 m[kMatrixDims];
		};
		
		inline void Matrix4x4Old::Multiply(const Matrix4x4Old* inp1, const Matrix4x4Old* inp2, Matrix4x4Old* outp3)
        {
#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
            CS_ASSERT(inp1 != outp3 && inp2 != outp3, "The input matrices must differ from the output Matrix (see vDSP_mmul difference between iOS6 and iOS7)");
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
        
        inline void Matrix4x4Old::Multiply(const Vector2* inpVec, const Matrix4x4Old* inpMat, Vector2* outpVec)
        {
            const f32* m = inpMat->m;
            
            outpVec->x = inpVec->x * m[0] + inpVec->y * m[4] + m[12];
			outpVec->y = inpVec->x * m[1] + inpVec->y * m[5] + m[13];
        }
        inline void Matrix4x4Old::Multiply(const Vector3* inpVec, const Matrix4x4Old* inpMat, Vector3* outpVec)
        {
            const f32* m = inpMat->m;
            
            outpVec->x = inpVec->x * m[0] + inpVec->y * m[4] + inpVec->z * m[8] + m[12];
			outpVec->y = inpVec->x * m[1] + inpVec->y * m[5] + inpVec->z * m[9] + m[13];
			outpVec->z = inpVec->x * m[2] + inpVec->y * m[6] + inpVec->z * m[10] + m[14];
        }
        inline void Matrix4x4Old::Multiply(const Vector4* inpVec, const Matrix4x4Old* inpMat, Vector4* outpVec)
        {
            const f32* m = inpMat->m;
            
            outpVec->x = inpVec->x * m[0] + inpVec->y * m[4] + inpVec->z * m[8] + inpVec->w * m[12];
			outpVec->y = inpVec->x * m[1] + inpVec->y * m[5] + inpVec->z * m[9] + inpVec->w * m[13];
			outpVec->z = inpVec->x * m[2] + inpVec->y * m[6] + inpVec->z * m[10] + inpVec->w * m[14];
			outpVec->w = inpVec->x * m[3] + inpVec->y * m[7] + inpVec->z * m[11] + inpVec->w * m[15];
        }
		
		Matrix4x4Old operator*(const Matrix4x4Old & inMat, f32 infScale);
		Matrix4x4Old operator*(f32 infScale, const Matrix4x4Old & inMat);

		inline Vector4 operator*(const Vector4& vec, const Matrix4x4Old &mat)
		{
			Vector4 Result; 
			
			Result.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[8] * vec.z + mat.m[12] * vec.w;
			Result.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[9] * vec.z + mat.m[13] * vec.w;
			Result.z = mat.m[2] * vec.x + mat.m[6] * vec.y + mat.m[10] * vec.z + mat.m[14] * vec.w;
			Result.w = mat.m[3] * vec.x + mat.m[7] * vec.y + mat.m[11] * vec.z + mat.m[15] * vec.w;
			
			return Result;
		}
		
		inline Vector3 operator*(const Vector3& vec, const Matrix4x4Old &mat)
		{
			Vector3 Result; 
	
			Result.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[8] * vec.z + mat.m[12];
			Result.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[9] * vec.z + mat.m[13];
			Result.z = mat.m[2] * vec.x + mat.m[6] * vec.y + mat.m[10] * vec.z + mat.m[14];
			
			return Result;
		}
        
        inline Vector2 operator*(const Vector2& vec, const Matrix4x4Old &mat)
		{
			Vector2 Result; 
    
			Result.x = mat.m[0] * vec.x + mat.m[4] * vec.y + mat.m[12];
			Result.y = mat.m[1] * vec.x + mat.m[5] * vec.y + mat.m[13];
			
			return Result;
		}
	}
}

#endif