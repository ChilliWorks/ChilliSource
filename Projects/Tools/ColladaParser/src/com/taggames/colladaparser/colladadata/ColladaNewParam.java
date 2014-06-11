//
// ColladaNewParam.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

public class ColladaNewParam 
{
	public String mstrSId;
	public ColladaSurface mSurface;
	public ColladaSampler2D mSampler2D;
	
	public ColladaNewParam()
	{
		mstrSId = new String();
		mSurface = new ColladaSurface();
		mSampler2D = new ColladaSampler2D();
	}
}
