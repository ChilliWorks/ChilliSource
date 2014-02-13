package com.taggames.moflow.powermanager;

import android.content.Context;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;

/**
 * Class to access the power management features of Android
 * @author Hugh McLaughlin 
 * @copyright Tag-Games 2012
 *
 */

public class CMoFlowPowerManager 
{
	//--------------------------------------------------------
	/// Lock Type
	///
	/// The different lock types we can have
	/// Note::ePartial lock is the only one that can keep the 
	/// cpu awake even when the user manually puts the device to 
	/// sleep. A full list of each ones effect can be found here 
	/// - http://developer.android.com/reference/android/os/PowerManager.html
	//--------------------------------------------------------
	public enum LOCK_TYPE 
	{ 
		PARTIAL_LOCK, SCREEN_DIM_LOCK, SCREEN_BRIGHT_LOCK, FULL_LOCK, TOTAL_LOCKS
	}
	//--------------------------------------------------------
	/// Private Static Members
	//--------------------------------------------------------
	private static Context mpParentContext;
	private static PowerManager mpcPowerManager;
	private static WakeLock[] maWakeLocks;
	//--------------------------------------------------------
	/// Setup
	///
	/// @param the context
	//--------------------------------------------------------
	public static void Setup(Context inpContext)
	{
		mpParentContext = inpContext;
		mpcPowerManager = (PowerManager) mpParentContext.getSystemService(Context.POWER_SERVICE);
		
		int arraySize = (int)LOCK_TYPE.TOTAL_LOCKS.ordinal();
		maWakeLocks = new WakeLock[arraySize];
	}
	//--------------------------------------------------------
	/// RequestWakeLock
	///
	/// @param the lock type.
	//--------------------------------------------------------
	public static boolean RequestWakeLock(LOCK_TYPE ineLockType)
	{
		if(mpcPowerManager == null)
			return false;
		
		WakeLock pcWakeLock = null;
		
		switch(ineLockType)
		{
		case PARTIAL_LOCK:
			pcWakeLock = mpcPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "PartialWakeLock");
			break;
		case SCREEN_DIM_LOCK:
			pcWakeLock = mpcPowerManager.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP, "ScreenDimWakeLock");
			break;
		case SCREEN_BRIGHT_LOCK:
			pcWakeLock = mpcPowerManager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP, "ScreenBrightWakeLock");
			break;
		case FULL_LOCK:
			pcWakeLock = mpcPowerManager.newWakeLock(PowerManager.FULL_WAKE_LOCK | PowerManager.ACQUIRE_CAUSES_WAKEUP, "FullWakeLock");
			break;
		default:
			pcWakeLock = mpcPowerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "PartialWakeLock");
			break;
		}
		
		//Store the wake lock in its place
		
		if(maWakeLocks[ineLockType.ordinal()] == null)	
		{			
			//Aquire the lock
			pcWakeLock.acquire();
			maWakeLocks[ineLockType.ordinal()] = pcWakeLock;
		}
		else
		{
			maWakeLocks[ineLockType.ordinal()].acquire();
		}
	
		return false;
	}
	//--------------------------------------------------------
	/// ReleaseLock
	///
	/// @param the lock type.
	//--------------------------------------------------------
	public static void ReleaseLock(LOCK_TYPE ineLockType)
	{
		if(mpcPowerManager == null)
			return;
		
		if(maWakeLocks[ineLockType.ordinal()] != null)
		{
			if(maWakeLocks[ineLockType.ordinal()].isHeld())
			{
				maWakeLocks[ineLockType.ordinal()].release();
			}
		}
	}
	//--------------------------------------------------------
	/// GetIsScreenOn
	///
	/// @return whether the screen is on.
	//--------------------------------------------------------
	public static boolean GetIsScreenOn()
	{
		if(mpcPowerManager == null)
			return false;
		
		return mpcPowerManager.isScreenOn();
	}
	//--------------------------------------------------------
	/// PutDeviceToSleep
	///
	/// @param how long to sleep the device
	//--------------------------------------------------------
	public static void PutDeviceToSleep(long inTime)
	{
		if(mpcPowerManager == null)
			return;
		
		mpcPowerManager.goToSleep(inTime);
	}
}
