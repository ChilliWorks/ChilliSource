/**
 * UserPropertiesParser.java
 * Chilli Source
 * Created by Ian Copland on 15/01/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
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

package com.chilliworks.chillisource.colladatocsmodel;

import java.util.Hashtable;

public class UserPropertiesParser 
{
	//-------------------------------------------------------
	/// Parse
	///
	/// Parses the user data string into a hash table of
	/// properties.
	///
	/// @param the string.
	/// @param the output properties.
	//-------------------------------------------------------
	static public Hashtable<String, String> Parse(String instrData)
	{
		Hashtable<String, String> properties = new Hashtable<String, String>();
		
		//first of all break into statements
		String[] strStatements = instrData.split("\n|\r|;");
		
		//now iterate over each statement, removing all whitespace then splitting into key and value.
		for (int i = 0; i < strStatements.length; i++)
		{
			strStatements[i] = strStatements[i].replaceAll(" |\t", "");
			String[] strSplit = strStatements[i].split("=");
			if (strSplit.length == 2)
			{
				properties.put(strSplit[0], strSplit[1]);
			}
		}
		
		return properties;
	}
}
