/**
 * CSProjectGenerator.java
 * Chilli Source
 * Created by Ian Copland on 15/10/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
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

package com.chilliworks.chillisource.csprojectgenerator;

import java.io.File;
import java.net.URISyntaxException;
import java.security.CodeSource;

import net.lingala.zip4j.core.ZipFile;
import net.lingala.zip4j.exception.ZipException;

import com.chilliworks.chillisource.toolutils.Logging;
import com.chilliworks.chillisource.toolutils.StringUtils;

/**
 * A collection of methods for generating new Chilli Source projects.
 * 
 * @author Ian Copland
 */
public final class CSProjectGenerator
{
	private static final String k_projectZipFilePath = "CSProjectGenerator/CSEmptyTemplate.zip";
	
	/**
	 * Generates a new Chilli Source project using the given options.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 */
	public static void generate(Options in_options)
	{
		validateOptions(in_options);
		unzipProject(in_options.m_outputDirectory);
	}
	/**
	 * Confirms that the given options are valid.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options.
	 */
	private static void validateOptions(Options in_options)
	{
		if (in_options.m_projectName.matches("[a-zA-Z0-9]+") == false)
		{
			Logging.logFatal("Package name can only contain: a-z, A-Z or 0-9.");
		}
		
		if (in_options.m_packageName.matches("[a-z.]+") == false)
		{
			Logging.logFatal("Package name can only contain: a-z and '.'");
		}
		
		//TODO: confirm output directory is empty.
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
		
		CodeSource codeSource = CSProjectGenerator.class.getProtectionDomain().getCodeSource();
		File jarFile;
		try 
		{
			jarFile = new File(codeSource.getLocation().toURI().getPath());
			jarDir = jarFile.getParentFile().getPath() + File.separator;
		} 
		catch (URISyntaxException e) 
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			Logging.logFatal("Could not get jar directory path.");
		}
		 
		return StringUtils.standardiseDirectoryPath(jarDir);
	}
	/**
	 * unzips the project to the given directory.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 */
	private static void unzipProject(String in_outputDirectory)
	{
		String pathToZip = getJarDirectoryPath() + k_projectZipFilePath;
		
		try 
		{
	         ZipFile zipFile = new ZipFile(pathToZip);
	         if (zipFile.isEncrypted())
	         {
	            Logging.logFatal("Could not open project zip file.");
	         }
	         zipFile.extractAll(in_outputDirectory);
	    } 
		catch (ZipException e) 
	    {
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
	        Logging.logFatal("Could not open project zip file.");
	    }
	}
}
