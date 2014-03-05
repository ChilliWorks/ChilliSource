//
//  ILaunchIntentReceiver.java
//  moFlow
// 
//  Created by Ian Copland on 04/12/2013
//  Copyright 2013 Tag Games Ltd. All rights reserved.
//

package com.chillisource.core;

import android.content.Intent;

//==========================================================
/// Launch Intent Receiver
///
/// This receives all new launch intents. There are two 
/// instances where the application can receive launch 
/// intents:
///
///  - On start up.
///  - When the activity is set to launchMode "singleTop" in
///    The manifest and a new launch intent is sent when the
///    activity is already runnning.
///
/// In both cases the "Launch Intent Received" event is sent
/// just before the next "Frame Begin" event is sent to
/// the native side of MoFlow. This means that any native
/// interface that is created during initialisation will
/// receive this event.
//==========================================================
public interface ILaunchIntentReceiver 
{
	//----------------------------------------------------
	/// On Launch Intent Received
	///
	/// Called when the application receives a launch
	/// intent.
	///
	/// @param The new launch intent.
	//----------------------------------------------------
	public abstract void OnLaunchIntentReceived(Intent inIntent);
}
