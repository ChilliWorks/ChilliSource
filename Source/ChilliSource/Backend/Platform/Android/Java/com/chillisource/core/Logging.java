/**
 *  CSLogging.java
 *  Chilli Source
 *
 *  Created by Scott Downie on 14/03/2014.
 *  Copyright 2014 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

import android.util.Log;

/**
 * Wrapper class around the Android logging system in order to abstract the 
 * tag.
 * 
 * @author S Downie
 */
public class Logging 
{
	/**
	 * Log message to LogCat under the "verbose" channel
	 * 
	 * @author S Downie
	 * 
	 * @param Message to display in LogCat
	 */
	public static void logVerbose(String in_message)
	{
		Log.d("Chilli Source", in_message);
	}
	/**
	 * Log message to LogCat under the "warning" channel
	 * 
	 * @author S Downie
	 * 
	 * @param Message to display in LogCat
	 */
	public static void logWarning(String in_message)
	{
		Log.w("Chilli Source", in_message);
	}
	/**
	 * Log message to LogCat under the "error" channel
	 * 
	 * @author S Downie
	 * 
	 * @param Message to display in LogCat
	 */
	public static void logError(String in_message)
	{
		Log.e("Chilli Source", in_message);
	}
	/**
	 * Log message to LogCat under the "error" channel
	 * will also cause the application to assert
	 * 
	 * @author S Downie
	 * 
	 * @param Message to display in LogCat
	 */
	public static void logFatal(String in_message)
	{
		Log.e("Chilli Source", in_message);
		assert false;
	}
}
