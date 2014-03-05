package com.chillisource.googleplay;

import com.chillisource.core.ChilliSourceActivity;
import com.chillisource.core.InterfaceIDType;
import com.chillisource.core.ResourceHelper;
import com.chillisource.core.INativeInterface;
import com.chillisource.googleplay.GCMService;
import com.chillisource.googleplay.GCMRegistrar;

import android.app.PendingIntent;
import android.content.Intent;

public class RemoteNotificationNativeInterface extends INativeInterface
{
	//--------------------------------------------------------------
	/// Static Member Data
	//--------------------------------------------------------------
	public static InterfaceIDType InterfaceID = new InterfaceIDType("CRemoteNotificationNativeInterface");
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
	}
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	@Override public boolean IsA(InterfaceIDType inInterfaceType)
	{
		return (inInterfaceType == InterfaceID);
	}
	//---------------------------------------------------------------------
	/// Register
	///
	/// Registers for remote notifications.
	//---------------------------------------------------------------------
	public void Register()
	{
		String strProjectID = "";
		int keyStringID = ResourceHelper.GetDynamicResourceIDForField(mActivity, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "GoogleProjectID");
		if(keyStringID > 0)
		{
			strProjectID = mActivity.getString(keyStringID);
		}
		else
		{
			android.util.Log.e("moFlow", "CRemoteNotificationNativeInterface: Failed to get 'GoogleProjectID' from the Values resource");
			return;
		}
		
		// Make sure the device has the proper dependencies.
		GCMService.SetSenderID(strProjectID);
        GCMRegistrar.checkDevice(ChilliSourceActivity.GetActivity());
        GCMRegistrar.checkManifest(ChilliSourceActivity.GetActivity());
        GCMRegistrar.register(ChilliSourceActivity.GetActivity(), strProjectID);
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
		unregIntent.putExtra("app", PendingIntent.getBroadcast(ChilliSourceActivity.GetActivity(), 0, new Intent(), 0));
		ChilliSourceActivity.GetActivity().startService(unregIntent);
	}
}
