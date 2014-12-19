/**
 * AndroidManifestGenerator.java
 * Chilli Source
 * Created by Ian Copland on 20/06/2014.
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

package com.chilliworks.chillisource.androidmanifestbuilder;

import java.io.StringReader;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.xml.sax.InputSource;

import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.Logging;

/**
 * Generates the android manifest containing all the required settings
 * for building Chilli Source as well as the extra settings requested
 * by the user.
 * 
 * @author Ian Copland
 */
public final class AndroidManifestBuilder
{
	/**
	 * Generates the android manifest from the Chilli Source template
	 * and the applications CS manifest.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The user manifest file path.
	 * @param The template manifest file path.
	 * @param The output AndroidManifest.xml file path.
	 */
	public static void generate(String in_inputFilePath, String in_templateFilePath, String in_outputFilePath)
	{
		UserManifestData userManifestData = readUserManifest(in_inputFilePath);
		String templateData = readTemplate(in_templateFilePath);
		String outputData = applyUserDataToTemplate(userManifestData, templateData);
		writeOutput(in_outputFilePath, outputData);
	}
	/**
	 * Reads the user manifest from file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The file path to the user manifest.
	 * 
	 * @return A container for all information in the user manifest.
	 */
	private static UserManifestData readUserManifest(String in_filePath)
	{
		Logging.logVerbose("Reading user manifest data from '" + in_filePath + "'");
		
		UserManifestData output = new UserManifestData();
		
		try
		{
			String xmlString = FileUtils.readFile(in_filePath);
			if (xmlString == null)
			{
				Logging.logFatal("Could not read user manifest file from '" + in_filePath + "'");
			}
			
			InputSource inputSource = new InputSource();
			inputSource.setCharacterStream(new StringReader(xmlString));
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
			dbFactory.setNamespaceAware(false);
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			Document doc = dBuilder.parse(inputSource);
			doc.getDocumentElement().normalize();
			
			//read all attributes
			Node root = doc.getElementsByTagName("cs-manifest").item(0);

			Node packageName = root.getAttributes().getNamedItem("package");
			if (packageName != null)
			{
				output.m_package = packageName.getNodeValue();
			}
			
			Node versionCodeNode = root.getAttributes().getNamedItem("versionCode");
			if (versionCodeNode != null)
			{
				output.m_versionCode = versionCodeNode.getNodeValue();
			}
			
			Node versionNameNode = root.getAttributes().getNamedItem("versionName");
			if (versionNameNode != null)
			{
				output.m_versionName = versionNameNode.getNodeValue();
			}
			
			Node minSdkVersionNode = root.getAttributes().getNamedItem("minSdkVersion");
			if (minSdkVersionNode != null)
			{
				output.m_minSdkVersion = minSdkVersionNode.getNodeValue();
			}
			
			Node targetSdkVersionNode = root.getAttributes().getNamedItem("targetSdkVersion");
			if (targetSdkVersionNode != null)
			{
				output.m_targetSdkVersion = targetSdkVersionNode.getNodeValue();
			}
			
			Node orientationNode = root.getAttributes().getNamedItem("screenOrientation");
			if (orientationNode != null)
			{
				output.m_orientation = orientationNode.getNodeValue();
			}
			
			Node debuggableNode = root.getAttributes().getNamedItem("debuggable");
			if (debuggableNode != null)
			{
				output.m_debuggable = debuggableNode.getNodeValue();
			}
			
			Node facebookAppIdNode = root.getAttributes().getNamedItem("facebookAppId");
			if (facebookAppIdNode != null)
			{
				output.m_facebookAppId = facebookAppIdNode.getNodeValue();
			}
			
			Node googlePlayProjectIdNode = root.getAttributes().getNamedItem("googlePlayProjectId");
			if (googlePlayProjectIdNode != null)
			{
				output.m_googlePlayProjectId = googlePlayProjectIdNode.getNodeValue();
			}
			
			output.m_manifestExtra = getElementContentAsString(xmlString, "manifest-extra");
			output.m_applicationExtra = getElementContentAsString(xmlString, "application-extra");
			output.m_permissions = getElementContentAsString(xmlString, "permissions");
		}
		catch (Exception e)
		{
			Logging.logFatal("Failed to read user manifest xml '" + in_filePath + "'");
		}
		
		return output;
	}
	/**
	 * Reads the template from file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The template file path.
	 * 
	 * @return A string containing the contents of the template file.
	 */
	private static String readTemplate(String in_filePath)
	{
		Logging.logVerbose("Reading manifest template from '" + in_filePath + "'");
		
		String templateData = FileUtils.readFile(in_filePath);
		if (templateData.length() == 0)
		{
			Logging.logFatal("Could not read template data");
		}
		return templateData;
	}
	/**
	 * Applies the user data to the template file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The user data.
	 * @param The template.
	 * 
	 * @return A string containing the output AndroidManifest.xml
	 */
	private static String applyUserDataToTemplate(UserManifestData in_userData, String in_template)
	{
		String output = in_template;
		
		output = output.replace("[[PACKAGE]]", in_userData.m_package);
		output = output.replace("[[VERSIONCODE]]", in_userData.m_versionCode);
		output = output.replace("[[VERSIONNAME]]", in_userData.m_versionName);
		output = output.replace("[[MINSDKVERSION]]", in_userData.m_minSdkVersion);
		output = output.replace("[[TARGETSDKVERSION]]", in_userData.m_targetSdkVersion);
		output = output.replace("[[ORIENTATION]]", in_userData.m_orientation);
		output = output.replace("[[DEBUGGABLE]]", in_userData.m_debuggable);
		output = output.replace("[[GOOGLEPLAYPROJECTID]]", in_userData.m_googlePlayProjectId);
		output = output.replace("[[FACEBOOKAPPID]]", in_userData.m_facebookAppId);
		output = output.replace("[[MANIFESTEXTRA]]", in_userData.m_manifestExtra);
		output = output.replace("[[APPLICATIONEXTRA]]", in_userData.m_applicationExtra);
		output = output.replace("[[PERMISSIONS]]", in_userData.m_permissions);
		
		return output;
	}
	/**
	 * Writes the output data to disk.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The output file path.
	 * @param The output data.
	 */
	private static void writeOutput(String in_filePath, String in_outputData)
	{
		Logging.logVerbose("Writing output file to '" + in_filePath + "'");
		
		if (FileUtils.writeFile(in_filePath, in_outputData) == false)
		{
			Logging.logFatal("Failed to write file '" + in_filePath + "'");
		}
	}
	/**
	 * Searches through an xml string for the given xml tag and returns it's
	 * content, including other tags, as a string.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The xml string.
	 * @param The element name.
	 * 
	 * @return The contents of the element as a string.
	 */
	private static String getElementContentAsString(String in_stringXml, String in_elementName)
	{
		String output = "";
		
		String startTag = "<" + in_elementName + ">";
		String endTag = "</" + in_elementName + ">";
		int start = in_stringXml.indexOf(startTag) + startTag.length();
		int end = in_stringXml.indexOf(endTag);
		if (start != -1 && end != -1)
		{
			output = in_stringXml.substring(start, end);
		}
		
		return output;
	}
}
