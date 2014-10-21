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

import java.awt.Font;
import java.util.ArrayList;
import java.util.List;

import com.chilliworks.chillisource.csfontbuilder.glyphsbuilder.GlyphsBuilder;
import com.chilliworks.chillisource.csfontbuilder.glyphsbuilder.GlyphsBuilderOptions;
import com.chilliworks.chillisource.toolutils.Colour;
import com.chilliworks.chillisource.toolutils.Integer2;
import com.chilliworks.chillisource.toolutils.Logging;
import com.chilliworks.chillisource.toolutils.StringUtils;
import com.chilliworks.chillisource.toolutils.Logging.LoggingLevel;

/**
 * Handles the entry point into the application. This manages the parsing of the input parameters and
 * passes them on to the rest of the tool.
 * 
 * @author Ian Copland
 */
public final class Main
{
	private static final String PARAM_MODE = "--mode";
	private static final String PARAM_OUTPUT = "--output";
	private static final String PARAM_CHARACTERS = "--characters";
	private static final String PARAM_FONT_NAME = "--fontname";
	private static final String PARAM_FONT_STYLE = "--fontstyle";
	private static final String PARAM_FONT_SIZE = "--fontsize";
	private static final String PARAM_FONT_COLOUR = "--fontcolour";
	private static final String PARAM_ENABLE_DROP_SHADOW = "--enabledropshadow";
	private static final String PARAM_DROP_SHADOW_COLOUR = "--dropshadowcolour";
	private static final String PARAM_DROP_SHADOW_OFFSET = "--dropshadowoffset";
	private static final String PARAM_ENABLE_OUTLINE = "--enableoutline";
	private static final String PARAM_OUTLINE_COLOUR = "--outlinecolour";
	private static final String PARAM_OUTLINE_SIZE = "--outlinesize";
	private static final String PARAM_ENABLE_GLOW = "--enableglow";
	private static final String PARAM_GLOW_COLOUR = "--glowcolour";
	private static final String PARAM_GLOW_SIZE = "--glowsize";
	private static final String PARAM_HELP = "--help";
	private static final String SHORT_PARAM_MODE = "-m";
	private static final String SHORT_PARAM_OUTPUT = "-o";
	private static final String SHORT_PARAM_CHARACTERS = "-ch";
	private static final String SHORT_PARAM_FONT_NAME = "-fn";
	private static final String SHORT_PARAM_FONT_STYLE = "-fst";
	private static final String SHORT_PARAM_FONT_SIZE = "-fsi";
	private static final String SHORT_PARAM_FONT_COLOUR = "-fc";
	private static final String SHORT_PARAM_ENABLE_DROP_SHADOW = "-eds";
	private static final String SHORT_PARAM_DROP_SHADOW_COLOUR = "-dsc";
	private static final String SHORT_PARAM_DROP_SHADOW_OFFSET = "-dso";
	private static final String SHORT_PARAM_ENABLE_OUTLINE = "-eo";
	private static final String SHORT_PARAM_OUTLINE_COLOUR = "-oc";
	private static final String SHORT_PARAM_OUTLINE_SIZE = "-os";
	private static final String SHORT_PARAM_ENABLE_GLOW = "-eg";
	private static final String SHORT_PARAM_GLOW_COLOUR = "-gc";
	private static final String SHORT_PARAM_GLOW_SIZE = "-gs";
	private static final String SHORT_PARAM_HELP = "-h";
	private static final String MODE_FONT = "font";
	private static final String MODE_GLYPHS = "glyphs";
	private static final String MODE_FONT_FROM_GLYPHS = "fontfromglyphs";
	private static final String FONT_STYLE_PLAIN = "plain";
	private static final String FONT_STYLE_BOLD = "bold";
	private static final String FONT_STYLE_ITALIC = "italic";
	
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
		//TODO: !? 
		Logging.logFatal("Unimplemented: void processFontMode(String[] in_arguments)");
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
		//read the base arguments
		GlyphsBuilderOptions options = new GlyphsBuilderOptions();
		for (int i = 0; i < in_arguments.length; ++i)
		{
			//output
			if (in_arguments[i].equalsIgnoreCase(PARAM_OUTPUT) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_OUTPUT) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_outputDirectoryPath = StringUtils.standardiseDirectoryPath(in_arguments[i+1]);
				else
					Logging.logFatal("No output directory path provided!");
				i++;
			}
			
