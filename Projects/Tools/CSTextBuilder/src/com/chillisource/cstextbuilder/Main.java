/*
 *  CMain.java
 *  MasterTextTool
 *
 *  Created by Robert Henning on 15/01/2013.
 *  Refactoring using new 'Tool Utils' base. Previous version
 *  is 1.2 using a single source file (TextExtractor.java). Use
 *  this for any older projects that require .h/.java output.
 *  
 *  Copyright 2013 Tag Games Ltd. All rights reserved.
 *
 */

package com.chillisource.cstextbuilder;

import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.StringUtils;
import com.chillisource.toolutils.Logging.LoggingLevel;

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
					options.m_inputFilePath = StringUtils.standardiseFilepath(arguments[i+1]);
				else
					Logging.logFatal("No input file path provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(k_paramOutput) == true || arguments[i].equalsIgnoreCase(k_paramOutputShort) == true)
			{
				if (i+1 < arguments.length)
					options.m_outputFilePath = StringUtils.standardiseFilepath(arguments[i+1]);
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
		
		TextExtractor.Start(options);
		
		Logging.finish();
	}
	//------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//------------------------------------------------------
	private static void printHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.k_verbose);
		Logging.logVerbose("Usage: java -jar TextExtractor.jar --input <file path> --output <file path> --langauge <lanauage code> [" + Logging.k_paramLoggingLevel + " <level>] [--help]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + k_paramInput + "(" + k_paramInputShort + "): The file path to the source .xls file.");
		Logging.logVerbose(" " + k_paramOutput + "(" + k_paramOutputShort + "): The output file path.");
		Logging.logVerbose(" " + k_paramLanguage + "(" + k_paramLanguageShort + "): Name of language column in source .xls to extract text from.");
		Logging.logVerbose(" " + Logging.k_paramLoggingLevel + "(" + Logging.k_paramLoggingLevelShort + "): The level of message to log to console.");
		Logging.logVerbose(" " + k_paramHelp + "(" + k_paramHelpShort + "): Print this help message.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.k_loggingLevelNone + ": No logging.");
		Logging.logVerbose(" " + Logging.k_loggingLevelFatal + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelError + ": Only log errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelWarning + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.k_loggingLevelVerbose + ": Log all messages.");
	}
}
