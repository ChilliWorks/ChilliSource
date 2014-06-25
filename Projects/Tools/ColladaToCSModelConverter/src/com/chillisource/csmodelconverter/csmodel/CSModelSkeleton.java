//
// MoModelSkeleton.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter.csmodel;

import java.util.LinkedList;

public class CSModelSkeleton 
{
	public LinkedList<CSModelSkeletonNode> mNodeList;
	public boolean mbLocked;
	
	public CSModelSkeleton()
	{
		mNodeList = new LinkedList<CSModelSkeletonNode>();
		mbLocked = false;
	}
}
