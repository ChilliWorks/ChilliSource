/*
 *  IActivityResults.java
 *  moFlow
 *
 *  Created by Scott Downie on 16/01/2013.
 *  Copyright 2013 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

import android.content.Intent;

public interface IActivityResults 
{
	//------------------------------------------------------
	/// On Activity Result
	///
	/// Called from the activity upon receiving
	/// an intent result
	///
	/// @param Request Code
	/// @param Result Code
	/// @param Intent
	//------------------------------------------------------
	public abstract void OnActivityResult(int requestCode, int resultCode, Intent data);
}