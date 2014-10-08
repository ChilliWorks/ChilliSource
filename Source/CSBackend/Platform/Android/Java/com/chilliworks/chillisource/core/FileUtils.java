/**
 * FileUtils.java
 * Chilli Source
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
import java.util.Arrays;
import java.util.List;

import com.chilliworks.chillisource.core.CSActivity;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Environment;

/**
 * A collection of convenience methods for working with 
 * File input and output.
 * 
 * @author Ian Copland
 */
public final class FileUtils
{
	/**
	 * An enum describing the different storage locations available on Android. Note that this 
	 * is not the same as Storage Location in the native side of the engine. This refers to 
	 * specific storage media such as internal or external storage.
	 * 
	 * @author Ian Copland
	 */
	public enum StorageLocation
	{
		k_root,
		k_externalStorage,
		k_internalStorage,
		k_apk
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return the "external storage" directory.
	 */
	public static String getExternalStorageDirectory()
	{
		String state = Environment.getExternalStorageState();

		if (Environment.MEDIA_MOUNTED.equals(state))
			return StringUtils.StandardisePath(Environment.getExternalStorageDirectory().toString());
		
		return "";
	}
	/**
	 * This will return the path to the directory on external storage 
	 * that important files should be stored in for the application 
	 * with the given package name. This directory will be:
	 *  	Android/data/<PackageName>/files/
	 *  
	 * @author Ian Copland
	 * 
	 * @param The package name.
	 * 
	 * @return The files directory.
	 */
	public static String getExternalFilesDirectory(String in_packageName)
	{
		return "Android/data/" + in_packageName + "/files/";
	}
	/**
	 * Returns whether or not the given file exists in the given 
	 * storage location.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The storage location. If APK, the CSApplication and CSActivity
	 * must exist.
	 * @param The file path.
	 * 
	 * @return Whether or not the path exists.
	 */
	public static boolean doesFileExist(StorageLocation in_storageLocation, String in_filePath)
	{
		switch (in_storageLocation)
		{
			case k_root:
				return doesFileExist(in_filePath);
			case k_externalStorage:
				return doesFileExist(StringUtils.StandardisePath(getExternalStorageDirectory()) + in_filePath);
			case k_internalStorage:
				return doesFileExistInternal(in_filePath);
			case k_apk:
				return doesFileExistAPK(CSApplication.get().getActivity(), in_filePath);
			default:
				Logging.logFatal("FileUtils: Invalid storage location.");
				return false;
		}
	}
	/**
	 * Reads the entire contents of a binary file in the given 
	 * storage location. The contents are read as a byte array.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The storage location. If APK, the CSApplication and CSActivity
	 * must exist.
	 * @param The filename.
	 * 
	 * @return The byte array.
	 */
	public static byte[] readFile(StorageLocation in_storageLocation, String in_filePath)
	{
		switch (in_storageLocation)
		{
			case k_root:
				return readFile(in_filePath);
			case k_externalStorage:
				return readFile(StringUtils.StandardisePath(getExternalStorageDirectory()) + in_filePath);
			case k_internalStorage:
				return readFileInternal(in_filePath);
			case k_apk:
				return readFileAPK(CSApplication.get().getActivity(), in_filePath);
			default:
				Logging.logFatal("FileUtils: Invalid storage location.");
				return null;
		}
	}
	/**
	 * Writes a byte array to file in the given storage location.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The storage location.
	 * @param The filename.
	 * @param The data to be written to file.
	 * 
	 * @return Whether or not the file was successfully written.
	 */
	public static boolean writeFile(StorageLocation in_storageLocation, String in_filePath, byte[] in_fileContents)
	{
		switch (in_storageLocation)
		{
			case k_root:
				return writeFile(in_filePath, in_fileContents);
			case k_externalStorage:
				return writeFile(StringUtils.StandardisePath(getExternalStorageDirectory()) + in_filePath, in_fileContents);
			case k_internalStorage:
				return writeFileInternal(in_filePath, in_fileContents);
			case k_apk:
				Logging.logFatal("FileUtils: Cannot write to the APK.");
				return false;
			default:
				Logging.logFatal("FileUtils: Invalid storage location.");
				return false;
		}
	}
	/**
	 * Deletes the given file from the given storage location. If the file 
	 * doesn't exist this will still return success. it will only return 
	 * an error if the file still exists after this has been called.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The storage location.
	 * @param The filename.
	 * 
	 * @return Whether or not the file removal was successful.
	 */
	public static boolean removeFile(StorageLocation in_storageLocation, String in_filePath)
	{
		switch (in_storageLocation)
		{
			case k_root:
				return removeFile(in_filePath);
			case k_externalStorage:
				return removeFile(StringUtils.StandardisePath(getExternalStorageDirectory()) + in_filePath);
			case k_internalStorage:
				return removeFileInternal(in_filePath);
			case k_apk:
				Logging.logFatal("FileUtils: Cannot remove file from APK.");
				return false;
			default:
				Logging.logFatal("FileUtils: Invalid storage location.");
				return false;
		}
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param The activiy.
	 * @param The file path inside the APK.
	 * 
	 * @return Whether or not the given file exists in the APK.
	 */
	public static boolean doesFileExistAPK(Activity in_activity, String in_filePath)
	{
		try
		{
			File file = new File(in_filePath);
			
			AssetManager assets = in_activity.getAssets();
			String[] filesArray = assets.list(file.getParent());
			
		    if (filesArray != null && filesArray.length > 0)
		    {
		    	List<String> filesList = Arrays.asList(filesArray);
		    	return filesList.contains(file.getName());
		    }
		}
		catch (Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
		}
		
	    return false;
	}
	/**
	 * Reads the entire contents of a binary file from the APK. 
	 * The contents are returned as a byte array. 
	 * 
	 * @author Ian Copland
	 * 
	 * @param The activity
	 * @param The filename.
	 * 
	 * @return The file contents.
	 */
	public static byte[] readFileAPK(Activity in_activity, String in_filePath)
	{
		final int k_bufferSize = 1024;
		
		DynamicByteBuffer dynamicByteBuffer = new DynamicByteBuffer(k_bufferSize);
		try
		{
			if (doesFileExistAPK(in_activity, in_filePath) == true)
			{
				InputStream stream = null;
				try
				{
					stream = new BufferedInputStream(in_activity.getAssets().open(in_filePath));
					
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
		        catch (Exception e)
		        {
		        	throw e;
		        }
				finally
				{
					if (stream != null)
					{
						stream.close();
					}
				}
			}
		}
		catch (Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
		}
		
		return dynamicByteBuffer.toByteArray();
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param The file path.
	 * 
	 * @return whether or not the given path exists.
	 */
	private static boolean doesFileExist(String in_filePath)
	{
		File file = new File(in_filePath);
		if (file.exists() == true && file.isFile() == true)
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
	 * @param The filename.
	 * 
	 * @return The byte array.
	 */
	private static byte[] readFile(String in_filePath)
	{
		byte[] abyOutput = null;
		try
		{
			if (doesFileExist(in_filePath) == true)
			{
				InputStream stream = null;
				try
				{
					File file = new File(in_filePath);
					abyOutput = new byte[(int)file.length()];
					stream = new BufferedInputStream(new FileInputStream(file));
					int dwTotalRead = 0;
					while(dwTotalRead < abyOutput.length)
					{
						int dwRemaining = abyOutput.length - dwTotalRead;
						int dwRead = stream.read(abyOutput, dwTotalRead, dwRemaining); 
						if (dwRead > 0)
						{
							dwTotalRead += dwRead;
						}
					}
				}
		        catch (Exception e)
		        {
		        	throw e;
		        }
				finally
				{
					if (stream != null)
					{
						stream.close();
					}
				}
			}
		}
		catch (Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
		}
		
		return abyOutput;
	}
	/**
	 * Writes a byte array to file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The filename.
	 * @param The data to be written to file.
	 * 
	 * @return Whether or not the file was successfully written.
	 */
	private static boolean writeFile(String in_filePath, byte[] in_fileContents)
	{
		boolean bSuccess = false;
		try
		{
			OutputStream stream = null;
			try
			{
				File file = new File(in_filePath);
				stream = new BufferedOutputStream(new FileOutputStream(file));
				stream.write(in_fileContents);
				bSuccess = true;
			}
	        catch (Exception e)
	        {
	        	throw e;
	        }
			finally
			{
				if (stream != null)
				{
					stream.close();
				}
			}
		}
		catch (Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
		}
		return bSuccess;
	}
	/**
	 * Deletes the given file on disk if it exists. If the file 
	 * doesn't exist this will still return successful, it will 
	 * only return an error if the file still exists after this 
	 * has been called.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The filename.
	 * 
	 * @return Whether or not this was succcessful.
	 */
	private static boolean removeFile(String in_filePath)
	{
		if (doesFileExist(in_filePath) == true)
		{
			File file = new File(in_filePath);
			return file.delete();
		}
		return true;
	}
	/**
	 * Note: This method requires the Chilli Source activity to have 
	 * been created to work.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The file path.
	 * 
	 * @return Whether or not the given file exists on internal 
	 * storage.
	 */
	private static boolean doesFileExistInternal(String in_filePath)
	{
		File file = CSApplication.get().getAppContext().getFileStreamPath(in_filePath);
	    if (file.exists() == true && file.isFile() == true)
	    {
	    	return true;
	    }
	    return false;
	}
	/**
	 * Reads the entire contents of a binary file from internal storage. 
	 * The contents are returned as a byte array. Note: This method 
	 * requires the Chilli Source activity to have been created to work.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The filename.
	 * @return The file contents.
	 */
	private static byte[] readFileInternal(String in_filePath)
	{
		byte[] abyOutput = null;
		try
		{
			if (doesFileExistInternal(in_filePath) == true)
			{
				InputStream stream = null;
				try
				{
					File file = CSApplication.get().getAppContext().getFileStreamPath(in_filePath);
					abyOutput = new byte[(int)file.length()];
					stream = new BufferedInputStream(CSApplication.get().getAppContext().openFileInput(in_filePath));
					int dwTotalRead = 0;
					while(dwTotalRead < abyOutput.length)
					{
						int dwRemaining = abyOutput.length - dwTotalRead;
						int dwRead = stream.read(abyOutput, dwTotalRead, dwRemaining); 
						if (dwRead > 0)
						{
							dwTotalRead += dwRead;
						}
					}
				}
		        catch (Exception e)
		        {
		        	throw e;
		        }
				finally
				{
					if (stream != null)
					{
						stream.close();
					}
				}
			}
		}
		catch (Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
		}
		
		return abyOutput;
	}
	/**
	 * Writes a byte array to a file on internal storage. 
	 * Note: This method requires the Chilli Source activity to 
	 * have been created to work.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The filename.
	 * @param The data to be written to file.
	 * @return Whether or not the file was successfully written.
	 */
	private static boolean writeFileInternal(String in_filePath, byte[] in_fileContents)
	{
		boolean bSuccess = false;
		try
		{
			OutputStream stream = null;
			try
			{
				stream = new BufferedOutputStream(CSApplication.get().getAppContext().openFileOutput(in_filePath, CSActivity.MODE_PRIVATE));
				stream.write(in_fileContents);
				bSuccess = true;
			}
	        catch (Exception e)
	        {
	        	throw e;
	        }
			finally
			{
				if (stream != null)
				{
					stream.close();
				}
			}
		}
		catch (Exception e)
		{
			Logging.logError(ExceptionUtils.ConvertToString(e));
		}
		return bSuccess;
	}
	/**
	 * Deletes the given file from internal storage. If the file doesn't 
	 * exist this will still return successful, it will only return an 
	 * error if the file still exists after this has been called. 
	 * Note: This method requires the Chilli Source activity to have 
	 * been created to work.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The filename
	 * @return Whether or not this was successful
	 */
	private static boolean removeFileInternal(String in_filePath)
	{
		if (doesFileExistInternal(in_filePath) == true)
		{
			return CSApplication.get().getAppContext().deleteFile(in_filePath);
		}
		
		return true;
	}
}
