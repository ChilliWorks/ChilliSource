/**
 * CLocalNotificationReceiver.java
 * Chilli Source
 * 
 * Created by Steven Hendrie on 24/10/2012.
 * Copyright 2012 Tag Games. All rights reserved.
 */

package com.chillisource.core;

import java.util.Iterator;

import com.chillisource.core.ResourceHelper;

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

/**
 * Receives all local notifications.
 * 
 * @author Steven Hendrie
 */
public class LocalNotificationReceiver extends BroadcastReceiver 
{
	/**
	 * Constants
	 */
	private final static String k_appOpenedFromNotification = "AppOpenedFromNotification";
	private final static String k_arrayOfKeysName = "kstrArrayOfKeysName";
	private final static String k_arrayOfValuesName = "kstrArrayOfValuesName";
	private final static String k_paramNameTitle = "Title";
	private final static String k_paramNameBody = "Body";
	
	/**
	 * Member Data
	 */
	private static PowerManager.WakeLock s_wakeLock;
	
	/**
	 * Called when a local notification broad cast is received. Funnels the notification
	 * into the app if open or into the notification bar if not
	 * 
	 * @author Steven Hendrie
	 * 
	 * @param The context.
	 * @param The intent.
	 */
	@SuppressWarnings("deprecation")
	@Override public void onReceive(Context in_context, Intent in_intent)
	{
		//evaluate whether or not the main engine activity is in the foreground
		boolean isAppInForeground = false;
        if (CSApplication.get() != null && CSApplication.get().isActive() == true)
        {
        	isAppInForeground = true;
        }
        
        //if the main engine activity is in the foreground, simply pass the
        //notification into it. Otherwise display a notification.
		if(isAppInForeground == true)
		{
			final Intent intent = new Intent(in_intent.getAction());
	        Bundle mapParams = in_intent.getExtras();
	        Iterator<String> iter = mapParams.keySet().iterator();
	        
	        while(iter.hasNext())
	        {
	        	String strKey =  iter.next();
	        	intent.putExtra(strKey, mapParams.get(strKey).toString());
	        }
	       
	        Runnable task = new Runnable()
	        { 
	        	@Override public void run() 
	        	{
	        		LocalNotificationNativeInterface localNotificationNI = (LocalNotificationNativeInterface)CSApplication.get().getSystem(LocalNotificationNativeInterface.InterfaceID);
	        		if (localNotificationNI != null)
	        		{
	        			localNotificationNI.onNotificationReceived(intent);
	        		}   
	        	}
	        };

	        CSApplication.get().scheduleMainThreadTask(task);
		}
		else
		{
			//aquire a wake lock
			if(s_wakeLock != null)
			{
				s_wakeLock.release();
			}
			
			PowerManager pm = (PowerManager) in_context.getSystemService(Context.POWER_SERVICE);
			s_wakeLock = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP | PowerManager.ON_AFTER_RELEASE, "NotificationWakeLock");
			s_wakeLock.acquire();
			
			//pull out the information from the intent
			Bundle params = in_intent.getExtras();
			CharSequence title = params.getString(k_paramNameTitle);
			CharSequence text =  params.getString(k_paramNameBody);		
			int intentId = params.getInt(LocalNotificationNativeInterface.k_paramNameNotificationId);

			int paramSize = params.size();
			String[] keys = new String[paramSize];
			String[] values = new String[paramSize];
			Iterator<String> iter = params.keySet().iterator();
			int paramNumber = 0;
			while(iter.hasNext())
			{
				keys[paramNumber] = iter.next();			
				values[paramNumber] = params.get(keys[paramNumber]).toString();
				++paramNumber;
			}	
	
			Intent openAppIntent = new Intent(in_context, CSActivity.class);
			openAppIntent.setAction("android.intent.action.MAIN");
			openAppIntent.addCategory("android.intent.category.LAUNCHER");
			openAppIntent.putExtra(k_appOpenedFromNotification, true);
			openAppIntent.putExtra(k_arrayOfKeysName, keys);
			openAppIntent.putExtra(k_arrayOfValuesName, values);
			openAppIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
			PendingIntent sContentIntent = PendingIntent.getActivity(in_context, intentId, openAppIntent, PendingIntent.FLAG_UPDATE_CURRENT);
			
			Bitmap largeIconBitmap = null;
			int LargeIconID = ResourceHelper.GetDynamicResourceIDForField(in_context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "ic_stat_notify_large");
			
			//Use small icon if no large icon
			if(LargeIconID == 0)
			{
				LargeIconID = ResourceHelper.GetDynamicResourceIDForField(in_context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "ic_stat_notify");
			}
			
			if(LargeIconID > 0)
			{
				largeIconBitmap = BitmapFactory.decodeResource(in_context.getResources(), LargeIconID);
			}
			
			Notification notification = new NotificationCompat.Builder(in_context)
				.setContentTitle(title)
				.setContentText(text)
				.setSmallIcon(ResourceHelper.GetDynamicResourceIDForField(in_context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "ic_stat_notify"))
				.setLargeIcon(largeIconBitmap)
				.setContentIntent(sContentIntent)
				.build();
			
			NotificationManager notificationManager = (NotificationManager) in_context.getSystemService(Context.NOTIFICATION_SERVICE);
			notificationManager.notify(intentId, notification);
			Toast.makeText(in_context, text, Toast.LENGTH_LONG).show();	 
		}
	}
}
