//
// CColladaUtils.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.colladaparserutils;

import java.util.ArrayList;

import org.xml.sax.Attributes;

public class ColladaUtils
{
	//--------------------------------------------------------------
	/// Split On Whitespace
	///
	/// Breaks a string up on all types of whitespace.
	///
	/// @return the output string array.
	//--------------------------------------------------------------
	public static String[] SplitOnWhitespace(String instrData)
	{
		String[] astrData = instrData.split("\\s+");
		ArrayList<String> llOutput = new ArrayList<String>();
		
		for (int i = 0; i < astrData.length; ++i)
		{
			if (astrData[i] != null && astrData[i].length() != 0)
			{
				llOutput.add(astrData[i]);
			}
		}
		
		String[] astrOutput = new String[llOutput.size()];
		return llOutput.toArray(astrOutput);
	}
	//--------------------------------------------------------------
	/// Get Int Attribute Or Zero
	///
	/// @return either the value of the attribute, or 0.
	//--------------------------------------------------------------
	public static int GetIntAttributeOrZero(Attributes inAttributes, String inName)
	{
		if (inAttributes.getIndex(inName) > -1)
		{
			return Integer.parseInt(inAttributes.getValue(inName));
		}
		
		return 0;
	}
	//--------------------------------------------------------------
	/// Get Int Attribute Or Zero
	///
	/// @return either the value of the attribute, or an empty string.
	//--------------------------------------------------------------
	public static String GetStringAttributeOrEmpty(Attributes inAttributes, String inName)
	{
		if (inAttributes.getIndex(inName) > -1)
		{
			return inAttributes.getValue(inName);
		}
		
		return "";
	}
}
