/**
 * SharedPreferencesNativeInterface.java
 * Chilli Source
 * Created by Ian Copland on 08/06/2011.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2011 Tag Games Limited
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

import android.app.Activity;
import android.content.SharedPreferences;

public class SharedPreferencesNativeInterface 
{
	static final int kDefaultPreferencesMode = 0;
	
	private static Activity msActivity;
	
	//-------------------------------------------------------------------
	/// Setup
	///
	/// Initialises the native interface.
	///
	/// @param The applications activity
	//-------------------------------------------------------------------
	public static void Setup(Activity inActivity)
	{
		msActivity = inActivity;
	}
	//-------------------------------------------------------------------
	/// Key Exists
	/// 
	/// method accessable from native for checking if a key exists in
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @return whether or not the given key is available in the specified
	//			shared preferences file. 
	//-------------------------------------------------------------------
	public static boolean KeyExists(String instrName, String instrKey)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		return prefs.contains(instrKey);
	}
	//-------------------------------------------------------------------
	/// Remove Key
	/// 
	/// method accessable from native for removing key from
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @return whether or not the given key was available to be
	/// 		removed
	//-------------------------------------------------------------------
	public static boolean RemoveKey(String instrName, String instrKey)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		SharedPreferences.Editor editor = prefs.edit();
		if(KeyExists(instrName,instrKey))
		{
			editor.remove(instrKey);
			editor.commit();
			return true;
		}else
			return false;
		
	}
	//-------------------------------------------------------------------
	/// Get String
	///
	/// method accessable from native for getting a string from
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @param default value to return if the key doesnt exist.
	/// @return the value for the given key.
	//-------------------------------------------------------------------
	public static String GetString(String instrName, String instrKey, String instrDefaultValue)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		return prefs.getString(instrKey, instrDefaultValue);
	}
	//-------------------------------------------------------------------
	/// Set String
	///
	/// method accessable from native for setting a string in
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @param The value to write
	//-------------------------------------------------------------------
	public static void SetString(String instrName, String instrKey, String instrValue)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		SharedPreferences.Editor editor = prefs.edit();
		editor.putString(instrKey, instrValue);
		editor.commit();
	}
	//-------------------------------------------------------------------
	/// Get Boolean
	///
	/// method accessable from native for getting a boolean from
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @param default value to return if the key doesnt exist.
	/// @return the value for the given key.
	//-------------------------------------------------------------------
	public static boolean GetBoolean(String instrName, String instrKey, boolean inbDefaultValue)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		return prefs.getBoolean(instrKey, inbDefaultValue);
	}
	//-------------------------------------------------------------------
	/// Set Boolean
	///
	/// method accessable from native for setting a boolean in
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @param The value to write
	//-------------------------------------------------------------------
	public static void SetBoolean(String instrName, String instrKey, boolean inbValue)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		SharedPreferences.Editor editor = prefs.edit();
		editor.putBoolean(instrKey, inbValue);
		editor.commit();
	}
	//-------------------------------------------------------------------
	/// Get Int
	///
	/// method accessable from native for getting an int from
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @param default value to return if the key doesnt exist.
	/// @return the value for the given key.
	//-------------------------------------------------------------------
	public static int GetInt(String instrName, String instrKey, int indwDefaultValue)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		return prefs.getInt(instrKey, indwDefaultValue);
	}
	//-------------------------------------------------------------------
	/// Set Int
	///
	/// method accessable from native for setting an int in
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @param The value to write
	//-------------------------------------------------------------------
	public static void SetInt(String instrName, String instrKey, int indwValue)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		SharedPreferences.Editor editor = prefs.edit();
		editor.putInt(instrKey, indwValue);
		editor.commit();
	}
	//-------------------------------------------------------------------
	/// Get Long
	///
	/// method accessable from native for getting an int from
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @param default value to return if the key doesnt exist.
	/// @return the value for the given key.
	//-------------------------------------------------------------------
	public static long GetLong(String instrName, String instrKey, long inqwDefaultValue)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		return prefs.getLong(instrKey, inqwDefaultValue);
	}
	//-------------------------------------------------------------------
	/// Set Int
	///
	/// method accessable from native for setting an int in
	/// shared preferences
	///
	/// @param the shared preferences name.
	/// @param the key.
	/// @param The value to write
	//-------------------------------------------------------------------
	public static void SetLong(String instrName, String instrKey, long inqwValue)
	{
		SharedPreferences prefs = msActivity.getSharedPreferences(instrName, kDefaultPreferencesMode);
		SharedPreferences.Editor editor = prefs.edit();
		editor.putLong(instrKey, inqwValue);
		editor.commit();
	}
}
