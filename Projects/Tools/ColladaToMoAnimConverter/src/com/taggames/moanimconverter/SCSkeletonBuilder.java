package com.taggames.moanimconverter;

import java.util.Collections;
import java.util.LinkedList;

import com.taggames.colladaparser.colladadata.*;
import com.taggames.colladaparser.colladadata.ColladaNode.COLLADA_NODE_TYPE;
import com.taggames.moanimconverter.moanim.*;
import com.taggames.toolutils.CMatrix4;
import com.taggames.toolutils.SCLogger;

public class SCSkeletonBuilder 
{
	//-------------------------------------------------------------------
	/// BuildSkeleton
	///
	/// Converts the input collections of data from the collada file into
	/// something useable for outputing in MoAnim format.
	//-------------------------------------------------------------------
	static public void BuildSkeleton(LinkedList<String> inNodesToExport, Collada inCollada, MoAnim inMoAnim)
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
	static private void TryBuildSkeletonForNode(Collada inCollada, MoAnim inAnim, ColladaNode inNode,LinkedList<String> inNodesToExport, boolean inbParentExported)
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
	static public void TryBuildSkeleton(LinkedList<ColladaNode> inNodes, String instrSkeletonName, MoAnim inModel)
	{
		if (inModel.mSkeleton.mbLocked == false)
		{
			BuildSkeleton(inNodes, instrSkeletonName, inModel);
			OrderSkeleton(inModel);
			inModel.mSkeleton.mbLocked = true;
		}
		else
		{
			MoAnim testModel = new MoAnim();
			BuildSkeleton(inNodes, instrSkeletonName, testModel);
			OrderSkeleton(testModel);
			
			if (CompareSkeletons(inModel.mSkeleton, testModel.mSkeleton) == false)
			{
				String strRootNode1 = "", strRootNode2 = "";
				if (inModel.mSkeleton.mNodeList.size() > 0)
					strRootNode1 = inModel.mSkeleton.mNodeList.get(0).mstrName;
				if (testModel.mSkeleton.mNodeList.size() > 0)
					strRootNode2 = testModel.mSkeleton.mNodeList.get(0).mstrName;
				SCLogger.LogFatalError("A second skeleton has been found. All meshes must be attached to the same skeleton. Attaching a mesh to a subset of the skeleton counts as a different skeleton. The two different skeleton root nodes are: '" + strRootNode1 + "' and '" + strRootNode2 + "'");
			}
		}
	}
	//-------------------------------------------------------------------
	/// Compare Skeletons
	///
	/// Compares the two skeletons to see if all relevent data in them 
	/// is the same.
	//-------------------------------------------------------------------
	static private boolean CompareSkeletons(MoAnimSkeleton inSkeleton1, MoAnimSkeleton inSkeleton2)
	{
		//check the list counts are the same
		if (inSkeleton1.mNodeList.size() != inSkeleton2.mNodeList.size())
			return false;
		
		//check all the skeleton nodes
		for (int i = 0; i < inSkeleton1.mNodeList.size(); i++)
		{
			MoAnimSkeletonNode node1 = inSkeleton1.mNodeList.get(i);
			MoAnimSkeletonNode node2 = inSkeleton2.mNodeList.get(i);
			
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
	static private boolean BuildSkeleton(LinkedList<ColladaNode> inNodes, String instrSkeletonName, MoAnim inModel)
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
	static private void AddToSkeleton(ColladaNode inNode, ColladaNode inParent, MoAnim inModel)
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
	static private void TryAddSkeletonNode(ColladaNode inNode, ColladaNode inParent, MoAnim inModel)
	{
		//check and see if this node already exists
		for (int i = 0; i < inModel.mSkeleton.mNodeList.size(); i++)
		{
			MoAnimSkeletonNode node = inModel.mSkeleton.mNodeList.get(i);
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
		MoAnimSkeletonNode newSkeletonNode = new MoAnimSkeletonNode();
		newSkeletonNode.mstrName = inNode.mstrName;
		newSkeletonNode.mstrId = inNode.mstrId;
		if (inParent != null)
		{
			newSkeletonNode.mstrParentId = inParent.mstrId;
		}
		
		newSkeletonNode.mInitialPoseMatrix = CMatrix4.CreateFrom2DArray(inNode.mMatrix.maafValues);
		
		//add the joint
		inModel.mSkeleton.mNodeList.add(newSkeletonNode);
	}
	//-------------------------------------------------------------------
	/// Order Skeleton
	///
	/// Orders the skeleton based on the the id.
	//-------------------------------------------------------------------
	static private void OrderSkeleton(MoAnim inModel)
	{
		Collections.sort(inModel.mSkeleton.mNodeList, new SkeletonNodeComparator());
		
		//iterator the list and get the parent indices
		for (MoAnimSkeletonNode node : inModel.mSkeleton.mNodeList)
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
	static public int GetIndexOfSkeletonNodeById(MoAnim inModel, String instrId)
	{
		for (int i = 0; i < inModel.mSkeleton.mNodeList.size(); i++)
		{
			MoAnimSkeletonNode node = inModel.mSkeleton.mNodeList.get(i);
			if (node.mstrId.equalsIgnoreCase(instrId))
			{
				return i;
			}
		}
		
		SCLogger.LogError("Cannot find skeleton node with ID: " + instrId);
		return -1;
	}
}