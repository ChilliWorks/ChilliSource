/*
 *  CMain.java
 *  PNGUtilities
 *
 *  Created by Ian Copland on 26/10/2012
 *  
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

package com.taggames.tgatomoimagetool;

public class TGAToMoImageOptions 
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
	public COMPRESSION_FORMAT eCompressionType = COMPRESSION_FORMAT.NONE;
	public boolean bPremultiply = false;
	public boolean bDither = false;
	public boolean bStats = false;
}
