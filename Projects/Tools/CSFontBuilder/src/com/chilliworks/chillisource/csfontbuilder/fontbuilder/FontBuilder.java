/**
 * FontBuilder.java
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

package com.chilliworks.chillisource.csfontbuilder.fontbuilder;

import java.io.File;
import java.util.Random;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder.FontFromGlyphsBuilder;
import com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder.FontFromGlyphsBuilderOptions;
import com.chilliworks.chillisource.csfontbuilder.glyphsbuilder.GlyphsBuilder;
import com.chilliworks.chillisource.csfontbuilder.glyphsbuilder.GlyphsBuilderOptions;
 
/**
 * A static class containing methods for building CSFont bitmap fonts from
 * vector fonts in either TTF or OTF format.
 * 
 * @author Ian Copland
 */
public final class FontBuilder
{
	private static final String TEMP_DIRECTORY_PATH_PREFIX = "_temp-glyphs-";
	
	/**
	 * Builds a CSFont from a TTF or OTF vector font with the given options.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_outputFilePath - The output csfont file path.
	 * @param in_glyphBuilderOptions - The glyphs builder options.
	 * @param in_fontFromGlyphsBuilderOptions - The font from glyphs builder options.
	 * 
	 * @throws CSException - An exception which provides a message describing 
	 * the error which has occurred.
	 */
	public static void build(String in_outputFilePath, GlyphsBuilderOptions in_glyphBuilderOptions, FontFromGlyphsBuilderOptions in_fontFromGlyphsBuilderOptions) throws CSException
	{
		String randomHex = Long.toHexString(new Random().nextLong());
		String outputDirectoryPath = StringUtils.getDirectory(in_outputFilePath);
		String tempDirectoryPath = outputDirectoryPath + TEMP_DIRECTORY_PATH_PREFIX + randomHex + "/";
		
		createTempDirectory(tempDirectoryPath);
		
		try
		{
			GlyphsBuilder.build(tempDirectoryPath, in_glyphBuilderOptions);
			FontFromGlyphsBuilder.build(tempDirectoryPath, in_outputFilePath, in_fontFromGlyphsBuilderOptions);
		}
		catch (CSException e)
		{
			throw new CSException(e.getMessage(), e);
		}
		finally
		{
			deleteTempDirectory(tempDirectoryPath);
		}
	}
	/**
	 * Creates the temporary directory to create the output files in.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_tempDirectoryPath - The temporary directory path.
	 * 
	 * @throws CSException - An exception which provides a message 
	 * describing the error which has occurred.
	 */
	private static void createTempDirectory(String in_tempDirectoryPath) throws CSException
	{
		if (new File(in_tempDirectoryPath).exists() == true)
		{
			throw new CSException("Could not create temp directory as it already exists: " + in_tempDirectoryPath);
		}
		
		if (FileUtils.createDirectory(in_tempDirectoryPath) == false)
		{
			throw new CSException("Could not create temp directory: " + in_tempDirectoryPath);
		}
	}
	/**
	 * Deletes the temporary directory and its contents.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_tempDirectoryPath - The temporary directory path.
	 * 
	 * @throws CSException - An exception which provides a message 
	 * describing the error which has occurred.
	 */
	private static void deleteTempDirectory(String in_tempDirectoryPath) throws CSException
	{
		if (FileUtils.deleteDirectory(in_tempDirectoryPath) == false)
		{
			throw new CSException("Could not delete temp directory: " + in_tempDirectoryPath);
		}
	}
}
