package com.taggames.premultipliedalphapngtool;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URLDecoder;
import java.util.LinkedList;

import com.taggames.toolutils.SCLogger;
import com.taggames.toolutils.SCStringUtils;

public class SCPngPremultiplier 
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
			String strInput = SCStringUtils.StandardiseFilepath(insOptions.strInputFilename);
			String strOutput = SCStringUtils.StandardiseFilepath(insOptions.strOutputFilename);
			
			//build the command list
			LinkedList<String> astrCommands = new LinkedList<String>();
			astrCommands.add(GetExecutableName());
			astrCommands.add("--input");
			astrCommands.add(strInput);
			astrCommands.add("--output");
			astrCommands.add(strOutput);
			
			//add the logging level to the command list
			switch (SCLogger.GetLoggingLevel())
			{
			case SCLogger.LOGGING_LEVEL_NONE:
				astrCommands.add("--logginglevel");
				astrCommands.add("none");
				break;
			case SCLogger.LOGGING_LEVEL_FATAL:
				astrCommands.add("--logginglevel");
				astrCommands.add("fatal");
				break;
			case SCLogger.LOGGING_LEVEL_ERROR:
				astrCommands.add("--logginglevel");
				astrCommands.add("error");
				break;
			case SCLogger.LOGGING_LEVEL_WARNING:
				astrCommands.add("--logginglevel");
				astrCommands.add("warning");
				break;
			case SCLogger.LOGGING_LEVEL_VERBOSE:
				astrCommands.add("--logginglevel");
				astrCommands.add("verbose");
				break;
			}
			
			//add when to display error messages to the command list.
			switch (SCLogger.GetWhenToDisplayErrors())
			{
			case SCLogger.DISPLAY_ERRORS_NEVER:
				astrCommands.add("--errordisplay");
				astrCommands.add("never");
				break;
			case SCLogger.DISPLAY_ERRORS_AT_END:
				astrCommands.add("--errordisplay");
				astrCommands.add("atend");
				break;
			case SCLogger.DISPLAY_ERRORS_WHEN_RECIEVED:
				astrCommands.add("--errordisplay");
				astrCommands.add("whenrecieved");
				break;
			}
			
			SCLogger.LogMessage("Premultiplying " + strInput);
			
			//run the application
			final Process process = new ProcessBuilder(astrCommands).start();
	
			//create runnables for polling the input and error streams.
			Runnable inputRunnable = new Runnable()
			{
				@Override public void run() 
				{
					ReadInputStream(process);
				}
			};
			Runnable errorRunnable = new Runnable()
			{
				@Override public void run() 
				{
					ReadErrorStream(process);
				}
			};
			
			//start these runnables.
			Thread inputThread = new Thread(inputRunnable);
			Thread errorThread = new Thread(errorRunnable);
			inputThread.start();
			errorThread.start();
			
			//wait for the threads to finish
			inputThread.join();
			errorThread.join();
			
			//end the process
			process.destroy();
		}
		catch (Exception e)
		{
			SCLogger.LogFatalError("Exception occurred in Premultiply(): \n" + e.getStackTrace());
		}
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
			String strUndecodedPathToHere = CMain.class.getProtectionDomain().getCodeSource().getLocation().getPath();
			String strPathToHere = URLDecoder.decode(strUndecodedPathToHere, "UTF-8");
			strPathToHere = strPathToHere.substring(0, strPathToHere.lastIndexOf("/") + 1);
			SCStringUtils.StandardiseFilepath(strPathToHere);
			
			//get which version of the executable should be used.
			String strOS = System.getProperty("os.name");
			
			if (strOS.startsWith("Windows") == true)
				strExecutableName = strPathToHere + "PreMultipliedAlphaPNGTool/PreMultipliedAlphaPNGToolWindows.exe";
			else if (strOS.startsWith("Mac") == true)
				strExecutableName = strPathToHere + "PreMultipliedAlphaPNGTool/PreMultipliedAlphaPNGToolOSX";
			else if (strOS.startsWith("Linux") == true)
				strExecutableName = strPathToHere + "PreMultipliedAlphaPNGTool/PreMultipliedAlphaPNGToolLinux";
			else
				SCLogger.LogFatalError("This platform is not supported!");
		}
		catch (Exception e)
		{
			SCLogger.LogFatalError("Exception occurred in GetExecutableName(): \n" + e.getStackTrace());
		}
		
		return strExecutableName;
	}
	//------------------------------------------------------
	/// Read Input Stream
	///
	/// reads and outputs the processes input stream.
	///
	/// @param the process that owns the input stream.
	//------------------------------------------------------
	private static void ReadInputStream(Process inProcess)
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
					SCLogger.LogMessage("  " + inputLine);
				}
			}
			while (inputLine != null);
		}
		catch (Exception e)
		{
			SCLogger.LogFatalError("Something has gone wrong while reading the error stream!\n" + e.getStackTrace().toString());
		}
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
					if (SCLogger.GetLoggingLevel() == SCLogger.LOGGING_LEVEL_VERBOSE)
						SCLogger.LogMessage("  " + errorLine);
					
					if (SCLogger.GetWhenToDisplayErrors() == SCLogger.DISPLAY_ERRORS_AT_END)
					{
						if (errorLine.startsWith("FATAL") == true)
							SCLogger.LogFatalError(errorLine.substring(7));
						else if (errorLine.startsWith("ERROR") == true)
							SCLogger.LogError(errorLine.substring(7));
						else if (errorLine.startsWith("WARNING") == true)
							SCLogger.LogWarning(errorLine.substring(9));
					}
				}
			}
			while (errorLine != null);
		}
		catch (Exception e)
		{
			SCLogger.LogFatalError("Something has gone wrong while reading the error stream!\n" + e.getStackTrace().toString());
		}
	}
}
