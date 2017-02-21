/**
 * StringUtils.java
 * ChilliSource
 * Created by Ian Copland on 30/01/2013
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

package com.chilliworks.chillisource.coreutils;

import java.io.File;
import java.nio.charset.Charset;

/**
 * A collection of convenience methods for working with Strings.
 * 
 * @author Ian Copland
 */
public class StringUtils 
{
	private static final Charset UTF8_CHARSET = Charset.forName("UTF-8");
	
	/**
	 * Converts a string in UTF-16 format to a UTF8 byte array.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_string - The string to convert.
	 * 
	 * @return The output byte array in UTF-8 format. 
	 */
	public static byte[] stringToUTF8Bytes(String in_string)
	{
		return in_string.getBytes(UTF8_CHARSET);
	}
	/**
	 * Converts UTF-8 byte array to a string in UTF-16 format.
	 * 
	 * @param in_utf8Bytes - The byte array to convert.
	 * 
	 * @return The output string.
	 */
	public static String utf8BytesToString(final byte[] in_utf8Bytes)
	{
		return new String(in_utf8Bytes, UTF8_CHARSET);
	}
	/**
	 * Standardised the path to a file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The file path.
	 * 
	 * @return The standardised file path.
	 */
	public static String standardiseFilePath(String in_filePath)
	{
		String standardisedPath = in_filePath.replace("\\", "/");
		standardisedPath = standardisedPath.replace("//", "/");
		if (standardisedPath.endsWith("/") == true)
		{
			standardisedPath = standardisedPath.substring(0, standardisedPath.length() - 1);
		}
		return standardisedPath;
	}
	/**
	 * Standardised the path to a file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The file path.
	 * 
	 * @return The standardised file path.
	 */
	public static String standardiseDirectoryPath(String in_directoryPath)
	{
		String standardisedPath = in_directoryPath.replace("\\", "/");
		standardisedPath = in_directoryPath.replace("//", "/");
		if (standardisedPath.endsWith("/") == false)
		{
			standardisedPath += "/";
		}
		return standardisedPath;
	}
	/**
	 * @author Ian Copland
	 *
	 * @param in_path - The absolute file path
	 * @param in_basePath - The base directory path
	 *
	 * @return The given file path relative to the base path.
	 */
	public static String getRelativeFilePath(String in_filePath, String in_basePath) 
	{
	    String relativePath = new File(in_basePath).toURI().relativize(new File(in_filePath).toURI()).getPath();
	    return standardiseFilePath(relativePath);
	}
	/**
	 * @author Ian Copland
	 *
	 * @param in_path - The absolute directory path
	 * @param in_basePath - The base directory path
	 *
	 * @return the given directory path relative to the base path.
	 */
	public static String getRelativeDirectoryPath(String in_filePath, String in_basePath) 
	{
	    String relativePath = new File(in_basePath).toURI().relativize(new File(in_filePath).toURI()).getPath();
	    return standardiseDirectoryPath(relativePath);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_filePath - The file path.
	 * 
	 * @return The extension of a file path string.
	 */
	public static String getExtension(String in_filePath)
	{
		int dwIndex = in_filePath.lastIndexOf(".");
		if (dwIndex != -1)
			return in_filePath.substring(dwIndex + 1);
		return "";
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_filePath - The file path.
	 * 
	 * @return The file path without the extension.
	 */
	public static String removeExtension(String in_filePath)
	{
		int index = in_filePath.lastIndexOf(".");
		
		if (index != -1)
		{
			return in_filePath.substring(0, index);
		}
		
		return in_filePath;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_filePath - The file path.
	 * 
	 * @return The directory path part of the given file name.
	 * This will be standardised.
	 */
	public static String getDirectory(String in_filePath)
	{
		String standardisedFilePath = StringUtils.standardiseFilePath(in_filePath);
		int index = standardisedFilePath.lastIndexOf("/");
		
		if (index != -1)
		{
			return standardisedFilePath.substring(0, index + 1);
		}
		
		return "";
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_filePath - The file path.
	 * 
	 * @return The file name part of the file path.
	 */
	public static String getFileName(String in_filePath)
	{
		String standardisedFilePath = StringUtils.standardiseFilePath(in_filePath);
		int index = standardisedFilePath.lastIndexOf("/");
		
		if (index != -1)
		{
			standardisedFilePath = standardisedFilePath.substring(index + 1);
		}
		
		return standardisedFilePath;
	}
	/**
	 * Removes all spaces, line returns and tabs from the given string.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_string - The input string with whitespace.
	 * 
	 * @return The input string minus whitespace.
	 */
	public static String removeWhitespace(String in_string)
	{
		String strOutput = new String(in_string);
		strOutput = strOutput.replace(" ", "");
		strOutput = strOutput.replace("\n", "");
		strOutput = strOutput.replace("\r", "");
		strOutput = strOutput.replace("\t", "");
		return strOutput;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_string - The string to check.
	 * 
	 * @return Whether or not the string contains any spaces, line returns and tabs.
	 */
	public static boolean containsWhitespace(String in_string)
	{
		return in_string.contains(" ") == true || in_string.contains("\n") == true || in_string.contains("\r") == true || in_string.contains("\t") == true;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_exception - The exception.
	 * 
	 * @return The contents of the exception in string form. This includes the
	 * stack trace.
	 */
	public static String convertExceptionToString(Exception in_exception)
	{
		String strOutput = in_exception.getLocalizedMessage();
		for (StackTraceElement element: in_exception.getStackTrace())
		{
			strOutput += "\n" + element.toString();
		}
		
		return strOutput;
	}
}
