/**
 * CSWebViewClient.java
 * Chilli Source
 * Created by Steven Hendrie on 15/12/2011.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2011 Tag Games Limited
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

import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.web.WebViewNativeInterface;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.webkit.WebView;

public class CSWebViewClient extends android.webkit.WebViewClient
{
	final String kstrGooglePlayURLScheme = "market";
	final String kstrAmazonUrlScheme = "amzn";
	private String mAnchor;
	private CSWebView mWebView;
	//--------------------------------------------------------
	/// Is Store Deep Link
	///
	/// @param The URL string.
	/// @return Whether or not it is a store deep link.
	//--------------------------------------------------------
	public boolean IsStoreDeepLink(final String instrUrl)
	{
		Uri uri = Uri.parse(instrUrl);
		if (uri != null)
		{
			String strScheme = uri.getScheme();
			if (strScheme != null)
			{
				return (strScheme.equalsIgnoreCase(kstrGooglePlayURLScheme) == true || strScheme.equalsIgnoreCase(kstrAmazonUrlScheme) == true);
			}
		}
		return false;
	}
	//--------------------------------------------------------
	/// This intercepts url clicks within the webview and
	/// ensures they are not opened in an external browser. 
	/// It will also check if the link was a store deep link 
	/// and instead  open the store.
	///
	/// @author S Hendrie
	///
	/// @param The URL string.
	///
	/// @return Whether or not it is a store deep link.
	//--------------------------------------------------------
	@Override public boolean shouldOverrideUrlLoading(WebView inView, String instrUrl)
	{
		Activity activity = CSApplication.get().getActivity();
		if (activity != null && IsStoreDeepLink(instrUrl) == true)
		{
			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.setData(Uri.parse(instrUrl));
			activity.startActivity(intent);
		}
		else
		{
			inView.loadUrl(instrUrl); 
		}
		
	    return true;
	}	
	
	@Override
	public void onPageFinished(WebView inView, String inURL)
	{
		WebViewNativeInterface.RemoveActivityIndicator();
		WebViewNativeInterface.AddDismissButton();
		
		if(mAnchor.length() > 0)
		{
			mWebView.loadUrl("javascript:window.location.href = '" + mAnchor + "';");
			mAnchor = "";
		}
	}
	
	@Override
	public void onReceivedError(WebView view, int errorCode, String description, String failingUrl)
	{
		WebViewNativeInterface.RemoveActivityIndicator();
		WebViewNativeInterface.OnWebviewDismissed(0);
	}
	
	public void SetAnchor(String inAnchor)
	{
		mAnchor = inAnchor;
	}
	
	public void SetView(CSWebView inWebView)
	{
		mWebView = inWebView;
	}
}
