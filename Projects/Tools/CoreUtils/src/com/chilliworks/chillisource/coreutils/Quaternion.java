/**
 * Quaternion.java
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
 * A basic Quaternion math class. This is immutable after construction so it can safely
 * be passed into methods.
 * 
 * @author Ian Copland
 */
public final class Quaternion 
{
	public static final Quaternion IDENTITY = new Quaternion(0.0, 0.0, 0.0, 1.0);
	
	private double m_x;
	private double m_y;
	private double m_z;
	private double m_w;

	/**
	 * Returns a normalised copy of the given quaternion.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The Quaternion to copy.
	 * 
	 * @return The normalised copy.
	 */
	public static Quaternion normalise(Quaternion in_quaternion)
	{
		double s = 1.0 / Math.sqrt(in_quaternion.m_w * in_quaternion.m_w + in_quaternion.m_x * in_quaternion.m_x + in_quaternion.m_y * in_quaternion.m_y + in_quaternion.m_z * in_quaternion.m_z);
		return new Quaternion(in_quaternion.m_x * s, in_quaternion.m_y * s, in_quaternion.m_z * s, in_quaternion.m_w * s);
	}
	/**
	 * Constructor. Creates a new quaternion with the given components. After 
	 * construction the quaternion is immutable.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_x - The x component.
	 * @param in_y - The y component.
	 * @param in_z - The z component.
	 * @param in_w - The w component.
	 */
	public Quaternion(double in_x, double in_y, double in_z, double in_w)
	{
		m_x = in_x;
		m_y = in_y;
		m_z = in_z;
		m_w = in_w;
	}
	/**
	 * Constructor. Creates a quaternion which describes the same rotation as
	 * the given rotation. Matrix. After construction the quaternion is 
	 * immutable.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_matrix - The rotation matrix.
	 */
	public Quaternion(Matrix4 in_matrix)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".
		
		double trace = in_matrix.get(0) + in_matrix.get(5) + in_matrix.get(10) + 1.0;
		double root = 0.0;
		
		if (trace > 0.0)
		{
			root = Math.sqrt(trace);
			m_w = 0.5 * root;
			root = 0.5 / root; 
			m_x = (in_matrix.get(6) - in_matrix.get(9)) * root;
			m_y = (in_matrix.get(8) - in_matrix.get(2)) * root;
			m_z = (in_matrix.get(1) - in_matrix.get(4)) * root;
		}
		else
		{
			int[] iNext = { 1, 2, 0 };
			int i = 0;
			if (in_matrix.get(5) > in_matrix.get(0))
			{
				i = 1;
			}
			
			if (in_matrix.get(10) > in_matrix.get(i,i))
			{
				i = 2;
			}
			
			int j = iNext[i];
			int k = iNext[j];
			
			root = Math.sqrt(in_matrix.get(i,i) - in_matrix.get(j,j) - in_matrix.get(k,k) + 1.0);
			
			double[] apkQuat = {0.0, 0.0, 0.0};
			apkQuat[i] = 0.5 * root;
			root = 0.5f/root;
			
			m_w = (in_matrix.get(k, j) - in_matrix.get(j, k)) * root;
			
			apkQuat[j] = (in_matrix.get(j, i) + in_matrix.get(i, j)) * root;
			apkQuat[k] = (in_matrix.get(k, i) + in_matrix.get(i, k)) * root;
			
			m_x = apkQuat[0];
			m_y = apkQuat[1];
			m_z = apkQuat[2];
		}
	}
	/**
	 * Constructor. Creates a quaternion which describes the same rotation as the
	 * given axis and angle. After construction the quaternion is immutable.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_axis - The axis.
	 * @param in_angle - The angle.
	 */
	public Quaternion(Vector3 in_axis, double in_angle)
	{
		Vector3 norm = Vector3.normalise(in_axis);
		double halfAngle = in_angle / 2.0;
		double sinAngle = Math.sin(halfAngle);
		
		m_x = norm.getX() * sinAngle;
		m_y = norm.getY() * sinAngle;
		m_z = norm.getZ() * sinAngle;
		m_w = Math.cos(halfAngle);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The x component of the quaternion.
	 */
	public double getX()
	{
		return m_x;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The y component of the quaternion.
	 */
	public double getY()
	{
		return m_y;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The z component of the quaternion.
	 */
	public double getZ()
	{
		return m_z;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The w component of the quaternion.
	 */
	public double getW()
	{
		return m_w;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The axis of rotation described by the Quaternion.
	 */
	public Vector3 getAxis()
	{
		double sqrLength = (m_x * m_x + m_y * m_y + m_z * m_z);

		if (sqrLength > 0.0)
		{
			double inverseLength = 1.0 / Math.sqrt(sqrLength);
			return new Vector3(m_x * inverseLength, m_y * inverseLength, m_z * inverseLength);
		}

		return new Vector3(1.0, 0.0, 0.0); 
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The angle of rotation described by the Quaternion.
	 */
	public double getAngle()
	{
		double sqrLength = (m_x * m_x + m_y * m_y + m_z * m_z);

		if (sqrLength > 0.0)
		{
			return 2.0 * Math.acos(m_w);
		} 

		return 0.0;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return A matrix which describes the same rotation as this quaternion.
	 */
	public Matrix4 toMatrix()
	{
		double x2 = m_x * m_x;
		double y2 = m_y * m_y;
		double z2 = m_z * m_z;
		double xy = m_x * m_y;
		double xz = m_x * m_z;
		double yz = m_y * m_z;
		double wx = m_w * m_x;
		double wy = m_w * m_y;
		double wz = m_w * m_z;
		
		return new Matrix4(1.0 - 2.0 * (y2 + z2), 2.0 * (xy + wz), 2.0 * (xz - wy), 0.0,
				2.0 * (xy - wz), 1.0 - 2.0 * (x2 + z2), 2.0 * (yz + wx), 0.0,
				2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (x2 + y2), 0.0,
				0.0, 0.0, 0.0, 1.0);
	}
	/**
	 * @author Ian Copland
	 *
	 * @return This object in string form.
	 */
	@Override public String toString()
	{
		return m_x + ", " + m_y + ", " + m_z + ", " + m_w;
	}
	/**
	 * Returns whether or not this object is equal in value to the given
	 * object.
	 * 
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
	    	
	    if ((in_object instanceof Quaternion) == false)
	    {
	    	return false;
	    }
	    
	    Quaternion vector4 = (Quaternion)in_object;
	    return (Double.compare(m_x, vector4.m_x) == 0 && Double.compare(m_y, vector4.m_y) == 0 && Double.compare(m_z, vector4.m_z) == 0 && Double.compare(m_w, vector4.m_w) == 0);
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The hash code for this object.
	 */
	@Override public int hashCode()
	{
		int hash = HashCodeUtils.INITIAL_VALUE;
		hash = HashCodeUtils.add(hash, m_x);
		hash = HashCodeUtils.add(hash, m_y);
		hash = HashCodeUtils.add(hash, m_z);
		hash = HashCodeUtils.add(hash, m_w);
		return hash;
	}
}
