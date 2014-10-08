/**
 * ExceptionUtils.java
 * Chilli Source
 * Created by Ian Copland on 16/12/2013.
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

package com.chilliworks.chillisource.core;

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
