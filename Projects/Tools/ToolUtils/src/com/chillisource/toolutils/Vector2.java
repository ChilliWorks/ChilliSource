package com.chillisource.toolutils;

public class Vector2 
{
	//------------------------------------------------------------
	/// public memeber data
	//------------------------------------------------------------
	public float x;
	public float y;
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public Vector2()
	{
		x = y = 0.0f;
	}
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public Vector2(float infX, float infY)
	{
		x = infX;
		y = infY;
	}
	//------------------------------------------------------------
	/// Copy
	///
	/// @return a copy of this vector.
	//------------------------------------------------------------
	public Vector2 copy()
	{
		return new Vector2(x, y);
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
	    if ((inObj instanceof Vector2) == false) 
	    	return false;
	    Vector2 vec = (Vector2)inObj;
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
