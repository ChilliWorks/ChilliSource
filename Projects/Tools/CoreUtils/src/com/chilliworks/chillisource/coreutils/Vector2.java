/**
 * Vector2.java
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
public final class Vector2
{
	public static final Vector2 ZERO = new Vector2(0.0, 0.0);
	public static final Vector2 ONE = new Vector2(1.0, 1.0);
	
	private final double m_x;
	private final double m_y;
	
	/**
	 * Returns the addition of the two provided Vector2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector2.
	 * @param in_b - the second Vector2.
	 * 
	 * @return The result of the addition.
	 */
	public static Vector2 add(Vector2 in_a, Vector2 in_b)
	{
		return new Vector2(in_a.getX() + in_b.getX(), in_a.getY() + in_b.getY());
	}
	/**
	 * Returns the subtraction of the two provided Vector2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector2.
	 * @param in_b - the second Vector2.
	 * 
	 * @return The result of the subtraction.
	 */
	public static Vector2 subtract(Vector2 in_a, Vector2 in_b)
	{
		return new Vector2(in_a.getX() - in_b.getX(), in_a.getY() - in_b.getY());
	}
	/**
	 * Returns the result of multiplying all components of the Vector2 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Vector2.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector2 multiply(Vector2 in_a, double in_b)
	{
		return new Vector2(in_a.getX() * in_b, in_a.getY() * in_b);
	}
	/**
	 * Returns the result of multiplying two Vector2s together.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector2.
	 * @param in_b - the second Vector2.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector2 multiply(Vector2 in_a, Vector2 in_b)
	{
		return new Vector2(in_a.getX() * in_b.getX(), in_a.getY() * in_b.getY());
	}
	/**
	 * Returns the result of dividing all components of the Vector2 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Vector2.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Vector2 divide(Vector2 in_a, double in_b)
	{
		double invB = 1.0 / in_b;
		return new Vector2(in_a.getX() * invB, in_a.getY() * in_b);
	}
	/**
	 * Returns the result of dividing two Vector2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector2.
	 * @param in_b - the second Vector2.
	 * 
	 * @return The result of the division.
	 */
	public static Vector2 divide(Vector2 in_a, Vector2 in_b)
	{
		return new Vector2(in_a.getX() / in_b.getX(), in_a.getY() / in_b.getY());
	}
	/**
	 * Takes the min value for each component from the two Vector2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector2.
	 * @param in_b - the second Vector2.
	 * 
	 * @return The min value.
	 */
	public static Vector2 min(Vector2 in_a, Vector2 in_b)
	{
		return new Vector2(Math.min(in_a.getX(), in_b.getX()), Math.min(in_a.getY(), in_b.getY()));
	}
	/**
	 * Takes the max value for each component from the two Vector2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Vector2.
	 * @param in_b - the second Vector2.
	 * 
	 * @return The max value.
	 */
	public static Vector2 max(Vector2 in_a, Vector2 in_b)
	{
		return new Vector2(Math.max(in_a.getX(), in_b.getX()), Math.max(in_a.getY(), in_b.getY()));
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
	public static Vector2 normalise(Vector2 in_vec)
	{
		double invLength = 1.0 / in_vec.getLength();
		return new Vector2(in_vec.m_x * invLength, in_vec.m_y * invLength);
	}
	/**
	 * Parses an Vector2 from a string. Vector2 values should be provided 
	 * as real numbers and each component should be separated by a comma. For 
	 * example:  2.0, 600.0.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_vector2String - The Vector2 string.
	 */
	public static Vector2 parseVec3(String in_vector2String)
	{
		String[] components = in_vector2String.split(",");
		if (components.length == 2)
		{
			double x = Double.parseDouble(components[0].trim());
			double y = Double.parseDouble(components[1].trim());
			
			return new Vector2(x, y);
		}
		else
		{
			return Vector2.ZERO;
		}
	}
	/**
	 * Constructor.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_x - The x value.
	 * @param in_y - The y value.
	 */
	public Vector2(double in_x, double in_y)
	{
		m_x = in_x;
		m_y = in_y;
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
	 * @return The length of the vector.
	 */
	public double getLength()
	{
		return Math.sqrt(m_x * m_x + m_y * m_y);
	}
	/**
	 * @author Ian Copland
	 *
	 * @return This object in string form.
	 */
	@Override public String toString()
	{
		return m_x + ", " + m_y;
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
	    	
	    if ((in_object instanceof Vector2) == false)
	    {
	    	return false;
	    }
	    
	    Vector2 vector2 = (Vector2)in_object;
	    return (Double.compare(m_x, vector2.m_x) == 0 && Double.compare(m_y, vector2.m_y) == 0);
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
		return hash;
	}
}
