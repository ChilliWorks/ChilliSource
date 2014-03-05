//  
//  CExceptionUtils.java
//  moFlow
//  
//  Created by Ian Copland on 16/12/2013
//  Copyright (c)2013 Tag Games Limited - All rights reserved
// 

package com.chillisource.core;

//=======================================================
/// Exception Utils
///
/// A collection of convenience methods for working with 
/// exceptions.
//=======================================================
public class ExceptionUtils
{
	//------------------------------------------------
	/// Get Stack Trace
	///
	/// @param The exception
	/// @return A string array containing the stack
	/// trace.
	//------------------------------------------------
	public static String[] GetStackTrace(Exception inException)
	{
		String[] astrStackTrace = new String[inException.getStackTrace().length];
		for (int i = 0; i < inException.getStackTrace().length; ++i)
		{
			StackTraceElement element = inException.getStackTrace()[i];
			astrStackTrace[i] = element.getFileName() + " - " + element.getClassName() + " - " + element.getMethodName() + " - " + element.getLineNumber();
		}
		return astrStackTrace;
	}
	//-----------------------------------------------
	/// Convert To String
	///
	/// @param The exception
	/// @return The details of the exception in string 
	/// format.
	//-----------------------------------------------
	public static String ConvertToString(Exception inException)
	{
		StringBuilder builder = new StringBuilder();
		builder.append(inException.getLocalizedMessage() + "\n");
		String[] astrStackTrace = GetStackTrace(inException);
		for (String strLine : astrStackTrace)
		{	
			builder.append("\t" + strLine + "\n");
		}
		return builder.toString();
	}
}
