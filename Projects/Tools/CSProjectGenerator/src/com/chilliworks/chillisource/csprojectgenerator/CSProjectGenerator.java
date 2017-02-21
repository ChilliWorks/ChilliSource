/**
 * CSProjectGenerator.java
 * ChilliSource
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
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.CodeSource;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.coreutils.ZipUtils;

/**
 * A collection of methods for generating new ChilliSource projects.
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
	 * Generates a new ChilliSource project using the given options.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 */
	public static void generate(Options in_options) throws CSException
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
	private static void validateOptions(Options in_options) throws CSException
	{
		//check that the project name is valid.
		if (in_options.m_projectName.matches("[a-zA-Z0-9]+") == false)
		{
			throw new CSException("Project name can only contain: a-z, A-Z or 0-9.");
		}
		
		//check that that package name is valid
		if (in_options.m_packageName.matches("[a-z.]+") == false)
		{
			throw new CSException("Package name can only contain: a-z and '.'");
		}
		
		//confirm that the project path doesn't already exist.
		String outputProjectDirectoryPathName = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + in_options.m_projectName);
		File outputProjectDirectory = new File(outputProjectDirectoryPathName);
		if (outputProjectDirectory.exists() == true)
		{
			throw new CSException("Output project directory '" + outputProjectDirectoryPathName + "' already exists!");
		}
		
		//ensure the output path is not inside ChilliSource as this will break copying of files.
		try
		{
			String outputProjectDirectoryPathNameAbs = outputProjectDirectory.getCanonicalPath();
			String csDirectoryPathName = StringUtils.standardiseDirectoryPath(getJarDirectoryPath(in_options) + k_csSourceDirectoryPath); 
			String csDirectoryPathNameAbs = new File(csDirectoryPathName).getCanonicalPath();
		
			Path outputProjectDirectoryPath = Paths.get(outputProjectDirectoryPathNameAbs);
			Path csDirectoryPath = Paths.get(csDirectoryPathNameAbs);

			if (outputProjectDirectoryPath.startsWith(csDirectoryPath) == true)
			{
				throw new CSException("Output project directory '" + outputProjectDirectoryPathName + "' cannot be located inside ChilliSource.");
			}
		}
		catch (Exception e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			throw new CSException("Failed to validate output directory path.", e);
		}
	}
	/**
	 * unzips the project to the temp directory.
	 * 
	 * @author Ian Copland
	 *
	 * @param in_options - The options with which to create the new project.
	 */
	private static void unzipProject(Options in_options) throws CSException
	{
		try 
		{
			Logging.logVerbose("Unzipping project to temporary directory.");
			
			String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
			if (new File(tempDirectoryPath).exists() == true)
			{
				throw new CSException("Temp directory already exists.");
			}
			
			String pathToZip = getJarDirectoryPath(in_options) + k_projectZipFilePath;
			ZipUtils.extract(pathToZip, tempDirectoryPath);
	    } 
		catch (IOException e) 
	    {
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			
			cleanupTemp(in_options);
	        throw new CSException("Could not open project zip file: " + k_projectZipFilePath, e);
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
	private static String getJarDirectoryPath(Options in_options) throws CSException
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
			throw new CSException("Could not get jar directory path.", e);
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
	private static void updateProjectFiles(Options in_options) throws CSException
	{
		String[] filesToUpdateContents = new String[]
		{
			".gitignore",
			"AppSource/App.cpp",
			"AppSource/App.h",
			"AppSource/State.cpp",
			"AppSource/State.h",
			"Content/BuildAll.bat",
			"Content/BuildAll.command",
			"Content/BuildAll.sh",
			"Content/AppResources/App.config",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/build.gradle",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/settings.gradle",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/" + k_templateProjectName.toLowerCase() + ".iml",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/.idea/.name",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/.idea/modules.xml",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/app/app.iml",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/app/build.gradle",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/app/proguard-rules.pro",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/app/src/main/AndroidManifest.xml",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/app/src/main/CSAndroidManifest.xml",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/app/src/main/jni/Android.mk",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/app/src/main/jni/Application.mk",
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/app/src/main/res/values/strings.xml",
			"Projects/iOS/" + k_templateProjectName + ".xcodeproj/project.pbxproj",
			"Projects/iOS/" + k_templateProjectName + ".xcodeproj/xcshareddata/xcschemes/" + k_templateProjectName + ".xcscheme",
			"Projects/iOS/ProjectResources/Info.plist",
			"Projects/Windows/" + k_templateProjectName + ".sln",
			"Projects/Windows/" + k_templateProjectName + ".vcxproj",
			"Projects/Windows/" + k_templateProjectName + ".vcxproj.filters",
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
			"Projects/Android/" + k_templateProjectName.toLowerCase() + "/" + k_templateProjectName.toLowerCase() + ".iml",
			"Projects/iOS/" + k_templateProjectName + ".xcodeproj/xcshareddata/xcschemes/" + k_templateProjectName + ".xcscheme",
			"Projects/Windows/" + k_templateProjectName + ".sln",
			"Projects/Windows/" + k_templateProjectName + ".vcxproj",
			"Projects/Windows/" + k_templateProjectName + ".vcxproj.filters"

		};
		
		updateFileNames(in_options, filesToUpdateFileName);
		
		String[] directoriesToUpdateDirectoryName = new String[]
		{
			"Projects/Android/" + k_templateProjectName.toLowerCase(),
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
	private static void updateFileContents(Options in_options, String[] in_filePaths) throws CSException
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
				throw new CSException("Could not read file: " + filePath);
			}

			fileContents = fileContents.replace(k_templatePackageName, in_options.m_packageName);
			fileContents = fileContents.replace(k_templateProjectName.toLowerCase(), in_options.m_projectName.toLowerCase());
			fileContents = fileContents.replace(k_templateProjectName.toUpperCase(), in_options.m_projectName.toUpperCase());
			fileContents = fileContents.replace(k_templateProjectName, in_options.m_projectName);
			
			if (FileUtils.writeFile(fullFilePath, fileContents) == false)
			{
				cleanupTemp(in_options);
				throw new CSException("Could not write file: " + filePath);
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
	private static void updateFileNames(Options in_options, String[] in_filePaths) throws CSException
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		
		for (String currentFilePath : in_filePaths)
		{
			Logging.logVerbose("Updating file name: " + currentFilePath);
			
			String fileName = StringUtils.getFileName(currentFilePath);
			String directory = StringUtils.getDirectory(currentFilePath);
			
			String newFileName = fileName.replace(k_templatePackageName, in_options.m_packageName);
			newFileName = newFileName.replace(k_templateProjectName.toLowerCase(), in_options.m_projectName.toLowerCase());
			newFileName = newFileName.replace(k_templateProjectName, in_options.m_projectName);
			
			String currentFullFilePath = StringUtils.standardiseFilePath(tempProjectDirectory + currentFilePath);
			String newFullFilePath = StringUtils.standardiseFilePath(tempProjectDirectory + directory + newFileName);
			
			if (FileUtils.renameFile(currentFullFilePath, newFullFilePath) == false)
			{
				cleanupTemp(in_options);
				throw new CSException("Failed to rename file: " + currentFilePath);
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
	private static void updateDirectoryNames(Options in_options, String[] in_directoryPaths) throws CSException
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		
		for (String currentDirectoryPath : in_directoryPaths)
		{
			Logging.logVerbose("Updating directory name: " + currentDirectoryPath);

			String newDirectoryPath = currentDirectoryPath.replace(k_templatePackageName, in_options.m_packageName);
			newDirectoryPath = newDirectoryPath.replace(k_templateProjectName.toLowerCase(), in_options.m_projectName.toLowerCase());
			newDirectoryPath = newDirectoryPath.replace(k_templateProjectName, in_options.m_projectName);
			
			String currentFullDirectoryPath = StringUtils.standardiseFilePath(tempProjectDirectory + currentDirectoryPath);
			String newFullDirectoryPath = StringUtils.standardiseFilePath(tempProjectDirectory + newDirectoryPath);
			
			if (FileUtils.renameDirectory(currentFullDirectoryPath, newFullDirectoryPath) == false)
			{
				cleanupTemp(in_options);
				throw new CSException("Failed to rename directory: " + newFullDirectoryPath);
			}
		}
	}
	/**
	 * Copies ChilliSource into the project directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options with which to create the new project.
	 */
	private static void copyChilliSource(Options in_options) throws CSException
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		String csSourcePath = StringUtils.standardiseDirectoryPath(getJarDirectoryPath(in_options) + k_csSourceDirectoryPath); 
		String csDestinationPath = StringUtils.standardiseDirectoryPath(tempProjectDirectory + k_csDestinationDirectoryPath);
		
		Logging.logVerbose("Copying ChilliSource into the project.");
		
		List<String> ignore = new ArrayList<>();
		Collections.addAll(ignore, "bin", "obj");
		if (FileUtils.copyDirectory(csSourcePath, csDestinationPath, ignore) == false)
		{
			cleanupTemp(in_options);
			throw new CSException("Could not copy ChilliSource into project.");
		}
		
		//remove any remnants of the git project.
		try
		{
			Path gitPath = Paths.get(csDestinationPath + ".git");
			if (Files.isDirectory(gitPath) == true)
			{
				FileUtils.deleteDirectory(gitPath.toString());
			}
			else
			{
				Files.deleteIfExists(gitPath);
			}
			Files.deleteIfExists(Paths.get(csDestinationPath + ".gitignore"));
		}
		catch (Exception e)
		{
			cleanupTemp(in_options);
			throw new CSException("Could not delete git remnants.", e);
		}
	}
	/**
	 * Copies the project to the output directory.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options with which to create the new project.
	 */
	private static void copyToOutput(Options in_options) throws CSException
	{
		String tempDirectoryPath = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + k_tempDirectory);
		String tempProjectDirectory = StringUtils.standardiseDirectoryPath(tempDirectoryPath + k_templateProjectName);
		String outputProjectDirectory = StringUtils.standardiseDirectoryPath(in_options.m_outputDirectory + in_options.m_projectName);
		
		Logging.logVerbose("Copying project to output directory: " + outputProjectDirectory);
		
		List<String> ignore = new ArrayList<>();
		Collections.addAll(ignore, "bin", "obj");
		if (FileUtils.copyDirectory(tempProjectDirectory, outputProjectDirectory, ignore) == false)
		{
			cleanupTemp(in_options);
			throw new CSException("Could not copy to output directory: " + outputProjectDirectory);
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
