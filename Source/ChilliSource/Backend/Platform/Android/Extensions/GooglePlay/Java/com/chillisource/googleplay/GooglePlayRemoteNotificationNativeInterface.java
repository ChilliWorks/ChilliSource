/**
 * CGooglePlayRemoteNotificationsNativeInterface.java
 * moFlow
 *
 * Created by Robert Henning on 23/01/2014
 * Copyright (c)2014 Tag Games Limited - All rights reserved
 */

package com.chillisource.googleplay;

import com.chillisource.core.CSApplication;
import com.chillisource.core.ResourceHelper;
import com.chillisource.core.InterfaceIDType;
import com.chillisource.core.INativeInterface;

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
		String strProjectID = "";
		int keyStringID = ResourceHelper.GetDynamicResourceIDForField(CSApplication.get().getActivityContext(), ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "GoogleProjectID");
		if(keyStringID > 0)
		{
			strProjectID = CSApplication.get().getActivityContext().getString(keyStringID);
			android.util.Log.e("moFlow", "CRemoteNotificationNativeInterface::RequestRemoteToken - Got strProjectID:"+strProjectID);
		}
		else
		{
			android.util.Log.e("moFlow", "CRemoteNotificationNativeInterface: Failed to get 'GoogleProjectID' from the Values resource");
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
