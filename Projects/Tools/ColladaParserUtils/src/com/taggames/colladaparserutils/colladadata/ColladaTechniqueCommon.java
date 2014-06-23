//
// ColladaTechniqueCommon.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

import java.util.LinkedList;

public class ColladaTechniqueCommon 
{
	public ColladaAccessor mAccessor;
	public LinkedList<ColladaInstanceMaterial> mInstanceMaterialList;
	
	public ColladaTechniqueCommon()
	{
		mAccessor = new ColladaAccessor();
		mInstanceMaterialList = new LinkedList<ColladaInstanceMaterial>();
	}
}
