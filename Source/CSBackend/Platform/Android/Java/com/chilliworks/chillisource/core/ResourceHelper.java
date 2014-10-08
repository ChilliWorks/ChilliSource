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

//===============================================================================
/// Resource Helper
///
/// Helper class that allows you to access resource files in different packages 
/// than the one your in, can be useful for referencing resources in engine 
/// that are provided on a per project basis
//===============================================================================
public class ResourceHelper 
{
	//---------------------------------------------------------
	/// Resource Subclass
	///
	/// Helper enum to provide names for all possible resource types
	//---------------------------------------------------------
    public static enum RESOURCE_SUBCLASS
    {
    	RESOURCE_ATTR("attr"),
    	RESOURCE_STRING("string"),
    	RESOURCE_DRAWABLE("drawable"),
    	RESOURCE_ID("id"),
    	RESOURCE_LAYOUT("layout"),
    	RESOURCE_STYLE("style");
    	
    	private String text;
    	
    	RESOURCE_SUBCLASS(String text)
    	{
    		this.text = text;
    	}
    	
    	public String getText()
    	{
    		return this.text;
    	}
    }
    //---------------------------------------------------------
  	/// Does Resource Exist
  	///
  	/// Uses reflection to evaluate whether the given resource
    /// exists in the res/ directory.
    ///
    /// @param the sub resource that we want from this class e.g. string
    /// @param the string ID of the field we want
    /// @return whether or not it exists.
  	//---------------------------------------------------------
    static public boolean DoesResourceExist(RESOURCE_SUBCLASS ineResourceSubClass, String inField) 
    {
    	return DoesResourceExist(CSApplication.get().getAppContext(), ineResourceSubClass, inField);
    }
    //---------------------------------------------------------
  	/// Resource Subclass
  	///
  	/// Gets a resource ID from this applications resources
    /// folder. This can be useful for referencing project
    /// resources from the engine.
    ///
    /// @param ineResourceSubClass - the sub resource that we want from this class e.g. string
    /// @param inField - the string ID of the field we want
    /// @return ID of the resource required
  	//---------------------------------------------------------
    static public int GetDynamicResourceIDForField(RESOURCE_SUBCLASS ineResourceSubClass, String inField) 
    {
    	return GetDynamicResourceIDForField(CSApplication.get().getAppContext(), ineResourceSubClass, inField);
    }
    //---------------------------------------------------------
  	/// Does Resource Exist
  	///
  	/// Uses reflection to evaluate whether the given resource
    /// exists in the res/ directory.
    ///
    /// @param the context from which to get the package name.
    /// @param the sub resource that we want from this class e.g. string
    /// @param the string ID of the field we want
    /// @return whether or not it exists.
  	//---------------------------------------------------------
    static public boolean DoesResourceExist(Context inContext, RESOURCE_SUBCLASS ineResourceSubClass, String inField) 
    {
    	return DoesResourceExist(inContext.getPackageName(), ineResourceSubClass, inField);
    }
    //---------------------------------------------------------
  	/// Resource Subclass
  	///
  	/// Gets a resource ID from any context.
    ///
    /// @param inContext - the context from which to get the package name.
    /// @param ineResourceSubClass - the sub resource that we want from this class e.g. string
    /// @param inField - the string ID of the field we want
    /// @return ID of the resource required
  	//---------------------------------------------------------
    static public int GetDynamicResourceIDForField(Context inContext, RESOURCE_SUBCLASS ineResourceSubClass, String inField) 
    {
    	return GetDynamicResourceIDForField(inContext.getPackageName(), ineResourceSubClass, inField);
    }
    //---------------------------------------------------------
  	/// Does Resource Exist
  	///
  	/// Uses reflection to evaluate whether the given resource
    /// exists in the res/ directory.
    ///
    /// @param the package name for the resources.
    /// @param the sub resource that we want from this class e.g. string
    /// @param the string ID of the field we want
    /// @return whether or not it exists.
  	//---------------------------------------------------------
    static public boolean DoesResourceExist(String instrPackageName, RESOURCE_SUBCLASS ineResourceSubClass, String inField) 
    {
    	//Construct the path to the resource passed in, the $ symbol is used to denote a subclass of the R.java e.g. string{}
    	String strTotalPathToResource = instrPackageName + ".R$" + ineResourceSubClass.getText();
    	try 
    	{
    		final Class<?> resource = Class.forName(strTotalPathToResource);
			try 
			{
				final Field field = resource.getField(inField);
				if (field != null)
				{
					return true;
				}
				return false;
			}
			catch (Exception e)
			{
				return false;
			}
		} 
    	catch (ClassNotFoundException e) 
		{
			e.printStackTrace();
		}
    	
    	return false;
    }
    //---------------------------------------------------------
  	/// Resource Subclass
  	///
  	/// Gets a resource ID from any package using reflection.
    ///
    /// @param inPackageResource - package path to the Resources class e.g. com.example.test.R
    /// @param ineResourceSubClass - the sub resource that we want from this class e.g. string
    /// @param inField - the string ID of the field we want
    /// @return ID of the resource required
  	//---------------------------------------------------------
    static public int GetDynamicResourceIDForField(String inPackageResource, RESOURCE_SUBCLASS ineResourceSubClass, String inField) 
    {
    	//Construct the path to the resource passed in, the $ symbol is used to denote a subclass of the R.java e.g. string{}
    	String strTotalPathToResource = inPackageResource + ".R$" + ineResourceSubClass.getText();
    	
    	try 
    	{
    		final Class<?> resource = Class.forName(strTotalPathToResource);
			try 
			{
				//Get the Field fromt the string description passed in
				final Field field = resource.getField(inField);
				int id = field.getInt(null);
				return id;
			} 
			catch (NoSuchFieldException e) 
			{
				e.printStackTrace();
			} 
			catch (IllegalArgumentException e) 
			{
				e.printStackTrace();
			} 
			catch (IllegalAccessException e) 
			{
				e.printStackTrace();
			}
		} 
    	catch (ClassNotFoundException e) 
		{
			e.printStackTrace();
		}
    	
    	return 0;
    }
}
