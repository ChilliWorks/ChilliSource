/**
 * CSModelTransformer.java
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

package com.chillisource.colladatocsmodel;

import com.chillisource.colladatocsmodel.csmodel.CSModel;
import com.chillisource.colladatocsmodel.csmodel.CSModelMesh;
import com.chillisource.colladatocsmodel.csmodel.CSModelVertex;

public class CSModelTransformer 
{
	//-------------------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------------------
	public CSModelTransformer()
	{
	}
	//-------------------------------------------------------------------
	/// Modify
	///
	/// This modifies the data in the MoModel to the desired output. For 
	/// example, changing from Z as up to Y as up.
	//-------------------------------------------------------------------
	public void Modify(CSModelConversionParameters inConversionParams, CSModel inMoModel)
	{
		if (inConversionParams.mbSwapYAndZ == true)
		{
			SwapYAndZ(inMoModel);
		}
		
		if (inConversionParams.mbFlipVerticalTexCoords == true)
		{
			FlipVerticalTexCoords(inMoModel);
		}
	}
	//-------------------------------------------------------------------
	/// Swap Y And Z
	///
	/// Swaps y and z for all data in the model. This is needed to
	/// convert from 3DS Max space to chilli source space.
	//-------------------------------------------------------------------
	private void SwapYAndZ(CSModel inMoModel)
	{
		float temp = 0.0f;
		
		//overall bounds
		temp = inMoModel.mvMin.z;
		inMoModel.mvMin.z = inMoModel.mvMin.y;
		inMoModel.mvMin.y = temp;
		
		temp = inMoModel.mvMax.z;
		inMoModel.mvMax.z = inMoModel.mvMax.y;
		inMoModel.mvMax.y = temp;
		
		
		//each mesh bounds, positions and normals
		for (CSModelMesh mesh: inMoModel.mMeshTable.values())
		{
			//bounds
			temp = mesh.mvMin.z;
			mesh.mvMin.z = mesh.mvMin.y;
			mesh.mvMin.y = temp;
			
			temp = mesh.mvMax.z;
			mesh.mvMax.z = mesh.mvMax.y;
			mesh.mvMax.y = temp;
			
			//position and normal
			for (CSModelVertex vert: mesh.mVertexList)
			{
				temp = vert.mvPosition.z;
				vert.mvPosition.z = vert.mvPosition.y;
				vert.mvPosition.y = temp;
				
				temp = vert.mvNormal.z;
				vert.mvNormal.z = vert.mvNormal.y;
				vert.mvNormal.y = temp;
			}
			
			//convert inverse bind matrices
			if (mesh.maInverseBindMatrices != null)
			{
				for (int i = 0; i < mesh.maInverseBindMatrices.length; i++)
				{	
					mesh.maInverseBindMatrices[i] = mesh.maInverseBindMatrices[i].swapYandZ();
				}
			}
		}
	}
	//-------------------------------------------------------------------
	/// Flip Vertical Tex Coords
	///
	/// Inverts the y component of the texture coordinates. This is
	/// because 3DS Max uses the opposite coordinate system to what
	/// chilli source uses.
	//-------------------------------------------------------------------
	private void FlipVerticalTexCoords(CSModel inMoModel)
	{
		for (CSModelMesh mesh: inMoModel.mMeshTable.values())
		{
			for (CSModelVertex vert: mesh.mVertexList)
			{
				vert.mvTextureCoordinate.y = 1.0f - vert.mvTextureCoordinate.y;
			}
		}
	}
}
