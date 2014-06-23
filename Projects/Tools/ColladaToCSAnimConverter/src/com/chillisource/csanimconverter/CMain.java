package com.chillisource.csanimconverter;

import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.StringUtils;

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
		Logging.start(inastrArgs);
		
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
					params.mstrInputFilepath = StringUtils.standardiseFilepath(inastrArgs[i+1]);
				else
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_OUTPUT) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_OUTPUT_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					params.mstrOutputFilepath = StringUtils.standardiseFilepath(inastrArgs[i+1]);
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//trasforms
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					ParseTransforms(params, inastrArgs[i+1]);
				else
					Logging.logFatal("No transforms provided!");
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
	Logging.logVerbose("Parameters:");
	Logging.logVerbose(" '" + PARAM_NAME_INPUT + "'('" + PARAM_NAME_INPUT_SHORT + "') -> The input filename.");
	Logging.logVerbose(" '" + PARAM_NAME_OUTPUT + "'('" + PARAM_NAME_OUTPUT_SHORT + "') -> The output filename.");
	Logging.logVerbose(" '" + PARAM_NAME_TRANSFORMS + "'('" + PARAM_NAME_TRANSFORMS_SHORT + "') -> A list of alterations to the output data.");
	Logging.logVerbose(" '--logginglevel'('-l') -> the level of messages to log.");
	Logging.logVerbose(" '--errordisplay'('-e') -> when to display errors.");
	Logging.logVerbose(" '" + PARAM_NAME_HELP + "'('" + PARAM_NAME_HELP_SHORT + "') -> Display this help message.");
	Logging.logVerbose("Transforms:");
	Logging.logVerbose(" '" + TRANSFORM_SWAPYANDZ + "' -> Z and Y coordinates of output data will be swapped.");
	Logging.logVerbose("Logging Levels:");
	Logging.logVerbose(" 'none' -> No logging.");
	Logging.logVerbose(" 'fatal' -> Only log fatal errors.");
	Logging.logVerbose(" 'error' -> Only log errors.");
	Logging.logVerbose(" 'warning' -> Log errors and warnings.");
	Logging.logVerbose(" 'verbose' -> Log all messages.");
	Logging.logVerbose("Error Display Options:");
	Logging.logVerbose(" 'never' -> Never display.");
	Logging.logVerbose(" 'atend' -> Log all errors at the end.");
	Logging.logVerbose(" 'whenreceived' -> Log all errors when received.");
}
}
