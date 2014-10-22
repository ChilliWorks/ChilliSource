/**
 * Glyphs.java
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

import com.chilliworks.chillisource.toolutils.Integer2;

/**
 * A container for the glyphs that will used to generated a bitmap font.
 * 
 * @author Ian Copland
 */
public final class Glyphs
{
	private final char[] m_characters;
	private final String[] m_filePaths;
	private final int m_fontSize;
	private final int m_lineHeight;
	private final int m_descent;
	private final Integer2 m_effectPadding;
	
	/**
	 * Constructor. 
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_characters - The character for each glyph.
	 * @param in_filePaths - The file path for each glyph.
	 * @param in_fontSize - The point size of the font the glyphs were created from.
	 * @param in_lineHeight - The total height of a line of text.
	 * @param in_descent - The amount of space under the baseline in a line of text.
	 * @param in_effectPadding - The padding added to the image for effects such as a
	 * glow. This padding can be used to ensure correct placement when rendering the
	 * final images.
	 */
	public Glyphs(char[] in_characters, String[] in_filePaths, int in_fontSize, int in_lineHeight, int in_descent, Integer2 in_effectPadding)
	{
		assert (in_characters.length == in_filePaths.length) : "Character and file path arrays cannot be different sizes.";
		
		m_characters = in_characters;
		m_filePaths = in_filePaths;
		m_effectPadding = in_effectPadding;
		m_fontSize = in_fontSize;
		m_lineHeight = in_lineHeight;
		m_descent = in_descent;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The number of glyphs.
	 */
	public int getNumGlyphs()
	{
		return m_filePaths.length;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The character for each glyph.
	 */
	public char[] getCharacters()
	{
		return m_characters;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The file path for each glyph.
	 */
	public String[] getFilePaths()
	{
		return m_filePaths;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The point size of the font the glyphs were created from.
	 */
	public int getFontSize()
	{
		return m_fontSize;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The total height of a line of text.
	 */
	public int getLineHeight()
	{
		return m_lineHeight;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The amount of space under the baseline in a line of text.
	 */
	public int getDescent()
	{
		return m_descent;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The amount of padding added to the glyph images for effects such as 
	 * the glow. This padding can be used to ensure correct placement when rendering the
	 * final images.
	 */
	public Integer2 getEffectPadding()
	{
		return m_effectPadding;
	}
}
