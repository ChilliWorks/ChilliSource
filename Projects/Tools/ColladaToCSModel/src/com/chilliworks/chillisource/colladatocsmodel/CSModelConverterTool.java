/**
 * CSModelConverterTool.java
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

import java.io.FileNotFoundException;
import java.util.LinkedList;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import com.chilliworks.chillisource.colladaparserutils.ColladaParser;
import com.chilliworks.chillisource.colladaparserutils.colladadata.*;
import com.chilliworks.chillisource.colladatocsmodel.csmodel.CSModel;
import com.chilliworks.chillisource.colladatocsmodel.csmodel.CSModelMesh;
import com.chilliworks.chillisource.toolutils.*;

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
			ColladaParser handler = new ColladaParser(colladaData);
			
			//create the parser
			SAXParserFactory factory = SAXParserFactory.newInstance();
			SAXParser saxParser = factory.newSAXParser();
			
			//start reading the collada file
			saxParser.parse(inParams.mstrInputFilepath, handler);
		}
		catch (FileNotFoundException e)
		{
			Logging.logFatal("Could not find file: " + inParams.mstrInputFilepath);
		}
		catch (Exception e)
		{
			Logging.logFatal("Exception occurred while parsing the collada file. The following is the exception error message:\n-\n " + StringUtils.convertExceptionToString(e) + "-");
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
		ModelChecker.CheckModel(model);
		
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
			Logging.logError("Output failed!");
		}
	}
	//-------------------------------------------------------------------
	/// Output Info On MoModel
	///
	/// Prints info on the output MoModel mesh.
	//-------------------------------------------------------------------
	private void OutputInfoOnMoModel(CSModel inModel)
	{
		Logging.logVerbose("MoModel Model Generated.");
		Logging.logVerbose(" Min Boundary: (" + inModel.mvMin.x + ", " + inModel.mvMin.y + ", " + inModel.mvMin.z + ")");
		Logging.logVerbose(" Max Boundary: (" + inModel.mvMax.x + ", " + inModel.mvMax.y + ", " + inModel.mvMax.z + ")");
		Logging.logVerbose(" Skeleton");
		Logging.logVerbose("  Num Skeleton Nodes: " + inModel.mSkeleton.mNodeList.size());
		Logging.logVerbose("   Number of which are Joints: " + SkeletonBuilder.GetNumberOfJoints(inModel));
		Logging.logVerbose(" Number of Meshes: " + inModel.mMeshTable.size());
		for (CSModelMesh mesh: inModel.mMeshTable.values())
		{
			Logging.logVerbose(" Mesh " + mesh.mstrName);
			Logging.logVerbose("  Number of Vertices: " + mesh.mVertexList.size());
			Logging.logVerbose("  Number of Indices: " + mesh.mIndexList.size());
			Logging.logVerbose("  Texture: " + mesh.mstrTextureName);
			Logging.logVerbose("  Min Boundary: (" + mesh.mvMin.x + ", " + mesh.mvMin.y + ", " + mesh.mvMin.z + ")");
			Logging.logVerbose("  Max Boundary: (" + mesh.mvMax.x + ", " + mesh.mvMax.y + ", " + mesh.mvMax.z + ")");
			Logging.logVerbose("  Material");
			Logging.logVerbose("   Emissive: (" + mesh.mMaterial.mvEmissive.x + ", " + mesh.mMaterial.mvEmissive.y + ", " + mesh.mMaterial.mvEmissive.z + ", " + mesh.mMaterial.mvEmissive.w + ")");
			Logging.logVerbose("   Ambient: (" + mesh.mMaterial.mvAmbient.x + ", " + mesh.mMaterial.mvAmbient.y + ", " + mesh.mMaterial.mvAmbient.z + ", " + mesh.mMaterial.mvAmbient.w + ")");
			Logging.logVerbose("   Diffuse: (" + mesh.mMaterial.mvDiffuse.x + ", " + mesh.mMaterial.mvDiffuse.y + ", " + mesh.mMaterial.mvDiffuse.z + ", " + mesh.mMaterial.mvDiffuse.w + ")");
			Logging.logVerbose("   Specular: (" + mesh.mMaterial.mvSpecular.x + ", " + mesh.mMaterial.mvSpecular.y + ", " + mesh.mMaterial.mvSpecular.z + ", " + mesh.mMaterial.mvSpecular.w + ")");
			Logging.logVerbose("   Shininess: " + mesh.mMaterial.mfShininess);
		}
	}
	//-------------------------------------------------------------------
	/// Output Info On Output
	///
	/// Output data on the outputted file.
	//-------------------------------------------------------------------
	private void OutputInfoOnOutput(CSModelConversionParameters inParams)
	{
		Logging.logVerbose("Successfully created " + inParams.mstrOutputFilepath);
		Logging.logVerbose(" Features");
		Logging.logVerbose("  Has Animation Data: " + Boolean.toString(inParams.mbHasAnimationData));
		Logging.logVerbose(" Vertex Declaration");
		Logging.logVerbose("  Position: " + Boolean.toString(inParams.mbVertexHasPosition));
		Logging.logVerbose("  Normal: " + Boolean.toString(inParams.mbVertexHasNormal));
		Logging.logVerbose("  Texture Coordinates: " + Boolean.toString(inParams.mbVertexHasTexCoords));
		Logging.logVerbose("  Colour: " + Boolean.toString(inParams.mbVertexHasColour));
		Logging.logVerbose("  Weights: " + Boolean.toString(inParams.mbVertexHasWeights));
		Logging.logVerbose("  Joint Indices: " + Boolean.toString(inParams.mbVertexHasJointIndices));
		Logging.logVerbose(" Transforms");
		Logging.logVerbose("  Flip vertical texture coordinates: " + Boolean.toString(inParams.mbFlipVerticalTexCoords));
		Logging.logVerbose("  Swap Y and Z: " + Boolean.toString(inParams.mbSwapYAndZ));
		Logging.logVerbose(" ");
	}
}
