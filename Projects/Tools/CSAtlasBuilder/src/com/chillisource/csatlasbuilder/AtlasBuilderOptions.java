/**
 * AtlasBuilderOptions.java
 * Chilli Source
 * Created by Ian Copland on 26/06/2014.
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

package com.chillisource.csatlasbuilder;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

import com.chillisource.texturepackerutils.TexturePacker.PlacementHeuristic;

public class AtlasBuilderOptions
{
	public String m_inputDirectoryPath = "";
	public String m_outputFilePath = "";
	public String m_fileList = "";
	public Map<File, String> m_imageFileToIDNameMap = new HashMap<File, String>();//Available through java library access only
	public int m_fixedWidth = -1;
	public int m_fixedHeight = -1;
	public int[] m_validHeights = new int[0];
	public int[] m_validWidths = new int[0];
	public int m_maxWidth = 2048;
	public int m_maxHeight = 2048;
	public int m_divisibleBy = 1;
	public boolean m_crop = true;
	public int m_padding = 0;
	public int m_innerPadding = 0;
	public int m_extrude = 0;
	public PlacementHeuristic m_packingHeuristic = PlacementHeuristic.BOTTOMRIGHT;
	public int m_lineHeight = 0;
	public String m_imageCompression = "";
	public String m_imageFormat = "";
	public boolean m_imageDither = false;
	public boolean m_imagePremultiplyAlpha = true;
}