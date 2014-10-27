/**
 * ZipUtils.java
 * Chilli Source
 * Created by Ian Copland on 16/10/2014
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

package com.chilliworks.chillisource.coreutils;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * A collection of methods for working with zip files.
 * 
 * @author Ian Copland
 */
public final class ZipUtils
{
	/**
	 * Extracts the contents of the zip file at the given file path to the
	 * given directory path.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_zipFilePath - The input zip file path.
	 * @param in_outputDirectoryPath - The output directory path.
	 * 
	 * @throws IOException
	 */
	public static void extract(String in_zipFilePath, String in_outputDirectoryPath) throws IOException
	{
		String zipFilePath = StringUtils.standardiseFilePath(in_zipFilePath);
		String outputDirectoryPath = StringUtils.standardiseDirectoryPath(in_outputDirectoryPath);
		
		try (ZipInputStream zipStream = new ZipInputStream(new FileInputStream(zipFilePath)))
		{
			ZipEntry zipEntry = null;
			while ((zipEntry = zipStream.getNextEntry()) != null)
			{
				if(zipEntry.isDirectory() == true)
				{
					File directory = new File(outputDirectoryPath + zipEntry.getName());
					if(directory.exists() == false)
					{
						directory.mkdirs();
					}
				}
				else
				{
					String relDirectoryPath = StringUtils.getDirectory(zipEntry.getName());
					
					File directory = new File(outputDirectoryPath + relDirectoryPath);
					if(directory.exists() == false)
					{
						directory.mkdirs();
					}
		
					extractFile(zipStream, outputDirectoryPath + zipEntry.getName());
				}  
			}
		}
		catch (IOException e)
		{
			throw e;
		}
	}
	/**
	 * Extracts the file described by the given zip entry to the given output file path.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_zipStream - The input zip stream.
	 * @param in_outputFilePath - The output directory path.
	 * 
	 * @throws IOException
	 */
	private static void extractFile(ZipInputStream in_zipStream, String in_outputFilePath) throws IOException
	{
		String outputFilePath = StringUtils.standardiseFilePath(in_outputFilePath);
		
	    try (BufferedOutputStream outputStream = new BufferedOutputStream(new FileOutputStream(new File(outputFilePath))))
	    {
	    	byte[] buffer = new byte[8192];
	    	int size = -1;
	    	
	    	while ((size = in_zipStream.read(buffer)) != -1)
		    {
	    		outputStream.write(buffer, 0, size);
		    }
	    }
	    catch (IOException e)
	    {
	    	throw e;
	    }
	}
}
