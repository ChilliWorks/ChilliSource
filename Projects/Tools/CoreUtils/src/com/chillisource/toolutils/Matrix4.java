package com.chillisource.toolutils;

public class Matrix4 
{
	//-----------------------------------------------------
	/// Constants
	//-----------------------------------------------------
	public static final Matrix4 IDENTITY = new Matrix4();
	//-----------------------------------------------------
	/// Public member data
	//-----------------------------------------------------
	public float[] mafData;
	//-----------------------------------------------------
	/// Constructor
	//-----------------------------------------------------
	public Matrix4()
	{
		mafData = new float[16];
		mafData[0] = 1.0f; 	mafData[1] = 0.0f; 	mafData[2] = 0.0f; 	mafData[3] = 0.0f;
		mafData[4] = 0.0f; 	mafData[5] = 1.0f; 	mafData[6] = 0.0f; 	mafData[7] = 0.0f;
		mafData[8] = 0.0f; 	mafData[9] = 0.0f; 	mafData[10] = 1.0f; mafData[11] = 0.0f;
		mafData[12] = 0.0f; mafData[13] = 0.0f; mafData[14] = 0.0f; mafData[15] = 1.0f;
	}
	//-----------------------------------------------------
	/// Constructor
	//-----------------------------------------------------
	public Matrix4(float infA, float infB, float infC, float infD,
					float infE,	float infF,	float infG, float infH,
					float infI, float infJ, float infK, float infL,
					float infM, float infN, float infO, float infP)
	{
		mafData = new float[16];
		mafData[0] = infA; 	mafData[1] = infB; 	mafData[2] = infC; 	mafData[3] = infD;
		mafData[4] = infE; 	mafData[5] = infF; 	mafData[6] = infG; 	mafData[7] = infH;
		mafData[8] = infI; 	mafData[9] = infJ; 	mafData[10] = infK; mafData[11] = infL;
		mafData[12] = infM; mafData[13] = infN; mafData[14] = infO; mafData[15] = infP;
	}
	//-----------------------------------------------------
	/// Copy
	///
	/// Creates a copy of the matrix.
	///
	/// @return the copy.
	//-----------------------------------------------------
	public Matrix4 copy()
	{
		Matrix4 newMat = new Matrix4();

		for (int i = 0; i < 16; i++)
		{
			newMat.mafData[i] = mafData[i];
		}
		
		return newMat;
	}
	//-----------------------------------------------------
	/// Set
	///
	/// Sets a matrix to the value of another matrix.
	///
	/// @param the matrix to copy.
	//-----------------------------------------------------
	public void set(Matrix4 inMat)
	{
		for (int i = 0; i < 16; i++)
		{
			mafData[i] = inMat.mafData[i];
		}
	}
	//-----------------------------------------------------
	/// Get
	///
	/// returns the value at the given index.
	///
	/// @param the x index.
	/// @param the y index.
	/// @return the value at this index.
	//-----------------------------------------------------
	public float get(int indwX, int indwY)
	{
		return mafData[indwX + indwY * 4];
	}
	//-----------------------------------------------------
	/// Create Rotate X
	///
	/// Create a matrix describing a rotation around the X
	/// axis.
	///
	/// @param the angle to rotate through.
	/// @return the rotation matrix.
	//-----------------------------------------------------
	public static Matrix4 createRotateX(float infAngleRads)
	{
		Matrix4 newMat = new Matrix4();

		float c = (float)Math.cos(infAngleRads);
		float s = (float)Math.sin(infAngleRads);

		newMat.mafData[5] = c;
		newMat.mafData[9] = s;
		newMat.mafData[6] = -s;
		newMat.mafData[10] = c;
		
		return newMat;
	}
	//-----------------------------------------------------
	/// Create Rotate Y
	///
	/// Create a matrix describing a rotation around the Y
	/// axis.
	///
	/// @param the angle to rotate through.
	/// @return the rotation matrix.
	//-----------------------------------------------------
	public static Matrix4 createRotateY(float infAngleRads)
	{
		Matrix4 newMat = new Matrix4();

		float c = (float)Math.cos(infAngleRads);
		float s = (float)Math.sin(infAngleRads);

		newMat.mafData[0] = c;
		newMat.mafData[2] = s;
		newMat.mafData[8] = -s;
		newMat.mafData[10] = c;
		
		return newMat;
	}
	//-----------------------------------------------------
	/// Create Rotate Z
	///
	/// Create a matrix describing a rotation around the Z
	/// axis.
	///
	/// @param the angle to rotate through.
	/// @return the rotation matrix.
	//-----------------------------------------------------
	public static Matrix4 createRotateZ(float infAngleRads)
	{
		Matrix4 newMat = new Matrix4();

		float c = (float)Math.cos(infAngleRads);
		float s = (float)Math.sin(infAngleRads);

		newMat.mafData[0] = c;
		newMat.mafData[4] = s;
		newMat.mafData[1] = -s;
		newMat.mafData[5] = c;
		
		return newMat;
	}
	//-----------------------------------------------------
	/// Create Translation
	///
	/// Create a matrix describing a translation
	///
	/// @param the translation
	/// @return the translation matrix.
	//-----------------------------------------------------
	public static Matrix4 createTranslation(Vector3 inTranslation)
	{
		Matrix4 newMat = new Matrix4();

		newMat.mafData[12] = inTranslation.x;
		newMat.mafData[13] = inTranslation.y;
		newMat.mafData[14] = inTranslation.z;
		
		return newMat;
	}
	//-----------------------------------------------------
	/// Create Scale
	///
	/// Create a matrix describing scaling
	///
	/// @param the scale
	/// @return the scale matrix.
	//-----------------------------------------------------
	public static Matrix4 createScale(Vector3 inScale)
	{
		Matrix4 newMat = new Matrix4();

		newMat.mafData[0] = inScale.x;
		newMat.mafData[5] = inScale.y;
		newMat.mafData[10] = inScale.z;
		
		return newMat;
	}
	//-----------------------------------------------------
	/// Create Rotation
	///
	/// Create a matrix describing a rotation
	///
	/// @param the rotation
	/// @return the rotation matrix.
	//-----------------------------------------------------
	public static Matrix4 createRotation(Quaternion inRot)
	{
		Matrix4 newMat = new Matrix4();

		float c = (float)Math.cos(inRot.w);
		float s = (float)Math.sin(inRot.w);

		newMat.mafData[0] = c + (1 - c) * (inRot.x * inRot.x);				newMat.mafData[4] = (1 - c) * (inRot.x * inRot.y) - (s * inRot.z);	newMat.mafData[8] = (1 - c) * (inRot.x * inRot.z) + (s * inRot.y);
		newMat.mafData[1] = (1 - c) * (inRot.x * inRot.y) + (s * inRot.z);	newMat.mafData[5] = c + (1 - c) * (inRot.y * inRot.y);				newMat.mafData[9] = (1 - c) * (inRot.y * inRot.z) - (s * inRot.x);
		newMat.mafData[2] = (1 - c) * (inRot.x * inRot.z) - (s * inRot.y);	newMat.mafData[6] = (1 - c) * (inRot.y * inRot.z) + (s * inRot.x);	newMat.mafData[10] = c + (1 - c) * (inRot.z * inRot.z); 
	
		
		return newMat;
	}
	//-----------------------------------------------------
	/// Multiply
	///
	/// Multiplies this matrix by another.
	///
	/// @param the other matrix.
	/// @return the multiplied matrix.
	//-----------------------------------------------------
	public void multiply(Matrix4 inB)
	{
		Matrix4 copy = this.copy();
		
		mafData[0] = copy.mafData[0] * inB.mafData[0] + copy.mafData[1] * inB.mafData[4] + copy.mafData[2] * inB.mafData[8] + copy.mafData[3] * inB.mafData[12];
		mafData[1] = copy.mafData[0] * inB.mafData[1] + copy.mafData[1] * inB.mafData[5] + copy.mafData[2] * inB.mafData[9] + copy.mafData[3] * inB.mafData[13];
		mafData[2] = copy.mafData[0] * inB.mafData[2] + copy.mafData[1] * inB.mafData[6] + copy.mafData[2] * inB.mafData[10] + copy.mafData[3] * inB.mafData[14];
		mafData[3] = copy.mafData[0] * inB.mafData[3] + copy.mafData[1] * inB.mafData[7] + copy.mafData[2] * inB.mafData[11] + copy.mafData[3] * inB.mafData[15];
		
		mafData[4] = copy.mafData[4] * inB.mafData[0] + copy.mafData[5] * inB.mafData[4] + copy.mafData[6] * inB.mafData[8] + copy.mafData[7] * inB.mafData[12];
		mafData[5] = copy.mafData[4] * inB.mafData[1] + copy.mafData[5] * inB.mafData[5] + copy.mafData[6] * inB.mafData[9] + copy.mafData[7] * inB.mafData[13];
		mafData[6] = copy.mafData[4] * inB.mafData[2] + copy.mafData[5] * inB.mafData[6] + copy.mafData[6] * inB.mafData[10] + copy.mafData[7] * inB.mafData[14];
		mafData[7] = copy.mafData[4] * inB.mafData[3] + copy.mafData[5] * inB.mafData[7] + copy.mafData[6] * inB.mafData[11] + copy.mafData[7] * inB.mafData[15];
		
		mafData[8] = copy.mafData[8] * inB.mafData[0] + copy.mafData[9] * inB.mafData[4] + copy.mafData[10] * inB.mafData[8] + copy.mafData[11] * inB.mafData[12];
		mafData[9] = copy.mafData[8] * inB.mafData[1] + copy.mafData[9] * inB.mafData[5] + copy.mafData[10] * inB.mafData[9] + copy.mafData[11] * inB.mafData[13];
		mafData[10] = copy.mafData[8] * inB.mafData[2] + copy.mafData[9] * inB.mafData[6] + copy.mafData[10] * inB.mafData[10] + copy.mafData[11] * inB.mafData[14];
		mafData[11] = copy.mafData[8] * inB.mafData[3] + copy.mafData[9] * inB.mafData[7] + copy.mafData[10] * inB.mafData[11] + copy.mafData[11] * inB.mafData[15];
		
		mafData[12] = copy.mafData[12] * inB.mafData[0] + copy.mafData[13] * inB.mafData[4] + copy.mafData[14] * inB.mafData[8] + copy.mafData[15] * inB.mafData[12];
		mafData[13] = copy.mafData[12] * inB.mafData[1] + copy.mafData[13] * inB.mafData[5] + copy.mafData[14] * inB.mafData[9] + copy.mafData[15] * inB.mafData[13];
		mafData[14] = copy.mafData[12] * inB.mafData[2] + copy.mafData[13] * inB.mafData[6] + copy.mafData[14] * inB.mafData[10] + copy.mafData[15] * inB.mafData[14];
		mafData[15] = copy.mafData[12] * inB.mafData[3] + copy.mafData[13] * inB.mafData[7] + copy.mafData[14] * inB.mafData[11] + copy.mafData[15] * inB.mafData[15];
	}
	//-----------------------------------------------------
	/// Returns the rotation as euler angles.
	///
	/// The following algorithm has been taken from "Computing 
	/// Euler angles from a rotation matrix" by Gregory G. Slabaugh. 
	/// The original document can be found at: 
	//// http://www.gregslabaugh.name/publications/euler.pdf
	///
	/// Note: While the algorithm can return two sets of possible 
	/// answers, we are only returning 1 for simplicities sake.
	///	
	/// if (R31 != 1 AND R31 != -1)
	///		A1 = -asin(R31)
	///		A2 = PI - A1
	///		B1 = atan2(R32 / cos A1, R33 / cos A1)
	///		B2 = atan2(R32 / cos A2, R33 / cos A2)
	///		C1 = atan2(R21 / cos A1, R11 / cos A1)
	///		C2 = atan2(R21 / cos A2, R11 / cos A2)
	/// else
	///		C = anything, so set to 0.
	///		if (R31 == -1)
	///			A = PI / 2
	///			B = C + atan2(R12,R13)
	///		else
	///			A = -PI / 2
	///			B = -C + atan2(-R12,-R13)
	///		end if
	/// end if
	///
	/// @return the euler angles.
	//-----------------------------------------------------
	public Vector3 getEulerAngles()
	{
		Vector3 out = new Vector3();
		if (mafData[8] != 1.0f && mafData[8] != -1.0f)
		{
			//only take one set of angles.
			out.y = -(float)Math.asin((double)mafData[8]); 
			out.x = (float)Math.atan2(mafData[9] / (float)Math.cos(out.y), mafData[9] / (float)Math.cos(out.y));
			out.z = (float)Math.atan2(mafData[4] / (float)Math.cos(out.y), mafData[0] / (float)Math.cos(out.y));
		}
		else
		{
			out.z = 0.0f;
			if (mafData[8] == -1)
			{
				out.y = ((float)Math.PI) / 2.0f;
				out.x = out.z + (float)Math.atan2(mafData[1], mafData[2]);
			}
			else
			{
				out.y = -((float)Math.PI) / 2.0f;
				out.x = -out.z + (float)Math.atan2(-mafData[1], -mafData[2]);
			}
		}
		
		return out;
	}
	//-----------------------------------------------------
	/// Contains Scale
	///
	/// @return whether or not this contains scaling.
	//-----------------------------------------------------
	public boolean containsScale()
	{
		float fTolerance = 0.00001f;
		Matrix4 theCopy = this.copy();
		
		theCopy.mafData[12] = 0.0f;
		theCopy.mafData[13] = 0.0f;
		theCopy.mafData[14] = 0.0f;
		
		Vector3 upRotated = Matrix4.multiply(new Vector3(0.0f ,1.0f, 0.0f), theCopy);
		Vector3 rightRotated = Matrix4.multiply(new Vector3(1.0f ,0.0f, 0.0f), theCopy);
		Vector3 forwardRotated = Matrix4.multiply(new Vector3(0.0f ,0.0f, 1.0f), theCopy);
		
		if (upRotated.length() < 1.0f - fTolerance || upRotated.length() > 1.0f + fTolerance ||
			rightRotated.length() < 1.0f - fTolerance || rightRotated.length() > 1.0f + fTolerance ||
			forwardRotated.length() < 1.0f - fTolerance || forwardRotated.length() > 1.0f + fTolerance)
		{
			return true;
		}
		
		return false;
	}
	//-----------------------------------------------------
	/// Get Translation
	///
	/// @return the translation component of this matrix.
	//-----------------------------------------------------
	public Vector3 getTranslation()
	{
		Vector3 translation = new Vector3();
		
		translation.x = mafData[12];
		translation.y = mafData[13];
		translation.z = mafData[14];
		
		return translation;
	}
	//-----------------------------------------------------
	/// Create From 2D Array
	///
	/// @param a 2D float array.
	/// @return a new matrix.
	//-----------------------------------------------------
	public static Matrix4 createFrom2DArray(float[][] inafData)
	{
		Matrix4 m = new Matrix4();
		
		m.mafData[0] = inafData[0][0]; 	m.mafData[1] = inafData[1][0]; 	m.mafData[2] = inafData[2][0]; 	m.mafData[3] = inafData[3][0];
		m.mafData[4] = inafData[0][1]; 	m.mafData[5] = inafData[1][1]; 	m.mafData[6] = inafData[2][1]; 	m.mafData[7] = inafData[3][1];
		m.mafData[8] = inafData[0][2]; 	m.mafData[9] = inafData[1][2]; 	m.mafData[10] = inafData[2][2]; m.mafData[11] = inafData[3][2];
		m.mafData[12] = inafData[0][3]; m.mafData[13] = inafData[1][3];	m.mafData[14] = inafData[2][3]; m.mafData[15] = inafData[3][3];
		
		return m;
	}
	//-----------------------------------------------------
	/// Multiply
	///
	/// @param vector
	/// @param matrix.
	/// @return the transformed vector
	//-----------------------------------------------------
	public static Vector3 multiply(Vector3 inVector, Matrix4 inMatrix)
	{
		Vector3 out = new Vector3();
		out.x = inVector.x * inMatrix.mafData[0] + inVector.y * inMatrix.mafData[4] + inVector.z * inMatrix.mafData[8] + inMatrix.mafData[12];
		out.y = inVector.x * inMatrix.mafData[1] + inVector.y * inMatrix.mafData[5] + inVector.z * inMatrix.mafData[9] + inMatrix.mafData[13];
		out.z = inVector.x * inMatrix.mafData[2] + inVector.y * inMatrix.mafData[6] + inVector.z * inMatrix.mafData[10] + inMatrix.mafData[14];
		return out;
	}
	//-----------------------------------------------------
	/// Multiply
	///
	/// @param matrix
	/// @param matrix
	/// @return the transformed matrix.
	//-----------------------------------------------------
	public static Matrix4 multiply(Matrix4 inA, Matrix4 inB)
	{
		Matrix4 out = new Matrix4();
		
		out.mafData[0] = inA.mafData[0] * inB.mafData[0] + inA.mafData[1] * inB.mafData[4] + inA.mafData[2] * inB.mafData[8] + inA.mafData[3] * inB.mafData[12];
		out.mafData[1] = inA.mafData[0] * inB.mafData[1] + inA.mafData[1] * inB.mafData[5] + inA.mafData[2] * inB.mafData[9] + inA.mafData[3] * inB.mafData[13];
		out.mafData[2] = inA.mafData[0] * inB.mafData[2] + inA.mafData[1] * inB.mafData[6] + inA.mafData[2] * inB.mafData[10] + inA.mafData[3] * inB.mafData[14];
		out.mafData[3] = inA.mafData[0] * inB.mafData[3] + inA.mafData[1] * inB.mafData[7] + inA.mafData[2] * inB.mafData[11] + inA.mafData[3] * inB.mafData[15];
		
		out.mafData[4] = inA.mafData[4] * inB.mafData[0] + inA.mafData[5] * inB.mafData[4] + inA.mafData[6] * inB.mafData[8] + inA.mafData[7] * inB.mafData[12];
		out.mafData[5] = inA.mafData[4] * inB.mafData[1] + inA.mafData[5] * inB.mafData[5] + inA.mafData[6] * inB.mafData[9] + inA.mafData[7] * inB.mafData[13];
		out.mafData[6] = inA.mafData[4] * inB.mafData[2] + inA.mafData[5] * inB.mafData[6] + inA.mafData[6] * inB.mafData[10] + inA.mafData[7] * inB.mafData[14];
		out.mafData[7] = inA.mafData[4] * inB.mafData[3] + inA.mafData[5] * inB.mafData[7] + inA.mafData[6] * inB.mafData[11] + inA.mafData[7] * inB.mafData[15];
		
		out.mafData[8] = inA.mafData[8] * inB.mafData[0] + inA.mafData[9] * inB.mafData[4] + inA.mafData[10] * inB.mafData[8] + inA.mafData[11] * inB.mafData[12];
		out.mafData[9] = inA.mafData[8] * inB.mafData[1] + inA.mafData[9] * inB.mafData[5] + inA.mafData[10] * inB.mafData[9] + inA.mafData[11] * inB.mafData[13];
		out.mafData[10] = inA.mafData[8] * inB.mafData[2] + inA.mafData[9] * inB.mafData[6] + inA.mafData[10] * inB.mafData[10] + inA.mafData[11] * inB.mafData[14];
		out.mafData[11] = inA.mafData[8] * inB.mafData[3] + inA.mafData[9] * inB.mafData[7] + inA.mafData[10] * inB.mafData[11] + inA.mafData[11] * inB.mafData[15];
		
		out.mafData[12] = inA.mafData[12] * inB.mafData[0] + inA.mafData[13] * inB.mafData[4] + inA.mafData[14] * inB.mafData[8] + inA.mafData[15] * inB.mafData[12];
		out.mafData[13] = inA.mafData[12] * inB.mafData[1] + inA.mafData[13] * inB.mafData[5] + inA.mafData[14] * inB.mafData[9] + inA.mafData[15] * inB.mafData[13];
		out.mafData[14] = inA.mafData[12] * inB.mafData[2] + inA.mafData[13] * inB.mafData[6] + inA.mafData[14] * inB.mafData[10] + inA.mafData[15] * inB.mafData[14];
		out.mafData[15] = inA.mafData[12] * inB.mafData[3] + inA.mafData[13] * inB.mafData[7] + inA.mafData[14] * inB.mafData[11] + inA.mafData[15] * inB.mafData[15];
		
		return out;
	}
	//-----------------------------------------------------
	/// to String
	///
	/// @return the matrix in string form.
	//-----------------------------------------------------
	public String toString()
	{
		String output = new String();
		output += "[\t" + mafData[0] + "\t" + mafData[1] + "\t" + mafData[2] + "\t" + mafData[3] + "\t]\n";
		output += "[\t" + mafData[4] + "\t" + mafData[5] + "\t" + mafData[6] + "\t" + mafData[7] + "\t]\n";
		output += "[\t" + mafData[8] + "\t" + mafData[9] + "\t" + mafData[10] + "\t" + mafData[11] + "\t]\n";
		output += "[\t" + mafData[12] + "\t" + mafData[13] + "\t" + mafData[14] + "\t" + mafData[15] + "\t]\n";
		return output;
	}
	//-----------------------------------------------------
	/// Swap Y and Z
	///
	/// @return the matrix in string form.
	//-----------------------------------------------------
	public Matrix4 swapYandZ()
	{
		Matrix4 output = new Matrix4();
		
		output.mafData[0] = mafData[0]; output.mafData[1] = mafData[2]; output.mafData[2] = mafData[1]; output.mafData[3] = mafData[3];
		output.mafData[4] = mafData[8]; output.mafData[5] = mafData[10];output.mafData[6] = mafData[9]; output.mafData[7] = mafData[11];
		output.mafData[8] = mafData[4]; output.mafData[9] = mafData[6]; output.mafData[10]= mafData[5]; output.mafData[11]= mafData[7];
		output.mafData[12]= mafData[12];output.mafData[13]= mafData[14];output.mafData[14]= mafData[13];output.mafData[15]= mafData[15];
		
		return output;
	}
	//-----------------------------------------------------
	/// to String
	///
	/// @param the translation
	/// @param the scale.
	/// @param the orientation.
	/// @return the transform.
	//-----------------------------------------------------
	public static Matrix4 composeTransforms(Vector3 inTranslate, Vector3 inScale, Quaternion inOrientation)
	{
		Matrix4 o = new Matrix4();
		Matrix4 rot = inOrientation.toMatrix();
		
		// Set up final matrix with scale, rotation and translation
		o.mafData[0] = inScale.x * rot.mafData[0]; 				o.mafData[1] = inScale.x * rot.mafData[1]; 		o.mafData[2] = inScale.x * rot.mafData[2];	 	o.mafData[3] = 0;
		o.mafData[4] = inScale.y * rot.mafData[4];				o.mafData[5] = inScale.y * rot.mafData[5]; 		o.mafData[6] = inScale.y * rot.mafData[6];	 	o.mafData[7] = 0;
		o.mafData[8] = inScale.z * rot.mafData[8]; 				o.mafData[9] = inScale.z * rot.mafData[9]; 		o.mafData[10] = inScale.z * rot.mafData[10]; 	o.mafData[11] = 0;
		o.mafData[12] = inTranslate.x; 							o.mafData[13] = inTranslate.y; 					o.mafData[14] = inTranslate.z; 					o.mafData[15] = 1;
	
		return o;
	}
	//-----------------------------------------------------
	/// Inverse
	///
	/// @return the inverse of this matrix.
	//-----------------------------------------------------
	public Matrix4 inverse()
	{
		float m00 = mafData[0],  m01 = mafData[1],  m02 = mafData[2],  m03 = mafData[3];
		float m10 = mafData[4],  m11 = mafData[5],  m12 = mafData[6],  m13 = mafData[7];
		float m20 = mafData[8],  m21 = mafData[9],  m22 = mafData[10], m23 = mafData[11];
		float m30 = mafData[12], m31 = mafData[13], m32 = mafData[14], m33 = mafData[15];
		
		float v0 = m20 * m31 - m21 * m30;
		float v1 = m20 * m32 - m22 * m30;
		float v2 = m20 * m33 - m23 * m30;
		float v3 = m21 * m32 - m22 * m31;
		float v4 = m21 * m33 - m23 * m31;
		float v5 = m22 * m33 - m23 * m32;
		
		float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
		float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
		float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
		float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);
		
