package com.taggames.toolutils;

public class CVector3 
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
	public CVector3()
	{
		x = y = z = 0.0f;
	}
	//------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------
	public CVector3(float infX, float infY, float infZ)
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
	public CVector3 Copy()
	{
		return new CVector3(x, y, z);
	}
	//------------------------------------------------------------
	/// Length
	///
	/// @return the length of this vector.
	//------------------------------------------------------------
	public float Length()
	{
		return (float)Math.sqrt((double)(x*x + y*y + z*z));
	}
	//------------------------------------------------------------
	/// Normalise
	///
	/// @return the normalised version of this vector.
	//------------------------------------------------------------
	public CVector3 Normalise()
	{
		CVector3 newVec = new CVector3();
		
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
	    if ((inObj instanceof CVector3) == false) 
	    	return false;
	    CVector3 vec = (CVector3)inObj;
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
