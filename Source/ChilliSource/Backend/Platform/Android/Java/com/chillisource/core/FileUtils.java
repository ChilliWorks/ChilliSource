//  
//  CFileUtils.java
//  moFlow
//  
//  Created by Ian Copland on 16/12/2013
//  Copyright (c)2013 Tag Games Limited - All rights reserved
// 

package com.chillisource.core;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import com.chillisource.core.ChilliSourceActivity;

import android.os.Environment;

//=======================================================
/// File Utils
///
/// A collection of convenience methods for working with 
/// File input and output.
//=======================================================
public class FileUtils
{
	//------------------------------------------------
	/// Storage Location
	///
	/// An enum describing the different storage locations
	/// available on Android. Note that this is not
	/// the same as Storage Location in the native
	/// side of the engine. This refers to specific
	/// storage media such as internal or external
	/// storage.
	//------------------------------------------------
	public enum StorageLocation
	{
		ROOT,
		EXTERNAL_STORAGE,
		INTERNAL_STORAGE
	}
	//------------------------------------------------
	/// Get External Storage Directory
	///
	/// @return the "external storage" directory.
	//------------------------------------------------
	public static final String GetExternalStorageDirectory()
	{
		String state = Environment.getExternalStorageState();

		if (Environment.MEDIA_MOUNTED.equals(state))
			return StringUtils.StandardisePath(Environment.getExternalStorageDirectory().toString());
		
		return "";
	}
	//------------------------------------------------
	/// Get External Files Directory
	///
	/// This will return the path to the directory on
	/// external storage that important files should be 
	/// stored in for the application with the given
	/// package name. This directory will be:
	///  
	/// 	Android/data/<PackageName>/files/
	///
	/// @param The package name of the relevant app.
	/// @return The files directory.
	//------------------------------------------------
	public static final String GetExternalFilesDirectory(final String instrPackageName)
	{
		return "Android/data/" + instrPackageName + "/files/";
	}
	//-----------------------------------------------
	/// Does File Exist
	///
	/// Returns whether or not the given file exists
	/// in the given storage location.
	///
	/// @param The storage location.
	/// @param The file path.
	/// @returns Whether or not the path exists.
	//-----------------------------------------------
	public static boolean DoesFileExist(StorageLocation ineStorageLocation, final String instrFilepath)
	{
		String strStorageLocation = "";
		switch (ineStorageLocation)
		{
			case EXTERNAL_STORAGE:
				strStorageLocation = GetExternalStorageDirectory();
				break;
			case INTERNAL_STORAGE:
				return DoesFileExistInternal(instrFilepath);
			default:
				break;
		}
		
		return DoesFileExist(StringUtils.StandardisePath(strStorageLocation) + instrFilepath);
	}
	//-----------------------------------------------
	/// Read File
	///
	/// Reads the entire contents of a binary file in
	/// the given storage location. The contents are
	/// read as a byte array.
	///
	/// @param The storage location.
	/// @param The filename.
	/// @return The byte array.
	//-----------------------------------------------
	public static byte[] ReadFile(StorageLocation ineStorageLocation, final String instrFilename)
	{
		String strStorageLocation = "";
		switch (ineStorageLocation)
		{
			case EXTERNAL_STORAGE:
				strStorageLocation = GetExternalStorageDirectory();
				break;
			case INTERNAL_STORAGE:
				return ReadFileInternal(instrFilename);
			default:
				break;
		}
		
		return ReadFile(StringUtils.StandardisePath(strStorageLocation) + instrFilename);
	}
	//-----------------------------------------------
	/// Write File
	///
	/// Writes a byte array to file in the given
	/// storage location.
	///
	/// @param The storage location.
	/// @param The filename.
	/// @param The data to be written to file.
	/// @param Whether or not the file was successfully
	/// written.
	//-----------------------------------------------
	public static boolean WriteFile(StorageLocation ineStorageLocation, final String instrFilename, final byte[] instrFileContents)
	{
		String strStorageLocation = "";
		
		switch (ineStorageLocation)
		{
			case EXTERNAL_STORAGE:
				strStorageLocation = GetExternalStorageDirectory();
				break;
			case INTERNAL_STORAGE:
				return WriteFileInternal(instrFilename, instrFileContents);
			default:
				break;
		}
		
		return WriteFile(StringUtils.StandardisePath(strStorageLocation) + instrFilename, instrFileContents);
	}
	//-----------------------------------------------
	/// Remove File
	///
	/// Deletes the given file from the given storage
	/// location. If the file doesn't exist this will still 
	/// return success. it will only return an error 
	/// if the file still exists after this has been 
	/// called.
	///
	/// @param The storage location.
	/// @param The filename.
	/// @return Whether or not the file removal was
	/// successful.
	//-----------------------------------------------
	public static boolean RemoveFile(StorageLocation ineStorageLocation, final String instrFilename)
	{
		String strStorageLocation = "";
		
		switch (ineStorageLocation)
		{
			case EXTERNAL_STORAGE:
				strStorageLocation = GetExternalStorageDirectory();
				break;
			case INTERNAL_STORAGE:
				return RemoveFileInternal(instrFilename);
			default:
				break;
		}
		
		return RemoveFile(StringUtils.StandardisePath(strStorageLocation) + instrFilename);
	}
	//-----------------------------------------------
	/// Does File Exist
	///
	/// @param The file path.
	/// @returns whether or not the given path exists.
	//-----------------------------------------------
	private static boolean DoesFileExist(final String instrFilepath)
	{
		File file = new File(instrFilepath);
		if (file.exists() == true && file.isFile() == true)
		{
			return true;
		}
		return false;
	}
	//-----------------------------------------------
	/// Read File
	///
	/// Reads the entire contents of a binary file to
	/// a byte array.
	///
	/// @param The filename.
	/// @return The byte array.
	//-----------------------------------------------
	private static byte[] ReadFile(final String instrFilename)
	{
		byte[] abyOutput = null;
		try
		{
			if (DoesFileExist(instrFilename) == true)
			{
				InputStream stream = null;
				try
				{
					File file = new File(instrFilename);
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
			android.util.Log.e("MoFlow", ExceptionUtils.ConvertToString(e));
		}
		
		return abyOutput;
	}
	//-----------------------------------------------
	/// Write File
	///
	/// Writes a byte array to file.
	///
	/// @param The filename.
	/// @param The data to be written to file.
	/// @param Whether or not the file was successfully
	/// written.
	//-----------------------------------------------
	private static boolean WriteFile(final String instrFilename, final byte[] instrFileContents)
	{
		boolean bSuccess = false;
		try
		{
			OutputStream stream = null;
			try
			{
				File file = new File(instrFilename);
				stream = new BufferedOutputStream(new FileOutputStream(file));
				stream.write(instrFileContents);
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
			android.util.Log.e("MoFlow", ExceptionUtils.ConvertToString(e));
		}
		return bSuccess;
	}
	//-----------------------------------------------
	/// Remove File
	///
	/// Deletes the given file on disk if it exists.
	/// If the file doesn't exist this will still return
	/// successful, it will only return an error if
	/// the file still exists after this has been
	/// called.
	///
	/// @param The filename.
	/// @return Whether or not this was succcessful.
	//-----------------------------------------------
	private static boolean RemoveFile(final String instrFilename)
	{
		if (DoesFileExist(instrFilename) == true)
		{
			File file = new File(instrFilename);
			return file.delete();
		}
		return true;
	}
	//-----------------------------------------------
	/// Does File Exist Internal
	///
	/// Note: This method requires the MoFlow activity
	/// to have been created to work.
	///
	/// @param The file path.
	/// @returns whether or not the given file exists
	/// on internal storage.
	//-----------------------------------------------
	private static boolean DoesFileExistInternal(final String instrFilepath)
	{
		File file = ChilliSourceActivity.GetActivity().getBaseContext().getFileStreamPath(instrFilepath);
	    if (file.exists() == true && file.isFile() == true)
	    {
	    	return true;
	    }
	    return false;
	}
	//-----------------------------------------------
	/// Read File Internal
	///
	/// Reads the entire contents of a binary file
	/// from internal storage. The contents are
	/// returned as a byte array. Note: This method 
	/// requires the MoFlow activity to have been 
	/// created to work.
	///
	/// @param The filename.
	/// @return The file contents.
	//-----------------------------------------------
	private static byte[] ReadFileInternal(final String instrFilename)
	{
		byte[] abyOutput = null;
		try
		{
			if (DoesFileExistInternal(instrFilename) == true)
			{
				InputStream stream = null;
				try
				{
					File file = ChilliSourceActivity.GetActivity().getBaseContext().getFileStreamPath(instrFilename);
					abyOutput = new byte[(int)file.length()];
					stream = new BufferedInputStream(ChilliSourceActivity.GetActivity().openFileInput(instrFilename));
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
			android.util.Log.e("MoFlow", ExceptionUtils.ConvertToString(e));
		}
		
		return abyOutput;
	}
	//-----------------------------------------------
	/// Write File Internal
	///
	/// Writes a byte array to a file on internal 
	/// storage.  Note: This method requires the MoFlow 
	/// activity to have been created to work.
	///
	/// @param The filename.
	/// @param The data to be written to file.
	/// @param Whether or not the file was successfully
	/// written.
	//-----------------------------------------------
	private static boolean WriteFileInternal(final String instrFilename, final byte[] instrFileContents)
	{
		boolean bSuccess = false;
		try
		{
			OutputStream stream = null;
			try
			{
				stream = new BufferedOutputStream(ChilliSourceActivity.GetActivity().openFileOutput(instrFilename, ChilliSourceActivity.MODE_PRIVATE));
				stream.write(instrFileContents);
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
			android.util.Log.e("MoFlow", ExceptionUtils.ConvertToString(e));
		}
		return bSuccess;
	}
	//-----------------------------------------------
	/// Remove File Internal
	///
	/// Deletes the given file from internal storage.
	/// If the file doesn't exist this will still return
	/// successful, it will only return an error if
	/// the file still exists after this has been
	/// called. Note: This method requires the MoFlow 
	/// activity to have been created to work.
	///
	/// @param The filename.
	/// @return Whether or not this was successful.
	//-----------------------------------------------
	private static boolean RemoveFileInternal(final String instrFilename)
	{
		if (DoesFileExistInternal(instrFilename) == true)
		{
			return ChilliSourceActivity.GetActivity().deleteFile(instrFilename);
		}
		
		return true;
	}
}
