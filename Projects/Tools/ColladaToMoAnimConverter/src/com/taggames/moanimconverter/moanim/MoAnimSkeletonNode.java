package com.taggames.moanimconverter.moanim;

import com.taggames.toolutils.CMatrix4;

public class MoAnimSkeletonNode 
{
	public String mstrId;
	public String mstrName;
	public String mstrParentId;
	public int mdwParentNodeIndex;
	public CMatrix4 mInitialPoseMatrix;
	
	public MoAnimSkeletonNode()
	{
		mstrId = new String();
		mstrName = new String();
		mstrParentId = new String();
		mdwParentNodeIndex = -1; 
		mInitialPoseMatrix = new CMatrix4();
	}
}
