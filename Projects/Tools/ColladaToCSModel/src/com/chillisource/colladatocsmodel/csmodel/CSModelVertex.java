/**
 * CSModelVertex.h
 * Chilli Source
 * Created by Ian Copland on 15/01/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
