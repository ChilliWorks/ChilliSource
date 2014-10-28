/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 08/10/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
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

package com.chilliworks.chillisource.csfontbuilder;

import java.util.ArrayList;
import java.util.List;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;
import com.chilliworks.chillisource.csfontbuilder.fontbuilder.FontBuilder;
import com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder.FontFromGlyphsBuilder;
import com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder.FontFromGlyphsBuilderOptions;
import com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder.FontFromGlyphsBuilderOptionsParser;
import com.chilliworks.chillisource.csfontbuilder.glyphsbuilder.GlyphsBuilder;
import com.chilliworks.chillisource.csfontbuilder.glyphsbuilder.GlyphsBuilderOptions;
import com.chilliworks.chillisource.csfontbuilder.glyphsbuilder.GlyphsBuilderOptionsParser;

/**
 * Handles the entry point into the application. This manages the parsing of the input parameters and
 * passes them on to the rest of the tool.
 * 
 * @author Ian Copland
 */
public final class Main
{
	private static final String PARAM_MODE = "--mode";
	private static final String PARAM_HELP = "--help";
	private static final String PARAM_INPUT = "--input";
	private static final String PARAM_OUTPUT = "--output";
	
	private static final String SHORT_PARAM_MODE = "-m";
	private static final String SHORT_PARAM_HELP = "-h";
	private static final String SHORT_PARAM_INPUT = "-i";
	private static final String SHORT_PARAM_OUTPUT = "-o";

	private static final String MODE_FONT = "font";
	private static final String MODE_GLYPHS = "glyphs";
	private static final String MODE_FONT_FROM_GLYPHS = "fontfromglyphs";
	
	/**
	 * An enum describing the possible modes that the tool can be run in.
	 * 
	 * @author Ian Copland
	 */
	private enum Mode
	{
		FONT,
		GLYPHS,
		FONT_FROM_GLYPHS
	}
	/**
	 * The entry point into the application
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_arguments - The input arguments.
	 */
	public static void main(String[] in_arguments) throws Exception 
	{
		String[] arguments = Logging.start(in_arguments);
		
		if (arguments.length == 0)
		{
			printHelpText();
			return;
		}
		
		//read the base arguments
		Mode mode = Mode.FONT;
		for (int i = 0; i < arguments.length; ++i)
		{
			//mode
			if (arguments[i].equalsIgnoreCase(PARAM_MODE) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_MODE) == true)
			{
				if (i+1 < arguments.length)
					mode = parseMode(arguments[i+1]);
				else
					Logging.logFatal("No mode provided!");
				i++;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(PARAM_HELP) == true || arguments[i].equalsIgnoreCase(SHORT_PARAM_HELP) == true)
			{
				printHelpText();
				return;
			}
		}
		
		//remove used commands
		arguments = stripBaseArguments(arguments);
		
		switch (mode)
		{
		case FONT:
			processFontMode(arguments);
			break;
		case GLYPHS:
			processGlyphsMode(arguments);
			break;
		case FONT_FROM_GLYPHS:
			processFontFromGlyphsMode(arguments);
			break;
		}
		
