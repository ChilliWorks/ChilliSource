/**
 * ColladaToCSModel.java
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
import com.chilliworks.chillisource.coreutils.*;

/**
 * Provides methods for converting a Collada model file to CSModel.
 * 
 * @author Ian Copland
 */
public final class ColladaToCSModel 
{
	/**
	 * Converts from a Collada model file to CSModel.
	 * 
	 * author Ian Copland
	 * 
	 * @param in_options - The options.
	 * 
	 * @throws Exception
	 */
	public static void convert(ColladaToCSModelOptions inParams) throws Exception
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
			saxParser.parse(inParams.m_inputFilePath, handler);
		}
		catch (FileNotFoundException e)
		{
			Logging.logFatal("Could not find file: " + inParams.m_inputFilePath);
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
		CSModelTransformer.transform(inParams, model);
			
		//output info on the mo model mesh
		outputInfoOnMoModel(model);

		//output MoStatic file
		CSModelOutputer outputer = new CSModelOutputer();
		
		if (outputer.Output(inParams, model) == true)
		{
			//output info on output
			outputInfoOnOutput(inParams);
		}
		else
		{
			Logging.logError("Output failed!");
		}
	}
	/**
	 * Prints information on the output mesh.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options.
	 */
	private static void outputInfoOnMoModel(CSModel in_options)
	{
		Logging.logVerbose("MoModel Model Generated.");
		Logging.logVerbose(" Min Boundary: (" + in_options.mvMin.getX() + ", " + in_options.mvMin.getY() + ", " + in_options.mvMin.getZ() + ")");
		Logging.logVerbose(" Max Boundary: (" + in_options.mvMax.getX() + ", " + in_options.mvMax.getY() + ", " + in_options.mvMax.getZ() + ")");
		Logging.logVerbose(" Skeleton");
		Logging.logVerbose("  Num Skeleton Nodes: " + in_options.mSkeleton.mNodeList.size());
		Logging.logVerbose("   Number of which are Joints: " + SkeletonBuilder.GetNumberOfJoints(in_options));
		Logging.logVerbose(" Number of Meshes: " + in_options.mMeshTable.size());
		for (CSModelMesh mesh: in_options.mMeshTable.values())
		{
			Logging.logVerbose(" Mesh " + mesh.mstrName);
			Logging.logVerbose("  Number of Vertices: " + mesh.mVertexList.size());
			Logging.logVerbose("  Number of Indices: " + mesh.mIndexList.size());
			Logging.logVerbose("  Texture: " + mesh.mstrTextureName);
			Logging.logVerbose("  Min Boundary: (" + mesh.mvMin.getX() + ", " + mesh.mvMin.getY() + ", " + mesh.mvMin.getZ() + ")");
			Logging.logVerbose("  Max Boundary: (" + mesh.mvMax.getX() + ", " + mesh.mvMax.getY() + ", " + mesh.mvMax.getZ() + ")");
			Logging.logVerbose("  Material");
			Logging.logVerbose("   Emissive: (" + mesh.mMaterial.mvEmissive.getX() + ", " + mesh.mMaterial.mvEmissive.getY() + ", " + mesh.mMaterial.mvEmissive.getZ() + ", " + mesh.mMaterial.mvEmissive.getW() + ")");
			Logging.logVerbose("   Ambient: (" + mesh.mMaterial.mvAmbient.getX() + ", " + mesh.mMaterial.mvAmbient.getY() + ", " + mesh.mMaterial.mvAmbient.getZ() + ", " + mesh.mMaterial.mvAmbient.getW() + ")");
			Logging.logVerbose("   Diffuse: (" + mesh.mMaterial.mvDiffuse.getX() + ", " + mesh.mMaterial.mvDiffuse.getY() + ", " + mesh.mMaterial.mvDiffuse.getZ() + ", " + mesh.mMaterial.mvDiffuse.getW() + ")");
			Logging.logVerbose("   Specular: (" + mesh.mMaterial.mvSpecular.getX() + ", " + mesh.mMaterial.mvSpecular.getY() + ", " + mesh.mMaterial.mvSpecular.getZ() + ", " + mesh.mMaterial.mvSpecular.getW() + ")");
			Logging.logVerbose("   Shininess: " + mesh.mMaterial.mfShininess);
		}
	}
	/**
	 * Prints information on the output file.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_options - The options.
	 */
	private static void outputInfoOnOutput(ColladaToCSModelOptions in_options)
	{
		Logging.logVerbose("Successfully created " + in_options.m_outputFilePath);
		Logging.logVerbose(" Features");
		Logging.logVerbose("  Has Animation Data: " + Boolean.toString(in_options.m_animated));
		Logging.logVerbose(" Vertex Declaration");
		Logging.logVerbose("  Position: " + Boolean.toString(in_options.m_vertexHasPosition));
		Logging.logVerbose("  Normal: " + Boolean.toString(in_options.m_vertexHasNormal));
		Logging.logVerbose("  Texture Coordinates: " + Boolean.toString(in_options.m_vertexHasTexCoords));
		Logging.logVerbose("  Colour: " + Boolean.toString(in_options.m_vertexHasColour));
		Logging.logVerbose("  Weights: " + Boolean.toString(in_options.m_vertexHasWeights));
		Logging.logVerbose("  Joint Indices: " + Boolean.toString(in_options.m_vertexHasJointIndices));
		Logging.logVerbose(" Transforms");
		Logging.logVerbose("  Flip vertical texture coordinates: " + Boolean.toString(in_options.m_flipVerticalTexCoords));
		Logging.logVerbose("  Swap Y and Z: " + Boolean.toString(in_options.m_swapYAndZ));
		Logging.logVerbose(" ");
	}
}
