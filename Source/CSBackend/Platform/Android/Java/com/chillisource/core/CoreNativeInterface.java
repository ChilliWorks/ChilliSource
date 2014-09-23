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

package com.chillisource.core;

import java.io.IOException;
import java.util.Locale;
import java.util.UUID;

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
import com.google.android.gms.ads.identifier.AdvertisingIdClient;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesNotAvailableException;
import com.google.android.gms.common.GooglePlayServicesRepairableException;
import com.google.android.gms.common.GooglePlayServicesUtil;

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
	private static final String k_preferencesKey = "CSPreferences";
	private static final String k_udidKey = "UDID";
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
	 * @return Locale code of device as a string
	 */
	public String getDefaultLocaleCode()
	{
		return Locale.getDefault().toString();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Mode of device as a string
	 */
	public String getDeviceModel()
	{
		return Build.MODEL.toString();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Manufacturer of device as a string
	 */
	public String getDeviceManufacturer()
	{
		return Build.MANUFACTURER.toString();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Type of device as a string
	 */
	public String getDeviceModelType()
	{
		return Build.DEVICE.toString();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return OS version as number
	 */
	public int getOSVersion()
	{
		return Build.VERSION.SDK_INT;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return Num of CPU cores available
	 */
	public int getNumberOfCores()
	{
		return Runtime.getRuntime().availableProcessors();
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
	 * Returns the UDID of this device if it can be obtained, otherwise returns an empty string
	 * 
	 * @author S Downie
	 *
	 * @return UDID string
	 */
	public String getUniqueId()
	{
		//--try the recommended Google advertising id
		
		//Google policy is that if the advertising id is available but the user has chosen not to
		//enable it then we cannot track the user by falling back on these other methods.
		int gpsAvailableResult = GooglePlayServicesUtil.isGooglePlayServicesAvailable(CSApplication.get().getAppContext());
		
		//If GPS is not potentially available i.e. Kindle etc then we can default to the older id types
		//If GPS is potentially available but the user has chosen not to install/update then we cannot fallback on
		//the older id types

		switch(gpsAvailableResult)
		{
		case ConnectionResult.SUCCESS:
			AdvertisingIdClient.Info advertisingIdInfo = getAdvertisingIdInfo();
			
			if(advertisingIdInfo.isLimitAdTrackingEnabled() == true)
			{
				//User has disabled the use of tracking
				return "";
			}

			String advertisingId = advertisingIdInfo.getId();
			if(advertisingId.equals("") == false)
			{
				//We cannot save this as the user is entitled to reset it
				return advertisingId;
			}
			
			break;
		case ConnectionResult.SERVICE_MISSING:
			//Fallback on other methods
			break;
		case ConnectionResult.SERVICE_DISABLED:
		case ConnectionResult.SERVICE_VERSION_UPDATE_REQUIRED:
		case ConnectionResult.SERVICE_INVALID:
		case ConnectionResult.DATE_INVALID:
			//Cannot fallback
			return "";
		}
		
		//Check and see if we have any other form of id stored
		String existingId = SharedPreferencesNativeInterface.GetString(k_preferencesKey, k_udidKey, "");
		if(existingId.equals("") == false)
		{
			return existingId;
		}

		//--try the mac address
		String macAddress = getMacAddress();
		if (macAddress.equals("") == false)
		{
			macAddress = HashMD5.generateHex(StringUtils.StringToUTF8ByteArray(macAddress));
			SharedPreferencesNativeInterface.SetString(k_preferencesKey, k_udidKey, macAddress);
			return macAddress;
		}

		//--try the Android ID
		String androidId = getAndroidID();
		if (androidId.equals("") == false)
		{
			androidId = HashMD5.generateHex(StringUtils.StringToUTF8ByteArray(androidId));
			SharedPreferencesNativeInterface.SetString(k_preferencesKey, k_udidKey, androidId);
			return androidId;
		}

		//--try the ID from the telephony manager
		String telephonyId = getTelephonyDeviceID();
		if (telephonyId.equals("") == false)
		{
			telephonyId = HashMD5.generateHex(StringUtils.StringToUTF8ByteArray(telephonyId));
			SharedPreferencesNativeInterface.SetString(k_preferencesKey, k_udidKey, telephonyId);
			return telephonyId;
		}

		//--if all this fails, fall back on generating a random hash.
		String randomId = HashMD5.generateHex(StringUtils.StringToUTF8ByteArray(UUID.randomUUID().toString()));
		SharedPreferencesNativeInterface.SetString(k_preferencesKey, k_udidKey, randomId);
		return randomId;
	}
	/**
	 * Method that returns the Device ID
	 * That can be acquired from TelephonyManager.getDeviceId().
	 * This id is not accessable if the device does not contain a
	 * sim card, or if the sim is unavailable (flight mode). If the 
	 * id is not accessable an empty string will be returned. This
	 * Should not change on factory reset of a device, but it may
	 * change if the sim card is changed.
	 * 
	 * @author Ian Copland
	 *
	 * @return the telephony device ID or an empty string.
	*/
	private String getTelephonyDeviceID()
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
	 * @author Ian Copland
	 *
	 * @return mac address or an empty string.
	*/
	private String getMacAddress()
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
	 * @author Ian Copland
	 *
	 * @return the unique Android ID or an empty string.
	*/
	private String getAndroidID()
	{
		String strID = Settings.Secure.getString(CSApplication.get().getActivityContext().getContentResolver(), Settings.Secure.ANDROID_ID);
		
		if (strID == null || strID.equalsIgnoreCase("9774d56d682e549c"))
			return "";

		return strID;
	}
	/**
	 * Google terms now state that the advertising id should be used in place of any other forms of id tracking (where available).
	 * The advertising id can be enabled/disabled/reset by the user and therefore is not guaranteed. Retrieval of the id is dependent
	 * on a certain version of the Google Play Store being installed (which means this id is Google only not Kindle).
	 * 
	 * If the version of the Play Store is not high enough then calling this method will fail. This method relies on the fact that
	 * the user has already been prompted to update to the latest Google Play Store.
	 * 
	 * NOTE: This method must not be called on the Android UI thread.
	 * 
	 * @author S Downie
	 * 
	 * @return Object containing Id and whether we are allowed to use it (NOTE: This can be null)
	 */
	private AdvertisingIdClient.Info getAdvertisingIdInfo()
	{
		try 
		{
			return AdvertisingIdClient.getAdvertisingIdInfo(CSApplication.get().getAppContext());
		} 
		catch (IllegalStateException e) 
		{
			
		} 
		catch (GooglePlayServicesRepairableException e) 
		{

		} 
		catch (IOException e) 
		{

		} 
		catch (GooglePlayServicesNotAvailableException e) 
		{

		}
		
		return null;
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
