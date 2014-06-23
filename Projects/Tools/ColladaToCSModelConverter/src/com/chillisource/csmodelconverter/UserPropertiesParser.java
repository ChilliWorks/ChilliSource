//
// CUserPropertiesParser.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter;

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
