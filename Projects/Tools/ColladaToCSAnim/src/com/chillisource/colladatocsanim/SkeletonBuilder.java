package com.chillisource.colladatocsanim;

import java.util.Collections;
import java.util.LinkedList;

import com.chillisource.colladatocsanim.csanim.*;
import com.chillisource.toolutils.Matrix4;
import com.chillisource.toolutils.Logging;
import com.taggames.colladaparserutils.colladadata.*;
import com.taggames.colladaparserutils.colladadata.ColladaNode.COLLADA_NODE_TYPE;

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
		
		newSkeletonNode.mInitialPoseMatrix = Matrix4.createFrom2DArray(inNode.mMatrix.maafValues);
		
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