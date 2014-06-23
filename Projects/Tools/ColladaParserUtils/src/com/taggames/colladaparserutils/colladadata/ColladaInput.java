//
// ColladaInput.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaInput 
{
	public String mstrSemantic;
	public String mstrSource;
	public int mdwOffset;
	public int mdwSet;
	
	public ColladaInput()
	{
		mstrSemantic = new String();
		mstrSource = new String();
		mdwOffset = 0;
		mdwSet = 0;
	}
}
