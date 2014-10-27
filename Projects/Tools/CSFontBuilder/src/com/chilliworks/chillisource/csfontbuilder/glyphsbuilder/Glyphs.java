/**
 * Glyphs.java
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

import java.awt.image.BufferedImage;

import com.chilliworks.chillisource.coreutils.Integer2;

/**
 * A container for information on a collection of glyphs. This is immutable after
 * construction.
 * 
 * @author Ian Copland
 */
public final class Glyphs
{
	private final char[] m_characters;
	private final BufferedImage[] m_images;
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
	 * @param in_images - The image for each glyph.
	 * @param in_fontSize - The point size of the font the glyphs were created from.
	 * @param in_lineHeight - The total height of a line of text.
	 * @param in_descent - The amount of space under the baseline in a line of text.
	 * @param in_effectPadding - The padding added to the image for effects such as a
	 * glow. This padding can be used to ensure correct placement when rendering the
	 * final images.
	 */
	public Glyphs(char[] in_characters, BufferedImage[] in_images, int in_fontSize, int in_lineHeight, int in_descent, Integer2 in_effectPadding)
	{
		assert (in_characters.length == in_images.length) : "Character and image arrays cannot be different sizes.";
		
		m_characters = in_characters;
		m_images = in_images;
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
		return m_images.length;
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
	 * @return The image for each glyph.
	 */
	public BufferedImage[] getImages()
	{
		return m_images;
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
