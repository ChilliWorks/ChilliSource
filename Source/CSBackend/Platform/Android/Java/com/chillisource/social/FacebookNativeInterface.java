/*
 *  CFacebookNativeInterface.java
 *  moFlow
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
import android.view.Window;
import android.view.WindowManager;

import com.facebook.FacebookOperationCanceledException;
import com.facebook.Session;
import com.facebook.Session.NewPermissionsRequest;
import com.facebook.Session.StatusCallback;
import com.facebook.SessionDefaultAudience;
import com.facebook.SessionLoginBehavior;
import com.facebook.SessionState;
import com.facebook.internal.SessionTracker;
import com.facebook.internal.Utility;
import com.facebook.widget.WebDialog;
import com.facebook.FacebookException;
import com.taggames.CTagResourceHelper;
import com.chillisource.core.CSApplication;
import com.chillisource.core.ExceptionUtils;
import com.chillisource.core.Logging;
import com.chillisource.core.INativeInterface;
import com.chillisource.core.InterfaceIDType;

public class FacebookNativeInterface extends INativeInterface
{
	private static final InterfaceIDType InterfaceID = new InterfaceIDType("FacebookNativeInterface");
	
	private static final int k_resultSuccess = 0;
	private static final int k_resultCancel = 1;
	private static final int k_resultFailure = 2;
	
	public native void OnAuthenticationComplete(boolean inbSuccess);
	public native void OnReadAuthorisationComplete(boolean inbSuccess);
	public native void OnWriteAuthorisationComplete(boolean inbSuccess);
	public native void OnPostToFeedComplete(int in_result);
	public native void OnPostRequestComplete(int in_result);
	
	public FacebookNativeInterface()
	{
		CTagResourceHelper.SetPackageName(CSApplication.get().getActivityContext().getPackageName());
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
	@Override public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		if(Session.getActiveSession() != null)
		{
			Session.getActiveSession().onActivityResult(CSApplication.get().getActivity(), requestCode, resultCode, data);
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
				if(TryResumeExistingSession() == false)
				{
					List<String> aPermissions = new ArrayList<String>(Arrays.asList(inastrReadPermissions));
					CreateNewSession(aPermissions);
				}
		       
		        synchronized(this)
		        {
		        	notifyAll();
		        }
			}
		};
		
		try
		{
			synchronized(task)
			{
				// Run the task.
				CSApplication.get().scheduleUIThreadTask(task);
				
				task.wait();
			}
		}
		catch(Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
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
    	SessionTracker Tracker = new SessionTracker(CSApplication.get().getActivityContext(), new Session.StatusCallback() 
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
	    	String applicationId = Utility.getMetadataApplicationId(CSApplication.get().getActivityContext());
	    	
	    	Session.setActiveSession(null);
	        Session session = new Session.Builder(CSApplication.get().getActivityContext()).setApplicationId(applicationId).build();
	        Session.setActiveSession(session);
	        existingSession = session;
	    }
	    
	    if(existingSession != null && !existingSession.isOpened())
		{
	        Session.OpenRequest openRequest = new Session.OpenRequest(CSApplication.get().getActivity());

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
		
		return (existingSession != null && existingSession.getState() != SessionState.CREATED_TOKEN_LOADED);
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
			Logging.logError(exception.getMessage());
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
		    	NewPermissionsRequest request = new NewPermissionsRequest(CSApplication.get().getActivity(), Arrays.asList(aReadPerms));
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
		
		try
		{
			synchronized(task)
			{
				// Run the task.
				CSApplication.get().scheduleUIThreadTask(task);
				
				task.wait();
			}
		}
		catch(Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
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
				NewPermissionsRequest request = new NewPermissionsRequest(CSApplication.get().getActivity(), Arrays.asList(aWritePerms));
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
		

		
		// Wait for the task to finish before pausing the rendering thread.
		try
		{
			synchronized(task)
			{
				// Run the task.
				CSApplication.get().scheduleUIThreadTask(task);
				
				task.wait();
			}
		}
		catch(Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
		}
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
    	CSApplication.get().scheduleUIThreadTask(new Runnable()
    	{
    		@Override
    		public void run()
    		{
		    	Dialog dialog = new WebDialog.Builder(CSApplication.get().getActivityContext(), Session.getActiveSession(), action, params).setOnCompleteListener(new WebDialog.OnCompleteListener()
    		    {
	    		    @Override
	    		    public void onComplete(Bundle values, FacebookException error)
	    		    {
	    		        if (error != null)
	    		        {	
	    		        	final int result = ((error instanceof FacebookOperationCanceledException) == true) ? k_resultCancel : k_resultFailure;
	    		        	
	    		        	CSApplication.get().scheduleMainThreadTask(new Runnable()
		    		    	{
					    		@Override
					    		public void run()
					    		{
					    			if(action == "feed")
							    		OnPostToFeedComplete(result);
							    	else
							    		OnPostRequestComplete(result);
					    		}
					    	});
	    		        }
	    		        else
	    		        {
	    		        	String postID = values.getString("post_id");
	    		        	final int result = (postID != null) ? k_resultSuccess : k_resultCancel;
	    		        	
	    		        	CSApplication.get().scheduleMainThreadTask(new Runnable()
	        		    	{
	        		    		@Override
	        		    		public void run()
	        		    		{
	        		    			if(action == "feed")
	        				    		OnPostToFeedComplete(result);
	        				    	else
	        				    		OnPostRequestComplete(result);
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
}