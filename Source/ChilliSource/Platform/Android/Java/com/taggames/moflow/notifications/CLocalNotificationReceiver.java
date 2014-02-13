/*
 *  CLocalNotificationReceiver.java
 *  moFlow
 *
 *  Created by Steven Hendrie on 24/10/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.notifications;

import java.util.Iterator;

import com.taggames.moflow.CMoFlowActivity;
import com.taggames.moflow.core.CMoFlowResourceHelper;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.PowerManager;
import android.support.v4.app.NotificationCompat;
import android.widget.Toast;

//=============================================================
/// Local Notification Receiver
///
/// Receives all local notifications.
//=============================================================
public class CLocalNotificationReceiver extends BroadcastReceiver 
{
	//---------------------------------------------------------
	/// Constants
	//---------------------------------------------------------
	private final static String kstrAppOpenedFromNotification = "AppOpenedFromNotification";
	private final static String kstrArrayOfKeysName = "kstrArrayOfKeysName";
	private final static String kstrArrayOfValuesName = "kstrArrayOfValuesName";
	//---------------------------------------------------------
	/// Static Member data
	//---------------------------------------------------------
	private static PowerManager.WakeLock sWakeLock;
	//---------------------------------------------------------
	/// On Receive
	///
	/// Called when a local notification broad cast is recieved.
	///
	/// @param The context.
	/// @param The intent.
	//---------------------------------------------------------
	@Override public void onReceive(Context inContext, Intent inIntent)
	{
		boolean bAppInForeground = false;
		
        CMoFlowActivity moFlowActivity = CMoFlowActivity.GetActivity();
        if (moFlowActivity != null && moFlowActivity.IsActive() == true)
        {
        	bAppInForeground = true;
        }
        
		if(bAppInForeground)
		{
			Intent intent = new Intent(inIntent.getAction());
	        intent.setClass(inContext, CLocalNotificationService.class);
	        Bundle mapParams = inIntent.getExtras();
	        Iterator<String> iter = mapParams.keySet().iterator();
	        while(iter.hasNext())
	        {
	        	String strKey =  iter.next();
	        	intent.putExtra(strKey, mapParams.get(strKey).toString());
	        }
	        inContext.startService(intent);
		}
		else
		{
			if(sWakeLock != null)sWakeLock.release();
			
			PowerManager pm = (PowerManager) inContext.getSystemService(Context.POWER_SERVICE);
			sWakeLock = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK |
	                PowerManager.ACQUIRE_CAUSES_WAKEUP |
	                PowerManager.ON_AFTER_RELEASE, "NotificationWakeLock");
			sWakeLock.acquire();
			
			Bundle mapParams = inIntent.getExtras();
			CharSequence strContentTitle = mapParams.getString("Title");
			CharSequence strContextText =  mapParams.getString("Body");		
			
			int dwIntentID = mapParams.getInt("IntentID");
			//This has been added by me and isn't part of the original notification therefore it doesn't get sent back down to client
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
	
			Intent sOpenAppIntent = new Intent(inContext,CMoFlowActivity.class);
			sOpenAppIntent.setAction("android.intent.action.MAIN");
			sOpenAppIntent.addCategory("android.intent.category.LAUNCHER");
			sOpenAppIntent.putExtra(kstrAppOpenedFromNotification, true);
			sOpenAppIntent.putExtra(kstrArrayOfKeysName, astrKeys);
			sOpenAppIntent.putExtra(kstrArrayOfValuesName, astrValues);
			sOpenAppIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
			PendingIntent sContentIntent = PendingIntent.getActivity(inContext, dwIntentID, sOpenAppIntent, PendingIntent.FLAG_UPDATE_CURRENT);
			
			Bitmap largeIconBitmap = null;
			int LargeIconID = CMoFlowResourceHelper.GetDynamicResourceIDForField(inContext, CMoFlowResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "gcmiconlarge");
			if(LargeIconID > 0)
			{
				largeIconBitmap = BitmapFactory.decodeResource(inContext.getResources(), LargeIconID);
			}
			
			Notification sNotification = new NotificationCompat.Builder(inContext)
			.setContentTitle(strContentTitle)
			.setContentText(strContextText)
			.setSmallIcon(CMoFlowResourceHelper.GetDynamicResourceIDForField(inContext, CMoFlowResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "gcmicon"))
			.setLargeIcon(largeIconBitmap)
			.setContentIntent(sContentIntent)
			.build();
			
			NotificationManager sNotificationManager = (NotificationManager) inContext.getSystemService(Context.NOTIFICATION_SERVICE);
			sNotificationManager.notify(dwIntentID, sNotification);
			Toast.makeText(inContext,strContextText, Toast.LENGTH_LONG).show();	 
		}
	}
}
