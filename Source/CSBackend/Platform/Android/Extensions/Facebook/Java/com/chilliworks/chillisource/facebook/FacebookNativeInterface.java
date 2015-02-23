/**
 * FacebookNativeInterface.java
 * Chilli Source
 * Created by Robert Henning on 01/05/2012.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
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

package com.chilliworks.chillisource.facebook;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import android.app.Activity;
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
import com.facebook.widget.FacebookDialog;
import com.facebook.widget.FacebookDialog.PendingCall;
import com.facebook.widget.WebDialog;
import com.facebook.FacebookException;
import com.facebook.UiLifecycleHelper;
import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.ExceptionUtils;
import com.chilliworks.chillisource.core.INativeInterface;
import com.chilliworks.chillisource.core.InterfaceIDType;
import com.chilliworks.chillisource.core.Logging;

public class FacebookNativeInterface extends INativeInterface
{
	private static final InterfaceIDType InterfaceID = new InterfaceIDType("FacebookNativeInterface");
	
	private static final int RESULT_SUCCESS = 0;
	private static final int RESULT_CANCEL = 1;
	private static final int RESULT_FAILURE = 2;
	
	private static final String KEY_LINK = "link";
	private static final String KEY_PICTURE = "picture";
	private static final String KEY_NAME = "name";
	private static final String KEY_CAPTION = "caption";
	private static final String KEY_DESCRIPTION = "description";
	
	private UiLifecycleHelper m_uiHelper = null;
	
	public native void OnAuthenticationComplete(boolean inbSuccess);
	public native void OnReadAuthorisationComplete(boolean inbSuccess);
	public native void OnWriteAuthorisationComplete(boolean inbSuccess);
	public native void OnPostToFeedComplete(int in_result);
	public native void OnPostRequestComplete(int in_result);

	/**
	 * Constructor
	 *
	 * @author T Kane
	 */	
	public FacebookNativeInterface()
	{
		final Runnable task = new Runnable()
		{
			@Override public void run() 
			{
				Activity activity = CSApplication.get().getActivity();
				m_uiHelper = new UiLifecycleHelper(activity, null);
				m_uiHelper.onCreate(null);
			}
		};
		
		CSApplication.get().scheduleUIThreadTask(task);						
	}
	/**
	 * Is A
	 *
	 * @author T Kane 
	 * 
	 * @param inInterfaceType - Interface type to check against
	 *
	 * @return Whether or not this implements the given interface
	 */
	@Override public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return inInterfaceType == FacebookNativeInterface.InterfaceID;
	}
    /**
     * Post to Facebook
     *
     * @author T Kane
     * 
     * @param in_graphPath - Facebook graph path.
     * @param in_params - Additional graph parameters
     */
    public void Post(String in_graphPath, String[] in_params)
    {
    	final String graphPath = in_graphPath;
    	final String[] params = in_params;
    	
    	final Runnable task = new Runnable()
		{
			@Override public void run() 
			{
		    	if (FacebookDialog.canPresentShareDialog(CSApplication.get().getAppContext(), FacebookDialog.ShareDialogFeature.SHARE_DIALOG)) 
		    	{
		    		PostViaShareDialog(params);
		    	}
		    	else
		    	{
		        	// For now, just call the existing post to feed request
		        	MakePostToFeedRequest(graphPath, params);
		    	}
			}
		};
		CSApplication.get().scheduleUIThreadTask(task);						
    }
	/**
	 * Attempt to open an existing session or prompt the user to 
	 * login if no session exists.
	 *
	 * @author T Kane 
	 * 
	 * @param in_readPermissions - Read permissions for Facebook
	 */
	public void Authenticate(final String[] in_readPermissions)
	{       
		final Runnable task = new Runnable()
		{
			@Override public void run() 
			{
				if(TryResumeExistingSession() == false)
				{
					List<String> permissions = new ArrayList<String>(Arrays.asList(in_readPermissions));
					CreateNewSession(permissions);
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
    /**
     * Prompt Facebook to ask the user to grant the given permissions.
     *
     * @author T Kane
     * 
     * @param in_permissions - List of permissions.
     */
    public void AuthoriseReadPermissions(String[] in_permissions)
    {
    	if (!IsSignedIn())
    	{
    		return;
    	}
    	
    	final String[] readPermissions = in_permissions;
    	
		final Runnable task = new Runnable()
		{
			@Override public void run() 
			{
		    	NewPermissionsRequest request = new NewPermissionsRequest(CSApplication.get().getActivity(), Arrays.asList(readPermissions));
		    	request.setCallback(new StatusCallback()
		    	{
		    	    @Override public void call(Session in_session, SessionState in_state, Exception in_exception) 
		    	    {
		    	        if (in_exception == null)
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
    /**
     * Prompt Facebook to ask the user to grant the given write permissions
     *
     * @author T Kane 
     * 
     * @param in_permissions - List of permissions to request
     */
    public void AuthoriseWritePermissions(String[] in_permissions)
    {
    	if (!IsSignedIn())
    	{
    		return;
    	}
    	
    	final String[] writePermissions = in_permissions;
    	
		final Runnable task = new Runnable()
		{
			@Override public void run() 
			{
				NewPermissionsRequest request = new NewPermissionsRequest(CSApplication.get().getActivity(), Arrays.asList(writePermissions));
		    	request.setCallback(new StatusCallback()
		    	{
		    	    @Override public void call(Session in_session, SessionState in_state, Exception in_exception) 
		    	    {
		    	        if (in_exception == null)
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
    /**
     * Determine whether current Facebook session has given permission.
     *
     * @author T Kane
     * 
     * @param in_permission - Facebook permission to query against
     *
     * @return True if session has permission, otherwise false.
     */
    public boolean HasPermission(final String in_permission)
    {
    	if (IsSignedIn())
    	{
    		List<String> permissionList = Session.getActiveSession().getPermissions();
    		return permissionList.contains(in_permission);
    	}
    	
    	return false;
    }
    /**
     * If a session is open then make the graph request.
     *
     * @author T Kane
     * 
     * @param in_graphPath - Facebook graph path
     * @param in_params - Additional graph parameters
     */
    public void MakePostToFeedRequest(String in_graphPath, String[] in_params)
    {
    	Bundle postParams = new Bundle();
    	for (int i = 0; i < in_params.length; i+=2)
    	{
    		postParams.putString(in_params[i], in_params[i+1]);
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
	/**
	 * Determine if we are signed into Facebook.
	 *
	 * @author T Kane
	 * 
	 * @return True if signed in, otherwise false.
	 */
	public boolean IsSignedIn()
	{
		Session session = Session.getActiveSession();
		
		if(session == null)
		{
			return false;
		}
		
		return session.isOpened();
	}
	/**
	 * Get access token for the active Facebook session.
	 *
	 * @author T Kane
	 * 
	 * @return Access token for the active Facebook Session
	 */
	public String GetActiveToken()
	{
		Session session = Session.getActiveSession();
		
		if(session == null)
		{
			return "";
		}
		
		return session.getAccessToken();
	}
    /**
     * If a session exists, log the user out and destroy any cached tokens.
     *
     * @author T Kane 
     */
    public void SignOut()
    {
    	if (IsSignedIn())
    	{
    		Session.getActiveSession().closeAndClearTokenInformation();
    	}
    }
	/**
	 * Called when the activity is suspended.
	 *
	 * @author T Kane
	 * 
	 */
	@Override public void onActivitySuspend()
	{
		super.onActivitySuspend();

		m_uiHelper.onPause();
	}
	/**
	 * Called when the activity is resumed
	 *
	 * @author T Kane
	 */
	@Override public void onActivityResume()
	{
		super.onActivityResume();

		m_uiHelper.onResume();
	}
	/**
	 * Called when the activity is destroyed
	 *
	 * @author T Kane
	 */
	@Override public void onActivityDestroy()
	{
		super.onActivityDestroy();

		m_uiHelper.onDestroy();
	}
	/**
	 * Called from the activity upon receiving an intent result.
	 *
	 * @author T Kane
	 * 
	 * @param in_requestCode - Request code
	 * @param in_resultCode - Result code
	 * @param in_data - Accompanying data
	 */
	@Override public void onActivityResult(int in_requestCode, int in_resultCode, Intent in_data)
	{
		if (Session.getActiveSession() != null)
		{
			Session.getActiveSession().onActivityResult(CSApplication.get().getActivity(), in_requestCode, in_resultCode, in_data);
		}
		
		m_uiHelper.onActivityResult(in_requestCode, in_resultCode, in_data, new FacebookDialog.Callback()
		{
			@Override public void onComplete(PendingCall pendingCall, Bundle data) 
			{
				int result = RESULT_FAILURE;
				String gesture = FacebookDialog.getNativeDialogCompletionGesture(data);
				if (gesture != null)
				{
					if ("post".equals(gesture))
					{
						result = RESULT_SUCCESS;
					}
					else if ("cancel".equals(gesture))
					{
						result = RESULT_CANCEL;
					}
				}
				final int finalResult = result;
				
				// Handle post on complete
	    		OnPostToFeedComplete(finalResult);
			}

			@Override public void onError(PendingCall pendingCall, Exception error, Bundle data) 
			{
				// Handle post on error
	        	final int result = RESULT_FAILURE;
	    		OnPostToFeedComplete(result);
			}
		});
	}
	/**
	 * Attempt to resume an existing Facebook session.
	 *
	 * @author T Kane 
	 * 
	 * @return true if existing session resumed, otherwise false.
	 */
	private boolean TryResumeExistingSession()
	{
		Session session = Session.getActiveSession();
		if ((session == null) || (session != null && session.getState() != SessionState.CREATED_TOKEN_LOADED))
		{
			return false;
		}
		
		return OpenSession(false, new ArrayList<String>());
	}
	/**
	 * Create a new Facebook session.
	 *
	 * @author T Kane
	 * 
	 * @param in_permissions - List of permissions.
	 */
	private void CreateNewSession(List<String> in_permissions)
	{
		OpenSession(true, in_permissions);
	}
	/**
	 * Open a Facebook session.
	 *
	 * @author T Kane
	 * 
	 * @param in_showLogin - Whether or not to show login screen
	 * @param in_readPermissions - Facebook session read permissions.
	 *
	 * @return True if session opened, otherwise false.
	 */
	private boolean OpenSession(boolean in_showLogin, final List<String> in_readPermissions)
	{	
    	SessionTracker Tracker = new SessionTracker(CSApplication.get().getActivityContext(), new Session.StatusCallback() 
		{
		    @Override public void call(Session session, SessionState state, Exception exception) 
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
	    
	    if (existingSession != null && !existingSession.isOpened())
		{
	        Session.OpenRequest openRequest = new Session.OpenRequest(CSApplication.get().getActivity());

            openRequest.setDefaultAudience(SessionDefaultAudience.FRIENDS);
            openRequest.setPermissions(in_readPermissions);
            if (in_showLogin)
            {
            	openRequest.setLoginBehavior(SessionLoginBehavior.SSO_WITH_FALLBACK);
            }
            else
            {
            	openRequest.setLoginBehavior(SessionLoginBehavior.SSO_ONLY);
            }
            
            Session.getActiveSession().openForRead(openRequest);
		}

		
		if (existingSession != null)
		{
			Session.setActiveSession(existingSession);
			OnSessionStateChanged(existingSession, existingSession.getState(), null);
		}
		
		return (existingSession != null && existingSession.getState() != SessionState.CREATED_TOKEN_LOADED);
    }
	/**
	 * Delegate called by Facebook SDK whenever session state changes.
	 *
	 * @author T Kane
	 * 
	 * @param in_session - Facebook session
	 * @param in_state - Session state
	 * @param in_exception - Any exception
	 */
    private void OnSessionStateChanged(Session in_session, SessionState in_state, Exception in_exception)
    {
		if (in_exception != null)
		{
			Logging.logError(in_exception.getMessage());
		}
		
		Session.setActiveSession(in_session);
		
    	if (in_state.isOpened())
    	{
    		OnAuthenticationComplete(true);
    	}
    	else if (in_state.isClosed())
    	{
    		in_session.closeAndClearTokenInformation();
    		OnAuthenticationComplete(false);
    	}
    }
    /**
     * Endeavour to use Facebook Share Dialog to share, if it's available.
     *
     * @author T Kane
     * 
     * @param in_params - Params to pass to share
     */
    private void PostViaShareDialog(String[] in_params)
    {
    	// Construct parameter 
    	Bundle params = new Bundle();
    	for (int i = 0; i < in_params.length; i += 2)
    	{
    		params.putString(in_params[i], in_params[i+1]);
    	}

    	FacebookDialog shareDialog = new FacebookDialog.ShareDialogBuilder(CSApplication.get().getActivity())
    		.setLink(params.getString(KEY_LINK))
    		.setPicture(params.getString(KEY_PICTURE))
    		.setName(params.getString(KEY_NAME))
    		.setCaption(params.getString(KEY_CAPTION))
    		.setDescription(params.getString(KEY_DESCRIPTION))
    		.build();
    	
    	// Track the dialog calls
    	m_uiHelper.trackPendingDialogCall(shareDialog.present());
    }
    /**
     * Show Dialog without notification bar. Actually calls the Web Dialog.
     *
     * @author T Kane 
     * 
     * @param in_action - Action
     * @param in_params - Additional paramters.
     */
    private void ShowDialogWithoutNotificationBar(final String in_action, final Bundle in_params)
    {
    	CSApplication.get().scheduleUIThreadTask(new Runnable()
    	{
    		@Override public void run()
    		{
		    	Dialog dialog = new WebDialog.Builder(CSApplication.get().getActivityContext(), Session.getActiveSession(), in_action, in_params).setOnCompleteListener(new WebDialog.OnCompleteListener()
    		    {
	    		    @Override public void onComplete(Bundle values, FacebookException error)
	    		    {
	    		        if (error != null)
	    		        {	
	    		        	final int result = ((error instanceof FacebookOperationCanceledException) == true) ? RESULT_CANCEL : RESULT_FAILURE;
			    			if (in_action == "feed")
			    			{
					    		OnPostToFeedComplete(result);
			    			}
					    	else
					    	{
					    		OnPostRequestComplete(result);
					    	}
	    		        }
	    		        else
	    		        {
	    		        	String postID = values.getString("post_id");
	    		        	final int result = (postID != null) ? RESULT_SUCCESS : RESULT_CANCEL;
    		    			if (in_action == "feed")
    		    			{
    				    		OnPostToFeedComplete(result);
    		    			}
    				    	else
    				    	{
    				    		OnPostRequestComplete(result);
    				    	}
	    		        }
	    		    }
	    		}).build();

	    		Window dialogWindow = dialog.getWindow();
	    		dialogWindow.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
	    		dialog.show();
    		}
    	});
    }
}