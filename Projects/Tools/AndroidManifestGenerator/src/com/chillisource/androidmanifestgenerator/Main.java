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

package com.chillisource.androidmanifestgenerator;

import com.taggames.toolutils.SCLogger;
import com.taggames.toolutils.SCStringUtils;

/**
 * Handles the entry point into the application. This manages the parsing of the input parameters and
 * passes them on to the rest of the tool.
 * 
 * @author I Copland
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
	 * @author I Copland
	 * 
	 * @param The input arguments.
	 */
	public static void main(String[] in_arguments) throws Exception 
	{
		SCLogger.Initialise(in_arguments);
		
		if (in_arguments.length == 0)
		{
			printHelpText();
			return;
		}
		
		String input = "";
		String template = "";
		String output = "";
		for (int i = 0; i < in_arguments.length; ++i)
		{
			//input
			if (in_arguments[i].equalsIgnoreCase(k_paramInput) == true || in_arguments[i].equalsIgnoreCase(k_paramInputShort) == true)
			{
				if (i+1 < in_arguments.length)
					input = SCStringUtils.StandardiseFilepath(in_arguments[i+1]);
				else
					SCLogger.LogFatalError("No input file path provided!");
				i++;
			}
			
			//template
			else if (in_arguments[i].equalsIgnoreCase(k_paramTemplate) == true || in_arguments[i].equalsIgnoreCase(k_paramTemplateShort) == true)
			{
				if (i+1 < in_arguments.length)
					template = SCStringUtils.StandardiseFilepath(in_arguments[i+1]);
				else
					SCLogger.LogFatalError("No template file path provided!");
				i++;
			}
			
			//output
			else if (in_arguments[i].equalsIgnoreCase(k_paramOutput) == true || in_arguments[i].equalsIgnoreCase(k_paramOutputShort) == true)
			{
				if (i+1 < in_arguments.length)
					output = SCStringUtils.StandardiseFilepath(in_arguments[i+1]);
				else
					SCLogger.LogFatalError("No output file path provided!");
				i++;
			}
			
			//help
			else if (in_arguments[i].equalsIgnoreCase(k_paramHelp) == true || in_arguments[i].equalsIgnoreCase(k_paramHelpShort) == true)
			{
				printHelpText();
				return;
			}
		}
		
		if (input.length() == 0 || template.length() ==0 || output.length() == 0)
		{
			SCLogger.LogFatalError("Must provide input, template and output file paths.");
		}
		
		AndroidManifestGenerator.generate(input, template, output);
		
		SCLogger.PrintErrorsAndWarnings();
	}
	/**
	 * Outputs help test for this tool. This will be displayed regardless of 
	 * the current logging level.
	 *
	 * @author I Copland
	 */
	public static void printHelpText()
	{
		SCLogger.SetLoggingLevel(SCLogger.LOGGING_LEVEL_VERBOSE);
		SCLogger.LogMessage("Parameters:");
		SCLogger.LogMessage(" '" + k_paramInput + "'('" + k_paramInputShort + "') -> The file path to the user manifest.");
		SCLogger.LogMessage(" '" + k_paramTemplate + "'('" + k_paramTemplateShort + "') -> The file path to the template manifest.");
		SCLogger.LogMessage(" '" + k_paramOutput + "'('" + k_paramOutputShort + "') -> The file path to the output AndroidManifest.xml.");
		SCLogger.LogMessage(" '--logginglevel'('-l') -> the level of messages to log.");
		SCLogger.LogMessage(" '--errordisplay'('-e') -> when to display errors.");
		SCLogger.LogMessage(" '" + k_paramHelp + "'('" + k_paramHelpShort + "') -> Display this help message.");
		SCLogger.LogMessage("Logging Levels:");
		SCLogger.LogMessage(" 'none' -> No logging.");
		SCLogger.LogMessage(" 'fatal' -> Only log fatal errors.");
		SCLogger.LogMessage(" 'error' -> Only log errors.");
		SCLogger.LogMessage(" 'warning' -> Log errors and warnings.");
		SCLogger.LogMessage(" 'verbose' -> Log all messages.");
		SCLogger.LogMessage("Error Display Options:");
		SCLogger.LogMessage(" 'never' -> Never display.");
		SCLogger.LogMessage(" 'atend' -> Log all errors at the end.");
		SCLogger.LogMessage(" 'whenreceived' -> Log all errors when received.");
	}
}
