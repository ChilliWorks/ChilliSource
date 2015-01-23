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
import java.io.IOException;
import java.net.URISyntaxException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.CodeSource;
import java.util.LinkedList;

import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.coreutils.ZipUtils;

/**
 * A collection of methods for generating new Chilli Source projects.
 * 
 * @author Ian Copland
 */
public final class CSProjectGenerator
{
	private static final String k_templateProjectName = "CSEmptyTemplate";
	private static final String k_templatePackageName = "ext.company.csemptytemplate";
	private static final String k_projectZipFilePath = "CSProjectGenerator/" + k_templateProjectName + ".zip";
	private static final String k_csSourceDirectoryPath = "../";
	private static final String k_csDestinationDirectoryPath = "ChilliSource/";
	private static final String k_tempDirectory = "_temp-csprojectgenerator/";
	
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
		unzipProject(in_options);
		updateProjectFiles(in_options);
		copyChilliSource(in_options);
		copyToOutput(in_options);
		cleanupTemp(in_options);
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
		//check that the project name is valid.
		if (in_options.m_projectName.matches("[a-zA-Z0-9]+") == false)
		{
			Logging.logFatal("Project name can only contain: a-z, A-Z or 0-9.");
		}
		
		//check that that package name is valid
		if (in_options.m_packageName.matches("[a-z.]+") == false)
		{
			Logging.logFatal("Package name can only contain: a-z and '.'");
		}
		
