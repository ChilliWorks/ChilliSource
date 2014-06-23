package com.chillisource.csanimconverter.csanim;

import com.chillisource.toolutils.Matrix4;

public class CSAnimSkeletonNode 
{
	public String mstrId;
	public String mstrName;
	public String mstrParentId;
	public int mdwParentNodeIndex;
	public Matrix4 mInitialPoseMatrix;
	
	public CSAnimSkeletonNode()
	{
		mstrId = new String();
		mstrName = new String();
		mstrParentId = new String();
		mdwParentNodeIndex = -1; 
		mInitialPoseMatrix = new Matrix4();
	}
}
