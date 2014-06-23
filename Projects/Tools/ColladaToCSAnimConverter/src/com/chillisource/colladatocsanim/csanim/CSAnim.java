package com.chillisource.colladatocsanim.csanim;

import java.util.LinkedList;

public class CSAnim
{
	/**
	 * Private data
	 */
	public CSAnimSkeleton mSkeleton;
	public LinkedList<CSAnimFrame> mFrames;
	public float mfFrameRate;
	
	/**
	 * Constructor
	 */
	public CSAnim()
	{
		mfFrameRate = 0.0f;
		mSkeleton = new CSAnimSkeleton();
		mFrames = new LinkedList<CSAnimFrame>();
	}
}
