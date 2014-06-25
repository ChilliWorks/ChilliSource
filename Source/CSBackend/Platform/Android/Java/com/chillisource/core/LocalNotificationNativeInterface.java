/**
 * LocalNotificationNativeInterface.java
 * Chilli Source
 * 
 * Created by Steven Hendrie on 28/09/2012.
 * Copyright 2011 Tag Games. All rights reserved.
 */

package com.chillisource.core;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;

import com.chillisource.core.InterfaceIDType;
import com.chillisource.core.LocalNotificationReceiver;

/**
 * A native interface for passing local notifications down
 * to the engine.
 * 
 * @author Steven Hendrie
 */
public class LocalNotificationNativeInterface extends INativeInterface
{
	/**
	 * Constants
	 */
	public final static InterfaceIDType InterfaceID = new InterfaceIDType("CLocalNotificationNativeInterface");
	public final static String k_paramNameNotificationId = "NotificationId";
	public final static String k_paramNameIntentId = "IntentId";
	public final static String k_paramNamePriority = "Priority";
	private final static String k_intentToTriggerMapName = "IntentIDsToTriggerTime";
	private final static String k_intentToNotificationMapName = "IntentIDsToNotificationIDs";
	private final static String k_appOpenedFromNotification = "AppOpenedFromNotification";
	private final static String k_arrayOfKeysName = "kstrArrayOfKeysName";
	private final static String k_arrayOfValuesName = "kstrArrayOfValuesName";
	private final static long k_notificationTimeLimitMs = 500 * 1000; 
	
	/**
	 * Members
	 */
	private Intent m_createNotificationIntent;
	private AlarmManager m_alarmManager;
	
	/**
	 * Constructor
	 * 
	 * @author Steven Hendrie
	 */
	public LocalNotificationNativeInterface()
	{
		super();
		
		m_createNotificationIntent = new Intent(CSApplication.get().getActivityContext(), LocalNotificationReceiver.class);
		m_createNotificationIntent.setAction("com.chillisource.core.ALARM_NOTIFICATION_INTENT");
		m_alarmManager = (AlarmManager)CSApplication.get().getActivityContext().getSystemService(Context.ALARM_SERVICE);
	}
	
	/**
	 * @author Steven Hendrie
	 * 
	 * @return whether or not this implements the given interface.
	 */
	@Override public boolean IsA(InterfaceIDType in_interfaceId) 
	{
		return (in_interfaceId.Equals(InterfaceID));
	}
	
	/**
	 * method accessible from native for allowing System notifications to 
	 * be scheduled for specified time.
	 * 
	 * @author Steven Hendrie
	 * 
	 * @param notification ID
	 * @param string array of keys for params (used to create param map)
	 * @param string array of values for params (used to create param map) 
	 * @param the trigger time in milliseconds
	 * @param The priority.
	 */
	public void scheduleNotificationForTime(int in_notificationId, String[] in_keys, String[] in_values, long in_time, int in_priority)
	{
		freeOutOfDateIntentIDs();		
		
		//Get the next unique id for the notification so that it will not overwrite any of the previously scheduled notifications.
		int intentId = getNextIntentID();
		
		//Pass any data to the intent so that it can be used when the Alarm is triggered
		for(int paramIndex = 0; paramIndex < in_keys.length; ++paramIndex)
		{
			m_createNotificationIntent.putExtra(in_keys[paramIndex], in_values[paramIndex]);
		}			
		
		m_createNotificationIntent.putExtra(k_paramNameNotificationId, in_notificationId);
		m_createNotificationIntent.putExtra(k_paramNameIntentId, intentId);
		m_createNotificationIntent.putExtra(k_paramNamePriority, in_priority);	
		
		//Map unique intent id to time it will be triggered, this will allow us to remove already fired intents later
		SharedPreferencesNativeInterface.SetLong(k_intentToTriggerMapName, Integer.toString(intentId), in_time);
		
		//Map unique intent id to notifications id - this will allow uys to cancel individual notification ids later
		SharedPreferencesNativeInterface.SetInt(k_intentToNotificationMapName, Integer.toString(intentId), in_notificationId);

		PendingIntent pendingIntent = PendingIntent.getBroadcast(CSApplication.get().getActivityContext(), intentId , m_createNotificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);
	
        //set alarm to time of trigger
		m_alarmManager.set(AlarmManager.RTC_WAKEUP, in_time, pendingIntent);
	}
	
	/**
	 * method accessable from native for cancelling previously scheduled 
	 * notifications by id
	 * 
	 * @author Steven Hendrie
	 * 
	 * @param inID
	 */
	public void cancelByID(int in_id)
	{
		SharedPreferences sharedPreferences = CSApplication.get().getActivityContext().getSharedPreferences(k_intentToNotificationMapName, 0);
		Map<String, ?> intentIDMap = sharedPreferences.getAll();
		
		List<Integer> intentIDsToBeRemoved = new ArrayList<Integer>();
		
		for(Map.Entry<String,?> entry : intentIDMap.entrySet())
		{
			if((Integer)entry.getValue() == in_id)
			{
				intentIDsToBeRemoved.add(Integer.parseInt(entry.getKey()));
			}
		}
		
		for(int dwID = 0; dwID < intentIDsToBeRemoved.size(); ++dwID)
		{
			PendingIntent pendingIntent = PendingIntent.getBroadcast(CSApplication.get().getActivityContext(), intentIDsToBeRemoved.get(dwID) , m_createNotificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);
			m_alarmManager.cancel(pendingIntent);
			SharedPreferencesNativeInterface.RemoveKey(k_intentToTriggerMapName, Integer.toString(intentIDsToBeRemoved.get(dwID)));
			SharedPreferencesNativeInterface.RemoveKey(k_intentToNotificationMapName, Integer.toString(intentIDsToBeRemoved.get(dwID)));
		}
        
	}
	
