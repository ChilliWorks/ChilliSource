/**
 * Main.java
 * Chilli Source
 * Created by Ian Copland on 15/01/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
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

package com.chilliworks.chillisource.colladatocsmodel;

import com.chilliworks.chillisource.coreutils.*;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;

/**
 * Provides the entry point into the application.
 * 
 * @author Ian Copland
 */
public final class Main 
{
	private static final String k_paramNameInput = "--input";
	private static final String k_paramNameOutput = "--output";
	private static final String k_paramNameVertexFormat = "--vertexformat";
	private static final String k_paramNameAnimated = "--animated";
	private static final String k_paramNameSwapHandedness = "--swaphandedness";
	private static final String k_paramNameSwapYAndZ = "--swapyandz";
	private static final String k_paramNameDisableMeshBatch = "--disablemeshbatch";
	private static final String k_paramNameDisabletTexCoordFlip = "--disabletexcoordflip";
	private static final String k_paramNameHelp = "--help";
	private static final String k_shortParamNameInput = "-i";
	private static final String k_shortParamNameOutput = "-o";
	private static final String k_shortParamNameVertexFormat = "-vf";
	private static final String k_shortParamNameAnimated = "-a";
	private static final String k_shortParamNameSwapHandedness = "-sh";
	private static final String k_shortParamNameSwapYAndZ = "-sy";
	private static final String k_shortParamNameDisableMeshBatch = "-dmb";
	private static final String k_shortParamNameDisabletTexCoordFlip = "-dtf";
	private static final String k_shortParamNameHelp = "-h";
	private static final char k_vertexElementPosition = 'p';
	private static final char k_vertexElementTexCoord = 't';
	private static final char k_vertexElementNormal = 'n';
	private static final char k_vertexElementColour = 'c';
	private static final char k_vertexElementWeights = 'w';
	private static final char k_vertexElementJoints = 'j';
	
	/**
	 * The entry point into the application.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_args - The input arguments.
	 */
	public static void main(String[] in_args) throws Exception 
	{
		//setup the logger.
		String[] arguments = Logging.start(in_args);
		
		//check the number of arguments make sense.
		if (arguments.length == 0)
		{
			printHelpText();
			return;
		}
		
		//gather up commands
		boolean vertexDeclarationProvided = false;
		ColladaToCSModelOptions params = new ColladaToCSModelOptions();
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(k_paramNameInput) == true || arguments[i].equalsIgnoreCase(k_shortParamNameInput) == true)
			{
				if (i+1 < arguments.length)
					params.m_inputFilePath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(k_paramNameOutput) == true || arguments[i].equalsIgnoreCase(k_shortParamNameOutput) == true)
			{
				if (i+1 < arguments.length)
					params.m_outputFilePath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//vertex declaration
			else if (arguments[i].equalsIgnoreCase(k_paramNameVertexFormat) == true || arguments[i].equalsIgnoreCase(k_shortParamNameVertexFormat) == true)
			{
				vertexDeclarationProvided = true;
				
				if (i+1 < arguments.length)
					ParseVertexDeclaration(params, arguments[i+1]);
				else
					Logging.logFatal("No vertex declaration provided!");
				i++;
			}
			
			//animated
			else if (arguments[i].equalsIgnoreCase(k_paramNameAnimated) == true || arguments[i].equalsIgnoreCase(k_shortParamNameAnimated) == true)
			{
				params.m_animated = true;
			}
			
			//animated
			else if (arguments[i].equalsIgnoreCase(k_paramNameSwapHandedness) == true || arguments[i].equalsIgnoreCase(k_shortParamNameSwapHandedness) == true)
			{
				params.m_swapHandedness = true;
			}
			
			//animated
			else if (arguments[i].equalsIgnoreCase(k_paramNameSwapYAndZ) == true || arguments[i].equalsIgnoreCase(k_shortParamNameSwapYAndZ) == true)
			{
				params.m_swapYAndZ = true;
			}
			
			//animated
			else if (arguments[i].equalsIgnoreCase(k_paramNameDisableMeshBatch) == true || arguments[i].equalsIgnoreCase(k_shortParamNameDisableMeshBatch) == true)
			{
				params.m_combineMeshes = false;
			}
			
			//animated
			else if (arguments[i].equalsIgnoreCase(k_paramNameDisabletTexCoordFlip) == true || arguments[i].equalsIgnoreCase(k_shortParamNameDisabletTexCoordFlip) == true)
			{
				params.m_flipVerticalTexCoords = false;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(k_paramNameHelp) == true || arguments[i].equalsIgnoreCase(k_shortParamNameHelp) == true)
			{
				printHelpText();
				return;
			}
			
			//failure
			else
			{
				Logging.logFatal("Invalid argument found: " + arguments[i]);
			}
		}
		
		if (vertexDeclarationProvided == false)
		{
			applyDefaultVertexFormat(params);
		}
		
		//check for weird combinations of parameters
		ParamsChecker.CheckParameters(params);
		
		ColladaToCSModel.convert(params);
		
