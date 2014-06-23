//
// CParamsChecker.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.colladatocsmodel;

import com.chillisource.toolutils.Logging;

public class ParamsChecker 
{
	//----------------------------------------------------
	/// Check Parameters
	///
	/// Checks the input parameters for anything weird
	/// and throws a warning if it finds anything.
	///
	/// @param the parameters.
	//----------------------------------------------------
	static public void CheckParameters(CSModelConversionParameters inParams)
	{
		if (inParams.mstrInputFilepath.length() == 0 || inParams.mstrOutputFilepath.length() == 0)
		{
			Logging.logFatal("Must provide an input and output file path.");
		}
		
		//check for no positional data
		if (inParams.mbVertexHasPosition == false)
		{
			Logging.logWarning("The 'positions' vertex element is not enabled meaning the output model will have no vertex position data.");
		}
		
		//check for use of animation without the animation vertex elements
		if (inParams.mbHasAnimationData == true && (inParams.mbVertexHasJointIndices == false || inParams.mbVertexHasWeights == false))
		{
			Logging.logWarning("The 'Animation Data' feature is being used without both the animation vertex elements('Joint Indices' and 'Weights') enabled.");
		}
		
		//check for use of the animation vertex elements without animation data
		if (inParams.mbHasAnimationData == false && (inParams.mbVertexHasJointIndices == true || inParams.mbVertexHasWeights == true))
		{
			Logging.logWarning("Animation vertex elements(joint indices or weights) are being used without the 'Animation Data' feature enabled.");
		}
				
		//check for use of combine meshes with an animated model.
		if (inParams.mbHasAnimationData == true && inParams.mbCombineMeshes == true)
		{
			Logging.logWarning("Combine meshes is enabled for an animated mesh. This will break the animation data, so turning it off.");
			inParams.mbCombineMeshes = false;
		}
	}
}
