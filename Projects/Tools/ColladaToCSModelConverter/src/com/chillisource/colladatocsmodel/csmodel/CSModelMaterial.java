//
// MoModelMaterial.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.colladatocsmodel.csmodel;

import com.chillisource.toolutils.Vector4;

public class CSModelMaterial 
{
	public Vector4 mvEmissive;
	public Vector4 mvAmbient;
	public Vector4 mvDiffuse;
	public Vector4 mvSpecular;
	public float mfShininess;
	public String mstrName;
	
	public CSModelMaterial()
	{
		mvEmissive = new Vector4();
		mvAmbient = new Vector4();
		mvDiffuse = new Vector4();
		mvSpecular = new Vector4();
		mfShininess = 0.0f;
		mstrName = new String();
	}
}