		Logging.finish();
	}
	//-------------------------------------------------------------------
	/// Parse Vertex Declaration
	///
	/// Parses a string vertex declaration.
	///
	/// @param the conversion params.
	/// @param the string to parse.
	//-------------------------------------------------------------------
	static void ParseVertexDeclaration(ColladaToCSModelOptions inParams, String instrVertexDecl)
	{
		for (int i = 0; i < instrVertexDecl.length(); ++i)
		{
			if (instrVertexDecl.charAt(i) == k_vertexElementPosition)
				inParams.m_vertexHasPosition = true;
			if (instrVertexDecl.charAt(i) == k_vertexElementNormal)
				inParams.m_vertexHasNormal = true;
			if (instrVertexDecl.charAt(i) == k_vertexElementTexCoord)
				inParams.m_vertexHasTexCoords = true;
			if (instrVertexDecl.charAt(i) == k_vertexElementColour)
				inParams.m_vertexHasColour = true;
			if (instrVertexDecl.charAt(i) == k_vertexElementWeights)
				inParams.m_vertexHasWeights = true;
			if (instrVertexDecl.charAt(i) == k_vertexElementJoints)
				inParams.m_vertexHasJointIndices = true;
		}
	}
	/**
	 * Applies the default vertex format. This always includes position, tex coordinates
	 * and normals. For animated models this also includes weights and joints.
	 * 
	 * @param Ian Copland
	 * 
	 * @param in_options - The options to apply the default to.
	 */
	static void applyDefaultVertexFormat(ColladaToCSModelOptions in_options)
	{
		in_options.m_vertexHasPosition = true;
		in_options.m_vertexHasTexCoords = true;
		in_options.m_vertexHasNormal = true;
		in_options.m_vertexHasColour = false;
		
		if (in_options.m_animated == true)
		{
			in_options.m_vertexHasJointIndices = true;
			in_options.m_vertexHasWeights = true;
		}
		else
		{
			in_options.m_vertexHasJointIndices = false;
			in_options.m_vertexHasWeights = false;
		}
	}
	/**
	 * Prints the help text. This will print regardless of logging level.
	 * 
	 * @param Ian Copland
	 */
	public static void printHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.VERBOSE);
		Logging.logVerbose("Usage: java -jar ColladaToCSModel.jar " + k_paramNameInput + " <file path> " + k_paramNameOutput + " <file path> [" + k_paramNameVertexFormat + " <vertex declaration>] [" + k_paramNameAnimated + "] [" + k_paramNameSwapHandedness + "] [" + k_paramNameSwapYAndZ + "] [" + k_paramNameDisableMeshBatch + "] [" + k_paramNameDisabletTexCoordFlip + "] [" + k_paramNameHelp + "] [" + Logging.PARAM_LOGGING_LEVEL + " <level>]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + k_paramNameInput + "(" + k_shortParamNameInput + "): The input filename.");
		Logging.logVerbose(" " + k_paramNameOutput + "(" + k_shortParamNameOutput + "): The output filename.");
		Logging.logVerbose(" " + k_paramNameVertexFormat + "(" + k_shortParamNameVertexFormat + "): [Optional] A list of elements that a vertex in this model will contain.");
		Logging.logVerbose(" " + k_paramNameAnimated + "(" + k_shortParamNameAnimated + "): [Optional] Enables outputting of animation data.");
		Logging.logVerbose(" " + k_paramNameSwapHandedness + "(" + k_shortParamNameSwapHandedness + "): [Optional] Enables transforming the output data to swap the coordinate system handedness.");
		Logging.logVerbose(" " + k_paramNameSwapYAndZ + "(" + k_shortParamNameSwapYAndZ + "): [Optional] Enables transforming the output data to swap the Y and Z coordinates.");
		Logging.logVerbose(" " + k_paramNameDisableMeshBatch + "(" + k_shortParamNameDisableMeshBatch + "): [Optional] Disables batching meshes with the same material together.");
		Logging.logVerbose(" " + k_paramNameDisabletTexCoordFlip + "(" + k_shortParamNameDisabletTexCoordFlip + "): [Optional] Disables flipping the Y component of the texture coordinates.");
		Logging.logVerbose(" " + Logging.PARAM_LOGGING_LEVEL + "(" + Logging.SHORT_PARAM_LOGGING_LEVEL + "): [Optional] The level of messages to log.");
		Logging.logVerbose(" " + k_paramNameHelp + "(" + k_shortParamNameHelp + "): [Optional] Display this help message.");
		Logging.logVerbose("Vertex Elements:");
		Logging.logVerbose(" " + k_vertexElementPosition + ": A vertex will contain a position.");
		Logging.logVerbose(" " + k_vertexElementTexCoord + ": A vertex will contain texture coordinates.");
		Logging.logVerbose(" " + k_vertexElementNormal + ": A vertex will contain a normal.");
		Logging.logVerbose(" " + k_vertexElementColour + ": A vertex will contain a colour.");
		Logging.logVerbose(" " + k_vertexElementWeights + ": A vertex will contain weights.");
		Logging.logVerbose(" " + k_vertexElementJoints + ": A vertex will contain joint indices.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_NONE + ": No logging.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_FATAL + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_ERROR + ": Only log errors.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_WARNING + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.LOGGING_LEVEL_VERBOSE + ": Log all messages.");
	}
}
