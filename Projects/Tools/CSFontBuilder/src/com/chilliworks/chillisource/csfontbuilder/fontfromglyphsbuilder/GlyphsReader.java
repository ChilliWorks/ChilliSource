/**
 * GlyphsReader.java
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

package com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.json.JSONObject;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;

/**
 * Provides a series of methods for reading bitmap font glyphs to file.
 * 
 * @author Ian Copland
 */
public final class GlyphsReader
{
	private static final String FONT_INFO_FILE_PATH = "FontInfo.csfontinfo";
	
	/**
	 * Reads bitmap glyph files and the Font Info file from the given 
	 * directory. 
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_inputDirectoryPath - The input directory containing the
	 * bitmap glyphs and font info file.
	 * 
	 * @return The glyphs container. This will never be null.
	 * 
	 * @throws CSException - An exception which provides a message describing
	 * the error which has occurred.
	 */
	public static Glyphs read(String in_inputDirectoryPath) throws CSException
	{
		String[] glyphFilePaths = findImageFilesInDirectory(in_inputDirectoryPath);
		char[] glyphCharacters = getCharacterForFilePaths(glyphFilePaths);
		return buildGlyphs(glyphCharacters, glyphFilePaths, in_inputDirectoryPath);
	}
	/**
	 * Retrieves the path to all files in the given directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_directoryPath - The directory to search for images in.
	 * 
	 * @return The array of file paths. The path will include the input
	 * directory path. This will never be null.
	 * 
	 * @throws CSException - An exception which provides a message describing
	 * the error which has occurred.
	 */
	private static String[] findImageFilesInDirectory(String in_directoryPath) throws CSException
	{
		File directory = new File(in_directoryPath);
		if (directory.exists() == false)
		{
			throw new CSException("Glyphs directory doesn't exist: " + in_directoryPath);
		}
		
		List<String> imageFilePaths = new ArrayList<>();
		
		String[] contents = directory.list();
		for (String itemPath : contents)
		{
			File item = new File(directory, itemPath);
			if (item.isFile() == true && itemPath.toLowerCase().endsWith(".png") == true)
			{
				imageFilePaths.add(item.getPath());
			}
		}
		
		return imageFilePaths.toArray(new String[0]);
	}
	/**
	 * Gets the character each image at the given file paths represents. This
	 * is parsed from the image file names, which is the unicode for the
	 * character.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_glyphFilePaths - The file paths of all the glyph images.
	 * 
	 * @return The array of characters. This will never be null.
	 */
	private static char[] getCharacterForFilePaths(String[] in_glyphFilePaths)
	{
		char[] characters = new char[in_glyphFilePaths.length];
		
		for (int i = 0; i < in_glyphFilePaths.length; ++i)
		{
			String glyphFilePath = in_glyphFilePaths[i];
			String glyphFileName = StringUtils.getFileName(glyphFilePath);
			String glyphFileRoot = StringUtils.removeExtension(glyphFileName);
			characters[i] = (char) Integer.parseInt(glyphFileRoot, 16);
		}
		
		return characters;
	}
	/**
	 * Builds the glyphs from the given data and the contents of the Font
	 * Info file in the given directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_characters - The list of glyph characters.
	 * @param in_filePaths - The list of glyph bitmap image file paths.
	 * @param in_inputDirectoryPath - The directory path that contains the 
	 * font info file.
	 * 
	 * @return A 4-tuple containing the font size, line height, descent and
	 * effect padding of the font. This will never be null.
	 * 
	 * @throws CSException - An exception which provides a message describing
	 * the error which has occurred.
	 */
	private static Glyphs buildGlyphs(char[] in_characters, String[] in_filePaths, String in_inputDirectoryPath) throws CSException
	{
		assert (in_characters.length == in_filePaths.length) : "Cannot build glyphs with different sized character and file path arrays.";
		
		String filePath = in_inputDirectoryPath + FONT_INFO_FILE_PATH;
		
		try
		{
			String fileContents = FileUtils.readFile(filePath);
			if (fileContents.length() == 0)
			{
				return null;
			}
			
			JSONObject jsonRoot = new JSONObject(fileContents);
			int fontSize = jsonRoot.getInt("FontSize");
			int lineHeight = jsonRoot.getInt("LineHeight");
			int descent = jsonRoot.getInt("Descent");
			int spaceAdvance = jsonRoot.getInt("SpaceAdvance");
			int verticalPadding = jsonRoot.getInt("VerticalPadding");
			
			JSONObject jsonGlyphs = jsonRoot.getJSONObject("Glyphs");
			Glyph[] glyphs = new Glyph[in_characters.length];
			for (int i = 0; i < in_characters.length; ++i)
			{
				char character = in_characters[i];
				JSONObject jsonGlyph = jsonGlyphs.getJSONObject("" + character);
				
				int origin = jsonGlyph.getInt("Origin");
				int advance = jsonGlyph.getInt("Advance");
				
				glyphs[i] = new Glyph(character, in_filePaths[i], origin, advance);
			}
			
			return new Glyphs(glyphs, fontSize, lineHeight, descent, spaceAdvance, verticalPadding);
		}
		catch (Exception e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			throw new CSException("Could not read the font info file: " + filePath, e);
		}
	}
}
