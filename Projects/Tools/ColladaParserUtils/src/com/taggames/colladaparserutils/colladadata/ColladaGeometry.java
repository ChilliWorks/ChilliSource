//
// ColladaGeometry.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaGeometry 
{
	public enum COLLADA_GEOMETRY_TYPE
	{
		NONE,
		MESH,
		SPLINE
	}
	
	public ColladaMesh mMesh;
	public String mstrId;
	public String mstrName;
	public COLLADA_GEOMETRY_TYPE meType; 
	
	public ColladaGeometry()
	{
		mMesh = new ColladaMesh();
		mstrId = new String();
		mstrName = new String();
		meType = COLLADA_GEOMETRY_TYPE.NONE;
	}
}
