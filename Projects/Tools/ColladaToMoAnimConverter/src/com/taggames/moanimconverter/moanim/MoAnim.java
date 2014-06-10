package com.taggames.moanimconverter.moanim;

import java.util.LinkedList;

public class MoAnim
{
	/**
	 * Private data
	 */
	public MoAnimSkeleton mSkeleton;
	public LinkedList<MoAnimFrame> mFrames;
	public float mfFrameRate;
	
	/**
	 * Constructor
	 */
	public MoAnim()
	{
		mfFrameRate = 0.0f;
		mSkeleton = new MoAnimSkeleton();
		mFrames = new LinkedList<MoAnimFrame>();
	}
}
