package com.chillisource.social;

import java.io.File;

import com.chillisource.core.CSApplication;
import com.chillisource.core.Logging;
import com.chillisource.core.InterfaceIDType;
import com.chillisource.core.INativeInterface;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.text.Html;

//=============================================================
/// Email Composition Native Interface
///
/// Native interface for sending email intents. This is capable
/// of sending emails to multiple recipients and sending 
/// multiple attachments.
//=============================================================
public class EmailCompositionNativeInterface extends INativeInterface
{
	//--------------------------------------------------------------
	/// Member Data
	//--------------------------------------------------------------
	public static InterfaceIDType InterfaceID = new InterfaceIDType("CEmailCompositionNativeInterface");
	private static final int EMAIL_RETURN_REQUEST = 0;
	private boolean mbActive = false;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public EmailCompositionNativeInterface()
	{
	}
	//--------------------------------------------------------------
	/// Is A
	///
	/// @return whether or not this implements the given interface.
	//--------------------------------------------------------------
	@Override public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return (inInterfaceType.Equals(InterfaceID));
	}
	//-----------------------------------------------
	/// Present
	///
	/// Presents the user with the email interface
	///
	/// @param vector of email addresses
	/// @param subject line
	/// @param content
	/// @param The filename of the attachement if there is one.
	/// @param bool if to format as html
	//-----------------------------------------------
	public void Present(String[] inastrRecipientAddresses, String instrSubject, String instrContents, String instrAttachmentFilename, boolean inbFormatAsHtml)
	{  
		if (mbActive == false)
		{
			mbActive = true;
			
			Intent emailIntent = new Intent(android.content.Intent.ACTION_SEND); 
			
			emailIntent.putExtra(android.content.Intent.EXTRA_EMAIL, inastrRecipientAddresses); 
			emailIntent.putExtra(android.content.Intent.EXTRA_SUBJECT, instrSubject);
			
			if(inbFormatAsHtml)
			{
				emailIntent.setType("text/html");
				emailIntent.putExtra(android.content.Intent.EXTRA_TEXT, Html.fromHtml(instrContents));
			}
			else
			{
				emailIntent.setType("text/plain");
				emailIntent.putExtra(android.content.Intent.EXTRA_TEXT, instrContents);
			}
			
			//add attachment if there is one
			if (instrAttachmentFilename.length() > 0)
			{
				try
				{
					Uri uri = Uri.fromFile(new File(instrAttachmentFilename));
					emailIntent.putExtra(android.content.Intent.EXTRA_STREAM, uri);
				}
				catch (Exception e)
				{
					Logging.logError("Failed to attach file '" + instrAttachmentFilename + "' to Send intent!");
				}
			}
			
			CSApplication.get().getActivity().startActivityForResult(emailIntent, EMAIL_RETURN_REQUEST);
		}
	}
	//------------------------------------------------------
	/// On Activity Result
	///
	/// Called from the activity upon receiving
	/// an intent result
	///
	/// @param Request Code
	/// @param Result Code
	/// @param Intent
	//------------------------------------------------------
	@Override public void onActivityResult(int indwRequestCode, int indwResultCode, Intent inData)
	{
		if (mbActive == true && indwRequestCode == EmailCompositionNativeInterface.EMAIL_RETURN_REQUEST)
		{
			mbActive = false;
			// We are hard coding the result code to OK because the email clients never return this code and
			// RESULT_CANCELED is chosen by default by the activity in the absence of actual code
			// We therefore can't detect if the email has ever been sent or cancelled. It is preferable to set it to sent in doubt
			OnEmailClosed(Activity.RESULT_OK); 
		}
	}
	//--------------------------------------------
	/// On Email Closed
	///
	/// Receives when email client has been closed
	//--------------------------------------------
	native public void OnEmailClosed(int indwResultCode);
}
