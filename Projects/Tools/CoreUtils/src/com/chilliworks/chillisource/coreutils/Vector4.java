/**
 * Vector4.java
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
public final class Vector4
{
	public static final Vector4 ZERO = new Vector4(0.0, 0.0, 0.0, 0.0);
	public static final Vector4 ONE = new Vector4(1.0, 1.0, 1.0, 1.0);
	
	private final double m_x;
	private final double m_y;
	private final double m_z;
	private final double m_w;
	
	/**
	 * Returns the addition of the two provided Vector4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector4.
	 * @param in_b - the second Vector4.
	 * 
	 * @return The result of the addition.
	 */
	public static Vector4 add(Vector4 in_a, Vector4 in_b)
	{
		return new Vector4(in_a.getX() + in_b.getX(), in_a.getY() + in_b.getY(), in_a.getZ() + in_b.getZ(), in_a.getW() + in_b.getW());
	}
	/**
	 * Returns the subtraction of the two provided Vector4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector4.
	 * @param in_b - the second Vector4.
	 * 
	 * @return The result of the subtraction.
	 */
	public static Vector4 subtract(Vector4 in_a, Vector4 in_b)
	{
		return new Vector4(in_a.getX() - in_b.getX(), in_a.getY() - in_b.getY(), in_a.getZ() - in_b.getZ(), in_a.getW() - in_b.getW());
	}
	/**
	 * Returns the result of multiplying all components of the Vector4 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Vector4.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector4 multiply(Vector4 in_a, double in_b)
	{
		return new Vector4(in_a.getX() * in_b, in_a.getY() * in_b, in_a.getZ() * in_b, in_a.getW() * in_b);
	}
	/**
	 * Returns the result of multiplying two Vector4s together.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector4.
	 * @param in_b - the second Vector4.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector4 multiply(Vector4 in_a, Vector4 in_b)
	{
		return new Vector4(in_a.getX() * in_b.getX(), in_a.getY() * in_b.getY(), in_a.getZ() * in_b.getZ(), in_a.getW() * in_b.getW());
	}
	/**
	 * Returns the result of dividing all components of the Vector4 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Vector4.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector4 divide(Vector4 in_a, double in_b)
	{
		double invB = in_b;
		return new Vector4(in_a.getX() * invB, in_a.getY() * invB, in_a.getZ() * invB, in_a.getW() * invB);
	}
	/**
	 * Returns the result of dividing two Vector4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector4.
	 * @param in_b - the second Vector4.
	 * 
	 * @return The result of the division.
	 */
	public static Vector4 divide(Vector4 in_a, Vector4 in_b)
	{
		return new Vector4(in_a.getX() / in_b.getX(), in_a.getY() / in_b.getY(), in_a.getZ() / in_b.getZ(), in_a.getW() / in_b.getW());
	}
	/**
	 * Takes the min value for each component from the two Vector4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector4.
	 * @param in_b - the second Vector4.
	 * 
	 * @return The min value.
	 */
	public static Vector4 min(Vector4 in_a, Vector4 in_b)
	{
		return new Vector4(Math.min(in_a.getX(), in_b.getX()), Math.min(in_a.getY(), in_b.getY()), Math.min(in_a.getZ(), in_b.getZ()), Math.min(in_a.getW(), in_b.getW()));
	}
	/**
	 * Takes the max value for each component from the two Vector4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector4.
	 * @param in_b - the second Vector4.
	 * 
	 * @return The max value.
	 */
	public static Vector4 max(Vector4 in_a, Vector4 in_b)
	{
		return new Vector4(Math.max(in_a.getX(), in_b.getX()), Math.max(in_a.getY(), in_b.getY()), Math.max(in_a.getZ(), in_b.getZ()), Math.max(in_a.getW(), in_b.getW()));
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
	public static Vector4 normalise(Vector4 in_vec)
	{
		double invLength = 1.0 / in_vec.getLength();
		return new Vector4(in_vec.m_x * invLength, in_vec.m_y * invLength, in_vec.m_z * invLength, in_vec.m_w * invLength);
	}
	/**
	 * Parses an Vector4 from a string. Vector4 values should be provided 
	 * as real numbers and each component should be separated by a comma. For 
	 * example:  2.0, 600.0, -7.0, -23.0.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_vector4String - The Vector4 string.
	 */
	public static Vector4 parseVec4(String in_vector4String)
	{
		String[] components = in_vector4String.split(",");
		if (components.length == 4)
		{
			double x = Double.parseDouble(components[0].trim());
			double y = Double.parseDouble(components[1].trim());
			double z = Double.parseDouble(components[2].trim());
			double w = Double.parseDouble(components[3].trim());
			
			return new Vector4(x, y, z, w);
		}
		else
		{
			return Vector4.ZERO;
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
	 * @param in_w - The w value.
	 */
	public Vector4(double in_x, double in_y, double in_z, double in_w)
	{
		m_x = in_x;
		m_y = in_y;
		m_z = in_z;
		m_w = in_w;
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
	 * @return The w component.
	 */
	public double getW()
	{
		return m_w;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The length of the vector.
	 */
	public double getLength()
	{
		return Math.sqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
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
	    	
	    if ((in_object instanceof Vector4) == false)
	    {
	    	return false;
	    }
	    
	    Vector4 vector4 = (Vector4)in_object;
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
