//
// MoModel.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.momodelconverter.momodel;

import java.util.Hashtable;

import com.taggames.toolutils.CVector3;

public class MoModel
{
	/**
	 * Private data
	 */
	public Hashtable<String, MoModelMesh> mMeshTable;
	public CVector3 mvMax;
	public CVector3 mvMin;
	public MoModelSkeleton mSkeleton;
	
	/**
	 * Constructor
	 */
	public MoModel()
	{
		mMeshTable = new Hashtable<String, MoModelMesh>();
		mvMax = new CVector3(-Float.MAX_VALUE, -Float.MAX_VALUE, -Float.MAX_VALUE);
		mvMin = new CVector3(Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
		mSkeleton = new MoModelSkeleton();
	}
}