			//characters
			else if (in_arguments[i].equalsIgnoreCase(PARAM_CHARACTERS) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_CHARACTERS) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_characters = in_arguments[i+1];
				else
					Logging.logFatal("No character list provided.");
				i++;
			}
			
			//font name
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FONT_NAME) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FONT_NAME) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_fontName = in_arguments[i+1];
				else
					Logging.logFatal("No font name provided!");
				i++;
			}
			
			//font style
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FONT_STYLE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FONT_STYLE) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_fontStyle = parseFontStyle(in_arguments[i+1]);
				else
					Logging.logFatal("No font style provided!");
				i++;
			}
			
			//font size
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FONT_SIZE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FONT_SIZE) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_fontSize = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No font size provided!");
				i++;
			}
			
			//font colour
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FONT_COLOUR) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FONT_COLOUR) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_fontColour = Colour.parseColour(in_arguments[i+1]);
				else
					Logging.logFatal("No font colour provided!");
				i++;
			}
			
			//drop shadow
			else if (in_arguments[i].equalsIgnoreCase(PARAM_ENABLE_DROP_SHADOW) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_ENABLE_DROP_SHADOW) == true)
			{
				options.m_enableDropShadow = true;
			}
			
			//drop shadow colour
			else if (in_arguments[i].equalsIgnoreCase(PARAM_DROP_SHADOW_COLOUR) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_DROP_SHADOW_COLOUR) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_dropShadowColour = Colour.parseColour(in_arguments[i+1]);
				else
					Logging.logFatal("No drop shadow colour provided!");
				i++;
			}
			
			//drop shadow offset
			else if (in_arguments[i].equalsIgnoreCase(PARAM_DROP_SHADOW_OFFSET) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_DROP_SHADOW_OFFSET) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_dropShadowOffset = Integer2.parseInt2(in_arguments[i+1]);
				else
					Logging.logFatal("No drop shadow offset provided!");
				i++;
			}
			
			//outline
			else if (in_arguments[i].equalsIgnoreCase(PARAM_ENABLE_OUTLINE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_ENABLE_OUTLINE) == true)
			{
				options.m_enableOutline = true;
			}
			
			//outline colour
			else if (in_arguments[i].equalsIgnoreCase(PARAM_OUTLINE_COLOUR) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_OUTLINE_COLOUR) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_outlineColour = Colour.parseColour(in_arguments[i+1]);
				else
					Logging.logFatal("No outline colour provided!");
				i++;
			}
			
			//outline size
			else if (in_arguments[i].equalsIgnoreCase(PARAM_OUTLINE_SIZE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_OUTLINE_SIZE) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_outlineSize = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No outline size provided!");
				i++;
			}
			
			//glow
			else if (in_arguments[i].equalsIgnoreCase(PARAM_ENABLE_GLOW) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_ENABLE_GLOW) == true)
			{
				options.m_enableGlow = true;
			}
			
			//glow colour
			else if (in_arguments[i].equalsIgnoreCase(PARAM_GLOW_COLOUR) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_GLOW_COLOUR) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_glowColour = Colour.parseColour(in_arguments[i+1]);
				else
					Logging.logFatal("No glow colour provided!");
				i++;
			}
			
			//glow size
			else if (in_arguments[i].equalsIgnoreCase(PARAM_GLOW_SIZE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_GLOW_SIZE) == true)
			{
				if (i+1 < in_arguments.length)
					options.m_glowSize = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No glow size provided!");
				i++;
			}
			
			//Error
			else
			{
				Logging.logFatal("Invalid parameter for 'Glyphs' mode: " + in_arguments[i]);
			}
		}
		
		GlyphsBuilder.build(options);
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
		//TODO: !? 
		Logging.logFatal("Unimplemented: void processFontFromGlyphsMode(String[] in_arguments)");
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
	 * Parses a font style from string.
	 *
	 * @author Ian Copland
	 * 
	 * @param The string to parse.
	 * 
	 * @return The font style the string is describing.
	 */
	private static int parseFontStyle(String in_stringToParse)
	{
		if (in_stringToParse.equalsIgnoreCase(FONT_STYLE_PLAIN) == true)
		{
			return Font.PLAIN;
		}
		else if (in_stringToParse.equalsIgnoreCase(FONT_STYLE_BOLD) == true)
		{
			return Font.BOLD;
		}
		else if (in_stringToParse.equalsIgnoreCase(FONT_STYLE_ITALIC) == true)
		{
			return Font.ITALIC;
		}
		else
		{
			Logging.logFatal("Invalid font style provided: " + in_stringToParse);
			return Font.PLAIN;
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
		Logging.setLoggingLevel(LoggingLevel.k_verbose);
		Logging.logVerbose("Usage: java -jar CFontBuilder.jar [" + Logging.k_paramLoggingLevel + " <level>] [" + PARAM_HELP + "]");
		Logging.logVerbose("Base Parameters:");
		Logging.logVerbose("  " + PARAM_MODE + "(" + SHORT_PARAM_MODE + "): The mode the tool is run in. This defaults to Font mode.");
		Logging.logVerbose("  " + Logging.k_paramLoggingLevel + "(" + Logging.k_paramLoggingLevelShort + "): [Optional] The level of messages to log.");
		Logging.logVerbose("  " + PARAM_HELP + "(" + SHORT_PARAM_HELP + "): [Optional] Display this help message.");
		Logging.logVerbose(" Modes:");
		Logging.logVerbose("  " + MODE_FONT + ": Creates a CSFont file from the input ttf or oft font file.");
		Logging.logVerbose("  " + MODE_GLYPHS + ": Creates bitmap glyphs from the input ttf or oft font file.");
		Logging.logVerbose("  " + MODE_FONT_FROM_GLYPHS + ": Creates a CSFont file from the input bitmap glyphs.");
		Logging.logVerbose(" Logging Levels:");
		Logging.logVerbose("  " + Logging.k_loggingLevelNone + ": No logging.");
		Logging.logVerbose("  " + Logging.k_loggingLevelFatal + ": Only log fatal errors.");
		Logging.logVerbose("  " + Logging.k_loggingLevelError + ": Only log errors.");
		Logging.logVerbose("  " + Logging.k_loggingLevelWarning + ": Log errors and warnings.");
		Logging.logVerbose("  " + Logging.k_loggingLevelVerbose + ": Log all messages.");
		Logging.logVerbose("Font Mode Parameters:");
		Logging.logVerbose(" TODO ");
		Logging.logVerbose("Glyphs Mode Parameters:");
		Logging.logVerbose("  " + PARAM_OUTPUT + "(" + SHORT_PARAM_OUTPUT + "): The output directory where the bitmap glyphs should be placed.");
		Logging.logVerbose("  " + PARAM_CHARACTERS + "(" + SHORT_PARAM_CHARACTERS + "): [Optional] The list of characters that glyphs should be created for. Defaults to the full character set for english, french, italian, german and spanish.");
		Logging.logVerbose("  " + PARAM_FONT_NAME + "(" + SHORT_PARAM_FONT_NAME + "): [Optional] The name of the system font that should be used to create the bitmap glyphs. Defaults to Arial.");
		Logging.logVerbose("  " + PARAM_FONT_NAME + "(" + SHORT_PARAM_FONT_NAME + "): [Optional] The font style that should be used. Defaults to Plain.");
		Logging.logVerbose("  " + PARAM_FONT_SIZE + "(" + SHORT_PARAM_FONT_SIZE + "): [Optional] The point size of the output glyphs. Defaults to 12.");
		Logging.logVerbose("  " + PARAM_FONT_COLOUR + "(" + SHORT_PARAM_FONT_COLOUR + "): [Optional] The colour of the output glyphs. Defaults to white.");
		Logging.logVerbose("  " + PARAM_ENABLE_DROP_SHADOW + "(" + SHORT_PARAM_ENABLE_DROP_SHADOW + "): [Optional] Enables outputting the glyphs with a drop shadow.");
		Logging.logVerbose("  " + PARAM_DROP_SHADOW_COLOUR + "(" + SHORT_PARAM_DROP_SHADOW_COLOUR + "): [Optional] Sets the colour of the drop shadow. Does nothing if a drop shadow is not enabled. Defaults to grey.");
		Logging.logVerbose("  " + PARAM_DROP_SHADOW_OFFSET + "(" + SHORT_PARAM_DROP_SHADOW_OFFSET + "): [Optional] Sets the offset of the drop shadow. Does nothing if a drop shadow is not enabled. Defaults to (2, 2).");
		Logging.logVerbose("  " + PARAM_ENABLE_OUTLINE + "(" + SHORT_PARAM_ENABLE_OUTLINE + "): [Optional] Enables outputting the glyphs with an outline.");
		Logging.logVerbose("  " + PARAM_OUTLINE_COLOUR + "(" + SHORT_PARAM_OUTLINE_COLOUR + "): [Optional] Sets the colour of the outline. Does nothing if an outline is not enabled. Defaults to black.");
		Logging.logVerbose("  " + PARAM_OUTLINE_SIZE + "(" + SHORT_PARAM_OUTLINE_SIZE + "): [Optional] Sets the size of the outline. Does nothing if an outline is not enabled. Defaults to 1.");
		Logging.logVerbose("  " + PARAM_ENABLE_GLOW + "(" + SHORT_PARAM_ENABLE_GLOW + "): [Optional] Enables outputting the glyphs with a glow.");
		Logging.logVerbose("  " + PARAM_GLOW_COLOUR + "(" + SHORT_PARAM_GLOW_COLOUR + "): [Optional] Sets the colour of the glow. Does nothing if a glow is not enabled. Defaults to white.");
		Logging.logVerbose("  " + PARAM_GLOW_SIZE + "(" + SHORT_PARAM_GLOW_SIZE + "): [Optional] Sets the size of the glow. Does nothing if a glow is not enabled. Defaults to 3.");
		Logging.logVerbose(" Font Styles:");
		Logging.logVerbose("  " + FONT_STYLE_PLAIN + ": The regular style for the font.");
		Logging.logVerbose("  " + FONT_STYLE_BOLD + ": The bold style for the font.");
		Logging.logVerbose("  " + FONT_STYLE_ITALIC + ": The italic style for the font.");
		Logging.logVerbose("Font From Glyphs Mode Parameters:");
		Logging.logVerbose(" TODO ");
	}
}
