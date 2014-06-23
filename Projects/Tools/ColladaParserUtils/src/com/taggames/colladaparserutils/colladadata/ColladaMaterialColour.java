//
// ColladaMaterialColour.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaMaterialColour 
{
	public ColladaColour mColour;
	public ColladaTexture mTexture;
	
	public ColladaMaterialColour()
	{
		mColour = new ColladaColour();
		mTexture = new ColladaTexture();
	}
}
