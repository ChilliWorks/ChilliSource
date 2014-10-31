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

import java.io.DataOutputStream;
import java.io.FileOutputStream;
import java.util.LinkedList;

import com.chilliworks.chillisource.colladatocsmodel.csmodel.*;
import com.chilliworks.chillisource.coreutils.*;


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
	DataOutputStream mStream;
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
	public boolean Output(ColladaToCSModelOptions inConversionParams, CSModel inMoModel) throws Exception
	{
		boolean bSuccess = true;
		
		CalculateSizeOfIndices(inMoModel);
		
		//try and open a new file stream. if this fails, throw a fatal error.
		try
		{
			mStream = new DataOutputStream(new FileOutputStream(inConversionParams.m_outputFilePath));
		}
		catch (Exception e)
		{
			mStream.close();
			Logging.logFatal("Failed to open output file: " + inConversionParams.m_outputFilePath);
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
			Logging.logFatal("Failed to write to file: " + inConversionParams.m_outputFilePath);
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
	private int GetNumFeatures(ColladaToCSModelOptions inConversionParams)
	{
		int dwNumFeatures = 0;
		
		if (inConversionParams.m_animated == true) 
			dwNumFeatures++;
		
		return dwNumFeatures;
	}
	//-------------------------------------------------------------------
	/// Get Num Vertex Elements
	///
	/// Calculates the number of vertex elements to be outputed from the 
	/// conversion parameters
	//-------------------------------------------------------------------
	private int GetNumVertexElements(ColladaToCSModelOptions inConversionParams)
	{
		int dwNumVertexElements = 0;
		
		if (inConversionParams.m_vertexHasPosition == true) 
			dwNumVertexElements++;
		if (inConversionParams.m_vertexHasNormal == true)
			dwNumVertexElements++;
		if (inConversionParams.m_vertexHasTexCoords == true)
			dwNumVertexElements++;
		if (inConversionParams.m_vertexHasColour == true) 
			dwNumVertexElements++;
		if (inConversionParams.m_vertexHasWeights == true) 
			dwNumVertexElements++;
		if (inConversionParams.m_vertexHasJointIndices == true) 
			dwNumVertexElements++;
		
		return dwNumVertexElements;
	}
	//-------------------------------------------------------------------
	/// Write Global Header
	///
	/// Writes the file format header.
	//-------------------------------------------------------------------
	private boolean WriteGlobalHeader(ColladaToCSModelOptions inConversionParams, CSModel inMoModel) throws Exception
	{
		//write the endianess check value and version number
		LittleEndianWriterUtils.writeUInt32(mStream, kdwEndiannessCheckValue);
		LittleEndianWriterUtils.writeUInt32(mStream, kdwVersionNumber);
		
		//output the feature declaration
		mStream.writeByte((byte)GetNumFeatures(inConversionParams));
		if (inConversionParams.m_animated == true) 
		{
			mStream.writeByte((byte)kdwFeatureHasAnimationData);
		}
		
		//output the Vertex Description
		mStream.writeByte((byte)GetNumVertexElements(inConversionParams));
		if (inConversionParams.m_vertexHasPosition == true)
			mStream.writeByte((byte)kdwVertexPosition);
		if (inConversionParams.m_vertexHasNormal == true)
			mStream.writeByte((byte)kdwVertexNormal);
		if (inConversionParams.m_vertexHasTexCoords == true) 
			mStream.writeByte((byte)kdwVertexTexCoord);
		if (inConversionParams.m_vertexHasColour == true) 
			mStream.writeByte((byte)kdwVertexColour);
		if (inConversionParams.m_vertexHasWeights == true) 
			mStream.writeByte((byte)kdwVertexWeight);
		if (inConversionParams.m_vertexHasJointIndices == true) 
			mStream.writeByte((byte)kdwVertexJointIndex);
		
		//index declaration
		mStream.writeByte((byte)mdwSizeOfIndices);
		
		//output the bounding box data
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModel.mvMin.getX());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModel.mvMin.getY());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModel.mvMin.getZ());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModel.mvMax.getX());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModel.mvMax.getY());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModel.mvMax.getZ());
		
		//output meshes!
		LittleEndianWriterUtils.writeUInt16(mStream, inMoModel.mMeshTable.size());
		
		if (inConversionParams.m_animated == true)
		{
			LittleEndianWriterUtils.writeInt16(mStream, (short)inMoModel.mSkeleton.mNodeList.size());
			mStream.writeByte((byte)SkeletonBuilder.GetNumberOfJoints(inMoModel));
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Skeleton
	///
	/// Writes the skeleton data to file.
	//-------------------------------------------------------------------
	private boolean WriteSkeleton(ColladaToCSModelOptions inConversionParams, CSModel inMoModel) throws Exception
	{
		if (inConversionParams.m_animated == true)
		{
			for (int i = 0; i < inMoModel.mSkeleton.mNodeList.size(); i++)
			{
				CSModelSkeletonNode node = inMoModel.mSkeleton.mNodeList.get(i);
				
				//write joint name
				StringWriterUtils.writeUTF8StringNullTerminated(mStream, node.mstrName);
				
				//write parent index
				LittleEndianWriterUtils.writeInt16(mStream, (short)node.mdwParentNodeIndex);
				
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
	private boolean WriteMeshHeader(ColladaToCSModelOptions inConversionParams, CSModelMesh inMoModelMesh) throws Exception
	{
		//write the mesh name
		StringWriterUtils.writeUTF8StringNullTerminated(mStream, inMoModelMesh.mstrName);
		
		//write the number of verts and indices
		if (mdwSizeOfIndices == 2)
		{
			LittleEndianWriterUtils.writeUInt16(mStream, inMoModelMesh.mVertexList.size());
			LittleEndianWriterUtils.writeUInt16(mStream, inMoModelMesh.mIndexList.size() / 3);
		}
		else
		{
			LittleEndianWriterUtils.writeUInt32(mStream, (long)inMoModelMesh.mVertexList.size());
			LittleEndianWriterUtils.writeUInt32(mStream, (long)inMoModelMesh.mIndexList.size() / 3);
		}
		
		//write the bounds
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModelMesh.mvMin.getX());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModelMesh.mvMin.getY());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModelMesh.mvMin.getZ());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModelMesh.mvMax.getX());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModelMesh.mvMax.getY());
		LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoModelMesh.mvMax.getZ());
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Mesh Body
	///
	/// Writes the mesh body data to file.
	//-------------------------------------------------------------------
	private boolean WriteMeshBody(ColladaToCSModelOptions inConversionParams, CSModelMesh inMoStaticMesh) throws Exception
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
	private boolean WriteInverseBindMatrices(ColladaToCSModelOptions inConversionParams, CSModelMesh inMoStaticMesh) throws Exception
	{
		if (inConversionParams.m_animated == true)
		{
			if (inMoStaticMesh.maInverseBindMatrices != null)
			{
				for (int i = 0; i < inMoStaticMesh.maInverseBindMatrices.length; i++)
				{
					for (int j = 0; j < 16; j++)
					{
						LittleEndianWriterUtils.writeFloat32(mStream, (float)inMoStaticMesh.maInverseBindMatrices[i].get(j));
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
	private boolean WriteVertices(ColladaToCSModelOptions inConversionParams, CSModelMesh inMoStaticMesh) throws Exception
	{
		LinkedList<CSModelVertex> vertexList = inMoStaticMesh.mVertexList;
		
		for (int i = 0; i < vertexList.size(); i++)
		{
			CSModelVertex vertex = vertexList.get(i);
			
			//write the position data
			if (inConversionParams.m_vertexHasPosition == true)
			{
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvPosition.getX());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvPosition.getY());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvPosition.getZ());
				LittleEndianWriterUtils.writeFloat32(mStream, 1.0f);
			}
			
			//write the normal data if its in the format
			if (inConversionParams.m_vertexHasNormal == true)
			{
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvNormal.getX());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvNormal.getY());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvNormal.getZ());
			}
			
			//write the tex coord data if its in the format
			if (inConversionParams.m_vertexHasTexCoords == true)
			{
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvTextureCoordinate.getX());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvTextureCoordinate.getY());
			}
			
			//write the colour data if its in the format
			if (inConversionParams.m_vertexHasColour == true)
			{
				mStream.writeByte((byte)vertex.mvVertexColour.getX());
				mStream.writeByte((byte)vertex.mvVertexColour.getY());
				mStream.writeByte((byte)vertex.mvVertexColour.getZ());
				mStream.writeByte((byte)vertex.mvVertexColour.getW());
			}
			
			//write the vertex weight if its in the format
			if (inConversionParams.m_vertexHasWeights == true)
			{
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvWeights.getX());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvWeights.getY());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvWeights.getZ());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)vertex.mvWeights.getW());
			}
			
			//write the joint indices if its in the format
			if (inConversionParams.m_vertexHasJointIndices == true)
			{
				mStream.writeByte((byte)vertex.mvJointIndices.getX());
				mStream.writeByte((byte)vertex.mvJointIndices.getY());
				mStream.writeByte((byte)vertex.mvJointIndices.getZ());
				mStream.writeByte((byte)vertex.mvJointIndices.getW());
			}
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Write Indices
	///
	/// Writes the indices to the output stream
	//-------------------------------------------------------------------
	private boolean WriteIndices(ColladaToCSModelOptions inConversionParams, CSModelMesh inMoStaticMesh) throws Exception
	{
		if (inMoStaticMesh.mIndexList.size() > 0)
		{
			LinkedList<Integer> indexList = inMoStaticMesh.mIndexList;
			
			if (mdwSizeOfIndices == 2)
			{
				for (int i = 0; i < indexList.size() / 3; i++)
				{
					LittleEndianWriterUtils.writeUInt16(mStream, indexList.get(i * 3 + 0));
					LittleEndianWriterUtils.writeUInt16(mStream, indexList.get(i * 3 + 1));
					LittleEndianWriterUtils.writeUInt16(mStream, indexList.get(i * 3 + 2));
				}
			}
			else
			{
				for (int i = 0; i < indexList.size() / 3; i++)
				{
					LittleEndianWriterUtils.writeUInt32(mStream, (long)indexList.get(i * 3 + 0));
					LittleEndianWriterUtils.writeUInt32(mStream, (long)indexList.get(i * 3 + 1));
					LittleEndianWriterUtils.writeUInt32(mStream, (long)indexList.get(i * 3 + 2));
				}
			}
		}
		return true;
	}
}
