package com.chillisource.csanimconverter;

import java.util.LinkedList;

import com.chillisource.toolutils.Matrix4;

public class Animation 
{
	public float mfFrameRate;
	public LinkedList<Matrix4> mTransformList;
	
	public Animation()
	{
		mfFrameRate = 0.0f;
		mTransformList = new LinkedList<Matrix4>();
	}
}
