//
// ColladaTriangles.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

import java.util.LinkedList;

public class ColladaTriangles 
{
	public String mstrMaterial;
	public int mdwCount;
	public LinkedList<ColladaInput> mInputList;
	public ColladaP mP;
	
	public ColladaTriangles()
	{
		mstrMaterial = new String();
		mdwCount = 0;
		mInputList = new LinkedList<ColladaInput>();
		mP = new ColladaP();
	}
}
