//
// ColladaSampler.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparserutils.colladadata;

import java.util.Hashtable;


public class ColladaSampler 
{
	public String mstrId;
	public Hashtable<String, ColladaInput> mInputTable;
	
	public ColladaSampler()
	{
		mstrId = new String();
		mInputTable = new Hashtable<String, ColladaInput>();
	}
}
