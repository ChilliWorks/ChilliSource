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

/**
 * A container for the glyphs that will used to generated a bitmap font.
 * 
 * @author Ian Copland
 */
public final class Glyphs
{
	private final Glyph[] m_glyphs;
	private final int m_fontSize;
	private final int m_lineHeight;
	private final int m_descent;
	private final int m_spaceAdvance;
	private final int m_verticalPadding;
	
	/**
	 * Constructor. 
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_glyphs - The glyphs.
	 * @param in_fontSize - The point size of the font the glyphs were created from.
	 * @param in_lineHeight - The total height of a line of text.
	 * @param in_descent - The amount of space under the baseline in a line of text.
	 * @param in_spaceAdvance - The advance for a space character.
	 * @param in_verticalPadding - The amount of padding at the top and bottom of a
	 * glyph.
	 */
	public Glyphs(Glyph[] in_glyphs, int in_fontSize, int in_lineHeight, int in_descent, int in_spaceAdvance, int in_verticalPadding)
	{
		m_glyphs = in_glyphs;
		m_fontSize = in_fontSize;
		m_lineHeight = in_lineHeight;
		m_descent = in_descent;
		m_spaceAdvance = in_spaceAdvance;
		m_verticalPadding = in_verticalPadding;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The number of glyphs.
	 */
	public int getNumGlyphs()
	{
		return m_glyphs.length;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The glyph at the given index.
	 */
	public Glyph getGlyph(int in_index)
	{
		assert (in_index >= 0 && in_index < m_glyphs.length) : "Glyph index out of range.";
		
		return m_glyphs[in_index];
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
	 * @return The advance for a space character.
	 */
	public int getSpaceAdvance()
	{
		return m_spaceAdvance;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The amount of padding at the top and bottom of a glyph.
	 */
	public int getVerticalPadding()
	{
		return m_verticalPadding;
	}
}
