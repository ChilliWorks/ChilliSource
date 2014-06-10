//
// ColladaProfileCommon.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

import java.util.Hashtable;

public class ColladaProfileCommon 
{
	public Hashtable<String, ColladaNewParam> mNewParams;
	public ColladaTechnique mTechnique;
	
	public ColladaProfileCommon()
	{
		mNewParams = new Hashtable<String, ColladaNewParam>();
		mTechnique = new ColladaTechnique();
	}
}
