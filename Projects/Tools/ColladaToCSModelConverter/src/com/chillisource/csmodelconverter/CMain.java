//
// CMain.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter;

import com.chillisource.toolutils.*;

public class CMain 
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
		Logging.start(inastrArgs);
		
		//check the number of arguments make sense.
		if (inastrArgs.length == 0)
		{
			PrintHelpText();
			return;
		}
		
		//gather up commands
		CSModelConversionParameters params = new CSModelConversionParameters();
		for (int i = 0; i < inastrArgs.length; ++i)
		{
			//input
			if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_INPUT) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_INPUT_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					params.mstrInputFilepath = StringUtils.standardiseFilepath(inastrArgs[i+1]);
				else
					Logging.logFatal("No input file provided!");
				i++;
			}
			
			//output
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_OUTPUT) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_OUTPUT_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					params.mstrOutputFilepath = StringUtils.standardiseFilepath(inastrArgs[i+1]);
				else
					Logging.logFatal("No output file provided!");
				i++;
			}
			
			//features
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_FEATURES) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_FEATURES_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					ParseFeatures(params, inastrArgs[i+1]);
				else
					Logging.logFatal("No features provided!");
				i++;
			}
			
			//vertex declaration
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_VERTEXDECLARATION) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_VERTEXDECLARATION_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					ParseVertexDeclaration(params, inastrArgs[i+1]);
				else
					Logging.logFatal("No vertex declaration provided!");
				i++;
			}
			
			//modifications
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					ParseModifications(params, inastrArgs[i+1]);
				else
					Logging.logFatal("No modifications provided!");
				i++;
			}
			
			//help
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_HELP) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_HELP_SHORT) == true)
			{
				PrintHelpText();
				return;
			}
		}
		
		//check for weird combinations of parameters
		SCParamsChecker.CheckParameters(params);
		
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
		Logging.logVerbose("Parameters:");
		Logging.logVerbose(" '" + PARAM_NAME_INPUT + "'('" + PARAM_NAME_INPUT_SHORT + "') -> The input filename.");
		Logging.logVerbose(" '" + PARAM_NAME_OUTPUT + "'('" + PARAM_NAME_OUTPUT_SHORT + "') -> The output filename.");
		Logging.logVerbose(" '" + PARAM_NAME_FEATURES + "'('" + PARAM_NAME_FEATURES_SHORT + "') -> A list of features that the output model will have.");
		Logging.logVerbose(" '" + PARAM_NAME_VERTEXDECLARATION + "'('" + PARAM_NAME_VERTEXDECLARATION_SHORT + "') -> A list of elements that a vertex in this model will contain.");
		Logging.logVerbose(" '" + PARAM_NAME_TRANSFORMS + "'('" + PARAM_NAME_TRANSFORMS_SHORT + "') -> A list of alterations to the output data.");
		Logging.logVerbose(" '--logginglevel'('-l') -> the level of messages to log.");
		Logging.logVerbose(" '--errordisplay'('-e') -> when to display errors.");
		Logging.logVerbose(" '" + PARAM_NAME_HELP + "'('" + PARAM_NAME_HELP_SHORT + "') -> Display this help message.");
		Logging.logVerbose("Features:");
		Logging.logVerbose(" '" + FEATURE_HASANIMATIONDATA+ "' -> The model will be outputed containing animation data.");
		Logging.logVerbose("Vertex Elements:");
		Logging.logVerbose(" '" + VERTEX_ELEMENT_POSITION + "' -> A vertex will contain a position.");
		Logging.logVerbose(" '" + VERTEX_ELEMENT_TEXCOORD + "' -> A vertex will contain texture coordinates.");
		Logging.logVerbose(" '" + VERTEX_ELEMENT_NORMAL + "' -> A vertex will contain a normal.");
		Logging.logVerbose(" '" + VERTEX_ELEMENT_COLOUR + "' -> A vertex will contain a colour.");
		Logging.logVerbose(" '" + VERTEX_ELEMENT_WEIGHTS + "' -> A vertex will contain weights.");
		Logging.logVerbose(" '" + VERTEX_ELEMENT_JOINT_INDICES + "' -> A vertex will contain joint indices.");
		Logging.logVerbose("Transforms:");
		Logging.logVerbose(" '" + MODIFICATION_SWAPYANDZ + "' -> Z and Y coordinates of output data will be swapped.");
		Logging.logVerbose(" '" + MODIFICATION_FLIPYTEXCOORDS + "' -> Texture coordinates will be flipped in the y axis.");
		Logging.logVerbose(" '" + MODIFICATION_COMBINEMESHES + "' -> Combines all meshes that use the same material into 1 mesh. Note: This doesn't work for animated models.");
		Logging.logVerbose("Logging Levels:");
		Logging.logVerbose(" 'none' -> No logging.");
		Logging.logVerbose(" 'fatal' -> Only log fatal errors.");
		Logging.logVerbose(" 'error' -> Only log errors.");
		Logging.logVerbose(" 'warning' -> Log errors and warnings.");
		Logging.logVerbose(" 'verbose' -> Log all messages.");
		Logging.logVerbose("Error Display Options:");
		Logging.logVerbose(" 'never' -> Never display.");
		Logging.logVerbose(" 'atend' -> Log all errors at the end.");
		Logging.logVerbose(" 'whenreceived' -> Log all errors when received.");
	}
}
