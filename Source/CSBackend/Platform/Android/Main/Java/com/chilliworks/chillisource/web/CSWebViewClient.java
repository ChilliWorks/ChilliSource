/**
 * CSWebViewClient.java
 * ChilliSource
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

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.webkit.WebView;

import com.chilliworks.chillisource.core.CSApplication;

public class CSWebViewClient extends android.webkit.WebViewClient
{
	final String kstrGooglePlayURLScheme = "market";
	final String kstrAmazonUrlScheme = "amzn";
	private String mAnchor;
	private CSWebView mWebView;

	/**
	 * Is Store Deep Link
	 *
	 * @author Steven Hendrie
	 *
	 * @param in_url -The URL string.
	 *
	 * @return Whether or not it is a store deep link.
	 */
	public boolean IsStoreDeepLink(final String in_url)
	{
		Uri uri = Uri.parse(in_url);
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
	/**
	 * This intercepts url clicks within the webview and
	 * ensures they are not opened in an external browser.
	 * It will also check if the link was a store deep link
	 * and instead  open the store.
	 *
	 * @author Steven Hendrie
	 *
	 * @param in_view - WebView
	 * @param in_url - The URL string.
	 *
	 * @return Whether or not we are handling the URL loading
	 */
	@Override public boolean shouldOverrideUrlLoading(WebView in_view, String in_url)
	{
		Activity activity = CSApplication.get().getActivity();
		if (activity != null && IsStoreDeepLink(in_url) == true)
		{
			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.setData(Uri.parse(in_url));
			activity.startActivity(intent);
		}
		else
		{
			CSWebView webView = (CSWebView) in_view;
			if(webView != null)
			{
				boolean handledExternally = WebViewNativeInterface.onLinkClicked(webView.getIndexID(), in_url);
				if(handledExternally == false)
				{
					in_view.loadUrl(in_url);
				}
			}
		}
		
	    return true;
	}
	/**
	 * Called when a web page is finished loading
	 *
	 * @author Steven Hendrie
	 *
	 * @param in_view - WebView
	 * @param in_url - The URL that finished loading.
	 */
	@Override
	public void onPageFinished(WebView in_view, String in_url)
	{
		WebViewNativeInterface.RemoveActivityIndicator();
		WebViewNativeInterface.AddDismissButton();
		
		if(mAnchor.length() > 0)
		{
			mWebView.loadUrl("javascript:window.location.href = '" + mAnchor + "';");
			mAnchor = "";
		}
	}
	/**
	 * Called when a web page is finished loading
	 *
	 * @author Steven Hendrie
	 *
	 * @param in_view - WebView
	 * @param in_errorCode - Error code reported
	 * @param in_description - String description of the error
	 * @param in_failingUrl - The URL that failed to load.
	 */
	@Override
	public void onReceivedError(WebView in_view, int in_errorCode, String in_description, String in_failingUrl)
	{
		WebViewNativeInterface.RemoveActivityIndicator();

		final CSWebView webView = (CSWebView) in_view;
		assert (webView != null) : "Should always be castable to CSWebView!";

		if(webView != null)
		{
			AlertDialog.Builder builder = new AlertDialog.Builder(webView.getContext());
			builder.setMessage(in_description)
					.setCancelable(false)
					.setPositiveButton("OK", new DialogInterface.OnClickListener()
					{
						public void onClick(DialogInterface dialog, int id)
						{
							WebViewNativeInterface.onWebviewDismissed(webView.getIndexID());
						}
					});

			AlertDialog alert = builder.create();
			alert.show();
		}
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
