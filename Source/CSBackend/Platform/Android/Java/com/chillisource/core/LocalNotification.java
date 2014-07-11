/**
 * LocalNotification.java
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

import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import org.json.JSONObject;

import android.content.Intent;

/**
 * A container for information on a single Local Notification. 
 * Also provides means to convert the notification to JSON and
 * to an Intent.
 * 
 * @author Ian Copland
 */
public final class LocalNotification
{
	public final static String k_paramNameNotificationId = "NotificationId";
	public final static String k_paramNameIntentId = "IntentId";
	public final static String k_paramNamePriority = "Priority";
	public final static String k_paramNameTime = "Time";
	
	private final static String k_intentAction = "com.chillisource.core.ALARM_NOTIFICATION_INTENT";
	
	private int m_intentId = 0;
	private int m_notificationId = 0;
	private int m_priority = 0;
	private long m_time = 0;
	private Map<String, String> m_params = new HashMap<String, String>();
	
	/**
	 * Constructor.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The intent Id.
	 * @param The notification Id.
	 * @param The priority.
	 * @param The notification time.
	 * @param The notification parameters.
	 */
	public LocalNotification(int in_intentId, int in_notificationId, int in_priority, long in_time, Map<String, String> in_params)
	{
		m_intentId = in_intentId;
		m_notificationId = in_notificationId;
		m_priority = in_notificationId;
		m_time = in_time;
		m_params.putAll(in_params);
	}
	/**
	 * Constructor. Creates a local notification from JSON.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The json object describing the local notification.
	 */
	public LocalNotification(JSONObject in_json)
	{
		//TODO:
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The intent Id.
	 */
	public int getIntentId()
	{
		return m_intentId;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The notification Id.
	 */
	public int getNotificationId()
	{
		return m_notificationId;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The priority.
	 */
	public int getPriority()
	{
		return m_priority;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The notification time.
	 */
	public long getTime()
	{
		return m_time;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return An immutable copy of the parameters.
	 */
	public Map<String, String> getParams()
	{
		return Collections.unmodifiableMap(m_params);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The local notification in JSON form.
	 */
	public JSONObject toJson()
	{
		//TODO:
		return null;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The local notification as an Intent.
	 */
	public Intent toIntent()
	{
		Intent intent = new Intent(CSApplication.get().getActivityContext(), LocalNotificationReceiver.class);
		intent.setAction(k_intentAction);
		
		for (Entry<String, String> entry : m_params.entrySet()) 
		{
			intent.putExtra(entry.getKey(), entry.getValue());
		}
		
		intent.putExtra(k_paramNameIntentId, m_intentId);
		intent.putExtra(k_paramNameNotificationId, m_notificationId);
		intent.putExtra(k_paramNamePriority, m_priority);	
		intent.putExtra(k_paramNameTime, m_time);
		
		return intent;
	}
}
