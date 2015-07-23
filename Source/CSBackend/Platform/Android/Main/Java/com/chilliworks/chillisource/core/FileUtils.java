/**
 * FileUtils.java
 * ChilliSource
 * Created by Ian Copland on 16/12/2013
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
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

package com.chilliworks.chillisource.core;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import android.os.Environment;

/**
 * A collection of convenience methods for working with file input and output.
 * 
 * @author Ian Copland
 */
public final class FileUtils
{
	/**
	 * @author Ian Copland
	 * 
	 * @return the "external storage" directory.
	 */
	public static String getExternalStorageDirectory()
	{
		String state = Environment.getExternalStorageState();

		if (Environment.MEDIA_MOUNTED.equals(state))
		{
			return StringUtils.standardiseDirectoryPath(Environment.getExternalStorageDirectory().toString());
		}

		Logging.logFatal("No external storage mounted.");
		return "";
	}
	/**
	 * This will return the path to the directory on external storage that important files should
	 * be stored in for the application with the given package name. This directory will be:
	 *
	 *  	Android/data/<PackageName>/files/
	 *
	 * @author Ian Copland
	 *
	 * @param in_packageName - The package name.
	 *
	 * @return The files directory.
	 */
	public static String getExternalFilesDirectory(String in_packageName)
	{
		return getExternalStorageDirectory() + "Android/data/" + in_packageName + "/files/";
	}
	/**
	 * This will return the path to the directory on external storage that unimportant files which
	 * can be lost and later recreated should be stored in for the application with the given
	 * package name. This directory will be:
	 *
	 *  	Android/data/<PackageName>/cache/
	 *
	 * @author Ian Copland
	 *
	 * @param in_packageName - The package name.
	 *
	 * @return The cache directory.
	 */
	public static String getExternalCacheDirectory(String in_packageName)
	{
		return getExternalStorageDirectory() + "Android/data/" + in_packageName + "/cache/";
	}
    /**
     * Returns the path to the directory used by the FileSystem which relates to the "Save Data"
     * Storage Location.
     *
     * @author Ian Copland
     *
     * @param in_packageName - The package name of the application.
     *
     * @return The save data directory.
     */
    public static String getSaveDataDirectory(String in_packageName)
	{
		return getExternalFilesDirectory(in_packageName) + "SaveData/";
	}
	/**
	 * @author Ian Copland
	 *
	 * @param in_filePath - The file path.
	 *
	 * @return whether or not the given path exists.
	 */
	public static boolean doesFileExist(String in_filePath)
	{
		File file = new File(in_filePath);
		if (file.exists() == true && file.isFile() == true)
		{
			return true;
		}
		return false;
	}
	/**
	 * @author Ian Copland
	 *
	 * @param in_directoryPath - The path to the directory to check.
	 *
	 * @return whether or not the given path exists.
	 */
	public static boolean doesDirectoryExist(String in_directoryPath)
	{
		File directory = new File(in_directoryPath);
		if (directory.exists() == true && directory.isDirectory() == true)
		{
			return true;
		}
		return false;
	}
	/**
	 * Reads the entire contents of a binary file to a byte array.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_filePath - The path to the file.
	 *
	 * @return The file contents.
	 */
	public static byte[] readBinaryFile(String in_filePath)
	{
		try
		{
			if (doesFileExist(in_filePath) == true)
			{
				final int k_bufferSize = 16 * 1024;
				DynamicByteBuffer dynamicByteBuffer = new DynamicByteBuffer(k_bufferSize);

				File file = new File(in_filePath);
				InputStream stream = new BufferedInputStream(new FileInputStream(file));
				try
				{
					byte[] buffer = new byte[k_bufferSize];
					int numRead = 0;
					while(numRead != -1)
					{
						numRead = stream.read(buffer, 0, k_bufferSize);
						if (numRead > 0)
						{
							dynamicByteBuffer.appendBytes(buffer, numRead);
						}
					}
				}
				finally
				{
					stream.close();
				}

				return dynamicByteBuffer.toByteArray();
			}
		}
		catch (Exception e)
		{
			Logging.logVerbose(ExceptionUtils.convertToString(e));
			Logging.logError("An error occurred while reading file '" + in_filePath + "': " + e.getMessage());
		}
		
		return null;
	}
	/**
	 * Reads the entire contents of a text file.
	 *
	 * @author Ian Copland
	 *
	 * @param in_filePath - The path to the file.
	 *
	 * @return The file contents.
	 */
	public static String readTextFile(String in_filePath)
	{
		byte[] contents = readBinaryFile(in_filePath);
		if (contents == null)
		{
			return "";
		}
		return StringUtils.utf8BytesToString(contents);
	}
	/**
	 * Writes a byte array to file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_filePath - The filename.
	 * @param in_fileContents - The data to be written to file.
	 * 
	 * @return Whether or not the file was successfully written.
	 */
	public static boolean writeBinaryFile(String in_filePath, byte[] in_fileContents)
	{
		boolean success = false;
		try
		{
			File file = new File(in_filePath);
			OutputStream stream = new BufferedOutputStream(new FileOutputStream(file));
			try
			{
				stream.write(in_fileContents);
				success = true;
			}
			finally
			{
				stream.close();
			}
		}
		catch (Exception e)
		{
			Logging.logVerbose(ExceptionUtils.convertToString(e));
			Logging.logError("An error occurred while writing file '" + in_filePath + "': " + e.getMessage());
		}
		return success;
	}
	/**
	 * Writes text to file.
	 *
	 * @author Ian Copland
	 *
	 * @param in_filePath - The filename.
	 * @param in_fileContents - The text to be written to file.
	 *
	 * @return Whether or not the file was successfully written.
	 */
	public static boolean writeBinaryFile(String in_filePath, String in_fileContents)
	{
		byte[] bytes = StringUtils.stringToUTF8Bytes(in_fileContents);
		return writeBinaryFile(in_filePath, bytes);
	}
	/**
	 * Creates a new directory at the given path. The path to the directory must already exist.
	 * This will fail if the directory already exists.
	 *
	 * @author Ian Copland
	 *
	 * @param in_directoryPath - The path to the directory which should be created.
	 *
	 * @return Whether or not this was successful.
	 */
	public static boolean createDirectory(String in_directoryPath)
	{
		File file = new File(in_directoryPath);
		return file.mkdir();
	}
	/**
	 * Deletes the given file on disk if it exists. If the file doesn't exist this will return
	 * an error.
	 *
	 * @author Ian Copland
	 *
	 * @param in_filePath - The filename.
	 *
	 * @return Whether or not this was successful.
	 */
	public static boolean removeFile(String in_filePath)
	{
		if (doesFileExist(in_filePath) == false)
		{
			return false;
		}

		File file = new File(in_filePath);
		return file.delete();
	}
	/**
	 * Deletes the given directory and all of its contents on disk if it exists. If the directory
	 * doesn't exist this will return an error.
	 *
	 * @author Ian Copland
	 *
	 * @param in_directoryPath - The path to the directory to remove.
	 *
	 * @return Whether or not this was successful.
	 */
	public static boolean removeDirectory(String in_directoryPath)
	{
		String directoryPath = StringUtils.standardiseDirectoryPath(in_directoryPath);

		if (doesDirectoryExist(directoryPath) == false)
		{
			return false;
		}

		File directory = new File(directoryPath);

		String[] directoryContents = directory.list();
		for (String directoryItemString : directoryContents)
		{
			File directoryItem = new File(directoryPath + directoryItemString);

			if (directoryItem.isDirectory() == true)
			{
				if (removeDirectory(directoryItem.getPath()) == false)
				{
					return false;
				}
			}
			else if (directoryItem.isFile() == true)
			{
				if (directoryItem.delete() == false)
				{
					return false;
				}
			}
			else
			{
				//Unknown item in directory, return false.
				return false;
			}
		}

		return directory.delete();
	}
    /**
     * Returns the size of the given file in bytes.
     *
     * @author Ian Copland
     *
     * @param in_filePath - The path to the file which is to have its size calculated.
     *
     * @return The size of the file in bytes.
     */
	public static long getFileSize(String in_filePath)
    {
        File file = new File(in_filePath);
        return file.length();
    }
}
