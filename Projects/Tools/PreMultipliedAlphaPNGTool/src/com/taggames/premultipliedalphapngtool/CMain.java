package com.taggames.premultipliedalphapngtool;

import com.chillisource.toolutils.Logging;

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
		Logging.start(inastrArgs);
		
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
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output param
			if (inastrArgs[i].equalsIgnoreCase("-o") == true || inastrArgs[i].equalsIgnoreCase("--output") == true)
			{
				if (i+1 < inastrArgs.length)
					options.strOutputFilename = inastrArgs[i+1];
				else
					Logging.logFatal("No output file provided!");
				i++;
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
			Logging.logFatal("No input filename provided.");
		if (options.strOutputFilename == "")
			Logging.logFatal("No output filename provided.");
		
		//now that the options have been gathered, perform the png pre-multiplying!
		SCPngPremultiplier.Premultiply(options);

		Logging.finish();
	}
	//------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//------------------------------------------------------
	private static void PrintHelpText()
	{
		Logging.setLoggingLevel(Logging.LOGGING_LEVEL_VERBOSE);
		Logging.logVerbose("java -jar PremultipliedAlphaPNGTool.jar --input <filepath> --output <filepath> [--logginglevel <level>] [--displayerrors <errordisplay>] [--help]");
		Logging.logVerbose("\t--input(-i) \t-> The input filepath.");
		Logging.logVerbose("\t--output(-o) \t-> The output filepath.");
		Logging.logVerbose("\t--logginglevel(-l) \t-> Sets the logging level. Possible values are: 'none', 'fatal', 'error', 'warning', and 'verbose'. The default is value is 'warning'");
		Logging.logVerbose("\t--errordisplay(-e) \t-> Sets when errors should be displayed. Possible values are: 'never', 'atend', and 'whenreceived'. The default value is 'atend'");
		Logging.logVerbose("\t--help(-h) \t-> Displays this help message.");
	}
}
