/**
 * FontFromGlyphsBuilderOptionsParser.java
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

package com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder;

import java.util.ArrayList;
import java.util.List;

import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.texturepackerutils.TexturePacker.PlacementHeuristic;

/**
 * A container for methods relating to parsing font from glyphs builder 
 * options.
 * 
 * @author Ian Copland
 */
public final class FontFromGlyphsBuilderOptionsParser
{
	public static final String PARAM_FIXED_WIDTH = "--fixedwidth";
	public static final String PARAM_FIXED_HEIGHT = "--fixedheight";
	public static final String PARAM_VALID_WIDTHS = "--validwidths";
	public static final String PARAM_VALID_HEIGHTS = "--validheights";
	public static final String PARAM_MAX_WIDTH = "--maxwidth";
	public static final String PARAM_MAX_HEIGHT = "--maxheight";
	public static final String PARAM_DIVISIBLE_BY = "--divisibleby";
	public static final String PARAM_PLACEMENT_HEURISTIC = "--placementheuristic";
	public static final String PARAM_IMAGE_COMPRESSION = "--imagecompression";
	public static final String PARAM_IMAGE_FORMAT = "--imageformat";
	public static final String PARAM_DITHER_IMAGE = "--ditherimage";
	public static final String PARAM_DISABLE_PREMULTIPLIED_ALPHA = "--disablepremultipliedalpha";
	
	public static final String SHORT_PARAM_FIXED_WIDTH = "-fw";
	public static final String SHORT_PARAM_FIXED_HEIGHT = "-fh";
	public static final String SHORT_PARAM_VALID_WIDTHS = "-vw";
	public static final String SHORT_PARAM_VALID_HEIGHTS = "-vh";
	public static final String SHORT_PARAM_MAX_WIDTH = "-mw";
	public static final String SHORT_PARAM_MAX_HEIGHT = "-mh";
	public static final String SHORT_PARAM_DIVISIBLE_BY = "-db";
	public static final String SHORT_PARAM_PLACEMENT_HEURISTIC = "-ph";
	public static final String SHORT_PARAM_IMAGE_COMPRESSION = "-ic";
	public static final String SHORT_PARAM_IMAGE_FORMAT = "-if";
	public static final String SHORT_PARAM_DITHER_IMAGE = "-di";
	public static final String SHORT_PARAM_DISABLE_PREMULTIPLIED_ALPHA = "-dpa";
	
	public static final String PLACEMENT_HEURISTIC_TOP_LEFT = "topleft";
	public static final String PLACEMENT_HEURISTIC_BOTTOM_RIGHT = "bottomright";
	
	/**
	 * Processes all font from glyph mode options.
	 *
	 * @author Ian Copland
	 * 
	 * @param in_arguments - The arguments for Font From Glyph mode. 
	 * @param out_options - The output options.
	 * 
	 * @return The list of unused arguments.
	 */
	public static String[] parse(String[] in_arguments, FontFromGlyphsBuilderOptions out_options)
	{
		assert (in_arguments != null) : "Arguments cannot be null";
		assert (out_options != null) : "Output options cannot be null";
		
		List<String> unusedArguments = new ArrayList<>();
		
		for (int i = 0; i < in_arguments.length; ++i)
		{			
			//fixed width
			if (in_arguments[i].equalsIgnoreCase(PARAM_FIXED_WIDTH) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FIXED_WIDTH) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_fixedWidth = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No fixed width provided!");
				++i;
			}
			
			//fixed height
			else if (in_arguments[i].equalsIgnoreCase(PARAM_FIXED_HEIGHT) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_FIXED_HEIGHT) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_fixedHeight = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No fixed height provided!");
				++i;
			}
			
