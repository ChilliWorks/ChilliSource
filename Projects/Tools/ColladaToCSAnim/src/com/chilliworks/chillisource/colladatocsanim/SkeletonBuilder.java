/**
 * SkeletonBuilder.java
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

package com.chilliworks.chillisource.colladatocsanim;

import java.util.Collections;
import java.util.LinkedList;

import com.chilliworks.chillisource.colladaparserutils.colladadata.*;
import com.chilliworks.chillisource.colladaparserutils.colladadata.ColladaNode.COLLADA_NODE_TYPE;
import com.chilliworks.chillisource.colladatocsanim.csanim.*;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.Matrix4;

public class SkeletonBuilder 
{
	//-------------------------------------------------------------------
	/// BuildSkeleton
	///
	/// Converts the input collections of data from the collada file into
	/// something useable for outputing in MoAnim format.
	//-------------------------------------------------------------------
	static public void BuildSkeleton(LinkedList<String> inNodesToExport, Collada inCollada, CSAnim inMoAnim)
	{
		//build the skeleton
		for (ColladaNode node : inCollada.mLibraryVisualScenes.get(inCollada.mScene.mInstanceVisualScene.mstrUrl.substring(1)).mRootNodes.values())
		{
			TryBuildSkeletonForNode(inCollada, inMoAnim, node, inNodesToExport, false);
		}
	}
	//-------------------------------------------------------------------
	/// TryBuildSkeletonForNode
	///
	/// Builds the model data for a node if its parent was build, or if 
	/// its id is in the list of nodes to export.
	//-------------------------------------------------------------------
	static private void TryBuildSkeletonForNode(Collada inCollada, CSAnim inAnim, ColladaNode inNode,LinkedList<String> inNodesToExport, boolean inbParentExported)
	{
		boolean bShouldExport = inbParentExported;
		
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
		if (bShouldExport == true && inNode.meType == ColladaNode.COLLADA_NODE_TYPE.CONTROLLER)
		{
			String skeletonName = inNode.mInstanceController.mSkeleton.mstrValue.substring(1);

			//export this node if possible
			if (skeletonName.length() > 0)
			{
					TryBuildSkeleton(new LinkedList<ColladaNode>(
							inCollada.mLibraryVisualScenes.get(inCollada.mScene.mInstanceVisualScene.mstrUrl.substring(1)).mRootNodes.values()),
							skeletonName, inAnim);
			}
		
		}
		
		//try children
		for (ColladaNode node: inNode.mChildNodes.values())
		{
			TryBuildSkeletonForNode(inCollada, inAnim, node, inNodesToExport, bShouldExport);
		}
	}
	//-------------------------------------------------------------------
	/// Try Build Skeleton
	///
	/// Tries to build the skeleton if it has not already been built. 
	/// If it has already been built, this then confirms that it is using 
	/// the same skeleton.
	//-------------------------------------------------------------------
	static public void TryBuildSkeleton(LinkedList<ColladaNode> inNodes, String instrSkeletonName, CSAnim inModel)
	{
		if (inModel.mSkeleton.mbLocked == false)
		{
			BuildSkeleton(inNodes, instrSkeletonName, inModel);
			OrderSkeleton(inModel);
			inModel.mSkeleton.mbLocked = true;
		}
		else
		{
			CSAnim testModel = new CSAnim();
			BuildSkeleton(inNodes, instrSkeletonName, testModel);
			OrderSkeleton(testModel);
			
			if (CompareSkeletons(inModel.mSkeleton, testModel.mSkeleton) == false)
			{
				String strRootNode1 = "", strRootNode2 = "";
				if (inModel.mSkeleton.mNodeList.size() > 0)
					strRootNode1 = inModel.mSkeleton.mNodeList.get(0).mstrName;
				if (testModel.mSkeleton.mNodeList.size() > 0)
					strRootNode2 = testModel.mSkeleton.mNodeList.get(0).mstrName;
				Logging.logFatal("A second skeleton has been found. All meshes must be attached to the same skeleton. Attaching a mesh to a subset of the skeleton counts as a different skeleton. The two different skeleton root nodes are: '" + strRootNode1 + "' and '" + strRootNode2 + "'");
			}
		}
	}
	//-------------------------------------------------------------------
	/// Compare Skeletons
	///
	/// Compares the two skeletons to see if all relevent data in them 
	/// is the same.
	//-------------------------------------------------------------------
	static private boolean CompareSkeletons(CSAnimSkeleton inSkeleton1, CSAnimSkeleton inSkeleton2)
	{
		//check the list counts are the same
		if (inSkeleton1.mNodeList.size() != inSkeleton2.mNodeList.size())
			return false;
		
		//check all the skeleton nodes
		for (int i = 0; i < inSkeleton1.mNodeList.size(); i++)
		{
			CSAnimSkeletonNode node1 = inSkeleton1.mNodeList.get(i);
			CSAnimSkeletonNode node2 = inSkeleton2.mNodeList.get(i);
			
			if (node1.mstrName.equals(node2.mstrName) == false)
				return false;
			if (node1.mdwParentNodeIndex != node2.mdwParentNodeIndex)
				return false;
		}
		
		return true;
	}
	//-------------------------------------------------------------------
	/// Build Skeleton
	///
	/// builds to the skeleton.
	//-------------------------------------------------------------------
	static private boolean BuildSkeleton(LinkedList<ColladaNode> inNodes, String instrSkeletonName, CSAnim inModel)
	{
		//find the "root" node that we are using
		for (ColladaNode node : inNodes)
		{
			if (node.mstrId.equals(instrSkeletonName))
			{
				//build skeleton
				AddToSkeleton(node, null, inModel);
				return true;
			}
			else
			{
				if (BuildSkeleton(new LinkedList<ColladaNode>(node.mChildNodes.values()), instrSkeletonName, inModel) == true)
				{
					return true;
				}
			}
		}
		
		return false;
	}
	//-------------------------------------------------------------------
	/// Add To Skeleton
	///
	/// Adds a node tree to the skeleton.
	//-------------------------------------------------------------------
	static private void AddToSkeleton(ColladaNode inNode, ColladaNode inParent, CSAnim inModel)
	{
		//if its a skeleton node, then add it to the skeleton node list
		if (inNode.meType == COLLADA_NODE_TYPE.JOINT || inNode.meType == COLLADA_NODE_TYPE.BASE)
		{
			TryAddSkeletonNode(inNode, inParent, inModel);
			for (ColladaNode node: inNode.mChildNodes.values())
			{
				AddToSkeleton(node, inNode, inModel);
			}
		}
	}
	//-------------------------------------------------------------------
	/// Try Add Skeleton Node
	///
	/// Tries to add a joint to the skeleton.
	//-------------------------------------------------------------------
	static private void TryAddSkeletonNode(ColladaNode inNode, ColladaNode inParent, CSAnim inModel)
	{
		//check and see if this node already exists
		for (int i = 0; i < inModel.mSkeleton.mNodeList.size(); i++)
		{
			CSAnimSkeletonNode node = inModel.mSkeleton.mNodeList.get(i);
			if (node.mstrId.equalsIgnoreCase(inNode.mstrId))
			{
				//if the node does already exist, then make sure the parent is correct
				if (node.mstrParentId.length() == 0 && inParent != null)
				{
					node.mstrParentId = inParent.mstrId;
				}
				return;
			}
		}
		
		//create a new skeleton node
		CSAnimSkeletonNode newSkeletonNode = new CSAnimSkeletonNode();
		newSkeletonNode.mstrName = inNode.mstrName;
		newSkeletonNode.mstrId = inNode.mstrId;
		if (inParent != null)
		{
			newSkeletonNode.mstrParentId = inParent.mstrId;
		}
		
		float[][] matData = inNode.mMatrix.maafValues;
		
		newSkeletonNode.mInitialPoseMatrix = new Matrix4(matData[0][0], matData[1][0], matData[2][0], matData[3][0],
			matData[0][1], matData[1][1], matData[2][1], matData[3][1],
			matData[0][2], matData[1][2], matData[2][2], matData[3][2],
			matData[0][3], matData[1][3], matData[2][3], matData[3][3]);
		
		//add the joint
		inModel.mSkeleton.mNodeList.add(newSkeletonNode);
	}
	//-------------------------------------------------------------------
	/// Order Skeleton
	///
	/// Orders the skeleton based on the the id.
	//-------------------------------------------------------------------
	static private void OrderSkeleton(CSAnim inModel)
	{
		Collections.sort(inModel.mSkeleton.mNodeList, new SkeletonNodeComparator());
		
		//iterator the list and get the parent indices
		for (CSAnimSkeletonNode node : inModel.mSkeleton.mNodeList)
		{
			if(node.mstrParentId.length() > 0)
			{
				node.mdwParentNodeIndex = GetIndexOfSkeletonNodeById(inModel, node.mstrParentId);
			}
			else
			{
				node.mdwParentNodeIndex = -1;
			}
		}
	}
	//-------------------------------------------------------------------
	/// Get Index Of Skeleton Node By Id
	///
	/// @return The index of the skeleton node with the passed in id.
	//-------------------------------------------------------------------
	static public int GetIndexOfSkeletonNodeById(CSAnim inModel, String instrId)
	{
		for (int i = 0; i < inModel.mSkeleton.mNodeList.size(); i++)
		{
			CSAnimSkeletonNode node = inModel.mSkeleton.mNodeList.get(i);
			if (node.mstrId.equalsIgnoreCase(instrId))
			{
				return i;
			}
		}
		
		Logging.logError("Cannot find skeleton node with ID: " + instrId);
		return -1;
	}
}