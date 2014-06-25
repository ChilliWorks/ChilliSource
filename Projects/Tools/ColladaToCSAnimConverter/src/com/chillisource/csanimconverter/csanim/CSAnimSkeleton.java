package com.chillisource.csanimconverter.csanim;

import java.util.LinkedList;

public class CSAnimSkeleton 
{
	public LinkedList<CSAnimSkeletonNode> mNodeList;
	public boolean mbLocked;
	
	public CSAnimSkeleton()
	{
		mNodeList = new LinkedList<CSAnimSkeletonNode>();
		mbLocked = false;
	}
}
