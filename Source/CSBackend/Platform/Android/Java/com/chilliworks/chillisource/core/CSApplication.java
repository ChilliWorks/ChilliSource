/**
 * CSApplication.java
 * Chilli Source
 * Created by Scott Downie on 12/03/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
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

import java.util.ArrayList;
import java.util.ListIterator;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;
import android.widget.RelativeLayout;

/**
 * Android application that mimics the ChilliSource application.
 * This holds all the Android systems and funnels the Android lifecycle
 * events
 * 
 * @author S Downie
 */
public class CSApplication 
{
	enum LifecycleState
	{
		k_notInitialised,
		k_inactive,
		k_active,
		k_foreground
	}
	
	private static CSApplication m_singleton = null;
	
	private ArrayList<INativeInterface> m_systems;
	private CSActivity m_activeActivity;
	private RelativeLayout m_rootViewContainer;
	private CoreNativeInterface m_coreSystem;
	private long m_milliSecsPerUpdate = 33;
	private long m_previousUpdateTime = 0;
	private long m_elapsedAppTime = 0;
	private boolean m_resetTimeSinceLastUpdate = false;
	private boolean m_isActive = false;
	
	private boolean m_initLifecycleEventOccurred = false;
	private boolean m_resumeLifecycleEventOccurred = false;
	private boolean m_foregroundLifecycleEventOccurred = false;
	private boolean m_backgroundLifecycleEventOccurred = false;
	private Intent m_receivedIntent = null;
	
	private LifecycleState m_currentAppLifecycleState = LifecycleState.k_notInitialised;
	