			//valid widths
			else if (in_arguments[i].equalsIgnoreCase(PARAM_VALID_WIDTHS) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_VALID_WIDTHS) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_validWidths = parseIntegerArray(in_arguments[i+1]);
				else
					Logging.logFatal("No valid widths provided!");
				++i;
			}
			
			//valid heights
			else if (in_arguments[i].equalsIgnoreCase(PARAM_VALID_HEIGHTS) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_VALID_HEIGHTS) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_validHeights = parseIntegerArray(in_arguments[i+1]);
				else
					Logging.logFatal("No valid heights provided!");
				++i;
			}
			
			//max width
			else if (in_arguments[i].equalsIgnoreCase(PARAM_MAX_WIDTH) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_MAX_WIDTH) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_maxWidth = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No max width provided!");
				++i;
			}
			
			//max height
			else if (in_arguments[i].equalsIgnoreCase(PARAM_MAX_HEIGHT) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_MAX_HEIGHT) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_maxHeight = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No max height provided!");
				++i;
			}
			
			//divisible by
			else if (in_arguments[i].equalsIgnoreCase(PARAM_DIVISIBLE_BY) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_DIVISIBLE_BY) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_divisibleBy = Integer.parseInt(in_arguments[i+1]);
				else
					Logging.logFatal("No divisible by value provided!");
				++i;
			}
			
			//packing heuristic
			else if (in_arguments[i].equalsIgnoreCase(PARAM_PLACEMENT_HEURISTIC) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_PLACEMENT_HEURISTIC) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_packingHeuristic = parsePlacementHeuristic(in_arguments[i+1]);
				else
					Logging.logFatal("No placement heuristic provided!");
				++i;
			}
			
			//image compression
			else if (in_arguments[i].equalsIgnoreCase(PARAM_IMAGE_COMPRESSION) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_IMAGE_COMPRESSION) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_imageCompression = in_arguments[i+1];
				else
					Logging.logFatal("No image compression provided!");
				++i;
			}
			
			//image format
			else if (in_arguments[i].equalsIgnoreCase(PARAM_IMAGE_FORMAT) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_IMAGE_FORMAT) == true)
			{
				if (i+1 < in_arguments.length)
					out_options.m_imageFormat = in_arguments[i+1];
				else
					Logging.logFatal("No image format provided!");
				++i;
			}
			
			//image dither
			else if (in_arguments[i].equalsIgnoreCase(PARAM_DITHER_IMAGE) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_DITHER_IMAGE) == true)
			{
				out_options.m_imageDither = true;
			}
			
			//image premultiply alpha
			else if (in_arguments[i].equalsIgnoreCase(PARAM_DISABLE_PREMULTIPLIED_ALPHA) == true || in_arguments[i].equalsIgnoreCase(SHORT_PARAM_DISABLE_PREMULTIPLIED_ALPHA) == true)
			{
				out_options.m_imagePremultiplyAlpha = false;
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
	 * Parses a comma separated list of integers into an array.
	 *
	 * @author Ian Copland
	 * 
	 * @param A string containing a comma separated list of
	 * integers.
	 * 
	 * @return An array containing the list of integers.
	 */
	private static int[] parseIntegerArray(String in_commaSeparatedList)
	{
		int[] integers = null;
		
		try
		{
			String[] stringIntegers = in_commaSeparatedList.split(",");
			integers = new int[stringIntegers.length];
			for (int i = 0; i < stringIntegers.length; ++i)
			{
				integers[i] = Integer.parseInt(stringIntegers[i]);
			}
		}
		catch (Exception e)
		{
			Logging.logFatal(StringUtils.convertExceptionToString(e));
		}
		
		return integers;
	}
	/**
	 * Parses the placement heuristic.
	 *
	 * @author Ian Copland
	 * 
	 * @param placement heuristic as a string.
	 * 
	 * @return The heuristic.
	 */
	private static PlacementHeuristic parsePlacementHeuristic(String in_packingHeuristic)
	{
		if (in_packingHeuristic.equalsIgnoreCase(PLACEMENT_HEURISTIC_TOP_LEFT) == true)
		{
			return PlacementHeuristic.TOPLEFT;
		}
		else if (in_packingHeuristic.equalsIgnoreCase(PLACEMENT_HEURISTIC_BOTTOM_RIGHT) == true)
		{
			return PlacementHeuristic.BOTTOMRIGHT;
		}
		else
		{
			Logging.logFatal("Invalid packing heuristic: " + in_packingHeuristic);
			return PlacementHeuristic.TOPLEFT;
		}
	}
}
