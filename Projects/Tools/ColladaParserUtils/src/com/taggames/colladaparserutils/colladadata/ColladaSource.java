//
// ColladaSource.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaSource 
{
	public ColladaFloatArray mFloatArray;
	public ColladaNameArray mNameArray;
	public ColladaTechniqueCommon mTechniqueCommon;
	public String mstrId;
	public String mstrContents;
	
	public ColladaSource()
	{
		mNameArray = new ColladaNameArray();
		mFloatArray = new ColladaFloatArray();
		mTechniqueCommon = new ColladaTechniqueCommon();
		mstrId = new String();
		mstrContents = new String();
	}
}
