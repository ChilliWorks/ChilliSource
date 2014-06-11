//
// MoModelVertex.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter.csmodel;

import com.taggames.toolutils.CIntVector4;
import com.taggames.toolutils.CVector2;
import com.taggames.toolutils.CVector3;
import com.taggames.toolutils.CVector4;


public class CSModelVertex 
{
	public CVector3 mvPosition;
	public CVector3 mvNormal;
	public CVector2 mvTextureCoordinate;
	public CVector4 mvVertexColour;
	public CVector4 mvWeights;
	public CIntVector4 mvJointIndices;
	
	public CSModelVertex()
	{
		mvPosition = new CVector3();
		mvNormal = new CVector3();
		mvTextureCoordinate = new CVector2();
		mvVertexColour = new CVector4();
		mvWeights = new CVector4();
		mvJointIndices = new CIntVector4();
	}

	public boolean equals(Object inObj)
	{
	    if (this == inObj)
	    {
	    	return true;
	    }
	    else if ((inObj instanceof CSModelVertex) == false)
	    {
	    	return false;
	    }
	    
	    CSModelVertex vertex = (CSModelVertex)inObj;
	    if (mvPosition.equals(vertex.mvPosition) && mvNormal.equals(vertex.mvNormal) && mvTextureCoordinate.equals(vertex.mvTextureCoordinate) &&
			mvVertexColour.equals(vertex.mvVertexColour) && mvWeights.equals(vertex.mvWeights)&& mvJointIndices.equals(vertex.mvJointIndices))
	    {
	    	return true;
	    }
	    return false;
	}
}
