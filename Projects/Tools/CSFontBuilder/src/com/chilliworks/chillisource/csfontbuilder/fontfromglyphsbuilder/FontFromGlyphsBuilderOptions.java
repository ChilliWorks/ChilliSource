/**
 * FontFromGlyphsBuilderOptions.java
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

package com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder;

import com.chilliworks.chillisource.texturepackerutils.TexturePacker.PlacementHeuristic;

/**
 * A container for the input options for the Font From Glyphs Builder.
 *  
 * @author Ian Copland
 */
public final class FontFromGlyphsBuilderOptions
{
	public int m_fixedWidth = -1;
	public int m_fixedHeight = -1;
	public int[] m_validHeights = new int[0];
	public int[] m_validWidths = new int[0];
	public int m_maxWidth = 2048;
	public int m_maxHeight = 2048;
	public int m_divisibleBy = 4;
	public PlacementHeuristic m_packingHeuristic = PlacementHeuristic.BOTTOMRIGHT;
	public String m_imageCompression = "";
	public String m_imageFormat = "";
	public boolean m_imageDither = false;
	public boolean m_imagePremultiplyAlpha = true;
}