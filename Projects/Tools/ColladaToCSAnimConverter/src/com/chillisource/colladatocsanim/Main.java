package com.chillisource.colladatocsanim;

import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.Logging.LoggingLevel;
import com.chillisource.toolutils.StringUtils;

public class Main 
{
	private static final String PARAM_NAME_INPUT 					= "--input";
	private static final String PARAM_NAME_OUTPUT 					= "--output";
	private static final String PARAM_NAME_TRANSFORMS				= "--transforms";
	private static final String PARAM_NAME_HELP						= "--help";
	private static final String PARAM_NAME_INPUT_SHORT 				= "-i";
	private static final String PARAM_NAME_OUTPUT_SHORT 			= "-o";
	private static final String PARAM_NAME_TRANSFORMS_SHORT			= "-t";
	private static final String PARAM_NAME_HELP_SHORT				= "-h";
	private static final char TRANSFORM_SWAPYANDZ					= 'y';
	
	/**
	 * Private members
	 */
	
	/**Main
	 * Entry point for the application.
	 */
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
		CSAnimConversionParameters params = new CSAnimConversionParameters();
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(PARAM_NAME_INPUT) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_INPUT_SHORT) == true)
			{
				if (i+1 < arguments.length)
					params.mstrInputFilepath = StringUtils.standardiseFilepath(arguments[i+1]);
				else
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_OUTPUT) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_OUTPUT_SHORT) == true)
			{
				if (i+1 < arguments.length)
					params.mstrOutputFilepath = StringUtils.standardiseFilepath(arguments[i+1]);
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//transforms
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS_SHORT) == true)
			{
				if (i+1 < arguments.length)
					ParseTransforms(params, arguments[i+1]);
				else
					Logging.logFatal("No transforms provided!");
				i++;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_HELP) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_HELP_SHORT) == true)
			{
				PrintHelpText();
				return;
			}
			
			//Failure
			else
			{
				Logging.logFatal("Invalid parameter found: " + arguments[i]);
			}
		}
		
		if (params.mstrInputFilepath.length() == 0 || params.mstrOutputFilepath.length() == 0)
		{
			Logging.logFatal("Must provide an input and output file path.");
		}
		
		CSAnimConverterTool converterTool = new CSAnimConverterTool();
		converterTool.Convert(params);
		
		Logging.finish();
	}
	//-------------------------------------------------------------------
	/// Parse Transforms
	///
	/// Parses a string of modifications.
	///
	/// @param the conversion params.
	/// @param the string to parse.
	//-------------------------------------------------------------------
	static void ParseTransforms(CSAnimConversionParameters inParams, String instrModifications)
	{
		for (int i = 0; i < instrModifications.length(); ++i)
		{
			if (instrModifications.charAt(i) == TRANSFORM_SWAPYANDZ)
				inParams.mbSwapYAndZ = true;
		}
	}
	//-------------------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//-------------------------------------------------------------------
	public static void PrintHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.k_verbose);
		Logging.logVerbose("Usage: java -jar ColladaToCSAnim.jar " + PARAM_NAME_INPUT + " <file path> " + PARAM_NAME_OUTPUT + " <file path> [" + PARAM_NAME_TRANSFORMS + " <transforms>] [" + PARAM_NAME_HELP + "] [" + Logging.k_paramLoggingLevel + " <level>]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + PARAM_NAME_INPUT + "(" + PARAM_NAME_INPUT_SHORT + "): The input filename.");
		Logging.logVerbose(" " + PARAM_NAME_OUTPUT + "(" + PARAM_NAME_OUTPUT_SHORT + "): The output filename.");
		Logging.logVerbose(" " + PARAM_NAME_TRANSFORMS + "(" + PARAM_NAME_TRANSFORMS_SHORT + "): [Optional] A list of alterations to the output data.");
		Logging.logVerbose(" " + Logging.k_paramLoggingLevel + "(" + Logging.k_paramLoggingLevelShort + "): [Optional] The level of messages to log.");
		Logging.logVerbose(" " + PARAM_NAME_HELP + "(" + PARAM_NAME_HELP_SHORT + "): [Optional] Display this help message.");
		Logging.logVerbose("Transforms:");
		Logging.logVerbose(" " + TRANSFORM_SWAPYANDZ + ": Z and Y coordinates of output data will be swapped.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.k_loggingLevelNone + ": No logging.");
		Logging.logVerbose(" " + Logging.k_loggingLevelFatal + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelError + ": Only log errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelWarning + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.k_loggingLevelVerbose + ": Log all messages.");
	}
}
