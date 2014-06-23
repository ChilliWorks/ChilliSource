//
// ColladaSurface.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaSurface 
{
	public String mstrType;
	public ColladaInitFrom mInitFrom;
	
	public ColladaSurface()
	{
		mstrType = new String();
		mInitFrom = new ColladaInitFrom();
	}
}
