/**
 * INativeInterface.java
 * Chilli Source
 * Created by Ian Copland on 09/08/2012.
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

import android.content.Intent;
import android.content.res.Configuration;

import com.chilliworks.chillisource.core.IQueryableInterface;

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
	 * @author Ian Copland
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
	/**
	 * Triggered when the activity config changes such as orientation,
	 * hardware keyboards, etc.
	 * 
	 * @author S Downie
	 * 
	 * @param New config
	 */
    public void onActivityConfigurationChanged(Configuration in_config)
    {
    	
    }
}
