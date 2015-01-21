/**
 * CkTool.java
 * Chilli Source
 * Created by Ian Copland on 20/01/2015.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Tag Games Limited
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

package com.chilliworks.chillisource.cktool;

import java.io.File;
import java.net.URISyntaxException;
import java.security.CodeSource;
import java.util.LinkedList;
import java.util.List;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.ExternalProcess;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;

/**
 * A basic wrapper arround the Cricket Audio cktool. This is used to make it easier
 * to use the tool accross different platforms.
 * 
 * @author Ian Copland
 */
public final class CkTool 
{
	/**
	 * Executes CkTool using the given options.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options.
	 * 
	 * @throws CSException - Any exception that occurs while executing the CkTool.
	 */
	public static void Execute(Options in_options) throws CSException
	{
		List<String> commandList = BuildCommandList(in_options);
		ExternalProcess process = new ExternalProcess(commandList);
		
		List<String> messages = process.getMessages();
		for (String message : messages)
		{
			if (message.toLowerCase().startsWith("error:") == true)
			{
				throw new CSException(message.substring(6));
			}
			else
			{
				Logging.logVerbose(message);
			}
		}
	}
	/**
	 * Builds the list of commands to execute from the given options.
	 * 
	 * @author Ian Copland
	 * 
	 * @return Directory path to jar folder
	 * 
	 * @throws CSException - Any exception that occurs while building the command list.
	 */
	private static List<String> BuildCommandList(Options in_options) throws CSException
	{
		LinkedList<String> commands = new LinkedList<String>();
		commands.add(GetExecutableFilePath());
		commands.add(ActionToString(in_options.m_action));
		
		if (Logging.getLoggingLevel() == Logging.LoggingLevel.VERBOSE)
		{
			commands.add("-verbose");
		}
		
		if (in_options.m_action == Options.Action.BUILD_BANK_SINGLE)
		{
			commands.add("-single");
		}
		
		if (in_options.m_format.isEmpty() == false)
		{
			commands.add("-format");
			commands.add(in_options.m_format);
		}
		
		if (in_options.m_volume.isEmpty() == false)
		{
			commands.add("-volume");
			commands.add(in_options.m_volume);
		}
		
		if (in_options.m_pan.isEmpty() == false)
		{
			commands.add("-pan");
			commands.add(in_options.m_pan);
		}
		
		if (in_options.m_loopStart.isEmpty() == false)
		{
			commands.add("-loopStart");
			commands.add(in_options.m_loopStart);
		}
		
		if (in_options.m_loopEnd.isEmpty() == false)
		{
			commands.add("-loopEnd");
			commands.add(in_options.m_loopEnd);
		}
		
		if (in_options.m_loopCount.isEmpty() == false)
		{
			commands.add("-loopCount");
			commands.add(in_options.m_loopCount);
		}
		
		commands.add(StringUtils.standardiseFilePath(in_options.m_inputFilePath));
		commands.add(StringUtils.standardiseFilePath(in_options.m_outputFilePath));
		return commands;
	}
	/**
	 * Get the directory path that contains this jar file. Note: This REQUIRES that the 
	 * jar be exported as a normal jar, NOT a runnable jar, otherwise it won't work.
	 * 
	 * @author Ian Copland
	 * 
	 * @return The directory path to jar folder.
	 * 
	 * @throws CSException - An exception thrown if the directory path cannot be calculated. 
	 */
	private static String getJarDirectoryPath() throws CSException
	{
		String jarDir = "";
		
		CodeSource codeSource = CkTool.class.getProtectionDomain().getCodeSource();
		File jarFile;
		try 
		{
			jarFile = new File(codeSource.getLocation().toURI().getPath());
			jarDir = jarFile.getParentFile().getPath() + File.separator;
		} 
		catch (URISyntaxException e) 
		{
			throw new CSException("An exception occurred while calculating the jar directory path().", e);
		}
		 
		return jarDir;
	}
	/**
	 * Returns the file path to the executable.
	 * 
	 * @author Ian Copland
	 * 
	 * @return The path of the exectable.
	 * 
	 * @throws CSException - An exception thrown if the file path cannot be calculated.
	 */
	private static String GetExecutableFilePath() throws CSException
	{
		String strExecutableName = "";
		
		try
		{
			//get the path to the jar
			String strPathToHere = getJarDirectoryPath();
			
			//get which version of the executable should be used.
			String strOS = System.getProperty("os.name");
			
			if (strOS.startsWith("Windows") == true)
			{
				strExecutableName = strPathToHere + "CkTool\\Windows\\cktool.exe";
			}
			else if (strOS.startsWith("Mac") == true)
			{
				strExecutableName = strPathToHere + "CkTool/OSX/cktool";
			}
			else if (strOS.startsWith("Linux") == true)
			{
				strExecutableName = strPathToHere + "CkTool/Linux/cktool";
			}
			else
			{
				Logging.logFatal("This platform is not supported!");
			}
		}
		catch (Exception e)
		{
			throw new CSException("Exception occurred in GetExecutableName()", e);
		}
		
		return strExecutableName;
	}
	/**
	 * Converts the given action to a string.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_action - The action to convert to string.
	 * 
	 * @return The action as a string.
	 * 
	 * @throws CSException - An exception thrown if the action cannot be converted to string.
	 */
	private static String ActionToString(Options.Action in_action) throws CSException
	{
		switch (in_action)
		{
		case BUILD_BANK:
		case BUILD_BANK_SINGLE:
			return "buildbank";
		case BUILD_STREAM:
			return "buildstream";
		default:
			throw new CSException("Invalid action!");
		}
	}
}
