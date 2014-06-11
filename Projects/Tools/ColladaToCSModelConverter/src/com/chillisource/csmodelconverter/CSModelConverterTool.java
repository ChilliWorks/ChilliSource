//
// CMoModelConverterTool.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.chillisource.csmodelconverter;

import java.io.FileNotFoundException;
import java.util.LinkedList;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import com.chillisource.csmodelconverter.csmodel.CSModel;
import com.chillisource.csmodelconverter.csmodel.CSModelMesh;
import com.taggames.toolutils.*;
import com.taggames.colladaparser.CColladaParser;
import com.taggames.colladaparser.colladadata.*;

public class CSModelConverterTool 
{
	//-------------------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------------------
	public CSModelConverterTool()
	{
	}
	//-------------------------------------------------------------------
	/// Convert
	///
	/// Converts a Collada file to a momodel file based on the given
	/// parameters.
	//-------------------------------------------------------------------
	public void Convert(CSModelConversionParameters inParams) throws Exception
	{
		Collada colladaData = new Collada();
		try
		{
			//create the handler
			CColladaParser handler = new CColladaParser(colladaData);
			
			//create the parser
			SAXParserFactory factory = SAXParserFactory.newInstance();
			SAXParser saxParser = factory.newSAXParser();
			
			//start reading the collada file
			saxParser.parse(inParams.mstrInputFilepath, handler);
		}
		catch (FileNotFoundException e)
		{
			SCLogger.LogFatalError("Could not find file: " + inParams.mstrInputFilepath);
		}
		catch (Exception e)
		{
			SCLogger.LogFatalError("Exception occurred while parsing the collada file. The following is the exception error message:\n-\n " + SCStringUtils.ConvertExceptionToString(e) + "-");
		}
		
		//get the meshes to be exported
		LinkedList<String> nodes = new LinkedList<String>();
		for (ColladaNode node : colladaData.mLibraryVisualScenes.get(colladaData.mScene.mInstanceVisualScene.mstrUrl.substring(1)).mRootNodes.values())
		{
			nodes.add(node.mstrId);
		}
		
		//convert to MoModel Format
		CSModelConverter converter = new CSModelConverter(); 
		CSModel model = converter.ConvertToMoModelFormat(nodes, colladaData, inParams);
		
		//Check the momodel for warnings and errors
		SCModelChecker.CheckModel(model);
		
		//Modify the data to the intended output format
		CSModelTransformer modifier = new CSModelTransformer(); 
		modifier.Modify(inParams, model);
			
		//output info on the mo model mesh
		OutputInfoOnMoModel(model);

		//output MoStatic file
		CSModelOutputer outputer = new CSModelOutputer();
		
		if (outputer.Output(inParams, model) == true)
		{
			//output info on output
			OutputInfoOnOutput(inParams);
		}
		else
		{
			SCLogger.LogError("Output failed!");
		}
	}
	//-------------------------------------------------------------------
	/// Output Info On MoModel
	///
	/// Prints info on the output MoModel mesh.
	//-------------------------------------------------------------------
	private void OutputInfoOnMoModel(CSModel inModel)
	{
		SCLogger.LogMessage("MoModel Model Generated.");
		SCLogger.LogMessage(" Min Boundary: (" + inModel.mvMin.x + ", " + inModel.mvMin.y + ", " + inModel.mvMin.z + ")");
		SCLogger.LogMessage(" Max Boundary: (" + inModel.mvMax.x + ", " + inModel.mvMax.y + ", " + inModel.mvMax.z + ")");
		SCLogger.LogMessage(" Skeleton");
		SCLogger.LogMessage("  Num Skeleton Nodes: " + inModel.mSkeleton.mNodeList.size());
		SCLogger.LogMessage("   Number of which are Joints: " + SCSkeletonBuilder.GetNumberOfJoints(inModel));
		SCLogger.LogMessage(" Number of Meshes: " + inModel.mMeshTable.size());
		for (CSModelMesh mesh: inModel.mMeshTable.values())
		{
			SCLogger.LogMessage(" Mesh " + mesh.mstrName);
			SCLogger.LogMessage("  Number of Vertices: " + mesh.mVertexList.size());
			SCLogger.LogMessage("  Number of Indices: " + mesh.mIndexList.size());
			SCLogger.LogMessage("  Texture: " + mesh.mstrTextureName);
			SCLogger.LogMessage("  Min Boundary: (" + mesh.mvMin.x + ", " + mesh.mvMin.y + ", " + mesh.mvMin.z + ")");
			SCLogger.LogMessage("  Max Boundary: (" + mesh.mvMax.x + ", " + mesh.mvMax.y + ", " + mesh.mvMax.z + ")");
			SCLogger.LogMessage("  Material");
			SCLogger.LogMessage("   Emissive: (" + mesh.mMaterial.mvEmissive.x + ", " + mesh.mMaterial.mvEmissive.y + ", " + mesh.mMaterial.mvEmissive.z + ", " + mesh.mMaterial.mvEmissive.w + ")");
			SCLogger.LogMessage("   Ambient: (" + mesh.mMaterial.mvAmbient.x + ", " + mesh.mMaterial.mvAmbient.y + ", " + mesh.mMaterial.mvAmbient.z + ", " + mesh.mMaterial.mvAmbient.w + ")");
			SCLogger.LogMessage("   Diffuse: (" + mesh.mMaterial.mvDiffuse.x + ", " + mesh.mMaterial.mvDiffuse.y + ", " + mesh.mMaterial.mvDiffuse.z + ", " + mesh.mMaterial.mvDiffuse.w + ")");
			SCLogger.LogMessage("   Specular: (" + mesh.mMaterial.mvSpecular.x + ", " + mesh.mMaterial.mvSpecular.y + ", " + mesh.mMaterial.mvSpecular.z + ", " + mesh.mMaterial.mvSpecular.w + ")");
			SCLogger.LogMessage("   Shininess: " + mesh.mMaterial.mfShininess);
		}
	}
	//-------------------------------------------------------------------
	/// Output Info On Output
	///
	/// Output data on the outputted file.
	//-------------------------------------------------------------------
	private void OutputInfoOnOutput(CSModelConversionParameters inParams)
	{
		SCLogger.LogMessage("Successfully created " + inParams.mstrOutputFilepath);
		SCLogger.LogMessage(" Features");
		SCLogger.LogMessage("  Has Animation Data: " + Boolean.toString(inParams.mbHasAnimationData));
		SCLogger.LogMessage(" Vertex Declaration");
		SCLogger.LogMessage("  Position: " + Boolean.toString(inParams.mbVertexHasPosition));
		SCLogger.LogMessage("  Normal: " + Boolean.toString(inParams.mbVertexHasNormal));
		SCLogger.LogMessage("  Texture Coordinates: " + Boolean.toString(inParams.mbVertexHasTexCoords));
		SCLogger.LogMessage("  Colour: " + Boolean.toString(inParams.mbVertexHasColour));
		SCLogger.LogMessage("  Weights: " + Boolean.toString(inParams.mbVertexHasWeights));
		SCLogger.LogMessage("  Joint Indices: " + Boolean.toString(inParams.mbVertexHasJointIndices));
		SCLogger.LogMessage(" Transforms");
		SCLogger.LogMessage("  Flip vertical texture coordinates: " + Boolean.toString(inParams.mbFlipVerticalTexCoords));
		SCLogger.LogMessage("  Mirror in the XZ Plane: " + Boolean.toString(inParams.mbMirrorInXZPlane));
		SCLogger.LogMessage("  Swap Y and Z: " + Boolean.toString(inParams.mbSwapYAndZ));
		SCLogger.LogMessage(" ");
	}
}
