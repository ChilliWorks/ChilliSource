/*
 *  IResumeable.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.core;

//==========================================================
/// IDestroyable
///
/// Objects implementing this can be destroy on the app
/// desstroying.
//==========================================================
public interface IDestroyable 
{
	//------------------------------------------------------
	/// On Destroy
	///
	/// Called when the application is destroying.
	//------------------------------------------------------
	public abstract void OnDestroy();
}
