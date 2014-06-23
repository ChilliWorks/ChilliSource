//
// ColladaFloatArray.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaFloatArray 
{
	public String mstrId;
	public int mdwCount;
	public float[] mafData;
	
	public ColladaFloatArray()
	{
		mstrId = new String();
		mdwCount = 0;
		mafData = null;
	}
}
