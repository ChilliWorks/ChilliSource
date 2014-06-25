package com.chillisource.csanimconverter;

import java.util.LinkedList;

import com.taggames.toolutils.CMatrix4;

public class Animation 
{
	public float mfFrameRate;
	public LinkedList<CMatrix4> mTransformList;
	
	public Animation()
	{
		mfFrameRate = 0.0f;
		mTransformList = new LinkedList<CMatrix4>();
	}
}
