package com.taggames.pngalphapremultiplier;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URLDecoder;
import java.util.LinkedList;

import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.StringUtils;
import com.chillisource.toolutils.Logging.LoggingLevel;

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
			String strInput = StringUtils.standardiseFilepath(insOptions.strInputFilename);
			String strOutput = StringUtils.standardiseFilepath(insOptions.strOutputFilename);
			
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
			case k_none:
				astrCommands.add("--logginglevel");
				astrCommands.add("none");
				break;
			case k_fatal:
				astrCommands.add("--logginglevel");
				astrCommands.add("fatal");
				break;
			case k_error:
				astrCommands.add("--logginglevel");
				astrCommands.add("error");
				break;
			case k_warning:
				astrCommands.add("--logginglevel");
				astrCommands.add("warning");
				break;
			case k_verbose:
				astrCommands.add("--logginglevel");
				astrCommands.add("verbose");
				break;
			}
			
			astrCommands.add("--errordisplay");
			astrCommands.add("whenrecieved");
			
			Logging.logVerbose("Premultiplying " + strInput);
			
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
			Logging.logFatal("Exception occurred in Premultiply(): \n" + e.getStackTrace());
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
			String strUndecodedPathToHere = Main.class.getProtectionDomain().getCodeSource().getLocation().getPath();
			String strPathToHere = URLDecoder.decode(strUndecodedPathToHere, "UTF-8");
			strPathToHere = strPathToHere.substring(0, strPathToHere.lastIndexOf("/") + 1);
			StringUtils.standardiseFilepath(strPathToHere);
			
			//get which version of the executable should be used.
			String strOS = System.getProperty("os.name");
			
			if (strOS.startsWith("Windows") == true)
				strExecutableName = strPathToHere + "PreMultipliedAlphaPNGTool/PreMultipliedAlphaPNGToolWindows.exe";
			else if (strOS.startsWith("Mac") == true)
				strExecutableName = strPathToHere + "PreMultipliedAlphaPNGTool/PreMultipliedAlphaPNGToolOSX";
			else if (strOS.startsWith("Linux") == true)
				strExecutableName = strPathToHere + "PreMultipliedAlphaPNGTool/PreMultipliedAlphaPNGToolLinux";
			else
				Logging.logFatal("This platform is not supported!");
		}
		catch (Exception e)
		{
			Logging.logFatal("Exception occurred in GetExecutableName(): \n" + e.getStackTrace());
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
					Logging.logVerbose("  " + inputLine);
				}
			}
			while (inputLine != null);
		}
		catch (Exception e)
		{
			Logging.logFatal("Something has gone wrong while reading the error stream!\n" + e.getStackTrace().toString());
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
					if (Logging.getLoggingLevel() == LoggingLevel.k_verbose)
						Logging.logVerbose("  " + errorLine);
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
