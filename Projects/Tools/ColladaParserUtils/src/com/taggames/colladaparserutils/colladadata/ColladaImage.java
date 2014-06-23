//
// ColladaImage.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaImage 
{
	public String mstrId;
	public ColladaInitFrom mInitFrom;
	
	public ColladaImage()
	{
		mstrId = new String();
		mInitFrom = new ColladaInitFrom();
	}
}
