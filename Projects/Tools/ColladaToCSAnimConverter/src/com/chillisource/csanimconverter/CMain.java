package com.chillisource.csanimconverter;

import com.taggames.toolutils.SCLogger;
import com.taggames.toolutils.SCStringUtils;

public class CMain 
{
	/**
	 * Constants
	 */
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
		SCLogger.Initialise(inastrArgs);
		
		//check the number of arguments make sense.
		if (inastrArgs.length == 0)
		{
			PrintHelpText();
			return;
		}
		
		//gather up commands
		CSAnimConversionParameters params = new CSAnimConversionParameters();
		for (int i = 0; i < inastrArgs.length; ++i)
		{
			//input
			if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_INPUT) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_INPUT_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					params.mstrInputFilepath = SCStringUtils.StandardiseFilepath(inastrArgs[i+1]);
				else
					SCLogger.LogFatalError("No input file provided!");
				i++;
			}
			
			//output
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_OUTPUT) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_OUTPUT_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					params.mstrOutputFilepath = SCStringUtils.StandardiseFilepath(inastrArgs[i+1]);
				else
					SCLogger.LogFatalError("No output file provided!");
				i++;
			}
			
			//trasforms
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					ParseTransforms(params, inastrArgs[i+1]);
				else
					SCLogger.LogFatalError("No transforms provided!");
				i++;
			}
			
			//help
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_HELP) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_HELP_SHORT) == true)
			{
				PrintHelpText();
				return;
			}
		}
		
		
		CSAnimConverterTool converterTool = new CSAnimConverterTool();
		converterTool.Convert(params);
		
		SCLogger.PrintErrorsAndWarnings();
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
	SCLogger.LogMessage("Parameters:");
	SCLogger.LogMessage(" '" + PARAM_NAME_INPUT + "'('" + PARAM_NAME_INPUT_SHORT + "') -> The input filename.");
	SCLogger.LogMessage(" '" + PARAM_NAME_OUTPUT + "'('" + PARAM_NAME_OUTPUT_SHORT + "') -> The output filename.");
	SCLogger.LogMessage(" '" + PARAM_NAME_TRANSFORMS + "'('" + PARAM_NAME_TRANSFORMS_SHORT + "') -> A list of alterations to the output data.");
	SCLogger.LogMessage(" '--logginglevel'('-l') -> the level of messages to log.");
	SCLogger.LogMessage(" '--errordisplay'('-e') -> when to display errors.");
	SCLogger.LogMessage(" '" + PARAM_NAME_HELP + "'('" + PARAM_NAME_HELP_SHORT + "') -> Display this help message.");
	SCLogger.LogMessage("Transforms:");
	SCLogger.LogMessage(" '" + TRANSFORM_SWAPYANDZ + "' -> Z and Y coordinates of output data will be swapped.");
	SCLogger.LogMessage("Logging Levels:");
	SCLogger.LogMessage(" 'none' -> No logging.");
	SCLogger.LogMessage(" 'fatal' -> Only log fatal errors.");
	SCLogger.LogMessage(" 'error' -> Only log errors.");
	SCLogger.LogMessage(" 'warning' -> Log errors and warnings.");
	SCLogger.LogMessage(" 'verbose' -> Log all messages.");
	SCLogger.LogMessage("Error Display Options:");
	SCLogger.LogMessage(" 'never' -> Never display.");
	SCLogger.LogMessage(" 'atend' -> Log all errors at the end.");
	SCLogger.LogMessage(" 'whenreceived' -> Log all errors when received.");
}
}
