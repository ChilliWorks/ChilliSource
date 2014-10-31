/**
 * GlyphsBuilderOptions.java
 * Chilli Source
 * Created by Ian Copland on 09/10/2014.
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

import com.chilliworks.chillisource.coreutils.Colour;
import com.chilliworks.chillisource.coreutils.Integer2;

/**
 * A container for the input options for the Glyphs Builder.
 *  
 * @author Ian Copland
 */
public final class GlyphsBuilderOptions
{
	public String m_characters = "!$%&,-.0123456789:;?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz¡£¥©®¿ÀÌÍßàáãäçèéêíóõöùúü–’“”…₩€元";
	public String m_fontName = "Arial";
	public int m_fontStyle = Font.PLAIN;
	public int m_fontSize = 12;
	public Colour m_fontColour = Colour.WHITE;
	public boolean m_enableDropShadow = false;
	public Colour m_dropShadowColour = new Colour(0.3, 0.3, 0.3, 0.85);
	public Integer2 m_dropShadowOffset = new Integer2(2, 2);
	public boolean m_enableOutline = false;
	public Colour m_outlineColour = Colour.BLACK;
	public int m_outlineSize = 1;
	public boolean m_enableGlow = false;
	public Colour m_glowColour = new Colour(1.0, 1.0, 1.0, 0.1);
	public int m_glowSize = 3;
}