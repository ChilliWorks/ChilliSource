/**
 * ColladaSkin.java
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

public class ColladaSkin 
{
	public String mstrSource;
	public ColladaBindShapeMatrix mBindShapeMatrix;
	public Hashtable<String, ColladaSource> mSourceTable;
	public ColladaJoints mJoints;
	public ColladaVertexWeights mVertexWeights;
	
	public ColladaSkin()
	{
		mstrSource = new String();
		mBindShapeMatrix = new ColladaBindShapeMatrix();
		mSourceTable = new Hashtable<String, ColladaSource>();
		mJoints = new ColladaJoints();
		mVertexWeights = new ColladaVertexWeights();
	}
}
