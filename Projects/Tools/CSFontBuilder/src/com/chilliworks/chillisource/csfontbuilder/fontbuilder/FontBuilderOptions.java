/**
 * FontBuilderOptions.java
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

import com.chilliworks.chillisource.toolutils.Colour;

/**
 * A container for the input options for the Font Builder.
 *  
 * @author Ian Copland
 */
public final class FontBuilderOptions
{
	public String m_fontName = "";
	public String m_outputFilePath = "";
	
	//bitmap glyph creation
	public Colour m_colour = Colour.WHITE;
	public boolean m_outline = false;
	public boolean m_dropShadow = false;
	public int m_fontPointSize = 12;
	String m_characters = "!$%&,-.0123456789:;?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz¡£¥©®¿ÀÌÍßàáãäçèéêíóõöùúü–’“”…₩€元";
	
}