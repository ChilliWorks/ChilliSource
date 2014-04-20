/*
 *  Matrix4x4Old.h
 *  moFlo
 *
 *  Created by Scott Downie on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_MATH_MATRIX_3X3_H_
#define _MO_FLO_CORE_MATH_MATRIX_3X3_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2Old.h>
#include <ChilliSource/Core/Math/Matrix4x4Old.h>

#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
#include <Accelerate/Accelerate.h>
#endif

namespace ChilliSource
{
	namespace Core
	{
		//================================================================
		/// Description:
		///
		/// 3x3 matrix container with a wrapper around matrix maths
		/// including transforms and multiplication
		//===============================================================
		class Matrix3x3Old
		{
		public:
            static const u32 kMatrixWidth = 3;
            static const u32 kMatrixDims  = kMatrixWidth * kMatrixWidth;
            
			Matrix3x3Old();
			Matrix3x3Old(const f32* paMatrix);
			Matrix3x3Old(f32 m00, f32 m01, f32 m02, 
					   f32 m10, f32 m11, f32 m12, 
					   f32 m20, f32 m21, f32 m22);
			Matrix3x3Old(const class Matrix4x4Old& incMatrix4x4Old);
			
			//---Methods
			Matrix3x3Old GetTranspose() const;

			
			void Translate(f32 inX, f32 inY);
			void Translate(const Vector2Old &inVec);
	
			void Rotate(f32 infAngleRads);
			
			void Scale(f32 inScale);
			void Scale(f32 inX, f32 inY);
			void Scale(const Vector2Old &Vec);

			void SetTransform(const Vector2Old & inTranslate, const Vector2Old & inScale, f32 infAngleRads);
            void SetTranslation(const Vector2Old& invTranslation);
            
			//---Operators
			f32 operator()(u32 inRow, u32 inColumn) const;

			Matrix3x3Old operator+(const Matrix3x3Old &rhs) const;			
			Matrix3x3Old operator-(const Matrix3x3Old &rhs) const;
			
			const Matrix3x3Old& operator+=(const Matrix3x3Old &rhs);
			const Matrix3x3Old& operator-=(const Matrix3x3Old &rhs);		
			const Matrix3x3Old& operator*=(const Matrix3x3Old &rhs);		
			const Matrix3x3Old& operator*=(f32 Scale);
			
			bool operator==(const Matrix3x3Old &rhs) const;
			
			bool operator!=(const Matrix3x3Old &rhs) const;
			
			Matrix3x3Old operator*(const Matrix3x3Old &rhs) const;
			
			static void Multiply(const Matrix3x3Old *  p1, const Matrix3x3Old *   p2, Matrix3x3Old *  pOut);
            static void Multiply(const Vector2Old* inpVec, const Matrix3x3Old* inpMat, Vector2Old* outpVec);
			
			static const Matrix3x3Old IDENTITY;
			
		public:
			f32 m[kMatrixDims];
		};
		
		
		inline void Matrix3x3Old::Multiply(const Matrix3x3Old *  inp1, const Matrix3x3Old *  inp2, Matrix3x3Old *  inpOut)
        {
#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
            vDSP_mmul(const_cast<f32*>(inp1->m), 1, const_cast<f32*>(inp2->m), 1, inpOut->m, 1, 3, 3, 3);
#else
			const f32 *  p1 = inp1->m;
			const f32 *  p2 = inp2->m;
			f32 *  pOut = inpOut->m;
			
			pOut[0] = p1[0] * p2[0]     + p1[1] * p2[3]     + p1[2] * p2[6];
            pOut[1] = p1[0] * p2[1]     + p1[1] * p2[4]     + p1[2] * p2[7];
            pOut[2] = p1[0] * p2[2]     + p1[1] * p2[5]     + p1[2] * p2[8];
            
            pOut[3] = p1[3] * p2[0]     + p1[4] * p2[3]     + p1[5] * p2[6];
            pOut[4] = p1[3] * p2[1]     + p1[4] * p2[4]     + p1[5] * p2[7];
            pOut[5] = p1[3] * p2[2]     + p1[4] * p2[5]     + p1[5] * p2[8];
            
            pOut[6] = p1[6] * p2[0]     + p1[7] * p2[3]     + p1[8] * p2[6];
            pOut[7] = p1[6] * p2[1]     + p1[7] * p2[4]     + p1[8] * p2[7];
            pOut[8] = p1[6] * p2[2]     + p1[7] * p2[5]     + p1[8] * p2[8];
#endif
		}
        
        inline void Matrix3x3Old::Multiply(const Vector2Old* inpVec, const Matrix3x3Old* inpMat, Vector2Old* outpVec)
        {
            const f32* m = inpMat->m;
            
            outpVec->x = m[0] * inpVec->x + m[3] * inpVec->y + m[6];
			outpVec->y = m[1] * inpVec->x + m[4] * inpVec->y + m[7];
        }
		
		Matrix3x3Old operator*(const Matrix3x3Old & inMat, f32 infScale);
		Matrix3x3Old operator*(f32 infScale, const Matrix3x3Old & inMat);

        inline Vector2Old operator*(const Vector2Old& vec, const Matrix3x3Old &mat)
		{
			Vector2Old Result; 
    
			Result.x = mat.m[0] * vec.x + mat.m[3] * vec.y + mat.m[6];
			Result.y = mat.m[1] * vec.x + mat.m[4] * vec.y + mat.m[7];
			
			return Result;
		}
		inline Vector3Old operator*(const Vector3Old& vec, const Matrix3x3Old &mat)
		{
			Vector3Old Result; 
			
			Result.x = mat.m[0] * vec.x + mat.m[3] * vec.y + mat.m[6] * vec.z;
			Result.y = mat.m[1] * vec.x + mat.m[4] * vec.y + mat.m[7] * vec.z;
			Result.z = mat.m[2] * vec.x + mat.m[5] * vec.y + mat.m[8] * vec.z;
			
			return Result;
		}
	}
}

#endif