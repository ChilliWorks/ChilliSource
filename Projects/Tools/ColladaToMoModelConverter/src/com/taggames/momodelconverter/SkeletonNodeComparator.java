//
// JointComparator.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.momodelconverter;

import java.util.Comparator;

import com.taggames.momodelconverter.momodel.MoModelSkeletonNode;

public class SkeletonNodeComparator implements Comparator<MoModelSkeletonNode>
{
	//-------------------------------------------------------------------
	/// Compare
	///
	/// Compares two MoModelSkeletonNode
	//-------------------------------------------------------------------
	public int compare(MoModelSkeletonNode inJoint1, MoModelSkeletonNode inJoint2)
	{
		
		if (inJoint1.mstrParentId.compareTo(inJoint2.mstrParentId) > 0)
			return 1;
		else if (inJoint1.mstrParentId.compareTo(inJoint2.mstrParentId) < 0)
			return -1;
		else
			return 0;
	}
}
