package com.taggames.moanimconverter;

import java.io.FileNotFoundException;
import java.util.LinkedList;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import com.taggames.colladaparser.CColladaParser;
import com.taggames.colladaparser.colladadata.*;
import com.taggames.moanimconverter.moanim.*;
import com.taggames.toolutils.SCLogger;

public class CMoAnimConverterTool 
{
	public CMoAnimConverterTool()
	{
		
	}
	
	public void Convert(MoAnimConversionParameters inParams) throws Exception
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
			String strFullStackTrace = new String();
			for (StackTraceElement stackTrace: e.getStackTrace())
				strFullStackTrace += stackTrace.toString() + "\n";
			SCLogger.LogFatalError("Exception occurred while parsing the collada file. The following is the exception error message:\n-\n " + strFullStackTrace + "-");
		}
		
		//get the nodes to be exported
		LinkedList<String> nodes = new LinkedList<String>();
		for (ColladaNode node : colladaData.mLibraryVisualScenes.get(colladaData.mScene.mInstanceVisualScene.mstrUrl.substring(1)).mRootNodes.values())
		{
			nodes.add(node.mstrId);
		}
		
		//create the new animation file
		MoAnim anim = new MoAnim();
		
		//build the skeleton for reference
		SCSkeletonBuilder.BuildSkeleton(nodes, colladaData, anim);
		
		//build the animation data
		CMoAnimConverter converter = new CMoAnimConverter(); 
		converter.ConvertToMoAnimFormat(nodes, colladaData, anim);
		
		//Modify the data to the intended output format
		CMoAnimTransformer modifier = new CMoAnimTransformer(); 
		modifier.Modify(inParams, anim);
			
		//output info on the MoAnim
		OutputInfoOnMoAnim(anim);

		//output MoStatic file
		CMoAnimOutputer outputer = new CMoAnimOutputer();
		
		if (outputer.Output(inParams, anim) == true)
		{
			//output info on output
			OutputInfoOnOutput(inParams);
		}
		else
		{
			SCLogger.LogError("Output failed!");
		}
	}
	
	/**
	 * Prints info on the output MoStatic mesh.
	 * @param inMeshList
	 */
	private void OutputInfoOnMoAnim(MoAnim inModel)
	{
		SCLogger.LogMessage("MoAnim Data Generated");
		SCLogger.LogMessage(" Frame rate: " + inModel.mfFrameRate);
		SCLogger.LogMessage(" Number of frames: " + inModel.mFrames.size());
		SCLogger.LogMessage(" Skeleton");
		SCLogger.LogMessage("  Skeleton Nodes: " + inModel.mSkeleton.mNodeList.size());
	}
	
	/**
	 * output data on the outputted file
	 * @param inParams
	 */
	private void OutputInfoOnOutput(MoAnimConversionParameters inParams)
	{
		SCLogger.LogMessage("Successfully created " + inParams.mstrOutputFilepath);
		SCLogger.LogMessage(" Transforms");
		SCLogger.LogMessage("  Mirror in the XZ plane: " + Boolean.toString(inParams.mbMirrorInXZPlane));
		SCLogger.LogMessage("  Swap Y and Z: " + Boolean.toString(inParams.mbSwapYAndZ));
		SCLogger.LogMessage(" ");
	}
}
