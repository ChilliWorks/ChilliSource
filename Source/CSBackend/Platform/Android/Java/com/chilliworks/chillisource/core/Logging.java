/**
 * Logging.java
 * Chilli Source
 * Created by Scott Downie on 14/03/2014.
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
