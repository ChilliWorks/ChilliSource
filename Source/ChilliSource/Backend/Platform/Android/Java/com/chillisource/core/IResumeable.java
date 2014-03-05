/*
 *  IResumeable.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

//==========================================================
/// Resumeable
///
/// Objects implementing this can be resumed on the app
/// resuming.
//==========================================================
public interface IResumeable 
{
	//------------------------------------------------------
	/// On Resume
	///
	/// Called when the application is resumed.
	//------------------------------------------------------
	public abstract void OnResume();
}
