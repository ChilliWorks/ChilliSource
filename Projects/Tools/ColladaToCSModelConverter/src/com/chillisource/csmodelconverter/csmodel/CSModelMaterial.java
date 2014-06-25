//
// MoModelMaterial.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter.csmodel;

import com.taggames.toolutils.CVector4;

public class CSModelMaterial 
{
	public CVector4 mvEmissive;
	public CVector4 mvAmbient;
	public CVector4 mvDiffuse;
	public CVector4 mvSpecular;
	public float mfShininess;
	public String mstrName;
	
	public CSModelMaterial()
	{
		mvEmissive = new CVector4();
		mvAmbient = new CVector4();
		mvDiffuse = new CVector4();
		mvSpecular = new CVector4();
		mfShininess = 0.0f;
		mstrName = new String();
	}
}
