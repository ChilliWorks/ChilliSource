/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 26/10/2012
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
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

package com.chilliworks.chillisource.tgatocsimage;

import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;

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
	public static void main(String inastrArgs[]) throws Exception 
	{	
		//setup the logger.
		String[] arguments = Logging.start(inastrArgs);
		
		//check the number of arguments make sense.
		if(arguments.length < 2)
		{
			PrintHelpText();
			return;
		}
		
		//Collect params
		TGAToCSImageOptions options = new TGAToCSImageOptions();
		for(int i = 0; i < arguments.length; ++i)
		{
			//input
			if(arguments[i].equalsIgnoreCase("-i") == true || arguments[i].equalsIgnoreCase("--input") == true)
			{
				if (i + 1 < arguments.length)
					options.strInputFilename = arguments[i + 1];
				else
					Logging.logFatal("No input path provided!");
				i++;
			}
			
			//output
			else if(arguments[i].equalsIgnoreCase("-o") == true || arguments[i].equalsIgnoreCase("--output") == true)
			{
				if (i + 1 < arguments.length)
					options.strOutputFilename = arguments[i + 1];
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//convert
			else if(arguments[i].equalsIgnoreCase("-ct") == true || arguments[i].equalsIgnoreCase("--convert") == true)
			{
				if (i + 1 < arguments.length)
					options.eConversionType = ConvertStringToConversionFormat(arguments[i + 1]);
				else
					Logging.logFatal("No conversion type provided!");
				i++;
			}
			
			//convertalpha
			else if(arguments[i].equalsIgnoreCase("-cta") == true || arguments[i].equalsIgnoreCase("--convertalpha") == true)
			{
				if (i + 1 < arguments.length)
					options.eConversionAlphaType = ConvertStringToConversionFormat(arguments[i + 1]);
				else
					Logging.logFatal("No conversion type provided!");
				i++;
			}
			
			//convertnoalpha
			else if(arguments[i].equalsIgnoreCase("-ctna") == true || arguments[i].equalsIgnoreCase("--convertnoalpha") == true)
			{
				if (i + 1 < arguments.length)
					options.eConversionNoAlphaType = ConvertStringToConversionFormat(arguments[i + 1]);
				else
					Logging.logFatal("No conversion type provided!");
				i++;
			}
			
			//compression
			else if(arguments[i].equalsIgnoreCase("-cn") == true || arguments[i].equalsIgnoreCase("--compression") == true)
			{
				if (i + 1 < arguments.length)
					options.eCompressionType = ConvertStringToCompressionFormat(arguments[i + 1]);
				else
					Logging.logFatal("No compression type provided!");
				i++;
			}
			
			//disable premultiplied alpha
			else if(arguments[i].equalsIgnoreCase("-dpa") == true || arguments[i].equalsIgnoreCase("--disablepremultipliedalpha") == true)
			{
				options.bPremultiply = false;
			}
			
			//dither
			else if(arguments[i].equalsIgnoreCase("-d") == true || arguments[i].equalsIgnoreCase("--dither") == true)
			{
				options.bDither = true;
			}

			//failure
			else
			{
				Logging.logFatal("Invalid parameter found: " + arguments[i]);
			}
		}
		
		// Confirm that the paramaters are valid.
		if(options.strInputFilename.equals(""))
			Logging.logFatal("No input path provided.");
		if (options.strOutputFilename.equals(""))
			Logging.logFatal("No output file provided.");
		
		TGAToCSImage.Run(options);

		Logging.finish();
	}
	//------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//------------------------------------------------------
	private static void PrintHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.VERBOSE);
		Logging.logVerbose("Usage: java -jar TGAToCSImage.jar --input <filename> --output <filename> [--convert <type>] [--convertalpha <type>] [--convertnoalpha <type>] [--compression <type>] [--disablepremultipliedalpha] [--dither] [" + Logging.PARAM_LOGGING_LEVEL + " <level>] [--help]");
		Logging.logVerbose(" --input(-i): The path to the source image TGA.");
		Logging.logVerbose(" --output(-o): The path to the output image csimage.");
		Logging.logVerbose(" --convert(-ct): [Optional] The type to convert to.");
		Logging.logVerbose(" --convertalpha(-cta): [Optional] The type to convert images with alpha to.");
		Logging.logVerbose(" --convertnoalpha(-ctna): [Optional] The type to convert images without alpha to.");
		Logging.logVerbose(" --compression(-cn): [Optional] The compression type. The default is zlib compression.");
		Logging.logVerbose(" --disablepremultipliedalpha(-dpa): [Optional] If set the output image will not have it's alpha premultiplied.");
		Logging.logVerbose(" --dither('-d'): [Optional] Whether or not to dither if converting to a smaller image format.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): [Optional] Sets the level of message to log.");
		Logging.logVerbose(" --help(-h): [Optional] Display this help message.");
		Logging.logVerbose("Conversion Types:");
		Logging.logVerbose(" L8");
		Logging.logVerbose(" LA88");
		Logging.logVerbose(" RGB565");
		Logging.logVerbose(" RGBA4444");
		Logging.logVerbose(" RGB888");
		Logging.logVerbose(" RGBA8888");
		Logging.logVerbose("Compression Types:");
		Logging.logVerbose(" None");
		Logging.logVerbose(" Default");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
	}
	//------------------------------------------------------
	/// Convert String To Conversion Format
	///
	/// @return the Conversion Format equivalent to the given
	/// string.
	//------------------------------------------------------
	public static TGAToCSImageOptions.OUTPUT_FORMAT ConvertStringToConversionFormat(String instrString)
	{
		if (instrString.equalsIgnoreCase("L8") == true)
			return TGAToCSImageOptions.OUTPUT_FORMAT.L8;
		if (instrString.equalsIgnoreCase("LA88") == true)
			return TGAToCSImageOptions.OUTPUT_FORMAT.LA88;
		if (instrString.equalsIgnoreCase("RGB565") == true)
			return TGAToCSImageOptions.OUTPUT_FORMAT.RGB565;
		if (instrString.equalsIgnoreCase("RGBA4444") == true)
			return TGAToCSImageOptions.OUTPUT_FORMAT.RGBA4444;
		if (instrString.equalsIgnoreCase("RGB888") == true)
			return TGAToCSImageOptions.OUTPUT_FORMAT.RGB888;
		if (instrString.equalsIgnoreCase("RGBA8888") == true)
			return TGAToCSImageOptions.OUTPUT_FORMAT.RGBA8888;
		
		Logging.logWarning("No valid conversion format found. Will default to the image's format.");
		return TGAToCSImageOptions.OUTPUT_FORMAT.NONE;
	}
	//------------------------------------------------------
	/// Convert String To Compression Format
	///
	/// @return the Conversion Format equivalent to the given
	/// string.
	//------------------------------------------------------
	public static TGAToCSImageOptions.COMPRESSION_FORMAT ConvertStringToCompressionFormat(String instrString)
	{
		if (instrString.equalsIgnoreCase("Default"))
			return TGAToCSImageOptions.COMPRESSION_FORMAT.DEFAULT_ZLIB;
		else if (instrString.equalsIgnoreCase("None"))
			return TGAToCSImageOptions.COMPRESSION_FORMAT.NONE;
		
		Logging.logWarning("No valid compression format found. Image will not be compressed.");
		return TGAToCSImageOptions.COMPRESSION_FORMAT.NONE;
	}
}
