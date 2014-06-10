//
// ColladaBindVertexInput.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

public class ColladaBindVertexInput 
{
	public String mstrSemantic;
	public String mstrInputSemantic;
	public String mstrInputSet;
	
	public ColladaBindVertexInput()
	{
		mstrSemantic = new String();
		mstrInputSemantic = new String();
		mstrInputSet = new String();
	}
}
