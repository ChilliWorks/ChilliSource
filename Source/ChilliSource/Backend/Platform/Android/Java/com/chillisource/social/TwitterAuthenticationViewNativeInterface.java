/*
 *  CTwitterAuthenticationViewNativeInterface.java
 *  Android Twitter Authentication View Native Interface
 *
 *  Created by Robert Henning on 11/05/12.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.social;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Color;
import android.text.InputType;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.TextView;
import android.widget.TextView.OnEditorActionListener;

import com.chillisource.core.Screen;

public class TwitterAuthenticationViewNativeInterface
{
	native public static void OnPINComplete(String instrPIN);
	
	public static final int	TWITTER_AUTHENTICATION_PIN_LENGTH = 7;
	
	private static Activity msActivity;
	private static RelativeLayout mLayout;
	
	
	// Debug
	private static final boolean ENABLE_PRINTS = false;
	
	//---------------------------------------------------------------------
	/// Initialise
	///
	/// Initialises the webview native interface
	///
	/// @param this applications main activity.
	//---------------------------------------------------------------------
	public static void Setup(Activity inActivity)
	{
		msActivity = inActivity;
	}
	
	public static void TwitterAuthViewPresent()
	{
		Runnable task = new Runnable() 
        {
		    public void run() 
		    {
		    	mLayout = CreateLayout();
		    	msActivity.addContentView(mLayout, new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.FILL_PARENT, RelativeLayout.LayoutParams.FILL_PARENT));
		    }
		};
		msActivity.runOnUiThread(task);
		
		try
		{
			task.wait();
		}
		catch(Exception e){}
	}
	
	public static void TwitterAuthViewPresentURL(String instrURL)
	{
		if(mWebview != null)
		{
			DEBUG_LOG("TwitterAuthViewPresentURL()", "Loading URL:\""+instrURL+"\"");
			mWebview.loadUrl(instrURL);
		}
	}
	
	public static void TwitterAuthViewDismiss()
	{
		if(mLayout != null)
		{
			mLayout.removeAllViews();
			mPINEntry = null;
			mWebview = null;
			ViewGroup vg = (ViewGroup)(mLayout.getParent());
			vg.removeView(mLayout);
		}
		mLayout = null;
	}
	
	public static boolean ValidatePIN(String instrText)
	{
		return (TWITTER_AUTHENTICATION_PIN_LENGTH == instrText.length());
	}
	
	public static void SetPIN(final String instrText)
	{
		mstrPIN = instrText;
		DEBUG_LOG("SetPIN", "Set PIN to:"+mstrPIN);
	}
	
	public static void OnPINError()
	{
		// As some point we may want to localise this. As Android's R.string system
		// is silly we can't use it as we would need to edit moFlow for each project
		// so instead I suggest passing in strings during init. which are added to
		// a list or array and then used here and in other relevant places below.
		AlertDialog ErrorAlert = new AlertDialog.Builder(msActivity).create();
		ErrorAlert.setTitle("Wrong PIN");
		ErrorAlert.setMessage("Oops! The PIN you entered is not correct. Please try again.");
		ErrorAlert.setButton(msActivity.getString(android.R.string.ok), new DialogInterface.OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				// You could do something here...
			}
		});
		ErrorAlert.show();
	}
	
	public static void OnPINCancelled()
	{
		AlertDialog CancelAlert = new AlertDialog.Builder(msActivity).create();
		CancelAlert.setTitle("Cancel Twitter Sign-In");
		CancelAlert.setMessage("Are you sure you want to cancel signing into Twitter?");
		CancelAlert.setButton(msActivity.getString(android.R.string.yes), new DialogInterface.OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				TwitterAuthViewDismiss();
			}
		});
		CancelAlert.setButton2(msActivity.getString(android.R.string.no), new DialogInterface.OnClickListener()
		{
			@Override
			public void onClick(DialogInterface dialog, int which)
			{
				// You could do something here...
			}
		});
		CancelAlert.show();
	}
	
	public static void OnPINComplete()
	{
		if((mPINEntry != null) && (ValidatePIN(mPINEntry.getText().toString())))
		{
			SetPIN(mPINEntry.getText().toString());
			// Dismiss keyboard
			// This does not work as expected
//			InputMethodManager in = (InputMethodManager)msActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
//			if(in.isActive())
//			{
//				in.toggleSoftInput(0, InputMethodManager.HIDE_IMPLICIT_ONLY | InputMethodManager.HIDE_NOT_ALWAYS);
//				DEBUG_LOG("OnPINComplete", "Keyboard active so dismissing keyboard");
//			}
//			else
//				DEBUG_LOG("OnPINComplete", "No keyboard active so skipping dismissing keyboard");
            // Go...
            OnPINComplete(mstrPIN);
            TwitterAuthViewDismiss();
		}
		else
			OnPINError();
	}
	
	public static RelativeLayout CreateLayout()
	{
		LayoutParams Params = null;
		int dwPaddingPixelsTop = Screen.GetPixelsFromDips(5.0f);
		int dwId = 1; // Must start a 1 and NOT 0.
		
		RelativeLayout ResultLayout = new RelativeLayout(msActivity);
		
		ResultLayout.setBackgroundColor(Color.BLACK);
		ResultLayout.setLayoutParams(new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.FILL_PARENT, RelativeLayout.LayoutParams.FILL_PARENT));
		ResultLayout.setPadding(0, dwPaddingPixelsTop, 0, 0);
		
		// Enter PIN: label
		TextView PINLabel = new TextView(msActivity);
		PINLabel.setLayoutParams(new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT));
		PINLabel.setText(new String("Enter PIN:"));
		PINLabel.setId(dwId++);
		ResultLayout.addView(PINLabel);
		
		// PIN text entry
		Params = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		Params.addRule(RelativeLayout.ALIGN_PARENT_TOP);
		Params.addRule(RelativeLayout.RIGHT_OF, PINLabel.getId());
		mPINEntry = new EditText(msActivity);
		mPINEntry.setOnEditorActionListener(new OnEditorActionListener()
		{
			@Override
			public boolean onEditorAction(TextView v, int actionId, KeyEvent event)
			{
				if(actionId == EditorInfo.IME_NULL)
				{
					// Read what the user has entered
					if(TwitterAuthenticationViewNativeInterface.ValidatePIN(v.getText().toString()))
					{
						TwitterAuthenticationViewNativeInterface.SetPIN(v.getText().toString());
						// Dismiss keyboard
						InputMethodManager in = (InputMethodManager)msActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
		                in.toggleSoftInput(0, 0);
					}
					else
						TwitterAuthenticationViewNativeInterface.OnPINError();
				}
				return false;
			}
		});
		mPINEntry.setLayoutParams(Params);
		mPINEntry.setInputType(InputType.TYPE_CLASS_NUMBER);
		mPINEntry.setEms(TwitterAuthenticationViewNativeInterface.TWITTER_AUTHENTICATION_PIN_LENGTH);
		mPINEntry.setId(dwId++);
		ResultLayout.addView(mPINEntry);
		Params = null;
		
		// OK button
		Params = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		Params.addRule(RelativeLayout.ALIGN_PARENT_RIGHT);
		Params.addRule(RelativeLayout.ALIGN_TOP, mPINEntry.getId());
		Button OkButton = new Button(msActivity);
		OkButton.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				TwitterAuthenticationViewNativeInterface.OnPINComplete();
			}
		});
		OkButton.setLayoutParams(Params);
		OkButton.setText(android.R.string.ok);
		OkButton.setId(dwId++);
		ResultLayout.addView(OkButton);
		Params = null;
		
		// Cancel button
		Params = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		Params.addRule(RelativeLayout.LEFT_OF, OkButton.getId());
		Params.addRule(RelativeLayout.ALIGN_TOP, mPINEntry.getId());
		Button CancelButton = new Button(msActivity);
		CancelButton.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				TwitterAuthenticationViewNativeInterface.OnPINCancelled();
			}
		});
		CancelButton.setLayoutParams(Params);
		CancelButton.setText(android.R.string.cancel);
		CancelButton.setId(dwId++);
		ResultLayout.addView(CancelButton);
		Params = null;
		
		// and finally, our web view
		Params = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT, RelativeLayout.LayoutParams.WRAP_CONTENT);
		Params.addRule(RelativeLayout.BELOW, mPINEntry.getId());
		mWebview = new WebView(msActivity);
		// Tell Android that we will handle web requests ourself
		mWebview.setWebViewClient(new WebViewClient()
		{
			@Override 
			public boolean shouldOverrideUrlLoading(WebView view, String url)
			{ 
			    view.loadUrl(url); 
			    return true;
			}	
		});
		// Now we need to tell Android that when we touch
		// the form field to enter some text to be kind
		// and give us a keyboard (silly Android!)
		mWebview.requestFocus(View.FOCUS_DOWN);
		mWebview.setOnTouchListener(new View.OnTouchListener()
		{
	        @Override
	        public boolean onTouch(View v, MotionEvent event)
	        {
	            switch (event.getAction())
	            {
	                case MotionEvent.ACTION_DOWN:
	                case MotionEvent.ACTION_UP:
	                    if(!v.hasFocus())
	                    {
	                        v.requestFocus();
	                    }
	                    break;
	            }
	            return false;
	        }
	    });
		mWebview.getSettings().setJavaScriptEnabled(true);
		mWebview.setLayoutParams(Params);
		mWebview.setId(dwId++);
		ResultLayout.addView(mWebview);
		Params = null;
		
		return ResultLayout;
	}
	
	// Member variables
	private static EditText 		mPINEntry;
	private static WebView			mWebview;
	private static String			mstrPIN;
	
	// Debug
	private static void DEBUG_LOG(String instrMethodName, String instrMessage)
	{
		if(ENABLE_PRINTS)
			android.util.Log.d("CTwitterAuthenticationViewNativeInterface."+instrMethodName, instrMessage);
	}
}
