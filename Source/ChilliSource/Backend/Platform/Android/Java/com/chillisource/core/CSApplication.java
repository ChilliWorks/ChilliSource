/**
 *  CSApplication.java
 *  ChilliSource
 *
 *  Created by Scott Downie on 12/03/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

import java.util.ArrayList;
import java.util.ListIterator;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
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
	private static CSApplication m_singleton = null;
	
	private ArrayList<INativeInterface> m_systems;
	private CSActivity m_activeActivity;
	private RelativeLayout m_rootViewContainer;
	private CoreNativeInterface m_coreSystem;
	private LoadingView m_loadingView = null;
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
	
	enum LifecycleState
	{
		k_none,
		k_init,
		k_resume,
		k_foreground,
		k_background,
		k_suspend,
		k_destroy
	}
	
	private LifecycleState m_currentAppLifecycleState = LifecycleState.k_none;
	
	/**
	 * Factory method for creating the application in native
	 * 
	 * @author S Downie
	 */
	public static native void createApplication();
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
	 * Triggered when the app is created
	 * 
	 * @author S Downie
	 */
	public void create(CSActivity in_activeActivity)
	{
		//Cannot create app with a null activity
		assert in_activeActivity != null;
		
		//Cannot init app more than once
		assert m_singleton == null;
		
		m_singleton = this;
		
		m_activeActivity = in_activeActivity;
		
		m_systems = new ArrayList<INativeInterface>();
		
    	//create the root view container that other Android UI is added to
		//and make it the root of the app activity
		m_rootViewContainer = new RelativeLayout(m_activeActivity);
		m_activeActivity.setContentView(m_rootViewContainer);
		m_rootViewContainer.addView(m_activeActivity.getSurface());
		m_loadingView = new LoadingView(m_activeActivity);
		
		//Present the "default" image
		m_loadingView.Present("com_chillisource_default");
		
		LoadSharedLibraries();
	}
	/**
	 * Triggered when the app is launched
	 * 
	 * @author S Downie
	 */
	public void init()
	{
		m_initLifecycleEventOccurred = true;
		m_currentAppLifecycleState = LifecycleState.k_none;
	}
	/**
	 * Triggered when the app is resumed after being invisible
	 * 
	 * @author S Downie
	 */
	public void resume()
	{
		//Present the "resuming" image
    	if(!m_loadingView.IsPresented())
    	{
    		m_loadingView.Present("com_chillisource_resume");
    	}
    	
		m_resetTimeSinceLastUpdate = true;
		m_resumeLifecycleEventOccurred = true;
		
		for (INativeInterface system : m_systems)
		{
			system.onActivityResume();
		}
	}
	/**
	 * Triggered when the app is resumed after being in the background
	 * 
	 * @author S Downie
	 */
	public void foreground()
	{
		m_isActive = true;
		m_foregroundLifecycleEventOccurred = true;
		
		for (INativeInterface system : m_systems)
		{
			system.onActivityForeground();
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
    	
    	ProcessAppLifecycleEvents();
    	
		//Make sure the core system is handled last
		m_coreSystem.update(deltaTime, m_elapsedAppTime);
		
		m_loadingView.Dismiss();
	}
	/**
	 * Application lifecycle events are deferred to the main thread and are
	 * processed and notified from this method
	 * 
	 * @author S Downie
	 */
	private void ProcessAppLifecycleEvents()
	{
		if(m_initLifecycleEventOccurred == true && m_currentAppLifecycleState == LifecycleState.k_none)
		{
			createApplication();
			CoreNativeInterface.create();
			m_coreSystem = (CoreNativeInterface)getSystem(CoreNativeInterface.InterfaceID);
			assert m_coreSystem != null;
			m_coreSystem.init();
			
			m_initLifecycleEventOccurred = false;
			m_currentAppLifecycleState = LifecycleState.k_init;
		}
		if(m_resumeLifecycleEventOccurred == true && m_currentAppLifecycleState == LifecycleState.k_init)
		{
			m_coreSystem.resume();
			m_resumeLifecycleEventOccurred = false;
			m_currentAppLifecycleState = LifecycleState.k_resume;
		}
		if(m_foregroundLifecycleEventOccurred == true && m_currentAppLifecycleState == LifecycleState.k_resume)
		{
			m_coreSystem.foreground();
			m_foregroundLifecycleEventOccurred = false;
			m_currentAppLifecycleState = LifecycleState.k_foreground;
		}
		if(m_receivedIntent != null && m_currentAppLifecycleState == LifecycleState.k_foreground)
		{
			for (INativeInterface system : m_systems)
			{
				system.onLaunchIntentReceived(m_receivedIntent);
			}
			
			m_receivedIntent = null;
		}
		if(m_backgroundLifecycleEventOccurred == true && m_currentAppLifecycleState == LifecycleState.k_foreground)
		{
			m_coreSystem.background();
			m_backgroundLifecycleEventOccurred = false;
			m_currentAppLifecycleState = LifecycleState.k_background;
		}
	}
	/**
	 * Triggered when the app is pushed back after being in the foreground
	 * 
	 * @author S Downie
	 */
	public void background()
	{
		for (ListIterator<INativeInterface> iterator = m_systems.listIterator(m_systems.size()); iterator.hasPrevious();) 
		{
			INativeInterface system = iterator.previous();
			system.onActivityBackground();
		}
		
		m_isActive = false;
		m_backgroundLifecycleEventOccurred = true;
	}
	/**
	 * Triggered when the app is no longer the active one
	 * 
	 * @author S Downie
	 */
	public void suspend()
	{		
		//create the task to be run on the rendering thread
		Runnable task = new Runnable()
		{
			@Override public void run() 
			{
				//Make sure the core system is handled first
				m_coreSystem.suspend();
				m_currentAppLifecycleState = LifecycleState.k_suspend;
				
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
			Log.e("ChilliSource", e.getMessage());
			e.printStackTrace();
		}
		
		for (ListIterator<INativeInterface> iterator = m_systems.listIterator(m_systems.size()); iterator.hasPrevious();) 
		{
			INativeInterface system = iterator.previous();
			system.onActivitySuspend();
		}
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
		m_currentAppLifecycleState = LifecycleState.k_destroy;
		
		for (ListIterator<INativeInterface> iterator = m_systems.listIterator(m_systems.size()); iterator.hasPrevious();) 
		{
			INativeInterface system = iterator.previous();
			system.onActivityDestroy();
		}
		
		m_rootViewContainer = null;
		m_activeActivity = null;
		m_loadingView = null;
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
		for (INativeInterface system : m_systems)
		{
			system.onActivityResult(in_requestCode, in_resultCode, in_data);
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
		m_systems.add(in_system);
	}
	/**
	 * @author S Downie
	 * 
	 * @param Interface ID of system to find 
	 * 
	 * @return The first system that implements the given interface ID.
	 */
	public INativeInterface getSystem(InterfaceIDType in_interfaceID)
	{
		for (INativeInterface system : m_systems)
		{
			if (system.IsA(in_interfaceID))
			{
				return system;
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
	public void setMaxFPS(int in_maxFPS)
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

		try
		{
			synchronized(task)
			{
				task.wait();
			}
		}
		catch(Exception e)
		{
		}
	}
	/**
	 * Load the shared Java libraries required by the engine
	 * and the application. Application libs are provided
	 * in the AdditionalSharedLibraries application meta-data
	 * 
	 * @author I Copland
	 * 
	 * @param Task
	 */
	private void LoadSharedLibraries()
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
			Log.e("ChilliSource", "Could not load additional libraries!");
		}
		
		//load the default libraries
		System.loadLibrary("fmodex");
		System.loadLibrary("fmodevent");
		System.loadLibrary("Application");
	}
}
