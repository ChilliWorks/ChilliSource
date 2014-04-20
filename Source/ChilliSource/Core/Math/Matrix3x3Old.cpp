/*
 *  Matrix4x4Old.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Math/Matrix3x3Old.h>

namespace ChilliSource
{
	namespace Core
	{
		const Matrix3x3Old Matrix3x3Old::IDENTITY(1,0,0, 
                                              0,1,0, 
                                              0,0,1);
		//------------------------------------------------
		/// Constructor
		///
		/// Default - Initialises to identity matrix
		//------------------------------------------------
		Matrix3x3Old::Matrix3x3Old()
		{
			(*this) = Matrix3x3Old::IDENTITY;
		}
		//------------------------------------------------
		/// Constructor
		///
		/// Initialses matrix with values
		///
		/// @param 16 float elements
		//------------------------------------------------
		Matrix3x3Old::Matrix3x3Old(f32 m00, f32 m01, f32 m02,
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
		Matrix3x3Old::Matrix3x3Old(const f32* paMatrix)
		{
			memcpy(&m, paMatrix, sizeof(f32) * kMatrixDims);
		}
		//------------------------------------------------
		/// Constructor
		///
		/// Intialises matrix from a 4x4
		///
		/// @param incMatrix4x4Old just copy the top-left 3x3 part
		//------------------------------------------------
		Matrix3x3Old::Matrix3x3Old(const Matrix4x4Old& incMatrix4x4Old)
		{
			m[0] = incMatrix4x4Old.m[0];
			m[1] = incMatrix4x4Old.m[1];
			m[2] = incMatrix4x4Old.m[2];

			m[3] = incMatrix4x4Old.m[4];
			m[4] = incMatrix4x4Old.m[5];
			m[5] = incMatrix4x4Old.m[6];

			m[6] = incMatrix4x4Old.m[8];
			m[7] = incMatrix4x4Old.m[9];
			m[8] = incMatrix4x4Old.m[10];
		}
		//------------------------------------------------
		/// Translate
		///
		/// Build a translation matrix
		/// @param X component
		/// @param Y component
		//------------------------------------------------
		void Matrix3x3Old::Translate(f32 inX, f32 inY)
		{
			Translate(Vector2Old(inX, inY));
		}
		//------------------------------------------------
		/// Translate
		///
		/// Build a translation matrix
		/// @param Translation vector
		//------------------------------------------------
		void Matrix3x3Old::Translate(const Vector2Old &inVec)
		{
			(*this) = Matrix3x3Old::IDENTITY;
			
			m[6] = inVec.x;
			m[7] = inVec.y;
		}
		//------------------------------------------------
		/// Transpose
		///
		/// Build a transposed matrix
		/// @param Translation vector
		//------------------------------------------------
		Matrix3x3Old Matrix3x3Old::GetTranspose() const
		{
			Matrix3x3Old cResult;
            
#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
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
		void Matrix3x3Old::Rotate(f32 infAngleRads)
		{
			(*this) = Matrix3x3Old::IDENTITY;
			
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
		void Matrix3x3Old::Scale(f32 inScale)
		{
			Scale(Vector2Old(inScale, inScale));
		}
		//------------------------------------------------
		/// Scale 
		///
		/// Build a scaling matrix
		/// @param X component
		/// @param Y component
		/// @param Scale factor
		//------------------------------------------------
		void Matrix3x3Old::Scale(f32 inX, f32 inY)
		{
			Scale(Vector2Old(inX, inY));
		}
		//------------------------------------------------
		/// Scale 
		///
		/// Build a scaling matrix 
		/// @param Scale dimensions vector
		//------------------------------------------------
		void Matrix3x3Old::Scale(const Vector2Old &Vec)
		{
			(*this) = Matrix3x3Old::IDENTITY;
			
			m[0] = Vec.x;
			m[4] = Vec.y;
		}
		void Matrix3x3Old::SetTransform(const Vector2Old & inTranslate, const Vector2Old & inScale, f32 infAngleRads)
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
        void Matrix3x3Old::SetTranslation(const Vector2Old& invTranslation)
        {
            m[6] = invTranslation.x;
            m[7] = invTranslation.y;
        }

		f32 Matrix3x3Old::operator()(u32 inRow, u32 inColumn) const
		{
			return m[inRow * Matrix3x3Old::kMatrixWidth + inColumn];
		}
		
		Matrix3x3Old Matrix3x3Old::operator+(const Matrix3x3Old &rhs) const
		{
			Matrix3x3Old Result; 
			for(u32 i=0; i<Matrix3x3Old::kMatrixDims; ++i)
			{
				Result.m[i] = this->m[i] + rhs.m[i];
			} 
			return Result;
		}
		
		Matrix3x3Old Matrix3x3Old::operator-(const Matrix3x3Old &rhs) const
		{
			Matrix3x3Old Result; 
			for(u32 i=0; i<Matrix3x3Old::kMatrixDims; ++i)
			{
				Result.m[i] = this->m[i] - rhs.m[i];
			} 
			return Result;
		}
		
		const Matrix3x3Old& Matrix3x3Old::operator+=(const Matrix3x3Old &rhs)
		{
			for(u32 i=0; i<Matrix3x3Old::kMatrixDims; ++i)
			{
				this->m[i] += rhs.m[i];
			} 
			return *this;
		}
		
		const Matrix3x3Old& Matrix3x3Old::operator-=(const Matrix3x3Old &rhs)
		{
			for(u32 i=0; i<Matrix3x3Old::kMatrixDims; ++i)
			{
				this->m[i] -= rhs.m[i];
			} 
			return *this;
		}
		
		const Matrix3x3Old& Matrix3x3Old::operator*=(const Matrix3x3Old &rhs)
		{
			(*this) = (*this) * rhs; 
			return *this;
		}
		
		const Matrix3x3Old& Matrix3x3Old::operator*=(f32 Scale)
		{
			for(u32 i=0; i<kMatrixDims; ++i)
			{
				this->m[i] *= Scale;
			}  
			return *this;
		}
		
		bool Matrix3x3Old::operator==(const Matrix3x3Old &rhs) const
		{
			return memcmp(&m, &rhs.m, sizeof(f32) * kMatrixDims) == 0;
		}
		
		bool Matrix3x3Old::operator!=(const Matrix3x3Old &rhs) const
		{
			return memcmp(&m, &rhs.m, sizeof(f32) * kMatrixDims) != 0;
		}
	
		Matrix3x3Old Matrix3x3Old::operator*(const Matrix3x3Old &m2) const
		{
			static Matrix3x3Old Result;

			Multiply(this, &m2, &Result);
			
			return Result;
		}
		
		Matrix3x3Old operator*(const Matrix3x3Old & inMat, f32 infScale)
        {
			Matrix3x3Old Result; 
			for(u32 i=0; i<Matrix3x3Old::kMatrixDims; ++i)
			{
				Result.m[i] = inMat.m[i] * infScale;
			} 
			return Result;
		}
		Matrix3x3Old operator*(f32 infScale, const Matrix3x3Old & inMat)
        {
			return inMat * infScale;
		}		
	}
}
