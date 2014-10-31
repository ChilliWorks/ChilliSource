/**
 * GlyphsBuilder.java
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

import com.chilliworks.chillisource.coreutils.CSException;
 
/**
 * A static class containing methods for converting a vector font (TTF or OTF) to
 * bitmap font glyphs with kerning info.
 * 
 * @author Ian Copland
 */
public final class GlyphsBuilder
{
	/**
	 * Converts the font described in the input options to bitmap font glyphs. Also
	 * outputs kerning info for the glyphs.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_outputDirectoryPath - The output directory for the glyphs.
	 * @param in_options - The builder options.
	 * 
	 * @throws CSException - An exception which provides a message describing the 
	 * error which has occurred.
	 */
	public static void build(String in_outputDirectoryPath, GlyphsBuilderOptions in_options) throws CSException
	{
		Glyphs glyphs = GlyphsRenderer.render(in_options);
		GlyphsWriter.write(glyphs, in_outputDirectoryPath);
	}	

}
