/**
 * LocalNotificationNativeInterface.java
 * Chilli Source
 * Created by Steven Hendrie on 28/09/2012.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.chilliworks.chillisource.core;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import com.chilliworks.chillisource.core.InterfaceIDType;

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
	public final static InterfaceIDType InterfaceID = new InterfaceIDType("LocalNotificationNativeInterface");
	private final static String k_sharedPrefsName = "LocalNotifications";
	private final static long k_notificationTimeLimitMs = 500 * 1000; 
	
	/**
	 * Members
	 */
	private AlarmManager m_alarmManager = null;
	private LocalNotificationStore m_notificationStore = new LocalNotificationStore(k_sharedPrefsName);
	
	/**
	 * Constructor
	 * 
	 * @author Steven Hendrie
	 */
	public LocalNotificationNativeInterface()
	{
		super();
		
		m_alarmManager = (AlarmManager)CSApplication.get().getActivityContext().getSystemService(Context.ALARM_SERVICE);
		freeOutOfDateIntentIDs();
	}
	
	/**
	 * @author Steven Hendrie
	 * 
	 * @param The interface Id.
	 * 
	 * @return whether or not this implements the given interface.
	 */
	@Override public boolean IsA(InterfaceIDType in_interfaceId) 
	{
		return (in_interfaceId.Equals(InterfaceID));
	}
	
	/**
	 * method accessible from native for allowing System notifications to 
	 * be scheduled for specified time. This should only be called on the Main thread.
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
		
		int intentId = getNextIntentID();
		Map<String, String> params = new HashMap<String, String>();
		for(int paramIndex = 0; paramIndex < in_keys.length; ++paramIndex)
		{
			params.put(in_keys[paramIndex], in_values[paramIndex]);
		}
		LocalNotification notification = new LocalNotification(intentId, in_notificationId, in_priority, in_time, params);
		m_notificationStore.add(notification);

		PendingIntent pendingIntent = PendingIntent.getBroadcast(CSApplication.get().getActivityContext(), intentId , notification.toIntent(), PendingIntent.FLAG_UPDATE_CURRENT);
		m_alarmManager.set(AlarmManager.RTC_WAKEUP, in_time, pendingIntent);
	}
	
	/**
	 * method accessible from native that returns all notifications.
	 * This should only be called on the Main thread.
	 * 
	 * @author Ian Copland
	 * 
	 * @return An array of all notifications.
	 */
	public LocalNotification[] getNotifications()
	{
		List<LocalNotification> notificationList = m_notificationStore.getNotifications();
		return notificationList.toArray(new LocalNotification[0]);
	}
	
	/**
	 * method accessable from native for cancelling previously scheduled 
	 * notifications by id. This should only be called on the Main thread.
	 * 
	 * @author Steven Hendrie
	 * 
	 * @param The notification Id.
	 */
	public void cancelByID(int in_notificationId)
	{
		List<LocalNotification> notifications = m_notificationStore.getNotifications();
		List<LocalNotification> cancelList = new ArrayList<LocalNotification>();
		for (LocalNotification notification : notifications)
		{
			if (notification.getNotificationId() == in_notificationId)
			{
				cancelList.add(notification);
			}
		}
		
		for(LocalNotification cancel : cancelList)
		{
			PendingIntent pendingIntent = PendingIntent.getBroadcast(CSApplication.get().getActivityContext(), cancel.getIntentId() , cancel.toIntent(), PendingIntent.FLAG_UPDATE_CURRENT);
			m_alarmManager.cancel(pendingIntent);
			m_notificationStore.remove(cancel);
		}
	}
	
	/**
	 * Method accessable from native for canceling all previously 
	 * scheduled notifications. This should only be called on the Main thread.
	 * 
	 * @author Steven Hendrie
	 */
	public void cancelAll()
	{
		List<LocalNotification> notifications = m_notificationStore.getNotifications();
		List<LocalNotification> cancelList = new ArrayList<LocalNotification>();
		for (LocalNotification notification : notifications)
		{
			cancelList.add(notification);
		}
		
		for (LocalNotification notification : cancelList)
		{
			PendingIntent pendingIntent = PendingIntent.getBroadcast(CSApplication.get().getActivityContext(), notification.getIntentId(), notification.toIntent(), PendingIntent.FLAG_UPDATE_CURRENT);
			m_alarmManager.cancel(pendingIntent);
			m_notificationStore.remove(notification);
		}
	}
	
	/**
	 * Called when a new notification intent is received. This can be called from
	 * the UI thread.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The received intent.
	 */
	public void onNotificationReceived(Intent in_intent)
	{
		LocalNotification receivedNotification = new LocalNotification(in_intent);
		
		LocalNotification storedNotification = m_notificationStore.getNotificationWithIntentId(receivedNotification.getIntentId());
		if (storedNotification != null)
		{
			m_notificationStore.remove(storedNotification);
			
			Map<String, String> params = storedNotification.getParams();
			int paramSize = params.size();
			String[] keys = new String[paramSize];
			String[] values = new String[paramSize];
			int index = 0;
			for (Entry<String, String> entry : params.entrySet()) 
			{
				keys[index] = entry.getKey();
				values[index] = entry.getValue();
				++index;
			}
			
			nativeOnNotificationReceived(storedNotification.getNotificationId(), keys, values, storedNotification.getPriority());
		}
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
			if(m_notificationStore.getNotificationWithIntentId(count) != null)
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
		long currentTime = System.currentTimeMillis();
		List<LocalNotification> notifications = m_notificationStore.getNotifications();
		List<LocalNotification> removeList = new ArrayList<LocalNotification>();
		for (LocalNotification notification : notifications)
		{
			if (currentTime > notification.getTime() + k_notificationTimeLimitMs)
			{
				removeList.add(notification);
			}
		}
		
		for (LocalNotification notification : removeList)
		{
			m_notificationStore.remove(notification);
		}
	}
	
	/**
	 * Passes a received notification down to the native
	 * side of the engine.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The notification Id.
	 * @param An array of the parameter keys.
	 * @param An array of the parameter values.
	 * @param The priority of the notification.
	 */
	native private void nativeOnNotificationReceived(int in_id, String[] in_keys, String[] in_values, int in_priority);
}