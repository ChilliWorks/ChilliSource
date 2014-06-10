//
// ColladaInstanceMaterial.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

public class ColladaInstanceMaterial 
{
	public String mstrSymbol;
	public String mstrTarget;
	public ColladaBindVertexInput mBindVertexInput;
	
	public ColladaInstanceMaterial()
	{
		mstrSymbol = new String();
		mstrTarget = new String();
		mBindVertexInput = new ColladaBindVertexInput();
	}
}
