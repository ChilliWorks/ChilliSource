/**
 * FontFromGlyphsBuilder.java
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

import java.io.File;
import java.util.ArrayList;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.texturepackerutils.PackedTexture;
import com.chilliworks.chillisource.texturepackerutils.TexturePacker;
 
/**
 * A static class that provides methods for building a CSFont bitmap font
 * from a collection of bitmap glyphs and an optional kerning info file.
 * 
 * @author Ian Copland
 */
public final class FontFromGlyphsBuilder
{
	/**
	 * Builds a CSFont bitmap font from a series of bitmap glyphs with the 
	 * given options.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_inputDirectoryPath - The input directory path containing the
	 * glyphs.
	 * @param in_outputFilePath - The output file path for the CSFont file.
	 * @param in_options - The builder options.
	 * 
	 * @throws CSException - An exception which provides a message describing 
	 * the error which has occurred.
	 */
	public static void build(String in_inputDirectoryPath, String in_outputFilePath, FontFromGlyphsBuilderOptions in_options) throws CSException
	{
		Glyphs glyphs = GlyphsReader.read(in_inputDirectoryPath);
		PackedTexture packedBitmapFont = packBitmapFont(in_options, glyphs);
		CSFontWriter.write(in_outputFilePath, in_options, glyphs, packedBitmapFont);
	}
	/**
	 * Uses the texture package to pack all gylphs into a single image. This takes into account
	 * the options provided to the tool for things like the size of the image and the packing
	 * heuristic.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options which should be used to generate the packed bitmap.
	 * @param in_glyphs - The glyphs which should be packed.
	 * 
	 * @return The packaged texture. This will never be null.
	 * 
	 * @throws CSException - An exception which provides a message describing the error which
	 * has occurred.
	 */
	private static PackedTexture packBitmapFont(FontFromGlyphsBuilderOptions in_options, Glyphs in_glyphs) throws CSException
	{
		try
		{
			final int PADDING = 2;
			
			ArrayList<File> filesToProcess = new ArrayList<File>();
			for (int i = 0; i < in_glyphs.getNumGlyphs(); ++i)
			{
				Glyph glyph = in_glyphs.getGlyph(i);
				filesToProcess.add(new File(glyph.getFilePath()));
			}
	
			TexturePacker packer = new TexturePacker();
			packer.setDivisibleBy(in_options.m_divisibleBy);
			packer.setExtrusion(0);
			packer.setFixedHeight(in_options.m_fixedHeight);
			packer.setFixedWidth(in_options.m_fixedWidth);
			packer.setMaxHeight(in_options.m_maxHeight);
			packer.setMaxWidth(in_options.m_maxWidth);
			packer.setValidHeights(in_options.m_validHeights);
			packer.setValidWidths(in_options.m_validWidths);
			packer.setHeuristic(in_options.m_packingHeuristic);
			packer.setInnerPadding(0);
			packer.setOuterPadding(PADDING);
			packer.enableCropping(true);
			return packer.pack(filesToProcess, null);
		}
		catch (Exception e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			throw new CSException("Could not pack bitmap font.", e);
		}
	}
}
