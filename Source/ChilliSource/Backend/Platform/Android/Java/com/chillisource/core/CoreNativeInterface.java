/**
 *  CoreNativeInterface.java
 *  Chilli Source
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 */

package com.chillisource.core;

import java.util.Locale;

import com.chillisource.core.FileUtils;
import com.chillisource.core.InterfaceIDType;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;

/**
* Native interface for communicating core os features to native.
* Also responsible for funneling lifecycle events to application
* 
* @author I Copland
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
	 * @author I Copland
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
	 * @author I Copland
	 */
	public native void memoryWarning();
	/**
	 * Triggered when the screen resolution changes
	 * 
	 * @author I Copland
	 * 
	 * @param The new width.
	 * @param The new height.
	 */
	public native void onResolutionChanged(int in_width, int in_height);
	/**
	 * Constructor
	 * 
	 * @author I Copland
	 */
	public CoreNativeInterface() throws NameNotFoundException
	{
		m_packageInfo = CSApplication.get().getAppContext().getPackageManager().getPackageInfo(CSApplication.get().getActivityContext().getPackageName(), PackageManager.GET_ACTIVITIES);
	}
	/**
	 * @author I Copland
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
	 * @author I Copland
	 * 
	 * @return Path to external storage
	 */
	public String getExternalStorageDirectory()
	{
		return FileUtils.getExternalStorageDirectory();
	}
	/**
	 * @author I Copland
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
	 * @author I Copland
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
	 * @author I Copland
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
	 * @author I Copland
	 * 
	 * @return Application package name
	 */
	public String getPackageName()
	{
		return CSApplication.get().getActivityContext().getPackageName();
	}
	/**
	 * @author I Copland
	 * 
	 * @return Path to APK directory
	 */
	public String getAPKDirectory()
	{
		return m_packageInfo.applicationInfo.sourceDir;
	}
	/**
	 * @author I Copland
	 * 
	 * @return Screen width in pixels
	 */
	@SuppressWarnings("deprecation")
	public int getScreenWidth()
	{
		return CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getWidth();
	}
	/**
	 * @author I Copland
	 * 
	 * @return Screen height in pixels
	 */
	@SuppressWarnings("deprecation")
	public int getScreenHeight()
	{
		return CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getHeight();
	}
	/**
	 * @author I Copland
	 * 
	 * @return Locale code of device as a string
	 */
	public String getDefaultLocaleCode()
	{
		return Locale.getDefault().toString();
	}
	/**
	 * @author I Copland
	 * 
	 * @return Mode of device as a string
	 */
	public String getDeviceModel()
	{
		return Build.MODEL.toString();
	}
	/**
	 * @author I Copland
	 * 
	 * @return Manufacturer of device as a string
	 */
	public String getDeviceManufacturer()
	{
		return Build.MANUFACTURER.toString();
	}
	/**
	 * @author I Copland
	 * 
	 * @return Type of device as a string
	 */
	public String getDeviceModelType()
	{
		return Build.DEVICE.toString();
	}
	/**
	 * @author I Copland
	 * 
	 * @return OS version as number
	 */
	public int getOSVersion()
	{
		return Build.VERSION.SDK_INT;
	}
	/**
	 * @author I Copland
	 * 
	 * @return Num of CPU cores available
	 */
	public int getNumberOfCores()
	{
		return Runtime.getRuntime().availableProcessors();
	}
	/**
	 * @author I Copland
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
	 * Method accesable from native that returns the Device ID
	 * That can be acquired from TelephonyManager.getDeviceId().
	 * This id is not accessable if the device does not contain a
	 * sim card, or if the sim is unavailable (flight mode). If the 
	 * id is not accessable an empty string will be returned. This
	 * Should not change on factory reset of a device, but it may
	 * change if the sim card is changed.
	 * 
	 * @author I Copland
	 *
	 * @return the telephony device ID or an empty string.
	*/
	public String getTelephonyDeviceID()
	{
		TelephonyManager phoneManager = (TelephonyManager) CSApplication.get().getActivityContext().getSystemService(Context.TELEPHONY_SERVICE);
		String strId = phoneManager.getDeviceId();
		
		if (strId == null)
			return "";
		return strId;
	}
	/**
	 * Returns the mac address of the device. This may not be available
	 * on some devices while wifi is turned off. This should not change
	 * on factory reset of a device.
	 * 
	 * @author I Copland
	 *
	 * @return mac address or an empty string.
	*/
	public String getMacAddress()
	{
		WifiManager wifiManager = (WifiManager)CSApplication.get().getActivityContext().getSystemService(Context.WIFI_SERVICE);
		String strMacAddress = wifiManager.getConnectionInfo().getMacAddress();
		
		if (strMacAddress == null)
			return "";
		return strMacAddress;
	}
	/**
	 * Returns the android ID for this device. This should be unique
	 * however a bug in 2.2 has lead to a large amount of devices
	 * on 2.2 sharing the same ID (9774d56d682e549c). This ID is 
	 * checked for and, if found, is discarded. If the id is
	 * unavailable or the duplicate ID is found, an empty string
	 * will be returned. This value may change if the device is
	 * factory reset.
	 *
	 * @author I Copland
	 *
	 * @return the unique Android ID or an empty string.
	*/
	public String getAndroidID()
	{
		String strID = Settings.Secure.getString(CSApplication.get().getActivityContext().getContentResolver(), Settings.Secure.ANDROID_ID);
		
		if (strID == null || strID.equalsIgnoreCase("9774d56d682e549c"))
			return "";

		return strID;
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
	 * @author I Copland
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
