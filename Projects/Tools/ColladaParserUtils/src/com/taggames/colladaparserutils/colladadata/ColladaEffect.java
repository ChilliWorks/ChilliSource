//
// ColladaEffect.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaEffect 
{
	public String mstrId;
	public ColladaProfileCommon mProfileCommon;
	//NOTE: "Extra" effect information is not read, as it is unlikely to be needed any time soon.
	
	public ColladaEffect()
	{
		mstrId = new String();
		mProfileCommon = new ColladaProfileCommon();
	}
}
