//
// MoModelJoint.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.colladatocsmodel.csmodel;

public class CSModelSkeletonNode
{
	enum SKELETON_NODE_TYPE
	{
		STANDARD,
		JOINT
	}
	public String mstrId;
	public String mstrParentId;
	public String mstrName;
	SKELETON_NODE_TYPE meType;
	public int mdwParentNodeIndex;

	//Joint specific stuff
	public boolean mbIsJoint;
	public String mstrSId;
	public int mdwJointIndex; 
	
	public CSModelSkeletonNode()
	{
		mstrId = new String();
		mstrParentId = new String();
		mstrName = new String();
		mdwParentNodeIndex = -1;
		
		mbIsJoint = false;
		mstrSId = new String();
		mdwJointIndex = -1;
	}
}
