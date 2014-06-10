//
// MoModelMesh.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.momodelconverter.momodel;

import java.util.LinkedList;

import com.taggames.toolutils.CMatrix4;
import com.taggames.toolutils.CVector3;

public class MoModelMesh 
{
	public LinkedList<MoModelVertex> mVertexList;
	public LinkedList<Integer> mIndexList;
	public String mstrTextureName;
	public MoModelMaterial mMaterial;
	public CVector3 mvMax;
	public CVector3 mvMin;
	public String mstrName;
	public MoModelSkeleton mSkeleton;
	public CMatrix4[] maInverseBindMatrices;
	
	public MoModelMesh()
	{
		maInverseBindMatrices = null;
		mVertexList = new LinkedList<MoModelVertex>();
		mIndexList = new LinkedList<Integer>();
		mvMax = new CVector3(-Float.MAX_VALUE, -Float.MAX_VALUE, -Float.MAX_VALUE);
		mvMin = new CVector3(Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
		mstrTextureName = new String();
		mMaterial = new MoModelMaterial();
		mstrName = new String();
		mSkeleton = new MoModelSkeleton();
	}
}
