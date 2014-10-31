/**
 * ParamsChecker.java
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

import com.chilliworks.chillisource.coreutils.Logging;

public class ParamsChecker 
{
	//----------------------------------------------------
	/// Check Parameters
	///
	/// Checks the input parameters for anything weird
	/// and throws a warning if it finds anything.
	///
	/// @param the parameters.
	//----------------------------------------------------
	static public void CheckParameters(ColladaToCSModelOptions inParams)
	{
		if (inParams.m_inputFilePath.length() == 0 || inParams.m_outputFilePath.length() == 0)
		{
			Logging.logFatal("Must provide an input and output file path.");
		}
		
		//check for no positional data
		if (inParams.m_vertexHasPosition == false)
		{
			Logging.logWarning("The 'positions' vertex element is not enabled meaning the output model will have no vertex position data.");
		}
		
		//check for use of animation without the animation vertex elements
		if (inParams.m_animated == true && (inParams.m_vertexHasJointIndices == false || inParams.m_vertexHasWeights == false))
		{
			Logging.logWarning("The 'Animation Data' feature is being used without both the animation vertex elements('Joint Indices' and 'Weights') enabled.");
		}
		
		//check for use of the animation vertex elements without animation data
		if (inParams.m_animated == false && (inParams.m_vertexHasJointIndices == true || inParams.m_vertexHasWeights == true))
		{
			Logging.logWarning("Animation vertex elements(joint indices or weights) are being used without the 'Animation Data' feature enabled.");
		}
				
		//check for use of combine meshes with an animated model.
		if (inParams.m_animated == true && inParams.m_combineMeshes == true)
		{
			Logging.logWarning("Combine meshes is enabled for an animated mesh. This will break the animation data, so turning it off.");
			inParams.m_combineMeshes = false;
		}
	}
}
