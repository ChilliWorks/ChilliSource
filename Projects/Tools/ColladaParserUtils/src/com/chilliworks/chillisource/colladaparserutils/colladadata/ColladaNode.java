/**
 * ColladaNode.java
 * ChilliSource
 * Created by Ian Copland on 16/01/2013.
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

package com.chilliworks.chillisource.colladaparserutils.colladadata;

import java.util.Hashtable;

import com.chilliworks.chillisource.coreutils.Matrix4;

public class ColladaNode 
{
	public enum COLLADA_NODE_TYPE
	{
		BASE,
		JOINT,
		LIGHT,
		GEOMETRY,
		CONTROLLER,
		CAMERA
	}
	
	public Hashtable<String, ColladaNode> mChildNodes;
	public String mstrId;
	public String mstrName;
	public String mstrType;
	public String mstrSId;
	public COLLADA_NODE_TYPE meType;
	public ColladaInstanceLight mInstanceLight;
	public ColladaInstanceGeometry mInstanceGeometry;
	public ColladaInstanceController mInstanceController;
	public ColladaInstanceCamera mInstanceCamera;
	public ColladaExtra mExtra;
	public Matrix4 m_transform = Matrix4.IDENTITY;
	
	public ColladaNode()
	{
		mChildNodes = new Hashtable<String, ColladaNode>();
		mstrId = new String();
		mstrName = new String();
		meType = COLLADA_NODE_TYPE.BASE;
		mInstanceLight = new ColladaInstanceLight();
		mInstanceGeometry = new ColladaInstanceGeometry();
		mInstanceController = new ColladaInstanceController();
		mInstanceCamera = new ColladaInstanceCamera();
		mExtra = new ColladaExtra();
	}
}
