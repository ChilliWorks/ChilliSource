/**
 * GlyphsBuilderOptionsParser.java
 * Chilli Source
 * Created by Ian Copland on 22/10/2014.
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

package com.chilliworks.chillisource.csfontbuilder.glyphsbuilder;

import java.awt.Font;
import java.util.ArrayList;
import java.util.List;

import com.chilliworks.chillisource.coreutils.Colour;
import com.chilliworks.chillisource.coreutils.Integer2;
import com.chilliworks.chillisource.coreutils.Logging;

/**
 * A container for methods relating to parsing glyphs builder options.
 * 
 * @author Ian Copland
 */
public final class GlyphsBuilderOptionsParser
{
	public static final String PARAM_CHARACTERS = "--characters";
	public static final String PARAM_FONT_NAME = "--fontname";
	public static final String PARAM_FONT_STYLE = "--fontstyle";
	public static final String PARAM_FONT_SIZE = "--fontsize";
	public static final String PARAM_FONT_COLOUR = "--fontcolour";
	public static final String PARAM_ENABLE_DROP_SHADOW = "--enabledropshadow";
	public static final String PARAM_DROP_SHADOW_COLOUR = "--dropshadowcolour";
	public static final String PARAM_DROP_SHADOW_OFFSET = "--dropshadowoffset";
	public static final String PARAM_ENABLE_OUTLINE = "--enableoutline";
	public static final String PARAM_OUTLINE_COLOUR = "--outlinecolour";
	public static final String PARAM_OUTLINE_SIZE = "--outlinesize";
	public static final String PARAM_ENABLE_GLOW = "--enableglow";
	public static final String PARAM_GLOW_COLOUR = "--glowcolour";
	public static final String PARAM_GLOW_SIZE = "--glowsize";
	
	public static final String SHORT_PARAM_CHARACTERS = "-ch";
	public static final String SHORT_PARAM_FONT_NAME = "-fn";
	public static final String SHORT_PARAM_FONT_STYLE = "-fst";
	public static final String SHORT_PARAM_FONT_SIZE = "-fsi";
	public static final String SHORT_PARAM_FONT_COLOUR = "-fc";
	public static final String SHORT_PARAM_ENABLE_DROP_SHADOW = "-eds";
	public static final String SHORT_PARAM_DROP_SHADOW_COLOUR = "-dsc";
	public static final String SHORT_PARAM_DROP_SHADOW_OFFSET = "-dso";
	public static final String SHORT_PARAM_ENABLE_OUTLINE = "-eo";
	public static final String SHORT_PARAM_OUTLINE_COLOUR = "-oc";
	public static final String SHORT_PARAM_OUTLINE_SIZE = "-os";
	public static final String SHORT_PARAM_ENABLE_GLOW = "-eg";
	public static final String SHORT_PARAM_GLOW_COLOUR = "-gc";
	public static final String SHORT_PARAM_GLOW_SIZE = "-gs";
	
	public static final String FONT_STYLE_PLAIN = "plain";
	public static final String FONT_STYLE_BOLD = "bold";
	public static final String FONT_STYLE_ITALIC = "italic";
	
	/**
	 * Processes all glyph mode options.
	 *
	 * @author Ian Copland
	 * 
	 * @param in_arguments - The arguments for Glyphs mode. 
	 * @param out_options - The output options.
	 * 
	 * @return The unused argument list.
	 */
	public static String[] parse(String[] in_arguments, GlyphsBuilderOptions out_options)
	{
		assert (in_arguments != null) : "Arguments cannot be null";
		assert (out_options != null) : "Output options cannot be null";
		
		List<String> unusedArguments = new ArrayList<String>();

		for (int i = 0; i < in_arguments.length; ++i)
		{
			//characters
			if (in_arguments[i].equalsIgnoreCase(PARAM_CHARACTERS) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_CHARACTERS) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_characters = in_arguments[i+1];
				else
					Logging.logFatal("No character list provided.");
				i++;
			}
			
			//font name
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FONT_NAME) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FONT_NAME) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_fontName = in_arguments[i+1];
				else
					Logging.logFatal("No font name provided!");
				i++;
			}
			
			//font style
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FONT_STYLE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FONT_STYLE) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_fontStyle = parseFontStyle(in_arguments[i+1]);
				else
					Logging.logFatal("No font style provided!");
				i++;
			}
			
			//font size
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FONT_SIZE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FONT_SIZE) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_fontSize = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No font size provided!");
				i++;
			}
			
			//font colour
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FONT_COLOUR) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FONT_COLOUR) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_fontColour = Colour.parseColour(in_arguments[i+1]);
				else
					Logging.logFatal("No font colour provided!");
				i++;
			}
			
			//drop shadow
			else if (in_arguments[i].equalsIgnoreCase(PARAM_ENABLE_DROP_SHADOW) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_ENABLE_DROP_SHADOW) == true)
			{
				out_options.m_enableDropShadow = true;
			}
			
			//drop shadow colour
			else if (in_arguments[i].equalsIgnoreCase(PARAM_DROP_SHADOW_COLOUR) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_DROP_SHADOW_COLOUR) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_dropShadowColour = Colour.parseColour(in_arguments[i+1]);
				else
					Logging.logFatal("No drop shadow colour provided!");
				i++;
			}
			
			//drop shadow offset
			else if (in_arguments[i].equalsIgnoreCase(PARAM_DROP_SHADOW_OFFSET) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_DROP_SHADOW_OFFSET) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_dropShadowOffset = Integer2.parseInt2(in_arguments[i+1]);
				else
					Logging.logFatal("No drop shadow offset provided!");
				i++;
			}
			
			//outline
			else if (in_arguments[i].equalsIgnoreCase(PARAM_ENABLE_OUTLINE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_ENABLE_OUTLINE) == true)
			{
				out_options.m_enableOutline = true;
			}
			
			//outline colour
			else if (in_arguments[i].equalsIgnoreCase(PARAM_OUTLINE_COLOUR) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_OUTLINE_COLOUR) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_outlineColour = Colour.parseColour(in_arguments[i+1]);
				else
					Logging.logFatal("No outline colour provided!");
				i++;
			}
			
			//outline size
			else if (in_arguments[i].equalsIgnoreCase(PARAM_OUTLINE_SIZE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_OUTLINE_SIZE) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_outlineSize = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No outline size provided!");
				i++;
			}
			
			//glow
			else if (in_arguments[i].equalsIgnoreCase(PARAM_ENABLE_GLOW) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_ENABLE_GLOW) == true)
			{
				out_options.m_enableGlow = true;
			}
			
			//glow colour
			else if (in_arguments[i].equalsIgnoreCase(PARAM_GLOW_COLOUR) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_GLOW_COLOUR) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_glowColour = Colour.parseColour(in_arguments[i+1]);
				else
					Logging.logFatal("No glow colour provided!");
				i++;
			}
			
			//glow size
			else if (in_arguments[i].equalsIgnoreCase(PARAM_GLOW_SIZE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_GLOW_SIZE) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_glowSize = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No glow size provided!");
				i++;
			}
			
			//otherwise add to the unused arg list
			else
			{
				unusedArguments.add(in_arguments[i]);
			}
		}
		
		return unusedArguments.toArray(new String[0]);
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
}
