/**
 * LocalNotificationService.java
 * Chilli Source
 * 
 * Created by Steven Hendrie on 24/10/2012.
 * Copyright 2012 Tag Games. All rights reserved.
 */

package com.chillisource.core;

import com.chillisource.core.LocalNotificationNativeInterface;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

/**
 * Handles received local notifications and passes them on to
 * the native side of the engine.
 * 
 * @author Steven Hendrie
 */
public class LocalNotificationService extends Service
{	
	/**
	 * Processes the given intent that has been passed from the 
	 * Local Notification Receiver. The intent describes a local 
	 * notification that can be passed on to moFlow.
	 * 
	 * @author Steven Hendrie
	 * 
	 * @param The intent.
	 * @param The flags.
	 * The start ID.
	 */
	@Override public int onStartCommand(Intent in_intent, int in_flags, int in_startId) 
	{
		if(in_intent != null)
		{
			LocalNotificationNativeInterface localNotificationNI = (LocalNotificationNativeInterface)CSApplication.get().getSystem(LocalNotificationNativeInterface.InterfaceID);
    		if (localNotificationNI != null)
    		{
    			localNotificationNI.onNotificationReceived(in_intent);
    		}     
		}
		
		return super.onStartCommand(in_intent, in_flags, in_flags);
	}
	
	/**
	 * This isn't used.
	 * 
	 * @author Steven Hendrie
	 */
	@Override public IBinder onBind(Intent arg0) 
	{
		return null;
	}
}
