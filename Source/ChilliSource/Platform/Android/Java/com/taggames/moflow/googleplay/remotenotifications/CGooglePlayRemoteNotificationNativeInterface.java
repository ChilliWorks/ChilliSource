/**
 * CGooglePlayRemoteNotificationsNativeInterface.java
 * moFlow
 *
 * Created by Robert Henning on 23/01/2014
 * Copyright (c)2014 Tag Games Limited - All rights reserved
 */

package com.taggames.moflow.googleplay.remotenotifications;

import com.taggames.moflow.CMoFlowActivity;
import com.taggames.moflow.core.CMoFlowResourceHelper;
import com.taggames.moflow.core.CInterfaceIDType;
import com.taggames.moflow.nativeinterface.INativeInterface;

public class CGooglePlayRemoteNotificationNativeInterface extends INativeInterface
{
	public static CInterfaceIDType InterfaceID = new CInterfaceIDType("CGooglePlayRemoteNotificationsNativeInterface");
	
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
	public CGooglePlayRemoteNotificationNativeInterface()
	{
	}
	//---------------------------------------------------------------------
	/// Is A
	///
	/// @param Interface ID
	/// @return Whether the system implements the given interface
	//---------------------------------------------------------------------
	@Override public boolean IsA(CInterfaceIDType inInterfaceType)
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
		int keyStringID = CMoFlowResourceHelper.GetDynamicResourceIDForField(mActivity, CMoFlowResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "GoogleProjectID");
		if(keyStringID > 0)
		{
			strProjectID = mActivity.getString(keyStringID);
			android.util.Log.e("moFlow", "CRemoteNotificationNativeInterface::RequestRemoteToken - Got strProjectID:"+strProjectID);
		}
		else
		{
			android.util.Log.e("moFlow", "CRemoteNotificationNativeInterface: Failed to get 'GoogleProjectID' from the Values resource");
			return;
		}
		
		// Make sure the device has the proper dependencies.
		GCMService.SetSenderID(strProjectID);
        GCMRegistrar.checkDevice(CMoFlowActivity.GetActivity());
        GCMRegistrar.checkManifest(CMoFlowActivity.GetActivity());
        GCMRegistrar.register(CMoFlowActivity.GetActivity(), strProjectID);
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
