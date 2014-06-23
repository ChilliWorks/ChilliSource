package com.chillisource.csanimconverter;

import java.io.FileNotFoundException;
import java.util.LinkedList;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import com.chillisource.csanimconverter.csanim.*;
import com.chillisource.toolutils.Logging;
import com.taggames.colladaparser.CColladaParser;
import com.taggames.colladaparser.colladadata.*;

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
			Logging.logFatal("Could not find file: " + inParams.mstrInputFilepath);
		}
		catch (Exception e)
		{
			String strFullStackTrace = new String();
			for (StackTraceElement stackTrace: e.getStackTrace())
				strFullStackTrace += stackTrace.toString() + "\n";
			Logging.logFatal("Exception occurred while parsing the collada file. The following is the exception error message:\n-\n " + strFullStackTrace + "-");
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
			Logging.logError("Output failed!");
		}
	}
	
	/**
	 * Prints info on the output MoStatic mesh.
	 * @param inMeshList
	 */
	private void OutputInfoOnMoAnim(CSAnim inModel)
	{
		Logging.logVerbose("MoAnim Data Generated");
		Logging.logVerbose(" Frame rate: " + inModel.mfFrameRate);
		Logging.logVerbose(" Number of frames: " + inModel.mFrames.size());
		Logging.logVerbose(" Skeleton");
		Logging.logVerbose("  Skeleton Nodes: " + inModel.mSkeleton.mNodeList.size());
	}
	
	/**
	 * output data on the outputted file
	 * @param inParams
	 */
	private void OutputInfoOnOutput(CSAnimConversionParameters inParams)
	{
		Logging.logVerbose("Successfully created " + inParams.mstrOutputFilepath);
		Logging.logVerbose(" Transforms");
		Logging.logVerbose("  Swap Y and Z: " + Boolean.toString(inParams.mbSwapYAndZ));
		Logging.logVerbose(" ");
	}
}
