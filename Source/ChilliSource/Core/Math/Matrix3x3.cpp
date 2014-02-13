/*
 *  Matrix4x4.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <moFlo/Core/Math/Matrix3x3.h>

namespace moFlo
{
	namespace Core
	{
		const CMatrix3x3 CMatrix3x3::IDENTITY(1,0,0, 
                                              0,1,0, 
                                              0,0,1);
		//------------------------------------------------
		/// Constructor
		///
		/// Default - Initialises to identity matrix
		//------------------------------------------------
		CMatrix3x3::CMatrix3x3()
		{
			(*this) = CMatrix3x3::IDENTITY;
		}
		//------------------------------------------------
		/// Constructor
		///
		/// Initialses matrix with values
		///
		/// @param 16 float elements
		//------------------------------------------------
		CMatrix3x3::CMatrix3x3(f32 m00, f32 m01, f32 m02,
							   f32 m10, f32 m11, f32 m12,
							   f32 m20, f32 m21, f32 m22)
		{
			m[0] = m00; m[1] = m01; m[2] = m02; 
			m[3] = m10; m[4] = m11; m[5] = m12; 
			m[6] = m20; m[7] = m21; m[8] = m22; 
		}
		//------------------------------------------------
		/// Constructor
		///
		/// Intialises matrix with the values given
		///
		/// @param Float array containing 16 values
		//------------------------------------------------
		CMatrix3x3::CMatrix3x3(const f32* paMatrix)
		{
			memcpy(&m, paMatrix, sizeof(f32) * kMatrixDims);
		}
		//------------------------------------------------
		/// Constructor
		///
		/// Intialises matrix from a 4x4
		///
		/// @param incMatrix4x4 just copy the top-left 3x3 part
		//------------------------------------------------
		CMatrix3x3::CMatrix3x3(const CMatrix4x4& incMatrix4x4)
		{
			m[0] = incMatrix4x4.m[0];
			m[1] = incMatrix4x4.m[1];
			m[2] = incMatrix4x4.m[2];

			m[3] = incMatrix4x4.m[4];
			m[4] = incMatrix4x4.m[5];
			m[5] = incMatrix4x4.m[6];

			m[6] = incMatrix4x4.m[8];
			m[7] = incMatrix4x4.m[9];
			m[8] = incMatrix4x4.m[10];
		}
		//------------------------------------------------
		/// Translate
		///
		/// Build a translation matrix
		/// @param X component
		/// @param Y component
		//------------------------------------------------
		void CMatrix3x3::Translate(f32 inX, f32 inY)
		{
			Translate(CVector2(inX, inY));
		}
		//------------------------------------------------
		/// Translate
		///
		/// Build a translation matrix
		/// @param Translation vector
		//------------------------------------------------
		void CMatrix3x3::Translate(const CVector2 &inVec)
		{
			(*this) = CMatrix3x3::IDENTITY;
			
			m[6] = inVec.x;
			m[7] = inVec.y;
		}
		//------------------------------------------------
		/// Transpose
		///
		/// Build a transposed matrix
		/// @param Translation vector
		//------------------------------------------------
		CMatrix3x3 CMatrix3x3::GetTranspose() const
		{
			CMatrix3x3 cResult;
            
#if defined TARGET_OS_IPHONE && defined ENABLE_QUICK_MATH
            vDSP_mtrans(const_cast<f32*>(m), 1, cResult.m, 1, 3, 3);
#else
            cResult.m[0]  = m[0];
			cResult.m[1]  = m[3];
			cResult.m[2]  = m[6];
            
			cResult.m[3]  = m[1];
			cResult.m[4]  = m[4];
			cResult.m[5]  = m[7];
            
			cResult.m[6]  = m[2];
			cResult.m[7]  = m[5];
			cResult.m[8]  = m[8];
#endif

			return cResult;
		}
		//------------------------------------------------
		/// Rotate 
		///
		/// Build a rotation matrix about the z-axis
		/// @param Angle to rotate in radians
		//------------------------------------------------
		void CMatrix3x3::Rotate(f32 infAngleRads)
		{
			(*this) = CMatrix3x3::IDENTITY;
			
			f32 c = cosf(infAngleRads);
			f32 s = sinf(infAngleRads);

			m[0] = c;
			m[3] = s;
			m[1] = -s;
			m[4] = c;
		}
		//------------------------------------------------
		/// Scale 
		///
		/// Build a uniform scaling matrix
		/// @param Scale factor
		//------------------------------------------------
		void CMatrix3x3::Scale(f32 inScale)
		{
			Scale(CVector2(inScale, inScale));
		}
		//------------------------------------------------
		/// Scale 
		///
		/// Build a scaling matrix
		/// @param X component
		/// @param Y component
		/// @param Scale factor
		//------------------------------------------------
		void CMatrix3x3::Scale(f32 inX, f32 inY)
		{
			Scale(CVector2(inX, inY));
		}
		//------------------------------------------------
		/// Scale 
		///
		/// Build a scaling matrix 
		/// @param Scale dimensions vector
		//------------------------------------------------
		void CMatrix3x3::Scale(const CVector2 &Vec)
		{
			(*this) = CMatrix3x3::IDENTITY;
			
			m[0] = Vec.x;
			m[4] = Vec.y;
		}
		void CMatrix3x3::SetTransform(const CVector2 & inTranslate, const CVector2 & inScale, f32 infAngleRads)
		{
            f32 c = cosf(infAngleRads);
			f32 s = sinf(infAngleRads);
            
            //--Rotation and scale
			m[0] = c * inScale.x;
			m[3] = s;
			m[1] = -s;
			m[4] = c * inScale.y;
            
            //--Translation
            m[6] = inTranslate.x;
            m[7] = inTranslate.y;
            
            m[2] = 0.0f;
            m[5] = 0.0f;
            m[8] = 1.0f;
		}
        void CMatrix3x3::SetTranslation(const CVector2& invTranslation)
        {
            m[6] = invTranslation.x;
            m[7] = invTranslation.y;
        }

		f32 CMatrix3x3::operator()(u32 inRow, u32 inColumn) const
		{
			return m[inRow * CMatrix3x3::kMatrixWidth + inColumn];
		}
		
		CMatrix3x3 CMatrix3x3::operator+(const CMatrix3x3 &rhs) const
		{
			CMatrix3x3 Result; 
			for(u32 i=0; i<CMatrix3x3::kMatrixDims; ++i)
			{
				Result.m[i] = this->m[i] + rhs.m[i];
			} 
			return Result;
		}
		
		CMatrix3x3 CMatrix3x3::operator-(const CMatrix3x3 &rhs) const
		{
			CMatrix3x3 Result; 
			for(u32 i=0; i<CMatrix3x3::kMatrixDims; ++i)
			{
				Result.m[i] = this->m[i] - rhs.m[i];
			} 
			return Result;
		}
		
		const CMatrix3x3& CMatrix3x3::operator+=(const CMatrix3x3 &rhs)
		{
			for(u32 i=0; i<CMatrix3x3::kMatrixDims; ++i)
			{
				this->m[i] += rhs.m[i];
			} 
			return *this;
		}
		
		const CMatrix3x3& CMatrix3x3::operator-=(const CMatrix3x3 &rhs)
		{
			for(u32 i=0; i<CMatrix3x3::kMatrixDims; ++i)
			{
				this->m[i] -= rhs.m[i];
			} 
			return *this;
		}
		
		const CMatrix3x3& CMatrix3x3::operator*=(const CMatrix3x3 &rhs)
		{
			(*this) = (*this) * rhs; 
			return *this;
		}
		
		const CMatrix3x3& CMatrix3x3::operator*=(f32 Scale)
		{
			for(u32 i=0; i<kMatrixDims; ++i)
			{
				this->m[i] *= Scale;
			}  
			return *this;
		}
		
		bool CMatrix3x3::operator==(const CMatrix3x3 &rhs) const
		{
			return memcmp(&m, &rhs.m, sizeof(f32) * kMatrixDims) == 0;
		}
		
		bool CMatrix3x3::operator!=(const CMatrix3x3 &rhs) const
		{
			return memcmp(&m, &rhs.m, sizeof(f32) * kMatrixDims) != 0;
		}
	
		CMatrix3x3 CMatrix3x3::operator*(const CMatrix3x3 &m2) const
		{
			static CMatrix3x3 Result;

			Multiply(this, &m2, &Result);
			
			return Result;
		}
		
		CMatrix3x3 operator*(const CMatrix3x3 & inMat, f32 infScale)
        {
			CMatrix3x3 Result; 
			for(u32 i=0; i<CMatrix3x3::kMatrixDims; ++i)
			{
				Result.m[i] = inMat.m[i] * infScale;
			} 
			return Result;
		}
		CMatrix3x3 operator*(f32 infScale, const CMatrix3x3 & inMat)
        {
			return inMat * infScale;
		}		
	}
}
