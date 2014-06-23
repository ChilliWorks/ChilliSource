//
// ColladaInstanceController.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaInstanceController 
{
	public String mstrUrl;
	public ColladaSkeleton mSkeleton;
	public ColladaBindMaterial mBindMaterial;
	
	public ColladaInstanceController()
	{
		mstrUrl = new String();
		mSkeleton = new ColladaSkeleton();
		mBindMaterial = new ColladaBindMaterial();
	}
}