	/**
	 * @author S Downie
	 * 
	 * @return Singleton instance of the application. This is not lazily created
	 * but is first created using by initialising the app
	 */
	public static CSApplication get()
	{
		return m_singleton;
	}
	/**
	 * @author S Downie
	 * 
	 * @return Whether the application has been initialised
	 * or has been scheduled to do so
	 */
	public boolean hasReceivedInit()
	{
		return 	m_initLifecycleEventOccurred || (m_currentAppLifecycleState != LifecycleState.k_notInitialised);
	}
	/**
	 * Triggered when the app is created
	 * 
	 * @author S Downie
	 * 
	 * @param Chilli source activity
	 */
	public static void create(CSActivity in_activeActivity)
	{
		//Cannot create app with a null activity
		assert in_activeActivity != null;
		
		//Cannot init app more than once
		assert m_singleton == null;
		
		m_singleton = new CSApplication(in_activeActivity);
	}
	/**
	 * Constructor. Private to enforce use of creation factory method
	 * 
	 * @author S Downie
	 * 
	 * @param Chilli source activity
	 */
	private CSApplication(CSActivity in_activeActivity)
	{
		m_activeActivity = in_activeActivity;
		
		m_systems = new ArrayList<INativeInterface>();
		
    	//create the root view container that other Android UI is added to
		//and make it the root of the app activity
		m_rootViewContainer = new RelativeLayout(m_activeActivity);
		m_activeActivity.setContentView(m_rootViewContainer);
		m_rootViewContainer.addView(m_activeActivity.getSurface());
		
		m_currentAppLifecycleState = LifecycleState.k_notInitialised;
		
		loadSharedLibraries();
	}
	/**
	 * Triggered when the app is launched
	 * 
	 * @author S Downie
	 */
	public void init()
	{
		m_initLifecycleEventOccurred = true;
	}
	/**
	 * Triggered when the app is resumed after being invisible
	 * 
	 * @author S Downie
	 */
	public void resume()
	{
		m_isActive = true;
		
		m_resetTimeSinceLastUpdate = true;
		m_resumeLifecycleEventOccurred = true;
		
		synchronized(m_systems)
		{
			for (INativeInterface system : m_systems)
			{
				system.onActivityResume();
			}
		}
	}
	/**
	 * Triggered when the app is resumed after being in the background
	 * 
	 * @author S Downie
	 */
	public void foreground()
	{
		m_foregroundLifecycleEventOccurred = true;
		
		synchronized(m_systems)
		{
			for (INativeInterface system : m_systems)
			{
				system.onActivityForeground();
			}
		}
	}
	/**
	 * Triggered every frame when the app is active
	 * 
	 * @author S Downie
	 */
	public void update()
	{
       	//Ensure we are rendering at the required frame rate. This is achieved by checking
    	//how long a frame has taken and sleeping the app until the intended frame time 
    	//has elapsed.
    	try
    	{
    		long currentTime = System.currentTimeMillis();
    		long elapsedTime = currentTime - m_previousUpdateTime;
    		if (elapsedTime < m_milliSecsPerUpdate)
    		{
    			Thread.sleep(m_milliSecsPerUpdate - elapsedTime);
    		}
    	}
    	catch(Exception e)
    	{
    		e.printStackTrace();
    	}
        
    	//if we are resuming, or this is the first frame then ensure the delta time will be 0.
      	if (m_resetTimeSinceLastUpdate  == true)
        {
      		m_previousUpdateTime = System.currentTimeMillis();
      		m_resetTimeSinceLastUpdate = false;
        }
    	
        //calculate delta time. Hopefully this will be kMillisecondsPerFrame in most 
    	//circumstances.
        long currentTime = System.currentTimeMillis();
    	float deltaTime = ((float)(currentTime - m_previousUpdateTime)) * 0.001f;
    	m_elapsedAppTime += deltaTime;
    	m_previousUpdateTime = currentTime;
    	
    	processAppLifecycleEvents(deltaTime, m_elapsedAppTime);
	}
	/**
	 * Application lifecycle events are deferred to the main thread and are
	 * processed and notified from this method
	 * 
	 * @author S Downie
	 */
	private void processAppLifecycleEvents(float in_timeSinceLastUpdate, long in_appElapsedTime)
	{
		if(m_initLifecycleEventOccurred == true && m_currentAppLifecycleState == LifecycleState.k_notInitialised)
		{
			CoreNativeInterface.create();
			m_coreSystem = (CoreNativeInterface)getSystem(CoreNativeInterface.InterfaceID);
			assert m_coreSystem != null;
			m_coreSystem.init();
			
			m_initLifecycleEventOccurred = false;
			m_currentAppLifecycleState = LifecycleState.k_inactive;
		}
		if(m_resumeLifecycleEventOccurred == true && m_currentAppLifecycleState == LifecycleState.k_inactive)
		{
			m_coreSystem.resume();
			m_resumeLifecycleEventOccurred = false;
			m_currentAppLifecycleState = LifecycleState.k_active;
		}
		if(m_foregroundLifecycleEventOccurred == true && m_currentAppLifecycleState == LifecycleState.k_active)
		{
			m_coreSystem.foreground();
			m_foregroundLifecycleEventOccurred = false;
			m_currentAppLifecycleState = LifecycleState.k_foreground;
		}
		if(m_receivedIntent != null && m_currentAppLifecycleState == LifecycleState.k_foreground)
		{
			synchronized(m_systems)
			{
				for (INativeInterface system : m_systems)
				{
					system.onLaunchIntentReceived(m_receivedIntent);
				}
			}
			
			m_receivedIntent = null;
		}
		
		m_coreSystem.update(in_timeSinceLastUpdate, in_appElapsedTime);
		
		if(m_backgroundLifecycleEventOccurred == true && m_currentAppLifecycleState == LifecycleState.k_foreground)
		{
			m_coreSystem.background();
			m_backgroundLifecycleEventOccurred = false;
			m_currentAppLifecycleState = LifecycleState.k_active;
		}
	}
	/**
	 * Triggered when the app is pushed back after being in the foreground
	 * 
	 * @author S Downie
	 */
	public void background()
	{
		if(m_currentAppLifecycleState == LifecycleState.k_foreground)
		{
			synchronized(m_systems)
			{
				for (ListIterator<INativeInterface> iterator = m_systems.listIterator(m_systems.size()); iterator.hasPrevious();) 
				{
					INativeInterface system = iterator.previous();
					system.onActivityBackground();
				}
			}

			m_backgroundLifecycleEventOccurred = true;
		}
	}
	/**
	 * Triggered when the app is no longer the active one
	 * 
	 * @author S Downie
	 */
	public void suspend()
	{	
		final boolean shouldBackground = (m_currentAppLifecycleState == LifecycleState.k_foreground);
		
		if(shouldBackground == true)
		{
			synchronized(m_systems)
			{
				for (ListIterator<INativeInterface> iterator = m_systems.listIterator(m_systems.size()); iterator.hasPrevious();) 
				{
					INativeInterface system = iterator.previous();
					system.onActivityBackground();
				}
			}
		}
		
		//create the task to be run on the rendering thread
		Runnable task = new Runnable()
		{
			@Override public void run() 
			{
				//Often suspend is triggered by Android before background. We need to ensure it is not!
				if(shouldBackground)
				{
					m_coreSystem.background();
					m_backgroundLifecycleEventOccurred = false;
					m_currentAppLifecycleState = LifecycleState.k_active;
				}
				
				m_coreSystem.suspend();
				m_currentAppLifecycleState = LifecycleState.k_inactive;
				
				synchronized(this)
				{
					notifyAll();
				}
			}
		};
		
		//run the task.
		m_activeActivity.getSurface().queueEvent(task);
		
		//wait for the task to finish before pausing the rendering thread.
		try
		{
			synchronized(task)
			{
				task.wait();
			}
		}
		catch (Exception e)
		{
			Logging.logError(e.getMessage());
			e.printStackTrace();
		}
		
		synchronized(m_systems)
		{
			for (ListIterator<INativeInterface> iterator = m_systems.listIterator(m_systems.size()); iterator.hasPrevious();) 
			{
				INativeInterface system = iterator.previous();
				system.onActivitySuspend();
			}
		}
		
		m_isActive = false;
	}
	/**
	 * Triggered when the app is terminated. This will destroy the singleton application
	 * 
	 * @author S Downie
	 */
	public void destroy()
	{
		//Make sure the core system is handled first
		m_coreSystem.destroy();
		m_currentAppLifecycleState = LifecycleState.k_notInitialised;
		
		synchronized(m_systems)
		{
			for (ListIterator<INativeInterface> iterator = m_systems.listIterator(m_systems.size()); iterator.hasPrevious();) 
			{
				INativeInterface system = iterator.previous();
				system.onActivityDestroy();
			}
		}
		
		m_rootViewContainer = null;
		m_activeActivity = null;
		m_systems = null;
		m_receivedIntent = null;
		m_singleton = null;
	}
	/**
	 * Triggered when the activity exits
	 * 
	 * @author S Downie
	 * 
	 * @param Request code that it started with
	 * @param Result code it returned
	 * @param Any additional data returned
	 */
	public void activityResult(int in_requestCode, int in_resultCode, Intent in_data)
	{
		synchronized(m_systems)
		{
			for (INativeInterface system : m_systems)
			{
				system.onActivityResult(in_requestCode, in_resultCode, in_data);
			}
		}
	}
	/**
	 * Triggered when the activity config changes such as orientation,
	 * hardware keyboards, etc.
	 * 
	 * @author S Downie
	 * 
	 * @param New config
	 */
    public void activityConfigurationChanged(Configuration in_config)
    {
		synchronized(m_systems)
		{
			for (INativeInterface system : m_systems)
			{
				system.onActivityConfigurationChanged(in_config);
			}
		}
    }
	/**
	 * Triggered when the activity receives an intent
	 * 
	 * @author S Downie
	 * 
	 * @param Intent
	 */
	public void activityIntent(Intent in_intent)
	{
		m_receivedIntent = in_intent;
	}
	/**
	 * @author S Downie
	 * 
	 * @param System 
	 */
	public void addSystem(INativeInterface in_system)
	{
		synchronized(m_systems)
		{
			m_systems.add(in_system);
		}
	}
	/**
	 * @author S Downie
	 * 
	 * @param Interface ID of system to find 
	 * 
	 * @return The first system that implements the given interface ID.
	 */
	public INativeInterface getSystem(InterfaceIDType in_interfaceId)
	{
		synchronized(m_systems)
		{
			for (INativeInterface system : m_systems)
			{
				if (system.IsA(in_interfaceId))
				{
					return system;
				}
			}
		}
		
		return null;
	}
	/**
	 * @author S Downie
	 * 
	 * @return Whether the application
	 */
	public boolean isActive()
	{
		return m_isActive;
	}
	/**
	 * @author S Downie
	 * 
	 * @return Android application context 
	 */
	public Context getAppContext()
	{
		return m_activeActivity.getApplicationContext();
	}
	/**
	 * We should look to ditch this function once we have established what 
	 * things should use appContext
	 * 
	 * @author S Downie
	 * 
	 * @return Android application context 
	 */
	public Context getActivityContext()
	{
		return m_activeActivity;
	}
	/**
	 * @author S Downie
	 * 
	 * @return Chilli source activity
	 */
	public Activity getActivity()
	{
		return m_activeActivity;
	}
	/**
	 * @author S Downie
	 * 
	 * param Android UI to add to app root view 
	 */
	public void addView(View in_view)
	{
		m_rootViewContainer.addView(in_view);
	}
	/**
	 * @author S Downie
	 * 
	 * param Android UI to remove from app root view 
	 */
	public void removeView(View in_view)
	{
		m_rootViewContainer.removeView(in_view);
	}
	/**
	 * @author S Downie
	 * 
	 * @param Max update frequency in seconds
	 */
	public void setPreferredFPS(int in_maxFPS)
	{
		m_milliSecsPerUpdate = (long) (1000.0f/(float)in_maxFPS);
	}
	/**
	 * Schedule a task to run on the main update thread
	 * 
	 * @author S Downie
	 * 
	 * @param Task
	 */
	public void scheduleMainThreadTask(Runnable in_task)
	{
		assert m_activeActivity != null;
		
		m_activeActivity.getSurface().queueEvent(in_task);
	}
	/**
	 * Schedule a task to run on the UI thread
	 * 
	 * @author S Downie
	 * 
	 * @param Task
	 */
	public void scheduleUIThreadTask(Runnable in_task)
	{
		assert m_activeActivity != null;
		
		m_activeActivity.runOnUiThread(in_task);
	}
	/**
	 * Terminate the application
	 * 
	 * @author S Downie
	 * 
	 * @param Task
	 */
	public void quit()
	{
		assert m_activeActivity != null;
		
		Runnable task = new Runnable()
		{
			@Override public void run()
			{
				//Stop the activity
				m_activeActivity.onPause();
				m_activeActivity.onStop();
				m_activeActivity.onDestroy();
				android.os.Process.killProcess(android.os.Process.myPid());
				
				notifyAll();
			}
		};

		scheduleUIThreadTask(task);
	}
	/**
	 * Load the shared Java libraries required by the engine
	 * and the application. Application libs are provided
	 * in the AdditionalSharedLibraries application meta-data
	 * 
	 * @author Ian Copland
	 * 
	 * @param Task
	 */
	private void loadSharedLibraries()
	{
		assert m_activeActivity != null;
		
		//load additional shared libraries
		try
		{
			Bundle bundle = m_activeActivity.getPackageManager().getApplicationInfo(m_activeActivity.getPackageName(), PackageManager.GET_META_DATA).metaData;
			String strAdditionalLibraries = bundle.getString("AdditionalSharedLibraries");
			if (strAdditionalLibraries != null)
			{
				String[] astrAdditionalLibraries = strAdditionalLibraries.split(" ");
				
				for (String strAdditionalLibrary : astrAdditionalLibraries)
				{
					System.loadLibrary(strAdditionalLibrary);
				}
			}
		}
		catch (Exception e)
		{
			Logging.logError("Could not load additional libraries!");
		}
		
		//load the default libraries
		System.loadLibrary("Application");
	}
}
