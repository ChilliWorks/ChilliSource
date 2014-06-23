//
// ColladaVertexWeights.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

import java.util.LinkedList;

public class ColladaVertexWeights 
{
	public int mdwCount;
	public LinkedList<ColladaInput> mInputs;
	public ColladaVCount mVCount;
	public ColladaV mV;
	
	public ColladaVertexWeights()
	{
		mdwCount = 0;
		mInputs = new LinkedList<ColladaInput>();
		mVCount = new ColladaVCount();
		mV = new ColladaV();
	}
}
