/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 15/10/2014.
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

package com.chilliworks.chillisource.csprojectgenerator;

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
	private static final String k_paramProjectName = "--projectname";
	private static final String k_paramPackageName = "--packagename";
	private static final String k_paramOutputDirectory = "--output";
	private static final String k_paramHelp = "--help";

	private static final String k_shortParamProjectName = "-pr";
	private static final String k_shortParamPackageName = "-pa";
	private static final String k_shortParamOutputDirectory = "-o";
	private static final String k_shortParamHelp= "-h";
	
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
		
		Options options = new Options();
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(k_paramProjectName) == true || arguments[i].equalsIgnoreCase(k_shortParamProjectName) == true)
			{
				if (i+1 < arguments.length)
					options.m_projectName = arguments[i+1];
				else
					Logging.logFatal("No project name provided!");
				i++;
			}
			
			//template
			else if (arguments[i].equalsIgnoreCase(k_paramPackageName) == true || arguments[i].equalsIgnoreCase(k_shortParamPackageName) == true)
			{
				if (i+1 < arguments.length)
					options.m_packageName = arguments[i+1];
				else
					Logging.logFatal("No package name provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(k_paramOutputDirectory) == true || arguments[i].equalsIgnoreCase(k_shortParamOutputDirectory) == true)
			{
				if (i+1 < arguments.length)
					options.m_outputDirectory = StringUtils.standardiseDirectoryPath(arguments[i+1]);
				else
					Logging.logFatal("No output directory path provided!");
				i++;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(k_paramHelp) == true || arguments[i].equalsIgnoreCase(k_shortParamHelp) == true)
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
		
		if (options.m_projectName.length() == 0 || options.m_packageName.length() ==0)
		{
			Logging.logFatal("Must provide project name, package name.");
		}
		
		CSProjectGenerator.generate(options);
		
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
		Logging.logVerbose("Usage: java -jar CSProjectGenerator.jar " + k_paramProjectName + " <project name> " + k_paramPackageName + " <package name> " + k_paramOutputDirectory + 
				" <directory path> [" + Logging.PARAM_LOGGING_LEVEL + " <level>] [" + k_paramHelp + "]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + k_paramProjectName + "(" + k_shortParamProjectName + "): The file path to the user manifest.");
		Logging.logVerbose(" " + k_paramPackageName + "(" + k_shortParamPackageName + "): The file path to the template manifest.");
		Logging.logVerbose(" " + k_paramOutputDirectory + "(" + k_shortParamOutputDirectory + "): [Optional] The file path to the output the project. By default the project is created in the present working directory.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): [Optional] The level of messages to log.");
		Logging.logVerbose(" " + k_paramHelp + "(" + k_shortParamHelp + "): [Optional] Display this help message.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
	}
}
