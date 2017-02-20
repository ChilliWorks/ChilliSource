/**
 * Main.java
 * ChilliSource
 * Created by Robert Henning on 15/01/2013.
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

package com.chilliworks.chillisource.cstextbuilder;

import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;

public class Main
{
	private static final String k_paramInput = "--input";
	private static final String k_paramInputShort = "-i";
	private static final String k_paramOutput = "--output";
	private static final String k_paramOutputShort = "-o";
	private static final String k_paramLanguage = "--language";
	private static final String k_paramLanguageShort = "-l";
	private static final String k_paramHelp = "--help";
	private static final String k_paramHelpShort = "-h";
	
	public static void main(String[] in_arguments) throws Exception
	{
		String[] arguments = Logging.start(in_arguments);
		Logging.logVerbose("Text Extractor; Version 1.3.");
		
		if (arguments.length == 0)
		{
			printHelpText();
			return;
		}
		
		ExtractTextOptions options = new ExtractTextOptions();
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(k_paramInput) == true || arguments[i].equalsIgnoreCase(k_paramInputShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_inputFilePath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No input file path provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(k_paramOutput) == true || arguments[i].equalsIgnoreCase(k_paramOutputShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_outputFilePath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No output file path provided!");
				i++;
			}
			
			//template
			else if (arguments[i].equalsIgnoreCase(k_paramLanguage) == true || arguments[i].equalsIgnoreCase(k_paramLanguage) == true)
			{
				if (i+1 < arguments.length)
					options.m_language = arguments[i+1];
				else
					Logging.logFatal("No langauge provided!");
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
		
		if (options.m_inputFilePath.length() == 0 || options.m_outputFilePath.length() ==0 || options.m_language.length() == 0)
		{
			Logging.logFatal("Must provide input and output file paths and a language.");
		}
		
		CSTextBuilder.run(options);
		
		Logging.finish();
	}
	//------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//------------------------------------------------------
	private static void printHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.VERBOSE);
		Logging.logVerbose("Usage: java -jar CSTextBuilder.jar " + k_paramInput + " <file path> " + k_paramOutput + " <file path> " + k_paramLanguage + " <language code> [" + 
				Logging.PARAM_LOGGING_LEVEL + " <level>] [" + k_paramHelp + "]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + k_paramInput + "(" + k_paramInputShort + "): The file path to the source .xls file.");
		Logging.logVerbose(" " + k_paramOutput + "(" + k_paramOutputShort + "): The output file path.");
		Logging.logVerbose(" " + k_paramLanguage + "(" + k_paramLanguageShort + "): Name of language column in source .xls to extract text from.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): The level of message to log to console.");
		Logging.logVerbose(" " + k_paramHelp + "(" + k_paramHelpShort + "): Print this help message.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
	}
}
