/**
 * OpenTypeKerningReader.java
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

package com.chilliworks.chillisource.csfontbuilder.glyphsbuilder;

import java.io.BufferedInputStream;
import java.io.IOException;

import com.chilliworks.chillisource.toolutils.Pair;

/**
 * A collection of methods for reading the kerning data from  Open Type Fonts (OTF)
 * and True Type Fonts (TTF).
 *  
 * @author Ian Copland
 */
public final class OpenTypeKerningReader
{
	/**
	 * Reads the kerning data from the given Open Type Font. As OFT is an extension
	 * to True Type Fonts (TTF) this can also read the Kerning data from a TTF. 
	 * 
	 * This will log a fatal error if the font could not be parsed.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_fontFilePath - The font file path.
	 * 
	 * @return The kerning info. Empty kerning info indicates that the font was 
	 * successfully read but contains no kerning data.
	 */
	public static KerningData readKerningData(String in_fontFilePath)
	{
		assert (in_fontFilePath != null) : "Cannot read kerning data from a null file path.";
		
		KerningData kerningData = new KerningData();
		
		//TODO: !?
//		try (BufferedInputStream inputStream = new BufferedInputStream(new FileInputStream(in_fontFilePath)))
//		{
//			Pair<Integer, Integer> kernTableInfo = findKernTable(inputStream);
//		}
//		catch (Exception e)
//		{
//			Logging.logVerbose(StringUtils.convertExceptionToString(e));
//			Logging.logFatal("Could not parse the font file '" + in_fontFilePath + "'");
//		}
		
		return kerningData;
	}
	/**
	 * Reads the given OTF input stream to find the location and size of the kern table 
	 * 
	 * This will log a fatal error if the font could not be parsed.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_fontFilePath - The font file path.
	 * 
	 * @return The kerning info. Empty kerning info indicates that the font was 
	 * successfully read but contains no kerning data.
	 * @throws IOException 
	 */
	public static Pair<Integer, Integer> findKernTable(BufferedInputStream in_inputStream) throws IOException
	{
		assert (in_inputStream != null) : "Cannot find kern table in null input stream.";
		
		//TODO: !? 
		
		return null;
	}
}
