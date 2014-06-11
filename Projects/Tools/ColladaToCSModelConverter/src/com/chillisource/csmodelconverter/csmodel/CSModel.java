//
// MoModel.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter.csmodel;

import java.util.Hashtable;

import com.taggames.toolutils.CVector3;

public class CSModel
{
	/**
	 * Private data
	 */
	public Hashtable<String, CSModelMesh> mMeshTable;
	public CVector3 mvMax;
	public CVector3 mvMin;
	public CSModelSkeleton mSkeleton;
	
	/**
	 * Constructor
	 */
	public CSModel()
	{
		mMeshTable = new Hashtable<String, CSModelMesh>();
		mvMax = new CVector3(-Float.MAX_VALUE, -Float.MAX_VALUE, -Float.MAX_VALUE);
		mvMin = new CVector3(Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
		mSkeleton = new CSModelSkeleton();
	}
}
