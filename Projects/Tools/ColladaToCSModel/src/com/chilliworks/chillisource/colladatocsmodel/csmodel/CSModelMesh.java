/**
 * CSModelMesh.h
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

package com.chilliworks.chillisource.colladatocsmodel.csmodel;

import java.util.LinkedList;

import com.chilliworks.chillisource.coreutils.Matrix4;
import com.chilliworks.chillisource.coreutils.Vector3;

public class CSModelMesh 
{
	public LinkedList<CSModelVertex> mVertexList;
	public LinkedList<Integer> mIndexList;
	public String mstrTextureName;
	public CSModelMaterial mMaterial;
	public Vector3 mvMax;
	public Vector3 mvMin;
	public String mstrName;
	public CSModelSkeleton mSkeleton;
	public Matrix4[] maInverseBindMatrices;
	
	public CSModelMesh()
	{
		maInverseBindMatrices = null;
		mVertexList = new LinkedList<CSModelVertex>();
		mIndexList = new LinkedList<Integer>();
		mvMax = new Vector3(-Float.MAX_VALUE, -Float.MAX_VALUE, -Float.MAX_VALUE);
		mvMin = new Vector3(Float.MAX_VALUE, Float.MAX_VALUE, Float.MAX_VALUE);
		mstrTextureName = new String();
		mMaterial = new CSModelMaterial();
		mstrName = new String();
		mSkeleton = new CSModelSkeleton();
	}
}
