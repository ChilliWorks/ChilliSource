/**
 * LocalNotificationStore.java
 * Chilli Source
 * Created by Ian Copland on 11/07/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
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

package com.chillisource.core;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.SharedPreferences;

/**
 * A container class for storing all currently pending Local Notifications.
 * Notifications are stored in Shared Preferences meaning they will persist
 * when the application is restarted.
 * 
 * @author Ian Copland
 */
public final class LocalNotificationStore
{
	private static final String k_notificationJsonKey = "NotificationJson";
	private static final String k_notificationArrayName = "LocalNotifications";
	
	private List<LocalNotification> m_notifications = new ArrayList<LocalNotification>();
	private String m_sharedPreferenceName = "";
	
	/**
	 * Constructor
	 * 
	 * @author Ian Copland
	 * 
	 * @param The name of the shared preferences instance.
	 */
	public LocalNotificationStore(String in_sharedPrefName)
	{
		m_sharedPreferenceName = in_sharedPrefName;
		
		try
		{
			JSONObject jsonRoot = readJson();
			JSONArray jsonNotifications = jsonRoot.getJSONArray(k_notificationArrayName);
			for (int i = 0; i < jsonNotifications.length();)
			{
				JSONObject notificationJson = jsonNotifications.getJSONObject(i);
				LocalNotification notification = new LocalNotification(notificationJson); 
				m_notifications.add(notification);
			}
		}
		catch (JSONException e)
		{
			Logging.logFatal("An exception occurred while constructing the initial local notification store from Json: \n" + ExceptionUtils.ConvertToString(e));
		}
	}
	/**
	 * Stores the given notification. This keeps a cached version of it and
	 * also stores it in Shared Preferences so that it will persist when
	 * restarting the application.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The local notification.
	 */
	public void add(LocalNotification in_notification)
	{
		m_notifications.add(in_notification);
		
		try
		{
			JSONObject jsonRoot = readJson();
			JSONArray jsonNotifications = jsonRoot.getJSONArray(k_notificationArrayName);
			jsonNotifications.put(in_notification.toJson());
			writeJson(jsonRoot);
		}
		catch (JSONException e)
		{
			Logging.logFatal("An exception occurred while adding to the local notification Json: \n" + ExceptionUtils.ConvertToString(e));
		}
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return An immutable list of stored notifications.
	 */
	public List<LocalNotification> getNotifications()
	{
		return Collections.unmodifiableList(m_notifications);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The notification with the given intent Id. If none exists
	 * then null will be returned.
	 */
	public LocalNotification getNotificationWithIntentId(int in_intentId)
	{
		for (LocalNotification notification : m_notifications)
		{
			if (notification.getIntentId() == in_intentId)
			{
				return notification;
			}
		}
		
		return null;
	}
	/**
	 * Removes the given notification from both the cache and Shared
	 * Preference.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The local notification.
	 */
	public void remove(LocalNotification in_notification)
	{
		m_notifications.remove(in_notification);
		
		try
		{
			JSONObject jsonRoot = readJson();
			JSONArray jsonNotifications = jsonRoot.getJSONArray(k_notificationArrayName);
			for (int i = 0; i < jsonNotifications.length();)
			{
				JSONObject notificationJson = jsonNotifications.getJSONObject(i);
				LocalNotification notification = new LocalNotification(notificationJson); 
				if (notification.getIntentId() == in_notification.getIntentId())
				{
					jsonNotifications.remove(i);
				}
				else
				{
					++i;
				}
			}
			writeJson(jsonRoot);
		}
		catch (JSONException e)
		{
			Logging.logFatal("An exception occurred while adding to the local notification Json: \n" + ExceptionUtils.ConvertToString(e));
		}
	}
	/**
	 * Read the stored Json from Shared Preferences. If no Json exists,
	 * a new Empty Json array will be returned.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The local notification.
	 */
	private JSONObject readJson()
	{
		SharedPreferences sharedPref = CSApplication.get().getAppContext().getSharedPreferences(m_sharedPreferenceName, Context.MODE_PRIVATE);
		String jsonString = sharedPref.getString(k_notificationJsonKey, "");
		
		JSONObject jsonRoot = null;
		try
		{
			if (jsonString != null)
			{
				jsonRoot = new JSONObject(jsonString);
			}
			else
			{
				jsonRoot = new JSONObject();
				JSONArray jsonArray = new JSONArray();
				jsonRoot.put(k_notificationArrayName, jsonArray);
			}
		}
		catch (JSONException e)
		{
			Logging.logFatal("An exception occurred while reading the local notification json: \n" + ExceptionUtils.ConvertToString(e));
		}
		
		return jsonRoot;
	}
	/**
	 * Writes the given notification Json to the Shared Preferences.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The notification Json.
	 */
	private void writeJson(JSONObject in_newJson)
	{
		SharedPreferences sharedPref = CSApplication.get().getAppContext().getSharedPreferences(m_sharedPreferenceName, Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = sharedPref.edit();
		editor.putString(k_notificationJsonKey, in_newJson.toString());
		editor.commit();
	}
}
