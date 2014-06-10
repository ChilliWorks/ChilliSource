package com.taggames.moanimconverter.moanim;

import java.util.LinkedList;

public class MoAnimSkeleton 
{
	public LinkedList<MoAnimSkeletonNode> mNodeList;
	public boolean mbLocked;
	
	public MoAnimSkeleton()
	{
		mNodeList = new LinkedList<MoAnimSkeletonNode>();
		mbLocked = false;
	}
}
