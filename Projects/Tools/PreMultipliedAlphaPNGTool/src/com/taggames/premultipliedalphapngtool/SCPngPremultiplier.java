package com.taggames.premultipliedalphapngtool;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URLDecoder;
import java.security.SecureRandom;
import java.util.LinkedList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import com.taggames.toolutils.SCFileSystemUtils;
import com.taggames.toolutils.SCLogger;
import com.taggames.toolutils.SCStringUtils;

public class SCPngPremultiplier 
{
	//-------------------------------------------------
	/// Constants
	//-------------------------------------------------
	static final String kstrCacheDirectory = "PreMultipliedAlphaPNGToolCache/";
	static final String kstrCacheManifestFilename = "Manifest.xml";
	static final String kstrExternalMessageIndent = "    ";
	static final String kstrCacheManifestRootElementName = "Manifest";
	static final String kstrCacheManifestCachedFileElementName = "CachedFile";
	static final String kstrCacheManifestOriginalLocationAttributeName = "OriginalLocation";
	static final String kstrCacheManifestCachedLocationAttributeName = "CachedLocation";
	static final int kdwNumRandomBytesInCachedName = 24;
	static final String[] kastrIgnoreDirectories = new String[]
	{
		".svn"
	};
	
	//-------------------------------------------------
	/// Static Member Data
	//-------------------------------------------------
	static SecureRandom msRandomNumberGenerator = new SecureRandom();
	//-------------------------------------------------
	/// Static Constructor
	//-------------------------------------------------
	static
	{
	
	}
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
			
