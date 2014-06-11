//
// ColladaVisualScene.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

import java.util.Hashtable;

public class ColladaVisualScene 
{
	public Hashtable<String, ColladaNode> mRootNodes;
	public String mstrId;
	
	public ColladaVisualScene()
	{
		mRootNodes = new Hashtable<String, ColladaNode>();
		mstrId = new String();
	}
}
