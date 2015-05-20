/**
 * RemoteNotificationNativeInterface.java
 * Chilli Source
 * Created by Robert Henning on 25/06/2011.
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

package com.chilliworks.chillisource.googleplay.core;

import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.InterfaceID;
import com.chilliworks.chillisource.core.System;
import com.chilliworks.chillisource.core.Logging;

import android.app.PendingIntent;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

public class RemoteNotificationNativeInterface extends System
{
	//--------------------------------------------------------------
	/// Static Member Data
	//--------------------------------------------------------------
    public static InterfaceID INTERFACE_ID = new InterfaceID();
	//--------------------------------------------------------------
	/// Native methods
	//--------------------------------------------------------------
	native public void OnRemoteTokenReceived(String instrText);
	native public void OnNotificationReceived(String[] inastrKeys, String[] inastrValues);
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public RemoteNotificationNativeInterface()
	{
        init();
	}
    /**
     * Allows querying of whether or not the system implements the interface described by the
     * given interface id.
     *
     * @author Ian Copland
     *
     * @param in_interfaceId - The interface id to check
     *
     * @return Whether the system implements the given interface
     */
    @Override public boolean isA(com.chilliworks.chillisource.core.InterfaceID in_interfaceId)
    {
        return (in_interfaceId == INTERFACE_ID);
    }
	//---------------------------------------------------------------------
	/// Register
	///
	/// Registers for remote notifications.
	//---------------------------------------------------------------------
	public void Register()
	{
		PackageManager manager = CSApplication.get().getActivityContext().getPackageManager();
		ApplicationInfo info;
		try 
		{
			info = manager.getApplicationInfo(CSApplication.get().getActivityContext().getPackageName(), PackageManager.GET_META_DATA);
		} 
		catch (NameNotFoundException e) 
		{
			Logging.logError("RemoteNotificationNativeInterface: Failed to get 'GoogleProjectID' from the metadata");
			e.printStackTrace();
			return;
		}
		
	    String strProjectID = info.metaData.getString("GoogleProjectID");
	    
		if(strProjectID == null || strProjectID.length() == 0)
		{
			Logging.logError("RemoteNotificationNativeInterface: Failed to get 'GoogleProjectID' from the metadata");
			return;
		}
		
		// Make sure the device has the proper dependencies.
		GCMService.SetSenderID(strProjectID);
        GCMRegistrar.checkDevice(CSApplication.get().getActivityContext());
        GCMRegistrar.checkManifest(CSApplication.get().getActivityContext());
        GCMRegistrar.register(CSApplication.get().getActivityContext(), strProjectID);
	}
	//---------------------------------------------------------------------
	/// Unregister
	///
	/// Sends a request to remove registration from GCM so that
	/// push notifications are not received anymore
	//---------------------------------------------------------------------
	public void Unregister()
	{
		Intent unregIntent = new Intent("com.google.android.c2dm.intent.UNREGISTER");
		unregIntent.putExtra("app", PendingIntent.getBroadcast(CSApplication.get().getActivityContext(), 0, new Intent(), 0));
		CSApplication.get().getActivityContext().startService(unregIntent);
	}
}
