/**
 * CSActivity.java
 * Chilli Source
 * Created by Ian Copland on 14/08/2012.
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

package com.chilliworks.chillisource.core;
import android.app.Activity;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;

import com.chilliworks.chillisource.core.CSPowerManager;
import com.chilliworks.chillisource.core.SharedPreferencesNativeInterface;
import com.chilliworks.chillisource.input.DeviceButtonNativeInterface;
import com.chilliworks.chillisource.input.DeviceButtonNativeInterface.DeviceButton;
import com.chilliworks.chillisource.networking.HttpRequestNativeInterface;
import com.chilliworks.chillisource.web.WebViewNativeInterface;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;

/**
 * The main activity for Chilli Source apps.
 * This feeds Android lifecycle events to the application
 *
 * @author Ian Copland
 */
public class CSActivity extends Activity 
{
	private static final int k_googlePlayServicesErrorDialogRequestCode = 7564568;
	private Surface m_surface;
	private AppConfig m_appConfig;
	
	/**
	 * Triggered when the activity is first created (i.e. on app launch).
	 * 
	 * @author Ian Copland
	 * 
	 * @param Saved instance state (Not used)
	 */
    @Override public void onCreate(Bundle savedInstanceState) 
    {	
        super.onCreate(savedInstanceState);
        try
        {
        	//go full screen!
        	requestWindowFeature(Window.FEATURE_NO_TITLE);
        	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        	m_appConfig = new AppConfig(this);
      
        	m_surface = new Surface(m_appConfig, this);
        	
        	CSApplication.create(this);
      
        	CSApplication.get().activityIntent(getIntent());
    		
          	//initialise the old style native interfaces (These should be removed over time as each of these is changed over to the new system!)
        	HttpRequestNativeInterface.Setup(this);
        	SharedPreferencesNativeInterface.Setup(this);
            WebViewNativeInterface.Setup(this);
            CSPowerManager.Setup(this);
        }
        catch (Exception e)
        {
        	Logging.logError("Activity.onCreate has thrown an exception: " + e.toString());
        	e.printStackTrace();
        }
    }
	/**
	 * Triggered when the activity becomes visible
	 * 
	 * @author Ian Copland
	 */
    @Override protected void onResume() 
    {
    	super.onResume();
    	
        m_surface.onResume();

        CSPowerManager.RequestWakeLock(CSPowerManager.LOCK_TYPE.SCREEN_DIM_LOCK);
        
        CSApplication.get().resume();
        
        if(m_appConfig.isGooglePlayServicesRequired() == true)
        {
    		//We require Google Play Services so we need to check if they require installing
    		int gpsAvailableResult = GooglePlayServicesUtil.isGooglePlayServicesAvailable(CSApplication.get().getAppContext());
    		
    		switch(gpsAvailableResult)
    		{
    		case ConnectionResult.SUCCESS:
    			break;
    		case ConnectionResult.SERVICE_MISSING:
    			//Kindle or unsupported device
    			break;
    		case ConnectionResult.SERVICE_DISABLED:
    		case ConnectionResult.SERVICE_VERSION_UPDATE_REQUIRED:
    			//Requires update
    			GooglePlayServicesUtil.getErrorDialog(gpsAvailableResult, this, k_googlePlayServicesErrorDialogRequestCode).show();
    			break;
    		}
        }
    }
	/**
	 * Triggered when the window focus changes (i.e. the activity is no
	 * longer the top of the view stack or becomes the top of the view stack).
	 * The order of this in relation to resume and stop is not defined
	 * 
	 * @author Ian Copland
	 * 
	 * @param Whether the window has focus or not
	 */
    @Override public void onWindowFocusChanged(boolean in_hasFocus)
    {
    	if(in_hasFocus == true)
    	{
    		super.onWindowFocusChanged(in_hasFocus);
    		CSApplication.get().foreground();
    	}
    	else
    	{
    		CSApplication.get().background();
    		super.onWindowFocusChanged(in_hasFocus);
    	}
    }
	/**
	 * Triggered when the activity is no longer wholly visible.
	 * 
	 * @author Ian Copland
	 */
    @Override public void onPause() 
    {
    	CSApplication.get().suspend();
    	
    	m_surface.onPause();
        CSPowerManager.ReleaseLock(CSPowerManager.LOCK_TYPE.SCREEN_DIM_LOCK);
       
        super.onPause();
    }
	/**
	 * Triggered when the activity is no longer visible.
	 * 
	 * @author Ian Copland
	 */
    @Override public void onStop()
    {
	    super.onStop();
    }
	/**
	 * Triggered when the activity is no active. This is the end of the app
	 * 
	 * @author Ian Copland
	 */
    @Override public void onDestroy()
    {
    	CSApplication.get().destroy();
	    super.onDestroy();
    }
	/**
	 * Triggered when the activity receives a new intent either on launch
	 * or by having it passed by another activity
	 * 
	 * @author Ian Copland
	 * 
	 * @param Intent
	 */
    @Override public void onNewIntent(Intent in_intent)
    {
    	setIntent(in_intent);
    	CSApplication.get().activityIntent(in_intent);
    	super.onNewIntent(in_intent);
    }
	/**
	 * Triggered when the activity exits.
	 * 
	 * @author Ian Copland
	 * 
	 * @param Request code that it started with
	 * @param Result code it returned
	 * @param Any additional data returned
	 */
    @Override protected void onActivityResult(int in_requestCode, int in_resultCode, Intent in_data) 
    {
    	if(in_requestCode != k_googlePlayServicesErrorDialogRequestCode)
    	{
    		CSApplication.get().activityResult(in_requestCode, in_resultCode, in_data);
    	}
    	else if(in_resultCode == RESULT_CANCELED)
    	{
    		//Terminate until downloaded
    		CSApplication.get().quit();
    	}
    	super.onActivityResult(in_requestCode, in_resultCode, in_data);
	}
	/**
	 * Triggered when the activity config changes such as orientation,
	 * hardware keyboards, etc.
	 * 
	 * @author Ian Copland
	 * 
	 * @param New config
	 */
    @Override public void onConfigurationChanged(Configuration in_config) 
    {    
    	CSApplication.get().activityConfigurationChanged(in_config);
    	super.onConfigurationChanged(in_config);
    }
	/**
	 * Triggered when the activity receives an event from the hard-key back button.
	 * This then forwards the event to the application
	 * 
	 * @author Ian Copland
	 */
    @Override public void onBackPressed() 
    {
    	DeviceButtonNativeInterface nativeInterface = (DeviceButtonNativeInterface)CSApplication.get().getSystem(DeviceButtonNativeInterface.InterfaceID);
    	nativeInterface.onTriggered(DeviceButton.k_backButton);
    }
	/**
	 * @author Ian Copland
	 * 
	 * @return Activity surface
	 */
    public Surface getSurface()
    {
    	return m_surface;
    }
}