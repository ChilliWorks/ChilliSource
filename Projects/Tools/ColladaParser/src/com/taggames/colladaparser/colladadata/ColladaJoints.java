//
// ColladaInstanceJoints.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

import java.util.LinkedList;

public class ColladaJoints 
{
	public LinkedList<ColladaInput> mInputs;
	
	public ColladaJoints()
	{
		mInputs = new LinkedList<ColladaInput>();
	}
}
