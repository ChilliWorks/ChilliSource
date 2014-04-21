/*
 *  Matrix4x4.h
 *  moFlo
 *
 *  Created by Scott Downie on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_MATH_MATRIX_3X3_H_
#define _MO_FLO_CORE_MATH_MATRIX_3X3_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Math/Vector2.h>
#include <ChilliSource/Core/Math/Matrix4x4.h>

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
		class Matrix3x3
		{
		public:
            
            static const u32 kMatrixWidth = 3;
            static const u32 kMatrixDims  = kMatrixWidth * kMatrixWidth;
            
			Matrix3x3();
            //------------------------------------------------
            /// Construct the matrix with the given transform
            ///
            /// @author S Downie
            ///
            /// @param Translation
            /// @param Scale
            /// @param Rotation in radians
            //------------------------------------------------
            Matrix3x3(const Vector2& in_translation, const Vector2& in_scale, f32 in_rotRads);
            
			Matrix3x3(const f32* paMatrix);
			Matrix3x3(f32 m00, f32 m01, f32 m02, 
					   f32 m10, f32 m11, f32 m12, 
					   f32 m20, f32 m21, f32 m22);
			Matrix3x3(const class Matrix4x4& incMatrix4x4);
			
			//---Methods
			Matrix3x3 GetTranspose() const;

			
			void Translate(f32 inX, f32 inY);
			void Translate(const Vector2 &inVec);
	
			void Rotate(f32 infAngleRads);
			
			void Scale(f32 inScale);
			void Scale(f32 inX, f32 inY);
			void Scale(const Vector2 &Vec);

			void SetTransform(const Vector2 & inTranslate, const Vector2 & inScale, f32 infAngleRads);
            void SetTranslation(const Vector2& invTranslation);
            
			//---Operators
			f32 operator()(u32 inRow, u32 inColumn) const;

			Matrix3x3 operator+(const Matrix3x3 &rhs) const;			
			Matrix3x3 operator-(const Matrix3x3 &rhs) const;
			
			const Matrix3x3& operator+=(const Matrix3x3 &rhs);
			const Matrix3x3& operator-=(const Matrix3x3 &rhs);		
			const Matrix3x3& operator*=(const Matrix3x3 &rhs);		
			const Matrix3x3& operator*=(f32 Scale);
			
			bool operator==(const Matrix3x3 &rhs) const;
			
			bool operator!=(const Matrix3x3 &rhs) const;
			
			Matrix3x3 operator*(const Matrix3x3 &rhs) const;
			
			static void Multiply(const Matrix3x3 *  p1, const Matrix3x3 *   p2, Matrix3x3 *  pOut);
            static void Multiply(const Vector2* inpVec, const Matrix3x3* inpMat, Vector2* outpVec);
			
			static const Matrix3x3 IDENTITY;
			
		public:
			f32 m[kMatrixDims];
		};
		
		
		inline void Matrix3x3::Multiply(const Matrix3x3 *  inp1, const Matrix3x3 *  inp2, Matrix3x3 *  inpOut)
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
        
        inline void Matrix3x3::Multiply(const Vector2* inpVec, const Matrix3x3* inpMat, Vector2* outpVec)
        {
            const f32* m = inpMat->m;
            
            outpVec->x = m[0] * inpVec->x + m[3] * inpVec->y + m[6];
			outpVec->y = m[1] * inpVec->x + m[4] * inpVec->y + m[7];
        }
		
		Matrix3x3 operator*(const Matrix3x3 & inMat, f32 infScale);
		Matrix3x3 operator*(f32 infScale, const Matrix3x3 & inMat);

        inline Vector2 operator*(const Vector2& vec, const Matrix3x3 &mat)
		{
			Vector2 Result; 
    
			Result.x = mat.m[0] * vec.x + mat.m[3] * vec.y + mat.m[6];
			Result.y = mat.m[1] * vec.x + mat.m[4] * vec.y + mat.m[7];
			
			return Result;
		}
		inline Vector3 operator*(const Vector3& vec, const Matrix3x3 &mat)
		{
			Vector3 Result; 
			
			Result.x = mat.m[0] * vec.x + mat.m[3] * vec.y + mat.m[6] * vec.z;
			Result.y = mat.m[1] * vec.x + mat.m[4] * vec.y + mat.m[7] * vec.z;
			Result.z = mat.m[2] * vec.x + mat.m[5] * vec.y + mat.m[8] * vec.z;
			
			return Result;
		}
	}
}

#endif