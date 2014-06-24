package com.chillisource.pngalphapremultiplier;

import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.Logging.LoggingLevel;

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
		if (options.strInputFilename == "")
			Logging.logFatal("No input filename provided.");
		if (options.strOutputFilename == "")
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
		Logging.setLoggingLevel(LoggingLevel.k_verbose);
		Logging.logVerbose("Usage: java -jar PNGAlphaPremultiplier.jar --input <filepath> --output <filepath> [--logginglevel <level>] [--displayerrors <errordisplay>] [--help]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" --input(-i): The input filepath.");
		Logging.logVerbose(" --output(-o): The output filepath.");
		Logging.logVerbose(" " + Logging.k_paramLoggingLevel + "(" + Logging.k_paramLoggingLevelShort + "): Sets the logging level.");
		Logging.logVerbose(" --help(-h): Displays this help message.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.k_loggingLevelNone + ": No logging.");
		Logging.logVerbose(" " + Logging.k_loggingLevelFatal + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelError + ": Only log errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelWarning + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.k_loggingLevelVerbose + ": Log all messages.");
	}
}
