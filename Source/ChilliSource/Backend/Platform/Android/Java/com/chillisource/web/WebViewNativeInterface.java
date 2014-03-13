package com.chillisource.web;

import java.util.HashMap;
import java.util.Map;

import android.annotation.SuppressLint;

import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import android.view.Gravity;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

import com.chillisource.core.CSActivity;
import com.chillisource.core.CSApplication;
import com.chillisource.core.ResourceHelper;
import com.chillisource.web.CSWebView;
import com.chillisource.web.CSWebViewClient;
import com.chillisource.web.WebViewCloseButton;

public class WebViewNativeInterface 
{
	native public static void OnWebviewDismissed(int inudwIndex);
	
	private static CSActivity msActivity;
	private static RelativeLayout msWebviewHolder;
	private static ProgressDialog mActivityIndicator;
	private static Map<Integer,WebViewCloseButton> mDismissButtonMap;
	
	//---------------------------------------------------------------------
	/// Initialise
	///
	/// Initialises the webview native interface
	///
	/// @param this applications main activity.
	/// @param the gl surface to attach the webview
	//---------------------------------------------------------------------
	@SuppressLint("UseSparseArrays")
	public static void Setup(CSActivity inActivity)
	{
		msActivity = inActivity;	
		mDismissButtonMap = new HashMap<Integer,WebViewCloseButton>();
		msWebviewHolder = new RelativeLayout(msActivity);	
		LayoutParams params = new RelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		msWebviewHolder.setLayoutParams(params);
		msWebviewHolder.setGravity(Gravity.CENTER);
		
		CSApplication.get().addView(msWebviewHolder);	
	}
	//---------------------------------------------------------------------
	/// Present
	///
	/// Load and add the webview to the screen
	///
	/// @param url string for the website to be displayed
	/// @param absolute value for width - determined in native 
	/// @param absolute value for height - determined in native
	//---------------------------------------------------------------------
	public static void Present(int inudwIndex, String instrURL, int inudwXAbsolute,int inudwYAbsolute)
	{
		final String strURL = instrURL;
		final int udwXAbsolute = inudwXAbsolute;
		final int udwYAbsolute = inudwYAbsolute;		
    	final int udwIndex = inudwIndex;
    	
    	CSApplication.get().scheduleUIThreadTask(new Runnable() 
        {
		    public void run() 
		    {		
		    	synchronized (msWebviewHolder) 
		    	{
		    		CSWebView sWebView = CreateBlankWebView(udwIndex, udwXAbsolute, udwYAbsolute, "");
		    		sWebView.loadUrl(strURL);
		    		msWebviewHolder.addView(sWebView);
		    		sWebView.requestFocus();
		    		AddActivityIndicator();
		    	}
		    }
		});		
	}
	//---------------------------------------------------------------------
	/// Present From File
	///
	/// Load the webview from file and display
	///
	/// @param html contents
	/// @param absolute value for width - determined in native 
	/// @param absolute value for height - determined in native
	/// @param base path
	//---------------------------------------------------------------------
	public static void PresentFromFile(int inudwIndex, String instrHTMLContents, int inudwXAbsolute, int inudwYAbsolute, String instrBasePath, String instrAnchor)
	{
		final String strHTMLContents = instrHTMLContents;
		final String strBasePath = instrBasePath;
		final String strAnchor = instrAnchor;
		final int udwXAbsolute = inudwXAbsolute;
		final int udwYAbsolute = inudwYAbsolute;
		final int udwIndex = inudwIndex;
		
		msActivity.runOnUiThread(new Runnable() 
        {
		    public void run() 
		    {
		    	synchronized (msWebviewHolder) 
		    	{
		    		CSWebView sWebView = CreateBlankWebView(udwIndex, udwXAbsolute, udwYAbsolute, strAnchor);
		    		sWebView.loadDataWithBaseURL(strBasePath, strHTMLContents, "text/html", "utf-8", null);
		    		msWebviewHolder.addView(sWebView);
		    		sWebView.requestFocus();
		    		AddActivityIndicator();
		    	}
		    }
		});			
	}
	//---------------------------------------------------------------------
	/// Present In External Browser
	///
	/// Load the webview from file and display
	///
	/// @param url string for the website to be displayed
	/// @param absolute value for width - determined in native 
	/// @param absolute value for height - determined in native
	//---------------------------------------------------------------------
	public static void PresentInExternalBrowser(String instrURL)
	{
		Intent i = new Intent(Intent.ACTION_VIEW);  
		i.setData(Uri.parse(instrURL));  
		msActivity.startActivity(i);  
	}
	//---------------------------------------------------------------------
	/// Dismiss
	///
	/// Remove the webview from the screen.
	///
	/// @param index of webview to be removed (current removed)
	//---------------------------------------------------------------------
	public static void Dismiss(final int inudwIndex)
	{
		final WebViewCloseButton dismissButton = mDismissButtonMap.get(inudwIndex);
		
		msActivity.runOnUiThread(new Runnable() 
        {
		    public void run() 
		    {
		    	synchronized (msWebviewHolder) 
		    	{
		    		if(dismissButton != null) msWebviewHolder.removeView(dismissButton);
		    		msWebviewHolder.removeView(msWebviewHolder.findViewById(inudwIndex));
		    	}
		    }
		});
		WebViewNativeInterface.OnWebviewDismissed(inudwIndex);
	}
	//---------------------------------------------------------------------
	/// Add Activity Indicator
	///
	//---------------------------------------------------------------------
	public static void AddActivityIndicator()
	{
		if(mActivityIndicator != null)
			return;
		
		mActivityIndicator = new ProgressDialog(msActivity);
		int LoadingTextID = ResourceHelper.GetDynamicResourceIDForField(msActivity, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "com_chillisource_webview_loading");
		if(LoadingTextID > 0)
		{
			String strMessage = msActivity.getString(LoadingTextID);
			mActivityIndicator.setMessage(strMessage);
		}
		mActivityIndicator.show();
	}
	//---------------------------------------------------------------------
	/// Remove Activity Indicator
	///
	//---------------------------------------------------------------------
	public static void RemoveActivityIndicator()
	{
		if(mActivityIndicator == null)
			return;
		
		mActivityIndicator.dismiss();
    	mActivityIndicator = null;
	}
	//---------------------------------------------------------------------
	/// Add Dismiss Button
	///
	/// Add a button to the webview so that it can be removed
	///
	/// @param index of webview to be removed
	//---------------------------------------------------------------------
	public static void AddDismissButton(int inudwIndex, float infSize)
	{
		final WebViewCloseButton dismissButton = new WebViewCloseButton(msActivity, inudwIndex, infSize);
		mDismissButtonMap.put(inudwIndex, dismissButton);	
		msActivity.runOnUiThread(new Runnable() 
        {
		    public void run() 
		    {	   
		    	synchronized (msWebviewHolder) 
		    	{
		    		msWebviewHolder.addView(dismissButton);	
		    	}
		    }
		});		
	}
	//---------------------------------------------------------------------
	/// Create blank WebView
	///
	/// Creates a blank webview to be added with all necessary 
	/// settings
	///
	/// @param index of webview to created
	/// @param width
	/// @param height
	/// @param Anchor
	/// @returns blank webview with settings
	//---------------------------------------------------------------------
	@SuppressLint("SetJavaScriptEnabled")
	public static CSWebView CreateBlankWebView(int inudwIndex, int inudwXAbsolute, int inudwYAbsolute, String inAnchor)
	{
		CSWebView webView = new CSWebView(msActivity, inudwIndex);
		webView.setId(inudwIndex);
    	
    	CSWebViewClient client = new CSWebViewClient();
    	client.SetAnchor(inAnchor);
    	client.SetView(webView);
    	
    	webView.setWebViewClient(client);
    	webView.getSettings().setJavaScriptEnabled(true);
    	
    	RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(inudwXAbsolute,inudwYAbsolute);
		params.addRule(RelativeLayout.CENTER_IN_PARENT);
		webView.setLayoutParams(params);
    	return webView;
	}
}
