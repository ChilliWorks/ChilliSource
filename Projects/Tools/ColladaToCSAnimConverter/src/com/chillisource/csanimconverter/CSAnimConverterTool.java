package com.chillisource.csanimconverter;

import java.io.FileNotFoundException;
import java.util.LinkedList;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import com.chillisource.csanimconverter.csanim.*;
import com.taggames.colladaparser.CColladaParser;
import com.taggames.colladaparser.colladadata.*;
import com.taggames.toolutils.SCLogger;

public class CSAnimConverterTool 
{
	public CSAnimConverterTool()
	{
		
	}
	
	public void Convert(CSAnimConversionParameters inParams) throws Exception
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
		CSAnim anim = new CSAnim();
		
		//build the skeleton for reference
		SCSkeletonBuilder.BuildSkeleton(nodes, colladaData, anim);
		
		//build the animation data
		CSAnimConverter converter = new CSAnimConverter(); 
		converter.ConvertToMoAnimFormat(nodes, colladaData, anim);
		
		//Modify the data to the intended output format
		CSAnimTransformer modifier = new CSAnimTransformer(); 
		modifier.Modify(inParams, anim);
			
		//output info on the MoAnim
		OutputInfoOnMoAnim(anim);

		//output MoStatic file
		CSAnimOutputer outputer = new CSAnimOutputer();
		
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
	private void OutputInfoOnMoAnim(CSAnim inModel)
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
	private void OutputInfoOnOutput(CSAnimConversionParameters inParams)
	{
		SCLogger.LogMessage("Successfully created " + inParams.mstrOutputFilepath);
		SCLogger.LogMessage(" Transforms");
		SCLogger.LogMessage("  Swap Y and Z: " + Boolean.toString(inParams.mbSwapYAndZ));
		SCLogger.LogMessage(" ");
	}
}
