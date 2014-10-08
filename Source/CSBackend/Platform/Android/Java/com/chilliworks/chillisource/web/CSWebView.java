/**
 * CSWebView.java
 * Chilli Source
 * Created by Ian Copland on 16/09/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
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

package com.chilliworks.chillisource.web;

import com.chilliworks.chillisource.web.WebViewNativeInterface;

import android.content.Context;
import android.view.KeyEvent;
import android.view.ViewGroup;

//============================================================
/// Web View
///
/// The Chilli Source implementation of a Web View. This differs 
/// from a standard webview by allowing the back button to close 
/// ∂the webview.
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
