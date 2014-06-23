//
// JointComparator.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.colladatocsmodel;

import java.util.Comparator;

import com.chillisource.colladatocsmodel.csmodel.CSModelSkeletonNode;

public class SkeletonNodeComparator implements Comparator<CSModelSkeletonNode>
{
	//-------------------------------------------------------------------
	/// Compare
	///
	/// Compares two MoModelSkeletonNode
	//-------------------------------------------------------------------
	public int compare(CSModelSkeletonNode inJoint1, CSModelSkeletonNode inJoint2)
	{
		
		if (inJoint1.mstrParentId.compareTo(inJoint2.mstrParentId) > 0)
			return 1;
		else if (inJoint1.mstrParentId.compareTo(inJoint2.mstrParentId) < 0)
			return -1;
		else
			return 0;
	}
}
