/*
 *  CFacebookNativeInterface.java
 *  moFlow Android
 *
 *  Created by Robert Henning on 01/05/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.social;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import android.app.Dialog;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;

import com.facebook.FacebookOperationCanceledException;
import com.facebook.Session;
import com.facebook.Session.NewPermissionsRequest;
import com.facebook.Session.StatusCallback;
import com.facebook.SessionDefaultAudience;
import com.facebook.SessionLoginBehavior;
import com.facebook.SessionState;
import com.facebook.Settings;
import com.facebook.internal.SessionTracker;
import com.facebook.internal.Utility;
import com.facebook.widget.WebDialog;
import com.facebook.FacebookException;
import com.taggames.CTagResourceHelper;
import com.chillisource.core.INativeInterface;
import com.chillisource.core.InterfaceIDType;
import com.chillisource.core.ResourceHelper;
import com.chillisource.core.IActivityResults;

public class FacebookNativeInterface extends INativeInterface implements IActivityResults
{
	private static final InterfaceIDType InterfaceID = new InterfaceIDType("CFacebookNativeInterface");
	
	public native void OnAuthenticationComplete(boolean inbSuccess);
	public native void OnReadAuthorisationComplete(boolean inbSuccess);
	public native void OnWriteAuthorisationComplete(boolean inbSuccess);
	public native void OnPostToFeedComplete(boolean inbSuccess);
	public native void OnPostRequestComplete(boolean inbSuccess);
	
	public FacebookNativeInterface()
	{
		CTagResourceHelper.SetPackageName(mActivity.getPackageName());
	}
	
	@Override
	public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return inInterfaceType == FacebookNativeInterface.InterfaceID;
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
	public void OnActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(Session.getActiveSession() != null)
		{
			Session.getActiveSession().onActivityResult(mActivity, requestCode, resultCode, data);
		}
	}
	//--------------------------------------------------------------
	/// Authenticate
	///
	/// Attempt to open an existing session or prompt the
	/// user to login if no session exists
	//--------------------------------------------------------------
	public void Authenticate(final String[] inastrReadPermissions)
	{       
		final Runnable task = new Runnable()
		{
			@Override 
			public void run() 
			{
				List<String> aPermissions = new ArrayList<String>(Arrays.asList(inastrReadPermissions));
				CreateNewSession(aPermissions);
		        
		        synchronized(this)
		        {
		        	notifyAll();
		        }
			}
		};
		
		// Run the task.
		mActivity.runOnUiThread(task);
		
		// Wait for the task to finish before pausing the rendering thread.
		try
		{
			synchronized(task)
			{
				task.wait();
			}
		}
		catch(Exception e)
		{
			Log.e("MoFlow", e.getMessage());
		}
	}
	//--------------------------------------------------------------------------------------
	/// Try Resume Existing Session
	//--------------------------------------------------------------------------------------
	private boolean TryResumeExistingSession()
	{
		return OpenSession(false, new ArrayList<String>());
	}
	//--------------------------------------------------------------------------------------
	/// Create New Session
	//--------------------------------------------------------------------------------------
	private void CreateNewSession(List<String> inaPermissions)
	{
		OpenSession(true, inaPermissions);
	}
	//--------------------------------------------------------------------------------------
	/// Open Session
	//--------------------------------------------------------------------------------------
	private boolean OpenSession(boolean inbShowLogin, final List<String> inaReadPermissions)
	{	
    	SessionTracker Tracker = new SessionTracker(mActivity, new Session.StatusCallback() 
	{
	    @Override
	    public void call(Session session, SessionState state, Exception exception) 
	    {
	        OnSessionStateChanged(session, state, exception);
	    }
	});
		
    	Session existingSession = Tracker.getOpenSession();
    	
	    if (existingSession == null || existingSession.getState().isClosed())
	    {
	    	String applicationId = Utility.getMetadataApplicationId(mActivity);
	    	
	    	Session.setActiveSession(null);
	        Session session = new Session.Builder(mActivity).setApplicationId(applicationId).build();
	        Session.setActiveSession(session);
	        existingSession = session;
	    }
	    
	    if(existingSession != null && !existingSession.isOpened())
		{
	        Session.OpenRequest openRequest = new Session.OpenRequest(mActivity);

	            openRequest.setDefaultAudience(SessionDefaultAudience.FRIENDS);
	            openRequest.setPermissions(inaReadPermissions);
	            if(inbShowLogin)
	            	openRequest.setLoginBehavior(SessionLoginBehavior.SSO_WITH_FALLBACK);
	            else
	            	openRequest.setLoginBehavior(SessionLoginBehavior.SSO_ONLY);
	            
	            Session.getActiveSession().openForRead(openRequest);
		}

		
		if(existingSession != null)
		{
			Session.setActiveSession(existingSession);
			OnSessionStateChanged(existingSession, existingSession.getState(), null);
		}
		
		return (existingSession != null);
    }
	//--------------------------------------------------------------------------------------
	/// Is Signed In
	///
	/// @return Whether there is an existing open session
	//--------------------------------------------------------------------------------------
	public boolean IsSignedIn()
	{
		Session session = Session.getActiveSession();
		
		if(session == null)
		{
			return false;
		}
		
		return session.isOpened();
	}
	//--------------------------------------------------------------------------------------
	/// Get Active Token
	///
	/// @return The access token for the active session
	//--------------------------------------------------------------------------------------
	public String GetActiveToken()
	{
		Session session = Session.getActiveSession();
		
		if(session == null)
		{
			return "";
		}
		
		return session.getAccessToken();
	}
	//--------------------------------------------------------------------------------------
	/// On Session State Changed
	///
	/// @param Active session
	/// @param State of active session
	/// @param Exception
	//--------------------------------------------------------------------------------------
    private void OnSessionStateChanged(Session session, SessionState state, Exception exception)
    {
		if(exception != null)
		{
			Log.e("Facebook", exception.getMessage());
		}
		
		Session.setActiveSession(session);
		
    	if(state.isOpened())
    	{
    		OnAuthenticationComplete(true);
    	}
    	else if(state.isClosed())
    	{
    		session.closeAndClearTokenInformation();
    		OnAuthenticationComplete(false);
    	}
    }
	//--------------------------------------------------------------------------------------
	/// Has Permission
	///
	/// @param Permission ID
    /// @return Whether the current session user has given the permission
	//--------------------------------------------------------------------------------------
    public boolean HasPermission(final String instrPermission)
    {
    	if(IsSignedIn())
    	{
    		List<String> permissionList = Session.getActiveSession().getPermissions();
    		return permissionList.contains(instrPermission);
    	}
    	
    	return false;
    }
	//--------------------------------------------------------------------------------------
	/// Authorise Read Permissions
	///
    /// Prompt Facebook to ask the users to grant the given permissions.
    /// Calls into native with the result
    ///
	/// @param List of read permissions IDs
	//--------------------------------------------------------------------------------------
    public void AuthoriseReadPermissions(String[] inaReadPerms)
    {
    	if(!IsSignedIn())
    	{
    		return;
    	}
    	
    	final String[] aReadPerms = inaReadPerms;
    	
		final Runnable task = new Runnable()
		{
			@Override 
			public void run() 
			{
		    	NewPermissionsRequest request = new NewPermissionsRequest(mActivity, Arrays.asList(aReadPerms));
		    	request.setCallback(new StatusCallback()
		    	{
		    	    @Override
		    	    public void call(Session session, SessionState state, Exception exception) 
		    	    {
		    	        if(exception == null)
		    	        {
		    	        	OnReadAuthorisationComplete(true);
		    	        }
		    	        else
		    	        {
		    	        	OnReadAuthorisationComplete(false);
		    	        }
		    	    }
		    	});
		        Session.getActiveSession().requestNewReadPermissions(request);
			}
		};
		
		// Run the task.
		mActivity.runOnUiThread(task);
		
		// Wait for the task to finish before pausing the rendering thread.
		try
		{
			task.wait();
		}
		catch(Exception e)
		{
			
		}
    }
	//--------------------------------------------------------------------------------------
	/// Authorise Write Permissions
	///
    /// Prompt Facebook to ask the users to grant the given permissions.
    /// Calls into native with the result
    ///
	/// @param List of write permissions IDs
	//--------------------------------------------------------------------------------------
    public void AuthoriseWritePermissions(String[] inaWritePerms)
    {
    	if(!IsSignedIn())
    	{
    		return;
    	}
    	
    	final String[] aWritePerms = inaWritePerms;
    	
		final Runnable task = new Runnable()
		{
			@Override 
			public void run() 
			{
				NewPermissionsRequest request = new NewPermissionsRequest(mActivity, Arrays.asList(aWritePerms));
		    	request.setCallback(new StatusCallback()
		    	{
		    	    @Override
		    	    public void call(Session session, SessionState state, Exception exception) 
		    	    {
		    	        if(exception == null)
		    	        {
		    	        	OnWriteAuthorisationComplete(true);
		    	        }
		    	        else
		    	        {
		    	        	OnWriteAuthorisationComplete(false);
		    	        }
		    	    }
		    	});
		        Session.getActiveSession().requestNewPublishPermissions(request);
			}
		};
		
		// Run the task.
		mActivity.runOnUiThread(task);
		
		// Wait for the task to finish before pausing the rendering thread.
		try
		{
			task.wait();
		}
		catch(Exception e){}
    }
	//--------------------------------------------------------------------------------------
	/// Make Post To Feed Request
	///
    /// If a session is open then make the graph request
    ///
    /// @param The graph path
	//--------------------------------------------------------------------------------------
    public void MakePostToFeedRequest(String instrGraphPath, String[] inParamsKeyValue)
    {
    	if(!IsSignedIn())
    	{
    		return;
    	}
    	
    	Bundle postParams = new Bundle();
    	for(int i=0; i<inParamsKeyValue.length; i+=2)
    	{
    		postParams.putString(inParamsKeyValue[i], inParamsKeyValue[i+1]);
    	}
    	
    	ShowDialogWithoutNotificationBar("feed", postParams);
    }
    
	//--------------------------------------------------------------------------------------
	/// Make Request to User
	///
    /// If a session is open then make the graph request
    ///
    /// @param The graph path
	//--------------------------------------------------------------------------------------
    public void MakeRequestToUser(String[] inParamsKeyValue)
    {
    	Bundle postParams = new Bundle();
    	for(int i=0; i<inParamsKeyValue.length; i+=2)
    	{
    		postParams.putString(inParamsKeyValue[i], inParamsKeyValue[i+1]);
    	}
    	
    	ShowDialogWithoutNotificationBar("apprequests", postParams);
    }
    
    private void ShowDialogWithoutNotificationBar(final String action, final Bundle params)
    {
    	mActivity.runOnUiThread(new Runnable()
    	{
    		@Override
    		public void run()
    		{
		    	Dialog dialog = new WebDialog.Builder(mActivity, Session.getActiveSession(), action, params).setOnCompleteListener(new WebDialog.OnCompleteListener()
    		    {
	    		    @Override
	    		    public void onComplete(Bundle values, FacebookException error)
	    		    {
	    		        if (error != null && !(error instanceof FacebookOperationCanceledException))
	    		        {	
		    		    	mActivity.GetSurface().queueEvent(new Runnable()
		    		    	{
					    		@Override
					    		public void run()
					    		{
					    			if(action == "feed")
							    		OnPostToFeedComplete(false);
							    	else
							    		OnPostRequestComplete(false);
					    		}
					    	});
	    		        }
	    		        else
	    		        {
	        		    	mActivity.GetSurface().queueEvent(new Runnable()
	        		    	{
	        		    		@Override
	        		    		public void run()
	        		    		{
	        		    			if(action == "feed")
	        				    		OnPostToFeedComplete(true);
	        				    	else
	        				    		OnPostRequestComplete(true);
	        		    		}
	        		    	});
	    		        }
	    		    }
	    		}).build();

	    		Window dialog_window = dialog.getWindow();
	    		dialog_window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
	    		    WindowManager.LayoutParams.FLAG_FULLSCREEN);
	
	    		dialog.show();
    		}
    	});
    }
	//--------------------------------------------------------------------------------------
	/// Sign Out
	///
    /// If a session exists log the user out and destroy any cached tokens
	//--------------------------------------------------------------------------------------
    public void SignOut()
    {
    	if(IsSignedIn())
    	{
    		Session.getActiveSession().closeAndClearTokenInformation();
    	}
    }
    //--------------------------------------------------------------------------------------
	/// Publish Install
	///
    /// Inform FB ads that the application has launched and it can determine
    /// if it is a fresh install
	//--------------------------------------------------------------------------------------
    public void PublishInstall()
    {
    	Runnable task = new Runnable()
		{ 
			@SuppressWarnings("deprecation")
			@Override public void run() 
			{
		    	String strAppID = mActivity.getString(ResourceHelper.GetDynamicResourceIDForField(mActivity, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "app_id"));
				android.util.Log.d("FBNativeInterface", strAppID);
		    	Settings.publishInstallAsync(mActivity, strAppID);				
			}
		};
		mActivity.runOnUiThread(task);    	
    }
}