		float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);
		
		float d00 = t00 * invDet;
		float d10 = t10 * invDet;
		float d20 = t20 * invDet;
		float d30 = t30 * invDet;
		
		float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		
		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;
		
		float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		
		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;
		
		float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		
		return new Matrix4(
					   d00, d01, d02, d03,
					   d10, d11, d12, d13,
					   d20, d21, d22, d23,
					   d30, d31, d32, d33
					   );
	}
	//-----------------------------------------------------
	/// DecomposeTransforms
	///
	/// This breaks a transform up into its translation,
	/// scale and orienation components.
	///
	/// @param OUT: The translation.
	/// @param OUT: The scale.
	/// @param OUT: The orientation.
	//-----------------------------------------------------
	public void decomposeTransforms(Vector3 outTranslate, Vector3 outScale, Quaternion outOrientation)
    {
		Matrix4 mat3x3 = new Matrix4();
		mat3x3.mafData[0] = mafData[0]; 	mat3x3.mafData[1] = mafData[1]; 	mat3x3.mafData[2] = mafData[2]; 	mat3x3.mafData[3] = 0;
		mat3x3.mafData[4] = mafData[4]; 	mat3x3.mafData[5] = mafData[5]; 	mat3x3.mafData[6] = mafData[6]; 	mat3x3.mafData[7] = 0;
		mat3x3.mafData[8] = mafData[8]; 	mat3x3.mafData[9] = mafData[9]; 	mat3x3.mafData[10] = mafData[10]; 	mat3x3.mafData[11] = 0;
		mat3x3.mafData[12] = 0; 			mat3x3.mafData[13] =  0; 			mat3x3.mafData[14] = 0; 			mat3x3.mafData[15] = 0;
		
		outTranslate.x = mafData[12];
		outTranslate.y = mafData[13]; 
		outTranslate.z = mafData[14];
		
		Matrix4 kQ = new Matrix4();
						  
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
		float fInvLength = 1.0f / (float)Math.sqrt(mafData[0] * mafData[0] + mafData[1] * mafData[1] + mafData[2] * mafData[2]);
		kQ.mafData[0] = mafData[0] * fInvLength;
		kQ.mafData[1] = mafData[1] * fInvLength;
		kQ.mafData[2] = mafData[2] * fInvLength;
		
		float fDot = kQ.mafData[0]*mafData[4] + kQ.mafData[1]*mafData[5] + kQ.mafData[2]*mafData[6];
		kQ.mafData[4] = mafData[4]-fDot*kQ.mafData[0];
		kQ.mafData[5] = mafData[5]-fDot*kQ.mafData[1];
		kQ.mafData[6] = mafData[6]-fDot*kQ.mafData[2];
		fInvLength = 1.0f/(float)Math.sqrt(kQ.mafData[4]*kQ.mafData[4] + kQ.mafData[5]*kQ.mafData[5] + kQ.mafData[6]*kQ.mafData[6]);
		kQ.mafData[4] *= fInvLength;
		kQ.mafData[5] *= fInvLength;
		kQ.mafData[6] *= fInvLength;
		
		fDot = kQ.mafData[0]*mafData[8] + kQ.mafData[1]*mafData[9] + kQ.mafData[2]*mafData[10];
		kQ.mafData[8] = mafData[8]-fDot*kQ.mafData[0];
		kQ.mafData[9] = mafData[9]-fDot*kQ.mafData[1];
		kQ.mafData[10] = mafData[10]-fDot*kQ.mafData[2];
		fDot = kQ.mafData[4]*mafData[8] + kQ.mafData[5]*mafData[9] + kQ.mafData[6]*mafData[10];
		kQ.mafData[8] -= fDot*kQ.mafData[4];
		kQ.mafData[9] -= fDot*kQ.mafData[5];
		kQ.mafData[10] -= fDot*kQ.mafData[6];
		fInvLength = 1.0f/(float)Math.sqrt(kQ.mafData[8]*kQ.mafData[8] + kQ.mafData[9]*kQ.mafData[9] + kQ.mafData[10]*kQ.mafData[10]);
		kQ.mafData[8] *= fInvLength;
		kQ.mafData[9] *= fInvLength;
		kQ.mafData[10] *= fInvLength;
		
		// guarantee that orthogonal matrix has determinant 1 (no reflections)
		float fDet = 	kQ.mafData[0]*kQ.mafData[5]*kQ.mafData[10] + kQ.mafData[4]*kQ.mafData[9]*kQ.mafData[2] +
        				kQ.mafData[8]*kQ.mafData[1]*kQ.mafData[6] - kQ.mafData[8]*kQ.mafData[5]*kQ.mafData[2] -
        				kQ.mafData[4]*kQ.mafData[1]*kQ.mafData[10] - kQ.mafData[0]*kQ.mafData[9]*kQ.mafData[6];
		
		if ( fDet < 0.0 )
		{
			for (int i = 0; i < 16; i++)
				kQ.mafData[i] = -kQ.mafData[i];
		}
		
		// build "right" matrix R
		Matrix4 kR = new Matrix4();
		kR.mafData[0] = kQ.mafData[0]*mafData[0] + kQ.mafData[1]*mafData[1] + kQ.mafData[2]*mafData[2];
		kR.mafData[4] = kQ.mafData[0]*mafData[4] + kQ.mafData[1]*mafData[5] + kQ.mafData[2]*mafData[6];
		kR.mafData[5] = kQ.mafData[4]*mafData[4] + kQ.mafData[5]*mafData[5] + kQ.mafData[6]*mafData[6];
		kR.mafData[8] = kQ.mafData[0]*mafData[8] + kQ.mafData[1]*mafData[9] + kQ.mafData[2]*mafData[10];
		kR.mafData[9] = kQ.mafData[4]*mafData[8] + kQ.mafData[5]*mafData[9] + kQ.mafData[6]*mafData[10];
		kR.mafData[10] = kQ.mafData[8]*mafData[8] + kQ.mafData[9]*mafData[9] + kQ.mafData[10]*mafData[10];
		
		// the scaling component
		outScale.x = kR.mafData[0];
		outScale.y = kR.mafData[5];
		outScale.z = kR.mafData[10];
		
		//shearing component
		//float fInvScaleX = 1.0f/outScale.x;
		//outShear.x = kR.mafData[4]*fInvScaleX;
		//outShear.y = kR.mafData[8]*fInvScaleX;
		//outShear.z = kR.mafData[9]/outScale.y;
		
		outOrientation.set(Quaternion.createFromMatrix(kQ));
	}
}
