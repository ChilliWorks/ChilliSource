package com.taggames.toolutils;

public class CVector2 
{
	//------------------------------------------------------------
	/// public memeber data
	//------------------------------------------------------------
	public float x;
	public float y;
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public CVector2()
	{
		x = y = 0.0f;
	}
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public CVector2(float infX, float infY)
	{
		x = infX;
		y = infY;
	}
	//------------------------------------------------------------
	/// Copy
	///
	/// @return a copy of this vector.
	//------------------------------------------------------------
	public CVector2 Copy()
	{
		return new CVector2(x, y);
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
	    if ((inObj instanceof CVector2) == false) 
	    	return false;
	    CVector2 vec = (CVector2)inObj;
	    if (Float.floatToIntBits(x) == Float.floatToIntBits(vec.x) && Float.floatToIntBits(y) == Float.floatToIntBits(vec.y))
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
		return "(" + x + ", " + y + ")";
	}
}
