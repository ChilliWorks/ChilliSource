//
// CMoModelConverter.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter;

import java.util.Hashtable;
import java.util.LinkedList;
import java.util.Stack;

import com.chillisource.csmodelconverter.csmodel.*;
import com.chillisource.toolutils.*;
import com.taggames.colladaparser.colladadata.*;
import com.taggames.colladaparser.colladadata.ColladaGeometry.COLLADA_GEOMETRY_TYPE;

public class CSModelConverter 
{
	//-------------------------------------------------------------------
	/// Index Type
	//-------------------------------------------------------------------
	enum MS_INDEX_TYPE
	{
		NONE,
		POSITION,
		NORMAL,
		TEXCOORD,
		COLOUR,
		WEIGHT,
		JOINT_INDEX
	}
	//-------------------------------------------------------------------
	/// Private Member Data
	//-------------------------------------------------------------------
	Stack<Matrix4> mMatrixStack;
	CSModelConversionParameters mConversionParams;
	SCUserPropertiesParser mPropertiesParser;
	//-------------------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------------------
	public CSModelConverter()
	{
		mMatrixStack = new Stack<Matrix4>();
		mMatrixStack.push(Matrix4.IDENTITY);
		mPropertiesParser = new SCUserPropertiesParser();
	}
	//-------------------------------------------------------------------
	/// Convert To MoModel Format
	///
	/// Converts the input collections of data from the Collada file into 
	/// something usable for outputting in MoModel format.
	//-------------------------------------------------------------------
	public CSModel ConvertToMoModelFormat(LinkedList<String> inNodesToExport, Collada inCollada, CSModelConversionParameters inParams)
	{
		mConversionParams = inParams;
		CSModel model = new CSModel();
		
		//build the model data
		for (ColladaNode node : inCollada.mLibraryVisualScenes.get(inCollada.mScene.mInstanceVisualScene.mstrUrl.substring(1)).mRootNodes.values())
		{
			TryBuildModelDataForNode(inCollada, model, node, inNodesToExport, false);
		}
		
		//set the model overall hit box
		for (CSModelMesh mesh: model.mMeshTable.values())
		{
			if (mesh.mvMin.x < model.mvMin.x)
				model.mvMin.x = mesh.mvMin.x;
			if (mesh.mvMin.y < model.mvMin.y)
				model.mvMin.y = mesh.mvMin.y;
			if (mesh.mvMin.z < model.mvMin.z)
				model.mvMin.z = mesh.mvMin.z;
			
			if (mesh.mvMax.x > model.mvMax.x)
				model.mvMax.x = mesh.mvMax.x;
			if (mesh.mvMax.y > model.mvMax.y)
				model.mvMax.y = mesh.mvMax.y;
			if (mesh.mvMax.z > model.mvMax.z)
				model.mvMax.z = mesh.mvMax.z;
		}
		
		return model;
	}
	//-------------------------------------------------------------------
	/// Set Texture
	///
	/// Sets the texture for the MoStatic mesh
	//-------------------------------------------------------------------
	private void SetTexture(ColladaMaterial inMaterial, Collada inCollada, CSModelMesh outMesh)
	{
		//get the image from the material
		String effectName = inMaterial.mInstanceEffect.mstrUrl.substring(1);
		ColladaEffect effect = inCollada.mLibraryEffects.get(effectName);
		String samplerName = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mDiffuse.mTexture.mstrTexture;
		
		if (samplerName.length() == 0)
			return;
		
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//                        HACK ALERT!
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// The FBX Collada exporter that 3DS Max uses is not fully
		// complient to the collada specification. The way it handles
		// diffuse textures in the technique is incorrect, refering
		// directly to the image, rather than a sampler2D that uses
		// the image. In order to support this we have added a hack 
		// that checks if the collada file was created using this 
		// exporter and bypasses the sampler2D if it is.
		//
		// If the FBX Collada exporter is no longer needed, please remove
		// this hack.
		
		ColladaImage image;
		if (inCollada.mAsset.mContributor.mAuthoringTool.mstrName.equals("FBX COLLADA exporter") == false)
		{
			String surfaceName = effect.mProfileCommon.mNewParams.get(samplerName).mSampler2D.mSource.mstrContents;
			String imageName = effect.mProfileCommon.mNewParams.get(surfaceName).mSurface.mInitFrom.mstrValue;
			image = inCollada.mLibraryImages.get(imageName);
		}
		else
		{
			image = inCollada.mLibraryImages.get(samplerName);
		}
		
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//                        END HACK
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		if (image != null)
		{
			//just take the first texture
			String strTextureFullpath = image.mInitFrom.mstrValue;
			
			//convert all "\" to "/"
			strTextureFullpath = strTextureFullpath.replace("\\", "/");
			
			//get rid of the directory paths
			String[] strBrokenUpTexture = strTextureFullpath.split("/");
			
			if (strBrokenUpTexture.length > 0)
			{
				//set the texture!
				outMesh.mstrTextureName = strBrokenUpTexture[strBrokenUpTexture.length - 1];
			}
			else
			{
				outMesh.mstrTextureName = strTextureFullpath;
			}
		}
	}
	//-------------------------------------------------------------------
	/// Try Build Model Data For Node
	///
	/// Builds the model data for a node if its parent was build, or if its 
	/// id is in the list of nodes to export.
	//-------------------------------------------------------------------
	private void TryBuildModelDataForNode(Collada inCollada, CSModel inModel, ColladaNode inNode,LinkedList<String> inNodesToExport, boolean inbParentExported)
	{
		boolean bShouldExport = inbParentExported;
		
		//calculate the world position of this node.
		Matrix4 localMat = Matrix4.IDENTITY;
		if (inNode.mMatrix != null && inNode.mMatrix.maafValues != null && inNode.mMatrix.maafValues != null)
			localMat = Matrix4.createFrom2DArray(inNode.mMatrix.maafValues);
		Matrix4 worldMat = Matrix4.multiply(localMat, mMatrixStack.peek());
		mMatrixStack.push(worldMat);
		
		//if the parent was not exported, check and see if this node should be
		if (bShouldExport == false)
		{
			for (String nodeName: inNodesToExport)
			{
				if (nodeName.equalsIgnoreCase(inNode.mstrId) == true)
				{
					bShouldExport = true;
					break;
				}
			}
		}
		
		//if this should be exported, then see if it can be
		if (bShouldExport == true && 
			inNode.meType == ColladaNode.COLLADA_NODE_TYPE.CONTROLLER || inNode.meType == ColladaNode.COLLADA_NODE_TYPE.GEOMETRY)
		{
			boolean bCanExport = false;
			String skeletonName = new String();
			String geomName = new String();
			Hashtable<String, String> materialNameMap = new Hashtable<String, String>();
			String controllerName = new String();
			
			if (inNode.meType == ColladaNode.COLLADA_NODE_TYPE.CONTROLLER)
			{
				controllerName = inNode.mInstanceController.mstrUrl.substring(1);
				skeletonName = inNode.mInstanceController.mSkeleton.mstrValue.substring(1);
				geomName = inCollada.mLibraryControllers.get(inNode.mInstanceController.mstrUrl.substring(1)).mSkin.mstrSource.substring(1);
				
				for (int i = 0; i < inNode.mInstanceController.mBindMaterial.mTechniqueCommon.mInstanceMaterialList.size(); i++)
				{
					ColladaInstanceMaterial material = inNode.mInstanceController.mBindMaterial.mTechniqueCommon.mInstanceMaterialList.get(i);
					if (material.mstrTarget.length() > 0 && material.mstrSymbol.length() > 0)
					{
						materialNameMap.put(material.mstrSymbol, material.mstrTarget.substring(1));
					}
				}
				
				bCanExport = true;
			}
			
			if (inNode.meType == ColladaNode.COLLADA_NODE_TYPE.GEOMETRY)
			{
				controllerName = null;
				geomName = inNode.mInstanceGeometry.mstrUrl.substring(1);
				
				for (int i = 0; i < inNode.mInstanceGeometry.mBindMaterial.mTechniqueCommon.mInstanceMaterialList.size(); i++)
				{
					ColladaInstanceMaterial material = inNode.mInstanceGeometry.mBindMaterial.mTechniqueCommon.mInstanceMaterialList.get(i);
					if (material.mstrTarget.length() > 0 && material.mstrSymbol.length() > 0)
					{
						materialNameMap.put(material.mstrSymbol, material.mstrTarget.substring(1));
					}
				}
				
				bCanExport = true;
			}
			
			//export this node if possible
			if (bCanExport == true)
			{
				//if there is a skeleton, build it!
				if (skeletonName.length() > 0)
				{
					SCSkeletonBuilder.TryBuildSkeleton(new LinkedList<ColladaNode>(inCollada.mLibraryVisualScenes.get(inCollada.mScene.mInstanceVisualScene.mstrUrl.substring(1)).mRootNodes.values()),
							skeletonName, inModel);
				}
				
				BuildModelData(geomName, controllerName, materialNameMap, inCollada, inModel);
			}
		
		}
		
		//try children
		for (ColladaNode node: inNode.mChildNodes.values())
		{
			TryBuildModelDataForNode(inCollada, inModel, node, inNodesToExport, bShouldExport);
		}
		
		mMatrixStack.pop();
	}
	
