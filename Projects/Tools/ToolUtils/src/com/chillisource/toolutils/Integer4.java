package com.chillisource.toolutils;

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
