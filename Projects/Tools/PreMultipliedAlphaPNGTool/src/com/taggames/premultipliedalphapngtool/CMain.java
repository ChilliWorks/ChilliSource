package com.taggames.premultipliedalphapngtool;

import com.taggames.toolutils.SCLogger;

public class CMain 
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
		SCLogger.Initialise(inastrArgs);
		
		//check the number of arguments make sense.
		if (inastrArgs.length == 0)
		{
			PrintHelpText();
			return;
		}
		
		//gather up commands
		PremultiplicationOptions options = new PremultiplicationOptions();
		for (int i = 0; i < inastrArgs.length; ++i)
		{
			//Input param
			if (inastrArgs[i].equalsIgnoreCase("-i") == true || inastrArgs[i].equalsIgnoreCase("--input") == true)
			{
				if (i+1 < inastrArgs.length)
					options.strInputFilename = inastrArgs[i+1];
				else
					SCLogger.LogFatalError("No input file provided!");
				i++;
			}
			
			//output param
			if (inastrArgs[i].equalsIgnoreCase("-o") == true || inastrArgs[i].equalsIgnoreCase("--output") == true)
			{
				if (i+1 < inastrArgs.length)
					options.strOutputFilename = inastrArgs[i+1];
				else
					SCLogger.LogFatalError("No output file provided!");
				i++;
			}
			
			//cache param
			if (inastrArgs[i].equalsIgnoreCase("-c") == true || inastrArgs[i].equalsIgnoreCase("--cache") == true)
			{
				options.bShouldCache = true;
			}
			
			//restore param
			if (inastrArgs[i].equalsIgnoreCase("-r") == true || inastrArgs[i].equalsIgnoreCase("--restore") == true)
			{
				SCPngPremultiplier.RestoreFromCache();
				SCLogger.PrintErrorsAndWarnings();
				System.exit(0);
			}
			
			//directory param
			if (inastrArgs[i].equalsIgnoreCase("-d") == true || inastrArgs[i].equalsIgnoreCase("--directory") == true)
			{
				String strDirectory = new String();
				if (i+1 < inastrArgs.length)
					strDirectory = inastrArgs[i+1];
				else
					SCLogger.LogFatalError("No directory path provided!");
				i++;
				
				SCPngPremultiplier.PremultiplyDirectory(strDirectory);
				SCLogger.PrintErrorsAndWarnings();
				System.exit(0);
			}
			
			//help param
			if (inastrArgs[i].equalsIgnoreCase("-h") == true || inastrArgs[i].equalsIgnoreCase("--help") == true)
			{
				PrintHelpText();
				return;
			}
		}
		
		//confirm that the params are valid.
		if (options.strInputFilename == "")
			SCLogger.LogFatalError("No input filename provided.");
		if (options.strOutputFilename == "")
			SCLogger.LogFatalError("No output filename provided.");
		
		//now that the options have been gathered, perform the png pre-multiplying!
		SCPngPremultiplier.Premultiply(options);

		SCLogger.PrintErrorsAndWarnings();
	}
	//------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//------------------------------------------------------
	private static void PrintHelpText()
	{
		SCLogger.SetLoggingLevel(SCLogger.LOGGING_LEVEL_VERBOSE);
		SCLogger.LogMessage("java -jar PremultipliedAlphaPNGTool.jar [--input <filepath>] [--output <filepath>] [--cache] [--restore] [--directory <path>] [--logginglevel <level>] [--displayerrors <errordisplay>] [--help]");
		SCLogger.LogMessage("\t--input(-i) \t-> The input filepath.");
		SCLogger.LogMessage("\t--output(-o) \t-> The output filepath.");
		SCLogger.LogMessage("\t--cache(-c) \t-> Cache the original file before converting.");
		SCLogger.LogMessage("\t--restore(-r) \t-> Restore all cached files to their original locations.");
		SCLogger.LogMessage("\t--directory(-d) \t-> Convert and cache all files in a directory recursively.");
		SCLogger.LogMessage("\t--logginglevel(-l) \t-> Sets the logging level. Possible values are: 'none', 'fatal', 'error', 'warning', and 'verbose'. The default is value is 'warning'");
		SCLogger.LogMessage("\t--errordisplay(-e) \t-> Sets when errors should be displayed. Possible values are: 'never', 'atend', and 'whenreceived'. The default value is 'atend'");
		SCLogger.LogMessage("\t--help(-h) \t-> Displays this help message.");
	}
}
