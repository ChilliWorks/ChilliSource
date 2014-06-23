package com.chillisource.toolutils;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.CodeSource;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;

public class FileUtils 
{
	/**
	 * Reads the entire contents of a file into a string.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The file path.
	 * 
	 * @return The contents of the file. Failure will return an
	 * empty string.
	 */
	public static String readFile(String in_filePath)
	{	
		String output = "";
		try
		{
			byte[] encoded = Files.readAllBytes(Paths.get(in_filePath));
			output = new String(encoded, StandardCharsets.UTF_8);
		}
		catch (Exception e)
		{
		}
		
		return output;
	}
	/**
	 * Writes the contents of a string to file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The output file path.
	 * @param The string to write.
	 * 
	 * @return Whether or not the write was successful.
	 */
	public static boolean writeFile(String in_filePath, String in_contents)
	{
		try
		{
			Files.write(Paths.get(in_filePath), in_contents.getBytes());
			return true;
		}
		catch (Exception e)
		{
			return false;
		}
	}
	/**
	 * Write a byte array to file.
	 * 
	 * @param The filename.
	 * @param The data to be written to file.
	 * @return Whether or not the file was successfully written.
	 */
	public static boolean writeFile(final String instrFilename, final byte[] instrFileContents)
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
			Logging.logError(StringUtils.convertExceptionToString(e));
		}
		return bSuccess;
	}
	/**
	 * Copies a file from one location to another.
	 * 
	 * @param the input filename.
	 * @param the output filename.
	 * @return whether or not this was successful.
	 */
	public static boolean copyFile(String instrInputFilepath, String instrOutputFilepath)
	{
		try
		{
			InputStream reader = new FileInputStream(instrInputFilepath);
			OutputStream writer = new FileOutputStream(instrOutputFilepath);
			
			byte[] abyBuffer = new byte[1024];
			int dwLength;
			while ((dwLength = reader.read(abyBuffer)) > 0) 
			{
				writer.write(abyBuffer, 0, dwLength);
			}
			
			reader.close();
			writer.close();
		}
		catch(Exception e)
		{
			Logging.logError("Failed to copy file '" + instrInputFilepath + "' to '" + instrOutputFilepath + "'");
			return false;
		}
		
		return true;
	}
	//----------------------------------------------------------------
	/// Copy Directory
	///
	/// Copies a directory from one location to another
	///
	/// @param the input directory.
	/// @param the output directory.
	/// @param whether or not to ignore dot directories and files.
	/// @return whether or not this was successful.
	//----------------------------------------------------------------
	public static boolean copyDirectory(String instrInputDirectory, String instrOutputDirectory, LinkedList<String> inastrIgnores)
	{
		try
		{
			//ignore some dirs.
			for (String strIgnore : inastrIgnores)
			{
				if (instrInputDirectory.contains(strIgnore) == true)
				{
					//This doesn't count as failure as we are choosing to ignore these dirs.
					return true;
				}
			}
			
			//if the directory doesn't exist, then create it!
			File destinationDir = new File(instrOutputDirectory);
			if (destinationDir.exists() == false)
			{
				destinationDir.mkdir();
			}
			
			
			//navigate the directory
			File sourceDir = new File(instrInputDirectory);
			String[] astrList = sourceDir.list();
			if (astrList != null) 
			{
				for (int i = 0; i < astrList.length; i++) 
				{
					File entry = new File(sourceDir, astrList[i]);
					if (entry.isDirectory())
					{
						copyDirectory(StringUtils.standardiseFilepath(instrInputDirectory) + StringUtils.standardiseFilepath(astrList[i]),
								StringUtils.standardiseFilepath(instrOutputDirectory) + StringUtils.standardiseFilepath(astrList[i]), inastrIgnores);
					}
					else
					{
						copyFile(StringUtils.standardiseFilepath(instrInputDirectory) + StringUtils.standardiseFilepath(astrList[i]),
								StringUtils.standardiseFilepath(instrOutputDirectory) + StringUtils.standardiseFilepath(astrList[i]));
					}
				}
			}
		}
		catch(Exception e)
		{
			Logging.logError("Failed to copy directory '" + instrInputDirectory + "' to '" + instrOutputDirectory + "'");
			return false;
		}
		
		return true;
	}
	//----------------------------------------------------------------
	/// Delete File
	///
	/// Deletes a file
	///
	/// @param the filename.
	/// @return whether or not this was successful.
	//----------------------------------------------------------------
	public static boolean deleteFile(String instrFilePath)
	{
		File f = new File(instrFilePath);
		
		if(!f.exists() || !f.isFile())
			return false;
		
		return f.delete();
	}
	//----------------------------------------------------------------
	/// Delete File
	///
	/// Deletes a file
	///
	/// @param the file to delete.
	/// @return whether or not this was successful.
	//----------------------------------------------------------------
	public static boolean deleteFile(File inFile)
	{
		if(!inFile.exists() || !inFile.isFile())
			return false;
		
		return inFile.delete();
	}
	//----------------------------------------------------------------
	/// Create Directory
	///
	/// Recursively creates directories.
	///
	/// @param The directory to create
	/// @return Whether or not this was successful. Failure to create
	///			the directory because it already exists counts as
	///			success.
	//----------------------------------------------------------------
	public static boolean createDirectory(String instrDirectory)
	{
		//break the path onto sections.
		String strDirectory = StringUtils.standardiseFilepath(instrDirectory);
		String[] strPathSections = strDirectory.split("/");
		
		String strPathSoFar = "";
		for (String strPathSection: strPathSections)
		{
			strPathSoFar += strPathSection + "/";
			
			File newDir = new File(strPathSoFar);
			if (newDir.exists() == false)
			{
				if (newDir.mkdir() == false)
				{
					Logging.logError("Failed to create directory: " + strPathSoFar);
					return false;
				}
			}
		}
		
		return true;
	}
	//----------------------------------------------------------------
	/// Delete Directory
	///
	/// Deletes a directory and all its contents.
	///
	/// @param The directory to delete.
	/// @return Whether or not this was successful. Failure to delete
	///			the directory because it does not exist counts as
	///			success.
	//----------------------------------------------------------------
	public static boolean deleteDirectory(String instrDirectory)
	{
		File directory = new File(instrDirectory);

		if (!directory.exists())
			return true;
		if (!directory.isDirectory())
			return false;
		
		String[] astrList = directory.list();
		if (astrList != null) 
		{
			for (int i = 0; i < astrList.length; i++) 
			{
				File entry = new File(directory, astrList[i]);
				if (entry.isDirectory())
				{
					if (!deleteDirectory(entry.getAbsolutePath()))
						return false;
				}
				else
				{
					if (!entry.delete())
						return false;
				}
			}
		}
		return directory.delete();
	}
	//----------------------------------------------------------------
	/// Get Filenames In Directory
	///
	/// returns all the filenames in the given directory.
	///
	/// @param the directory name.
	/// @param the list of files.
	//----------------------------------------------------------------
	static public String[] getFilenamesInDirectory(String instrDirectory)
	{
		String strDirectory = StringUtils.standardiseFilepath(instrDirectory);
		String[] astrFilenames = getFullFilenamesInDirectory(strDirectory);
		
		for (int i = 0; i < astrFilenames.length; ++i)
			astrFilenames[i] = astrFilenames[i].substring(strDirectory.length());
		
		return astrFilenames;
	}
	//----------------------------------------------------------------
	/// Get Full Filenames In Directory
	///
	/// returns all the filenames in the given directory. Files names
	/// include the input directory.
	///
	/// @param the directory name.
	/// @param the list of files.
	//----------------------------------------------------------------
	static public String[] getFullFilenamesInDirectory(String instrDirectory)
	{
		String strDirectory = StringUtils.standardiseFilepath(instrDirectory);
		ArrayList<String> astrFilenames = new ArrayList<String>();
		
		File directory = new File(strDirectory);
		if (!directory.exists() || !directory.isDirectory())
			return new String[0];
		
		String[] astrList = directory.list();
		if (astrList != null) 
		{
			for (int i = 0; i < astrList.length; i++) 
			{
				File entry = new File(directory, astrList[i]);
				if (entry.isDirectory())
				{
					String[] astrMoreFilenames = getFullFilenamesInDirectory(entry.getPath());
					astrFilenames.addAll(Arrays.asList(astrMoreFilenames));
				}
				else
				{
					astrFilenames.add(entry.getPath());
				}
			}
		}
		
		String[] astrOutput = new String[astrFilenames.size()];
		astrFilenames.toArray(astrOutput);
		return astrOutput;
	}
	//----------------------------------------------------------------
	/// Get Directories In Directory
	///
	/// returns all the directories in the given directory.
	///
	/// @param the directory name.
	/// @param the list of directories.
	//----------------------------------------------------------------
	static public String[] getDirectoriesInDirectory(String instrDirectory)
	{
		String strDirectory = StringUtils.standardiseFilepath(instrDirectory);
		String[] astrDirectories = getFullDirectoriesInDirectory(strDirectory);
		
		for (int i = 0; i < astrDirectories.length; ++i)
			astrDirectories[i] = astrDirectories[i].substring(strDirectory.length());
		
		return astrDirectories;
	}
	//----------------------------------------------------------------
	/// Get Full Directories In Directory
	///
	/// returns all the directories in the given directory. directory names
	/// include the input directory.
	///
	/// @param the directory name.
	/// @param the list of files.
	//----------------------------------------------------------------
	static public String[] getFullDirectoriesInDirectory(String instrDirectory)
	{
		String strDirectory = StringUtils.standardiseFilepath(instrDirectory);
		ArrayList<String> astrDirectories = new ArrayList<String>();
		
		File directory = new File(strDirectory);
		if (!directory.exists() || !directory.isDirectory())
			return new String[0];
		
		String[] astrList = directory.list();
		if (astrList != null) 
		{
			for (int i = 0; i < astrList.length; i++) 
			{
				File entry = new File(directory, astrList[i]);
				if (entry.isDirectory())
				{
					astrDirectories.add(entry.getPath());
					String[] astrMoreDirectories = getFullDirectoriesInDirectory(entry.getPath());
					astrDirectories.addAll(Arrays.asList(astrMoreDirectories));
				}
			}
		}
		
		String[] astrOutput = new String[astrDirectories.size()];
		astrDirectories.toArray(astrOutput);
		return astrOutput;
	}
	//----------------------------------------------------------------
	/// Calculate File Checksum
	///
	/// calculates the checksum for the file at the given path.
	///
	/// @param the filename.
	/// @return the checksum.
	//----------------------------------------------------------------
	static public String calculateFileChecksum(String instrFilename)
	{
		String strChecksum = "";
		
		try
		{
			InputStream fis =  new FileInputStream(instrFilename);
	
			byte[] buffer = new byte[1024];
			MessageDigest complete = MessageDigest.getInstance("MD5");
			int numRead;
			
			do 
			{
			    numRead = fis.read(buffer);
			    if (numRead > 0) 
			    {
			        complete.update(buffer, 0, numRead);
			    }
			} 
			while (numRead != -1);
			
			fis.close();
			byte[] abyChecksum = complete.digest();
			
			for (int i=0; i < abyChecksum.length; i++) 
			{
				strChecksum += Integer.toString((abyChecksum[i] & 0xff) + 0x100, 16).substring(1);
		    }
		}
		catch (Exception e)
		{
			Logging.logError("Could not calculate checksum for file: " + instrFilename);
		}
		
		return strChecksum;
	}
	//----------------------------------------------------------------
	/// Get Path To Here
	///
	/// @return the full path to the jar files location.
	//----------------------------------------------------------------
	static public String getPathToHere()
	{
		String strPathToHere = "";
		try
		{
			CodeSource codeSource = FileUtils.class.getProtectionDomain().getCodeSource();
			File jarFile = new File(codeSource.getLocation().toURI().getPath());
			strPathToHere = jarFile.getParentFile().getPath();
			strPathToHere = StringUtils.standardiseFilepath(strPathToHere);
		}
		catch (Exception e)
		{
			Logging.logError("Failed to get path to here.");
		}
		return strPathToHere;
	}
}
