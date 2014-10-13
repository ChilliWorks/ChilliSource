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

/**
 * Performs transformations on the output model data, such as coordinate system
 * conversions.
 * 
 * @author Ian Copland
 *
 */
public final class CSAnimTransformer 
{
	/**
	 * This applies all transformations requested in the input parameters, such as
	 * swapping the handedness of the output coordinate system.
	 *
	 * @author Ian Copland
	 * 
	 * @param in_conversionParams - The tools input parameters.
	 * @param in_anim - The animation to apply transformations to.
	 */
	public static void transform(ColladaToCSAnimOptions in_conversionParams, CSAnim in_anim)
	{
		if (in_conversionParams.m_swapHandedness == true)
		{
			swapHandedness(in_anim);
		}
		
		if (in_conversionParams.m_swapYAndZ == true)
		{
			swapYAndZ(in_anim);
		}
	}
	/**
	 * Swaps the X and Y component of all data in the output animation. This allows conversion
	 * between different coordinate systems.
	 * 
	 * @author Ian Copland
	 * 
	 * @param inMoModel
	 */
	private static void swapHandedness(CSAnim in_anim)
	{
		for (int i = 0; i < in_anim.mFrames.size(); i++)
		{
			CSAnimFrame frame = in_anim.mFrames.get(i);
			
			for (int j = 0; j < frame.mNodeTranslations.size(); j++)
			{
				Vector3 translation = frame.mNodeTranslations.get(j);
				Quaternion orientation = frame.mNodeOrienations.get(j);
				
				translation.y = -translation.y;
				
				orientation.y = -orientation.y;
				orientation.w = -orientation.w;
			}
		}
	}
	/**
	 * Swaps the X and Y component of all data in the output animation. This allows conversion
	 * between different coordinate systems.
	 * 
	 * @author Ian Copland
	 * 
	 * @param inMoModel
	 */
	private static void swapYAndZ(CSAnim in_anim)
	{
		for (int i = 0; i < in_anim.mFrames.size(); i++)
		{
			CSAnimFrame frame = in_anim.mFrames.get(i);
			
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
