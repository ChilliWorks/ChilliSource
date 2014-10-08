/**
 * AppConfig.java
 * Chilli Source
 * Created by Scott Downie on 23/09/2014.
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

import org.json.JSONObject;

import android.app.Activity;

/**
 * Parses the App.config to obtain Android specific configuration properties
 * 
 * @author S Downie
 */
public final class AppConfig 
{
	private final String k_configFilePath = "AppResources/App.config";
	
	private String m_surfaceFormat = "rgb565_depth24";
	private boolean m_requiresGooglePlayServices = false;
	
	/**
	 * Constructor that parses the App.config
	 * 
	 * @author S Downie
	 * 
	 * @param Activity used to load file
	 */
	AppConfig(Activity in_activity)
	{
		parse(in_activity);
	}
	/**
	 * @author S Downie
	 * 
	 * @return Config property representing surface format
	 */
	public String getSurfaceFormat()
	{
		return m_surfaceFormat;
	}
	/**
	 * @author S Downie
	 * 
	 * @return Whether Google play services is required allowing by the app
	 */
	public boolean isGooglePlayServicesRequired()
	{
		return m_requiresGooglePlayServices;
	}
	/**
	 * Reads the properties from the App.config file.
	 *
	 * @author S Downie
	 * 
	 * @param The activity
	 */
	private void parse(Activity in_activity)
	{
		if (FileUtils.doesFileExistAPK(in_activity, k_configFilePath))
		{
			byte[] byteContents = FileUtils.readFileAPK(in_activity, k_configFilePath);
			String stringContents = StringUtils.UTF8ByteArrayToString(byteContents);
			
			try
			{
				JSONObject root = new JSONObject(stringContents);
				if (root.has("PreferredSurfaceFormat") == true)
				{
					m_surfaceFormat = root.getString("PreferredSurfaceFormat");
				}
				
				if(root.has("Android") == true)
				{
					JSONObject android = root.getJSONObject("Android");
					
					if(android.has("PreferredSurfaceFormat") == true)
					{
						m_surfaceFormat = android.getString("PreferredSurfaceFormat");
					}
					
					if(android.has("GooglePlay") == true)
					{
						JSONObject googlePlay = android.getJSONObject("GooglePlay");

						if(googlePlay.has("RequiresGooglePlayServices") == true)
						{
							m_requiresGooglePlayServices = googlePlay.getBoolean("RequiresGooglePlayServices");
						}
					}
				}
			}
			catch (Exception e)
			{
				Logging.logFatal("Could not load App.config!");
			}
		}
		else
		{
			Logging.logFatal("App.config does not exist!");
		}
	}
}
