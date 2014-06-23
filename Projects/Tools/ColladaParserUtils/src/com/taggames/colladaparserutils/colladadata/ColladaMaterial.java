//
// ColladaMaterial.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaMaterial 
{
	public String mstrId;
	public String mstrName;
	public ColladaInstanceEffect mInstanceEffect;
	
	public ColladaMaterial()
	{
		mstrId = new String();
		mstrName = new String();
		mInstanceEffect = new ColladaInstanceEffect();
	}
}
