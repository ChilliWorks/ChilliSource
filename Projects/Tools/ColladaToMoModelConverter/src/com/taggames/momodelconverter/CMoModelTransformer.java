//
// CMoModelModifier.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.momodelconverter;

import java.util.LinkedList;

import com.taggames.momodelconverter.momodel.MoModel;
import com.taggames.momodelconverter.momodel.MoModelMesh;
import com.taggames.momodelconverter.momodel.MoModelVertex;
import com.taggames.toolutils.CMatrix4;

public class CMoModelTransformer 
{
	//-------------------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------------------
	public CMoModelTransformer()
	{
	}
	//-------------------------------------------------------------------
	/// Modify
	///
	/// This modifies the data in the MoModel to the desired output. For 
	/// example, changing from Z as up to Y as up.
	//-------------------------------------------------------------------
	public void Modify(MoModelConversionParameters inConversionParams, MoModel inMoModel)
	{
		boolean bReorderWinding = true;
		
		if (inConversionParams.mbMirrorInXZPlane == true)
		{
			MirrorInXZPlane(inMoModel);
			bReorderWinding = !bReorderWinding;
		}
		
		if (inConversionParams.mbSwapYAndZ == true)
		{
			SwapYAndZ(inMoModel);
			bReorderWinding = !bReorderWinding;
		}
		
		if (inConversionParams.mbFlipVerticalTexCoords == true)
		{
			FlipVerticalTexCoords(inMoModel);
		}
		
		if (bReorderWinding == true)
		{
			//each mesh bounds, positions and normals
			for (MoModelMesh mesh: inMoModel.mMeshTable.values())
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
	}
	//-------------------------------------------------------------------
	/// Mirror In XZ Plane
	///
	/// Mirrors all the data in the model in the XZ plane.
	//-------------------------------------------------------------------
	private void MirrorInXZPlane(MoModel inMoModel)
	{
		//overall bounds
		float temp = inMoModel.mvMin.y;
		inMoModel.mvMin.y = -inMoModel.mvMax.y;
		inMoModel.mvMax.y = -temp;
		
		//each mesh bounds, positions and normals
		for (MoModelMesh mesh: inMoModel.mMeshTable.values())
		{
			//bounds
			temp = mesh.mvMin.y;
			mesh.mvMin.y = -mesh.mvMax.y;
			mesh.mvMax.y = -temp;
			
			//position and normal
			for (MoModelVertex vert: mesh.mVertexList)
			{
				vert.mvPosition.y = -vert.mvPosition.y;
				vert.mvNormal.y = -vert.mvNormal.y;
			}
			
			//Mirror inverse bind matrices
			if (mesh.maInverseBindMatrices != null)
			{
				for (int i = 0; i < mesh.maInverseBindMatrices.length; i++)
				{
					CMatrix4 invertY = new CMatrix4(
							1.0f, 0.0f, 0.0f, 0.0f,
							0.0f,-1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);
					mesh.maInverseBindMatrices[i] = CMatrix4.Multiply(CMatrix4.Multiply(invertY.Inverse(), mesh.maInverseBindMatrices[i]), invertY);
				}
			}
		}
	}
	//-------------------------------------------------------------------
	/// Swap Y And Z
	///
	/// Swaps y and z for all data in the model. This is needed to
	/// convert from 3DS Max space to moFlow space.
	//-------------------------------------------------------------------
	private void SwapYAndZ(MoModel inMoModel)
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
		for (MoModelMesh mesh: inMoModel.mMeshTable.values())
		{
			//bounds
			temp = mesh.mvMin.z;
			mesh.mvMin.z = mesh.mvMin.y;
			mesh.mvMin.y = temp;
			
			temp = mesh.mvMax.z;
			mesh.mvMax.z = mesh.mvMax.y;
			mesh.mvMax.y = temp;
			
			//position and normal
			for (MoModelVertex vert: mesh.mVertexList)
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
					mesh.maInverseBindMatrices[i] = mesh.maInverseBindMatrices[i].SwapYandZ();
				}
			}
		}
	}
	//-------------------------------------------------------------------
	/// Flip Vertical Tex Coords
	///
	/// Inverts the y component of the texture coordinates. This is
	/// because 3DS Max uses the opposite coordinate system to what
	/// moFlow uses.
	//-------------------------------------------------------------------
	private void FlipVerticalTexCoords(MoModel inMoModel)
	{
		for (MoModelMesh mesh: inMoModel.mMeshTable.values())
		{
			for (MoModelVertex vert: mesh.mVertexList)
			{
				vert.mvTextureCoordinate.y = 1.0f - vert.mvTextureCoordinate.y;
			}
		}
	}
}
