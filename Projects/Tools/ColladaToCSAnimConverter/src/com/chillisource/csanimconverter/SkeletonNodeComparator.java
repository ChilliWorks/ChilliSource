//
// JointComparator.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csanimconverter;

import java.util.Comparator;

import com.chillisource.csanimconverter.csanim.CSAnimSkeletonNode;

public class SkeletonNodeComparator implements Comparator<CSAnimSkeletonNode>
{
	//-------------------------------------------------------------------
	/// Compare
	///
	/// Compares two MoModelSkeletonNode
	//-------------------------------------------------------------------
	public int compare(CSAnimSkeletonNode inJoint1, CSAnimSkeletonNode inJoint2)
	{
		
		if (inJoint1.mstrParentId.compareTo(inJoint2.mstrParentId) > 0)
			return 1;
		else if (inJoint1.mstrParentId.compareTo(inJoint2.mstrParentId) < 0)
			return -1;
		else
			return 0;
	}
}
