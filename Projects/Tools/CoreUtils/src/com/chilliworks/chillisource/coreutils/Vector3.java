/**
 * Vector3.java
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
 * A vector math class. This is immutable after creation meaning it can
 * safely be passed into methods without side-effects.
 * 
 * @author Ian Copland
 */
public final class Vector3
{
	public static final Vector3 ZERO = new Vector3(0.0, 0.0, 0.0);
	public static final Vector3 ONE = new Vector3(1.0, 1.0, 1.0);
	
	private final double m_x;
	private final double m_y;
	private final double m_z;
	
	/**
	 * Returns the addition of the two provided Vector3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector3.
	 * @param in_b - the second Vector3.
	 * 
	 * @return The result of the addition.
	 */
	public static Vector3 add(Vector3 in_a, Vector3 in_b)
	{
		return new Vector3(in_a.getX() + in_b.getX(), in_a.getY() + in_b.getY(), in_a.getZ() + in_b.getZ());
	}
	/**
	 * Returns the subtraction of the two provided Vector3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector3.
	 * @param in_b - the second Vector3.
	 * 
	 * @return The result of the subtraction.
	 */
	public static Vector3 subtract(Vector3 in_a, Vector3 in_b)
	{
		return new Vector3(in_a.getX() - in_b.getX(), in_a.getY() - in_b.getY(), in_a.getZ() - in_b.getZ());
	}
	/**
	 * Returns the result of multiplying all components of the Vector3 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Vector3.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector3 multiply(Vector3 in_a, double in_b)
	{
		return new Vector3(in_a.getX() * in_b, in_a.getY() * in_b, in_a.getZ() * in_b);
	}	
	/**
	 * Returns the result of multiplying the Vector3 by a 4x4 matrix.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Vector3.
	 * @param in_b - the Matrix4.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector3 multiply(Vector3 inVector, Matrix4 inMatrix)
	{
		double x = inVector.getX() * inMatrix.get(0) + inVector.getY() * inMatrix.get(4) + inVector.getZ() * inMatrix.get(8) + inMatrix.get(12);
		double y = inVector.getX() * inMatrix.get(1) + inVector.getY() * inMatrix.get(5) + inVector.getZ() * inMatrix.get(9) + inMatrix.get(13);
		double z = inVector.getX() * inMatrix.get(2) + inVector.getY() * inMatrix.get(6) + inVector.getZ() * inMatrix.get(10) + inMatrix.get(14);
		return new Vector3(x, y, z);
	}
	/**
	 * Returns the result of multiplying two Vector3s together.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector3.
	 * @param in_b - the second Vector3.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector3 multiply(Vector3 in_a, Vector3 in_b)
	{
		return new Vector3(in_a.getX() * in_b.getX(), in_a.getY() * in_b.getY(), in_a.getZ() * in_b.getZ());
	}
	/**
	 * Returns the result of dividing all components of the Vector3 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Vector3.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector3 divide(Vector3 in_a, double in_b)
	{
		double invB = 1.0 / in_b;
		return new Vector3(in_a.getX() * invB, in_a.getY() * invB, in_a.getZ() * invB);
	}
	/**
	 * Returns the result of dividing two Vector3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector3.
	 * @param in_b - the second Vector3.
	 * 
	 * @return The result of the division.
	 */
	public static Vector3 divide(Vector3 in_a, Vector3 in_b)
	{
		return new Vector3(in_a.getX() / in_b.getX(), in_a.getY() / in_b.getY(), in_a.getZ() / in_b.getZ());
	}
	/**
	 * Takes the min value for each component from the two Vector3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector3.
	 * @param in_b - the second Vector3.
	 * 
	 * @return The min value.
	 */
	public static Vector3 min(Vector3 in_a, Vector3 in_b)
	{
		return new Vector3(Math.min(in_a.getX(), in_b.getX()), Math.min(in_a.getY(), in_b.getY()), Math.min(in_a.getZ(), in_b.getZ()));
	}
	/**
	 * Takes the max value for each component from the two Vector3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector3.
	 * @param in_b - the second Vector3.
	 * 
	 * @return The max value.
	 */
	public static Vector3 max(Vector3 in_a, Vector3 in_b)
	{
		return new Vector3(Math.max(in_a.getX(), in_b.getX()), Math.max(in_a.getY(), in_b.getY()), Math.max(in_a.getZ(), in_b.getZ()));
	}
	/**
	 * returns a normalised copy of the given vector.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_vec - the vector to normalise.
	 * 
	 * @return The normalised vector.
	 */
	public static Vector3 normalise(Vector3 in_vec)
	{
		double invLength = 1.0 / in_vec.getLength();
		return new Vector3(in_vec.m_x * invLength, in_vec.m_y * invLength, in_vec.m_z * invLength);
	}
	/**
	 * Parses an Vector3 from a string. Vector3 values should be provided 
	 * as real numbers and each component should be separated by a comma. For 
	 * example:  2.0, 600.0, -7.0.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_vector3String - The Vector3 string.
	 */
	public static Vector3 parseVec3(String in_vector3String)
	{
		String[] components = in_vector3String.split(",");
		if (components.length == 3)
		{
			double x = Double.parseDouble(components[0].trim());
			double y = Double.parseDouble(components[1].trim());
			double z = Double.parseDouble(components[2].trim());
			
			return new Vector3(x, y, z);
		}
		else
		{
			return Vector3.ZERO;
		}
	}
	/**
	 * Constructor.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_x - The x value.
	 * @param in_y - The y value.
	 * @param in_z - The z value.
	 */
	public Vector3(double in_x, double in_y, double in_z)
	{
		m_x = in_x;
		m_y = in_y;
		m_z = in_z;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The x component.
	 */
	public double getX()
	{
		return m_x;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The y component.
	 */
	public double getY()
	{
		return m_y;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The z component.
	 */
	public double getZ()
	{
		return m_z;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The length of the vector.
	 */
	public double getLength()
	{
		return Math.sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
	}
	/**
	 * @author Ian Copland
	 *
	 * @return This object in string form.
	 */
	@Override public String toString()
	{
		return m_x + ", " + m_y + ", " + m_z;
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
	    	
	    if ((in_object instanceof Vector3) == false)
	    {
	    	return false;
	    }
	    
	    Vector3 vector3 = (Vector3)in_object;
	    return (Double.compare(m_x, vector3.m_x) == 0 && Double.compare(m_y, vector3.m_y) == 0 && Double.compare(m_z, vector3.m_z) == 0);
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
		return hash;
	}
}
