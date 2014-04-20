/*
 *  Matrix4x4Old.cpp
 *  moFlo
 *
 *  Created by Scott Downie on 28/09/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Math/Matrix4x4Old.h>
#include <ChilliSource/Core/Math/QuaternionOld.h>

namespace ChilliSource
{
	namespace Core
	{
        Matrix4x4Old Matrix4x4Old::CreateOrthoMatrix(f32 infWidth, f32 infHeight, f32 infNear, f32 infFar)
        {
            f32 Depth = infFar - infNear;
            
            f32 A = 2.0f/infWidth;
            f32 B = 2.0f/infHeight;
            f32 C = 2.0f/Depth;
            
            f32 Tz = (infNear + infFar)/ Depth;
            
            return Core::Matrix4x4Old
            (
             A, 0,  0,  0, 
             0, B,  0,  0, 
             0, 0, -C,  0, 
             0, 0,  -Tz, 1
             );
        }
        
        Matrix4x4Old Matrix4x4Old::CreateOrthoMatrixOffset(f32 infLeft, f32 infRight, f32 infBottom, f32 infTop, f32 infNear, f32 infFar)
        {
			f32 Depth = infFar - infNear;
            
			f32 A = 2.0f/(infRight - infLeft);
			f32 B = 2.0f/(infTop - infBottom);
			f32 C = 2.0f/Depth;
            
			f32 Tx = (infRight + infLeft)/(infRight - infLeft);
			f32 Ty = (infTop + infBottom)/(infTop - infBottom);
			f32 Tz = (infNear + infFar)/Depth;
            
			return Core::Matrix4x4Old
            (
             A, 0,  0,  0, 
             0, B,  0,  0, 
             0, 0, -C,  0, 
             -Tx, -Ty,  -Tz, 1
             );
        }
		const Matrix4x4Old Matrix4x4Old::IDENTITY(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		//------------------------------------------------
		/// Constructor
		///
		/// Default - Initialises to identity matrix
		//------------------------------------------------
		Matrix4x4Old::Matrix4x4Old()
		{
			(*this) = Matrix4x4Old::IDENTITY;
		}
		//------------------------------------------------
		/// Constructor
		///
		/// Initialses matrix with values
		///
		/// @param 16 float elements
		//------------------------------------------------
		Matrix4x4Old::Matrix4x4Old(f32 m00, f32 m01, f32 m02, f32 m03,
                               f32 m10, f32 m11, f32 m12, f32 m13,
                               f32 m20, f32 m21, f32 m22, f32 m23,
                               f32 m30, f32 m31, f32 m32, f32 m33)
		{
			m[0]  = m00; m[1]  = m01; m[2]  = m02; m[3]  = m03;
			m[4]  = m10; m[5]  = m11; m[6]  = m12; m[7]  = m13;
			m[8]  = m20; m[9]  = m21; m[10] = m22; m[11] = m23;
			m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
		}
		//------------------------------------------------
		/// Constructor
		///
		/// Intialises matrix with the values given
		///
		/// @param Float array containing 16 values
		//------------------------------------------------
		Matrix4x4Old::Matrix4x4Old(const f32* paMatrix)
		{
			memcpy(&m, paMatrix, sizeof(f32) * kMatrixDims);
		}
        //------------------------------------------------
		/// Constructor
		///
		/// @param From 3x3 matrix
		//------------------------------------------------
        Matrix4x4Old::Matrix4x4Old(const Matrix3x3Old& inmat)
        {
            m[0]  = inmat.m[0]; m[1]  = inmat.m[1]; m[2]  = inmat.m[2]; m[3]  = 0.0f;
			m[4]  = inmat.m[3]; m[5]  = inmat.m[4]; m[6]  = inmat.m[5]; m[7]  = 0.0f;
			m[8]  = 0.0f;       m[9]  = 0.0f;       m[10] = 1.0f;       m[11] = 0.0f;
			m[12] = inmat.m[6]; m[13] = inmat.m[7]; m[14] = inmat.m[8]; m[15] = 1.0f;
        }
		//------------------------------------------------
		/// Inverse
		///
		///
		/// @param Inverse of the matrix
		//------------------------------------------------
		Matrix4x4Old Matrix4x4Old::Inverse() const
		{
			f32 m00 = m[0],  m01 = m[1],  m02 = m[2],  m03 = m[3];
			f32 m10 = m[4],  m11 = m[5],  m12 = m[6],  m13 = m[7];
			f32 m20 = m[8],  m21 = m[9],  m22 = m[10], m23 = m[11];
			f32 m30 = m[12], m31 = m[13], m32 = m[14], m33 = m[15];
			
			f32 v0 = m20 * m31 - m21 * m30;
			f32 v1 = m20 * m32 - m22 * m30;
			f32 v2 = m20 * m33 - m23 * m30;
			f32 v3 = m21 * m32 - m22 * m31;
			f32 v4 = m21 * m33 - m23 * m31;
			f32 v5 = m22 * m33 - m23 * m32;
			
			f32 t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
			f32 t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
			f32 t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
			f32 t30 = - (v3 * m10 - v1 * m11 + v0 * m12);
			
            f32 fDenom = (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);
            
            if (fDenom)
            {
                f32 invDet = 1 / fDenom;
                
                f32 d00 = t00 * invDet;
                f32 d10 = t10 * invDet;
                f32 d20 = t20 * invDet;
                f32 d30 = t30 * invDet;
                
                f32 d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
                f32 d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
                f32 d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
                f32 d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
                
                v0 = m10 * m31 - m11 * m30;
                v1 = m10 * m32 - m12 * m30;
                v2 = m10 * m33 - m13 * m30;
                v3 = m11 * m32 - m12 * m31;
                v4 = m11 * m33 - m13 * m31;
                v5 = m12 * m33 - m13 * m32;
                
                f32 d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
                f32 d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
                f32 d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
                f32 d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
                
                v0 = m21 * m10 - m20 * m11;
                v1 = m22 * m10 - m20 * m12;
                v2 = m23 * m10 - m20 * m13;
                v3 = m22 * m11 - m21 * m12;
                v4 = m23 * m11 - m21 * m13;
                v5 = m23 * m12 - m22 * m13;
                
                f32 d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
                f32 d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
                f32 d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
                f32 d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
                
                return Matrix4x4Old(
                               d00, d01, d02, d03,
                               d10, d11, d12, d13,
                               d20, d21, d22, d23,
                               d30, d31, d32, d33
                               );
            }
            else 
            {
                return Matrix4x4Old(IDENTITY);
            }
		}
		//------------------------------------------------
		/// Translate
		///
		/// Build a translation matrix
		/// @param X component
		/// @param Y component
		/// @param Z component
		//------------------------------------------------
		void Matrix4x4Old::Translate(f32 inX, f32 inY, f32 inZ)
		{
			Translate(Vector3(inX, inY, inZ));
		}
		//------------------------------------------------
		/// Translate
		///
		/// Build a translation matrix
		/// @param Translation vector
		//------------------------------------------------
		void Matrix4x4Old::Translate(const Vector3 &inVec)
		{
			(*this) = Matrix4x4Old::IDENTITY;
			
			m[12] = inVec.x;
			m[13] = inVec.y;
			m[14] = inVec.z;
		}
		//------------------------------------------------
		/// Transpose
		///
		/// Build a transposed matrix
		/// @param Translation vector
		//------------------------------------------------
		Matrix4x4Old Matrix4x4Old::GetTranspose() const
		{
			Matrix4x4Old cResult;
            
#if defined CS_TARGETPLATFORM_IOS && defined CS_ENABLE_FASTMATH
            vDSP_mtrans(const_cast<f32*>(m), 1, cResult.m, 1, 4, 4);
#else            
            cResult.m[0]  = m[0];
			cResult.m[1]  = m[4];
			cResult.m[2]  = m[8];
			cResult.m[3]  = m[12];
            
			cResult.m[4]  = m[1];
			cResult.m[5]  = m[5];
			cResult.m[6]  = m[9];
			cResult.m[7]  = m[13];
            
			cResult.m[8]  = m[2];
			cResult.m[9]  = m[6];
			cResult.m[10] = m[10];
			cResult.m[11] = m[14];
            
			cResult.m[12] = m[3];
			cResult.m[13] = m[7];
			cResult.m[14] = m[11];
			cResult.m[15] = m[15];
#endif

			return cResult;
		}
		//------------------------------------------------
		/// Rotate X
		///
		/// Build a rotation matrix about the x-axis
		/// @param Angle to rotate in radians
		//------------------------------------------------
		void Matrix4x4Old::RotateX(f32 infAngleRads)
		{
			(*this) = Matrix4x4Old::IDENTITY;
			
			f32 c = cosf(infAngleRads);
			f32 s = sinf(infAngleRads);

			m[5] = c;
			m[9] = s;
			m[6] = -s;
			m[10] = c;
		}
		//------------------------------------------------
		/// Rotate Y
		///
		/// Build a rotation matrix about the y-axis
		/// @param Angle to rotate in radians
		//------------------------------------------------
		void Matrix4x4Old::RotateY(f32 infAngleRads)
		{
			(*this) = Matrix4x4Old::IDENTITY;
			
			f32 c = cosf(infAngleRads);
			f32 s = sinf(infAngleRads);

			m[0] = c;
			m[2] = s;
			m[8] = -s;
			m[10] = c;
		}
		//------------------------------------------------
		/// Rotate Z
		///
		/// Build a rotation matrix about the z-axis
		/// @param Angle to rotate in radians
		//------------------------------------------------
		void Matrix4x4Old::RotateZ(f32 infAngleRads)
		{
			(*this) = Matrix4x4Old::IDENTITY;
			
			f32 c = cosf(infAngleRads);
			f32 s = sinf(infAngleRads);

			m[0] = c;
			m[4] = s;
			m[1] = -s;
			m[5] = c;
		}
		//------------------------------------------------
		/// Rotate 
		///
		/// Build a rotation matrix about an arbitrary axis
		/// @param X component
		/// @param Y component
		/// @param Z component
		/// @param Angle to rotate in radians
		//------------------------------------------------
		void Matrix4x4Old::Rotate(f32 inXAxis, f32 inYAxis, f32 inZAxis, f32 infAngleRads)
		{
			Rotate(Vector3(inXAxis, inYAxis, inZAxis), infAngleRads);
		}
		//------------------------------------------------
		/// Rotate 
		///
		/// Build a rotation matrix about an arbitrary axis
		/// @param Axis
		/// @param Angle to rotate in radians
		//------------------------------------------------
		void Matrix4x4Old::Rotate(const Vector3 &vAxis, f32 infAngleRads)
		{
			(*this) = Matrix4x4Old::IDENTITY;
			
			f32 c = cosf(infAngleRads);
			f32 s = sinf(infAngleRads);

			m[0] = c + (1 - c) * (vAxis.x * vAxis.x);				m[4] = (1 - c) * (vAxis.x * vAxis.y) - (s * vAxis.z);	m[8] = (1 - c) * (vAxis.x * vAxis.z) + (s * vAxis.y);
			m[1] = (1 - c) * (vAxis.x * vAxis.y) + (s * vAxis.z);	m[5] = c + (1 - c) * (vAxis.y * vAxis.y);				m[9] = (1 - c) * (vAxis.y * vAxis.z) - (s * vAxis.x);
			m[2] = (1 - c) * (vAxis.x * vAxis.z) - (s * vAxis.y);	m[6] = (1 - c) * (vAxis.y * vAxis.z) + (s * vAxis.x);	m[10] = c + (1 - c) * (vAxis.z * vAxis.z); 
		}
		//------------------------------------------------
		/// Scale 
		///
		/// Build a uniform scaling matrix
		/// @param Scale factor
		//------------------------------------------------
		void Matrix4x4Old::Scale(f32 inScale)
		{
			Scale(Vector3(inScale, inScale, inScale));
		}
		//------------------------------------------------
		/// Scale 
		///
		/// Build a scaling matrix
		/// @param X component
		/// @param Y component
		/// @param Z component
		/// @param Scale factor
		//------------------------------------------------
		void Matrix4x4Old::Scale(f32 inX, f32 inY, f32 inZ)
		{
			Scale(Vector3(inX, inY, inZ));
		}
		//------------------------------------------------
		/// Scale 
		///
		/// Build a scaling matrix 
		/// @param Scale dimensions vector
		//------------------------------------------------
		void Matrix4x4Old::Scale(const Vector3 &Vec)
		{
			(*this) = Matrix4x4Old::IDENTITY;
			
			m[0] = Vec.x;
			m[5] = Vec.y;
			m[10] = Vec.z;
		}
		//------------------------------------------------
		/// Look At 
		///
		/// Build a matrix that that has a direction 
		/// and orientation
		/// @param Position
		/// @param Direction vector
		/// @param Up vector
		//------------------------------------------------
		void Matrix4x4Old::LookAt(const Vector3 &vPos, const Vector3 &vTarget, const Vector3 &vUp)
		{
			(*this) = Matrix4x4Old::IDENTITY;
			
			Vector3 vZ = vPos - vTarget; 
			vZ.Normalise();
			
			Vector3 vX = Vector3::CrossProduct(vUp, vZ);
			Vector3 vY = Vector3::CrossProduct(vZ, vX);
			
			m[0] = vX.x; m[1] = vY.x; m[2]  = vZ.x; m[3]  = 0;
			m[4] = vX.y; m[5] = vY.y; m[6]  = vZ.y;	m[7]  = 0;
			m[8] = vX.z; m[9] = vY.z; m[10] = vZ.z;	m[11] = 0;
			m[12] = -Vector3::DotProduct(vX, vPos); m[13] = -Vector3::DotProduct(vY, vPos); m[14] = -Vector3::DotProduct(vZ, vPos); m[15] = 1.0f;
		}
		
        Vector3 Matrix4x4Old::Right() const
        {
            return Vector3(m[0], m[1], m[2]);
        }
        
        Vector3 Matrix4x4Old::Up() const
        {
            return Vector3(m[4], m[5], m[6]);
        }
        
        Vector3 Matrix4x4Old::Forward() const
        {
            return Vector3(m[8], m[9], m[10]);
        }
		Vector3 Matrix4x4Old::GetTranslation() const
		{
			return Vector3(m[12], m[13], m[14]);
		}
		void Matrix4x4Old::SetTranslation(const Vector3& invTrans) 
		{
			m[12] = invTrans.x; m[13] = invTrans.y; m[14] = invTrans.z;
		}
		void Matrix4x4Old::SetTransform(const Vector3 & inTranslate, const Vector3 & inScale, const QuaternionOld & inOrientation)
		{
			Matrix4x4Old rot;
			inOrientation.ToRotationMatrix(rot);
			
			// Set up final matrix with scale, rotation and translation
			m[0] = inScale.x * rot.m[0]; m[1] = inScale.x * rot.m[1]; m[2] = inScale.x * rot.m[2];	 m[3] = 0;
			m[4] = inScale.y * rot.m[4]; m[5] = inScale.y * rot.m[5]; m[6] = inScale.y * rot.m[6];	 m[7] = 0;
			m[8] = inScale.z * rot.m[8]; m[9] = inScale.z * rot.m[9]; m[10] = inScale.z * rot.m[10]; m[11] = 0;
			
			m[12] = inTranslate.x; m[13] = inTranslate.y; m[14] = inTranslate.z; m[15] = 1;
		}
		void Matrix4x4Old::DecomposeTransforms(Vector3 & outTranslate, Vector3 & outScale, QuaternionOld & outOrientation) const
        {
//			Matrix4x4Old mat3x3(
//			m[0], m[1], m[2], 0,
//			m[4], m[5], m[6], 0,
//		    m[8], m[9], m[10], 0,
//			0,    0,    0,    0);
			
			outTranslate.x = m[12];
			outTranslate.y = m[13]; 
			outTranslate.z = m[14];
			
			Matrix4x4Old kQ;
							  
			// Factor M = QR = QDU where Q is orthogonal, D is diagonal,
			// and U is upper triangular with ones on its diagonal.  Algorithm uses
			// Gram-Schmidt orthogonalization (the QR algorithm).
			//
			// If M = [ m0 | m1 | m2 ] and Q = [ q0 | q1 | q2 ], then
			//
			//   q0 = m0/|m0|
			//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
			//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
			//
			// where |V| indicates length of vector V and A*B indicates dot
			// product of vectors A and B.  The matrix R has entries
			//
			//   r00 = q0*m0  r01 = q0*m1  r02 = q0*m2
			//   r10 = 0      r11 = q1*m1  r12 = q1*m2
			//   r20 = 0      r21 = 0      r22 = q2*m2
			//
			// so D = diag(r00,r11,r22) and U has entries u01 = r01/r00,
			// u02 = r02/r00, and u12 = r12/r11.
			
			// Q = rotation
			// D = scaling
			// U = shear
			
			// D stores the three diagonal entries r00, r11, r22
			// U stores the entries U[0] = u01, U[1] = u02, U[2] = u12
			
			// build orthogonal matrix Q
			f32 fInvLength = 1.0f/sqrtf(m[0]*m[0]
											+ m[1]*m[1] +
											m[2]*m[2]);
			kQ.m[0] = m[0]*fInvLength;
			kQ.m[1] = m[1]*fInvLength;
			kQ.m[2] = m[2]*fInvLength;
			
			f32 fDot = kQ.m[0]*m[4] + kQ.m[1]*m[5] +
            kQ.m[2]*m[6];
			kQ.m[4] = m[4]-fDot*kQ.m[0];
			kQ.m[5] = m[5]-fDot*kQ.m[1];
			kQ.m[6] = m[6]-fDot*kQ.m[2];
			fInvLength = 1.0f/sqrtf(kQ.m[4]*kQ.m[4] + kQ.m[5]*kQ.m[5] +
									   kQ.m[6]*kQ.m[6]);
			kQ.m[4] *= fInvLength;
			kQ.m[5] *= fInvLength;
			kQ.m[6] *= fInvLength;
			
			fDot = kQ.m[0]*m[8] + kQ.m[1]*m[9] +
            kQ.m[2]*m[10];
			kQ.m[8] = m[8]-fDot*kQ.m[0];
			kQ.m[9] = m[9]-fDot*kQ.m[1];
			kQ.m[10] = m[10]-fDot*kQ.m[2];
			fDot = kQ.m[4]*m[8] + kQ.m[5]*m[9] +
            kQ.m[6]*m[10];
			kQ.m[8] -= fDot*kQ.m[4];
			kQ.m[9] -= fDot*kQ.m[5];
			kQ.m[10] -= fDot*kQ.m[6];
			fInvLength = 1.0f/sqrtf(kQ.m[8]*kQ.m[8] + kQ.m[9]*kQ.m[9] +
									   kQ.m[10]*kQ.m[10]);
			kQ.m[8] *= fInvLength;
			kQ.m[9] *= fInvLength;
			kQ.m[10] *= fInvLength;
			
			// guarantee that orthogonal matrix has determinant 1 (no reflections)
			f32 fDet = kQ.m[0]*kQ.m[5]*kQ.m[10] + kQ.m[4]*kQ.m[9]*kQ.m[2] +
            kQ.m[8]*kQ.m[1]*kQ.m[6] - kQ.m[8]*kQ.m[5]*kQ.m[2] -
            kQ.m[4]*kQ.m[1]*kQ.m[10] - kQ.m[0]*kQ.m[9]*kQ.m[6];
			
			if ( fDet < 0.0 )
			{
				for (u32 i = 0; i < 16; i++)
					kQ.m[i] = -kQ.m[i];
			}
			
			// build "right" matrix R
			Matrix4x4Old kR;
			kR.m[0] = kQ.m[0]*m[0] + kQ.m[1]*m[1] +
            kQ.m[2]*m[2];
			kR.m[4] = kQ.m[0]*m[4] + kQ.m[1]*m[5] +
            kQ.m[2]*m[6];
			kR.m[5] = kQ.m[4]*m[4] + kQ.m[5]*m[5] +
            kQ.m[6]*m[6];
			kR.m[8] = kQ.m[0]*m[8] + kQ.m[1]*m[9] +
            kQ.m[2]*m[10];
			kR.m[9] = kQ.m[4]*m[8] + kQ.m[5]*m[9] +
            kQ.m[6]*m[10];
			kR.m[10] = kQ.m[8]*m[8] + kQ.m[9]*m[9] +
            kQ.m[10]*m[10];
			
			// the scaling component
			outScale.x = kR.m[0];
			outScale.y = kR.m[5];
			outScale.z = kR.m[10];
			
			outOrientation = QuaternionOld(kQ);
			
			// the shear component
			/*f32 fInvD0 = 1.0f/kD[0];
			kU[0] = kR.m[4]*fInvD0;
			kU[1] = kR.m[8]*fInvD0;
			kU[2] = kR.m[9]/kD[1];			
			*/
			/*
			//Courtesy of the Admiral: http://www.gamedev.net/topic/441695-transform-matrix-decomposition/
			// (1,4) -> (0,3) -> 0 + 3*4

			outTranslate.x = m[12];
			outTranslate.y = m[13]; 
			outTranslate.z = m[14];

	
			f32 sX = sqrtf(m[0]*m[0] + m[4]*m[4] + m[8]*m[8]);
			f32 sY = sqrtf(m[1]*m[1] + m[5]*m[5] + m[9]*m[9]);
			f32 sZ = sqrtf(m[2]*m[2] + m[6]*m[6] + m[10]*m[10]);
			
			outScale.x = sX;
			outScale.y = sY;
			outScale.z = sZ;

			//Mrot = 
			//	M11/sx   M12/sx   M13/sx   0       
			//	M21/sy   M22/sy   M23/sy   0       
			//	M31/sz   M32/sz   M33/sz   0      
			//	0        0        0        1
			Matrix4x4Old matRot(
			m[0]/sX, m[4]/sX, m[8]/sX ,0,
			m[1]/sY, m[5]/sY, m[9]/sY ,0,
			m[2]/sZ, m[6]/sZ, m[10]/sZ ,0,
			0, 0, 0, 1);
			outOrientation = QuaternionOld(matRot);
			*/

		}

		f32 Matrix4x4Old::GetTrace()
		{
			f32 fTrace = 0.0f;
			for (u32 i = 0; i < kMatrixDims; i += (kMatrixWidth + 1))
				fTrace += m[i];
			return fTrace;
		}
		
		f32 Matrix4x4Old::operator()(u32 inRow, u32 inColumn) const
		{
			return m[inRow * kMatrixWidth + inColumn];
		}
		
		Matrix4x4Old Matrix4x4Old::operator+(const Matrix4x4Old &rhs) const
		{
			Matrix4x4Old Result; 
			for(u32 i=0; i<kMatrixDims; ++i)
			{
				Result.m[i] = this->m[i] + rhs.m[i];
			} 
			return Result;
		}
		
		Matrix4x4Old Matrix4x4Old::operator-(const Matrix4x4Old &rhs) const
		{
			Matrix4x4Old Result; 
			for(u32 i=0; i<kMatrixDims; ++i)
			{
				Result.m[i] = this->m[i] - rhs.m[i];
			} 
			return Result;
		}
		
		const Matrix4x4Old& Matrix4x4Old::operator+=(const Matrix4x4Old &rhs)
		{
			for(u32 i=0; i<Matrix4x4Old::kMatrixDims; ++i)
			{
				this->m[i] += rhs.m[i];
			} 
			return *this;
		}
		
		const Matrix4x4Old& Matrix4x4Old::operator-=(const Matrix4x4Old &rhs)
		{
			for(u32 i=0; i<Matrix4x4Old::kMatrixDims; ++i)
			{
				this->m[i] -= rhs.m[i];
			} 
			return *this;
		}
		
		const Matrix4x4Old& Matrix4x4Old::operator*=(const Matrix4x4Old &rhs)
		{
			(*this) = (*this) * rhs; 
			return *this;
		}
		
		const Matrix4x4Old& Matrix4x4Old::operator*=(f32 Scale)
		{
			for(u32 i=0; i<kMatrixDims; ++i)
			{
				this->m[i] *= Scale;
			}  
			return *this;
		}
		
		bool Matrix4x4Old::operator==(const Matrix4x4Old &rhs) const
		{
			return memcmp(&m, &rhs.m, sizeof(f32) * kMatrixDims) == 0;
		}
		
		bool Matrix4x4Old::operator!=(const Matrix4x4Old &rhs) const
		{
			return memcmp(&m, &rhs.m, sizeof(f32) * kMatrixDims) != 0;
		}
		
		Matrix4x4Old Matrix4x4Old::operator*(const Matrix4x4Old &m2) const
		{
			static Matrix4x4Old Result;

			Multiply(this, &m2, &Result);
			
			return Result;
		}
		
		Matrix4x4Old operator*(const Matrix4x4Old & inMat, f32 infScale){
			Matrix4x4Old Result; 
			for(u32 i=0; i<Matrix4x4Old::kMatrixDims; ++i)
			{
				Result.m[i] = inMat.m[i] * infScale;
			} 
			return Result;
		}
		Matrix4x4Old operator*(f32 infScale, const Matrix4x4Old & inMat)
        {
			return inMat * infScale;
		}
	}
}
