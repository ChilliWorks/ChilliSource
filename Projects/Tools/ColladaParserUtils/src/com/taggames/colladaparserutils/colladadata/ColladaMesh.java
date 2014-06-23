//
// ColladaMesh.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

import java.util.Hashtable;
import java.util.LinkedList;

public class ColladaMesh 
{
	public Hashtable<String, ColladaSource> mSources;
	public ColladaVertices mVertices;
	public LinkedList<ColladaTriangles> mTrianglesList;
	
	public ColladaMesh()
	{
		mSources = new Hashtable<String, ColladaSource>();
		mVertices = new ColladaVertices();
		mTrianglesList = new LinkedList<ColladaTriangles>();
	}
}
