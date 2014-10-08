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

import com.chilliworks.chillisource.toolutils.*;
import com.chilliworks.chillisource.toolutils.Logging.LoggingLevel;

public class Main 
{
	//-------------------------------------------------------------------
	/// Constants
	//-------------------------------------------------------------------
	private static final String PARAM_NAME_INPUT 					= "--input";
	private static final String PARAM_NAME_OUTPUT 					= "--output";
	private static final String PARAM_NAME_FEATURES					= "--features";
	private static final String PARAM_NAME_VERTEXDECLARATION		= "--vertexdeclaration";
	private static final String PARAM_NAME_TRANSFORMS				= "--transforms";
	private static final String PARAM_NAME_HELP						= "--help";
	private static final String PARAM_NAME_INPUT_SHORT 				= "-i";
	private static final String PARAM_NAME_OUTPUT_SHORT				= "-o";
	private static final String PARAM_NAME_FEATURES_SHORT			= "-f";
	private static final String PARAM_NAME_VERTEXDECLARATION_SHORT	= "-v";
	private static final String PARAM_NAME_TRANSFORMS_SHORT			= "-t";
	private static final String PARAM_NAME_HELP_SHORT				= "-h";
	private static final char FEATURE_HASANIMATIONDATA				= 'a';
	private static final char VERTEX_ELEMENT_POSITION				= 'p';
	private static final char VERTEX_ELEMENT_TEXCOORD				= 't';
	private static final char VERTEX_ELEMENT_NORMAL					= 'n';
	private static final char VERTEX_ELEMENT_COLOUR					= 'c';
	private static final char VERTEX_ELEMENT_WEIGHTS				= 'w';
	private static final char VERTEX_ELEMENT_JOINT_INDICES			= 'j';
	private static final char MODIFICATION_SWAPYANDZ				= 'y';
	private static final char MODIFICATION_FLIPYTEXCOORDS			= 'f';
	private static final char MODIFICATION_COMBINEMESHES			= 'c';
	//-------------------------------------------------------------------
	/// Main
	///
	/// Entry point into the application.
	//-------------------------------------------------------------------
	public static void main(String[] inastrArgs) throws Exception 
	{
		//setup the logger.
		String[] arguments = Logging.start(inastrArgs);
		
		//check the number of arguments make sense.
		if (arguments.length == 0)
		{
			PrintHelpText();
			return;
		}
		
		//gather up commands
		CSModelConversionParameters params = new CSModelConversionParameters();
		for (int i = 0; i < arguments.length; ++i)
		{
			//input
			if (arguments[i].equalsIgnoreCase(PARAM_NAME_INPUT) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_INPUT_SHORT) == true)
			{
				if (i+1 < arguments.length)
					params.mstrInputFilepath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_OUTPUT) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_OUTPUT_SHORT) == true)
			{
				if (i+1 < arguments.length)
					params.mstrOutputFilepath = StringUtils.standardiseFilePath(arguments[i+1]);
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//features
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_FEATURES) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_FEATURES_SHORT) == true)
			{
				if (i+1 < arguments.length)
					ParseFeatures(params, arguments[i+1]);
				else
					Logging.logFatal("No features provided!");
				i++;
			}
			
			//vertex declaration
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_VERTEXDECLARATION) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_VERTEXDECLARATION_SHORT) == true)
			{
				if (i+1 < arguments.length)
					ParseVertexDeclaration(params, arguments[i+1]);
				else
					Logging.logFatal("No vertex declaration provided!");
				i++;
			}
			
			//modifications
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS_SHORT) == true)
			{
				if (i+1 < arguments.length)
					ParseModifications(params, arguments[i+1]);
				else
					Logging.logFatal("No modifications provided!");
				i++;
			}
			
			//help
			else if (arguments[i].equalsIgnoreCase(PARAM_NAME_HELP) == true || arguments[i].equalsIgnoreCase(PARAM_NAME_HELP_SHORT) == true)
			{
				PrintHelpText();
				return;
			}
			
			//failure
			else
			{
				Logging.logFatal("Invalid argument found: " + arguments[i]);
			}
		}
		
		//check for weird combinations of parameters
		ParamsChecker.CheckParameters(params);
		
		CSModelConverterTool converterTool = new CSModelConverterTool();
		converterTool.Convert(params);
		
		Logging.finish();
	}
	//-------------------------------------------------------------------
	/// Parse Features
	///
	/// Parses a string of features.
	///
	/// @param the conversion params.
	/// @param the string to parse.
	//-------------------------------------------------------------------
	static void ParseFeatures(CSModelConversionParameters inParams, String instrFeatures)
	{
		for (int i = 0; i < instrFeatures.length(); ++i)
		{
			if (instrFeatures.charAt(i) == FEATURE_HASANIMATIONDATA)
				inParams.mbHasAnimationData = true;
		}
	}
	//-------------------------------------------------------------------
	/// Parse Vertex Declaration
	///
	/// Parses a string vertex declaration.
	///
	/// @param the conversion params.
	/// @param the string to parse.
	//-------------------------------------------------------------------
	static void ParseVertexDeclaration(CSModelConversionParameters inParams, String instrVertexDecl)
	{
		for (int i = 0; i < instrVertexDecl.length(); ++i)
		{
			if (instrVertexDecl.charAt(i) == VERTEX_ELEMENT_POSITION)
				inParams.mbVertexHasPosition = true;
			if (instrVertexDecl.charAt(i) == VERTEX_ELEMENT_NORMAL)
				inParams.mbVertexHasNormal = true;
			if (instrVertexDecl.charAt(i) == VERTEX_ELEMENT_TEXCOORD)
				inParams.mbVertexHasTexCoords = true;
			if (instrVertexDecl.charAt(i) == VERTEX_ELEMENT_COLOUR)
				inParams.mbVertexHasColour = true;
			if (instrVertexDecl.charAt(i) == VERTEX_ELEMENT_WEIGHTS)
				inParams.mbVertexHasWeights = true;
			if (instrVertexDecl.charAt(i) == VERTEX_ELEMENT_JOINT_INDICES)
				inParams.mbVertexHasJointIndices = true;
		}
	}
	//-------------------------------------------------------------------
	/// Parse Modifications
	///
	/// Parses a string of modifications.
	///
	/// @param the conversion params.
	/// @param the string to parse.
	//-------------------------------------------------------------------
	static void ParseModifications(CSModelConversionParameters inParams, String instrModifications)
	{
		for (int i = 0; i < instrModifications.length(); ++i)
		{
			if (instrModifications.charAt(i) == MODIFICATION_SWAPYANDZ)
				inParams.mbSwapYAndZ = true;
			if (instrModifications.charAt(i) == MODIFICATION_FLIPYTEXCOORDS)
				inParams.mbFlipVerticalTexCoords = true;
			if (instrModifications.charAt(i) == MODIFICATION_COMBINEMESHES)
				inParams.mbCombineMeshes = true;
		}
	}
	//-------------------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//-------------------------------------------------------------------
	public static void PrintHelpText()
	{
		Logging.setLoggingLevel(LoggingLevel.k_verbose);
		Logging.logVerbose("Usage: java -jar ColladaToCSModel.jar " + PARAM_NAME_INPUT + " <file path> " + PARAM_NAME_OUTPUT + " <file path> [" + PARAM_NAME_FEATURES + " <features>] [" + PARAM_NAME_VERTEXDECLARATION + " <vertex declaration>] [" + PARAM_NAME_TRANSFORMS + " <transforms>] [" + PARAM_NAME_HELP + "] [" + Logging.k_paramLoggingLevel + " <level>]");
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" " + PARAM_NAME_INPUT + "(" + PARAM_NAME_INPUT_SHORT + "): The input filename.");
		Logging.logVerbose(" " + PARAM_NAME_OUTPUT + "(" + PARAM_NAME_OUTPUT_SHORT + "): The output filename.");
		Logging.logVerbose(" " + PARAM_NAME_FEATURES + "(" + PARAM_NAME_FEATURES_SHORT + "): [Optional] A list of features that the output model will have.");
		Logging.logVerbose(" " + PARAM_NAME_VERTEXDECLARATION + "(" + PARAM_NAME_VERTEXDECLARATION_SHORT + "): [Optional] A list of elements that a vertex in this model will contain.");
		Logging.logVerbose(" " + PARAM_NAME_TRANSFORMS + "(" + PARAM_NAME_TRANSFORMS_SHORT + "): [Optional] A list of alterations to the output data.");
		Logging.logVerbose(" " + Logging.k_paramLoggingLevel + "(" + Logging.k_paramLoggingLevelShort + "): [Optional] The level of messages to log.");
		Logging.logVerbose(" " + PARAM_NAME_HELP + "(" + PARAM_NAME_HELP_SHORT + "): [Optional] Display this help message.");
		Logging.logVerbose("Features:");
		Logging.logVerbose(" " + FEATURE_HASANIMATIONDATA + ": The model will be outputed containing animation data.");
		Logging.logVerbose("Vertex Elements:");
		Logging.logVerbose(" " + VERTEX_ELEMENT_POSITION + ": A vertex will contain a position.");
		Logging.logVerbose(" " + VERTEX_ELEMENT_TEXCOORD + ": A vertex will contain texture coordinates.");
		Logging.logVerbose(" " + VERTEX_ELEMENT_NORMAL + ": A vertex will contain a normal.");
		Logging.logVerbose(" " + VERTEX_ELEMENT_COLOUR + ": A vertex will contain a colour.");
		Logging.logVerbose(" " + VERTEX_ELEMENT_WEIGHTS + ": A vertex will contain weights.");
		Logging.logVerbose(" " + VERTEX_ELEMENT_JOINT_INDICES + ": A vertex will contain joint indices.");
		Logging.logVerbose("Transforms:");
		Logging.logVerbose(" " + MODIFICATION_SWAPYANDZ + ": Z and Y coordinates of output data will be swapped.");
		Logging.logVerbose(" " + MODIFICATION_FLIPYTEXCOORDS + ": Texture coordinates will be flipped in the y axis.");
		Logging.logVerbose(" " + MODIFICATION_COMBINEMESHES + ": Combines all meshes that use the same material into 1 mesh. Note: This doesn't work for animated models.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" " + Logging.k_loggingLevelNone + ": No logging.");
		Logging.logVerbose(" " + Logging.k_loggingLevelFatal + ": Only log fatal errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelError + ": Only log errors.");
		Logging.logVerbose(" " + Logging.k_loggingLevelWarning + ": Log errors and warnings.");
		Logging.logVerbose(" " + Logging.k_loggingLevelVerbose + ": Log all messages.");
	}
}
