//
// CMoModelOutputer.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.momodelconverter;

import java.util.LinkedList;

import com.taggames.momodelconverter.momodel.*;
import com.taggames.toolutils.*;


public class CMoModelOutputer 
{
	//-------------------------------------------------------------------
	/// Constants
	//-------------------------------------------------------------------
	private static final int kdwEndiannessCheckValue = 6666;
	private static final int kdwVersionNumber = 11;
	private static final String kdwFileExtension = ".momodel";
	
	private static final int kdwFeatureHasTexture = 1;
	private static final int kdwFeatureHasMaterial = 2;
	private static final int kdwFeatureHasAnimationData = 3;
	
	private static final int kdwVertexPosition = 1;
	private static final int kdwVertexNormal = 2;
	private static final int kdwVertexTexCoord = 3;
	private static final int kdwVertexColour = 4;
	private static final int kdwVertexWeight = 5;
	private static final int kdwVertexJointIndex = 6;
	
	private static final int kdwSkeletonNodeTypeStandard = 0;
	private static final int kdwSkeletonNodeTypeJoint = 1;
	
	private static final int HIGHEST_UNSIGNED_SHORT = 65535;
	//-------------------------------------------------------------------
	/// Private Member Data
	//-------------------------------------------------------------------
	CLittleEndianOutputStream mStream;
	int mdwSizeOfIndices;
	//-------------------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------------------
	public CMoModelOutputer()
	{
	}
	//-------------------------------------------------------------------
	/// Output
	///
	/// Outputs the given MoModel data to file.
	//-------------------------------------------------------------------
	public boolean Output(MoModelConversionParameters inConversionParams, MoModel inMoModel) throws Exception
	{
		boolean bSuccess = true;
		
		CalculateSizeOfIndices(inMoModel);
		
		//try and open a new file stream. if this fails, throw a fatal error.
		try
		{
			String strOutputFilepath = inConversionParams.mstrOutputFilepath;
			if (inConversionParams.mstrOutputFilepath.endsWith(kdwFileExtension) == false)
				strOutputFilepath += kdwFileExtension;
				
			mStream = new CLittleEndianOutputStream(strOutputFilepath);
		}
		catch (Exception e)
		{
			mStream.Close();
			SCLogger.LogFatalError("Failed to open output file: " + inConversionParams.mstrOutputFilepath);
		}
		
		//output the file
		try
		{
			if (mStream != null)
			{
				bSuccess = WriteGlobalHeader(inConversionParams, inMoModel);
				if (bSuccess == true)
				{
					bSuccess = WriteSkeleton(inConversionParams, inMoModel);
					if (bSuccess == true)
					{
						for (MoModelMesh mesh: inMoModel.mMeshTable.values())
						{
							bSuccess = WriteMeshHeader(inConversionParams, mesh);
							if (bSuccess == false)
								break;
							
							bSuccess = WriteMeshBody(inConversionParams, mesh);
							if (bSuccess == false)
								break;
						}
					}
				}
			}
		}
		catch (Exception e)
		{
			mStream.Close();
			SCLogger.LogFatalError("Failed to write to file: " + inConversionParams.mstrOutputFilepath);
		}
		
		//close the filestream
		mStream.Close();
		
		//output the materials
		if (inConversionParams.mbHasMaterial == true)
		{
			CMoModelMaterialBuilder materialOutputer = new CMoModelMaterialBuilder();
			materialOutputer.OutputMaterialsForModel(inConversionParams, inMoModel);
		}
		
		return bSuccess;
	}
	//-------------------------------------------------------------------
	/// Calculate Size Of Indices
	///
	/// Calculates the size of an index. This will try and be of size 2 
	/// (unsigned short integer), but if its too big it will become 4.
	//-------------------------------------------------------------------
	private void CalculateSizeOfIndices(MoModel inMoModel)
	{
		mdwSizeOfIndices = 2;
		
		for (MoModelMesh mesh: inMoModel.mMeshTable.values())
		{
			if (mesh.mIndexList.size() >= HIGHEST_UNSIGNED_SHORT || mesh.mVertexList.size() >= HIGHEST_UNSIGNED_SHORT)
			{
				mdwSizeOfIndices = 4;
			}
		}
	}
	//-------------------------------------------------------------------
	/// Get Num Features
	///
	/// Calculates the number of features to be outputed from the 
	/// conversion parameters
	//-------------------------------------------------------------------
	private int GetNumFeatures(MoModelConversionParameters inConversionParams)
	{
		int dwNumFeatures = 0;
		
		if (inConversionParams.mbHasTexture == true) 
			dwNumFeatures++;
		if (inConversionParams.mbHasMaterial == true) 
			dwNumFeatures++;
		if (inConversionParams.mbHasAnimationData == true) 
			dwNumFeatures++;
		
		return dwNumFeatures;
	}
	//-------------------------------------------------------------------
	/// Get Num Vertex Elements
	///
	/// Calculates the number of vertex elements to be outputed from the 
	/// conversion parameters
	//-------------------------------------------------------------------
	private int GetNumVertexElements(MoModelConversionParameters inConversionParams)
	{
		int dwNumVertexElements = 0;
		
		if (inConversionParams.mbVertexHasPosition == true) 
			dwNumVertexElements++;
		if (inConversionParams.mbVertexHasNormal == true)
			dwNumVertexElements++;
		if (inConversionParams.mbVertexHasTexCoords == true)
			dwNumVertexElements++;
		if (inConversionParams.mbVertexHasColour == true) 
			dwNumVertexElements++;
		if (inConversionParams.mbVertexHasWeights == true) 
			dwNumVertexElements++;
		if (inConversionParams.mbVertexHasJointIndices == true) 
			dwNumVertexElements++;
		
		return dwNumVertexElements;
	}
	//-------------------------------------------------------------------
	/// Write Global Header
	///
	/// Writes the file format header.
	//-------------------------------------------------------------------
	private boolean WriteGlobalHeader(MoModelConversionParameters inConversionParams, MoModel inMoModel) throws Exception
	{
		//write the endianess check value and version number
		mStream.WriteUnsignedInt(kdwEndiannessCheckValue);
		mStream.WriteUnsignedInt(kdwVersionNumber);
		
		//output the feature declaration
		mStream.WriteByte((byte)GetNumFeatures(inConversionParams));
		if (inConversionParams.mbHasTexture == true)
			mStream.WriteByte((byte)kdwFeatureHasTexture);
		if (inConversionParams.mbHasMaterial == true) 
			mStream.WriteByte((byte)kdwFeatureHasMaterial);
		if (inConversionParams.mbHasAnimationData == true) 
			mStream.WriteByte((byte)kdwFeatureHasAnimationData);
		
		//output the Vertex Description
		mStream.WriteByte((byte)GetNumVertexElements(inConversionParams));
		if (inConversionParams.mbVertexHasPosition == true)
			mStream.WriteByte((byte)kdwVertexPosition);
		if (inConversionParams.mbVertexHasNormal == true)
			mStream.WriteByte((byte)kdwVertexNormal);
		if (inConversionParams.mbVertexHasTexCoords == true) 
			mStream.WriteByte((byte)kdwVertexTexCoord);
		if (inConversionParams.mbVertexHasColour == true) 
			mStream.WriteByte((byte)kdwVertexColour);
		if (inConversionParams.mbVertexHasWeights == true) 
			mStream.WriteByte((byte)kdwVertexWeight);
		if (inConversionParams.mbVertexHasJointIndices == true) 
			mStream.WriteByte((byte)kdwVertexJointIndex);
		
		//index declaration
		mStream.WriteByte((byte)mdwSizeOfIndices);
		
		//output the bounding box data
		mStream.WriteFloat(inMoModel.mvMin.x);
		mStream.WriteFloat(inMoModel.mvMin.y);
		mStream.WriteFloat(inMoModel.mvMin.z);
		mStream.WriteFloat(inMoModel.mvMax.x);
		mStream.WriteFloat(inMoModel.mvMax.y);
		mStream.WriteFloat(inMoModel.mvMax.z);
		
		//output meshes!
		mStream.WriteUnsignedShort(inMoModel.mMeshTable.size());
		
		if (inConversionParams.mbHasAnimationData == true)
		{
			mStream.WriteShort((short)inMoModel.mSkeleton.mNodeList.size());
			mStream.WriteByte((byte)SCSkeletonBuilder.GetNumberOfJoints(inMoModel));
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Skeleton
	///
	/// Writes the skeleton data to file.
	//-------------------------------------------------------------------
	private boolean WriteSkeleton(MoModelConversionParameters inConversionParams, MoModel inMoModel) throws Exception
	{
		if (inConversionParams.mbHasAnimationData == true)
		{
			for (int i = 0; i < inMoModel.mSkeleton.mNodeList.size(); i++)
			{
				MoModelSkeletonNode node = inMoModel.mSkeleton.mNodeList.get(i);
				
				//write joint name
				mStream.WriteNullTerminatedAsciiString(node.mstrName);
				
				//write parent index
				mStream.WriteShort((short)node.mdwParentNodeIndex);
				
				//write the note type
				if (node.mbIsJoint == false)
				{
					mStream.WriteByte((byte)kdwSkeletonNodeTypeStandard);
				}
				else
				{
					mStream.WriteByte((byte)kdwSkeletonNodeTypeJoint);
					
					//Also write the joint index.
					mStream.WriteByte((byte)node.mdwJointIndex);
				}
			}
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Mesh Header
	///
	/// Writes the mesh header data to file.
	//-------------------------------------------------------------------
	private boolean WriteMeshHeader(MoModelConversionParameters inConversionParams, MoModelMesh inMoModelMesh) throws Exception
	{
		//write the mesh name
		mStream.WriteNullTerminatedAsciiString(inMoModelMesh.mstrName);
		
		//write the number of verts and indices
		if (mdwSizeOfIndices == 2)
		{
			mStream.WriteUnsignedShort(inMoModelMesh.mVertexList.size());
			mStream.WriteUnsignedShort(inMoModelMesh.mIndexList.size() / 3);
		}
		else
		{
			mStream.WriteUnsignedInt((long)inMoModelMesh.mVertexList.size());
			mStream.WriteUnsignedInt((long)inMoModelMesh.mIndexList.size() / 3);
		}
		
		//write the bounds
		mStream.WriteFloat(inMoModelMesh.mvMin.x);
		mStream.WriteFloat(inMoModelMesh.mvMin.y);
		mStream.WriteFloat(inMoModelMesh.mvMin.z);
		mStream.WriteFloat(inMoModelMesh.mvMax.x);
		mStream.WriteFloat(inMoModelMesh.mvMax.y);
		mStream.WriteFloat(inMoModelMesh.mvMax.z);
		
		//if using a texture, output the texture name
		if (inConversionParams.mbHasTexture == true)
		{
			//if the texture exists, output the tex name
			if (inMoModelMesh.mstrTextureName.isEmpty() == false)
			{
				mStream.WriteNullTerminatedAsciiString(inMoModelMesh.mstrTextureName);
			}
			else
			{
				SCLogger.LogError("Format specifed as using a texture, but no texture found for mesh: " + inMoModelMesh.mstrName);
				return false;
			}
		}
		
		//write the material
		if (inConversionParams.mbHasMaterial == true)
		{
			//output material name
			String strOutputFullpath = inConversionParams.mstrOutputFilepath.replace("\\", "/");
			String[] strBrokenUpOut = strOutputFullpath.split("/");
			String outName;
			if (strBrokenUpOut.length > 0)
				outName = strBrokenUpOut[strBrokenUpOut.length - 1];
			else
				outName = strOutputFullpath;
			outName = outName.replace(".momodel", "");
			mStream.WriteNullTerminatedAsciiString(inMoModelMesh.mMaterial.mstrName + ".momaterial");

		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Mesh Body
	///
	/// Writes the mesh body data to file.
	//-------------------------------------------------------------------
	private boolean WriteMeshBody(MoModelConversionParameters inConversionParams, MoModelMesh inMoStaticMesh) throws Exception
	{
		boolean bSuccess = true;
		
		bSuccess = WriteInverseBindMatrices(inConversionParams, inMoStaticMesh);
		
		if (bSuccess == true)
			bSuccess = WriteVertices(inConversionParams, inMoStaticMesh);
		
		if (bSuccess == true)
			bSuccess = WriteIndices(inConversionParams, inMoStaticMesh);
		
		return bSuccess;
	}
	//-------------------------------------------------------------------
	/// Write Inverse Bind Matrices
	///
	/// Writes the inverse bind matrices if they need to be.
	//-------------------------------------------------------------------
	private boolean WriteInverseBindMatrices(MoModelConversionParameters inConversionParams, MoModelMesh inMoStaticMesh) throws Exception
	{
		if (inConversionParams.mbHasAnimationData == true)
		{
			if (inMoStaticMesh.maInverseBindMatrices != null)
			{
				for (int i = 0; i < inMoStaticMesh.maInverseBindMatrices.length; i++)
				{
					for (int j = 0; j < 16; j++)
					{
						mStream.WriteFloat(inMoStaticMesh.maInverseBindMatrices[i].mafData[j]);
					}
				}
			}
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Vertices
	///
	/// Writes the vertices to the output stream
	//-------------------------------------------------------------------
	private boolean WriteVertices(MoModelConversionParameters inConversionParams, MoModelMesh inMoStaticMesh) throws Exception
	{
		LinkedList<MoModelVertex> vertexList = inMoStaticMesh.mVertexList;
		
		for (int i = 0; i < vertexList.size(); i++)
		{
			MoModelVertex vertex = vertexList.get(i);
			
			//write the position data
			if (inConversionParams.mbVertexHasPosition == true)
			{
				mStream.WriteFloat(vertex.mvPosition.x);
				mStream.WriteFloat(vertex.mvPosition.y);
				mStream.WriteFloat(vertex.mvPosition.z);
				mStream.WriteFloat(1.0f);
			}
			
			
			//write the normal data if its in the format
			if (inConversionParams.mbVertexHasNormal == true)
			{
				mStream.WriteFloat(vertex.mvNormal.x);
				mStream.WriteFloat(vertex.mvNormal.y);
				mStream.WriteFloat(vertex.mvNormal.z);
			}
			
			//write the tex coord data if its in the format
			if (inConversionParams.mbVertexHasTexCoords == true)
			{
				mStream.WriteFloat(vertex.mvTextureCoordinate.x);
				mStream.WriteFloat(vertex.mvTextureCoordinate.y);
			}
			
			//write the colour data if its in the format
			if (inConversionParams.mbVertexHasColour == true)
			{
				mStream.WriteByte((byte)vertex.mvVertexColour.x);
				mStream.WriteByte((byte)vertex.mvVertexColour.y);
				mStream.WriteByte((byte)vertex.mvVertexColour.z);
				mStream.WriteByte((byte)vertex.mvVertexColour.w);
			}
			
			//write the vertex weight if its in the format
			if (inConversionParams.mbVertexHasWeights == true)
			{
				mStream.WriteFloat(vertex.mvWeights.x);
				mStream.WriteFloat(vertex.mvWeights.y);
				mStream.WriteFloat(vertex.mvWeights.z);
				mStream.WriteFloat(vertex.mvWeights.w);
			}
			
			//write the joint indices if its in the format
			if (inConversionParams.mbVertexHasJointIndices == true)
			{
				mStream.WriteByte((byte)vertex.mvJointIndices.x);
				mStream.WriteByte((byte)vertex.mvJointIndices.y);
				mStream.WriteByte((byte)vertex.mvJointIndices.z);
				mStream.WriteByte((byte)vertex.mvJointIndices.w);
			}
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Indices
	///
	/// Writes the indices to the output stream
	//-------------------------------------------------------------------
	private boolean WriteIndices(MoModelConversionParameters inConversionParams, MoModelMesh inMoStaticMesh) throws Exception
	{
		if (inMoStaticMesh.mIndexList.size() > 0)
		{
			LinkedList<Integer> indexList = inMoStaticMesh.mIndexList;
			
			if (mdwSizeOfIndices == 2)
			{
				for (int i = 0; i < indexList.size() / 3; i++)
				{
					mStream.WriteUnsignedShort(indexList.get(i * 3 + 0));
					mStream.WriteUnsignedShort(indexList.get(i * 3 + 1));
					mStream.WriteUnsignedShort(indexList.get(i * 3 + 2));
				}
			}
			else
			{
				for (int i = 0; i < indexList.size() / 3; i++)
				{
					mStream.WriteUnsignedInt((long)indexList.get(i * 3 + 0));
					mStream.WriteUnsignedInt((long)indexList.get(i * 3 + 1));
					mStream.WriteUnsignedInt((long)indexList.get(i * 3 + 2));
				}
			}
		}
		return true;
	}
}
