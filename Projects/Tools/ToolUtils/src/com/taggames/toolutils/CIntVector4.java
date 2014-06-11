package com.taggames.toolutils;

public class CIntVector4 
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
	public CIntVector4()
	{
		x = y = z = w = 0;
	}
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public CIntVector4(int indwX, int indwY, int indwZ, int indwW)
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
	public CIntVector4 Copy()
	{
		return new CIntVector4(z, y, z, w);
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
	    if ((inObj instanceof CIntVector4) == false) 
	    	return false;
	    CIntVector4 vec = (CIntVector4)inObj;
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
