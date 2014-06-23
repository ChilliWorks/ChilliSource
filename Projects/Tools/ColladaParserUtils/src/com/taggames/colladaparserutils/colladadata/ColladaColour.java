//
// ColladaColour.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

public class ColladaColour 
{
	public float mfR;
	public float mfG;
	public float mfB;
	public float mfA;
	
	public ColladaColour()
	{
		mfR = 1.0f;
		mfG = 1.0f;
		mfB = 1.0f;
		mfA = 1.0f;
	}
	
	public ColladaColour(float infR, float infG, float infB, float infA)
	{
		mfR = infR;
		mfG = infG;
		mfB = infB;
		mfA = infA;
	}
}
