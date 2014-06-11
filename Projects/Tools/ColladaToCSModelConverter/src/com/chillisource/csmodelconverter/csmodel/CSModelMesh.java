//
// MoModelMesh.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter.csmodel;

import java.util.LinkedList;

import com.taggames.toolutils.CMatrix4;
import com.taggames.toolutils.CVector3;

public class CSModelMesh 
{
	public LinkedList<CSModelVertex> mVertexList;
	public LinkedList<Integer> mIndexList;
	public String mstrTextureName;
	public CSModelMaterial mMaterial;
	public CVector3 mvMax;
	public CVector3 mvMin;
	public String mstrName;
	public CSModelSkeleton mSkeleton;
	public CMatrix4[] maInverseBindMatrices;
	
	public CSModelMesh()
	{
		maInverseBindMatrices = null;
		mVertexList = new LinkedList<CSModelVertex>();
		mIndexList = new LinkedList<Integer>();
		mvMax = new CVector3(-Float.MAX_VALUE, -Float.MAX_VALUE, -Float.MAX_VALUE);
		mvMin = new CVector3(Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
		mstrTextureName = new String();
		mMaterial = new CSModelMaterial();
		mstrName = new String();
		mSkeleton = new CSModelSkeleton();
	}
}
