/**
 * ExternalProcess.java
 * Chilli Source
 * Created by Ian Copland on 07/09/2012
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
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

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.LinkedList;
import java.util.List;

//==================================================
/// External Process
///
/// Runs external processes.
//==================================================
public final class ExternalProcess 
{
	//------------------------------------------------------
	/// Private member data
	//------------------------------------------------------
	private List<String> mstrMessages = new LinkedList<String>();
	private List<String> mstrWarnings = new LinkedList<String>();
	private List<String> mstrErrors = new LinkedList<String>();
	//----------------------------------------------
	/// Constructor
	//----------------------------------------------
	public ExternalProcess()
	{
	}
	//----------------------------------------------
	/// Constructor
	//----------------------------------------------
	public ExternalProcess(List<String> inastrCommands)
	{
		run(inastrCommands);
	}
	//------------------------------------------------------
	/// Run External Process
	///
	/// reads and outputs the processes input stream.
	///
	/// @param the process that owns the input stream.
	//------------------------------------------------------
	public void run(List<String> inastrCommands)
	{
		try
		{
			//run the application
			final Process process = new ProcessBuilder(inastrCommands).start();
			Runnable inputRunnable = new Runnable()
			{
				@Override public void run() 
				{
					readInputStream(process);
				}
			};
			Runnable errorRunnable = new Runnable()
			{
				@Override public void run() 
				{
					readErrorStream(process);
				}
			};
			Thread inputThread = new Thread(inputRunnable);
			Thread errorThread = new Thread(errorRunnable);
			inputThread.start();
			errorThread.start();
			inputThread.join();
			errorThread.join();
			process.waitFor();
			process.destroy();
		}
		catch (Exception e)
		{
			Logging.logFatal("Exception occurred while running external process: \n" + e.getStackTrace());
		}
	}
	//------------------------------------------------------
	/// Get Messages
	///
	/// @return all messages from the run application.
	//------------------------------------------------------
	public List<String> getMessages()
	{
		return mstrMessages;
	}
	//------------------------------------------------------
	/// Get Warnings
	///
	/// @return all warnings from the run application.
	//------------------------------------------------------
	public List<String> getWarnings()
	{
		return mstrWarnings;
	}
	//------------------------------------------------------
	/// Get Errors
	///
	/// @return all errors from the run application.
	//------------------------------------------------------
	public List<String> getErrors()
	{
		return mstrErrors;
	}
	//------------------------------------------------------
	/// Read Input Stream
	///
	/// reads and outputs the processes input stream.
	///
	/// @param the process that owns the input stream.
	//------------------------------------------------------
	private void readInputStream(Process inProcess)
	{
		try
		{
			InputStream inputStream = inProcess.getInputStream();
			InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
			BufferedReader bufferedInputReader = new BufferedReader(inputStreamReader);
			
			String inputLine = null;
			do
			{
				inputLine = bufferedInputReader.readLine();
				if (inputLine != null)
				{	
					mstrMessages.add(inputLine);
				}
			}
			while (inputLine != null);
		}
		catch (Exception e)
		{
			Logging.logFatal("Something has gone wrong while reading the input stream!\n" + e.getStackTrace().toString());
		}
	}
	//------------------------------------------------------
	/// Read Error Stream
	///
	/// reads and outputs the processes error stream.
	///
	/// @param the process that owns the error stream.
	//------------------------------------------------------
	private void readErrorStream(Process inProcess)
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
					String strMessage;
					if (errorLine.startsWith("FATAL") == true)
					{
						strMessage = errorLine.substring(7);
						mstrErrors.add(strMessage);
					}
					else if (errorLine.startsWith("ERROR") == true)
					{
						strMessage = errorLine.substring(7);
						mstrErrors.add(strMessage);
					}
					else if (errorLine.startsWith("WARNING") == true)
					{
						strMessage = errorLine.substring(9);
						mstrWarnings.add(strMessage);
					}
					else
					{
						strMessage = errorLine;
						mstrErrors.add(strMessage);
					}
		
				}
			}
			while (errorLine != null);
		}
		catch (Exception e)
		{
			Logging.logFatal("Something has gone wrong while reading the error stream!\n" + e.getStackTrace().toString());
		}
	}
}
