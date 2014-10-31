/**
 * Integer4.java
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
 * An integer vector class. This is immutable after creation meaning it can
 * safely be passed into methods without side-effects.
 * 
 * @author Ian Copland
 */
public final class Integer4
{
	public static final Integer4 ZERO = new Integer4(0, 0, 0, 0);
	public static final Integer4 ONE = new Integer4(1, 1, 1, 1);
	
	private final int m_x;
	private final int m_y;
	private final int m_z;
	private final int m_w;
	
	/**
	 * Returns the addition of the two provided Integer4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer4.
	 * @param in_b - the second Integer4.
	 * 
	 * @return The result of the addition.
	 */
	public static Integer4 add(Integer4 in_a, Integer4 in_b)
	{
		return new Integer4(in_a.getX() + in_b.getX(), in_a.getY() + in_b.getY(), in_a.getZ() + in_b.getZ(), in_a.getW() + in_b.getW());
	}
	/**
	 * Returns the subtraction of the two provided Integer4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer4.
	 * @param in_b - the second Integer4.
	 * 
	 * @return The result of the subtraction.
	 */
	public static Integer4 subtract(Integer4 in_a, Integer4 in_b)
	{
		return new Integer4(in_a.getX() - in_b.getX(), in_a.getY() - in_b.getY(), in_a.getZ() - in_b.getZ(), in_a.getW() - in_b.getW());
	}
	/**
	 * Returns the result of multiplying all components of the Integer4 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Integer4.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Integer4 multiply(Integer4 in_a, int in_b)
	{
		return new Integer4(in_a.getX() * in_b, in_a.getY() * in_b, in_a.getZ() * in_b, in_a.getW() * in_b);
	}
	/**
	 * Returns the result of multiplying the two Interger4's together.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer4.
	 * @param in_b - the second Integer4.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Integer4 multiply(Integer4 in_a, Integer4 in_b)
	{
		return new Integer4(in_a.getX() * in_b.getX(), in_a.getY() * in_b.getY(), in_a.getZ() * in_b.getZ(), in_a.getW() * in_b.getW());
	}
	/**
	 * Takes the min value for each component from the two Integer4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer4.
	 * @param in_b - the second Integer4.
	 * 
	 * @return The min value.
	 */
	public static Integer4 min(Integer4 in_a, Integer4 in_b)
	{
		return new Integer4(Math.min(in_a.getX(), in_b.getX()), Math.min(in_a.getY(), in_b.getY()), Math.min(in_a.getZ(), in_b.getZ()), Math.min(in_a.getW(), in_b.getW()));
	}
	/**
	 * Takes the max value for each component from the two Integer4s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer4.
	 * @param in_b - the second Integer4.
	 * 
	 * @return The max value.
	 */
	public static Integer4 max(Integer4 in_a, Integer4 in_b)
	{
		return new Integer4(Math.max(in_a.getX(), in_b.getX()), Math.max(in_a.getY(), in_b.getY()), Math.max(in_a.getZ(), in_b.getZ()), Math.max(in_a.getW(), in_b.getW()));
	}
	/**
	 * Parses an Integer4 from a string. Integer4 values should be provided 
	 * as integers and each component should be separated by a comma. For 
	 * example:  2, 600, -7, -23.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_int4String - The Integer4 string.
	 */
	public static Integer4 parseInt4(String in_int4String)
	{
		String[] components = in_int4String.split(",");
		if (components.length == 4)
		{
			int x = Integer.parseInt(components[0].trim());
			int y = Integer.parseInt(components[1].trim());
			int z = Integer.parseInt(components[2].trim());
			int w = Integer.parseInt(components[3].trim());
			
			return new Integer4(x, y, z, w);
		}
		else
		{
			return Integer4.ZERO;
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
	public Integer4(int in_x, int in_y, int in_z, int in_w)
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
	public int getX()
	{
		return m_x;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The y component.
	 */
	public int getY()
	{
		return m_y;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The z component.
	 */
	public int getZ()
	{
		return m_z;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The w component.
	 */
	public int getW()
	{
		return m_w;
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
	    	
	    if ((in_object instanceof Integer4) == false)
	    {
	    	return false;
	    }
	    
	    Integer4 integer4 = (Integer4)in_object;
	    return (m_x == integer4.m_x && m_y == integer4.m_y && m_z == integer4.m_z && m_w == integer4.m_w);
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
