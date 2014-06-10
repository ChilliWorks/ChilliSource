//
// ColladaAccessor.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

import java.util.LinkedList;

public class ColladaAccessor 
{
	public LinkedList<ColladaParam> mParams;
	public int mdwCount;
	public int mdwStride;
	public String mstrSource;
	
	public ColladaAccessor()
	{
		mParams = new LinkedList<ColladaParam>();
		mdwCount = 0;
		mdwStride = 0;
		mstrSource = new String();
	}
}
