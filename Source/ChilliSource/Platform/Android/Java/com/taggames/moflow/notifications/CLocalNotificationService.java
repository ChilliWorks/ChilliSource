/*
 *  CLocalNotificationService.java
 *  moFlow
 *
 *  Created by Steven Hendrie on 24/10/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.notifications;

import java.util.Iterator;

import com.taggames.moflow.nativeinterface.CLocalNotificationNativeInterface;
import android.app.Service;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;

//=============================================================
/// Local Notification Service
///
/// Handles received local notifications and passes them on
/// to moFlow.
//=============================================================
public class CLocalNotificationService extends Service
{	
	//---------------------------------------------------------
	/// On Bind
	///
	/// This isn't used.
	//---------------------------------------------------------
	@Override public IBinder onBind(Intent arg0) 
	{
		return null;
	}
	//---------------------------------------------------------
	/// On Start Command
	///
	/// Processes the given intent that has been passed from
	/// the Local Notification Receiver. The intent describes
	/// a local notification that can be passed on to moFlow.
	///
	/// @param The intent.
	/// @param The flags.
	/// @param The start ID.
	//---------------------------------------------------------
	@Override public int onStartCommand(Intent inIntent, int flags, int startId) 
	{
		if(inIntent != null)
		{
			Bundle mapParams = inIntent.getExtras();

			mapParams.remove("IntentID");
			int dwParamSize = mapParams.size();

			String[] astrKeys = new String[dwParamSize];
			String[] astrValues = new String[dwParamSize];

			Iterator<String> iter = mapParams.keySet().iterator();
			int udwParamNumber = 0;
			while(iter.hasNext())
			{
				astrKeys[udwParamNumber] = iter.next();			
				astrValues[udwParamNumber] = mapParams.get(astrKeys[udwParamNumber]).toString();
				++udwParamNumber;
			}	

			CLocalNotificationNativeInterface.ApplicationDidReceiveLocalNotification(astrKeys, astrValues);        
		}
		return super.onStartCommand(inIntent, flags, startId);
	}
}
