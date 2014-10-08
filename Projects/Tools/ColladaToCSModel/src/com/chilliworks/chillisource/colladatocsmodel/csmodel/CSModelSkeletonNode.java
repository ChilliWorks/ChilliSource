/**
 * CSModelJoint.h
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

public class CSModelSkeletonNode
{
	enum SKELETON_NODE_TYPE
	{
		STANDARD,
		JOINT
	}
	public String mstrId;
	public String mstrParentId;
	public String mstrName;
	SKELETON_NODE_TYPE meType;
	public int mdwParentNodeIndex;

	//Joint specific stuff
	public boolean mbIsJoint;
	public String mstrSId;
	public int mdwJointIndex; 
	
	public CSModelSkeletonNode()
	{
		mstrId = new String();
		mstrParentId = new String();
		mstrName = new String();
		mdwParentNodeIndex = -1;
		
		mbIsJoint = false;
		mstrSId = new String();
		mdwJointIndex = -1;
	}
}
