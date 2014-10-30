/**
 * GlyphsWriter.java
 * Chilli Source
 * Created by Ian Copland on 21/10/2014.
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

import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import org.json.JSONObject;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;

/**
 * Provides a series of methods for writing bitmap font glyphs to file.
 * 
 * @author Ian Copland
 */
public final class GlyphsWriter
{
	private static final String FONT_INFO_FILE_PATH = "FontInfo.csfontinfo";
	
	/**
	 * Saves the given glyphs to the output directory. This will write the bitmap images
	 * and a JSON file containing the information on the glyphs, such as font size,
	 * line height and the descent.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_glyphs - The glyphs that should be written to disk.
	 * @param in_outputDirectoryPath - The path to the output directory.
	 * 
	 * @throws CSException - An exception which provides a message describing the error 
	 * which has occurred.
	 */
	public static void write(Glyphs in_glyphs, String in_outputDirectoryPath) throws CSException
	{
		writeBitmapGlyphs(in_glyphs, in_outputDirectoryPath);
		writeFontInfo(in_glyphs, in_outputDirectoryPath);
	}
	/**
	 * Saves the output bitmap glyphs to the given directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_glyphs - The glyphs that should be written to disk.
	 * @param in_outputDirectoryPath - The path to the output directory.
	 * 
	 * @throws CSException - An exception which provides a message describing the error 
	 * which has occurred.
	 */
	private static void writeBitmapGlyphs(Glyphs in_glyphs, String in_outputDirectoryPath) throws CSException
	{
		try 
		{
			for (int i = 0; i < in_glyphs.getNumGlyphs(); ++i)
			{
				Glyph glyph = in_glyphs.getGlyph(i);
				String characterString = Integer.toHexString((int)glyph.getCharacter());
				String upperCharacterString = characterString.toUpperCase();
				
			    File outputfile = new File(in_outputDirectoryPath + upperCharacterString + ".png");
			    ImageIO.write(glyph.getImage(), "png", outputfile);
			}
		} 
		catch (IOException e) 
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			throw new CSException("Failed to save glyphs to directory: " + in_outputDirectoryPath, e);
		}
	}
	/**
	 * Writes the font into for the given glyphs to the given directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_glyphs - The glyphs whose info should be written to disk.
	 * @param in_outputDirectoryPath - The path to the output directory.
	 * 
	 * @throws CSException - An exception which provides a message describing
	 * the error which has occurred.
	 */
	private static void writeFontInfo(Glyphs in_glyphs, String in_outputDirectoryPath) throws CSException
	{
		JSONObject jsonRoot = new JSONObject();
		jsonRoot.put("FontSize", in_glyphs.getFontSize());
		jsonRoot.put("LineHeight", in_glyphs.getLineHeight());
		jsonRoot.put("Descent", in_glyphs.getDescent());
		jsonRoot.put("SpaceAdvance", in_glyphs.getSpaceAdvance());
		jsonRoot.put("VerticalPadding", in_glyphs.getVerticalPadding());
		
		JSONObject jsonGlyphs = new JSONObject();
		for (int i = 0; i < in_glyphs.getNumGlyphs(); ++i)
		{
			Glyph glyph = in_glyphs.getGlyph(i);
			JSONObject jsonGlyph = new JSONObject();
			jsonGlyph.put("Origin", glyph.getOrigin());
			jsonGlyph.put("Advance", glyph.getAdvance());
			jsonGlyphs.put("" + glyph.getCharacter(), jsonGlyph);
		}
		jsonRoot.put("Glyphs", jsonGlyphs);
		
		String jsonString = jsonRoot.toString(2);
		
		String outputFilePath = StringUtils.standardiseFilePath(in_outputDirectoryPath + FONT_INFO_FILE_PATH);
		if (FileUtils.writeFile(outputFilePath, jsonString) == false)
		{
			throw new CSException("Failed to write the font info file: " + outputFilePath);
		}
	}
}
