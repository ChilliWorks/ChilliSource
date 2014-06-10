//
// CMain.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.momodelconverter;

import com.taggames.toolutils.*;

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
	private static final char FEATURE_HASTEXTURE					= 't';
	private static final char FEATURE_HASMATERIAL					= 'm';
	private static final char FEATURE_HASANIMATIONDATA				= 'a';
	private static final char VERTEX_ELEMENT_POSITION				= 'p';
	private static final char VERTEX_ELEMENT_TEXCOORD				= 't';
	private static final char VERTEX_ELEMENT_NORMAL					= 'n';
	private static final char VERTEX_ELEMENT_COLOUR					= 'c';
	private static final char VERTEX_ELEMENT_WEIGHTS				= 'w';
	private static final char VERTEX_ELEMENT_JOINT_INDICES			= 'j';
	private static final char MODIFICATION_SWAPYANDZ				= 'y';
	private static final char MODIFICATION_MIRRORINXZPLANE			= 'm';
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
		SCLogger.Initialise(inastrArgs);
		
		//check the number of arguments make sense.
		if (inastrArgs.length == 0)
		{
			PrintHelpText();
			return;
		}
		
		//gather up commands
		MoModelConversionParameters params = new MoModelConversionParameters();
		for (int i = 0; i < inastrArgs.length; ++i)
		{
			//input
			if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_INPUT) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_INPUT_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					params.mstrInputFilepath = SCStringUtils.StandardiseFilepath(inastrArgs[i+1]);
				else
					SCLogger.LogFatalError("No input file provided!");
				i++;
			}
			
			//output
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_OUTPUT) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_OUTPUT_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					params.mstrOutputFilepath = SCStringUtils.StandardiseFilepath(inastrArgs[i+1]);
				else
					SCLogger.LogFatalError("No output file provided!");
				i++;
			}
			
			//features
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_FEATURES) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_FEATURES_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					ParseFeatures(params, inastrArgs[i+1]);
				else
					SCLogger.LogFatalError("No features provided!");
				i++;
			}
			
			//vertex declaration
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_VERTEXDECLARATION) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_VERTEXDECLARATION_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					ParseVertexDeclaration(params, inastrArgs[i+1]);
				else
					SCLogger.LogFatalError("No vertex declaration provided!");
				i++;
			}
			
			//modifications
			else if (inastrArgs[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS) == true || inastrArgs[i].equalsIgnoreCase(PARAM_NAME_TRANSFORMS_SHORT) == true)
			{
				if (i+1 < inastrArgs.length)
					ParseModifications(params, inastrArgs[i+1]);
				else
					SCLogger.LogFatalError("No modifications provided!");
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
		
		CMoModelConverterTool converterTool = new CMoModelConverterTool();
		converterTool.Convert(params);
		
		SCLogger.PrintErrorsAndWarnings();
	}
	//-------------------------------------------------------------------
	/// Parse Features
	///
	/// Parses a string of features.
	///
	/// @param the conversion params.
	/// @param the string to parse.
	//-------------------------------------------------------------------
	static void ParseFeatures(MoModelConversionParameters inParams, String instrFeatures)
	{
		for (int i = 0; i < instrFeatures.length(); ++i)
		{
			if (instrFeatures.charAt(i) == FEATURE_HASTEXTURE)
				inParams.mbHasTexture = true;
			if (instrFeatures.charAt(i) == FEATURE_HASMATERIAL)
				inParams.mbHasMaterial = true;
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
	static void ParseVertexDeclaration(MoModelConversionParameters inParams, String instrVertexDecl)
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
	static void ParseModifications(MoModelConversionParameters inParams, String instrModifications)
	{
		for (int i = 0; i < instrModifications.length(); ++i)
		{
			if (instrModifications.charAt(i) == MODIFICATION_SWAPYANDZ)
				inParams.mbSwapYAndZ = true;
			if (instrModifications.charAt(i) == MODIFICATION_MIRRORINXZPLANE)
				inParams.mbMirrorInXZPlane = true;
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
		SCLogger.LogMessage("Parameters:");
		SCLogger.LogMessage(" '" + PARAM_NAME_INPUT + "'('" + PARAM_NAME_INPUT_SHORT + "') -> The input filename.");
		SCLogger.LogMessage(" '" + PARAM_NAME_OUTPUT + "'('" + PARAM_NAME_OUTPUT_SHORT + "') -> The output filename.");
		SCLogger.LogMessage(" '" + PARAM_NAME_FEATURES + "'('" + PARAM_NAME_FEATURES_SHORT + "') -> A list of features that the output model will have.");
		SCLogger.LogMessage(" '" + PARAM_NAME_VERTEXDECLARATION + "'('" + PARAM_NAME_VERTEXDECLARATION_SHORT + "') -> A list of elements that a vertex in this model will contain.");
		SCLogger.LogMessage(" '" + PARAM_NAME_TRANSFORMS + "'('" + PARAM_NAME_TRANSFORMS_SHORT + "') -> A list of alterations to the output data.");
		SCLogger.LogMessage(" '--logginglevel'('-l') -> the level of messages to log.");
		SCLogger.LogMessage(" '--errordisplay'('-e') -> when to display errors.");
		SCLogger.LogMessage(" '" + PARAM_NAME_HELP + "'('" + PARAM_NAME_HELP_SHORT + "') -> Display this help message.");
		SCLogger.LogMessage("Features:");
		SCLogger.LogMessage(" '" + FEATURE_HASTEXTURE + "' -> The model will be outputed containing a texture.");
		SCLogger.LogMessage(" '" + FEATURE_HASMATERIAL + "' -> The model will be outputed containing a material.");
		SCLogger.LogMessage(" '" + FEATURE_HASANIMATIONDATA+ "' -> The model will be outputed containing animation data.");
		SCLogger.LogMessage("Vertex Elements:");
		SCLogger.LogMessage(" '" + VERTEX_ELEMENT_POSITION + "' -> A vertex will contain a position.");
		SCLogger.LogMessage(" '" + VERTEX_ELEMENT_TEXCOORD + "' -> A vertex will contain texture coordinates.");
		SCLogger.LogMessage(" '" + VERTEX_ELEMENT_NORMAL + "' -> A vertex will contain a normal.");
		SCLogger.LogMessage(" '" + VERTEX_ELEMENT_COLOUR + "' -> A vertex will contain a colour.");
		SCLogger.LogMessage(" '" + VERTEX_ELEMENT_WEIGHTS + "' -> A vertex will contain weights.");
		SCLogger.LogMessage(" '" + VERTEX_ELEMENT_JOINT_INDICES + "' -> A vertex will contain joint indices.");
		SCLogger.LogMessage("Transforms:");
		SCLogger.LogMessage(" '" + MODIFICATION_SWAPYANDZ + "' -> Z and Y coordinates of output data will be swapped.");
		SCLogger.LogMessage(" '" + MODIFICATION_MIRRORINXZPLANE + "' -> The output data will be mirrored in the XY plane.");
		SCLogger.LogMessage(" '" + MODIFICATION_FLIPYTEXCOORDS + "' -> Texture coordinates will be flipped in the y axis.");
		SCLogger.LogMessage(" '" + MODIFICATION_COMBINEMESHES + "' -> Combines all meshes that use the same material into 1 mesh. Note: This doesn't work for animated models.");
		SCLogger.LogMessage("Logging Levels:");
		SCLogger.LogMessage(" 'none' -> No logging.");
		SCLogger.LogMessage(" 'fatal' -> Only log fatal errors.");
		SCLogger.LogMessage(" 'error' -> Only log errors.");
		SCLogger.LogMessage(" 'warning' -> Log errors and warnings.");
		SCLogger.LogMessage(" 'verbose' -> Log all messages.");
		SCLogger.LogMessage("Error Display Options:");
		SCLogger.LogMessage(" 'never' -> Never display.");
		SCLogger.LogMessage(" 'atend' -> Log all errors at the end.");
		SCLogger.LogMessage(" 'whenreceived' -> Log all errors when received.");
	}
}
