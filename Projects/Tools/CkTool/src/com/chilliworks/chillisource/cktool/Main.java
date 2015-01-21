/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 20/01/2015.
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

package com.chilliworks.chillisource.cktool;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;

/**
 * A static class that provides the main entry point into the application.
 * 
 * @author Ian Copland
 */
public final class Main 
{
	private static final String PARAM_ACTION = "--action";
	private static final String PARAM_INPUT = "--input";
	private static final String PARAM_OUTPUT = "--output";
	private static final String PARAM_FORMAT = "--format";
	private static final String PARAM_VOLUME = "--volume";
	private static final String PARAM_PAN = "--pan";
	private static final String PARAM_LOOP_START = "--loopstart";
	private static final String PARAM_LOOP_END = "--loopend";
	private static final String PARAM_LOOP_COUNT = "--loopcount";
	private static final String PARAM_HELP = "--help";
	
	private static final String SHORT_PARAM_ACTION = "-a";
	private static final String SHORT_PARAM_INPUT = "-i";
	private static final String SHORT_PARAM_OUTPUT = "-o";
	private static final String SHORT_PARAM_FORMAT = "-f";
	private static final String SHORT_PARAM_VOLUME = "-v";
	private static final String SHORT_PARAM_PAN = "-p";
	private static final String SHORT_PARAM_LOOP_START = "-ls";
	private static final String SHORT_PARAM_LOOP_END = "-le";
	private static final String SHORT_PARAM_LOOP_COUNT = "-lc";
	private static final String SHORT_PARAM_HELP = "-h";
	
	private static final String ACTION_BUILD_BANK = "buildbank";
	private static final String ACTION_BUILD_BANK_SINGLE = "buildbanksingle";
	private static final String ACTION_BUILD_STREAM = "buildstream";
	
	/**
	 * The entry point into the application.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_arguments - The argument list.
	 */
	public static void main(String[] in_arguments)
	{	
		//setup the logger.
		String[] arguments = Logging.start(in_arguments);
		
		//check the number of arguments make sense.
		if (arguments.length == 0)
		{
			PrintHelpText();
			return;
		}
		
		//gather up commands
		Options options = new Options();
		for (int i = 0; i < arguments.length; ++i)
		{
			//Action param
			if (arguments[i].equalsIgnoreCase(PARAM_ACTION) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_ACTION) == true)
			{
				if (i+1 < arguments.length)
					options.m_action = ParseAction(arguments[i+1]);
				else
					Logging.logFatal("No action provided!");
				++i;
			}
			
			//Input param
			else if (arguments[i].equalsIgnoreCase(PARAM_INPUT) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_INPUT) == true)
			{
				if (i+1 < arguments.length)
					options.m_inputFilePath = arguments[i+1];
				else
					Logging.logFatal("No input file provided!");
				++i;
			}
			
