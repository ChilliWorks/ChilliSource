/**
 * Integer2.java
 * Chilli Source
 * Created by Ian Copland on 09/10/2014.
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
public final class Integer2
{
	public static final Integer2 ZERO = new Integer2(0, 0);
	public static final Integer2 ONE = new Integer2(1, 1);
	
	private final int m_x;
	private final int m_y;
	
	/**
	 * Returns the addition of the two provided Integer2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer2.
	 * @param in_b - the second Integer2.
	 * 
	 * @return The result of the addition.
	 */
	public static Integer2 add(Integer2 in_a, Integer2 in_b)
	{
		return new Integer2(in_a.getX() + in_b.getX(), in_a.getY() + in_b.getY());
	}
	/**
	 * Returns the subtraction of the two provided Integer2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer2.
	 * @param in_b - the second Integer2.
	 * 
	 * @return The result of the subtraction.
	 */
	public static Integer2 subtract(Integer2 in_a, Integer2 in_b)
	{
		return new Integer2(in_a.getX() - in_b.getX(), in_a.getY() - in_b.getY());
	}
	/**
	 * Returns the result of multiplying all components of the Integer2 by
	 * the scalar.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the Integer2.
	 * @param in_b - the scalar.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Integer2 multiply(Integer2 in_a, int in_b)
	{
		return new Integer2(in_a.getX() * in_b, in_a.getY() * in_b);
	}
	/**
	 * Returns the result of multiplying the two Interger2's together.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer2.
	 * @param in_b - the second Integer2.
	 * 
	 * @return The result of the multiplication.
	 */
	public static Integer2 multiply(Integer2 in_a, Integer2 in_b)
	{
		return new Integer2(in_a.getX() * in_b.getX(), in_a.getY() * in_b.getY());
	}
	/**
	 * Takes the min value for each component from the two Integer2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer2.
	 * @param in_b - the second Integer2.
	 * 
	 * @return The min value.
	 */
	public static Integer2 min(Integer2 in_a, Integer2 in_b)
	{
		return new Integer2(Math.min(in_a.getX(), in_b.getX()), Math.min(in_a.getY(), in_b.getY()));
	}
	/**
	 * Takes the max value for each component from the two Integer2s.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_a - the first Integer2.
	 * @param in_b - the second Integer2.
	 * 
	 * @return The max value.
	 */
	public static Integer2 max(Integer2 in_a, Integer2 in_b)
	{
		return new Integer2(Math.max(in_a.getX(), in_b.getX()), Math.max(in_a.getY(), in_b.getY()));
	}
	/**
	 * Parses an Integer2 from a string. Integer2 values should be provided 
	 * as integers and each component should be separated by a comma. For 
	 * example:  2, 600.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_int2String - The Integer2 string.
	 */
	public static Integer2 parseInt2(String in_int2String)
	{
		String[] components = in_int2String.split(",");
		if (components.length == 2)
		{
			int x = Integer.parseInt(components[0].trim());
			int y = Integer.parseInt(components[1].trim());
			
			return new Integer2(x, y);
		}
		else
		{
			return Integer2.ZERO;
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
	public Integer2(int in_x, int in_y)
	{
		m_x = in_x;
		m_y = in_y;
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
	    	
	    if ((in_object instanceof Integer2) == false)
	    {
	    	return false;
	    }
	    
	    Integer2 integer2 = (Integer2)in_object;
	    return (m_x == integer2.m_x && m_y == integer2.m_y);
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
