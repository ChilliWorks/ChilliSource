/**
 * Matrix4.java
 * Chilli Source
 * Created by Ian Copland on 30/01/2013
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.chilliworks.chillisource.coreutils;

/**
 * A basic 4x4 matrix math class. This is immutable after construction to ensure it is safe
 * to pass into methods.
 * 
 * @author Ian Copland
 */
public final class Matrix4 
{
	public static final Matrix4 IDENTITY = new Matrix4(
			1.0, 0.0, 0.0, 0.0,  
			0.0, 1.0, 0.0, 0.0,  
			0.0, 0.0, 1.0, 0.0,  
			0.0, 0.0, 0.0, 1.0);

	private double[] m_data;

	/**
	 * Creates a new matrix which describes the given translation.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_translation - The translation the matrix should describe.
	 * 
	 * @return The translation matrix.
	 */
	public static Matrix4 createTranslation(Vector3 in_translation)
	{
		return new Matrix4(1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 0.0, 0.0,
				in_translation.getX(), in_translation.getY(), in_translation.getZ(), 1.0);
	}
	/**
	 * Creates a new matrix which describes the given scale.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_scale - The scale the matrix should describe.
	 * 
	 * @return The scale matrix.
	 */
	public static Matrix4 createScale(Vector3 in_scale)
	{
		return new Matrix4(in_scale.getX(), 0.0, 0.0, 0.0,
				0.0, in_scale.getY(), 0.0, 0.0,
				0.0, 0.0, in_scale.getZ(), 0.0,
				0.0, 0.0, 0.0, 1.0);
	}
	/**
	 * Creates a new rotation matrix which describes the given rotation
	 * around the X axis.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_angle - The angle around the X axis in radians.
	 * 
	 * @return The rotation matrix.
	 */
	public static Matrix4 createRotateX(double in_angle)
	{
		double cosA = Math.cos(in_angle);
		double sinA = Math.sin(in_angle);
		
		return new Matrix4(1.0, 0.0, 0.0, 0.0,
				0.0, cosA, sinA, 0.0,
				0.0, -sinA, cosA, 0.0,
				0.0, 0.0, 0.0, 1.0);
	}
	/**
	 * Creates a new rotation matrix which describes the given rotation
	 * around the Y axis.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_angle - The angle around the Y axis in radians.
	 * 
	 * @return The rotation matrix.
	 */
	public static Matrix4 createRotateY(double in_angle)
	{
		double cosA = Math.cos(in_angle);
		double sinA = Math.sin(in_angle);
		
		return new Matrix4(cosA, 0.0, -sinA, 0.0,
				0.0, 1.0, 0.0, 0.0,
				sinA, 0.0, cosA, 0.0,
				0.0, 0.0, 0.0, 1.0);
	}
	/**
	 * Creates a new rotation matrix which describes the given rotation
	 * around the Z axis.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_angle - The angle around the Z axis in radians.
	 * 
	 * @return The rotation matrix.
	 */
	public static Matrix4 createRotateZ(double in_angle)
	{
		double cosA = Math.cos(in_angle);
		double sinA = Math.sin(in_angle);
		
		return new Matrix4(cosA, sinA, 0.0, 0.0,
				-sinA, cosA, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0);
	}
	/**
	 * Creates a rotation matrix that describes the rotation given by
	 * the input quaternion.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_orientation - The orientation quaternion.
	 * 
	 * @return The rotation matrix.
	 */
	public static Matrix4 createRotation(Quaternion in_orientation)
	{
		double cosW = Math.cos(in_orientation.getW());
		double sinW = Math.sin(in_orientation.getW());
		
		double a0 = cosW + (1 - cosW) * (in_orientation.getX() * in_orientation.getX());
		double a1 = (1 - cosW) * (in_orientation.getX() * in_orientation.getY()) + (sinW * in_orientation.getZ());
		double a2 = (1 - cosW) * (in_orientation.getX() * in_orientation.getZ()) - (sinW * in_orientation.getY());

		double b0 = (1 - cosW) * (in_orientation.getX() * in_orientation.getY()) - (sinW * in_orientation.getZ());
		double b1 = cosW + (1 - cosW) * (in_orientation.getY() * in_orientation.getY());
		double b2 = (1 - cosW) * (in_orientation.getY() * in_orientation.getZ()) + (sinW * in_orientation.getX());

		double c0 = (1 - cosW) * (in_orientation.getX() * in_orientation.getZ()) + (sinW * in_orientation.getY());
		double c1 = (1 - cosW) * (in_orientation.getY() * in_orientation.getZ()) - (sinW * in_orientation.getX());
		double c2 = cosW + (1 - cosW) * (in_orientation.getZ() * in_orientation.getZ());
	
		return new Matrix4(a0, a1, a2, 0.0,
				b0, b1, b2, 0.0,
				c0, c1, c2, 0.0,
				0.0, 0.0, 0.0, 1.0);
	}
	/**
	 * Creates a new transform matrix from the given translation, scale and orientation.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_translation - The translation vector.
	 * @param in_scale - The scale vector.
	 * @param in_orientation - The orientation quaternion.
	 * 
	 * @return The new transform matrix.
	 */
	public static Matrix4 createTransform(Vector3 in_translation, Vector3 in_scale, Quaternion in_orientation)
	{
		Matrix4 rot = in_orientation.toMatrix();
		
		return new Matrix4(in_scale.getX() * rot.m_data[0], in_scale.getX() * rot.m_data[1], in_scale.getX() * rot.m_data[2], 0.0,
				in_scale.getY() * rot.m_data[4], in_scale.getY() * rot.m_data[5], in_scale.getY() * rot.m_data[6], 0.0,
				in_scale.getZ() * rot.m_data[8], in_scale.getZ() * rot.m_data[9], in_scale.getZ() * rot.m_data[10], 0.0,
				in_translation.getX(), in_translation.getY(), in_translation.getZ(), 1.0);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_a - The first matrix.
	 * @param in_b - The second matrix.
	 * 
	 * @return The result of multiplying the two matrices together.
	 */
	public static Matrix4 multiply(Matrix4 inA, Matrix4 inB)
	{
		double a0 = inA.m_data[0] * inB.m_data[0] + inA.m_data[1] * inB.m_data[4] + inA.m_data[2] * inB.m_data[8] + inA.m_data[3] * inB.m_data[12];
		double a1 = inA.m_data[0] * inB.m_data[1] + inA.m_data[1] * inB.m_data[5] + inA.m_data[2] * inB.m_data[9] + inA.m_data[3] * inB.m_data[13];
		double a2 = inA.m_data[0] * inB.m_data[2] + inA.m_data[1] * inB.m_data[6] + inA.m_data[2] * inB.m_data[10] + inA.m_data[3] * inB.m_data[14];
		double a3 = inA.m_data[0] * inB.m_data[3] + inA.m_data[1] * inB.m_data[7] + inA.m_data[2] * inB.m_data[11] + inA.m_data[3] * inB.m_data[15];
		
		double b0 = inA.m_data[4] * inB.m_data[0] + inA.m_data[5] * inB.m_data[4] + inA.m_data[6] * inB.m_data[8] + inA.m_data[7] * inB.m_data[12];
		double b1 = inA.m_data[4] * inB.m_data[1] + inA.m_data[5] * inB.m_data[5] + inA.m_data[6] * inB.m_data[9] + inA.m_data[7] * inB.m_data[13];
		double b2 = inA.m_data[4] * inB.m_data[2] + inA.m_data[5] * inB.m_data[6] + inA.m_data[6] * inB.m_data[10] + inA.m_data[7] * inB.m_data[14];
		double b3 = inA.m_data[4] * inB.m_data[3] + inA.m_data[5] * inB.m_data[7] + inA.m_data[6] * inB.m_data[11] + inA.m_data[7] * inB.m_data[15];
		
		double c0 = inA.m_data[8] * inB.m_data[0] + inA.m_data[9] * inB.m_data[4] + inA.m_data[10] * inB.m_data[8] + inA.m_data[11] * inB.m_data[12];
		double c1 = inA.m_data[8] * inB.m_data[1] + inA.m_data[9] * inB.m_data[5] + inA.m_data[10] * inB.m_data[9] + inA.m_data[11] * inB.m_data[13];
		double c2 = inA.m_data[8] * inB.m_data[2] + inA.m_data[9] * inB.m_data[6] + inA.m_data[10] * inB.m_data[10] + inA.m_data[11] * inB.m_data[14];
		double c3 = inA.m_data[8] * inB.m_data[3] + inA.m_data[9] * inB.m_data[7] + inA.m_data[10] * inB.m_data[11] + inA.m_data[11] * inB.m_data[15];
		
		double d0 = inA.m_data[12] * inB.m_data[0] + inA.m_data[13] * inB.m_data[4] + inA.m_data[14] * inB.m_data[8] + inA.m_data[15] * inB.m_data[12];
		double d1 = inA.m_data[12] * inB.m_data[1] + inA.m_data[13] * inB.m_data[5] + inA.m_data[14] * inB.m_data[9] + inA.m_data[15] * inB.m_data[13];
		double d2 = inA.m_data[12] * inB.m_data[2] + inA.m_data[13] * inB.m_data[6] + inA.m_data[14] * inB.m_data[10] + inA.m_data[15] * inB.m_data[14];
		double d3 = inA.m_data[12] * inB.m_data[3] + inA.m_data[13] * inB.m_data[7] + inA.m_data[14] * inB.m_data[11] + inA.m_data[15] * inB.m_data[15];
		
		return new Matrix4(a0, a1, a2, a3,
				b0, b1, b2, b3,
				c0, c1, c2, c3,
				d0, d1, d2, d3);
	}
	/**
	 * Returns a new matrix that is the inverse of the given matrix.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_mat - The matrix to get the inverse of.
	 * 
	 * @return The inverse matrix.
	 */
	public static Matrix4 inverse(Matrix4 in_mat)
	{
		double m00 = in_mat.m_data[0]; double m01 = in_mat.m_data[1]; double m02 = in_mat.m_data[2]; double m03 = in_mat.m_data[3];
		double m10 = in_mat.m_data[4]; double m11 = in_mat.m_data[5]; double m12 = in_mat.m_data[6]; double m13 = in_mat.m_data[7];
		double m20 = in_mat.m_data[8]; double m21 = in_mat.m_data[9]; double m22 = in_mat.m_data[10]; double m23 = in_mat.m_data[11];
		double m30 = in_mat.m_data[12]; double m31 = in_mat.m_data[13]; double m32 = in_mat.m_data[14]; double m33 = in_mat.m_data[15];
		
		double v0 = m20 * m31 - m21 * m30;
		double v1 = m20 * m32 - m22 * m30;
		double v2 = m20 * m33 - m23 * m30;
		double v3 = m21 * m32 - m22 * m31;
		double v4 = m21 * m33 - m23 * m31;
		double v5 = m22 * m33 - m23 * m32;
		
		double t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
		double t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
		double t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
		double t30 = - (v3 * m10 - v1 * m11 + v0 * m12);
		
		double invDet = 1.0 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);
		
		double d00 = t00 * invDet;
		double d10 = t10 * invDet;
		double d20 = t20 * invDet;
		double d30 = t30 * invDet;
		
		double d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		double d11 =  (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		double d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		double d31 =  (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		
		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;
		
		double d02 =  (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		double d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		double d22 =  (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		double d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		
		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;
		
		double d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		double d13 =  (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		double d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		double d33 =  (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		
		return new Matrix4(d00, d01, d02, d03,
			d10, d11, d12, d13,
			d20, d21, d22, d23,
			d30, d31, d32, d33);
	}
	/**
	 * Returns a copy of the given matrix with the Y and Z components swapped.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_mat - The matrix to swap the components of.
	 * 
	 * @return The new matrix with swapped Y and Z.
	 */
	public static Matrix4 swapYandZ(Matrix4 in_mat)
	{
		return new Matrix4(in_mat.m_data[0], in_mat.m_data[2], in_mat.m_data[1], in_mat.m_data[3],
			in_mat.m_data[8], in_mat.m_data[10], in_mat.m_data[9], in_mat.m_data[11],
			in_mat.m_data[4], in_mat.m_data[6], in_mat.m_data[5], in_mat.m_data[7],
			in_mat.m_data[12], in_mat.m_data[14], in_mat.m_data[13], in_mat.m_data[15]);
	}
	/**
	 * Constructor. Creates the matrix with the given components. After construction this
	 * is immutable.
	 * 
	 * @param in_a0 - The component at row 1, column 1.
	 * @param in_a1 - The component at row 1, column 2.
	 * @param in_a2 - The component at row 1, column 3.
	 * @param in_a3 - The component at row 1, column 4.
	 * @param in_b0 - The component at row 2, column 1.
	 * @param in_b1 - The component at row 2, column 2.
	 * @param in_b2 - The component at row 2, column 3.
	 * @param in_b3 - The component at row 2, column 4.
	 * @param in_c0 - The component at row 3, column 1.
	 * @param in_c1 - The component at row 3, column 2.
	 * @param in_c2 - The component at row 3, column 3.
	 * @param in_c3 - The component at row 3, column 4.
	 * @param in_d0 - The component at row 4, column 1.
	 * @param in_d1 - The component at row 4, column 2.
	 * @param in_d2 - The component at row 4, column 3.
	 * @param in_d3 - The component at row 4, column 4.
	 */
	public Matrix4(double in_a0, double in_a1, double in_a2, double in_a3,
			double in_b0, double in_b1, double in_b2, double in_b3,
			double in_c0, double in_c1, double in_c2, double in_c3,
			double in_d0, double in_d1, double in_d2, double in_d3)
	{
		m_data = new double[16];
		m_data[0] = in_a0; 	m_data[1] = in_a1; 	m_data[2] = in_a2; 	m_data[3] = in_a3;
		m_data[4] = in_b0; 	m_data[5] = in_b1; 	m_data[6] = in_b2; 	m_data[7] = in_b3;
		m_data[8] = in_c0; 	m_data[9] = in_c1; 	m_data[10] = in_c2; m_data[11] = in_c3;
		m_data[12] = in_d0; m_data[13] = in_d1; m_data[14] = in_d2; m_data[15] = in_d3;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_column - The column. 
	 * @param in_row - The row.
	 * 
	 * @return The value at the given column and row.
	 */
	public double get(int in_column, int in_row)
	{
		return m_data[in_column + in_row * 4];
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_index - The index into the 
	 * 
	 * @return The value at the given index. Values are stored in row-major 
	 * ordering.
	 */
	public double get(int in_index)
	{
		return m_data[in_index];
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Returns the rotation and euler angles.
	 */
	public Vector3 getEulerAngles()
	{
		// The following algorithm has been taken from "Computing Euler angles from a rotation matrix" by Gregory G. Slabaugh. 
		// The original document can be found at: http://www.gregslabaugh.name/publications/euler.pdf
		// Note: While the algorithm can return two sets of possible  answers, we are only returning 1 for simplicities sake.
		
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;
		if (m_data[8] != 1.0 && m_data[8] != -1.0)
		{
			//only take one set of angles.
			y = -Math.asin(m_data[8]); 
			x = Math.atan2(m_data[9] / Math.cos(y), m_data[9] / Math.cos(y));
			z = Math.atan2(m_data[4] / Math.cos(y), m_data[0] / Math.cos(y));
		}
		else
		{
			z = 0.0;
			if (m_data[8] == -1)
			{
				y = Math.PI * 0.5;
				x = z + Math.atan2(m_data[1], m_data[2]);
			}
			else
			{
				y = -Math.PI * 0.5;
				x = -z + Math.atan2(-m_data[1], -m_data[2]);
			}
		}
		
		return new Vector3(x, y ,z);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Whether or not this matrix has a scaling component.
	 */
	public boolean containsScale()
	{
		Matrix4 theCopy = new Matrix4(m_data[0], m_data[1], m_data[2], m_data[3], 
				m_data[4], m_data[5], m_data[6], m_data[6], 
				m_data[8], m_data[9], m_data[10], m_data[11], 
				0.0, 0.0, 0.0, m_data[15]);

		Vector3 rotationX = Vector3.multiply(new Vector3(1.0, 0.0, 0.0), theCopy);
		Vector3 rotationY = Vector3.multiply(new Vector3(0.0, 1.0, 0.0), theCopy);
		Vector3 rotationZ = Vector3.multiply(new Vector3(0.0, 0.0, 1.0), theCopy);
		
		final double TOLERANCE = 0.00001;
		if (rotationY.getLength() < 1.0 - TOLERANCE || rotationY.getLength() > 1.0 + TOLERANCE ||
			rotationX.getLength() < 1.0 - TOLERANCE || rotationX.getLength() > 1.0 + TOLERANCE ||
			rotationZ.getLength() < 1.0 - TOLERANCE || rotationZ.getLength() > 1.0 + TOLERANCE)
		{
			return true;
		}
		
		return false;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The translation part of the transform described by the matrix.
	 */
	public Vector3 getTranslation()
	{
		return new Vector3(m_data[12], m_data[13], m_data[14]);
	}
	/**
	 * Returns the transform decomposed into its translation, scale and orientation.
	 * 
	 * @author Ian Copland
	 * 
	 * @return A 3-tuple containg the transform, scale and orientation of the transform.
	 */
	public Tuple3<Vector3, Vector3, Quaternion> decomposeTransforms()
    {
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
		double invLength = 1.0 / Math.sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2]);
		double qA0 = m_data[0] * invLength;
		double qA1 = m_data[1] * invLength;
		double qA2 = m_data[2] * invLength;
		
		double fDot = qA0 * m_data[4] + qA1 * m_data[5] + qA2 * m_data[6];
		double qB0 = m_data[4] - fDot * qA0;
		double qB1 = m_data[5] - fDot * qA1;
		double qB2 = m_data[6] - fDot * qA2;
		invLength = 1.0 / Math.sqrt(qB0 * qB0 + qB1 * qB1 + qB2 * qB2);
		qB0 *= invLength;
		qB1 *= invLength;
		qB2 *= invLength;
		
		fDot = qA0 * m_data[8] + qA1 * m_data[9] + qA2 * m_data[10];
		double qC0 = m_data[8] - fDot * qA0;
		double qC1 = m_data[9] - fDot * qA1;
		double qC2 = m_data[10] - fDot * qA2;
		fDot = qB0 * m_data[8] + qB1 * m_data[9] + qB2 * m_data[10];
		qC0 -= fDot * qB0;
		qC1 -= fDot * qB1;
		qC2 -= fDot * qB2;
		invLength = 1.0 / Math.sqrt(qC0 * qC0 + qC1 * qC1 + qC2 * qC2);
		qC0 *= invLength;
		qC1 *= invLength;
		qC2 *= invLength;
		
		//guarantee that orthogonal matrix has determinant 1 (no reflections)
		double fDet = qA0 * qB1 * qC2 + qB0 * qC1 * qA2 +
				qC0 * qA1 * qB2 - qC0 * qB1 * qA2 -
				qB0 * qA1 * qC2 - qA0 * qC1 * qB2;
		
		Matrix4 q;
		if (fDet >= 0.0)
		{
			q = new Matrix4(qA0, qA1, qA2, 0.0,
					qB0, qB1, qB2, 0.0,
					qC0, qC1, qC2, 0.0,
					0.0, 0.0, 0.0, 1.0);
		}
		else
		{
			q = new Matrix4(-qA0, -qA1, -qA2, 0.0,
					-qB0, -qB1, -qB2, 0.0,
					-qC0, -qC1, -qC2, 0.0,
					0.0, 0.0, 0.0, -1.0);
		}
		
		//build "right" matrix R
		double rA0 = q.m_data[0] * m_data[0] + q.m_data[1] * m_data[1] + q.m_data[2] * m_data[2];
		double rB0 = q.m_data[0] * m_data[4] + q.m_data[1] * m_data[5] + q.m_data[2] * m_data[6];
		double rB1 = q.m_data[4] * m_data[4] + q.m_data[5] * m_data[5] + q.m_data[6] * m_data[6];
		double rC0 = q.m_data[0] * m_data[8] + q.m_data[1] * m_data[9] + q.m_data[2] * m_data[10];
		double rC1 = q.m_data[4] * m_data[8] + q.m_data[5] * m_data[9] + q.m_data[6] * m_data[10];
		double rC2 = q.m_data[8] * m_data[8] + q.m_data[9] * m_data[9] + q.m_data[10] * m_data[10];
		Matrix4 r = new Matrix4(rA0, 0.0, 0.0, 0.0,
				rB0, rB1, 0.0, 0.0,
				rC0, rC1, rC2, 0.0,
				0.0, 0.0, 0.0, 1.0);
		
		//get the translation
		Vector3 translation = new Vector3(m_data[12], m_data[13], m_data[14]);
		
		//the scaling component
		Vector3 scale = new Vector3(r.m_data[0], r.m_data[5], r.m_data[10]);
	
		//get the orientation
		Quaternion orientation = new Quaternion(q);
		
		return new Tuple3<Vector3, Vector3, Quaternion>(translation, scale, orientation);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return A string representation of the matrix.
	 */
	@Override public String toString()
	{
		return  "" + m_data[0] + ", " + m_data[1] + ", " + m_data[2] + ", " + m_data[3] + ", " + 
			m_data[4] + ", " + m_data[5] + ", " + m_data[6] + ", " + m_data[7] + ", " + 
			m_data[8] + ", " + m_data[9] + ", " + m_data[10] + ", " + m_data[11] + ", " +
			m_data[12] + ", " + m_data[13] + ", " + m_data[14] + ", " + m_data[15];
	}
	/**
	 * @author Ian Copland
	 *
	 * @param in_object - The other object.
	 *
	 * @return Whether or not the given object is equal to this.
	 */
	@Override public boolean equals(Object in_object)
	{
		if (in_object == this)
	    {
	    	return true;
	    }
		
		if (in_object == null)
	    {
	    	return false;
	    }
	    	
	    if ((in_object instanceof Matrix4) == false)
	    {
	    	return false;
	    }
	    
	    Matrix4 matrix4 = (Matrix4)in_object;
	    for (int i = 0; i < 16; ++i)
    	{
	    	if (Double.compare(m_data[i], matrix4.m_data[i]) != 0)
	    	{
	    		return false;
	    	}
    	}
	    
	    return true;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The hash code for this object.
	 */
	@Override public int hashCode()
	{
		int hash = HashCodeUtils.INITIAL_VALUE;
		for (int i = 0; i < 16; ++i)
    	{
			hash = HashCodeUtils.add(hash, m_data[i]);
    	}
		return hash;
	}
}
