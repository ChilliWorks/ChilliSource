package com.taggames.toolutils;

import java.nio.charset.Charset;

public class SCStringUtils 
{
	//-------------------------------------------------------
	/// Constants
	//-------------------------------------------------------
	private static final Charset kUTF8Charset = Charset.forName("UTF-8");
	//-------------------------------------------------------
	/// String To UTF8 Bytes
	///
	/// Converts a string in UTF-16 format to a UTF8 byte array. 
	///
	/// @param The string to convert.
	/// @return The output byte array in UTF-8 format.
	//-------------------------------------------------------
	public static byte[] StringToUTF8Bytes(final String instrString)
	{
		return instrString.getBytes(kUTF8Charset);
	}
	//-------------------------------------------------------
	/// UTF8 Bytes To String
	///
	/// Converts UTF-8 byte array to a string in UTF-16 format.
	///
	/// @param The byte array to convert.
	/// @return The output string.
	//-------------------------------------------------------
	public static String UTF8BytesToString(final byte[] inabyUTF8)
	{
		return new String(inabyUTF8, kUTF8Charset);
	}
	//-------------------------------------------------
	/// Standardise Filepath
	///
	/// Converts a filepath string to a standard format.
	///
	/// @param the directory
	/// @output the standardised path.
	//-------------------------------------------------
	public static String StandardiseFilepath(String instrFilepath)
	{
		String strStandardisedPath = new String(instrFilepath.replace("\\", "/"));
		if (strStandardisedPath.endsWith("/") == false && strStandardisedPath.contains(".") == false)
			strStandardisedPath += "/";
		return strStandardisedPath;
	}
	//-------------------------------------------------
	/// Get Extension
	///
	/// returns the extension of a filepath string
	///
	/// @param the directory
	/// @output the standardised path.
	//-------------------------------------------------
	public static String GetExtension(String instrFilepath)
	{
		int dwIndex = instrFilepath.lastIndexOf(".");
		if (dwIndex != -1)
			return instrFilepath.substring(dwIndex + 1);
		return "";
	}
	//-------------------------------------------------
	/// Remove Extension
	///
	/// removes the extension of a filepath string
	///
	/// @param the directory
	/// @output the standardised path.
	//-------------------------------------------------
	public static String RemoveExtension(String instrFilepath)
	{
		int dwIndex = instrFilepath.lastIndexOf(".");
		if (dwIndex != -1)
			return instrFilepath.substring(0, dwIndex);
		return instrFilepath;
	}
	//-------------------------------------------------
	/// Get Directory
	///
	/// removes the filename from a filepath.
	///
	/// @param the filepath
	/// @output the standardised directory path.
	//-------------------------------------------------
	public static String GetDirectory(String instrFilepath)
	{
		String strStandardisedFilepath = SCStringUtils.StandardiseFilepath(instrFilepath);
		int dwIndex = strStandardisedFilepath.lastIndexOf("/");
		if (dwIndex != -1)
			return strStandardisedFilepath.substring(0, dwIndex + 1);
		return "";
	}
	//-------------------------------------------------
	/// Remove Whitespace
	///
	/// Removes all spaces, line returns and tabs from
	/// the given string.
	///
	/// @param the string
	/// @output the string with no whitespace
	//-------------------------------------------------
	public static String RemoveWhitespace(String instrFilepath)
	{
		String strOutput = new String(instrFilepath);
		strOutput = strOutput.replace(" ", "");
		strOutput = strOutput.replace("\n", "");
		strOutput = strOutput.replace("\r", "");
		strOutput = strOutput.replace("\t", "");
		return strOutput;
	}
	//-------------------------------------------------
	/// Convert Exception To String
	///
	/// Returns the contents of a stack trace as a string.
	///
	/// @param the exception
	/// @output the string.
	//-------------------------------------------------
	public static String ConvertExceptionToString(Exception inException)
	{
		String strOutput = inException.getLocalizedMessage();
		for (StackTraceElement element: inException.getStackTrace())
		{
			strOutput += "\n" + element.toString();
		}
		
		return strOutput;
	}
	//-------------------------------------------------
	/// Compare String
	///
	/// Compares two Strings
	///
	/// @param the source string
	/// @param the string to compare
	/// @param true = ignore case
	/// @output true is Strings match, false otherwise.
	//-------------------------------------------------
	public static boolean CompareStrings(String instrSource, String instrCompare, boolean inbIgnoreCase)
	{
		if((instrSource == null) || (instrCompare == null))
			return false;
		
		if(!inbIgnoreCase)
			return instrSource.equals(instrCompare);
		else
			return instrSource.equalsIgnoreCase(instrCompare);
	}
	
	//-------------------------------------------------
	/// Compare String
	///
	/// Compares a String against two other strings.
	/// A positive result is returned if either comparing
	/// strings match the source string
	///
	/// @param the source string
	/// @param the first string to compare
	/// @param the second string to compare
	/// @param true = ignore case
	/// @output true is Strings match, false otherwise.
	//-------------------------------------------------
	public static boolean CompareStrings(String instrSource, String instrCompare1, String instrCompare2, boolean inbIgnoreCase)
	{
		return (CompareStrings(instrSource, instrCompare1, inbIgnoreCase) || CompareStrings(instrSource, instrCompare2, inbIgnoreCase));
	}
}
