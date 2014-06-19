//
// CModelChecker.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 18/04/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter;

import com.chillisource.csmodelconverter.csmodel.CSModel;
import com.chillisource.csmodelconverter.csmodel.CSModelMesh;
import com.taggames.toolutils.SCLogger;

public class SCModelChecker 
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
			SCLogger.LogWarning("Mesh is too large! This model will require integer indices to render which chilli source currently does not support!");
		}
		
		//test the amount of skeleton nodes
		if (inModel.mSkeleton.mNodeList.size() > 256)
		{
			SCLogger.LogWarning("The skeleton contains a huge number of nodes! Over 256 nodes is probably too many.");
		}
		
		//test the amount of joints
		if (SCSkeletonBuilder.GetNumberOfJoints(inModel) > 60)
		{
			SCLogger.LogWarning("The skeleton contains a huge number of joints! Over 60 nodes is probably too many.");
		}
	}
}
