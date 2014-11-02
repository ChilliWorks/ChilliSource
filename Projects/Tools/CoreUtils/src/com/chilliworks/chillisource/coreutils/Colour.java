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

package com.chilliworks.chillisource.coreutils;

/**
 * A class for containing RGBA colour values in the range 0.0-1.0. After construction the contents of the
 * class are immutable making it safe to pass into methods.
 * 
 * @author Ian Copland
 */
public final class Colour
{
	public final static Colour WHITE = new Colour(1.0, 1.0, 1.0, 1.0);
	public final static Colour BLACK = new Colour(0.0, 0.0, 0.0, 1.0);
	
	private final double m_r;
	private final double m_g;
	private final double m_b;
	private final double m_a;
	
	/**
	 * Parses a colour from a string. colour values should be given in
	 * the range 0.0 - 1.0 and each component should be separated by
	 * a comma. For example: 0.8, 1.0, 0.1, 1.0.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_colourString - The colour string
	 */
	public static Colour parseColour(String in_colourString)
	{
		String[] components = in_colourString.split(",");
		if (components.length == 3 || components.length == 4)
		{
			double r = Float.parseFloat(components[0].trim());
			double g = Float.parseFloat(components[1].trim());
			double b = Float.parseFloat(components[2].trim());
			double a = 1.0;
			if (components.length == 4)
			{
				a = Float.parseFloat(components[3].trim());
			}
			
			r = Math.max(Math.min(r, 1.0), 0.0);
			g = Math.max(Math.min(g, 1.0), 0.0);
			b = Math.max(Math.min(b, 1.0), 0.0);
			a = Math.max(Math.min(a, 1.0), 0.0);
			
			return new Colour(r, g, b, a);
		}
		else
		{
			return Colour.WHITE;
		}
	}
	/**
	 * Constructs a new colour with the given RGBA values.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_r - The red colour value.
	 * @param in_g - The green colour value.
	 * @param in_b - The blue colour value.
	 * @param in_a - The alpha value.
	 */
	public Colour(double in_r, double in_g, double in_b, double in_a)
	{
		assert (in_r >= 0.0 && in_r <= 1.0) : "Colour: red value must be in the range 0.0-1.0.";
		assert (in_g >= 0.0 && in_g <= 1.0) : "Colour: green value must be in the range 0.0-1.0.";
		assert (in_b >= 0.0 && in_b <= 1.0) : "Colour: blue value must be in the range 0.0-1.0.";
		assert (in_a >= 0.0 && in_a <= 1.0) : "Colour: alpha value must be in the range 0.0-1.0.";
		
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
	public double getR()
	{
		return m_r;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The green colour value.
	 */
	public double getG()
	{
		return m_g;
	}	
	/**
	 * @author Ian Copland
	 *
	 * @return The blue colour value.
	 */
	public double getB()
	{
		return m_b;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The alpha value.
	 */
	public double getA()
	{
		return m_a;
	}
	/**
	 * @author Ian Copland
	 *
	 * @return This object in string form.
	 */
	@Override public String toString()
	{
		return "" + m_r + ", " + m_g + ", " + m_b + "," + m_a;
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
	    	
	    if ((in_object instanceof Colour) == false)
	    {
	    	return false;
	    }
	    
	    Colour colour = (Colour)in_object;
	    return (Double.compare(m_r, colour.m_r) == 0 && Double.compare(m_g, colour.m_g) == 0 && Double.compare(m_b, colour.m_b) == 0 && Double.compare(m_a, colour.m_a) == 0);
	}
	/**
	 * @author Ian Copland
	 *
	 * @return The hash code for this object.
	 */
	@Override public int hashCode()
	{
		int hash = HashCodeUtils.INITIAL_VALUE;
		hash = HashCodeUtils.add(hash, m_r);
		hash = HashCodeUtils.add(hash, m_g);
		hash = HashCodeUtils.add(hash, m_b);
		hash = HashCodeUtils.add(hash, m_a);
		return hash;
	}
}
