package com.taggames.moflow.nativeinterface;

import com.taggames.moflow.CMoFlowActivity;
import com.taggames.moflow.core.CInterfaceIDType;
import com.taggames.moflow.core.CMoFlowResourceHelper;
import com.taggames.moflow.googleplay.remotenotifications.GCMService;
import com.taggames.moflow.googleplay.remotenotifications.GCMRegistrar;

import android.app.PendingIntent;
import android.content.Intent;

public class CRemoteNotificationNativeInterface extends INativeInterface
{
	//--------------------------------------------------------------
	/// Static Member Data
	//--------------------------------------------------------------
	public static CInterfaceIDType InterfaceID = new CInterfaceIDType("CRemoteNotificationNativeInterface");
	//--------------------------------------------------------------
	/// Native methods
	//--------------------------------------------------------------
	native public void OnRemoteTokenReceived(String instrText);
	native public void OnNotificationReceived(String[] inastrKeys, String[] inastrValues);
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public CRemoteNotificationNativeInterface()
	{
	}
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	@Override public boolean IsA(CInterfaceIDType inInterfaceType)
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
		int keyStringID = CMoFlowResourceHelper.GetDynamicResourceIDForField(mActivity, CMoFlowResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "GoogleProjectID");
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
        GCMRegistrar.checkDevice(CMoFlowActivity.GetActivity());
        GCMRegistrar.checkManifest(CMoFlowActivity.GetActivity());
        GCMRegistrar.register(CMoFlowActivity.GetActivity(), strProjectID);
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
		unregIntent.putExtra("app", PendingIntent.getBroadcast(CMoFlowActivity.GetActivity(), 0, new Intent(), 0));
		CMoFlowActivity.GetActivity().startService(unregIntent);
	}
}
