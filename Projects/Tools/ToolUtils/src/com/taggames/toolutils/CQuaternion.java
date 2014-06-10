package com.taggames.toolutils;

public class CQuaternion 
{
	//------------------------------------------------------------
	/// Public member data
	//------------------------------------------------------------
	public float x;
	public float y;
	public float z;
	public float w;
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public CQuaternion()
	{
		x = y = z = w = 0.0f;
	}
	//------------------------------------------------------------
	/// Set
	///
	/// Sets this quaternion to the value of the given quaternion.
	///
	/// @param the quaternion to copy.
	//------------------------------------------------------------
	public void Set(CQuaternion inQuat)
	{
		x = inQuat.x;
		y = inQuat.y;
		z = inQuat.z;
		w = inQuat.w;
	}
	//------------------------------------------------------------
	/// To Matrix
	///
	/// @return the matrix equivalent of this quaternion.
	//------------------------------------------------------------
	public CMatrix4 ToMatrix()
	{
		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;
		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;
		
		CMatrix4 output = new CMatrix4();
		output.mafData[0] = 1.0f - 2.0f * (y2 + z2);	output.mafData[1] = 2.0f * (xy + wz);			output.mafData[2] = 2.0f * (xz - wy);			output.mafData[3] = 0.0f;
		output.mafData[4] = 2.0f * (xy - wz);			output.mafData[5] = 1.0f - 2.0f * (x2 + z2);	output.mafData[6] = 2.0f * (yz + wx);			output.mafData[7] = 0.0f;
		output.mafData[8] = 2.0f * (xz + wy);			output.mafData[9] = 2.0f * (yz - wx);			output.mafData[10] = 1.0f - 2.0f * (x2 + y2);	output.mafData[11] = 0.0f;
		output.mafData[12] = 0.0f;						output.mafData[13] = 0.0f;						output.mafData[14] = 0.0f;						output.mafData[15] = 1.0f;
		
		return output;
	}
	//------------------------------------------------------------
	/// Get Axis
	///
	/// @return the axis
	//------------------------------------------------------------
	public CVector3 GetAxis()
	{
		float fSqrLength = (x * x + y * y + z * z);

		if (fSqrLength > 0.0f)
		{
			float fInvLength = 1.0f / (float)Math.sqrt(fSqrLength);
			return new CVector3(x * fInvLength, y * fInvLength, z * fInvLength);
		}

		return new CVector3(1.0f, 0.0f, 0.0f);
	}
	//------------------------------------------------------------
	/// Get Angle
	///
	/// @return the angle.
	//------------------------------------------------------------
	public float GetAngle()
	{
		float fSqrLength = (x * x + y * y + z * z);

		if (fSqrLength > 0.0f)
		{
			return 2.0f * (float)Math.acos(w);
		} 

		return 0.0f;
	}
	//------------------------------------------------------------
	/// Normalise
	///
	/// @return a normalised version of this quaternion
	//------------------------------------------------------------
	public CQuaternion Normalise()
	{
		CQuaternion output = new CQuaternion();
		float s = 1.0f / (float)Math.sqrt(w * w + x * x + y * y + z * z);
		output.w = w * s;
		output.x = x * s;
		output.y = y * s;
		output.z = z * s;

		return output;
	}
	//------------------------------------------------------------
	/// Create From Matrix
	///
	/// Converts a rotation matrix into a quaternion.
	///
	/// @param the rotation matrix.
	/// @return the quaterion.
	//------------------------------------------------------------
	public static CQuaternion CreateFromMatrix(CMatrix4 inMat)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".
		
		CQuaternion newQuat = new CQuaternion();
		
		float fTrace = inMat.mafData[0]+inMat.mafData[5]+inMat.mafData[10] + 1.0f;
		float fRoot = 0.0f;
		
		if (fTrace > 0.0)
		{
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = (float)Math.sqrt(fTrace);  // 2w
			newQuat.w = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;  // 1/(4w)
			newQuat.x = (inMat.mafData[6]-inMat.mafData[9])*fRoot;
			newQuat.y = (inMat.mafData[8]-inMat.mafData[2])*fRoot;
			newQuat.z = (inMat.mafData[1]-inMat.mafData[4])*fRoot;
		}
		else
		{
			// |w| <= 1/2
			int[] s_iNext = { 1, 2, 0 };
			int i = 0;
			if ( inMat.mafData[5] > inMat.mafData[0] )
				i = 1;
			if ( inMat.mafData[10] > inMat.Get(i,i) )
				i = 2;
			int j = s_iNext[i];
			int k = s_iNext[j];
			
			fRoot = (float)Math.sqrt(inMat.Get(i,i)-inMat.Get(j,j)-inMat.Get(k,k) + 1.0f);
			
			float[] apkQuat = {0.0f, 0.0f, 0.0f};
			apkQuat[i] = 0.5f*fRoot;
			fRoot = 0.5f/fRoot;
			newQuat.w = (inMat.Get(k,j)-inMat.Get(j,k))*fRoot;
			apkQuat[j] = (inMat.Get(j,i)+inMat.Get(i,j))*fRoot;
			apkQuat[k] = (inMat.Get(k,i)+inMat.Get(i,k))*fRoot;
			
			newQuat.x = apkQuat[0];
			newQuat.y = apkQuat[1];
			newQuat.z = apkQuat[2];
		}

		return newQuat;
	}
	//------------------------------------------------------------
	/// Create From Axis Angle
	///
	/// Creates a new quaternion from an axis and an angle.
	///
	/// @param the axis.
	/// @param the angle.
	/// @return the quaterion.
	//------------------------------------------------------------
	public static CQuaternion CreateFromAxisAngle(CVector3 invAxis, float infAngle)
	{
		CQuaternion output = new CQuaternion();
		CVector3 Vnorm = invAxis.Normalise();

		float fHalfAngle = infAngle / 2.0f;
		float fSinAngle = (float)Math.sin(fHalfAngle);
		output.x = Vnorm.x * fSinAngle;
		output.y = Vnorm.y * fSinAngle;
		output.z = Vnorm.z * fSinAngle;
		output.w = (float)Math.cos(fHalfAngle);
		
		return output;
	}
}
