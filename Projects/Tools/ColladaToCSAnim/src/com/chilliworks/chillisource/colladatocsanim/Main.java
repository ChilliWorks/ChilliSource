/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 16/01/2013.
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

package com.chilliworks.chillisource.colladatocsanim;

import com.chilliworks.chillisource.toolutils.Logging;
import com.chilliworks.chillisource.toolutils.StringUtils;
import com.chilliworks.chillisource.toolutils.Logging.LoggingLevel;

/**
 * The entry point into the application.
 * 
 * @author Ian Copland
 */
public final class Main 
{
	private static final String k_paramNameInput = "--input";
	private static final String k_paramNameOutput = "--output";
	private static final String k_paramNameSwapHandedness = "--swaphandedness";
	private static final String k_paramNameSwapYAndZ = "--swapyandz";
	private static final String k_paramNameHelp = "--help";
	private static final String k_shortParamNameInput = "-i";
	private static final String k_shortParamNameOutput = "-o";
	private static final String k_shortParamNameSwapHandedness = "-sh";
	private static final String k_shortParamNameSwapYAndZ = "-sy";
	private static final String k_shortParamNameHelp = "-h";

	/**
	 * Entry point for the application.
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
		ColladaToCSAnimOptions params = new ColladaToCSAnimOptions();
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(k_paramNameInput) == true || arguments[i].equalsIgnoreCase(k_shortParamNameInput) == true)
			{
				if (i+1 < arguments.length)
					params.m_inputFilePath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(k_paramNameOutput) == true || arguments[i].equalsIgnoreCase(k_shortParamNameOutput) == true)
			{
				if (i+1 < arguments.length)
					params.m_outputFilePath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//swap handedness
			else if (arguments[i].equalsIgnoreCase(k_paramNameSwapHandedness) == true || arguments[i].equalsIgnoreCase(k_shortParamNameSwapHandedness) == true)
			{
				params.m_swapHandedness = true;
			}
			
			//swap y and z
			else if (arguments[i].equalsIgnoreCase(k_paramNameSwapYAndZ) == true || arguments[i].equalsIgnoreCase(k_shortParamNameSwapYAndZ) == true)
			{
				params.m_swapYAndZ = true;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(k_paramNameHelp) == true || arguments[i].equalsIgnoreCase(k_shortParamNameHelp) == true)
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
		
		if (params.m_inputFilePath.length() == 0 || params.m_outputFilePath.length() == 0)
		{
			Logging.logFatal("Must provide an input and output file path.");
		}
		
		ColladaToCSAnim.Convert(params);
		
		Logging.finish();
	}
	/**
	 * Prints out instructions on how to use this tool. This will print regardless
	 * of the logging level.
	 * 
	 * @author Ian Copland
	 */
	public static void printHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.k_verbose);
		Logging.logVerbose("Usage: java -jar ColladaToCSAnim.jar " + k_paramNameInput + " <file path> " + k_paramNameOutput + " <file path> [" + k_paramNameSwapHandedness + " <transforms>] [" + k_paramNameHelp + "] [" + Logging.k_paramLoggingLevel + " <level>]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + k_paramNameInput + "(" + k_shortParamNameInput + "): The input filename.");
		Logging.logVerbose(" " + k_paramNameOutput + "(" + k_shortParamNameOutput + "): The output filename.");
		Logging.logVerbose(" " + k_paramNameSwapHandedness + "(" + k_shortParamNameSwapHandedness + "): [Optional] Swaps the handedness of the output coordinate system.");
		Logging.logVerbose(" " + k_paramNameSwapYAndZ + "(" + k_shortParamNameSwapYAndZ + "): [Optional] Swaps the Y and Z of the output coordinate system.");
		Logging.logVerbose(" " + Logging.k_paramLoggingLevel + "(" + Logging.k_paramLoggingLevelShort + "): [Optional] The level of messages to log.");
		Logging.logVerbose(" " + k_paramNameHelp + "(" + k_shortParamNameHelp + "): [Optional] Display this help message.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.k_loggingLevelNone + ": No logging.");
		Logging.logVerbose(" " + Logging.k_loggingLevelFatal + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelError + ": Only log errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelWarning + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.k_loggingLevelVerbose + ": Log all messages.");
	}
}
