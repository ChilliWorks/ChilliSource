//
// MoModelMesh.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.colladatocsmodel.csmodel;

import java.util.LinkedList;

import com.chillisource.toolutils.Matrix4;
import com.chillisource.toolutils.Vector3;

public class CSModelMesh 
{
	public LinkedList<CSModelVertex> mVertexList;
	public LinkedList<Integer> mIndexList;
	public String mstrTextureName;
	public CSModelMaterial mMaterial;
	public Vector3 mvMax;
	public Vector3 mvMin;
	public String mstrName;
	public CSModelSkeleton mSkeleton;
	public Matrix4[] maInverseBindMatrices;
	
	public CSModelMesh()
	{
		maInverseBindMatrices = null;
		mVertexList = new LinkedList<CSModelVertex>();
		mIndexList = new LinkedList<Integer>();
		mvMax = new Vector3(-Float.MAX_VALUE, -Float.MAX_VALUE, -Float.MAX_VALUE);
		mvMin = new Vector3(Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
		mstrTextureName = new String();
		mMaterial = new CSModelMaterial();
		mstrName = new String();
		mSkeleton = new CSModelSkeleton();
	}
}
