/**
 * ZipUtils.java
 * ChilliSource
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

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;
import java.util.zip.ZipOutputStream;

/**
 * A collection of methods for working with zip files.
 * 
 * @author Ian Copland
 */
public final class ZipUtils
{
    private static final int BUFFER_SIZE = 8 * 1024;
	
	/**
     * Zips the contents of of the given directory. The directory itself will
     * not be included in the zip. The zip should not already exist. Optionally
	 * extensions can be provided which should remain uncompressed inside the zip.
     *
     * @author Ian Copland
     *
     * @param in_directoryPath - The path to the directory whose contents will
     * be zipped.
     * @param in_zipFilePath - The path to the output zip file.
     * @param in_uncompressedExtensions - A list of file extensions which should 
     * not be compressed. These must contain no whitespace and must have a leading
     * full stop.
     * 
     * @throws IOException - if the zip could not be created.
     */
	public static void zip(String in_directoryPath, String in_zipFilePath, List<String> in_uncompressedExtensions) throws IOException
	{
		if (new File(in_zipFilePath).exists() == true)
		{
			throw new IOException("Cannot create zip file becuase it already exists.");
		}

        byte[] buffer = new byte[BUFFER_SIZE];
		
        try (ZipOutputStream zipStream = new ZipOutputStream(new BufferedOutputStream(new FileOutputStream(in_zipFilePath))))
		{
            File directory = new File(in_directoryPath);
            String[] directoryContents = directory.list();
            for (String directoryItemName : directoryContents) 
            {
                File directoryItem = new File(directory, directoryItemName);
                if (directoryItem.isDirectory() == true) 
                {
                    zipDirectoryRecursive(zipStream, directoryItem.getAbsolutePath(), in_directoryPath, in_uncompressedExtensions, buffer);
                } 
                else if (directoryItem.isFile() == true) 
                {
                    zipFile(zipStream, directoryItem.getAbsolutePath(), in_directoryPath, in_uncompressedExtensions, buffer);
                }
            }
        }
        catch(IOException e) 
		{
        	e.printStackTrace();
        	Logging.logFatal(e.getMessage());
        	
        	File zip = new File(in_zipFilePath);
        	if (zip.exists() == true)
    		{
        		zip.delete();
    		}
        	
            throw e;
        }
	}
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
	 * Recursively adds the directory to the given zip.
	 *
	 * @author Ian Copland
	 *
	 * @param in_zipStream - The stream to add the file to.
	 * @param in_absDirectoryPath - The full path to the directory.
	 * @param in_relativeTo - The directory the zip entries should be relative to
	 * @param in_uncompressedExtensions - A list of file extensions which should not be 
	 * compressed
	 * @param in_copyBuffer - A buffer used by the zipFile method. This is provided
	 * as an optimisation to avoid re-allocating the buffer for each file.
	 * 
	 * @throws IOException - If the directory could not be added to the zip.
	 */
	private static void zipDirectoryRecursive(ZipOutputStream in_zipStream, String in_absDirectoryPath, String in_relativeTo, List<String> in_uncompressedExtensions, byte[] in_copyBuffer) throws IOException
	{
	    File directory = new File(in_absDirectoryPath);

	    String[] directoryContents = directory.list();
	    for (String directoryItemName : directoryContents) 
	    {
	        File directoryItem = new File(directory, directoryItemName);
	        if (directoryItem.isDirectory() == true) 
	        {
	            zipDirectoryRecursive(in_zipStream, directoryItem.getAbsolutePath(), in_relativeTo, in_uncompressedExtensions, in_copyBuffer);
	        } else if (directoryItem.isFile() == true) 
	        {
	            zipFile(in_zipStream, directoryItem.getAbsolutePath(), in_relativeTo, in_uncompressedExtensions, in_copyBuffer);
	        }
	    }
	}
	/**
	 * Adds the given file to the zip.
	 *
	 * @author Ian Copland
	 *
	 * @param in_zipStream - The stream to add the file to.
	 * @param in_absFilePath - The full file path.
	 * @param in_relativeTo - The path the file is relative to.
	 * @param in_uncompressedExt - A list of file extensions which should not be compressed
	 * @param in_copyBuffer - a buffer used in copying the file contents. This is provided
	 * as an optimisation to avoid re-allocating the buffer for each file.
	 * 
	 * @throws IOException - If the file could not be added to the zip file.
	 */
	private static void zipFile(ZipOutputStream in_zipStream, String in_absFilePath, String in_relativeTo, List<String> in_uncompressedExt, byte[] in_copyBuffer) throws IOException
	{
        String relativePath = StringUtils.getRelativeFilePath(in_absFilePath, in_relativeTo);
        ZipEntry entry = new ZipEntry(relativePath);

        if (hasExtension(relativePath, in_uncompressedExt) == true) 
        {
        	long fileSize = new File(in_absFilePath).length();
        	long crc = FileUtils.calcFileCRC32(in_absFilePath); 
        	
        	entry.setMethod(ZipOutputStream.STORED);
        	entry.setCrc(crc);
        	entry.setSize(fileSize);
        	entry.setCompressedSize(fileSize);
        } 
        else 
        {
        	entry.setMethod(ZipOutputStream.DEFLATED);
        }
        
        in_zipStream.putNextEntry(entry);
		
	    try (BufferedInputStream bufferedInputStream = new BufferedInputStream(new FileInputStream(in_absFilePath), in_copyBuffer.length))
	    {        
	        int count = 0;
	        while((count = bufferedInputStream.read(in_copyBuffer, 0, in_copyBuffer.length)) != -1) 
	        {
	            in_zipStream.write(in_copyBuffer, 0, count);
	        }
	    }
	    catch(IOException e) 
	    {
	        throw e;
	    }
	}
	/**
	 * @author Ian Copland
	 *
	 * @param in_filePath - A file path.
	 * @param in_extensions - A list of extensions.
	 *
	 * @return Whether or not the given file path has one of the given extensions.
	 */
	private static boolean hasExtension(String in_filePath, List<String> in_extensions) 
	{
	    String lowerFilePath = in_filePath.toLowerCase();
	    for (String extension : in_extensions) 
	    {
	        if (lowerFilePath.endsWith(extension) == true) 
	        {
	            return true;
	        }
	    }
	    return false;
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
	    	byte[] buffer = new byte[BUFFER_SIZE];
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
