package com.taggames.moflow.view.webview;

import com.taggames.moflow.nativeinterface.CWebViewNativeInterface;

import android.webkit.WebView;
import android.webkit.WebViewClient;

public class CWebViewClient extends WebViewClient
{
	private String mAnchor;
	private WebView mWebView;
	
	@Override 
	//show the web page in webview but not in web browser
	public boolean shouldOverrideUrlLoading(WebView view, String url)
	{ 
	    view.loadUrl(url); 
	    return true;
	}	
	
	@Override
	public void onPageFinished(WebView inView, String inURL)
	{
		CWebViewNativeInterface.RemoveActivityIndicator();
		
		if(mAnchor.length() > 0)
		{
			mWebView.loadUrl("javascript:window.location.href = '" + mAnchor + "';");
			mAnchor = "";
		}
	}
	
	@Override
	public void onReceivedError(WebView view, int errorCode, String description, String failingUrl)
	{
		CWebViewNativeInterface.RemoveActivityIndicator();
		CWebViewNativeInterface.OnWebviewDismissed(0);
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
