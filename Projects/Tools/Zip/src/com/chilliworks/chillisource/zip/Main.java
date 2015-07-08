/**
 * Main.java
 * ChilliSource
 * Created by Ian Copland on 01/07/2015.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Tag Games Limited
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

package com.chilliworks.chillisource.zip;

import java.util.ArrayList;
import java.util.List;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;

/**
 * The entry point into the application.
 * 
 * @author Ian Copland
 */
public final class Main 
{
	private static final String PARAM_NAME_INPUT = "--input";
	private static final String PARAM_NAME_OUTPUT = "--output";
	private static final String PARAM_NAME_UNCOMPRESSED_EXTENSIONS = "--uncompressedext";
	private static final String PARAM_NAME_HELP = "--help";
	private static final String SHORT_PARAM_NAME_INPUT = "-i";
	private static final String SHORT_PARAM_NAME_OUTPUT = "-o";
	private static final String SHORT_PARAM_NAME_UNCOMPRESSED_EXTENSIONS = "-ue";
	private static final String SHORT_PARAM_NAME_HELP = "-h";

	/**
	 * Entry point for the application. This parses and validates the input arguments for the tool
	 * then passes them into the Zipper.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The input arguments.
	 */
	public static void main(String[] in_args) throws Exception 
	{
		//setup the logger.
		String[] arguments = Logging.start(in_args);
		
		//check the number of arguments make sense.
		if (arguments.length == 0)
		{
			printHelpText();
			return;
		}
		
		//gather up commands
		ZipOptions params = new ZipOptions();
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(PARAM_NAME_INPUT) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_NAME_INPUT) == true)
			{
				if (i+1 < arguments.length)
					params.m_inputDirectoryPath = StringUtils.standardiseDirectoryPath(arguments[i+1]);
				else
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_OUTPUT) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_NAME_OUTPUT) == true)
			{
				if (i+1 < arguments.length)
					params.m_outputFilePath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//uncompressed extensions
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_UNCOMPRESSED_EXTENSIONS) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_NAME_UNCOMPRESSED_EXTENSIONS) == true)
			{
				if (i+1 < arguments.length)
					params.m_uncompressedExtensions = parseExtensionList(arguments[i+1]);
				else
					Logging.logFatal("No extension list provided!");
				i++;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_HELP) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_NAME_HELP) == true)
			{
				printHelpText();
				return;
			}
			
			//Failure
			else
			{
				Logging.logFatal("Invalid parameter found: " + arguments[i]);
			}
		}
		
		if (params.m_inputDirectoryPath.length() == 0 || params.m_outputFilePath.length() == 0)
		{
			Logging.logFatal("Must provide an input directory path and an output file path.");
		}
		
		try
		{
			Zipper.zip(params);
		}
		catch (CSException e)
		{
			Logging.logFatal(e.getMessage());
		}
		
		Logging.finish();
	}
	/**
	 * Parses a string containing a comma separated list of extensions. Extensions may optionally contain
	 * a leading full stop ("."). Trailing and leading whitespace around the entries is also valid, but
	 * extensions cannot contain whitespace.
	 * 
	 * Any trailing or leading whitespace will be removed from output entries, and any missing leading
	 * full stops will be added.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_extensionList - A comma separated list of extensions. Whitespace and full stops may be
	 * included.
	 * 
	 * @return The output list of extensions. Entries will not have leading or trailing whitespace and
	 * will have a leading full stop.
	 */
	private static List<String> parseExtensionList(String in_extensionList)
	{		
		String[] splitList;
		if (in_extensionList.contains(",") == true)
		{
			splitList = in_extensionList.split(",");
		}
		else
		{
			splitList = new String[1];
			splitList[0] = in_extensionList;
		}
		
		List<String> output = new ArrayList<>();
		for (String extension : splitList)
		{
			extension = extension.trim();

			if (extension.isEmpty() == true)
			{
				Logging.logFatal("Extension list cannot contain empty extensions.");
			}
			
			if (extension.equals(".") == true)
			{
				Logging.logFatal("Invalid extension '" + extension + "'");
			}
			
			if (extension.startsWith(".") == false)
			{
				extension = "." + extension;
			}
			
			if (StringUtils.containsWhitespace(extension) == true)
			{
				Logging.logFatal("Invalid extension '" + extension + "' contains whitespace.");
			}
			
			output.add(extension);
		}
		
		return output;
	}
	/**
	 * Prints out instructions on how to use this tool. This will print regardless
	 * of the logging level.
	 * 
	 * @author Ian Copland
	 */
	private static void printHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.VERBOSE);
		Logging.logVerbose("Usage: java -jar ColladaToCSAnim.jar " + PARAM_NAME_INPUT + " <file path> " + PARAM_NAME_OUTPUT + " <file path> [" + PARAM_NAME_UNCOMPRESSED_EXTENSIONS + "] [" 
				+ PARAM_NAME_HELP + "] [" + Logging.PARAM_LOGGING_LEVEL + " <level>]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + PARAM_NAME_INPUT + "(" + SHORT_PARAM_NAME_INPUT + "): The input directory path. This directory will not be included in the zip, only its contents.");
		Logging.logVerbose(" " + PARAM_NAME_OUTPUT + "(" + SHORT_PARAM_NAME_OUTPUT + "): The output zip file name.");
		Logging.logVerbose(" " + PARAM_NAME_UNCOMPRESSED_EXTENSIONS + "(" + SHORT_PARAM_NAME_UNCOMPRESSED_EXTENSIONS + "): [Optional] A comma separated list of extensions which should be left "
				+ "uncompressed in the zip. Extensions may optionally contain a leading full stop ('.'). Trailing and leading whitespace around the entries is also valid, but "
				+ "extensions cannot contain whitespace.");
		Logging.logVerbose(" " + PARAM_NAME_HELP + "(" + SHORT_PARAM_NAME_HELP + "): [Optional] Display this help message.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): [Optional] The level of messages to log.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
	}
}
