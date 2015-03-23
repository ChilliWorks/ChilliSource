/**
 * EmailComposerNativeInterface.java
 * Chilli Source
 * Created by Steven Hendrie on 14/08/2012.
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

package com.chilliworks.chillisource.social;

import java.io.File;

import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.INativeInterface;
import com.chilliworks.chillisource.core.InterfaceIDType;
import com.chilliworks.chillisource.core.Logging;

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
public class EmailComposerNativeInterface extends INativeInterface
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
	public EmailComposerNativeInterface()
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
	public void Present(String[] inastrRecipientAddresses, String instrSubject, String instrContents, boolean inbFormatAsHtml, String instrAttachmentFilename)
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
		if (mbActive == true && indwRequestCode == EmailComposerNativeInterface.EMAIL_RETURN_REQUEST)
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