			//check if this should be cached.
			if (insOptions.bShouldCache == true)
				CacheFile(strInput);
			
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
	//-------------------------------------------------
	/// Premultiply Directory
	///
	/// Premultiplies and caches a directory recursively
	///
	/// @param the directory.
	//-------------------------------------------------
	public static void PremultiplyDirectory(String instrDirectory)
	{
		//insure the path is in the right format
		String strDirectory = SCStringUtils.StandardiseFilepath(instrDirectory);
		
		//Check that this is indeed a directory
		File directory = new File(strDirectory);
		if (directory.isDirectory() == false)
			SCLogger.LogFatalError("Directory '" + strDirectory + "' does not exist.");
		
		//iterate through each item the directory contains.
		String[] astrContents = directory.list();
		for (String strItem: astrContents)
		{
			boolean bShouldIgnore = false;
			for (String strIgnoreDirectory: kastrIgnoreDirectories)
			{
				if (strItem.equalsIgnoreCase(strIgnoreDirectory) == true)
					bShouldIgnore = true;
			}
			
			if (bShouldIgnore == false)
			{
				File item = new File(strDirectory + strItem);
				
				if (item.isDirectory() == true)
				{
					PremultiplyDirectory(strDirectory + strItem);
				}
				else if (item.isFile() == true)
				{
					//Check if this is a png, if it is then pre-multiply it!
					if (strItem.toLowerCase().endsWith(".png") == true)
					{
						PremultiplicationOptions options = new PremultiplicationOptions();
						options.strInputFilename = strDirectory + strItem;
						options.strOutputFilename = strDirectory + strItem;
						options.bShouldCache = true;
						Premultiply(options);
					}
				}
			}
		}
	}
	//-------------------------------------------------
	/// Restore From Cache
	///
	/// Restores all the cached items to where they
	/// originally came from.
	//-------------------------------------------------
	public static void RestoreFromCache()
	{
		try
		{
			DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
			Document manifestDoc = null;
			
			//open the manifest. If it doesnt exist create it.
			if ((new File(kstrCacheDirectory + kstrCacheManifestFilename)).exists() == true)
				manifestDoc = docBuilder.parse(kstrCacheDirectory + kstrCacheManifestFilename);
			else
				SCLogger.LogWarning("The cache is empty.");
			
			//get the root element
			if (manifestDoc != null)
			{
				Node rootElement = manifestDoc.getFirstChild();
				
				//check to see if this file already exists.
				NodeList nodes = rootElement.getChildNodes();
				for (int i = 0; i < nodes.getLength(); ++i)
				{
					Element node = (Element) nodes.item(i);
					if (node.hasAttribute(kstrCacheManifestOriginalLocationAttributeName) == true && node.hasAttribute(kstrCacheManifestCachedLocationAttributeName) == true)
					{
						SCLogger.LogMessage("Restoring: " + node.getAttribute(kstrCacheManifestOriginalLocationAttributeName));
						SCFileSystemUtils.CopyFile(node.getAttribute(kstrCacheManifestCachedLocationAttributeName), node.getAttribute(kstrCacheManifestOriginalLocationAttributeName));
					}
				}
			}
			
			//delete the directory
			SCLogger.LogMessage("Deleting cache.");
			SCFileSystemUtils.DeleteDirectory(kstrCacheDirectory);
		}
		catch (Exception e)
		{
			SCLogger.LogError("Error while restoring cache.");
		}
	}
	//-------------------------------------------------
	/// CacheFile
	///
	/// Stores a file in the cache.
	///
	/// @param the file.
	//-------------------------------------------------
	private static void CacheFile(String instrFilepath)
	{
		SCLogger.LogMessage("Caching: " + instrFilepath);
		
		//get the directory to copy to. Insure the filename its getting copied too has a random 
		//element, so the files with the same name don't overwrite each other
		String strCopyTo = new String(instrFilepath);
		int dwLastSlash = strCopyTo.lastIndexOf("/");
		if (dwLastSlash != -1)
			strCopyTo = strCopyTo.substring(dwLastSlash + 1);
		long ddwRandomNumber = msRandomNumberGenerator.nextLong();
		String strRandomString = "" + ddwRandomNumber + "-";
		strCopyTo = kstrCacheDirectory + strRandomString + strCopyTo;
		
		//create the directory
		SCFileSystemUtils.CreateDirectory(kstrCacheDirectory);
		
		//copy the file.
		SCFileSystemUtils.CopyFile(instrFilepath, strCopyTo);
		
		//store the copied file in the manifest.
		AddToManifest(instrFilepath, strCopyTo);
	}
	//------------------------------------------------------
	/// AddToManifest
	///
	/// Adds a cached file and its original location to the
	/// cache manifest.
	///
	/// @param the original path.
	/// @param the cached path.
	//------------------------------------------------------
	private static void AddToManifest(String instrFilepath, String instrCopyTo)
	{
		try
		{
			DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
			Document manifestDoc;
			
			//open the manifest. If it doesnt exist create it.
			if ((new File(kstrCacheDirectory + kstrCacheManifestFilename)).exists() == false)
			{
				manifestDoc = docBuilder.newDocument();
				manifestDoc.appendChild(manifestDoc.createElement(kstrCacheManifestRootElementName));
			}
			else
			{
				manifestDoc = docBuilder.parse(kstrCacheDirectory + kstrCacheManifestFilename);
			}
			
			//get the root element
			Node rootElement = manifestDoc.getFirstChild();
			
			//check to see if this file already exists.
			NodeList nodes = rootElement.getChildNodes();
			for (int i = 0; i < nodes.getLength(); ++i)
			{
				Element node = (Element) nodes.item(i);
				if (node.hasAttribute(kstrCacheManifestOriginalLocationAttributeName) == true)
				{
					if (node.getAttribute(kstrCacheManifestOriginalLocationAttributeName).equalsIgnoreCase(instrFilepath) == true)
					{
						SCLogger.LogWarning("File " + instrFilepath + " is already cached.");
						return;
					}
				}
			}
			
			//set the entry
			Element entry = manifestDoc.createElement(kstrCacheManifestCachedFileElementName);
			entry.setAttribute(kstrCacheManifestOriginalLocationAttributeName, instrFilepath);
			entry.setAttribute(kstrCacheManifestCachedLocationAttributeName, instrCopyTo);
			
			//add this to the rootElement
			rootElement.appendChild(entry);
			
			// write the content into xml file
			TransformerFactory transformerFactory = TransformerFactory.newInstance();
			Transformer transformer = transformerFactory.newTransformer();
			DOMSource source = new DOMSource(manifestDoc);
			StreamResult result = new StreamResult(new File(kstrCacheDirectory + kstrCacheManifestFilename));
			transformer.transform(source, result);
		}
		catch (Exception e)
		{
			SCLogger.LogError("Error while adding cached file to Manifest.xml");
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
				strExecutableName = strPathToHere + "Native/PreMultipliedAlphaPNGToolWindows.exe";
			else if (strOS.startsWith("Mac") == true)
				strExecutableName = strPathToHere + "Native/PreMultipliedAlphaPNGToolOSX";
			else if (strOS.startsWith("Linux") == true)
				strExecutableName = strPathToHere + "Native/PreMultipliedAlphaPNGToolLinux";
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
					SCLogger.LogMessage(kstrExternalMessageIndent + inputLine);
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
						SCLogger.LogMessage(kstrExternalMessageIndent + errorLine);
					
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
