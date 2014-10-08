/**
 * GooglePlayRemoteNotificationsNativeInterface.java
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

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.INativeInterface;
import com.chilliworks.chillisource.core.InterfaceIDType;
import com.chilliworks.chillisource.core.Logging;

public class GooglePlayRemoteNotificationNativeInterface extends INativeInterface
{
	public static InterfaceIDType InterfaceID = new InterfaceIDType("CGooglePlayRemoteNotificationsNativeInterface");
	
	//---------------------------------------------------------------------
	/// Native On Remote Token Received
	///
	/// Used to pass the remote token to the native code.
	///
	/// @param The device token to pass to the server
	//---------------------------------------------------------------------
	public native void NativeOnRemoteTokenReceived(final String inToken);
	//---------------------------------------------------------------------
	/// Native On Remote Notification Received
	///
	/// Used to pass the remote notification to the native code.
	///
	/// @param Keys for notification data
	/// @param Values for notification data
	//---------------------------------------------------------------------
	public native void NativeOnRemoteNotificationReceived(final String[] inastrKeys, final String[] inastrValues);
	
	//---------------------------------------------------------------------
	/// Constructor
	//---------------------------------------------------------------------
	public GooglePlayRemoteNotificationNativeInterface()
	{
	}
	//---------------------------------------------------------------------
	/// Is A
	///
	/// @param Interface ID
	/// @return Whether the system implements the given interface
	//---------------------------------------------------------------------
	@Override public boolean IsA(InterfaceIDType inInterfaceType)
	{
		return (inInterfaceType == InterfaceID);
	}
	//---------------------------------------------------------------------
	/// Request Remote Token
	///
	/// Registers a remote notification token which is returned to the
	/// application ready to be passed to the server.
	//---------------------------------------------------------------------
	public void RequestRemoteToken()
	{
		PackageManager manager = CSApplication.get().getActivityContext().getPackageManager();
		ApplicationInfo info;
		try 
		{
			info = manager.getApplicationInfo(CSApplication.get().getActivityContext().getPackageName(), PackageManager.GET_META_DATA);
		} 
		catch (NameNotFoundException e) 
		{
			Logging.logError("CRemoteNotificationNativeInterface: Failed to get 'GoogleProjectID' from the metadata");
			e.printStackTrace();
			return;
		}
		
	    String strProjectID = info.metaData.getString("GoogleProjectID");
	    
		if(strProjectID == null || strProjectID.length() == 0)
		{
			Logging.logError("CRemoteNotificationNativeInterface: Failed to get 'GoogleProjectID' from the metadata");
			return;
		}
		
		// Make sure the device has the proper dependencies.
		GCMService.SetSenderID(strProjectID);
        GCMRegistrar.checkDevice(CSApplication.get().getActivityContext());
        GCMRegistrar.checkManifest(CSApplication.get().getActivityContext());
        GCMRegistrar.register(CSApplication.get().getActivityContext(), strProjectID);
	}
	//---------------------------------------------------------------------
	/// On Remote Token Received
	///
	/// Passes the remote notification token for this device back to the
	/// application.
	///
	/// @param The device token to pass to the server
	//---------------------------------------------------------------------
	public void OnRemoteTokenReceived(final String instrToken)
	{
		NativeOnRemoteTokenReceived(instrToken);
	}
	//---------------------------------------------------------------------
	/// On Remote Notification Received
	///
	/// Passes the message and payload to the app.
	///
	/// @param Keys for notification data
	/// @param Values for notification data
	//---------------------------------------------------------------------	
	public void OnRemoteNotificationReceived(final String[] inastrKeys, final String[] inastrValues)
	{
		NativeOnRemoteNotificationReceived(inastrKeys, inastrValues);
	}
}
