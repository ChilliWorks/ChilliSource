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
	 * @return The glyphs container. This will be null if the read failed.
	 */
	public static Glyphs read(String in_inputDirectoryPath)
	{
		String[] glyphFilePaths = findImageFilesInDirectory(in_inputDirectoryPath);
		char[] glyphCharacters = getCharacterForFilePaths(glyphFilePaths);
		if (glyphCharacters == null)
		{
			return null;
		}
		
		return new Glyphs(glyphCharacters, glyphFilePaths, 0, 0, 0, null);
	}
	/**
	 * Retrieves the path to all files in the given directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_directoryPath - The directory to search for images in.
	 * 
	 * @return The array of file paths. The path will include the input
	 * directory path. 
	 */
	public static String[] findImageFilesInDirectory(String in_directoryPath)
	{
		File directory = new File(in_directoryPath);
		if (directory.exists() == false)
		{
			return null;
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
	 * @return The array of characters. Will return null if parsing the characters
	 * failed.
	 */
	public static char[] getCharacterForFilePaths(String[] in_glyphFilePaths)
	{
		char[] characters = new char[in_glyphFilePaths.length];
		
		//TODO: Continue here.
		
		return characters;
	}
}