		Logging.finish();
	}
	/**
	 * Processes the font mode arguments and passes them onto the tool.
	 *
	 * @author Ian Copland
	 * 
	 * @param in_arguments - The arguments for Font mode. 
	 */
	private static void processFontMode(String[] in_arguments)
	{
		GlyphsBuilderOptions glyphsBuilderOptions = new GlyphsBuilderOptions();
		String[] remainingArguments = GlyphsBuilderOptionsParser.parse(in_arguments, glyphsBuilderOptions);
		
		FontFromGlyphsBuilderOptions fontFromGlyphsBuilderOptions = new FontFromGlyphsBuilderOptions();
		remainingArguments = FontFromGlyphsBuilderOptionsParser.parse(remainingArguments, fontFromGlyphsBuilderOptions);
		
		String outputFilePath = "";
		
		for (int i = 0; i < remainingArguments.length; ++i)
		{
			//output
			if (remainingArguments[i].equalsIgnoreCase(PARAM_OUTPUT) == true || remainingArguments[i].equalsIgnoreCase(SHORT_PARAM_OUTPUT) == true)
			{
				if (i+1 < remainingArguments.length)
					outputFilePath = StringUtils.standardiseFilePath(remainingArguments[i+1]);
				else
					Logging.logFatal("No output directory path provided!");
				i++;
			}
			
			//Error
			else
			{
				Logging.logFatal("Invalid parameter for 'Font' mode: " + remainingArguments[i]);
			}
		}
		
		if (outputFilePath.length() == 0)
		{
			Logging.logFatal("An output directory path must be specified.");
		}
		
		try
		{
			FontBuilder.build(outputFilePath, glyphsBuilderOptions, fontFromGlyphsBuilderOptions);
		}
		catch (CSException e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			Logging.logFatal(e.getMessage());
		}
	}
	/**
	 * Processes the glyphs mode arguments and passes them onto the tool.
	 *
	 * @author Ian Copland
	 * 
	 * @param in_arguments - The arguments for Glyphs mode. 
	 */
	private static void processGlyphsMode(String[] in_arguments)
	{
		GlyphsBuilderOptions options = new GlyphsBuilderOptions();
		String[] remainingArguments = GlyphsBuilderOptionsParser.parse(in_arguments, options);
		
		String outputDirectoryPath = "";
		
		for (int i = 0; i < remainingArguments.length; ++i)
		{
			//output
			if (remainingArguments[i].equalsIgnoreCase(PARAM_OUTPUT) == true || remainingArguments[i].equalsIgnoreCase(SHORT_PARAM_OUTPUT) == true)
			{
				if (i+1 < remainingArguments.length)
					outputDirectoryPath = StringUtils.standardiseDirectoryPath(remainingArguments[i+1]);
				else
					Logging.logFatal("No output directory path provided!");
				i++;
			}
			
			//Error
			else
			{
				Logging.logFatal("Invalid parameter for 'Glyphs' mode: " + remainingArguments[i]);
			}
		}
		
		if (outputDirectoryPath.length() == 0)
		{
			Logging.logFatal("An output directory path must be specfied.");
		}
		
		try
		{
			GlyphsBuilder.build(outputDirectoryPath, options);
		}
		catch (CSException e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			Logging.logFatal(e.getMessage());
		}
	}
	/**
	 * Processes the font from glyphs mode arguments and passes them onto the tool.
	 *
	 * @author Ian Copland
	 * 
	 * @param in_arguments - The arguments for Font From Gylphs mode. 
	 */
	private static void processFontFromGlyphsMode(String[] in_arguments)
	{
		FontFromGlyphsBuilderOptions options = new FontFromGlyphsBuilderOptions();
		String[] remainingArguments = FontFromGlyphsBuilderOptionsParser.parse(in_arguments, options);
		
		String inputDirectoryPath = "";
		String outputFilePath = "";
		
		for (int i = 0; i < remainingArguments.length; ++i)
		{
			//input
			if (remainingArguments[i].equalsIgnoreCase(PARAM_INPUT) == true || remainingArguments[i].equalsIgnoreCase(SHORT_PARAM_INPUT) == true)
			{
				if (i+1 < remainingArguments.length)
					inputDirectoryPath = StringUtils.standardiseDirectoryPath(remainingArguments[i+1]);
				else
					Logging.logFatal("No input directory path provided!");
				i++;
			}
			
			//output
			else if (remainingArguments[i].equalsIgnoreCase(PARAM_OUTPUT) == true || remainingArguments[i].equalsIgnoreCase(SHORT_PARAM_OUTPUT) == true)
			{
				if (i+1 < remainingArguments.length)
					outputFilePath = StringUtils.standardiseFilePath(remainingArguments[i+1]);
				else
					Logging.logFatal("No output file path provided!");
				i++;
			}
			
			//Error
			else
			{
				Logging.logFatal("Invalid parameter for 'Glyphs' mode: " + remainingArguments[i]);
			}
		}
		
		try
		{
			FontFromGlyphsBuilder.build(inputDirectoryPath, outputFilePath, options);
		}
		catch (CSException e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			Logging.logFatal(e.getMessage());
		}
	}
	/**
	 * Strips the base arguments from the argument list.
	 *
	 * @author Ian Copland
	 * 
	 * @param The string to parse.
	 * 
	 * @return The mode the string is describing.
	 */
	private static String[] stripBaseArguments(String[] in_arguments)
	{
		List<String> output = new ArrayList<>();
		for (int i = 0; i < in_arguments.length; ++i)
		{
			if (in_arguments[i].equalsIgnoreCase(PARAM_MODE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_MODE) == true)
			{
				++i;
				continue;
			}
			else if (in_arguments[i].equalsIgnoreCase(PARAM_HELP) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_HELP) == true)
			{
				continue;
			}
			else
			{
				output.add(in_arguments[i]);
			}
		}
		return output.toArray(new String[0]);
	}
	/**
	 * Parses a mode from the given string.
	 *
	 * @author Ian Copland
	 * 
	 * @param The string to parse.
	 * 
	 * @return The mode the string is describing.
	 */
	private static Mode parseMode(String in_stringToParse)
	{
		if (in_stringToParse.equalsIgnoreCase(MODE_FONT) == true)
		{
			return Mode.FONT;
		}
		else if (in_stringToParse.equalsIgnoreCase(MODE_GLYPHS) == true)
		{
			return Mode.GLYPHS;
		}
		else if (in_stringToParse.equalsIgnoreCase(MODE_FONT_FROM_GLYPHS) == true)
		{
			return Mode.FONT_FROM_GLYPHS;
		}
		else
		{
			Logging.logFatal("Invalid mode provided: " + in_stringToParse);
			return Mode.FONT;
		}
	}
	/**
	 * Outputs help test for this tool. This will be displayed regardless of 
	 * the current logging level.
	 *
	 * @author Ian Copland
	 */
	private static void printHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.VERBOSE);
		Logging.logVerbose("Usage: java -jar CFontBuilder.jar [" + Logging.PARAM_LOGGING_LEVEL + " <level>] [" + PARAM_HELP + "] [" + PARAM_MODE + " <mode>] <mode specific parameters>");
		Logging.logVerbose(" ");
		Logging.logVerbose("Base Parameters:");
		Logging.logVerbose(" " + PARAM_MODE + "(" + SHORT_PARAM_MODE + "): The mode the tool is run in. This defaults to Font mode.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): [Optional] The level of messages to log.");
		Logging.logVerbose(" " + PARAM_HELP + "(" + SHORT_PARAM_HELP + "): [Optional] Display this help message.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Font Mode Parameters:");
		Logging.logVerbose(" " + PARAM_OUTPUT + "(" + SHORT_PARAM_OUTPUT + "): The output file path of the CSFont file.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_CHARACTERS + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_CHARACTERS + "): [Optional] The list of characters that glyphs should be created for. Defaults to the full character set for english, french, italian, german and spanish.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_FONT_NAME + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_FONT_NAME + "): [Optional] The name of the system font that should be used to create the bitmap glyphs. Defaults to Arial.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_FONT_STYLE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_FONT_STYLE + "): [Optional] The font style that should be used. Defaults to Plain.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_FONT_SIZE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_FONT_SIZE + "): [Optional] The point size of the output glyphs. Defaults to 12.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_FONT_COLOUR + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_FONT_COLOUR + "): [Optional] The colour of the output glyphs. Defaults to white.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_ENABLE_DROP_SHADOW + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_ENABLE_DROP_SHADOW + "): [Optional] Enables outputting the glyphs with a drop shadow.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_DROP_SHADOW_COLOUR + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_DROP_SHADOW_COLOUR + "): [Optional] Sets the colour of the drop shadow. Does nothing if a drop shadow is not enabled. Defaults to grey.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_DROP_SHADOW_OFFSET + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_DROP_SHADOW_OFFSET + "): [Optional] Sets the offset of the drop shadow. Does nothing if a drop shadow is not enabled. Defaults to (2, 2).");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_ENABLE_OUTLINE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_ENABLE_OUTLINE + "): [Optional] Enables outputting the glyphs with an outline.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_OUTLINE_COLOUR + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_OUTLINE_COLOUR + "): [Optional] Sets the colour of the outline. Does nothing if an outline is not enabled. Defaults to black.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_OUTLINE_SIZE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_OUTLINE_SIZE + "): [Optional] Sets the size of the outline. Does nothing if an outline is not enabled. Defaults to 1.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_ENABLE_GLOW + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_ENABLE_GLOW + "): [Optional] Enables outputting the glyphs with a glow.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_GLOW_COLOUR + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_GLOW_COLOUR + "): [Optional] Sets the colour of the glow. Does nothing if a glow is not enabled. Defaults to white.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_GLOW_SIZE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_GLOW_SIZE + "): [Optional] Sets the size of the glow. Does nothing if a glow is not enabled. Defaults to 3.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_FIXED_WIDTH + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_FIXED_WIDTH + "): [Optional] Forces the output font image to have the given width.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_FIXED_HEIGHT + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_FIXED_HEIGHT + "): [Optional] Forces the output font image to have the given height.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_VALID_WIDTHS + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_VALID_WIDTHS + "): [Optional] A comma separated list of valid output widths.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_VALID_HEIGHTS + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_VALID_HEIGHTS + "): [Optional] A comma separated list of valid output heights.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_MAX_WIDTH + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_MAX_WIDTH + "): [Optional] The maximum possible width for the output font image.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_MAX_HEIGHT + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_MAX_HEIGHT + "): [Optional] The maximum possible height for the output font image.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_DIVISIBLE_BY + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_DIVISIBLE_BY + "): [Optional] The output image will dimensions divisible by the given value.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_PLACEMENT_HEURISTIC + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_PLACEMENT_HEURISTIC + "): [Optional] The heuristic to use when packing characters.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_IMAGE_COMPRESSION + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_IMAGE_COMPRESSION + "): [Optional] The compression used for the output font image.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_IMAGE_FORMAT + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_IMAGE_FORMAT + "): [Optional] The format of the output font image.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_IMAGE_FORMAT + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_IMAGE_FORMAT + "): [Optional] If set the output font image will be dithered.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_DISABLE_PREMULTIPLIED_ALPHA + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_DISABLE_PREMULTIPLIED_ALPHA + "): [Optional] If set the output font image will not have its alpha premultiplied.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Glyphs Mode Parameters:");
		Logging.logVerbose(" " + PARAM_OUTPUT + "(" + SHORT_PARAM_OUTPUT + "): The output directory where the bitmap glyphs should be placed.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_CHARACTERS + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_CHARACTERS + "): [Optional] The list of characters that glyphs should be created for. Defaults to the full character set for english, french, italian, german and spanish.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_FONT_NAME + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_FONT_NAME + "): [Optional] The name of the system font that should be used to create the bitmap glyphs. Defaults to Arial.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_FONT_STYLE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_FONT_STYLE + "): [Optional] The font style that should be used. Defaults to Plain.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_FONT_SIZE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_FONT_SIZE + "): [Optional] The point size of the output glyphs. Defaults to 12.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_FONT_COLOUR + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_FONT_COLOUR + "): [Optional] The colour of the output glyphs. Defaults to white.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_ENABLE_DROP_SHADOW + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_ENABLE_DROP_SHADOW + "): [Optional] Enables outputting the glyphs with a drop shadow.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_DROP_SHADOW_COLOUR + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_DROP_SHADOW_COLOUR + "): [Optional] Sets the colour of the drop shadow. Does nothing if a drop shadow is not enabled. Defaults to grey.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_DROP_SHADOW_OFFSET + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_DROP_SHADOW_OFFSET + "): [Optional] Sets the offset of the drop shadow. Does nothing if a drop shadow is not enabled. Defaults to (2, 2).");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_ENABLE_OUTLINE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_ENABLE_OUTLINE + "): [Optional] Enables outputting the glyphs with an outline.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_OUTLINE_COLOUR + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_OUTLINE_COLOUR + "): [Optional] Sets the colour of the outline. Does nothing if an outline is not enabled. Defaults to black.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_OUTLINE_SIZE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_OUTLINE_SIZE + "): [Optional] Sets the size of the outline. Does nothing if an outline is not enabled. Defaults to 1.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_ENABLE_GLOW + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_ENABLE_GLOW + "): [Optional] Enables outputting the glyphs with a glow.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_GLOW_COLOUR + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_GLOW_COLOUR + "): [Optional] Sets the colour of the glow. Does nothing if a glow is not enabled. Defaults to white.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.PARAM_GLOW_SIZE + "(" + GlyphsBuilderOptionsParser.SHORT_PARAM_GLOW_SIZE + "): [Optional] Sets the size of the glow. Does nothing if a glow is not enabled. Defaults to 3.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Font From Glyphs Mode Parameters:");
		Logging.logVerbose(" " + PARAM_INPUT + "(" + SHORT_PARAM_INPUT + "): The input directory path containing the bitmap glyphs and the FontInfo.csfontinfo file.");
		Logging.logVerbose(" " + PARAM_OUTPUT + "(" + SHORT_PARAM_OUTPUT + "): The output file path of the CSFont file.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_FIXED_WIDTH + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_FIXED_WIDTH + "): [Optional] Forces the output font image to have the given width.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_FIXED_HEIGHT + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_FIXED_HEIGHT + "): [Optional] Forces the output font image to have the given height.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_VALID_WIDTHS + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_VALID_WIDTHS + "): [Optional] A comma separated list of valid output widths.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_VALID_HEIGHTS + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_VALID_HEIGHTS + "): [Optional] A comma separated list of valid output heights.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_MAX_WIDTH + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_MAX_WIDTH + "): [Optional] The maximum possible width for the output font image.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_MAX_HEIGHT + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_MAX_HEIGHT + "): [Optional] The maximum possible height for the output font image.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_DIVISIBLE_BY + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_DIVISIBLE_BY + "): [Optional] The output image will dimensions divisible by the given value.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_PLACEMENT_HEURISTIC + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_PLACEMENT_HEURISTIC + "): [Optional] The heuristic to use when packing characters.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_IMAGE_COMPRESSION + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_IMAGE_COMPRESSION + "): [Optional] The compression used for the output font image.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_IMAGE_FORMAT + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_IMAGE_FORMAT + "): [Optional] The format of the output font image.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_IMAGE_FORMAT + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_IMAGE_FORMAT + "): [Optional] If set the output font image will be dithered.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PARAM_DISABLE_PREMULTIPLIED_ALPHA + "(" + FontFromGlyphsBuilderOptionsParser.SHORT_PARAM_DISABLE_PREMULTIPLIED_ALPHA + "): [Optional] If set the output font image will not have its alpha premultiplied.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Modes:");
		Logging.logVerbose(" " + MODE_FONT + ": Creates a CSFont file from the input ttf or oft font file.");
		Logging.logVerbose(" " + MODE_GLYPHS + ": Creates bitmap glyphs from the input ttf or oft font file.");
		Logging.logVerbose(" " + MODE_FONT_FROM_GLYPHS + ": Creates a CSFont file from the input bitmap glyphs.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Font Styles:");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.FONT_STYLE_PLAIN + ": The regular style for the font.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.FONT_STYLE_BOLD + ": The bold style for the font.");
		Logging.logVerbose(" " + GlyphsBuilderOptionsParser.FONT_STYLE_ITALIC + ": The italic style for the font.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Placement Heuristics:");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PLACEMENT_HEURISTIC_TOP_LEFT + ": Place top left.");
		Logging.logVerbose(" " + FontFromGlyphsBuilderOptionsParser.PLACEMENT_HEURISTIC_BOTTOM_RIGHT + ": Place bottom right.");
		Logging.logVerbose(" ");
		Logging.logVerbose("Image Formats:");
		Logging.logVerbose(" L8");
		Logging.logVerbose(" LA88");
		Logging.logVerbose(" RGB565");
		Logging.logVerbose(" RGBA4444");
		Logging.logVerbose(" RGB888");
		Logging.logVerbose(" RGBA8888");
		Logging.logVerbose(" ");
		Logging.logVerbose("Compression Types:");
		Logging.logVerbose(" None");
		Logging.logVerbose(" Default");
	}
}
