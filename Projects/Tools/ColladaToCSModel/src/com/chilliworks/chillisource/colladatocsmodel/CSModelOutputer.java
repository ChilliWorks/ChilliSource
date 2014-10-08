/**
 * CSModelOutputer.java
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

import com.chilliworks.chillisource.colladatocsmodel.csmodel.*;
import com.chilliworks.chillisource.toolutils.*;


public class CSModelOutputer 
{
	//-------------------------------------------------------------------
	/// Constants
	//-------------------------------------------------------------------
	private static final int kdwEndiannessCheckValue = 6666;
	private static final int kdwVersionNumber = 12;

	private static final int kdwFeatureHasAnimationData = 1;
	
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
	LittleEndianOutputStream mStream;
	int mdwSizeOfIndices;
	//-------------------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------------------
	public CSModelOutputer()
	{
	}
	//-------------------------------------------------------------------
	/// Output
	///
	/// Outputs the given MoModel data to file.
	//-------------------------------------------------------------------
	public boolean Output(CSModelConversionParameters inConversionParams, CSModel inMoModel) throws Exception
	{
		boolean bSuccess = true;
		
		CalculateSizeOfIndices(inMoModel);
		
		//try and open a new file stream. if this fails, throw a fatal error.
		try
		{
			mStream = new LittleEndianOutputStream(inConversionParams.mstrOutputFilepath);
		}
		catch (Exception e)
		{
			mStream.close();
			Logging.logFatal("Failed to open output file: " + inConversionParams.mstrOutputFilepath);
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
						for (CSModelMesh mesh: inMoModel.mMeshTable.values())
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
			mStream.close();
			Logging.logFatal("Failed to write to file: " + inConversionParams.mstrOutputFilepath);
		}
		
		//close the filestream
		mStream.close();
		
		return bSuccess;
	}
	//-------------------------------------------------------------------
	/// Calculate Size Of Indices
	///
	/// Calculates the size of an index. This will try and be of size 2 
	/// (unsigned short integer), but if its too big it will become 4.
	//-------------------------------------------------------------------
	private void CalculateSizeOfIndices(CSModel inMoModel)
	{
		mdwSizeOfIndices = 2;
		
		for (CSModelMesh mesh: inMoModel.mMeshTable.values())
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
	private int GetNumFeatures(CSModelConversionParameters inConversionParams)
	{
		int dwNumFeatures = 0;
		
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
	private int GetNumVertexElements(CSModelConversionParameters inConversionParams)
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
	private boolean WriteGlobalHeader(CSModelConversionParameters inConversionParams, CSModel inMoModel) throws Exception
	{
		//write the endianess check value and version number
		mStream.writeUnsignedInt(kdwEndiannessCheckValue);
		mStream.writeUnsignedInt(kdwVersionNumber);
		
		//output the feature declaration
		mStream.writeByte((byte)GetNumFeatures(inConversionParams));
		if (inConversionParams.mbHasAnimationData == true) 
			mStream.writeByte((byte)kdwFeatureHasAnimationData);
		
		//output the Vertex Description
		mStream.writeByte((byte)GetNumVertexElements(inConversionParams));
		if (inConversionParams.mbVertexHasPosition == true)
			mStream.writeByte((byte)kdwVertexPosition);
		if (inConversionParams.mbVertexHasNormal == true)
			mStream.writeByte((byte)kdwVertexNormal);
		if (inConversionParams.mbVertexHasTexCoords == true) 
			mStream.writeByte((byte)kdwVertexTexCoord);
		if (inConversionParams.mbVertexHasColour == true) 
			mStream.writeByte((byte)kdwVertexColour);
		if (inConversionParams.mbVertexHasWeights == true) 
			mStream.writeByte((byte)kdwVertexWeight);
		if (inConversionParams.mbVertexHasJointIndices == true) 
			mStream.writeByte((byte)kdwVertexJointIndex);
		
		//index declaration
		mStream.writeByte((byte)mdwSizeOfIndices);
		
		//output the bounding box data
		mStream.writeFloat(inMoModel.mvMin.x);
		mStream.writeFloat(inMoModel.mvMin.y);
		mStream.writeFloat(inMoModel.mvMin.z);
		mStream.writeFloat(inMoModel.mvMax.x);
		mStream.writeFloat(inMoModel.mvMax.y);
		mStream.writeFloat(inMoModel.mvMax.z);
		
		//output meshes!
		mStream.writeUnsignedShort(inMoModel.mMeshTable.size());
		
		if (inConversionParams.mbHasAnimationData == true)
		{
			mStream.writeShort((short)inMoModel.mSkeleton.mNodeList.size());
			mStream.writeByte((byte)SkeletonBuilder.GetNumberOfJoints(inMoModel));
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Skeleton
	///
	/// Writes the skeleton data to file.
	//-------------------------------------------------------------------
	private boolean WriteSkeleton(CSModelConversionParameters inConversionParams, CSModel inMoModel) throws Exception
	{
		if (inConversionParams.mbHasAnimationData == true)
		{
			for (int i = 0; i < inMoModel.mSkeleton.mNodeList.size(); i++)
			{
				CSModelSkeletonNode node = inMoModel.mSkeleton.mNodeList.get(i);
				
				//write joint name
				mStream.writeNullTerminatedAsciiString(node.mstrName);
				
				//write parent index
				mStream.writeShort((short)node.mdwParentNodeIndex);
				
				//write the note type
				if (node.mbIsJoint == false)
				{
					mStream.writeByte((byte)kdwSkeletonNodeTypeStandard);
				}
				else
				{
					mStream.writeByte((byte)kdwSkeletonNodeTypeJoint);
					
					//Also write the joint index.
					mStream.writeByte((byte)node.mdwJointIndex);
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
	private boolean WriteMeshHeader(CSModelConversionParameters inConversionParams, CSModelMesh inMoModelMesh) throws Exception
	{
		//write the mesh name
		mStream.writeNullTerminatedAsciiString(inMoModelMesh.mstrName);
		
		//write the number of verts and indices
		if (mdwSizeOfIndices == 2)
		{
			mStream.writeUnsignedShort(inMoModelMesh.mVertexList.size());
			mStream.writeUnsignedShort(inMoModelMesh.mIndexList.size() / 3);
		}
		else
		{
			mStream.writeUnsignedInt((long)inMoModelMesh.mVertexList.size());
			mStream.writeUnsignedInt((long)inMoModelMesh.mIndexList.size() / 3);
		}
		
		//write the bounds
		mStream.writeFloat(inMoModelMesh.mvMin.x);
		mStream.writeFloat(inMoModelMesh.mvMin.y);
		mStream.writeFloat(inMoModelMesh.mvMin.z);
		mStream.writeFloat(inMoModelMesh.mvMax.x);
		mStream.writeFloat(inMoModelMesh.mvMax.y);
		mStream.writeFloat(inMoModelMesh.mvMax.z);
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Mesh Body
	///
	/// Writes the mesh body data to file.
	//-------------------------------------------------------------------
	private boolean WriteMeshBody(CSModelConversionParameters inConversionParams, CSModelMesh inMoStaticMesh) throws Exception
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
	private boolean WriteInverseBindMatrices(CSModelConversionParameters inConversionParams, CSModelMesh inMoStaticMesh) throws Exception
	{
		if (inConversionParams.mbHasAnimationData == true)
		{
			if (inMoStaticMesh.maInverseBindMatrices != null)
			{
				for (int i = 0; i < inMoStaticMesh.maInverseBindMatrices.length; i++)
				{
					for (int j = 0; j < 16; j++)
					{
						mStream.writeFloat(inMoStaticMesh.maInverseBindMatrices[i].mafData[j]);
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
	private boolean WriteVertices(CSModelConversionParameters inConversionParams, CSModelMesh inMoStaticMesh) throws Exception
	{
		LinkedList<CSModelVertex> vertexList = inMoStaticMesh.mVertexList;
		
		for (int i = 0; i < vertexList.size(); i++)
		{
			CSModelVertex vertex = vertexList.get(i);
			
			//write the position data
			if (inConversionParams.mbVertexHasPosition == true)
			{
				mStream.writeFloat(vertex.mvPosition.x);
				mStream.writeFloat(vertex.mvPosition.y);
				mStream.writeFloat(vertex.mvPosition.z);
				mStream.writeFloat(1.0f);
			}
			
			
			//write the normal data if its in the format
			if (inConversionParams.mbVertexHasNormal == true)
			{
				mStream.writeFloat(vertex.mvNormal.x);
				mStream.writeFloat(vertex.mvNormal.y);
				mStream.writeFloat(vertex.mvNormal.z);
			}
			
			//write the tex coord data if its in the format
			if (inConversionParams.mbVertexHasTexCoords == true)
			{
				mStream.writeFloat(vertex.mvTextureCoordinate.x);
				mStream.writeFloat(vertex.mvTextureCoordinate.y);
			}
			
			//write the colour data if its in the format
			if (inConversionParams.mbVertexHasColour == true)
			{
				mStream.writeByte((byte)vertex.mvVertexColour.x);
				mStream.writeByte((byte)vertex.mvVertexColour.y);
				mStream.writeByte((byte)vertex.mvVertexColour.z);
				mStream.writeByte((byte)vertex.mvVertexColour.w);
			}
			
			//write the vertex weight if its in the format
			if (inConversionParams.mbVertexHasWeights == true)
			{
				mStream.writeFloat(vertex.mvWeights.x);
				mStream.writeFloat(vertex.mvWeights.y);
				mStream.writeFloat(vertex.mvWeights.z);
				mStream.writeFloat(vertex.mvWeights.w);
			}
			
			//write the joint indices if its in the format
			if (inConversionParams.mbVertexHasJointIndices == true)
			{
				mStream.writeByte((byte)vertex.mvJointIndices.x);
				mStream.writeByte((byte)vertex.mvJointIndices.y);
				mStream.writeByte((byte)vertex.mvJointIndices.z);
				mStream.writeByte((byte)vertex.mvJointIndices.w);
			}
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Indices
	///
	/// Writes the indices to the output stream
	//-------------------------------------------------------------------
	private boolean WriteIndices(CSModelConversionParameters inConversionParams, CSModelMesh inMoStaticMesh) throws Exception
	{
		if (inMoStaticMesh.mIndexList.size() > 0)
		{
			LinkedList<Integer> indexList = inMoStaticMesh.mIndexList;
			
			if (mdwSizeOfIndices == 2)
			{
				for (int i = 0; i < indexList.size() / 3; i++)
				{
					mStream.writeUnsignedShort(indexList.get(i * 3 + 0));
					mStream.writeUnsignedShort(indexList.get(i * 3 + 1));
					mStream.writeUnsignedShort(indexList.get(i * 3 + 2));
				}
			}
			else
			{
				for (int i = 0; i < indexList.size() / 3; i++)
				{
					mStream.writeUnsignedInt((long)indexList.get(i * 3 + 0));
					mStream.writeUnsignedInt((long)indexList.get(i * 3 + 1));
					mStream.writeUnsignedInt((long)indexList.get(i * 3 + 2));
				}
			}
		}
		return true;
	}
}
