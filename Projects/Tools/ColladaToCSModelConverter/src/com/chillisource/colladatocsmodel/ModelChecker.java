//
// CModelChecker.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 18/04/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.colladatocsmodel;

import com.chillisource.colladatocsmodel.csmodel.CSModel;
import com.chillisource.colladatocsmodel.csmodel.CSModelMesh;
import com.chillisource.toolutils.Logging;

public class ModelChecker 
{
	static final int kdwMaxUnsignedShort = 65536;
	//----------------------------------------------------
	/// Check Model
	///
	/// Checks whether the model will be able to be
	/// loaded into chilli source. This checks things like
	/// triangle count and number of mesh influencing joints.
	///
	/// @param The model.
	//----------------------------------------------------
	static public void CheckModel(CSModel inModel)
	{
		//Test that the model is not too big for chilli source to render.
		boolean bContainsLargeMesh = false;
		for (CSModelMesh mesh : inModel.mMeshTable.values())
		{
			boolean bFoundLargeIndex = false;
			for (Integer dwIndex : mesh.mIndexList)
			{
				if (dwIndex > kdwMaxUnsignedShort)
				{
					bFoundLargeIndex = true;
				}
			}
			
			if (bFoundLargeIndex == true || mesh.mIndexList.size() > kdwMaxUnsignedShort)
			{
				bContainsLargeMesh = true;
				break;
			}
		}
		if (bContainsLargeMesh == true)
		{
			Logging.logWarning("Mesh is too large! This model will require integer indices to render which chilli source currently does not support!");
		}
		
		//test the amount of skeleton nodes
		if (inModel.mSkeleton.mNodeList.size() > 256)
		{
			Logging.logWarning("The skeleton contains a huge number of nodes! Over 256 nodes is probably too many.");
		}
		
		//test the amount of joints
		if (SkeletonBuilder.GetNumberOfJoints(inModel) > 60)
		{
			Logging.logWarning("The skeleton contains a huge number of joints! Over 60 nodes is probably too many.");
		}
	}
}
