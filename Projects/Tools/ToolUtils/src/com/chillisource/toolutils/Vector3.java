package com.chillisource.toolutils;

public class Vector3 
{
	//------------------------------------------------------------
	/// Public Member data
	//------------------------------------------------------------
	public float x;
	public float y;
	public float z;
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public Vector3()
	{
		x = y = z = 0.0f;
	}
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public Vector3(float infX, float infY, float infZ)
	{
		x = infX;
		y = infY;
		z = infZ;
	}
	//------------------------------------------------------------
	/// Copy
	///
	/// @returns a copy of this vector.
	//------------------------------------------------------------
	public Vector3 copy()
	{
		return new Vector3(x, y, z);
	}
	//------------------------------------------------------------
	/// Length
	///
	/// @return the length of this vector.
	//------------------------------------------------------------
	public float length()
	{
		return (float)Math.sqrt((double)(x*x + y*y + z*z));
	}
	//------------------------------------------------------------
	/// Normalise
	///
	/// @return the normalised version of this vector.
	//------------------------------------------------------------
	public Vector3 normalise()
	{
		Vector3 newVec = new Vector3();
		
		if (!(x == 0.0f && y == 0.0f && z ==0.0f))
		{
			float magni = (float)Math.sqrt((double)(x*x + y*y + z*z));
			newVec.x = x / magni;
			newVec.y = y / magni;
			newVec.z = z / magni;
		}
		
		return newVec;
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
	    if ((inObj instanceof Vector3) == false) 
	    	return false;
	    Vector3 vec = (Vector3)inObj;
	    if (Float.floatToIntBits(x) == Float.floatToIntBits(vec.x) && Float.floatToIntBits(y) == Float.floatToIntBits(vec.y)  && Float.floatToIntBits(z) == Float.floatToIntBits(vec.z))
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
		return "(" + x + ", " + y + ", " + z + ")";
	}
}