	//-------------------------------------------------------------------
	/// Build Model Data
	///
	/// Builds the model data for the MoStatic mesh from the collada meshes.
	//-------------------------------------------------------------------
	private void BuildModelData(String instrGeometryName, String instrControllerName, Hashtable<String, String> instrMaterialNameMap, Collada inCollada, CSModel inModel)
	{
		ColladaGeometry geometry = null;
		if (inCollada.mLibraryGeometries.containsKey(instrGeometryName) == true)
			geometry = inCollada.mLibraryGeometries.get(instrGeometryName);
		
		ColladaController controller = null;
		if (instrControllerName != null && inCollada.mLibraryControllers.containsKey(instrControllerName) == true)
			controller = inCollada.mLibraryControllers.get(instrControllerName);
		
		if (geometry != null && geometry.meType == COLLADA_GEOMETRY_TYPE.MESH)
		{
			//A geometry "mesh" can contain more than one mesh, so iterate over them
			for (int i = 0; i < geometry.mMesh.mTrianglesList.size(); i++)
			{
				ColladaTriangles triangles = geometry.mMesh.mTrianglesList.get(i);
				
				//find the material
				String strMaterialName = "";
				if (triangles.mstrMaterial != null && triangles.mstrMaterial.length() > 0 && instrMaterialNameMap.containsKey(triangles.mstrMaterial) == true)
				{
					strMaterialName = instrMaterialNameMap.get(triangles.mstrMaterial);
				}
				
				ColladaMaterial material = null;
				if (strMaterialName != null && strMaterialName.length() > 0 && inCollada.mLibraryMaterials.containsKey(strMaterialName) == true)
				{
					material = inCollada.mLibraryMaterials.get(strMaterialName);
				}
				
				//if the "Combine Meshes" transform is being used, then combine all meshes that use the same material.
				CSModelMesh mesh = null;
				if (material != null && mConversionParams.mbCombineMeshes == true)
				{
					//get the mesh name
					String meshName = triangles.mstrMaterial;
					
					//get the mesh
					mesh = inModel.mMeshTable.get(meshName);
					
					if (mesh == null)
					{
						mesh = new CSModelMesh();
						mesh.mstrName = meshName;
						inModel.mMeshTable.put(meshName, mesh);
					}
				}
				else
				{
					String strMeshName = geometry.mstrName;
					
					//if this collada mesh contains more than one mesh, append the material thats being used so we can differentiate.
					if (geometry.mMesh.mTrianglesList.size() > 1)
					{
						if (material == null)
						{
							Logging.logFatal("Cannot have multiple meshes in one geometry if they do not have materials.");
						}
						
						strMeshName += "-" + triangles.mstrMaterial;
					}
					
					mesh = new CSModelMesh();
					mesh.mstrName = strMeshName;
					inModel.mMeshTable.put(mesh.mstrName, mesh);
					
					if (controller != null)
					{
						BuildInverseBindMatrices(inModel, mesh, controller);
					}
				}
				
				//build the vertex and index data
				BuildVertices(geometry, controller, triangles, mesh, inModel);
				
				//Get the texture name
				SetTexture(material, inCollada, mesh);
				
				//build the material
				BuildMaterial(material, inCollada, mesh);
			}
		}
	}
	//-------------------------------------------------------------------
	/// Build Inverse Bind Matrices
	///
	/// Adds the inverse bind matrix data to the mesh.
	//-------------------------------------------------------------------
	private void BuildInverseBindMatrices(CSModel inModel, CSModelMesh inMesh, ColladaController inController)
	{
		//iterate and get the inputs
		String jointSourceName = new String();
		String ibpSourceName = new String();
		
		for (ColladaInput input : inController.mSkin.mJoints.mInputs)
		{
			if (input.mstrSemantic.equalsIgnoreCase("INV_BIND_MATRIX") == true)
				ibpSourceName = input.mstrSource.substring(1);
			if (input.mstrSemantic.equalsIgnoreCase("JOINT") == true)
				jointSourceName = input.mstrSource.substring(1);
		}
		
		if (jointSourceName.length() == 0 || ibpSourceName.length() == 0)
			Logging.logFatal("Could not get the source names for the Inverse Bind Pose data.");
		
		//get the sources
		ColladaSource jointSource = inController.mSkin.mSourceTable.get(jointSourceName);
		ColladaSource ibpSource = inController.mSkin.mSourceTable.get(ibpSourceName);
		
		if (jointSource.mNameArray.mdwCount != ibpSource.mFloatArray.mdwCount / 16 || jointSource.mNameArray.mdwCount != SCSkeletonBuilder.GetNumberOfJoints(inModel))
		{
			Logging.logFatal("The number of joints in the controller does not match the number of joints in the Skeleton! This is most likely becuase there is something in the skeleton hierarchy that the tool cannot handle, for example geometry.");
		}
		
		//iterate through the joints
		inMesh.maInverseBindMatrices = new Matrix4[jointSource.mNameArray.mdwCount];
		for (int i = 0; i < jointSource.mNameArray.mdwCount; i++)
		{
			//get the joint sid and matrix
			String jointName = jointSource.mNameArray.mstrData[i];
			
			Matrix4 newMat = new Matrix4();
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 4; k++)
					newMat.mafData[j + 4 * k] = ibpSource.mFloatArray.mafData[i * 16 + (j * 4 + k)];
			
			//get the index for this joint
			int jointIndex = SCSkeletonBuilder.GetIndexOfJointBySId(inModel, jointName);
			
			inMesh.maInverseBindMatrices[jointIndex] = newMat;
		}
	}
	//-------------------------------------------------------------------
	/// Build Material
	///
	/// Builds the material from the collada data.
	//-------------------------------------------------------------------
	private void BuildMaterial(ColladaMaterial inMaterial, Collada inCollada, CSModelMesh inMesh)
	{
		//get the effect for this geometry
		String effectName = inMaterial.mInstanceEffect.mstrUrl.substring(1);
		ColladaEffect effect = inCollada.mLibraryEffects.get(effectName);
		
		//get each of the material properties
		CSModelMaterial newMat = new CSModelMaterial();
		
		//set the name
		newMat.mstrName = inMaterial.mstrName;
		
		//emissive (Note: Phong materials dont contain emissive data. In this case the value will default to that specified in "ColladaBlinnAndPhong")
		newMat.mvEmissive.x = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mEmissive.mColour.mfR;
		newMat.mvEmissive.y = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mEmissive.mColour.mfG;
		newMat.mvEmissive.z = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mEmissive.mColour.mfB;
		newMat.mvEmissive.w = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mEmissive.mColour.mfA;
		
		//ambient
		newMat.mvAmbient.x = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mAmbient.mColour.mfR;
		newMat.mvAmbient.y = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mAmbient.mColour.mfG;
		newMat.mvAmbient.z = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mAmbient.mColour.mfB;
		newMat.mvAmbient.w = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mAmbient.mColour.mfA;
		
		//diffuse (NOTE: If there is a texture, the collada file does not contain diffuse data. In this case the value will default to that specified 
		//in "ColladaBlinnAndPhong")
		newMat.mvDiffuse.x = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mDiffuse.mColour.mfR;
		newMat.mvDiffuse.y = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mDiffuse.mColour.mfG;
		newMat.mvDiffuse.z = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mDiffuse.mColour.mfB;
		newMat.mvDiffuse.w = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mAmbient.mColour.mfA;
		
		//specular
		newMat.mvSpecular.x = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mSpecular.mColour.mfR;
		newMat.mvSpecular.y = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mSpecular.mColour.mfG;
		newMat.mvSpecular.z = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mSpecular.mColour.mfB;
		newMat.mvSpecular.w = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mSpecular.mColour.mfA;
		
		//shininess
		newMat.mfShininess = effect.mProfileCommon.mTechnique.mBlinnAndPhong.mShininess.mFloat.mfValue;
		
		inMesh.mMaterial = newMat;
	}
	//-------------------------------------------------------------------
	/// Build Vertices
	///
	/// Builds the vertex and index data from the collada data.
	//-------------------------------------------------------------------
	private void BuildVertices(ColladaGeometry inGeometry, ColladaController inController, ColladaTriangles inTriangles, CSModelMesh inMesh, CSModel inModel)
	{
		//build the index type array
		MS_INDEX_TYPE[] eIndexTypeArray = new MS_INDEX_TYPE[inTriangles.mInputList.size()];
		for (int i = 0; i < inTriangles.mInputList.size(); i++)
		{
			MS_INDEX_TYPE eType = MS_INDEX_TYPE.NONE;
			
			if (inTriangles.mInputList.get(i).mstrSemantic.equalsIgnoreCase("VERTEX") == true)
				eType = MS_INDEX_TYPE.POSITION;
			else if (inTriangles.mInputList.get(i).mstrSemantic.equalsIgnoreCase("NORMAL") == true)
				eType = MS_INDEX_TYPE.NORMAL;
			else if (inTriangles.mInputList.get(i).mstrSemantic.equalsIgnoreCase("COLOR") == true)
				eType = MS_INDEX_TYPE.COLOUR;
			else if (inTriangles.mInputList.get(i).mstrSemantic.equalsIgnoreCase("TEXCOORD") == true)
				eType = MS_INDEX_TYPE.TEXCOORD;
			
			eIndexTypeArray[inTriangles.mInputList.get(i).mdwOffset] = eType;
		}
		
		//build the weight data
		CSModelWeightIndexData weightData = BuildWeightIndexData(inController);
		
		//use this to build the vertices
		for (int i = 0; i < inTriangles.mP.madwValues.length / inTriangles.mInputList.size(); i++)
		{
			int dwPositionIndex = -1;
			int dwNormalIndex = -1;
			int dwTexCoordIndex = -1;
			int dwColourIndex = -1;
			int dwWeightDataIndex = -1;
			
			for (int j = 0; j < inTriangles.mInputList.size(); j++)
			{
				switch (eIndexTypeArray[j])
				{
				case POSITION:
					dwPositionIndex = inTriangles.mP.madwValues[i * inTriangles.mInputList.size() + j];
					
					if (weightData != null)
					{
						dwWeightDataIndex = dwPositionIndex;
					}
					
					break;
				case NORMAL:
					dwNormalIndex = inTriangles.mP.madwValues[i * inTriangles.mInputList.size() + j];
					break;
				case COLOUR:
					dwColourIndex = inTriangles.mP.madwValues[i * inTriangles.mInputList.size() + j];
					break;
				case TEXCOORD:
					dwTexCoordIndex = inTriangles.mP.madwValues[i * inTriangles.mInputList.size() + j];
					break;
				case NONE:
					Logging.logError("Found unknown index type in mesh: " + inGeometry.mstrId);
					break;
				default:
					break;
				}
			}
			
			AddVertex(inMesh, BuildVertex(inGeometry, inController, inTriangles, inModel, weightData, dwPositionIndex, dwNormalIndex, dwTexCoordIndex, dwColourIndex, dwWeightDataIndex));
		}
	}
	//-------------------------------------------------------------------
	/// Build Weight Index Data
	///
	/// Builds the weight index data from the collada controller.
	//-------------------------------------------------------------------
	private CSModelWeightIndexData BuildWeightIndexData(ColladaController inController)
	{
		if (inController != null)
		{
			//build the index type array
			MS_INDEX_TYPE[] eIndexTypeArray = new MS_INDEX_TYPE[inController.mSkin.mVertexWeights.mInputs.size()];
			for (int i = 0; i < inController.mSkin.mVertexWeights.mInputs.size(); i++)
			{
				MS_INDEX_TYPE eType = MS_INDEX_TYPE.NONE;
				
				if (inController.mSkin.mVertexWeights.mInputs.get(i).mstrSemantic.equalsIgnoreCase("JOINT") == true)
					eType = MS_INDEX_TYPE.JOINT_INDEX;
				else if (inController.mSkin.mVertexWeights.mInputs.get(i).mstrSemantic.equalsIgnoreCase("WEIGHT") == true)
					eType = MS_INDEX_TYPE.WEIGHT;
				
				eIndexTypeArray[inController.mSkin.mVertexWeights.mInputs.get(i).mdwOffset] = eType;
			}
			
			CSModelWeightIndexData weightData = new CSModelWeightIndexData();
			weightData.mWeightIndices = new int[inController.mSkin.mVertexWeights.mdwCount][];
			weightData.mJointIndices = new int[inController.mSkin.mVertexWeights.mdwCount][];
			int dwCount = 0;
			for (int i = 0; i < inController.mSkin.mVertexWeights.mdwCount; i++)
			{
				weightData.mWeightIndices[i] = new int[inController.mSkin.mVertexWeights.mVCount.madwData[i]];
				weightData.mJointIndices[i] = new int[inController.mSkin.mVertexWeights.mVCount.madwData[i]];
				for (int j = 0; j < inController.mSkin.mVertexWeights.mVCount.madwData[i]; j++)
				{
					for (int k = 0; k < eIndexTypeArray.length; k++)
					{
						switch (eIndexTypeArray[k])
						{
						case WEIGHT:
							weightData.mWeightIndices[i][j] = inController.mSkin.mVertexWeights.mV.madwData[dwCount++];					
							break;
						case JOINT_INDEX:
							weightData.mJointIndices[i][j] = inController.mSkin.mVertexWeights.mV.madwData[dwCount++];
							break;
						default:
							break;
						}
					}
				}
			}
			
			return weightData;
		}
		
		return null;
	}
	//-------------------------------------------------------------------
	/// Build Vertex
	///
	/// Builds a vertex from a collada mesh and collada index data.
	//-------------------------------------------------------------------
	private CSModelVertex BuildVertex(ColladaGeometry inGeometry, ColladaController inController, ColladaTriangles inTriangles, CSModel inModel, CSModelWeightIndexData inWeightData, int indwPositionIndex, int indwNormalIndex, int indwTexCoordIndex, int indwColourIndex, int indwWeightDataIndex)
	{
		CSModelVertex newVert = new CSModelVertex();
		AddPosition(inGeometry, inController, inTriangles, newVert, indwPositionIndex);
		AddNormal(inGeometry, inController, inTriangles, newVert, indwNormalIndex);
		AddTextureCoordinate(inGeometry, inTriangles, newVert, indwTexCoordIndex);
		AddColour(inGeometry, inTriangles, newVert, indwColourIndex);
		AddVertexWeights(inGeometry, inController, newVert, inWeightData, indwWeightDataIndex);
		AddJointIndices(inGeometry, inController, inModel, newVert, inWeightData, indwWeightDataIndex);
		return newVert;
	}
	//-------------------------------------------------------------------
	/// Add Position
	///
	/// Adds the position to a vertex.
	//-------------------------------------------------------------------
	private void AddPosition(ColladaGeometry inGeometry, ColladaController inController, ColladaTriangles inTriangles, CSModelVertex inVertex, int indwPositionIndex)
	{
		//add the position
		if (indwPositionIndex != -1)
		{
			//get the position input
			ColladaInput input = GetInputFromGeometry(inGeometry, inTriangles, "VERTEX");
			if (inGeometry.mMesh.mVertices.mstrId.equals(input.mstrSource.substring(1)) == false || inGeometry.mMesh.mVertices.mInput.mstrSemantic.equals("POSITION") == false)
			{
				Logging.logFatal("'Vertices' with id '" + inGeometry.mMesh.mVertices.mstrId + "' and semantic '"+ inGeometry.mMesh.mVertices.mInput.mstrSemantic +"' is not supported.");
			}
			
			//get the source and stride
			ColladaSource source = inGeometry.mMesh.mSources.get(inGeometry.mMesh.mVertices.mInput.mstrSource.substring(1));
			int dwStride = source.mTechniqueCommon.mAccessor.mdwStride;
			if (dwStride < 3)
			{
				Logging.logFatal("A stride of less than 3 is not supported for POSITION sources.");
			}
			
			//get the vertex data
			inVertex.mvPosition.x = source.mFloatArray.mafData[indwPositionIndex * dwStride + 0];
			inVertex.mvPosition.y = source.mFloatArray.mafData[indwPositionIndex * dwStride + 1];
			inVertex.mvPosition.z = source.mFloatArray.mafData[indwPositionIndex * dwStride + 2];
			
			//if there is a controller, apply its bind shape matrix
			if (inController != null)
			{
				//convert the bind shape matrix to something we can use.
				Matrix4 bindShapeMatrix = Matrix4.createFrom2DArray(inController.mSkin.mBindShapeMatrix.maafValues);
				inVertex.mvPosition = Matrix4.multiply(inVertex.mvPosition, bindShapeMatrix);
			}
			
			inVertex.mvPosition = Matrix4.multiply(inVertex.mvPosition, mMatrixStack.peek());
		}
	}
	//-------------------------------------------------------------------
	/// Add Normal
	///
	/// Adds the normal to a vertex.
	//-------------------------------------------------------------------
	private void AddNormal(ColladaGeometry inGeometry, ColladaController inController, ColladaTriangles inTriangles, CSModelVertex inVertex, int indwNormalIndex)
	{
		if (indwNormalIndex != -1)
		{
			//get the normal input
			ColladaInput input = GetInputFromGeometry(inGeometry, inTriangles, "NORMAL");
			
			//get the source and stride
			ColladaSource source = inGeometry.mMesh.mSources.get(input.mstrSource.substring(1));
			int dwStride = source.mTechniqueCommon.mAccessor.mdwStride;
			if (dwStride < 3)
			{
				Logging.logFatal("A stride of less than 3 is not supported for NORMAL sources.");
			}
			
			inVertex.mvNormal.x = source.mFloatArray.mafData[indwNormalIndex * dwStride + 0];
			inVertex.mvNormal.y = source.mFloatArray.mafData[indwNormalIndex * dwStride + 1];
			inVertex.mvNormal.z = source.mFloatArray.mafData[indwNormalIndex * dwStride + 2];
			
			//if there is a controller, apply its bind shape matrix
			if (inController != null)
			{
				Matrix4 bindShapeMatrix = Matrix4.createFrom2DArray(inController.mSkin.mBindShapeMatrix.maafValues);
				bindShapeMatrix.mafData[12] = 0.0f;
				bindShapeMatrix.mafData[13] = 0.0f;
				bindShapeMatrix.mafData[14] = 0.0f;
				
				inVertex.mvNormal = Matrix4.multiply(inVertex.mvNormal, bindShapeMatrix);
			}
			Matrix4 normalWorldMatrix = mMatrixStack.peek().copy();
			normalWorldMatrix.mafData[12] = 0.0f;
			normalWorldMatrix.mafData[13] = 0.0f;
			normalWorldMatrix.mafData[14] = 0.0f;

			inVertex.mvNormal = Matrix4.multiply(inVertex.mvNormal, normalWorldMatrix);
			inVertex.mvNormal = inVertex.mvNormal.normalise();
		}
	}
	//-------------------------------------------------------------------
	/// Add Texture Coordinates
	///
	/// Adds the texture coordinates to a vertex.
	//-------------------------------------------------------------------
	private void AddTextureCoordinate(ColladaGeometry inGeometry, ColladaTriangles inTriangles, CSModelVertex inVertex, int indwTexCoordIndex)
	{
		//add the texture coordinates
		if (indwTexCoordIndex != -1)
		{
			//get the tex coord input
			ColladaInput input = GetInputFromGeometry(inGeometry, inTriangles, "TEXCOORD");
			
			//get the source and stride
			ColladaSource source = inGeometry.mMesh.mSources.get(input.mstrSource.substring(1));
			int dwStride = source.mTechniqueCommon.mAccessor.mdwStride;
			if (dwStride < 2)
			{
				Logging.logFatal("A stride of less than 2 is not supported for TEXCOORD sources.");
			}
			
			inVertex.mvTextureCoordinate.x = source.mFloatArray.mafData[indwTexCoordIndex * dwStride + 0]; 
			inVertex.mvTextureCoordinate.y = source.mFloatArray.mafData[indwTexCoordIndex * dwStride + 1]; 
		}
	}
	//-------------------------------------------------------------------
	/// Add Colour
	///
	/// Adds the colour to a vertex.
	//-------------------------------------------------------------------
	private void AddColour(ColladaGeometry inGeometry, ColladaTriangles inTriangles, CSModelVertex inVertex, int indwColourIndex)
	{
		//add the vertex colours
		if (indwColourIndex != -1)
		{
			//get the colour input
			ColladaInput input = GetInputFromGeometry(inGeometry, inTriangles, "COLOR");
			
			//get the source and stride
			ColladaSource source = inGeometry.mMesh.mSources.get(input.mstrSource.substring(1));
			int dwStride = source.mTechniqueCommon.mAccessor.mdwStride;
			if (dwStride < 3)
			{
				Logging.logFatal("A stride of less than 2 is not supported for COLOR sources.");
			}
			
			inVertex.mvVertexColour.x = source.mFloatArray.mafData[indwColourIndex * dwStride + 0] * 255.0f; 
			inVertex.mvVertexColour.y = source.mFloatArray.mafData[indwColourIndex * dwStride + 1] * 255.0f;
			inVertex.mvVertexColour.z = source.mFloatArray.mafData[indwColourIndex * dwStride + 2] * 255.0f;
			inVertex.mvVertexColour.w = 255.0f;
		}
	}
	//-------------------------------------------------------------------
	/// Add Vertex Weights
	///
	/// Adds the vertex weights to a vertex.
	//-------------------------------------------------------------------
	private void AddVertexWeights(ColladaGeometry inGeometry, ColladaController inController, CSModelVertex inVertex, CSModelWeightIndexData inWeightData, int indwWeightDataIndex)
	{
		//add the vertex weights
		if (indwWeightDataIndex != -1 && inWeightData != null)
		{
			//get the vertex weight input
			ColladaInput input = GetInputFromController(inController, "WEIGHT");
			
			//get the source
			ColladaSource source = inController.mSkin.mSourceTable.get(input.mstrSource.substring(1));
			
			//weight 0
			if (inWeightData.mWeightIndices[indwWeightDataIndex].length > 0)
			{
				int index = inWeightData.mWeightIndices[indwWeightDataIndex][0];
				inVertex.mvWeights.x = source.mFloatArray.mafData[index];
			}
			
			//weight 1
			if (inWeightData.mWeightIndices[indwWeightDataIndex].length > 1)
			{
				int index = inWeightData.mWeightIndices[indwWeightDataIndex][1];
				inVertex.mvWeights.y = source.mFloatArray.mafData[index];
			}
			
			//weight 2
			if (inWeightData.mWeightIndices[indwWeightDataIndex].length > 2)
			{
				int index = inWeightData.mWeightIndices[indwWeightDataIndex][2];
				inVertex.mvWeights.z = source.mFloatArray.mafData[index];
			}
			
			//weight 3
			if (inWeightData.mWeightIndices[indwWeightDataIndex].length > 3)
			{
				int index = inWeightData.mWeightIndices[indwWeightDataIndex][3];
				inVertex.mvWeights.w = source.mFloatArray.mafData[index];
			}
			
			//check that the weights are valid.
			final float kfTolerance = 0.0001f;
			float fTotal = inVertex.mvWeights.x +inVertex.mvWeights.y + inVertex.mvWeights.z + inVertex.mvWeights.w;
			if (fTotal < 1.0f - kfTolerance || fTotal > 1.0f + kfTolerance)
			{
				Logging.logWarning("Vertex weights do not add up to 1.0.");
			}
		}
	}
	//-------------------------------------------------------------------
	/// Add Joint Indices
	///
	/// Adds the joint indices to a vertex.
	//-------------------------------------------------------------------
	private void AddJointIndices(ColladaGeometry inGeometry, ColladaController inController, CSModel inModel, CSModelVertex inVertex, CSModelWeightIndexData inWeightData, int indwWeightDataIndex)
	{
		//add the joint indices
		if (indwWeightDataIndex != -1 && inWeightData != null)
		{
			//get the vertex weight input
			ColladaInput input = GetInputFromController(inController, "JOINT");
			
			//get the source
			ColladaSource source = inController.mSkin.mSourceTable.get(input.mstrSource.substring(1));
			
			//weight 0
			if (inWeightData.mJointIndices[indwWeightDataIndex].length > 0)
			{
				int index = inWeightData.mJointIndices[indwWeightDataIndex][0];
				inVertex.mvJointIndices.x = SCSkeletonBuilder.GetIndexOfJointBySId(inModel, source.mNameArray.mstrData[index]);
			}
			
			//weight 1
			if (inWeightData.mJointIndices[indwWeightDataIndex].length > 1)
			{
				int index = inWeightData.mJointIndices[indwWeightDataIndex][1];
				inVertex.mvJointIndices.y = SCSkeletonBuilder.GetIndexOfJointBySId(inModel, source.mNameArray.mstrData[index]);
			}
			
			//weight 2
			if (inWeightData.mJointIndices[indwWeightDataIndex].length > 2)
			{
				int index = inWeightData.mJointIndices[indwWeightDataIndex][2];
				inVertex.mvJointIndices.z = SCSkeletonBuilder.GetIndexOfJointBySId(inModel, source.mNameArray.mstrData[index]);
			}
			
			//weight 3
			if (inWeightData.mJointIndices[indwWeightDataIndex].length > 3)
			{
				int index = inWeightData.mJointIndices[indwWeightDataIndex][3];
				inVertex.mvJointIndices.w = SCSkeletonBuilder.GetIndexOfJointBySId(inModel, source.mNameArray.mstrData[index]);
			}
		}
	}
	//-------------------------------------------------------------------
	/// Get Input From Geometry
	///
	/// Get the input for the given sematic.
	//-------------------------------------------------------------------
	private ColladaInput GetInputFromGeometry(ColladaGeometry inGeometry, ColladaTriangles inTriangles, String instrSemantic)
	{
		for (ColladaInput input : inTriangles.mInputList)
		{
			if (input.mstrSemantic.equals(instrSemantic))
			{
				return input;
			}
		}
		
		Logging.logFatal("Geometry '" + inGeometry.mstrName + "' doesn't contain semantic '" + instrSemantic + "'");
		return null;
	}
	//-------------------------------------------------------------------
	/// Get Input From Controller
	///
	/// Get the input for the given sematic.
	//-------------------------------------------------------------------
	private ColladaInput GetInputFromController(ColladaController inController, String instrSemantic)
	{
		for (ColladaInput input : inController.mSkin.mVertexWeights.mInputs)
		{
			if (input.mstrSemantic.equals(instrSemantic))
			{
				return input;
			}
		}
		
		Logging.logFatal("Controller '" + inController.mstrId + "' doesn't contain semantic '" + instrSemantic + "'");
		return null;
	}
	//-------------------------------------------------------------------
	/// Add Vertex
	///
	/// Checks to see if a vertex already exists. If it does then simply 
	/// add an index to it, if not add it and add a index to the new vertex.
	//-------------------------------------------------------------------
	private void AddVertex(CSModelMesh inMesh, CSModelVertex inVertex)
	{
		//check and see if this vertex already exists
		int dwIndex = FindVertex(inMesh, inVertex);
		
		if (dwIndex > -1)
		{
			inMesh.mIndexList.add(dwIndex);
		}
		else
		{
			inMesh.mVertexList.add(inVertex);
			inMesh.mIndexList.add(inMesh.mVertexList.size() - 1);
			
			//update mesh boundries
			if (inVertex.mvPosition.x > inMesh.mvMax.x)
				inMesh.mvMax.x = inVertex.mvPosition.x;
			if (inVertex.mvPosition.y > inMesh.mvMax.y)
				inMesh.mvMax.y = inVertex.mvPosition.y;
			if (inVertex.mvPosition.z > inMesh.mvMax.z)
				inMesh.mvMax.z = inVertex.mvPosition.z;
			if (inVertex.mvPosition.x < inMesh.mvMin.x)
				inMesh.mvMin.x = inVertex.mvPosition.x;
			if (inVertex.mvPosition.y < inMesh.mvMin.y)
				inMesh.mvMin.y = inVertex.mvPosition.y;
			if (inVertex.mvPosition.z < inMesh.mvMin.z)
				inMesh.mvMin.z = inVertex.mvPosition.z;
		}
	}
	//-------------------------------------------------------------------
	/// Find Vertex
	///
	/// Finds an identical vertex to the one passed in, and returns its
	/// index in the mesh. if it does not exist -1 is returned.
	//-------------------------------------------------------------------
	private int FindVertex(CSModelMesh inMesh, CSModelVertex inVertex)
	{
		int count = 0;
		for (CSModelVertex vert: inMesh.mVertexList)
		{
			if (vert.equals(inVertex) == true)
				return count;
			
			count++;
		}
		return -1;
	}
}
















