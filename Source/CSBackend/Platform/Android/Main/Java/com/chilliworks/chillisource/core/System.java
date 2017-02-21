/**
 * System.java
 * ChilliSource
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
 * the application such as onResume() and onSuspend()
 *
 * @author S Downie
 */
public abstract class System implements IQueryableInterface
{
    /**
     * Initialises the system, adding it to the application system list and calling appropriate
     * lifecycle events. This *must* be the first method called on a System. Remember to call
     * destroyApplication() once finished.
     *
     * This can be called from any thread.
     *
     * @author Ian Copland
     */
    public final void init()
    {
        CSApplication.get().addSystem(this);
    }
    /**
     * Cleans up the system, calling appropriate life cycle events, then removes it from the
     * application system list.
     *
     * This *must* be the last method called on the system.
     *
     * This can be called from any thread.
     *
     * @author Ian Copland
     */
    public final void destroy()
    {
        CSApplication.get().removeSystem(this);
    }
    /**
     * Called when the entity has been added to the application.
     *
     * This is always called on the main thread.
     *
     * @author Ian Copland
     */
    protected void onInit() {}
	/**
	 * Triggered when the activity is made visible after being suspended.
     *
     * This is always called on the UI thread.
	 * 
	 * @author S Downie
	 */
    protected void onResume() {}
	/**
	 * Triggered when the activity is brought to the foreground after being in the background.
     *
     * This is always called on the UI thread.
	 * 
	 * @author S Downie
	 */
    protected void onForeground() {}
	/**
	 * Triggered when the activity is sent to the background after being in the foreground.
     *
     * This is always called on the UI thread.
	 * 
	 * @author S Downie
	 */
    protected void onBackground() {}
	/**
	 * Triggered when the activity is closed and no longer active.
     *
     * This is always called on the UI thread.
	 * 
	 * @author S Downie
	 */
    protected void onSuspend() {}
	/**
	 * Called immediately before the System is removed from the Application.
     *
     * This is always called on the UI thread.
	 * 
	 * @author Ian Copland
	 */
    protected void onDestroy() {}
	/**
	 * Triggered when the activity receives a launch intent.
     *
     * This is always called on the UI thread.
	 * 
	 * @author S Downie
	 * 
	 * @param in_intent - Intent
	 */
    protected void onLaunchIntentReceived(Intent in_intent) {}
	/**
	 * Triggered when the activity exits.
     *
     * This is always called on the UI thread.
	 * 
	 * @author S Downie
	 * 
	 * @param in_requestCode - Request code that it started with
	 * @param in_resultCode - Result code it returned
	 * @param in_data - Any additional data returned
	 */
    protected void onActivityResult(int in_requestCode, int in_resultCode, Intent in_data) {}
	/**
	 * Triggered when the activity config changes such as orientation, hardware keyboards, etc..
     *
     * This is always called on the UI thread.
	 * 
	 * @author S Downie
	 * 
	 * @param in_config - New config
	 */
    protected void onActivityConfigurationChanged(Configuration in_config) {}
}
