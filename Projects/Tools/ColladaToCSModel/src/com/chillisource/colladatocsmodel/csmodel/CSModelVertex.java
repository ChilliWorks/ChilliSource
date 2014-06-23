//
// MoModelVertex.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.colladatocsmodel.csmodel;

import com.chillisource.toolutils.Integer4;
import com.chillisource.toolutils.Vector2;
import com.chillisource.toolutils.Vector3;
import com.chillisource.toolutils.Vector4;


public class CSModelVertex 
{
	public Vector3 mvPosition;
	public Vector3 mvNormal;
	public Vector2 mvTextureCoordinate;
	public Vector4 mvVertexColour;
	public Vector4 mvWeights;
	public Integer4 mvJointIndices;
	
	public CSModelVertex()
	{
		mvPosition = new Vector3();
		mvNormal = new Vector3();
		mvTextureCoordinate = new Vector2();
		mvVertexColour = new Vector4();
		mvWeights = new Vector4();
		mvJointIndices = new Integer4();
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
