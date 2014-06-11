//
// JointComparator.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.moanimconverter;

import java.util.Comparator;

import com.taggames.moanimconverter.moanim.MoAnimSkeletonNode;

public class SkeletonNodeComparator implements Comparator<MoAnimSkeletonNode>
{
	//-------------------------------------------------------------------
	/// Compare
	///
	/// Compares two MoModelSkeletonNode
	//-------------------------------------------------------------------
	public int compare(MoAnimSkeletonNode inJoint1, MoAnimSkeletonNode inJoint2)
	{
		
		if (inJoint1.mstrParentId.compareTo(inJoint2.mstrParentId) > 0)
			return 1;
		else if (inJoint1.mstrParentId.compareTo(inJoint2.mstrParentId) < 0)
			return -1;
		else
			return 0;
	}
}
