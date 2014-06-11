//
// MoModelConversionParameters.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter;

public class CSModelConversionParameters 
{
	public String mstrInputFilepath;
	public String mstrOutputFilepath;
	public boolean mbHasAnimationData;
	public boolean mbVertexHasPosition;
	public boolean mbVertexHasTexCoords;
	public boolean mbVertexHasNormal;
	public boolean mbVertexHasColour;
	public boolean mbVertexHasWeights;
	public boolean mbVertexHasJointIndices;
	public boolean mbSwapYAndZ;
	public boolean mbFlipVerticalTexCoords;
	public boolean mbCombineMeshes;
	
	public CSModelConversionParameters()
	{
		mstrInputFilepath 		= new String();
		mstrOutputFilepath 		= new String();
		mbHasAnimationData 		= false;
		mbVertexHasPosition 	= false;
		mbVertexHasTexCoords 	= false;
		mbVertexHasNormal 		= false;
		mbVertexHasColour 		= false;
		mbVertexHasWeights 		= false;
		mbVertexHasJointIndices = false;
		mbSwapYAndZ 			= false;
		mbFlipVerticalTexCoords = false;
		mbCombineMeshes			= false;
	}
}
