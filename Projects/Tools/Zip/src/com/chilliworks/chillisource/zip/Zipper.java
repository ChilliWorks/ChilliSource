/**
 * Zipper.java
 * ChilliSource
 * Created by Ian Copland on 01/07/2015.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Tag Games Limited
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

package com.chilliworks.chillisource.zip;

import java.io.IOException;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.ZipUtils;

/**
 * Provides the ability to zip the contents of the given directory path. Optionally
 * extensions can be provided which should remain uncompressed inside the zip.
 * 
 * @author Ian Copland
 */
public final class Zipper
{
	/**
	 * Zips the contents of the directory specified in the options to the given output
	 * zip file. Optionally extensions can be provided which should remain uncompressed 
	 * inside the zip.
	 * 
	 * @param in_options - The zip options.
	 * 
	 * @throws CSException - This is thrown if any fatal errors occur during the process
	 * of zipping.
	 */
	public static void zip(ZipOptions in_options) throws CSException
	{
		try
		{
			ZipUtils.zip(in_options.m_inputDirectoryPath, in_options.m_outputFilePath, in_options.m_uncompressedExtensions);
		}
		catch (IOException e)
		{
			e.printStackTrace();
			
			Logging.logFatal(e.getMessage());
			
			throw new CSException("Could not create the zip file '" + in_options.m_outputFilePath + "' from the contents of '" + in_options.m_inputDirectoryPath + "'.");
		}
	}
}
