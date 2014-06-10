//
// ColladaSkin.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

import java.util.Hashtable;

public class ColladaSkin 
{
	public String mstrSource;
	public ColladaBindShapeMatrix mBindShapeMatrix;
	public Hashtable<String, ColladaSource> mSourceTable;
	public ColladaJoints mJoints;
	public ColladaVertexWeights mVertexWeights;
	
	public ColladaSkin()
	{
		mstrSource = new String();
		mBindShapeMatrix = new ColladaBindShapeMatrix();
		mSourceTable = new Hashtable<String, ColladaSource>();
		mJoints = new ColladaJoints();
		mVertexWeights = new ColladaVertexWeights();
	}
}
