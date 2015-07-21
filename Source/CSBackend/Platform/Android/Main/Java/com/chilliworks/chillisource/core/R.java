/**
 * ResourceHelper.java
 * Chilli Source
 * Created by Hugh McLaughlin on 04/07/2012.
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

import java.lang.reflect.Field;

import android.content.Context;

/**
 * A series of convenience methods for accessing resources in res. This is useful in cases where
 * the R class cannot be directly included in a source file as it isn't part of the project. For
 * example, it allows code in ChilliSource to access resources included in the project.
 *
 * @author Hugh McLaughlin
 */
public final class R
{
	/**
	 * Helper enum to provide names for all possible resource types
	 *
	 * @author Hugh McLaughlin
	 */
    public enum Type
    {
    	ATTRIBUTE("attr"),
    	STRING("string"),
    	DRAWABLE("drawable"),
    	ID("id"),
    	LAYOUT("layout"),
    	STYLE("style");
    	
    	private String m_name = "";

		/**
		 * @author Hugh McLaughlin
		 *
		 * @param in_name - The name of this resource type.
		 */
    	Type(String in_name)
    	{
			m_name = in_name;
    	}
		/**
		 * @author Hugh McLaughlin
		 *
		 * @return The name of this resource type.
		 */
    	public String getName()
    	{
    		return m_name;
    	}
    }
	/**
	 * Checks whether or not the resource with the given name and type exists in res for the
	 * ChilliSource app.
	 *
	 * Instead of using the applicationId (as returned by Activity.getPackageName()), the "original"
	 * packageName specified in the CSAndroidManifest.xml will be used.
	 *
	 * @author Ian Copland
	 *
	 * @param in_type - The type of the resource.
	 * @param in_name - The name of the resource.
	 *
	 * @return Whether or not the resource with the given name exists.
	 */
    static public boolean doesExist(Type in_type, String in_name)
    {
    	return doesExist(CSApplication.get().getPackageName(), in_type, in_name);
    }
	/**
	 * Returns the resource with the given type and name from res for the ChilliSource app.
	 *
	 * Instead of using the applicationId - as returned by Activity.getPackageName() -  the
	 * "original" packageName, specified in the CSAndroidManifest.xml will be used.
	 *
	 * @param in_type - The type of the resource.
	 * @param in_name - The name of the resource.
	 *
	 * @return The Id of the requested resource.
	 */
    static public int getId(Type in_type, String in_name)
    {
    	return getId(CSApplication.get().getPackageName(), in_type, in_name);
    }
	/**
	 * Returns whether or not the resource with the given type and name exists in res for the
	 * given context.
	 *
	 * The applicationId - as returned by Application.getPackageName() - is used as the package
	 * name.
	 *
	 * @param in_context - The context form which to get the applicationId.
	 * @param in_type - The type of the resource.
	 * @param in_name - The name of the resource.
	 *
	 * @return Whether or not the resource with the given name exists.
	 */
    static public boolean doesExist(Context in_context, Type in_type, String in_name)
    {
    	return doesExist(in_context.getPackageName(), in_type, in_name);
    }
	/**
	 * Returns the id of the resource with the given type and name from res for the given context.
	 *
	 * The applicationId - as returned by Application.getPackageName() - is used as the package
	 * name.
	 *
	 * @param in_context - The context form which to get the applicationId.
	 * @param in_type - The type of the resource.
	 * @param in_name - The name of the resource.
	 *
	 * @return Whether or not the resource with the given name exists.
	 */
    static public int getId(Context in_context, Type in_type, String in_name)
    {
    	return getId(in_context.getPackageName(), in_type, in_name);
    }
	/**
	 * Returns whether or not the resource with the given type and name exists in res for the
	 * given packageName.
	 *
	 * @param in_packageName - The packageName which should be used.
	 * @param in_type - The type of the resource.
	 * @param in_name - The name of the resource.
	 *
	 * @return Whether or not the resource with the given name exists.
	 */
    static public boolean doesExist(String in_packageName, Type in_type, String in_name)
    {
    	String className = in_packageName + ".R$" + in_type.getName();
    	try 
    	{
    		final Class<?> resource = Class.forName(className);
			Field field = resource.getField(in_name);
			return (field != null);
		}
		catch (NoSuchFieldException e)
		{
			return false;
		}
    	catch (ClassNotFoundException e)
		{
			Logging.logVerbose(ExceptionUtils.convertToString(e));
			Logging.logFatal("Failed to check if resource '" + in_name + "' exists in '" + className+ "' becuase class doesn't exist.");
		}
		catch (Exception e)
		{
			Logging.logVerbose(ExceptionUtils.convertToString(e));
			Logging.logFatal("Failed to check if resource '" + in_name + "' exists in '" + className + "' becuase an exception occurred: " + e.getMessage());
		}
    	
    	return false;
    }
	/**
	 * Returns the id of the resource with the given type and name from res for the given
	 * packageName.
	 *
	 * @param in_packageName - The packageName which should be used.
	 * @param in_type - The type of the resource.
	 * @param in_name - The name of the resource.
	 *
	 * @return Whether or not the resource with the given name exists.
	 */
    static public int getId(String in_packageName, Type in_type, String in_name)
    {
    	String className = in_packageName + ".R$" + in_type.getName();
    	try 
    	{
    		final Class<?> resource = Class.forName(className);
			final Field field = resource.getField(in_name);
			return field.getInt(null);
		}
		catch (NoSuchFieldException e)
		{
			Logging.logVerbose(ExceptionUtils.convertToString(e));
			Logging.logFatal("Failed to get Id for resource '" + in_name + "' in '" + className + "' becuase the the resource name doesn't exist.");
		}
    	catch (ClassNotFoundException e) 
		{
			Logging.logVerbose(ExceptionUtils.convertToString(e));
			Logging.logFatal("Failed to get Id for resource '" + in_name + "' in '" + className + "' becuase class doesn't exist.");
		}
		catch (Exception e)
		{
			Logging.logVerbose(ExceptionUtils.convertToString(e));
			Logging.logFatal("\"Failed to get Id for resource '" + in_name + "' in '" + className + "' becuase an exception occurred: " + e.getMessage());
		}
    	
    	return 0;
    }
}
