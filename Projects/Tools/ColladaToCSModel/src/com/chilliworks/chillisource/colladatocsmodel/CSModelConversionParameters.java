/**
 * CSModelConversionParameters.java
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

package com.chilliworks.chillisource.colladatocsmodel;

public class CSModelConversionParameters 
{
	public String mstrInputFilepath;
	public String mstrOutputFilepath;
	public boolean mbHasAnimationData;
	public boolean mbVertexHasPosition;
	public boolean mbVertexHasTexCoords;
	public boolean mbVertexHasNormal;
	public boolean mbVertexHasColour;
	public boolean mbVertexHasWeights;
	public boolean mbVertexHasJointIndices;
	public boolean mbSwapYAndZ;
	public boolean mbFlipVerticalTexCoords;
	public boolean mbCombineMeshes;
	
	public CSModelConversionParameters()
	{
		mstrInputFilepath 		= new String();
		mstrOutputFilepath 		= new String();
		mbHasAnimationData 		= false;
		mbVertexHasPosition 	= false;
		mbVertexHasTexCoords 	= false;
		mbVertexHasNormal 		= false;
		mbVertexHasColour 		= false;
		mbVertexHasWeights 		= false;
		mbVertexHasJointIndices = false;
		mbSwapYAndZ 			= false;
		mbFlipVerticalTexCoords = false;
		mbCombineMeshes			= false;
	}
}
