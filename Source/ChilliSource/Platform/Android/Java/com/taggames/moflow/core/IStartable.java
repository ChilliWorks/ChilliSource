/*
 *  IStartable.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.core;

//==========================================================
/// Startable
///
/// Objects implementing this can be resumed on the app
/// starting.
//==========================================================
public interface IStartable 
{
	//------------------------------------------------------
	/// On Start
	///
	/// Called when the application is resumed.
	//------------------------------------------------------
	public abstract void OnStart();
}