	/**
	 * Method accessable from native for cancelling all previously 
	 * scheduled notifications
	 * 
	 * @author Steven Hendrie
	 */
	public void cancelAll()
	{
		SharedPreferences sharedPreferences = CSApplication.get().getActivityContext().getSharedPreferences(k_intentToTriggerMapName, 0);
		Map<String, ?> pendingIntentIDMap = sharedPreferences.getAll();
		
		List<Integer> intentIDsToBeRemoved = new ArrayList<Integer>();
		
		for(Map.Entry<String,?> entry : pendingIntentIDMap.entrySet())
		{
			intentIDsToBeRemoved.add(Integer.parseInt(entry.getKey()));
		}
		
		for(int dwID = 0; dwID < intentIDsToBeRemoved.size(); ++dwID)
		{
			PendingIntent pendingIntent = PendingIntent.getBroadcast(CSApplication.get().getActivityContext(), intentIDsToBeRemoved.get(dwID) , m_createNotificationIntent, PendingIntent.FLAG_UPDATE_CURRENT);
			m_alarmManager.cancel(pendingIntent);
			SharedPreferencesNativeInterface.RemoveKey(k_intentToTriggerMapName, Integer.toString(intentIDsToBeRemoved.get(dwID)));
			SharedPreferencesNativeInterface.RemoveKey(k_intentToNotificationMapName, Integer.toString(intentIDsToBeRemoved.get(dwID)));
		}
	}
	
	/**
	 * Called when a new notification intent is received.
	 * 
	 * @author Ian Copland
	 */
	public void onNotificationReceived(final Intent in_intent)
	{
		Bundle params = in_intent.getExtras();
		
		//remove the extra data from the intent
		int notificationId = Integer.parseInt(params.getString(k_paramNameNotificationId));
		int priority = Integer.parseInt(params.getString(k_paramNamePriority));
		params.remove(LocalNotificationNativeInterface.k_paramNameNotificationId);
		params.remove(LocalNotificationNativeInterface.k_paramNameIntentId);
		params.remove(LocalNotificationNativeInterface.k_paramNamePriority);
		
		//copy the intent contents to the arrays
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
		
		nativeOnNotificationReceived(notificationId, keys, values, priority);
	}
	
	/**
	 * Determines the next free ID for intent that is passed to the the 
	 * alarm manager to use so that it will not overwrite previously 
	 * placed alarms.
	 * 
	 * @author Steven Hendrie
	 * 
	 * @return Unique ID for notification
	 */
	private int getNextIntentID()
	{
		Boolean foundUniqueID = false;
		int count = 0;
		while(!foundUniqueID)
		{
			//if no notification currently uses this ID
			if(SharedPreferencesNativeInterface.KeyExists(k_intentToTriggerMapName, Integer.toString(count)))
			{
				count++;
			}
			else
			{
				foundUniqueID = true;
			}
		}
		return count;
	}
	
	/**
	 * Goes through all currently placed intentIDs and checks the time 
	 * that they were fired. If they were over the defined time limit 
	 * then they will be removed allowing them to be overwritten. 
	 * This prevents preferences from getting too large
	 * 
	 * @author Steven Hendrie
	 */
	private void freeOutOfDateIntentIDs()
	{
		SharedPreferences sharedPreferences = CSApplication.get().getActivityContext().getSharedPreferences(k_intentToTriggerMapName, 0);
		
		Map<String, ?> intentIDMap = sharedPreferences.getAll();
		List<String> keysToRemove = new ArrayList<String>();
		
		long currentTime = System.currentTimeMillis();
		
		for(Map.Entry<String,?> entry : intentIDMap.entrySet())
		{
			String currentKey = entry.getKey();
			long value =((Number)entry.getValue()).longValue();
			
			if(currentTime > value + k_notificationTimeLimitMs)
			{
				keysToRemove.add(currentKey);
			}
		}		
		for(int key = 0 ; key < keysToRemove.size(); ++key)
		{
			SharedPreferencesNativeInterface.RemoveKey(k_intentToTriggerMapName, keysToRemove.get(key));	
			SharedPreferencesNativeInterface.RemoveKey(k_intentToNotificationMapName, keysToRemove.get(key));	
		}
	}
	
	/**
	 * Passes a received notification down to the native
	 * side of the engine.
	 * 
	 * @author Ian Copland
	 */
	native private void nativeOnNotificationReceived(int in_id, String[] in_keys, String[] in_values, int in_priority);
}