/*
 *  QueryableInterface.java
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.core;

//=============================================================
/// Queryable Interface
///
/// Interface that allows type querying even on generics.
///
/// Any classes inheriting from this will have to provide
/// the public member "InterfaceIDType InterfaceID".
//=============================================================
public abstract class IQueryableInterface
{
	//---------------------------------------------------------
	/// IsA
	///
	/// @return whether or not it is off the given type.
	//---------------------------------------------------------
	public abstract boolean IsA(CInterfaceIDType inInterfaceType);
}
