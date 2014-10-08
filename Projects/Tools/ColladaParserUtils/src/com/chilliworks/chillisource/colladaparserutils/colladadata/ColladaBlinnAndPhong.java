/**
 * ColladaBlinnAndPhong.java
 * Chilli Source
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
