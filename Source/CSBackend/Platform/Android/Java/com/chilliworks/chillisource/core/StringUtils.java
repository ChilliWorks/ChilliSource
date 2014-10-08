/**
 * StringUtils.java
 * Chilli Source
 * Created by Ian Copland on 08/01/2014.
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

import java.nio.charset.Charset;

//=================================================================
/// String Utils
///
/// A series of convenience methods for working with strings. This
/// includes functionality like conversions to and from byte arrays.
//=================================================================
public class StringUtils 
{
	//-------------------------------------------------------
	/// Constants
	//-------------------------------------------------------
	private static final Charset kUTF8Charset = Charset.forName("UTF-8");
	//-------------------------------------------------------
	/// String To UTF8 Byte Array
	///
	/// Converts a string in UTF-16 format to a UTF8 byte array. 
	///
	/// @param The string to convert.
	/// @return The output byte array in UTF-8 format.
	//-------------------------------------------------------
	public static byte[] StringToUTF8ByteArray(final String instrString)
	{
		return instrString.getBytes(kUTF8Charset);
	}
	//-------------------------------------------------------
	/// UTF8 Byte Array To String
	///
	/// Converts UTF-8 byte array to a string in UTF-16 format.
	///
	/// @param The byte array to convert.
	/// @return The output string.
	//-------------------------------------------------------
	public static String UTF8ByteArrayToString(final byte[] inabyUTF8)
	{
		return new String(inabyUTF8, kUTF8Charset);
	}
	//-------------------------------------------------------
	/// Standardise Path
	///
	/// @param The file path to standardise.
	/// @param The standardised file path. The standardised
	/// path will use forward slashes as the path separator
	/// and will end in a forward slash if it is a directory.
	//-------------------------------------------------------
	public static String StandardisePath(final String instrPath)
	{
		String strOutput = instrPath.replace("\\", "/");
		
		int dwLastSlash = strOutput.lastIndexOf("/");
		if (dwLastSlash == -1)
		{
			dwLastSlash = 0;
		}
		
		if (strOutput.length() > 0 && strOutput.substring(dwLastSlash).contains(".") == false)
		{
			strOutput += "/";
		}
		
		return strOutput;
	}
}
