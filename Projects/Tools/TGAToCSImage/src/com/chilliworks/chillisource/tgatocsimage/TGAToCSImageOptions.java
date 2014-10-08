/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 26/10/2012
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
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

package com.chilliworks.chillisource.tgatocsimage;

public class TGAToCSImageOptions 
{
	//--------------------------------------------------------------
	/// Image Format
	//--------------------------------------------------------------
	public enum OUTPUT_FORMAT
	{
		NONE,
		L8,
		LA88,
		RGB565,
		RGBA4444,
		RGB888,
		RGBA8888
	}
	//--------------------------------------------------------------
	/// Image Format
	//--------------------------------------------------------------
	public enum COMPRESSION_FORMAT
	{
		NONE,
		DEFAULT_ZLIB,
	}
	
	public String strInputFilename = new String();
	public String strOutputFilename = new String();
	public OUTPUT_FORMAT eConversionType = OUTPUT_FORMAT.NONE;
	public OUTPUT_FORMAT eConversionAlphaType = OUTPUT_FORMAT.NONE;
	public OUTPUT_FORMAT eConversionNoAlphaType = OUTPUT_FORMAT.NONE;
	public COMPRESSION_FORMAT eCompressionType = COMPRESSION_FORMAT.DEFAULT_ZLIB;
	public boolean bPremultiply = true;
	public boolean bDither = false;
}
