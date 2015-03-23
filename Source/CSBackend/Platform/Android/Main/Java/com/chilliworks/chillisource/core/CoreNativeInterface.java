/**
 * CoreNativeInterface.java
 * Chilli Source
 * Created by Ian Copland on 09/08/2012.
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

import com.chilliworks.chillisource.core.FileUtils;
import com.chilliworks.chillisource.core.InterfaceIDType;

import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.util.DisplayMetrics;

/**
* Native interface for communicating core os features to native.
* Also responsible for funneling lifecycle events to application
* 
* @author Ian Copland
*/
public class CoreNativeInterface extends INativeInterface
{	
	//--------------------------------------------------------------
	/// Static Member Data
	//--------------------------------------------------------------
	public static InterfaceIDType InterfaceID = new InterfaceIDType("CoreNativeInterface");

	//--------------------------------------------------------------
	/// Member Data
	//--------------------------------------------------------------
	private PackageInfo m_packageInfo;
	//--------------------------------------------------------------
	/// Native methods
	//--------------------------------------------------------------
	/**
	 * Force the native interface to create this system
	 * 
	 * @author S Downie
	 */
	public static native void create();
	/**
	 * Triggered when the application is launched
	 * This doesn't override from INativeInterface in
	 * order to ensure that core is called after all other interfaces
	 * 
	 * @author S Downie
	 */
	public native void init();
	/**
	 * Triggered when the application is resumed after launch or suspension.
	 * This doesn't override from INativeInterface in
	 * order to ensure that core is called before all other interfaces
	 * 
	 * @author S Downie
	 */
	public native void resume();
	/**
	 * Triggered when the application is brought
	 * to the foreground after being in the background.
	 * This doesn't override from INativeInterface in
	 * order to ensure that core is called before all other interfaces
	 * 
	 * @author S Downie
	 */
	public native void foreground();
	/**
	 * Updates the native application
	 * 
	 * @author Ian Copland
	 * 
	 * @param Time in seconds since last update
	 * @param Total elapsed time of app
	 */
	public native void update(float in_timeSinceLastUpdate, long in_appElapsedTime);
	/**
	 * Triggered when the application is sent
	 * to the background after being in the foreground
	 * This doesn't override from INativeInterface in
	 * order to ensure that core is called after all other interfaces
	 * 
	 * @author S Downie
	 */
	public native void background();
	/**
	 * Triggered when the application is suspended
	 * This doesn't override from INativeInterface in
	 * order to ensure that core is called after all other interfaces
	 * 
	 * @author S Downie
	 */
	public native void suspend();
	/**
	 * Triggered when the application is terminated
	 * This doesn't override from INativeInterface in
	 * order to ensure that core is called after all other interfaces
	 * 
	 * @author S Downie
	 */
	public native void destroy();
	/**
	 * Triggered when the app is low on memory to tell native
	 * to clean itself up
	 * 
	 * @author Ian Copland
	 */
	public native void memoryWarning();
	/**
	 * Triggered when the screen resolution changes
	 * 
	 * @author Ian Copland
	 * 
	 * @param The new width.
	 * @param The new height.
	 */
	public native void onResolutionChanged(int in_width, int in_height);
	/**
	 * Constructor
	 * 
	 * @author Ian Copland
	 */
	public CoreNativeInterface() throws NameNotFoundException
	{
		m_packageInfo = CSApplication.get().getAppContext().getPackageManager().getPackageInfo(CSApplication.get().getActivityContext().getPackageName(), PackageManager.GET_ACTIVITIES);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param Java system interface ID
	 * 
	 * @return Whether the system implements the ID
	 */
	@Override public boolean IsA(InterfaceIDType in_interfaceType) 
	{
		return (in_interfaceType.Equals(InterfaceID));
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The activity.
	 */
	public Activity getActivity()
	{
		return CSApplication.get().getActivity();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Path to external storage
	 */
	public String getExternalStorageDirectory()
	{
		return FileUtils.getExternalStorageDirectory();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Application name as specified by the project
	 */
	public String getApplicationName()
	{
		if (m_packageInfo.applicationInfo != null)
		{
			return CSApplication.get().getActivityContext().getPackageManager().getApplicationLabel(m_packageInfo.applicationInfo).toString();
		}
		else
		{
			return "ApplicationInfoWrong";
		}
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Application version code as specified by the manifest
	 */
	public int getApplicationVersionCode()
	{
		try 
		{
			return CSApplication.get().getActivityContext().getPackageManager().getPackageInfo(CSApplication.get().getActivityContext().getPackageName(), 0).versionCode;
		} 
		catch (NameNotFoundException e) 
		{
			return 0;
		}
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Application version name as specified by the manifest
	 */
	public String getApplicationVersionName()
	{
		try 
		{
			return CSApplication.get().getActivityContext().getPackageManager().getPackageInfo(CSApplication.get().getActivityContext().getPackageName(), 0).versionName;
		} 
		catch (NameNotFoundException e) 
		{
			return "";
		}
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Application package name
	 */
	public String getPackageName()
	{
		return CSApplication.get().getActivityContext().getPackageName();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Path to APK directory
	 */
	public String getAPKDirectory()
	{
		return m_packageInfo.applicationInfo.sourceDir;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Screen width in pixels
	 */
	@SuppressWarnings("deprecation")
	public int getScreenWidth()
	{
		return CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getWidth();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Screen height in pixels
	 */
	@SuppressWarnings("deprecation")
	public int getScreenHeight()
	{
		return CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getHeight();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Density of screen in dpi
	 */
	public float getScreenDensity()
	{
		DisplayMetrics metrics = new DisplayMetrics();
		CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getMetrics(metrics);
		return metrics.density;
	}
	/**
	 * @author S Downie
	 * 
	 * @param Max FPS to limit device to
	 */
	public void setPreferredFPS(int in_maxFPS)
	{
		CSApplication.get().setPreferredFPS(in_maxFPS);
	}
	/**
	 * Terminate the activity and the app
	 * 
	 * @author Ian Copland
	 */
    public void forceQuit()
    {
    	CSApplication.get().quit();
    }
	/**
	 * @author S Downie
	 * 
	 * @return system time in milliseconds
	 */
    public long getSystemTimeInMilliseconds()
    {
    	return System.currentTimeMillis(); 
    }
}
