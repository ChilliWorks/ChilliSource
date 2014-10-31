/**
 * Integer3.java
 * Chilli Source
 * Created by Ian Copland on 27/10/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
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
public final class Integer3
{
	public static final Integer3 ZERO = new Integer3(0, 0, 0);
	public static final Integer3 ONE = new Integer3(1, 1, 1);
	
	private final int m_x;
	private final int m_y;
	private final int m_z;
	
	/**
	 * Returns the addition of the two provided Integer3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer3.
	 * @param in_b - the second Integer3.
	 * 
	 * @return The result of the addition.
	 */
	public static Integer3 add(Integer3 in_a, Integer3 in_b)
	{
		return new Integer3(in_a.getX() + in_b.getX(), in_a.getY() + in_b.getY(), in_a.getZ() + in_b.getZ());
	}
	/**
	 * Returns the subtraction of the two provided Integer3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer3.
	 * @param in_b - the second Integer3.
	 * 
	 * @return The result of the subtraction.
	 */
	public static Integer3 subtract(Integer3 in_a, Integer3 in_b)
	{
		return new Integer3(in_a.getX() - in_b.getX(), in_a.getY() - in_b.getY(), in_a.getZ() - in_b.getZ());
	}
	/**
	 * Returns the result of multiplying all components of the Integer3 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Integer3.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Integer3 multiply(Integer3 in_a, int in_b)
	{
		return new Integer3(in_a.getX() * in_b, in_a.getY() * in_b, in_a.getZ() * in_b);
	}
	/**
	 * Returns the result of multiplying the two Interger3's together.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer3.
	 * @param in_b - the second Integer3.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Integer3 multiply(Integer3 in_a, Integer3 in_b)
	{
		return new Integer3(in_a.getX() * in_b.getX(), in_a.getY() * in_b.getY(), in_a.getZ() * in_b.getZ());
	}
	/**
	 * Takes the min value for each component from the two Integer3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer3.
	 * @param in_b - the second Integer3.
	 * 
	 * @return The min value.
	 */
	public static Integer3 min(Integer3 in_a, Integer3 in_b)
	{
		return new Integer3(Math.min(in_a.getX(), in_b.getX()), Math.min(in_a.getY(), in_b.getY()), Math.min(in_a.getZ(), in_b.getZ()));
	}
	/**
	 * Takes the max value for each component from the two Integer3s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer3.
	 * @param in_b - the second Integer3.
	 * 
	 * @return The max value.
	 */
	public static Integer3 max(Integer3 in_a, Integer3 in_b)
	{
		return new Integer3(Math.max(in_a.getX(), in_b.getX()), Math.max(in_a.getY(), in_b.getY()), Math.max(in_a.getZ(), in_b.getZ()));
	}
	/**
	 * Parses an Integer3 from a string. Integer3 values should be provided 
	 * as integers and each component should be separated by a comma. For 
	 * example:  2, 600, -7.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_int3String - The Integer3 string.
	 */
	public static Integer3 parseInt3(String in_int3String)
	{
		String[] components = in_int3String.split(",");
		if (components.length == 3)
		{
			int x = Integer.parseInt(components[0].trim());
			int y = Integer.parseInt(components[1].trim());
			int z = Integer.parseInt(components[2].trim());
			
			return new Integer3(x, y, z);
		}
		else
		{
			return Integer3.ZERO;
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
	public Integer3(int in_x, int in_y, int in_z)
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
	    	
	    if ((in_object instanceof Integer3) == false)
	    {
	    	return false;
	    }
	    
	    Integer3 integer3 = (Integer3)in_object;
	    return (m_x == integer3.m_x && m_y == integer3.m_y && m_z == integer3.m_z);
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
