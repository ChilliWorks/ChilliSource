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
import com.chilliworks.chillisource.coreutils.Matrix4;
import com.chilliworks.chillisource.coreutils.Vector2;
import com.chilliworks.chillisource.coreutils.Vector3;

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
		in_model.mvMin = new Vector3(in_model.mvMin.getX(), -in_model.mvMax.getY(), in_model.mvMin.getZ());
		in_model.mvMax = new Vector3(in_model.mvMax.getX(), -in_model.mvMin.getY(), in_model.mvMax.getZ());
		
		//each mesh bounds, positions and normals
		for (CSModelMesh mesh: in_model.mMeshTable.values())
		{
			//bounds
			mesh.mvMin = new Vector3(mesh.mvMin.getX(), -mesh.mvMax.getY(), mesh.mvMin.getZ());
			mesh.mvMax = new Vector3(mesh.mvMax.getX(), -mesh.mvMin.getY(), mesh.mvMax.getZ());
			
			//position and normal
			for (CSModelVertex vert: mesh.mVertexList)
			{
				vert.mvPosition = new Vector3(vert.mvPosition.getX(), -vert.mvPosition.getY(), vert.mvPosition.getZ());
				vert.mvNormal = new Vector3(vert.mvNormal.getX(), -vert.mvNormal.getY(), vert.mvNormal.getZ());
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
					mesh.maInverseBindMatrices[i] = Matrix4.multiply(Matrix4.multiply(Matrix4.inverse(invertY), mesh.maInverseBindMatrices[i]), invertY);
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
		//overall bounds
		in_model.mvMin = new Vector3(in_model.mvMin.getX(), in_model.mvMin.getZ(), in_model.mvMin.getY());
		in_model.mvMax = new Vector3(in_model.mvMax.getX(), in_model.mvMax.getZ(), in_model.mvMax.getY());
		
		//each mesh bounds, positions and normals
		for (CSModelMesh mesh: in_model.mMeshTable.values())
		{
			//bounds
			mesh.mvMin = new Vector3(mesh.mvMin.getX(), mesh.mvMin.getZ(), mesh.mvMin.getY());
			mesh.mvMax = new Vector3(mesh.mvMax.getX(), mesh.mvMax.getZ(), mesh.mvMax.getY());
			
			//position and normal
			for (CSModelVertex vert: mesh.mVertexList)
			{
				vert.mvPosition = new Vector3(vert.mvPosition.getX(), vert.mvPosition.getZ(), vert.mvPosition.getY());
				vert.mvNormal = new Vector3(vert.mvNormal.getX(), vert.mvNormal.getZ(), vert.mvNormal.getY());
			}
			
			//convert inverse bind matrices
			if (mesh.maInverseBindMatrices != null)
			{
				for (int i = 0; i < mesh.maInverseBindMatrices.length; i++)
				{	
					mesh.maInverseBindMatrices[i] = Matrix4.swapYandZ(mesh.maInverseBindMatrices[i]);
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
				vert.mvTextureCoordinate = new Vector2(vert.mvTextureCoordinate.getX(), 1.0f - vert.mvTextureCoordinate.getY());
			}
		}
	}
}
