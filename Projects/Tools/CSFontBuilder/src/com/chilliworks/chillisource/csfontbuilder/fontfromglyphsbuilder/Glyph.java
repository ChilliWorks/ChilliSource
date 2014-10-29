/**
 * GlyphInfo.java
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

/**
 * A simple container for information on a single glyph.
 * 
 * @author Ian Copland
 */
public final class Glyph
{
	private final char m_character;
	private final String m_filePath;
	private final int m_origin;
	private final int m_advance;
	
	/**
	 * Constructor.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_character - The character the glyph represents.
	 * @param in_filePath - The file path to the bitmap glyph image.
	 * @param in_origin - The x position of the glyphs origin in the image.
	 * @param in_advance - The x advance from the origin that the next glyph
	 * should be rendered at.
	 * @param in_glyphSize - The glyph size.
	 */
	public Glyph(char in_character, String in_filePath, int in_origin, int in_advance)
	{
		m_character = in_character;
		m_filePath = in_filePath;
		m_origin = in_origin;
		m_advance = in_advance;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The character the glyph represents.
	 */
	public char getCharacter()
	{
		return m_character;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The path to the bitmap glyph image.
	 */
	public String getFilePath()
	{
		return m_filePath;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The x position of the glyphs origin in the image.
	 */
	public int getOrigin()
	{
		return m_origin;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The x advance from the origin that the next glyph
	 * should be rendered at.
	 */
	public int getAdvance()
	{
		return m_advance;
	}
}
