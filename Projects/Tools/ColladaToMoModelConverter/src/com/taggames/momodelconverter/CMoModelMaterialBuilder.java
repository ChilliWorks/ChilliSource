//
// CMoModelMaterialBuilder.h
// ColladaToMoModelConverter
//
// Created by Ian Copland on 15/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.momodelconverter;

import java.util.LinkedList;

import com.taggames.momodelconverter.momodel.MoModel;
import com.taggames.momodelconverter.momodel.MoModelMaterial;
import com.taggames.momodelconverter.momodel.MoModelMesh;
import com.taggames.toolutils.*;

public class CMoModelMaterialBuilder 
{
	//-------------------------------------------------------------------
	/// Private Member Data
	//-------------------------------------------------------------------
	CLittleEndianOutputStream mStream;
	//-------------------------------------------------------------------
	/// Constructor
	//-------------------------------------------------------------------
	public CMoModelMaterialBuilder() 
	{
		
	}
	//-------------------------------------------------------------------
	/// Output Materials For Model
	///
	/// Outputs all the materials associated with the model. They will 
	/// be placed alongside the model output file.
	//-------------------------------------------------------------------
	public void OutputMaterialsForModel(MoModelConversionParameters inConversionParams, MoModel inModel) throws Exception
	{
		//get the output directory.
		String strDirectory = inConversionParams.mstrOutputFilepath.replace('\\', '/');
		int dwLastSlash =  strDirectory.lastIndexOf("/");
		if (dwLastSlash != -1)
			strDirectory = strDirectory.substring(0, dwLastSlash + 1);
		else
			strDirectory = "";
		
		//create a material for each mesh. Note that this will now overwrite duplicate materials. This should probably be changed to just
		//output each material once?
		for (MoModelMesh mesh: inModel.mMeshTable.values())
		{
			//output this material
			OutputMaterial(strDirectory, mesh, inConversionParams);
		}
	}
	//-------------------------------------------------------------------
	/// Output Material
	///
	/// Opens the stream and writes to a single material file.
	//-------------------------------------------------------------------
	private void OutputMaterial(String instrDirectory, MoModelMesh inMesh, MoModelConversionParameters inConversionParams) throws Exception
	{
		String strFilename = instrDirectory + inMesh.mMaterial.mstrName + ".momaterial";
		
		//try and open a new file stream. if this fails, throw an error.
		try
		{
			mStream = new CLittleEndianOutputStream(strFilename);
			
			WriteMaterial(inMesh, inConversionParams);
		}
		catch (Exception e)
		{
			SCLogger.LogError("Creating material failed: " + strFilename);
		}
		finally
		{
			mStream.Close();
		}
	}
	//-------------------------------------------------------------------
	/// Write Material
	///
	/// Fills the output material file with data from the "MoModelMaterial"
	//-------------------------------------------------------------------
	private void WriteMaterial(MoModelMesh inMesh, MoModelConversionParameters inConversionParams) throws Exception
	{
		LinkedList<String> materialLines = new LinkedList<String>();
		
		MoModelMaterial mat = inMesh.mMaterial;
		
		//start defining the material
		materialLines.add("<Material>");
		
		//if it has normals, define the lighting params
		if (inConversionParams.mbVertexHasNormal == true)
		{
			materialLines.add("    <Lighting>");
			materialLines.add("        <Emissive value=\"" + mat.mvEmissive.x + " " + mat.mvEmissive.y + " " + mat.mvEmissive.z + " " + mat.mvEmissive.w + "\" />");
			materialLines.add("        <Ambient value=\"" + mat.mvAmbient.x + " " + mat.mvAmbient.y + " " + mat.mvAmbient.z + " " + mat.mvAmbient.w + "\" />");
			materialLines.add("        <Diffuse value=\"" + mat.mvDiffuse.x + " " + mat.mvDiffuse.y + " " + mat.mvDiffuse.z + " " + mat.mvDiffuse.w + "\" />");
			materialLines.add("        <Specular value=\"" + mat.mvSpecular.x + " " + mat.mvSpecular.y + " " + mat.mvSpecular.z + " " + mat.mvSpecular.w + "\" />");
			materialLines.add("        <Intensity value=\"" + mat.mfShininess + "\" />");
			materialLines.add("    </Lighting>");
		}
		
		//if it has a texture, define the texture name
		if (inConversionParams.mbVertexHasTexCoords == true)
		{
			materialLines.add("    <Textures>");
			materialLines.add("        <Texture location=\"Package\" image-name=\"" + inMesh.mstrTextureName + "\" />");
			materialLines.add("    </Textures>");
		}
		materialLines.add("    <Shader location=\"Package\" file-name=\"Default3D\" />");
		materialLines.add("</Material>");
		
		for (String strLine: materialLines)
		{
			mStream.WriteAsciiString(strLine + "\n");
		}
	}
}
