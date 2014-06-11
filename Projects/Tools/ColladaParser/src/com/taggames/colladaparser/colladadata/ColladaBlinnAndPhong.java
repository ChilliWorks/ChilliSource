//
// ColladaBlinnAndPhong.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

public class ColladaBlinnAndPhong 
{
	public static final ColladaColour DEFAULT_EMISSIVE = new ColladaColour(0.0f, 0.0f, 0.0f, 1.0f);
	public static final ColladaColour DEFAULT_DIFFUSE = new ColladaColour(1.0f, 1.0f, 1.0f, 1.0f);
	
	public ColladaMaterialColour mEmissive;
	public ColladaMaterialColour mAmbient;
	public ColladaMaterialColour mDiffuse;
	public ColladaMaterialColour mSpecular;
	public ColladaShininess mShininess;
	//Note: There should be more data here, however it has not been included becuase its unlikely to be needed any time soon.
	
	public ColladaBlinnAndPhong()
	{
		mEmissive = new ColladaMaterialColour();
		mAmbient = new ColladaMaterialColour();
		mDiffuse = new ColladaMaterialColour();
		mSpecular = new ColladaMaterialColour();
		mShininess = new ColladaShininess();
		
		//Default the diffuse
		mEmissive.mColour = DEFAULT_EMISSIVE;
		mDiffuse.mColour = DEFAULT_DIFFUSE;
	}
}
