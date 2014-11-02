/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 20/06/2014.
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

package com.chilliworks.chillisource.androidmanifestbuilder;

import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;

/**
 * Handles the entry point into the application. This manages the parsing of the input parameters and
 * passes them on to the rest of the tool.
 * 
 * @author Ian Copland
 */
public final class Main
{
	private static final String k_paramInput = "--input";
	private static final String k_paramInputShort = "-i";
	private static final String k_paramTemplate = "--template";
	private static final String k_paramTemplateShort = "-t";
	private static final String k_paramOutput = "--output";
	private static final String k_paramOutputShort = "-o";
	private static final String k_paramHelp = "--help";
	private static final String k_paramHelpShort = "-h";
	
	/**
	 * The entry point into the application
	 * 
	 * @author Ian Copland
	 * 
	 * @param The input arguments.
	 */
	public static void main(String[] in_arguments) throws Exception 
	{
		String[] arguments = Logging.start(in_arguments);
		
		if (arguments.length == 0)
		{
			printHelpText();
			return;
		}
		
		String input = "";
		String template = "";
		String output = "";
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(k_paramInput) == true || arguments[i].equalsIgnoreCase(k_paramInputShort) == true)
			{
				if (i+1 < arguments.length)
					input = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No input file path provided!");
				i++;
			}
			
			//template
			else if (arguments[i].equalsIgnoreCase(k_paramTemplate) == true || arguments[i].equalsIgnoreCase(k_paramTemplateShort) == true)
			{
				if (i+1 < arguments.length)
					template = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No template file path provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(k_paramOutput) == true || arguments[i].equalsIgnoreCase(k_paramOutputShort) == true)
			{
				if (i+1 < arguments.length)
					output = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No output file path provided!");
				i++;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(k_paramHelp) == true || arguments[i].equalsIgnoreCase(k_paramHelpShort) == true)
			{
				printHelpText();
				return;
			}
			
			//failure
			else
			{
				Logging.logFatal("Invalid parameter found: " + arguments[i]);
			}
		}
		
		if (input.length() == 0 || template.length() ==0 || output.length() == 0)
		{
			Logging.logFatal("Must provide input, template and output file paths.");
		}
		
		AndroidManifestBuilder.generate(input, template, output);
		
		Logging.finish();
	}
	/**
	 * Outputs help test for this tool. This will be displayed regardless of 
	 * the current logging level.
	 *
	 * @author Ian Copland
	 */
	public static void printHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.VERBOSE);
		Logging.logVerbose("Usage: java -jar AndroidManifestBuilder.jar --input <file path> --template <file path> --output <file path> [--logginglevel <level>] [--help]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + k_paramInput + "(" + k_paramInputShort + "): The file path to the user manifest.");
		Logging.logVerbose(" " + k_paramTemplate + "(" + k_paramTemplateShort + "): The file path to the template manifest.");
		Logging.logVerbose(" " + k_paramOutput + "(" + k_paramOutputShort + "): The file path to the output AndroidManifest.xml.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): [Optional] The level of messages to log.");
		Logging.logVerbose(" " + k_paramHelp + "(" + k_paramHelpShort + "): [Optional] Display this help message.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
	}
}
