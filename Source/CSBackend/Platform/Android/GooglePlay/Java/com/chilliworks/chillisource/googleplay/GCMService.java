/**
 * GCMService.java
 * Chilli Source
 * Created by Robert Henning on 23/01/2014.
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

package com.chilliworks.chillisource.googleplay;

import java.util.HashMap;
import java.util.Set;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v4.app.NotificationCompat;

import com.chilliworks.chillisource.core.CSActivity;
import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.ResourceHelper;
import com.chilliworks.chillisource.googleplay.GCMBaseIntentService;
import com.chilliworks.chillisource.googleplay.GooglePlayRemoteNotificationNativeInterface;

//=============================================================
/// Remote Notification Service
///
/// IntentService responsible for handling GCM messages.
//=============================================================
public class GCMService extends GCMBaseIntentService 
{
	//---------------------------------------------------------
	/// Constants
	//---------------------------------------------------------
	private static String kstrSenderID = "";
	//---------------------------------------------------------
	/// Set Sender ID
	///
	/// Sets up the sender ID.
	///
	/// @param The Sender ID.
	//---------------------------------------------------------
	public static void SetSenderID(String instrSenderID)
	{
		kstrSenderID = instrSenderID;
	}
	//---------------------------------------------------------
	/// Constructor
	//---------------------------------------------------------
    public GCMService() 
    {
        super(kstrSenderID);
        
        mNativeInterface = new GooglePlayRemoteNotificationNativeInterface();
    }
	//---------------------------------------------------------
	/// On Registered
    ///
    /// This is called when the application has successfully
    /// registered for remote notifications.
    ///
    /// @param The Context.
    /// @param The registration ID.
	//---------------------------------------------------------
    @Override protected void onRegistered(Context context, String registrationId) 
    {
    	mNativeInterface.OnRemoteTokenReceived(registrationId);
    }
	//---------------------------------------------------------
	/// On Unregistered
    ///
    /// This is called when the application has successfully
    /// unregistered for remote notifications.
    ///
    /// @param The context.
    /// @param The registraton ID.
	//---------------------------------------------------------
    @Override protected void onUnregistered(Context context, String registrationId) 
    {
    }
	//---------------------------------------------------------
	/// On Message
    ///
    /// This is called when a notification is received. This
    /// is processed and passed on to the engine.
    ///
    /// @param The context.
    /// @param The intent.
	//---------------------------------------------------------
    @Override protected void onMessage(Context context, Intent intent) 
    {
        HashMap<String, String> params = new HashMap<String, String>();
        Bundle extras = intent.getExtras();
        Set<String> keySet = extras.keySet();
        for (String strKey : keySet)
        {
        	params.put(strKey, extras.getString(strKey));
        }
        generateNotification(context, params);
    }
	//---------------------------------------------------------
	/// On Deleted Message
    ///
    /// @param The context.
    /// @param The total.
	//---------------------------------------------------------
    @Override protected void onDeletedMessages(Context context, int total) 
    {
    }
	//---------------------------------------------------------
	/// On Error
    ///
    /// @param The context.
    /// @param The error ID.
	//---------------------------------------------------------
    @Override public void onError(Context context, String errorId) 
    {
    }
	//---------------------------------------------------------
	/// On Recoverable Error
    ///
    /// @param The context.
    /// @param The error ID.
	//---------------------------------------------------------
    @Override protected boolean onRecoverableError(Context context, String errorId) 
    {
        return super.onRecoverableError(context, errorId);
    }
	//---------------------------------------------------------
	/// Generate Notification
    ///
    /// Issues a notification to inform the user that server 
    /// has sent a message.
    ///
    /// @param The Context.
    /// @param The message.
	//---------------------------------------------------------
    private void generateNotification(Context context, HashMap<String, String> inParams) 
    {
    	//if the application is active then pass this information on to the application.
        if(CSApplication.get() != null && CSApplication.get().isActive() == true)
        {
        	//build the array of keys and values
    		int dwNumEntries = inParams.size();
    		String[] astrKeys = new String[dwNumEntries];
    		String[] astrValues = new String[dwNumEntries];
    		int dwCount = 0;
    		for(String strKey : inParams.keySet())
            {
    			astrKeys[dwCount] = strKey;
    			astrValues[dwCount] = inParams.get(strKey);
    			dwCount++;
            }
    		
    		// Send this to the native side of the engine.
    		mNativeInterface.OnRemoteNotificationReceived(astrKeys, astrValues);
        }
        // Otherwise display a notification.
        else
        {
        	String strTitle = context.getString(ResourceHelper.GetDynamicResourceIDForField(context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "app_name"));
        	String strMessage = inParams.get("message");
        	
            Intent notificationIntent = new Intent(context, CSActivity.class);
            notificationIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
            PendingIntent intent = PendingIntent.getActivity(context, 0, notificationIntent, 0);
            
			Bitmap largeIconBitmap = null;
			int largeIconID = ResourceHelper.GetDynamicResourceIDForField(context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "ic_stat_notify_large");
			
			//If no large icon then use the small icon
        	if(largeIconID == 0)
        	{
        		largeIconID = ResourceHelper.GetDynamicResourceIDForField(context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "ic_stat_notify");
        	}
			
			if(largeIconID > 0)
			{
				largeIconBitmap = BitmapFactory.decodeResource(context.getResources(), largeIconID);
			}
			
			Notification notification = new NotificationCompat.Builder(context)
			.setContentTitle(strTitle)
			.setContentText(strMessage)
			.setSmallIcon(ResourceHelper.GetDynamicResourceIDForField(context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "ic_stat_notify"))
			.setLargeIcon(largeIconBitmap)
			.setContentIntent(intent)
			.build();
			
			NotificationManager notificationManager = (NotificationManager)context.getSystemService(Context.NOTIFICATION_SERVICE);
			notificationManager.notify(0, notification);
        }
    }

    // Member vars
    GooglePlayRemoteNotificationNativeInterface mNativeInterface;
}