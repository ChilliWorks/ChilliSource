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

package com.chilliworks.chillisource.colladatocsmodel;

import java.util.LinkedList;

import com.chilliworks.chillisource.colladatocsmodel.csmodel.CSModel;
import com.chilliworks.chillisource.colladatocsmodel.csmodel.CSModelMesh;
import com.chilliworks.chillisource.colladatocsmodel.csmodel.CSModelVertex;
import com.chilliworks.chillisource.toolutils.Matrix4;

public final class CSModelTransformer 
{
	/**
	 * Applies all transforms specified in the input options.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options.
	 * @param in_model - The model.
	 */
	public static void transform(ColladaToCSModelOptions in_options, CSModel in_model)
	{
		if (in_options.m_swapHandedness == true)
		{
			swapHandedness(in_model);
		}
		
		if (in_options.m_swapYAndZ == true)
		{
			swapYAndZ(in_model);
		}
		
		if (in_options.m_flipVerticalTexCoords == true)
		{
			flipVerticalTexCoords(in_model);
		}
	}
	/**
	 * Swaps the handedness of the output model data.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_model - The model.
	 */
	private static void swapHandedness(CSModel in_model)
	{
		//overall bounds
		float temp = in_model.mvMin.y;
		in_model.mvMin.y = -in_model.mvMax.y;
		in_model.mvMax.y = -temp;
		
		//each mesh bounds, positions and normals
		for (CSModelMesh mesh: in_model.mMeshTable.values())
		{
			//bounds
			temp = mesh.mvMin.y;
			mesh.mvMin.y = -mesh.mvMax.y;
			mesh.mvMax.y = -temp;
			
			//position and normal
			for (CSModelVertex vert: mesh.mVertexList)
			{
				vert.mvPosition.y = -vert.mvPosition.y;
				vert.mvNormal.y = -vert.mvNormal.y;
			}
			
			//Mirror inverse bind matrices
			if (mesh.maInverseBindMatrices != null)
			{
				for (int i = 0; i < mesh.maInverseBindMatrices.length; i++)
				{
					Matrix4 invertY = new Matrix4(
							1.0f, 0.0f, 0.0f, 0.0f,
							0.0f,-1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
					mesh.maInverseBindMatrices[i] = Matrix4.multiply(Matrix4.multiply(invertY.inverse(), mesh.maInverseBindMatrices[i]), invertY);
				}
			}
		}
		
		//each mesh bounds, positions and normals
		for (CSModelMesh mesh: in_model.mMeshTable.values())
		{
			//indices
			LinkedList<Integer> newIndices = new LinkedList<Integer>();
			for (int i = 0; i < mesh.mIndexList.size() / 3; i++)
			{
				newIndices.add(mesh.mIndexList.get(i * 3 + 0));
				newIndices.add(mesh.mIndexList.get(i * 3 + 2));
				newIndices.add(mesh.mIndexList.get(i * 3 + 1));
			}
			mesh.mIndexList = newIndices;
		}
	}
	/**
	 * Swaps y and z for all data in the model. This is needed to convert 
	 * from 3DS Max space to chilli source space.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_model - The model.
	 */
	private static void swapYAndZ(CSModel in_model)
	{
		float temp = 0.0f;
		
		//overall bounds
		temp = in_model.mvMin.z;
		in_model.mvMin.z = in_model.mvMin.y;
		in_model.mvMin.y = temp;
		
		temp = in_model.mvMax.z;
		in_model.mvMax.z = in_model.mvMax.y;
		in_model.mvMax.y = temp;
		
		
		//each mesh bounds, positions and normals
		for (CSModelMesh mesh: in_model.mMeshTable.values())
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
	/**
	 * Inverts the y component of the texture coordinates. This is because 3DS Max 
	 * uses the opposite coordinate system to what chilli source uses.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_model - The model.
	 */
	private static void flipVerticalTexCoords(CSModel in_model)
	{
		for (CSModelMesh mesh: in_model.mMeshTable.values())
		{
			for (CSModelVertex vert: mesh.mVertexList)
			{
				vert.mvTextureCoordinate.y = 1.0f - vert.mvTextureCoordinate.y;
			}
		}
	}
}
