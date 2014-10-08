/**
 * AtlasImage.java
 * Chilli Source
 * Created by Ian Copland on 09/09/2014.
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

package com.chilliworks.chillisource.csatlasbuilder;

import java.io.File;

/**
 * A simple container class for information on a single exportable image
 * in a texture atlas.
 * 
 * @author Ian Copland
 *
 */
public final class AtlasImage
{
	private File m_sourceImageFile;
	private String m_atlasImageId;
	
	/**
	 * Constructor.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_file - The image file.
	 * @param in_atlasImageId - The atlas image id.
	 */
	public AtlasImage(File in_sourceImageFile, String in_atlasImageId)
	{
		m_sourceImageFile = in_sourceImageFile;
		m_atlasImageId = in_atlasImageId;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The source image file.
	 */
	public File getSourceImageFile()
	{
		return m_sourceImageFile;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The atlas image Id.
	 */
	public String getAtlasImageId()
	{
		return m_atlasImageId;
	}
}
