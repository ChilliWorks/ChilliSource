/*
 *  CCoreNativeInterface.java
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

import java.util.Locale;

import com.chillisource.core.FileUtils;
import com.chillisource.core.InterfaceIDType;
import com.chillisource.core.ILaunchIntentReceiver;


import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.provider.Settings;
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
import android.util.FloatMath;

//=============================================================
/// Core Native Interface
///
/// Native interface for communicating core features between
/// native and java.
//=============================================================
public class CoreNativeInterface extends INativeInterface implements ILaunchIntentReceiver
{	
	//--------------------------------------------------------------
	/// Static Member Data
	//--------------------------------------------------------------
	public static InterfaceIDType InterfaceID = new InterfaceIDType("CCoreNativeInterface");
	//--------------------------------------------------------------
	/// Member Data
	//--------------------------------------------------------------
	private PackageInfo mPackageInfo;
	//--------------------------------------------------------------
	/// Native methods
	//--------------------------------------------------------------
	public native void update(float in_timeSinceLastUpdate, long in_appElapsedTime);
	public native void memoryWarning();
	public native void orientationChanged(int indwOrientation);
	public native void onBackPressed();
	public native void applicationDidReceiveLaunchingURL(String instrURL);
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
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public CoreNativeInterface() throws NameNotFoundException
	{
		mPackageInfo = CSApplication.get().getAppContext().getPackageManager().getPackageInfo(CSApplication.get().getActivityContext().getPackageName(), PackageManager.GET_ACTIVITIES);
	}
	//--------------------------------------------------------------
	/// Is A
	///
	/// @return whether or not this implements the given interface.
	//--------------------------------------------------------------
	@Override public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return (inInterfaceType.Equals(InterfaceID));
	}
	//-------------------------------------------------------------
	/// On Activity Result
	///
	/// Receives intents containing launching URLs. These will be
	/// passed on to the native side of the application, sending a
	/// Launching Actions event.
	///
	/// @param The new launch intent.
	//-------------------------------------------------------------
	@Override public void OnLaunchIntentReceived(Intent inIntent) 
	{
		if(inIntent != null)
    	{
    		String strUri = inIntent.getDataString();
    		
    		if(strUri != null)
    		{
    			applicationDidReceiveLaunchingURL(strUri);
    		}
    	}
	}
	//--------------------------------------------------------------
	/// Get External Storage Directory
	///
	/// A method accessable from native that returns the external
	/// storage directory.
	///
	/// @return the external storage directory.
	//--------------------------------------------------------------
	public String GetExternalStorageDirectory()
	{
		return FileUtils.GetExternalStorageDirectory();
	}
	//--------------------------------------------------------------
	/// Get Application Name
	///
	/// A method accessable from native that returns the application
	/// name.
	///
	/// @return the application name.
	//--------------------------------------------------------------
	public String GetApplicationName()
	{
		if (mPackageInfo.applicationInfo != null)
		{
			return CSApplication.get().getActivityContext().getPackageManager().getApplicationLabel(mPackageInfo.applicationInfo).toString();
		}
		else
		{
			return "ApplicationInfoWrong";
		}
	}
	//--------------------------------------------------------------
	/// Get Application Version Code
	///
	/// A method accessable from native that returns the application
	/// version code as found in the manifest.
	///
	/// @return the application code.
	//--------------------------------------------------------------
	public int GetApplicationVersionCode()
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
	//--------------------------------------------------------------
	/// Get Application Version Name
	///
	/// A method accessable from native that returns the application
	/// version name as found in the manifest.
	///
	/// @return the application version name.
	//--------------------------------------------------------------
	public String GetApplicationVersionName()
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
	//--------------------------------------------------------------
	/// Get Package Name
	///
	/// A method accessable from native that returns the package
	/// name.
	///
	/// @return the package name.
	//--------------------------------------------------------------
	public String GetPackageName()
	{
		return CSApplication.get().getActivityContext().getPackageName();
	}
	//--------------------------------------------------------------
	/// Get APK Directory
	///
	/// A method accessable from native that returns this applications
	/// apk directory.
	///
	/// @return the apk directory.
	//--------------------------------------------------------------
	public String GetAPKDirectory()
	{
		return mPackageInfo.applicationInfo.sourceDir;
	}
	//--------------------------------------------------------------
	/// Get Orientation
	///
	/// A method accessable from native that returns this devices
	/// orientation.
	///
	/// @return the device orientation are reported by the OS.
	//--------------------------------------------------------------
	public int GetOrientation()
	{
		return CSApplication.get().getActivityContext().getResources().getConfiguration().orientation;
	}
	//--------------------------------------------------------------
	/// Returns the constant value for Landscape Orientation
	///
	/// A method accessable from native that returns the constant
	/// value for landscape orientation.
	///
	/// @return the constant value for landscape orientation.
	//--------------------------------------------------------------
	public int GetOrientationLandscapeConstant()
	{
		return Configuration.ORIENTATION_LANDSCAPE;
	}
	//--------------------------------------------------------------
	/// Returns the constant value for Portrait Orientation
	///
	/// A method accessable from native that returns the constant
	/// value for portrait orientation.
	///
	/// @return the constant value for portrait orientation.
	//--------------------------------------------------------------
	public int GetOrientationPortraitConstant()
	{
		return Configuration.ORIENTATION_PORTRAIT;
	}
	//--------------------------------------------------------------
	/// Get Screen Width
	///
	/// A method accessable from native that returns this devices
	/// screen width.
	///
	/// @return the screen width.
	//--------------------------------------------------------------
	public int GetScreenWidth()
	{
		return CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getWidth();
	}
	//--------------------------------------------------------------
	/// Get Screen Height
	///
	/// A method accessable from native that returns this devices
	/// screen height
	///
	/// @return the screen height.
	//--------------------------------------------------------------
	public int GetScreenHeight()
	{
		return CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getHeight();
	}
	//--------------------------------------------------------------
	/// Get Default Locale Code
	///
	/// A method accessable from native that returns this devices
	/// currently set default locale code.
	///
	/// @retuurn the locale code as a string.
	//--------------------------------------------------------------
	public String GetDefaultLocaleCode()
	{
		String strLocaleCode = Locale.getDefault().toString();
		return strLocaleCode;
	}
	//--------------------------------------------------------------
	/// Get Device Model
	///
	/// A method accessable from native that returns the device model
	///
	/// @return the device model.
	//--------------------------------------------------------------
	public String GetDeviceModel()
	{
		return Build.MODEL.toString();
	}
	//--------------------------------------------------------------
	/// Get Device Manufacturer
	///
	/// A method accessable from native that returns the device
	/// manufacturer.
	///
	// @return the device manufacturer.
	//--------------------------------------------------------------
	public String GetDeviceManufacturer()
	{
		return Build.MANUFACTURER.toString();
	}
	//--------------------------------------------------------------
	/// Get Device Model Type
	///
	/// A method accessable from native that returns this devices
	/// model type.
	///
	/// @return the device type
	//--------------------------------------------------------------
	public String GetDeviceModelType()
	{
		return Build.DEVICE.toString();
	}
	//--------------------------------------------------------------
	/// Get OS Version
	///
	/// A method accessable from native that returns the OS version
	/// number. This is the internal api level number, not the public
	/// version number. For example: an android 2.2 device will return
	/// 8 - the api level used for android 2.2.
	///
	/// @return the OS version number.
	//--------------------------------------------------------------
	public int GetOSVersion()
	{
		return Build.VERSION.SDK_INT;
	}
	//--------------------------------------------------------------
	/// Get Number of Cores
	///
	/// A method accessable from native that returns the number of
	/// cores available on the device.
	///
	/// @return the number of cores.
	//--------------------------------------------------------------
	public int GetNumberOfCores()
	{
		return Runtime.getRuntime().availableProcessors();
	}
	//--------------------------------------------------------------
	/// Get Screen Density
	///
	/// A method accessable from native that returns the screen
	/// density of this device.
	///
	/// @return the screen density.
	//--------------------------------------------------------------
	public float GetScreenDensity()
	{
		DisplayMetrics metrics = new DisplayMetrics();
		CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getMetrics(metrics);
		return metrics.density;
	}
	//--------------------------------------------------------------
	/// Get Telephony Device ID
	///
	/// Method accesable from native that returns the Device ID
	/// That can be acquired from TelephonyManager.getDeviceId().
	/// This id is not accessable if the device does not contain a
	/// sim card, or if the sim is unavailable (flight mode). If the 
	/// id is not accessable an empty string will be returned. This
	/// Should not change on factory reset of a device, but it may
	/// change if the sim card is changed.
	///
	/// @return the telephony device ID or an empty string.
	//--------------------------------------------------------------
	public String GetTelephonyDeviceID()
	{
		TelephonyManager phoneManager = (TelephonyManager) CSApplication.get().getActivityContext().getSystemService(Context.TELEPHONY_SERVICE);
		String strId = phoneManager.getDeviceId();
		
		if (strId == null)
			return "";
		return strId;
	}
	//--------------------------------------------------------------
	/// Get Mac Address
	///
	/// Returns the mac address of the device. This may not be available
	/// on some devices while wifi is turned off. This should not change
	/// on factory reset of a device.
	///
	/// @return the Mac Address or an empty string.
	//--------------------------------------------------------------
	public String GetMacAddress()
	{
		WifiManager wifiManager = (WifiManager)CSApplication.get().getActivityContext().getSystemService(Context.WIFI_SERVICE);
		String strMacAddress = wifiManager.getConnectionInfo().getMacAddress();
		
		if (strMacAddress == null)
			return "";
		return strMacAddress;
	}
	//--------------------------------------------------------------
	/// Get Android ID
	///
	/// Returns the android ID for this device. This should be unique
	/// however a bug in 2.2 has lead to a large amount of devices
	/// on 2.2 sharing the same ID (9774d56d682e549c). This ID is 
	/// checked for and, if found, is discarded. If the id is
	/// unavailable or the duplicate ID is found, an empty string
	/// will be returned. This value may change if the device is
	/// factory reset.
	///
	/// @return the unique Android ID or an empty string.
	//--------------------------------------------------------------
	public String GetAndroidID()
	{
		String strID = Settings.Secure.getString(CSApplication.get().getActivityContext().getContentResolver(), Settings.Secure.ANDROID_ID);
		
		if (strID == null || strID.equalsIgnoreCase("9774d56d682e549c"))
			return "";

		return strID;
	}
	//------------------------------------------------------------------
	/// Set Max FPS
	///
	/// @param Max FPS to clamp to
	//------------------------------------------------------------------
	public void SetMaxFPS(int in_maxFPS)
	{
		CSApplication.get().setMaxFPS(in_maxFPS);
	}
    //-----------------------------------------------------------------------------------------------------
    /// Force Quit
    ///
    /// Kill the current process
    //-----------------------------------------------------------------------------------------------------
    public void ForceQuit()
    {
    	CSApplication.get().quit();
    }
    //-----------------------------------------------------------------------------------------------------
    /// GetSystemTimeInMilliseconds
    ///
    /// @return system time in milliseconds
    //-----------------------------------------------------------------------------------------------------
    public long GetSystemTimeInMilliseconds()
    {
    	return System.currentTimeMillis(); 
    }
    //-----------------------------------------------------------------------------------------------------
    /// Get Physical Screen Size
    ///
    /// @return the physical screen size in inches.
    //-----------------------------------------------------------------------------------------------------
    public float GetPhysicalScreenSize()
    {
    	//get the display metrics
    	DisplayMetrics displayMetrics = new DisplayMetrics();
    	CSApplication.get().getActivity().getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
    	
    	//calculate the diagonal physical size from dpi and resolution.
    	float sizeX = GetScreenWidth() / displayMetrics.xdpi;
    	float sizeY = GetScreenHeight() / displayMetrics.ydpi;
    	return (float)FloatMath.sqrt((sizeX * sizeX) + (sizeY * sizeY));
    }
}
