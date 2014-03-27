package com.chillisource.web;

import com.chillisource.core.CSApplication;
import com.chillisource.web.WebViewNativeInterface;
import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.webkit.WebView;

public class CSWebViewClient extends android.webkit.WebViewClient
{
	final String kstrGooglePlayURLScheme = "market";
	final String kstrAmazonUrlScheme = "amzn";
	private String mAnchor;
	private WebView mWebView;
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
	
	public void SetView(WebView inWebView)
	{
		mWebView = inWebView;
	}
}
