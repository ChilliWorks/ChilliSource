/*
 *  CInterfaceIDType.java
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.core;

//=========================================================
/// Interface ID Type
///
/// A Wrapper class for containing the interface ID. This
/// allows us to change the ID type for optimisation later
/// on. (e.g, convert to a hashed value rather than a 
/// string compare).
//=========================================================
public class CInterfaceIDType
{
	//----------------------------------------------------
	/// Private Member Data
	//----------------------------------------------------
	private String mstrID;
	//----------------------------------------------------
	/// Constructor
	//----------------------------------------------------
	public CInterfaceIDType(String instrInterfaceName)
	{
		mstrID = instrInterfaceName;
	}
	//----------------------------------------------------
	/// Equals
	//----------------------------------------------------
	public boolean Equals(CInterfaceIDType inOther)
	{
		if (mstrID.equals(inOther.mstrID) == true)
		{
			return true;
		}
		return false;
	}
}
