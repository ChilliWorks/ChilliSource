package com.chillisource.toolutils;

public class Vector4 
{
	//------------------------------------------------------------
	/// public member data.
	//------------------------------------------------------------
	public float x;
	public float y;
	public float z;
	public float w;
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public Vector4()
	{
		x = y = z = w = 0.0f;
	}
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public Vector4(float infX, float infY, float infZ, float infW)
	{
		x = infX;
		y = infY;
		z = infZ;
		w = infW;
	}
	//------------------------------------------------------------
	/// Copy
	///
	/// @return a copy of this vector.
	//------------------------------------------------------------
	public Vector4 copy()
	{
		return new Vector4(z, y, z, w);
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
	    if ((inObj instanceof Vector4) == false) 
	    	return false;
	    Vector4 vec = (Vector4)inObj;
	    if (Float.floatToIntBits(x) == Float.floatToIntBits(vec.x) && Float.floatToIntBits(y) == Float.floatToIntBits(vec.y)  && Float.floatToIntBits(z) == Float.floatToIntBits(vec.z) && Float.floatToIntBits(w) == Float.floatToIntBits(vec.w))
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
