package com.chillisource.googleplay;

import com.chillisource.core.CSApplication;
import com.chillisource.core.Logging;
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
		int keyStringID = ResourceHelper.GetDynamicResourceIDForField(CSApplication.get().getActivityContext(), ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "GoogleProjectID");
		if(keyStringID > 0)
		{
			strProjectID = CSApplication.get().getActivityContext().getString(keyStringID);
		}
		else
		{
			Logging.logError("CRemoteNotificationNativeInterface: Failed to get 'GoogleProjectID' from the Values resource");
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