			//output param
			else if (arguments[i].equalsIgnoreCase(PARAM_OUTPUT) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_OUTPUT) == true)
			{
				if (i+1 < arguments.length)
					options.m_outputFilePath = arguments[i+1];
				else
					Logging.logFatal("No output file provided!");
				++i;
			}
			
			//format param
			else if (arguments[i].equalsIgnoreCase(PARAM_FORMAT) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_FORMAT) == true)
			{
				if (i+1 < arguments.length)
					options.m_format = arguments[i+1];
				else
					Logging.logFatal("No format provided!");
				++i;
			}
			
			//volume param
			else if (arguments[i].equalsIgnoreCase(PARAM_VOLUME) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_VOLUME) == true)
			{
				if (i+1 < arguments.length)
					options.m_volume = arguments[i+1];
				else
					Logging.logFatal("No format provided!");
				++i;
			}
			
			//pan param
			else if (arguments[i].equalsIgnoreCase(PARAM_PAN) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_PAN) == true)
			{
				if (i+1 < arguments.length)
					options.m_pan = arguments[i+1];
				else
					Logging.logFatal("No pan provided!");
				++i;
			}
			
			//loop start param
			else if (arguments[i].equalsIgnoreCase(PARAM_LOOP_START) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_LOOP_START) == true)
			{
				if (i+1 < arguments.length)
					options.m_loopStart = arguments[i+1];
				else
					Logging.logFatal("No loop start provided!");
				++i;
			}
			
			//loop end param
			else if (arguments[i].equalsIgnoreCase(PARAM_LOOP_END) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_LOOP_END) == true)
			{
				if (i+1 < arguments.length)
					options.m_loopEnd = arguments[i+1];
				else
					Logging.logFatal("No loop end provided!");
				++i;
			}
			
			//loop count param
			else if (arguments[i].equalsIgnoreCase(PARAM_LOOP_COUNT) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_LOOP_COUNT) == true)
			{
				if (i+1 < arguments.length)
					options.m_loopCount = arguments[i+1];
				else
					Logging.logFatal("No loop count provided!");
				++i;
			}
			
			//help param
			else if (arguments[i].equalsIgnoreCase(PARAM_HELP) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_HELP) == true)
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
		
		ValidateOptions(options);
		
		try
		{
			CkTool.Execute(options);
		}
		catch (CSException in_exception)
		{
			Logging.logFatal(in_exception.getMessage());
		}

		Logging.finish();
	}
	/**
	 * Parses an action. If the given action string is not valid the app will terminate.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_actionString - The action in string form.
	 *
	 * @return The parsed action.
	 */
	private static Options.Action ParseAction(String in_actionString)
	{
		if (in_actionString.equalsIgnoreCase(ACTION_BUILD_BANK) == true)
		{
			return Options.Action.BUILD_BANK;
		}
		else if (in_actionString.equalsIgnoreCase(ACTION_BUILD_BANK_SINGLE) == true)
		{
			return Options.Action.BUILD_BANK_SINGLE;
		} 
		else if (in_actionString.equalsIgnoreCase(ACTION_BUILD_STREAM) == true)
		{
			return Options.Action.BUILD_STREAM;
		} 
		else
		{
			Logging.logFatal("Invalid action provided: " + in_actionString);
			return Options.Action.NONE;
		}
	}
	/**
	 * Confirms that the given options are valid. If they are not, the application will exit.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options.
	 */
	private static void ValidateOptions(Options in_options)
	{
		if (in_options.m_action == Options.Action.NONE)
		{
			Logging.logFatal("Must provide an action.");
		}
		
		if (in_options.m_inputFilePath.isEmpty() == true)
		{
			Logging.logFatal("Must provide an input file path.");
		}
		
		if (in_options.m_outputFilePath.isEmpty() == true)
		{
			Logging.logFatal("Must provide an output file path.");
		}
		
		if (in_options.m_action == Options.Action.BUILD_BANK && in_options.m_format.isEmpty() == false)
		{
			Logging.logFatal("Format cannot be applied to a bank created from a xml description.");
		}
		
		if (in_options.m_action == Options.Action.BUILD_BANK && in_options.m_volume.isEmpty() == false)
		{
			Logging.logFatal("Volume cannot be applied to a bank created from a xml description.");
		}
		
		if (in_options.m_action == Options.Action.BUILD_BANK && in_options.m_pan.isEmpty() == false)
		{
			Logging.logFatal("Pan cannot be applied to a bank created from a xml description.");
		}
		
		if (in_options.m_action == Options.Action.BUILD_BANK && in_options.m_loopStart.isEmpty() == false)
		{
			Logging.logFatal("Loop start cannot be applied to a bank created from a xml description.");
		}
		
		if (in_options.m_action == Options.Action.BUILD_BANK && in_options.m_loopEnd.isEmpty() == false)
		{
			Logging.logFatal("Loop end cannot be applied to a bank created from a xml description.");
		}
		
		if (in_options.m_action == Options.Action.BUILD_BANK && in_options.m_loopCount.isEmpty() == false)
		{
			Logging.logFatal("Loop count cannot be applied to a bank created from a xml description.");
		}
	}
	/**
	 * Prints the help text to the console. Forces the logging level to verbose.
	 * 
	 * @author Ian Copland
	 */
	private static void PrintHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.VERBOSE);
		Logging.logVerbose(" ");
		Logging.logVerbose("==========");
		Logging.logVerbose("  CkTool  ");
		Logging.logVerbose("==========");
		Logging.logVerbose(" ");
		Logging.logVerbose("CkTool is a simple cross-platform java wrapper around the Cricket Audio cktool. For further information see the Cricket");
		Logging.logVerbose("Audio Documentation at: http://www.crickettechnology.com/");
		Logging.logVerbose(" ");
		Logging.logVerbose("Chilli Source has obtained special permission to include Cricket Audio as part of the engine. See the documentation on CricketAudioSystem for " +
				"details on how to use Cricket Audio. You are free to use Cricket Audio as part of Chilli Source within your app subject to the Cricket Audio free license " +
				"(http://www.crickettechnology.com/free_license) as outline below:");
		Logging.logVerbose(" ");
		Logging.logVerbose("Cricket Audio free license");
		Logging.logVerbose(" ");
		Logging.logVerbose("Cricket Audio is available FREE in binary form for iOS, Android, Windows Phone 8, OS X, Windows, and Linux. ");
		Logging.logVerbose(" ");
		Logging.logVerbose("This free license has a few requirements:");
		Logging.logVerbose(" ");
		Logging.logVerbose("* Include the following message somewhere in your app, visible to users (for example, on a credits screen):");
		Logging.logVerbose(" ");
		Logging.logVerbose("                                      Built with Cricket Audio");
		Logging.logVerbose(" ");
		Logging.logVerbose("                                      www.crickettechnology.com");
		Logging.logVerbose(" ");
		Logging.logVerbose("* You may optionally include our logo (available in .svg or .png format).");
		Logging.logVerbose(" ");
		Logging.logVerbose("* Do not distribute our software to anyone, or post it for download on any site; refer them to our website instead.");
		Logging.logVerbose("  This license does not permit you to distribute Cricket Audio in an SDK, library, or other software development product. Contact us at " +
				"info@crickettechnology.com for additional licensing options.");
		Logging.logVerbose(" ");
		Logging.logVerbose("* If you release an app that uses Cricket Audio, you must let Cricket Technology know. Cricket Technology will include a link to it on our customers page.");
		Logging.logVerbose(" ");
		Logging.logVerbose("If you do not want to include the credit in your app, or if you do not want to notify us of your app's release, or if you want the source code, " +
				"you can purchase a source code license (http://www.crickettechnology.com/source_license).");
		Logging.logVerbose(" ");
		Logging.logVerbose("-------");
		Logging.logVerbose(" Usage");
		Logging.logVerbose("-------");
		Logging.logVerbose(" ");
		Logging.logVerbose("java -jar CkTool.jar " + PARAM_ACTION + " <action> " + PARAM_INPUT + " <filepath> " + PARAM_OUTPUT + " <filepath> [" + PARAM_FORMAT + " <format>] " +
				"[" + PARAM_VOLUME + " <volume>] [" + PARAM_PAN + " <pan>] [" + PARAM_LOOP_START + " <frame>] [" + PARAM_LOOP_END + " <frame>] [" + PARAM_LOOP_COUNT + " <count>] " +
				"[" + Logging.PARAM_LOGGING_LEVEL + " <level>] ["+ PARAM_HELP +"]");
		Logging.logVerbose(" ");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + PARAM_ACTION + "(" + SHORT_PARAM_ACTION + "): The action to perform.");
		Logging.logVerbose(" " + PARAM_INPUT + "(" + SHORT_PARAM_INPUT + "): The input filepath.");
		Logging.logVerbose(" " + PARAM_OUTPUT + "(" + SHORT_PARAM_OUTPUT + "): The output filepath.");
		Logging.logVerbose(" " + PARAM_FORMAT + "(" + SHORT_PARAM_FORMAT + "): The output format of the audio. This only applies to streams and banks created from a single audio file.");
		Logging.logVerbose(" " + PARAM_VOLUME + "(" + SHORT_PARAM_VOLUME + "): The output volume of the audio. This only applies to streams and banks created from a single audio file.");
		Logging.logVerbose(" " + PARAM_PAN + "(" + SHORT_PARAM_PAN + "): The output pan of the audio. This only applies to streams and banks created from a single audio file.");
		Logging.logVerbose(" " + PARAM_LOOP_START + "(" + SHORT_PARAM_LOOP_START + "): The start frame of a loop in the audio. This only applies to streams and banks created from a single audio file.");
		Logging.logVerbose(" " + PARAM_LOOP_END + "(" + SHORT_PARAM_LOOP_END + "): The end frame of a loop in the audio. This only applies to streams and banks created from a single audio file.");
		Logging.logVerbose(" " + PARAM_LOOP_COUNT + "(" + SHORT_PARAM_LOOP_COUNT + "): The number of times the audio will loop. This only applies to streams and banks created from a single audio file.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): Sets the logging level.");
		Logging.logVerbose(" " + PARAM_HELP + "p(" + SHORT_PARAM_HELP + "): Displays this help message.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Actions:");
		Logging.logVerbose(" " + ACTION_BUILD_BANK + ": Builds an audio bank (.ckb) from the given xml bank description file (.ckbx).");
		Logging.logVerbose(" " + ACTION_BUILD_BANK_SINGLE + ": Builds an audio bank (.ckb) which contains a single peice of audio from the given audio file.");
		Logging.logVerbose(" " + ACTION_BUILD_STREAM + ": Builds an audio stream (.cks) from the given audio file.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
	}
}
