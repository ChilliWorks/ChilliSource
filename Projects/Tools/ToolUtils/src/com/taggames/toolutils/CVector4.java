package com.taggames.toolutils;

public class CVector4 
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
	public CVector4()
	{
		x = y = z = w = 0.0f;
	}
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public CVector4(float infX, float infY, float infZ, float infW)
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
	public CVector4 Copy()
	{
		return new CVector4(z, y, z, w);
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
	    if ((inObj instanceof CVector4) == false) 
	    	return false;
	    CVector4 vec = (CVector4)inObj;
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
