/*
 *  IPauseable.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

//==========================================================
/// Pauseable
///
/// Objects implementing this can be paused on the app
/// pausing.
//==========================================================
public interface IPauseable 
{
	//------------------------------------------------------
	/// On Pause
	///
	/// Called when the application is paused.
	//------------------------------------------------------
	public abstract void OnPause();
}