		//confirm that the project path doesn't already exist.
		String outputProjectDirectoryPathName = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + in_options.m_projectName);
		File outputProjectDirectory = new File(outputProjectDirectoryPathName);
		if (outputProjectDirectory.exists() == true)
		{
			Logging.logFatal("Output project directory '" + outputProjectDirectoryPathName + "' already exists!");
		}
		
		//ensure the output path is not inside Chilli Source as this will break copying of files.
		try
		{
			String outputProjectDirectoryPathNameAbs = outputProjectDirectory.getCanonicalPath();
			String csDirectoryPathName = StringUtils.standardiseDirectoryPath(getJarDirectoryPath(in_options) + k_csSourceDirectoryPath); 
			String csDirectoryPathNameAbs = new File(csDirectoryPathName).getCanonicalPath();
		
			Path outputProjectDirectoryPath = Paths.get(outputProjectDirectoryPathNameAbs);
			Path csDirectoryPath = Paths.get(csDirectoryPathNameAbs);

			if (outputProjectDirectoryPath.startsWith(csDirectoryPath) == true)
			{
				Logging.logFatal("Output project directory '" + outputProjectDirectoryPathName + "' cannot be located inside Chilli Source.");
			}
		}
		catch (Exception e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			Logging.logFatal("Failed to validate output directory path.");
		}
	}
	/**
	 * unzips the project to the temp directory.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 */
	private static void unzipProject(Options in_options)
	{
		try 
		{
			Logging.logVerbose("Unzipping project to temporary directory.");
			
			String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
			if (new File(tempDirectoryPath).exists() == true)
			{
				Logging.logFatal("Temp directory already exists.");
			}
			
			String pathToZip = getJarDirectoryPath(in_options) + k_projectZipFilePath;
			ZipUtils.extract(pathToZip, tempDirectoryPath);
	    } 
		catch (IOException e) 
	    {
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			
			cleanupTemp(in_options);
	        Logging.logFatal("Could not open project zip file: " + k_projectZipFilePath);
	    }
	}
	/**
	 * Get the directory path that contains this jar file
	 * 
	 * @author S Downie
	 * 
	 * @param in_options - The options with which to create the new project.
	 * 
	 * @return Directory path to jar folder
	 */
	private static String getJarDirectoryPath(Options in_options)
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
			
			cleanupTemp(in_options);
			Logging.logFatal("Could not get jar directory path.");
		}
		 
		return StringUtils.standardiseDirectoryPath(jarDir);
	}
	/**
	 * updates the project and package name throughout the project.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 */
	private static void updateProjectFiles(Options in_options)
	{
		String[] filesToUpdateContents = new String[]
		{
			"AppSource/App.cpp",
			"AppSource/App.h",
			"AppSource/State.cpp",
			"AppSource/State.h",
			"Content/BuildAll.bat",
			"Content/BuildAll.command",
			"Content/BuildAll.sh",
			"Content/AppResources/App.config",
			"Projects/iOS/" + k_templateProjectName + ".xcodeproj/project.pbxproj",
			"Projects/iOS/ProjectResources/" + k_templateProjectName + "-Info.plist",
			"Projects/Windows/" + k_templateProjectName + ".sln",
			"Projects/Windows/" + k_templateProjectName + ".vcxproj",
			"Projects/Android/.project",
			"Projects/Android/AndroidManifest.xml",
			"Projects/Android/CSAndroidManifest.xml",
			"Projects/Android/jni/Android.mk",
			"Projects/Android/jni/Application.mk",
			"Projects/Android/res/values/strings.xml",
			"Tools/Scripts/content_builder.py",
			"Tools/Scripts/file_system_utils.py",
			"Tools/Scripts/model_builder.py",
			"Tools/Scripts/text_builder.py",
			"Tools/Scripts/texture_atlas_builder.py",
			"Tools/Scripts/texture_builder.py"
		};
		
		updateFileContents(in_options, filesToUpdateContents);
		
		String[] filesToUpdateFileName = new String[]
		{
			"Projects/iOS/ProjectResources/" + k_templateProjectName + "-Info.plist",
			"Projects/Windows/" + k_templateProjectName + ".sln",
			"Projects/Windows/" + k_templateProjectName + ".vcxproj",
		};
		
		updateFileNames(in_options, filesToUpdateFileName);
		
		String[] directoriesToUpdateDirectoryName = new String[]
		{
			"Projects/iOS/" + k_templateProjectName + ".xcodeproj"
		};
		
		updateDirectoryNames(in_options, directoriesToUpdateDirectoryName);
	}
	/**
	 * Updates the contents of the files at the given file paths to the project 
	 * and package name given in the options.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 * @param in_filePaths - The array of file paths that should have their
	 * contents updated.
	 */
	private static void updateFileContents(Options in_options, String[] in_filePaths)
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		
		for (String filePath : in_filePaths)
		{
			Logging.logVerbose("Updating file contents: " + filePath);
			
			String fullFilePath = StringUtils.standardiseFilePath(tempProjectDirectory + filePath);
			String fileContents = FileUtils.readFile(fullFilePath);
			if (fileContents == null || fileContents.length() == 0)
			{
				cleanupTemp(in_options);
				Logging.logFatal("Could not read file: " + filePath);
			}
			
			fileContents = fileContents.replace(k_templateProjectName, in_options.m_projectName);
			fileContents = fileContents.replace(k_templatePackageName, in_options.m_packageName);
			
			if (FileUtils.writeFile(fullFilePath, fileContents) == false)
			{
				cleanupTemp(in_options);
				Logging.logFatal("Could not write file: " + filePath);
			}
		}
	}
	/**
	 * Updates the files names of the files at the given file paths to the project 
	 * and package name given in the options.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 * @param in_filePaths - The array of file paths that should have their
	 * file names updated.
	 */
	private static void updateFileNames(Options in_options, String[] in_filePaths)
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		
		for (String currentFilePath : in_filePaths)
		{
			Logging.logVerbose("Updating file name: " + currentFilePath);
			
			String newFilePath = currentFilePath.replace(k_templateProjectName, in_options.m_projectName);
			newFilePath = newFilePath.replace(k_templatePackageName, in_options.m_packageName);
			
			String currentFullFilePath = StringUtils.standardiseFilePath(tempProjectDirectory + currentFilePath);
			String newFullFilePath = StringUtils.standardiseFilePath(tempProjectDirectory + newFilePath);
			
			if (FileUtils.renameFile(currentFullFilePath, newFullFilePath) == false)
			{
				cleanupTemp(in_options);
				Logging.logFatal("Failed to rename file: " + currentFilePath);
			}
		}
	}
	/**
	 * Updates the directory names of the directories at the given directory paths 
	 * to the project and package name given in the options.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 * @param in_filePaths - The array of directory paths that should have their
	 * directory names updated.
	 */
	private static void updateDirectoryNames(Options in_options, String[] in_directoryPaths)
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		
		for (String currentDirectoryPath : in_directoryPaths)
		{
			Logging.logVerbose("Updating directory name: " + currentDirectoryPath);
			
			String newDirectoryPath = currentDirectoryPath.replace(k_templateProjectName, in_options.m_projectName);
			newDirectoryPath = newDirectoryPath.replace(k_templatePackageName, in_options.m_packageName);
			
			String currentFullDirectoryPath = StringUtils.standardiseFilePath(tempProjectDirectory + currentDirectoryPath);
			String newFullDirectoryPath = StringUtils.standardiseFilePath(tempProjectDirectory + newDirectoryPath);
			
			if (FileUtils.renameDirectory(currentFullDirectoryPath, newFullDirectoryPath) == false)
			{
				cleanupTemp(in_options);
				Logging.logFatal("Failed to rename directory: " + newFullDirectoryPath);
			}
		}
	}
	/**
	 * Copies Chilli Source into the project directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options with which to create the new project.
	 */
	private static void copyChilliSource(Options in_options)
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		String csSourcePath = StringUtils.standardiseDirectoryPath(getJarDirectoryPath(in_options) + k_csSourceDirectoryPath); 
		String csDestinationPath = StringUtils.standardiseDirectoryPath(tempProjectDirectory + k_csDestinationDirectoryPath);
		
		Logging.logVerbose("Copying Chilli Source into the project.");
		if (FileUtils.copyDirectory(csSourcePath, csDestinationPath, new LinkedList<String>()) == false)
		{
			cleanupTemp(in_options);
			Logging.logFatal("Could not copy Chilli Source into project.");
		}
	}
	/**
	 * Copies the project to the output directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options with which to create the new project.
	 */
	private static void copyToOutput(Options in_options)
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		String outputProjectDirectory = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + in_options.m_projectName);
		
		Logging.logVerbose("Copying project to output directory: " + outputProjectDirectory);
		if (FileUtils.copyDirectory(tempProjectDirectory, outputProjectDirectory, new LinkedList<String>()) == false)
		{
			cleanupTemp(in_options);
			Logging.logFatal("Could not copy to output directory: " + outputProjectDirectory);
		}
	}
	/**
	 * Cleans up the temporary directory.
	 *
	 * @author Ian Copland
	 * 
	 * @param in_options - The options with which to create the new project.
	 */
	private static void cleanupTemp(Options in_options)
	{
		Logging.logVerbose("Deleting temp directory.");
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		FileUtils.deleteDirectory(tempDirectoryPath);
	}
}
