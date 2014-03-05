package com.chillisource.social;

import android.app.Activity;
import android.telephony.SmsManager;
import android.util.Log;

public class SMSCompositionNativeInterface 
{
	native public static void OnSMSClosed();
	
	
	private static Activity msActivity;
	private static SmsManager mSMSManager;
	
	//---------------------------------------------------------------------
	/// Setup
	///
	/// Initialises the email composition native interface
	///
	/// @param this applications main activity.
	//---------------------------------------------------------------------
	public static void Setup(Activity inActivity)
	{
		msActivity = inActivity;
		mSMSManager = SmsManager.getDefault();
	}
	
	//--------------------------------------------------------------------------------------
	/// Present
	///
	/// Presents the user with the email interface
	///
	/// @param vector of email addresses
	/// @param subject line
	/// @param content
	/// @param result callback delegate
	/// @param bool if to format as html
	//--------------------------------------------------------------------------------------
	public static void PresentSMS(String[] inastrRecipientNumbers, //Vector of email addresses
								String instrContents)
	{  
		Log.d("MoFlow","<<<<<<<<<<<<<<<<<<< PresentSMS 1 >>>>>>>>>>>>>>>>>>");
		for(int udwNumber =0 ; udwNumber < inastrRecipientNumbers.length; ++udwNumber)
		{
			Log.d("MoFlow","<<<<<<<<<<<<<<<<<<< PresentSMS 2 >>>>>>>>>>>>>>>>>>");
			mSMSManager.sendTextMessage(inastrRecipientNumbers[udwNumber], null, instrContents, null, null);
			Log.d("MoFlow","<<<<<<<<<<<<<<<<<<< PresentSMS 3 >>>>>>>>>>>>>>>>>>");
		}
		
	}
	//--------------------------------------------------------------------------------------
	/// Dismiss
	///
	/// Removes the email display
	///
	//--------------------------------------------------------------------------------------
	public static void Dismiss()
	{
		SMSCompositionNativeInterface.OnSMSClosed();
	}
	
}
