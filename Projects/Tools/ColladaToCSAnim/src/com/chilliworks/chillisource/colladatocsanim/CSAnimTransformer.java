/**
 * CSAnimTransformer.java
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

package com.chilliworks.chillisource.colladatocsanim;

import com.chilliworks.chillisource.colladatocsanim.csanim.*;
import com.chilliworks.chillisource.toolutils.Quaternion;
import com.chilliworks.chillisource.toolutils.Vector3;

public class CSAnimTransformer 
{
	
	/**
	 * Constructor
	 */
	public CSAnimTransformer()
	{
		
	}
	
	/**
	 * This modifies the data in the MoModel to the desired output. for example, changing from Z as up to Y as up.
	 * @param inConversionParams
	 * @param inMoModel
	 */
	public void Modify(CSAnimConversionParameters inConversionParams, CSAnim inMoModel)
	{
		if (inConversionParams.mbSwapYAndZ == true)
		{
			SwapYAndZ(inMoModel);
		}
	}
	
	/**
	 * Switches the x and y in the transform, such that Y is now the up axis.
	 * @param inMoModel
	 */
	private void SwapYAndZ(CSAnim inAnim)
	{
		for (int i = 0; i < inAnim.mFrames.size(); i++)
		{
			CSAnimFrame frame = inAnim.mFrames.get(i);
			
			for (int j = 0; j < frame.mNodeTranslations.size(); j++)
			{
				Vector3 translation = frame.mNodeTranslations.get(j);
				Quaternion orientation = frame.mNodeOrienations.get(j);
				Vector3 scale = frame.mNodeScalings.get(j);
				
				float temp = translation.y;
				translation.y = translation.z;
				translation.z = temp;
				
				Vector3 vAxis = orientation.getAxis();
				float fAngle = orientation.getAngle();
				temp = vAxis.y;
				vAxis.y = vAxis.z;
				vAxis.z = temp;
				fAngle = -fAngle;
				Quaternion newOrientation = Quaternion.createFromAxisAngle(vAxis, fAngle);
				orientation.set(newOrientation);
				
				temp = scale.y;
				scale.y = scale.z;
				scale.z = temp;
			}
		}
	}
}
