/**
 *  CSActivity.java
 *  Chilli Source
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

import android.content.Intent;

import com.chillisource.core.IQueryableInterface;

/**
 * Base class for Java systems. Allows systems to receive lifecycle events from
 * the application
 *
 * @author S Downie
 */
public abstract class INativeInterface extends IQueryableInterface
{
	/**
	 * Constructor registers the system with the Application.
	 * TODO: Changes this to an external explicit registration method
	 * 
	 * @author I Copland
	 */
	public INativeInterface()
	{
		CSApplication.get().addSystem(this);
	}
	/**
	 * Triggered when the activity launches
	 * 
	 * @author S Downie
	 */
	public void onActivityCreate()
	{
	
	}
	/**
	 * Triggered when the activity is made visible
	 * after being suspended
	 * 
	 * @author S Downie
	 */
	public void onActivityResume()
	{
	
	}
	/**
	 * Triggered when the activity is brought
	 * to the foreground after being in the background
	 * 
	 * @author S Downie
	 */
	public void onActivityForeground()
	{
	
	}
	/**
	 * Triggered when the activity is sent
	 * to the background after being in the foreground
	 * 
	 * @author S Downie
	 */
	public void onActivityBackground()
	{

	}
	/**
	 * Triggered when the activity is closed and no
	 * longer active
	 * 
	 * @author S Downie
	 */
	public void onActivitySuspend()
	{

	}
	/**
	 * Triggered when the activity is terminated
	 * 
	 * @author S Downie
	 */
	public void onActivityDestroy()
	{

	}
	/**
	 * Triggered when the activity receives a launch intent
	 * 
	 * @author S Downie
	 * 
	 * @param Intent
	 */
	public void onLaunchIntentReceived(Intent in_intent)
	{
		
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
	public void onActivityResult(int in_requestCode, int in_resultCode, Intent in_data)
	{

	}
}
