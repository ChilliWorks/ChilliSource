/**
 * ModelChecker.java
 * Chilli Source
 * Created by Ian Copland on 18/04/2013.
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

import com.chilliworks.chillisource.colladatocsmodel.csmodel.CSModel;
import com.chilliworks.chillisource.colladatocsmodel.csmodel.CSModelMesh;
import com.chilliworks.chillisource.coreutils.Logging;

public class ModelChecker 
{
	static final int kdwMaxUnsignedShort = 65536;
	//----------------------------------------------------
	/// Check Model
	///
	/// Checks whether the model will be able to be
	/// loaded into chilli source. This checks things like
	/// triangle count and number of mesh influencing joints.
	///
	/// @param The model.
	//----------------------------------------------------
	static public void CheckModel(CSModel inModel)
	{
		//Test that the model is not too big for chilli source to render.
		boolean bContainsLargeMesh = false;
		for (CSModelMesh mesh : inModel.mMeshTable.values())
		{
			boolean bFoundLargeIndex = false;
			for (Integer dwIndex : mesh.mIndexList)
			{
				if (dwIndex > kdwMaxUnsignedShort)
				{
					bFoundLargeIndex = true;
				}
			}
			
			if (bFoundLargeIndex == true || mesh.mIndexList.size() > kdwMaxUnsignedShort)
			{
				bContainsLargeMesh = true;
				break;
			}
		}
		if (bContainsLargeMesh == true)
		{
			Logging.logWarning("Mesh is too large! This model will require integer indices to render which chilli source currently does not support!");
		}
		
		//test the amount of skeleton nodes
		if (inModel.mSkeleton.mNodeList.size() > 256)
		{
			Logging.logWarning("The skeleton contains a huge number of nodes! Over 256 nodes is probably too many.");
		}
		
		//test the amount of joints
		if (SkeletonBuilder.GetNumberOfJoints(inModel) > 60)
		{
			Logging.logWarning("The skeleton contains a huge number of joints! Over 60 nodes is probably too many.");
		}
	}
}
