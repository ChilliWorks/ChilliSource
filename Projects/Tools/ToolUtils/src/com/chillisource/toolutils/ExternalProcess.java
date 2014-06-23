//
//	ExternalProcess.java
//	Toolchain Manager
//
//	Created by Ian Copland on 07/09/2012
//	Copyright (c) 2012 Tag Games Ltd. All rights reserved.
//

package com.chillisource.toolutils;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.LinkedList;

//==================================================
/// External Process
///
/// Runs external processes.
//==================================================
public class ExternalProcess 
{
	//------------------------------------------------------
	/// Private member data
	//------------------------------------------------------
	private LinkedList<String> mstrMessages = new LinkedList<String>();
	private LinkedList<String> mstrWarnings = new LinkedList<String>();
	private LinkedList<String> mstrErrors = new LinkedList<String>();
	//----------------------------------------------
	/// Constructor
	//----------------------------------------------
	public ExternalProcess()
	{
	}
	//----------------------------------------------
	/// Constructor
	//----------------------------------------------
	public ExternalProcess(LinkedList<String> inastrCommands)
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
	public void run(LinkedList<String> inastrCommands)
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
	public LinkedList<String> getMessages()
	{
		return mstrMessages;
	}
	//------------------------------------------------------
	/// Get Warnings
	///
	/// @return all warnings from the run application.
	//------------------------------------------------------
	public LinkedList<String> getWarnings()
	{
		return mstrWarnings;
	}
	//------------------------------------------------------
	/// Get Errors
	///
	/// @return all errors from the run application.
	//------------------------------------------------------
	public LinkedList<String> getErrors()
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
