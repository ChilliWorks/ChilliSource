/**
 * CSPowerManager.java
 * Chilli Source
 * Created by Hugh McLaughlin on 06/03/2012.
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

import android.content.Context;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;

public class CSPowerManager 
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
	@SuppressWarnings("deprecation")
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
}
