/**
 * PngPremultiplier.java
 * Chilli Source
 * Created by Ian Copland on 30/01/2013.
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

package com.chilliworks.chillisource.pngalphapremultiplier;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URISyntaxException;
import java.security.CodeSource;
import java.util.LinkedList;

import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;

public class PngPremultiplier 
{
	//-------------------------------------------------
	/// Premultiply
	///
	/// This uses the native tool to pre-multiply the
	/// pngs and keeps caches the png if requested.
	///
	/// @param the pre-multiplication options.
	//-------------------------------------------------
	public static void Premultiply(PremultiplicationOptions insOptions)
	{
		try
		{	
			String strInput = StringUtils.standardiseFilePath(insOptions.strInputFilename);
			String strOutput = StringUtils.standardiseFilePath(insOptions.strOutputFilename);
			
			//build the command list
			LinkedList<String> astrCommands = new LinkedList<String>();
			astrCommands.add(GetExecutableName());
			astrCommands.add("--input");
			astrCommands.add(strInput);
			astrCommands.add("--output");
			astrCommands.add(strOutput);
			
			//add the logging level to the command list
			switch (Logging.getLoggingLevel())
			{
			case NONE:
				astrCommands.add("--logginglevel");
				astrCommands.add("none");
				break;
			case FATAL:
				astrCommands.add("--logginglevel");
				astrCommands.add("fatal");
				break;
			case ERROR:
				astrCommands.add("--logginglevel");
				astrCommands.add("error");
				break;
			case WARNING:
				astrCommands.add("--logginglevel");
				astrCommands.add("warning");
				break;
			case VERBOSE:
				astrCommands.add("--logginglevel");
				astrCommands.add("warning");
				break;
			}
			
			Logging.logVerbose("Premultiplying " + strInput);
			
			//run the application
			final Process process = new ProcessBuilder(astrCommands).start();
	
			//create runnables for polling the error streams.
			Runnable errorRunnable = new Runnable()
			{
				@Override public void run() 
				{
					ReadErrorStream(process);
				}
			};
			
			//start these runnables.
			Thread errorThread = new Thread(errorRunnable);
			errorThread.start();
			
			//wait for the threads to finish
			errorThread.join();
			
			//end the process
			process.destroy();
		}
		catch (Exception e)
		{
			Logging.logFatal("Exception occurred in Premultiply(): \n" + StringUtils.convertExceptionToString(e));
		}
	}
	/**
	 * Get the directory path that contains this jar file
	 * 
	 * @author S Downie
	 * 
	 * @return Directory path to jar folder
	 */
	private static String getJarDirectoryPath()
	{
		String jarDir = "";
		
		CodeSource codeSource = PngPremultiplier.class.getProtectionDomain().getCodeSource();
		File jarFile;
		try 
		{
			jarFile = new File(codeSource.getLocation().toURI().getPath());
			jarDir = jarFile.getParentFile().getPath() + File.separator;
		} 
		catch (URISyntaxException e) 
		{
			e.printStackTrace();
		}
		 
		return jarDir;
	}
	//------------------------------------------------------
	/// GetExecutableName
	///
	/// @return returns the name of the executable that
	/// should be used for this platform.
	//------------------------------------------------------
	private static String GetExecutableName()
	{
		String strExecutableName = "";
		
		try
		{
			//get the path to the jar
			String strPathToHere = getJarDirectoryPath();
			
			//get which version of the executable should be used.
			String strOS = System.getProperty("os.name");
			
			if (strOS.startsWith("Windows") == true)
				strExecutableName = strPathToHere + "PNGAlphaPremultiplier\\PNGAlphaPremultiplierWindows.exe";
			else if (strOS.startsWith("Mac") == true)
				strExecutableName = strPathToHere + "PNGAlphaPremultiplier/PNGAlphaPremultiplierOSX";
			else if (strOS.startsWith("Linux") == true)
				strExecutableName = strPathToHere + "PNGAlphaPremultiplier/PNGAlphaPremultiplierLinux";
			else
				Logging.logFatal("This platform is not supported!");
		}
		catch (Exception e)
		{
			Logging.logFatal("Exception occurred in GetExecutableName(): \n" + StringUtils.convertExceptionToString(e));
		}
		
		return strExecutableName;
	}
	//------------------------------------------------------
	/// Read Error Stream
	///
	/// reads and outputs the processes error stream.
	///
	/// @param the process that owns the error stream.
	//------------------------------------------------------
	private static void ReadErrorStream(Process inProcess)
	{
		try
		{
			InputStream errorStream = inProcess.getErrorStream();
			InputStreamReader errorStreamReader = new InputStreamReader(errorStream);
			BufferedReader bufferedErrorReader = new BufferedReader(errorStreamReader);
			
			String errorLine = null;
			do
			{
				errorLine = bufferedErrorReader.readLine();
				if (errorLine != null)
				{
					if (errorLine.startsWith("WARNING:") == true)
					{
						Logging.logWarning(errorLine.substring(8));
					}
					else if (errorLine.startsWith("ERROR:") == true)
					{
						Logging.logError(errorLine.substring(6));
					}
					else if (errorLine.startsWith("FATAL:") == true)
					{
						Logging.logFatal(errorLine.substring(6));
					}
				}
			}
			while (errorLine != null);
		}
		catch (Exception e)
		{
			Logging.logFatal("Something has gone wrong while reading the error stream!\n" + StringUtils.convertExceptionToString(e));
		}
	}
}
