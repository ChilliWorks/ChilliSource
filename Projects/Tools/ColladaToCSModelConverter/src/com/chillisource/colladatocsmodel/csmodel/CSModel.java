//
// MoModel.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.colladatocsmodel.csmodel;

import java.util.Hashtable;

import com.chillisource.toolutils.Vector3;

public class CSModel
{
	/**
	 * Private data
	 */
	public Hashtable<String, CSModelMesh> mMeshTable;
	public Vector3 mvMax;
	public Vector3 mvMin;
	public CSModelSkeleton mSkeleton;
	
	/**
	 * Constructor
	 */
	public CSModel()
	{
		mMeshTable = new Hashtable<String, CSModelMesh>();
		mvMax = new Vector3(-Float.MAX_VALUE, -Float.MAX_VALUE, -Float.MAX_VALUE);
		mvMin = new Vector3(Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
		mSkeleton = new CSModelSkeleton();
	}
}
