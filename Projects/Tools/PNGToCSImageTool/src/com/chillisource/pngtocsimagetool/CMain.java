/*
 *  CMain.java
 *  PNGUtilities
 *
 *  Created by Ian Copland on 26/10/2012
 *  
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

package com.chillisource.pngtocsimagetool;

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
	public static void main(String inastrArgs[]) throws Exception 
	{	
		//setup the logger.
		SCLogger.Initialise(inastrArgs);
		
		//check the number of arguments make sense.
		if(inastrArgs.length < 2)
		{
			PrintHelpText();
			return;
		}
		
		//Collect params
		PNGToCSImageOptions options = new PNGToCSImageOptions();
		for(int i = 0; i < inastrArgs.length; ++i)
		{
			//input
			if(inastrArgs[i].equalsIgnoreCase("-i") == true || inastrArgs[i].equalsIgnoreCase("--input") == true)
			{
				if (i + 1 < inastrArgs.length)
					options.strInputFilename = inastrArgs[i + 1];
				else
					SCLogger.LogFatalError("No input path provided!");
				i++;
			}
			
			//output
			if(inastrArgs[i].equalsIgnoreCase("-o") == true || inastrArgs[i].equalsIgnoreCase("--output") == true)
			{
				if (i + 1 < inastrArgs.length)
					options.strOutputFilename = inastrArgs[i + 1];
				else
					SCLogger.LogFatalError("No output file provided!");
				i++;
			}
			
			//convert
			if(inastrArgs[i].equalsIgnoreCase("-ct") == true || inastrArgs[i].equalsIgnoreCase("--convert") == true)
			{
				if (i + 1 < inastrArgs.length)
					options.eConversionType = ConvertStringToConversionFormat(inastrArgs[i + 1]);
				else
					SCLogger.LogFatalError("No conversion type provided!");
				i++;
			}
			
			//convertalpha
			if(inastrArgs[i].equalsIgnoreCase("-cta") == true || inastrArgs[i].equalsIgnoreCase("--convertalpha") == true)
			{
				if (i + 1 < inastrArgs.length)
					options.eConversionAlphaType = ConvertStringToConversionFormat(inastrArgs[i + 1]);
				else
					SCLogger.LogFatalError("No conversion type provided!");
				i++;
			}
			
			//convertnoalpha
			if(inastrArgs[i].equalsIgnoreCase("-ctna") == true || inastrArgs[i].equalsIgnoreCase("--convertnoalpha") == true)
			{
				if (i + 1 < inastrArgs.length)
					options.eConversionNoAlphaType = ConvertStringToConversionFormat(inastrArgs[i + 1]);
				else
					SCLogger.LogFatalError("No conversion type provided!");
				i++;
			}
			
			//compression
			if(inastrArgs[i].equalsIgnoreCase("-cn") == true || inastrArgs[i].equalsIgnoreCase("--compression") == true)
			{
				if (i + 1 < inastrArgs.length)
					options.eCompressionType = ConvertStringToCompressionFormat(inastrArgs[i + 1]);
				else
					SCLogger.LogFatalError("No compression type provided!");
				i++;
			}
			
			//premultiply
			if(inastrArgs[i].equalsIgnoreCase("-p") == true || inastrArgs[i].equalsIgnoreCase("--premultiply") == true)
			{
				options.bPremultiply = true;
			}
			
			//dither
			if(inastrArgs[i].equalsIgnoreCase("-d") == true || inastrArgs[i].equalsIgnoreCase("--dither") == true)
			{
				options.bDither = true;
			}
			
			//stats
			if(inastrArgs[i].equalsIgnoreCase("-s") == true || inastrArgs[i].equalsIgnoreCase("--stats") == true)
			{
				options.bStats = true;
			}
		}
		
		// Confirm that the paramaters are valid.
		if(options.strInputFilename.equals(""))
			SCLogger.LogFatalError("No input path provided.");
		if (options.strOutputFilename.equals(""))
			SCLogger.LogFatalError("No output file provided.");
		
		SCPNGToCSImage.Run(options);

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
		SCLogger.LogMessage("java -jar PNGToMoImage.jar --input <filename> --output <filename> [--convert <type>] [--compression <type>] [--premultiply] [--dither]");
		SCLogger.LogMessage("\t'--input'('-i')              The path to the source image PNG.");
		SCLogger.LogMessage("\t'--output'('-o')             The path to the output image csimage.");
		SCLogger.LogMessage("\t'--convert'('-ct')           (Optional) The type to convert to.");
		SCLogger.LogMessage("\t'--convertalpha'('-cta')     (Optional) The type to convert images with alpha to.");
		SCLogger.LogMessage("\t'--convertnoalpha'('-ctna')  (Optional) The type to convert images without alpha to.");
		SCLogger.LogMessage("\t'--compression'('-cn')       (Optional) The compression type.");
		SCLogger.LogMessage("\t'--premultiply'('-p')        (Optional) Whether or not to premultiply the output csimage.");
		SCLogger.LogMessage("\t'--dither'('-d')             (Optional) Whether or not to dither if converting to a smaller image format.");
		SCLogger.LogMessage("\t'--stats'('-s')             (Optional) Whether or not to log out stats.");
		SCLogger.LogMessage("Conversion Types:");
		SCLogger.LogMessage("\tL8");
		SCLogger.LogMessage("\tLA88");
		SCLogger.LogMessage("\tRGB565");
		SCLogger.LogMessage("\tRGBA4444");
		SCLogger.LogMessage("\tRGB888");
		SCLogger.LogMessage("\tRGBA8888");
		SCLogger.LogMessage("Compression Types:");
		SCLogger.LogMessage("\tNone");
		SCLogger.LogMessage("\tDefault");
	}
	//------------------------------------------------------
	/// Convert String To Conversion Format
	///
	/// @return the Conversion Format equivalent to the given
	/// string.
	//------------------------------------------------------
	public static PNGToCSImageOptions.OUTPUT_FORMAT ConvertStringToConversionFormat(String instrString)
	{
		if (instrString.equalsIgnoreCase("L8") == true)
			return PNGToCSImageOptions.OUTPUT_FORMAT.L8;
		if (instrString.equalsIgnoreCase("LA88") == true)
			return PNGToCSImageOptions.OUTPUT_FORMAT.LA88;
		if (instrString.equalsIgnoreCase("RGB565") == true)
			return PNGToCSImageOptions.OUTPUT_FORMAT.RGB565;
		if (instrString.equalsIgnoreCase("RGBA4444") == true)
			return PNGToCSImageOptions.OUTPUT_FORMAT.RGBA4444;
		if (instrString.equalsIgnoreCase("RGB888") == true)
			return PNGToCSImageOptions.OUTPUT_FORMAT.RGB888;
		if (instrString.equalsIgnoreCase("RGBA8888") == true)
			return PNGToCSImageOptions.OUTPUT_FORMAT.RGBA8888;
		
		SCLogger.LogWarning("No valid conversion format found. Will default to the image's format.");
		return PNGToCSImageOptions.OUTPUT_FORMAT.NONE;
	}
	//------------------------------------------------------
	/// Convert String To Compression Format
	///
	/// @return the Conversion Format equivalent to the given
	/// string.
	//------------------------------------------------------
	public static PNGToCSImageOptions.COMPRESSION_FORMAT ConvertStringToCompressionFormat(String instrString)
	{
		if (instrString.equalsIgnoreCase("Default"))
			return PNGToCSImageOptions.COMPRESSION_FORMAT.DEFAULT_ZLIB;
		
		SCLogger.LogWarning("No valid compression format found. Image will not be compressed.");
		return PNGToCSImageOptions.COMPRESSION_FORMAT.NONE;
	}
}
