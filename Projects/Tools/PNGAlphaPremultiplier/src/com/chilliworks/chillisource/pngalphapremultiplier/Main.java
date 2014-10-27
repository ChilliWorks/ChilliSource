/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 30/01/2013.
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

package com.chilliworks.chillisource.pngalphapremultiplier;

import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;

public class Main 
{
	//------------------------------------------------------
	/// Main
	///
	/// The entry point for the application. This processes
	/// the inputs and relays them onto the rest of the
	/// application.
	///
	/// @param The array of input strings.
	//------------------------------------------------------
	public static void main(String[] inastrArgs) throws Exception 
	{	
		//setup the logger.
		String[] arguments = Logging.start(inastrArgs);
		
		//check the number of arguments make sense.
		if (arguments.length == 0)
		{
			PrintHelpText();
			return;
		}
		
		//gather up commands
		PremultiplicationOptions options = new PremultiplicationOptions();
		for (int i = 0; i < arguments.length; ++i)
		{
			//Input param
			if (arguments[i].equalsIgnoreCase("-i") == true || arguments[i].equalsIgnoreCase("--input") == true)
			{
				if (i+1 < arguments.length)
					options.strInputFilename = arguments[i+1];
				else
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output param
			else if (arguments[i].equalsIgnoreCase("-o") == true || arguments[i].equalsIgnoreCase("--output") == true)
			{
				if (i+1 < arguments.length)
					options.strOutputFilename = arguments[i+1];
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//help param
			else if (arguments[i].equalsIgnoreCase("-h") == true || arguments[i].equalsIgnoreCase("--help") == true)
			{
				PrintHelpText();
				return;
			}
			
			//failure
			else
			{
				Logging.logFatal("Invalid argument found: " + arguments[i]);
			}
		}
		
		//confirm that the params are valid.
		if (options.strInputFilename.length() == 0)
			Logging.logFatal("No input filename provided.");
		if (options.strOutputFilename.length() == 0)
			Logging.logFatal("No output filename provided.");
		
		//now that the options have been gathered, perform the png pre-multiplying!
		PngPremultiplier.Premultiply(options);

		Logging.finish();
	}
	//------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//------------------------------------------------------
	private static void PrintHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.VERBOSE);
		Logging.logVerbose("Usage: java -jar PNGAlphaPremultiplier.jar --input <filepath> --output <filepath> [--logginglevel <level>] [--displayerrors <errordisplay>] [--help]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" --input(-i): The input filepath.");
		Logging.logVerbose(" --output(-o): The output filepath.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): Sets the logging level.");
		Logging.logVerbose(" --help(-h): Displays this help message.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
	}
}
