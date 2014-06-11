//
// MoModelSkeleton.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.momodelconverter.momodel;

import java.util.LinkedList;

public class MoModelSkeleton 
{
	public LinkedList<MoModelSkeletonNode> mNodeList;
	public boolean mbLocked;
	
	public MoModelSkeleton()
	{
		mNodeList = new LinkedList<MoModelSkeletonNode>();
		mbLocked = false;
	}
}
