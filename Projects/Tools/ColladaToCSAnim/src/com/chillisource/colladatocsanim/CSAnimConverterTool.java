/**
 * CSAnimConverterTool.java
 * Chilli Source
 * Created by Ian Copland on 16/01/2013.
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

package com.chillisource.colladatocsanim;

import java.io.FileNotFoundException;
import java.util.LinkedList;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import com.chillisource.colladatocsanim.csanim.*;
import com.chillisource.toolutils.Logging;
import com.taggames.colladaparserutils.ColladaParser;
import com.taggames.colladaparserutils.colladadata.*;

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
		SkeletonBuilder.BuildSkeleton(nodes, colladaData, anim);
		
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
