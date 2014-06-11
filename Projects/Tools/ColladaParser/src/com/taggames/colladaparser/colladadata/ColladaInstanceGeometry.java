//
// ColladaInstanceGeometry.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

public class ColladaInstanceGeometry 
{
	public String mstrUrl;
	public ColladaBindMaterial mBindMaterial;
	
	public ColladaInstanceGeometry()
	{
		mstrUrl = new String();
		mBindMaterial = new ColladaBindMaterial();
	}
}
