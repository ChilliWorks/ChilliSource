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

package com.chilliworks.chillisource.toolutils;

public class Integer4 
{
	//------------------------------------------------------------
	/// public member data.
	//------------------------------------------------------------
	public int x;
	public int y;
	public int z;
	public int w;
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public Integer4()
	{
		x = y = z = w = 0;
	}
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public Integer4(int indwX, int indwY, int indwZ, int indwW)
	{
		x = indwX;
		y = indwY;
		z = indwZ;
		w = indwW;
	}
	//------------------------------------------------------------
	/// Copy
	///
	/// @return a copy of this vector.
	//------------------------------------------------------------
	public Integer4 copy()
	{
		return new Integer4(z, y, z, w);
	}
	//------------------------------------------------------------
	/// Equals
	///
	/// @param any object.
	/// @return whether or not this equals the given object.
	//------------------------------------------------------------
	public boolean equals(Object inObj)
	{
	    if ( this == inObj ) 
	    	return true;
	    if ((inObj instanceof Integer4) == false) 
	    	return false;
	    Integer4 vec = (Integer4)inObj;
	    if (x == vec.x && y == vec.y  && z == vec.z && w == vec.w)
	    	return true;
	    return false;
	}
	//------------------------------------------------------------
	/// To String
	///
	/// @return this vector in string form.
	//------------------------------------------------------------
	public String toString()
	{
		return "(" + x + ", " + y + ", " + z + "," + w + ")";
	}
}
