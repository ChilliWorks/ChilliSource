/**
 * Colour.java
 * Chilli Source
 * Created by Ian Copland on 09/10/2014
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

package com.chilliworks.chillisource.toolutils;

/**
 * A class for containing RGBA colour values in the range 0.0-1.0. After construction the contents of the
 * class are immutable making it safe to pass into methods.
 * 
 * @author Ian Copland
 */
public final class Colour
{
	public static Colour k_white = new Colour(1.0f, 1.0f, 1.0f, 1.0f);
	public static Colour k_black = new Colour(0.0f, 0.0f, 0.0f, 1.0f);
	
	private final float m_r;
	private final float m_g;
	private final float m_b;
	private final float m_a;
	
	/**
	 * Constructs a new colour with the given RGBA values.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_r - The red colour value.
	 * @param in_g - The green colour value.
	 */
	public Colour(float in_r, float in_g, float in_b, float in_a)
	{
		assert (in_r >= 0.0f && in_r <= 1.0f) : "Colour: red value must be in the range 0.0-1.0.";
		assert (in_g >= 0.0f && in_g <= 1.0f) : "Colour: green value must be in the range 0.0-1.0.";
		assert (in_b >= 0.0f && in_b <= 1.0f) : "Colour: blue value must be in the range 0.0-1.0.";
		assert (in_a >= 0.0f && in_a <= 1.0f) : "Colour: alpha value must be in the range 0.0-1.0.";
		
		m_r = in_r;
		m_g = in_g;
		m_b = in_b;
		m_a = in_a;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The red colour value.
	 */
	public float getR()
	{
		return m_r;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The green colour value.
	 */
	public float getG()
	{
		return m_g;
	}	
	/**
	 * @author Ian Copland
	 *
	 * @return The blue colour value.
	 */
	public float getB()
	{
		return m_b;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The alpha value.
	 */
	public float getA()
	{
		return m_a;
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
	public boolean equals(Object in_object)
	{
		if (in_object == this)
	    {
	    	return true;
	    }
		
		if (in_object == null)
	    {
	    	return false;
	    }
	    	
	    if ((in_object instanceof Colour) == false)
	    {
	    	return false;
	    }
	    
	    Colour colour = (Colour)in_object;
	    return (Float.compare(m_r, colour.m_r) == 0 && Float.compare(m_g, colour.m_g) == 0 && Float.compare(m_b, colour.m_b) == 0 && Float.compare(m_a, colour.m_a) == 0);
	}
	/**

	 * @author Ian Copland
	 *
	 * @return This object in string form.
	 */
	public String toString()
	{
		return "(" + m_r + ", " + m_g + ", " + m_b + "," + m_a + ")";
	}
}
