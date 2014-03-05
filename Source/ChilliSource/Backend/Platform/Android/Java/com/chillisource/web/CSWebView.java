//
//  CMoFlowActivity.java
//  moFlow
//
//  Created by Ian Copland on 16/09/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

package com.chillisource.web;

import com.chillisource.web.WebViewNativeInterface;

import android.content.Context;
import android.view.KeyEvent;
import android.view.ViewGroup;

//============================================================
/// Web View
///
/// The moFlow implementation of a Web View. This differs from
/// a standard webview by allowing the back button to close
/// the webview.
//============================================================
public class CSWebView extends android.webkit.WebView
{
	private int mdwIndex;

	//------------------------------------------------------
	/// Constructor
	///
	/// @param The context used to create the Web View.
	/// @param The index of this webview.
	//------------------------------------------------------
	public CSWebView(Context inContext, int indwIndex) 
	{
		super(inContext);
		mdwIndex = indwIndex;
	}
	//------------------------------------------------------
	/// On Key Down
	///
	/// This is called whenever a key is pressed when this
	/// is the top view.
	///
	/// @param The code for the given key.
	/// @param The key event.
	/// @param Whether or not the key event should be consumed.
	//------------------------------------------------------
	@Override public boolean onKeyDown(int indwKeyCode, KeyEvent inKeyEvent) 
	{
		if (indwKeyCode == KeyEvent.KEYCODE_BACK)
		{
			ViewGroup viewGroup = (ViewGroup)(getParent());
			if (viewGroup != null)
			{
				viewGroup.removeView(this);
				WebViewNativeInterface.Dismiss(mdwIndex);
			}
			return true;
		}
		return super.onKeyDown(indwKeyCode, inKeyEvent);
	}
}
