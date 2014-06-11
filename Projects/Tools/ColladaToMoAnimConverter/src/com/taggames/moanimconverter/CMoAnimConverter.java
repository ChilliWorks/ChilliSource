package com.taggames.moanimconverter;

import java.util.LinkedList;


import com.taggames.colladaparser.colladadata.*;
import com.taggames.moanimconverter.moanim.*;
import com.taggames.toolutils.CMatrix4;
import com.taggames.toolutils.CQuaternion;
import com.taggames.toolutils.CVector3;
import com.taggames.toolutils.SCLogger;

public class CMoAnimConverter 
{
	/**
	 * private data
	 */
	Animation[] mAnimations;
	
	/**
	 * Constructor
	 */
	public CMoAnimConverter()
	{
		
	}
	
	/**
	 * Converts the input collections of data from the collada file into something useable for outputing in MoAnim format.
	 * @param inMeshList
	 * @param inTextureList
	 */
	public void ConvertToMoAnimFormat(LinkedList<String> inNodesToExport, Collada inCollada, MoAnim inMoAnim)
	{
		mAnimations = new Animation[inMoAnim.mSkeleton.mNodeList.size()];
		
		for (int i = 0; i < inMoAnim.mSkeleton.mNodeList.size(); i++)
			mAnimations[i] = new Animation();
		
		LinkedList<String> allowedNodes = new LinkedList<String>();
		
		//build the list of acceptable nodes
		for (ColladaNode node : inCollada.mLibraryVisualScenes.get(inCollada.mScene.mInstanceVisualScene.mstrUrl.substring(1)).mRootNodes.values())
		{
			TryAddToAllowedList(node, inNodesToExport, allowedNodes, false);
		}
		
		//iterate through all animation channels
		for (ColladaChannel channel: inCollada.mAnimation.mChannelList)
		{
			TryAddChannel(inCollada, inMoAnim, allowedNodes, channel);
		}
		
		//convert the animation data, intro frame data.
		ConvertAnimationsToFrames(inMoAnim);
	}
	
	/**
	 * Adds to the list of node names that are allowed. Only animation on nodes contained in this list will be outputted.
	 * @param inNode
	 * @param inNodesToExport
	 * @param inAllowedNodes
	 */
	private void TryAddToAllowedList(ColladaNode inNode, LinkedList<String> inNodesToExport, LinkedList<String> inAllowedNodes, boolean inbParentExported)
	{
		boolean bShouldAdd = inbParentExported;
		
		//if the parent was not exported, check and see if this node should be
		if (bShouldAdd == false)
		{
			for (String nodeName: inNodesToExport)
			{
				if (nodeName.equalsIgnoreCase(inNode.mstrId) == true)
				{
					bShouldAdd = true;
					break;
				}
			}
		}
		
		//if it should be added, then add it!
		if (bShouldAdd == true)
		{
			inAllowedNodes.add(inNode.mstrId);
		}
		
		//try children
		for (ColladaNode node: inNode.mChildNodes.values())
		{
			TryAddToAllowedList(node, inNodesToExport, inAllowedNodes, bShouldAdd);
		}
	}
	
	/**
	 * Adds the animation described by the input channel if the channel is targeting an allowed node.
	 * @param inCollada
	 * @param inAllowedNodes
	 * @param inChannel
	 */
	private void TryAddChannel(Collada inCollada, MoAnim inMoAnim, LinkedList<String> inAllowedNodes, ColladaChannel inChannel)
	{
		//get the targeted node name.
		String[] strSplitTarget = inChannel.mstrTarget.split("/");
		String strTargetNode = strSplitTarget[0];
		
		for (String nodeName: inAllowedNodes)
		{
			if (nodeName.equalsIgnoreCase(strTargetNode) == true)
			{
				//if this channel should indeed be used then get the sampler
				ColladaSampler sampler = inCollada.mAnimation.mSamplerTable.get(inChannel.mstrSource.substring(1));
				
				//use the sampler to get the sources
				ColladaSource timelineSource = inCollada.mAnimation.mSourceTable.get(sampler.mInputTable.get("INPUT").mstrSource.substring(1));
				ColladaSource transformSource = inCollada.mAnimation.mSourceTable.get(sampler.mInputTable.get("OUTPUT").mstrSource.substring(1));
				ColladaSource interpolationSource = inCollada.mAnimation.mSourceTable.get(sampler.mInputTable.get("INTERPOLATION").mstrSource.substring(1));

				//get the skeleton node name
				ColladaNode node = GetNodeWithID(new LinkedList<ColladaNode>(
						inCollada.mLibraryVisualScenes.get(inCollada.mScene.mInstanceVisualScene.mstrUrl.substring(1)).mRootNodes.values()), 
						strTargetNode);
				
				//if this node is indeed a skeleton node, then add the animation for it.
				if (node != null && node.mstrId.length() > 0)
					AddAnimation(inMoAnim, timelineSource, transformSource, interpolationSource, node.mstrId);
				
				return;
			}
		}
	}
	
	/**
	 * Adds an animation to the "animation" list. This list is then used to build the frame data for outputting in MoAnim.
	 * @param inTimelineSource
	 * @param inTransformSource
	 * @param inInterpolationSource
	 */
	public void AddAnimation(MoAnim inMoAnim, ColladaSource inTimelineSource, ColladaSource inTransformSource, ColladaSource inInterpolationSource, String instrSkeletonNodeId)
	{
		Animation anim = new Animation();
		
		//confirm that all interpolations are linear, throw an error if they are not
		for (String interpType: inInterpolationSource.mNameArray.mstrData)
		{
			if (interpType.equals("LINEAR") == false)
			{
				SCLogger.LogFatalError("Unsupported Interpolation type: " + interpType);
			}
		}
		
		//get the frame rate
		if (inTimelineSource.mFloatArray.mdwCount > 1)
		{
			anim.mfFrameRate = inTimelineSource.mFloatArray.mafData[1] - inTimelineSource.mFloatArray.mafData[0];
		}
		
		//get the index of the skeleton node this is paired with.
		int index = SCSkeletonBuilder.GetIndexOfSkeletonNodeById(inMoAnim, instrSkeletonNodeId);
		
		if(index == -1)
			return;
		
		//get all the matrix data
		float[] m = inTransformSource.mFloatArray.mafData;
		for (int i = 0; i < inTransformSource.mFloatArray.mdwCount / 16; i++)
		{
			//create new matrix from the float array
			CMatrix4 mat = new CMatrix4();
			mat.mafData[0] = m[i*16 + 0];		mat.mafData[1] = m[i*16 + 4];		mat.mafData[2] = m[i*16 + 8];		mat.mafData[3] = m[i*16 + 12];
			mat.mafData[4] = m[i*16 + 1];		mat.mafData[5] = m[i*16 + 5];		mat.mafData[6] = m[i*16 + 9];		mat.mafData[7] = m[i*16 + 13];
			mat.mafData[8] = m[i*16 + 2];		mat.mafData[9] = m[i*16 + 6];		mat.mafData[10] = m[i*16 + 10];		mat.mafData[11] = m[i*16 + 14];
			mat.mafData[12] = m[i*16 + 3];		mat.mafData[13] = m[i*16 + 7];		mat.mafData[14] = m[i*16 + 11];		mat.mafData[15] = m[i*16 + 15];
			
			//add matrix to animation
			anim.mTransformList.add(mat);
		}

		//add anim to anim array
		mAnimations[index] = anim;
	}
	
	/**
	 * Gets the collada node with the passed in ID
	 * @param inCollada
	 * @param strTargetNode
	 * @return
	 */
	private ColladaNode GetNodeWithID(LinkedList<ColladaNode> inNodeList, String strTargetNode)
	{
		for (ColladaNode node:	inNodeList)
		{
			if (node.mstrId.equals(strTargetNode) == true)
				return node;
			
			ColladaNode childNode = GetNodeWithID(new LinkedList<ColladaNode>(node.mChildNodes.values()), strTargetNode);
			
			if (childNode != null)
				return childNode;
		}
		
		return null;
	}
	
	/**
	 * Convert the Animation data into frame data that can be used for outputting in MoAnim
	 * @param inAnim
	 */
	private void ConvertAnimationsToFrames(MoAnim inAnim)
	{
		if (mAnimations.length > 0 && mAnimations.length == inAnim.mSkeleton.mNodeList.size())
		{
			//get the first non 0 framerate
			for (int i = 0; i < mAnimations.length; i++)
			{
				if (mAnimations[i].mfFrameRate != 0.0f)
				{
					inAnim.mfFrameRate = mAnimations[i].mfFrameRate;
					break;
				}
			}
			
			//get the number of frames
			int dwNumFrames = 0;
			for (int i = 0; i < mAnimations.length; i++)
			{
				if (mAnimations[i].mTransformList.size() != 0)
				{
					dwNumFrames = mAnimations[i].mTransformList.size();
					break;
				}
			}
			
			//iterate through each frame
			for (int i = 0; i < dwNumFrames; i++)
			{
				MoAnimFrame frame = new MoAnimFrame();
				
				for (int j = 0; j < mAnimations.length; j++)
				{
					CMatrix4 transform = new CMatrix4();
					if (mAnimations[j].mTransformList.size() > 0)
						transform = mAnimations[j].mTransformList.get(i);
					else
						transform = inAnim.mSkeleton.mNodeList.get(j).mInitialPoseMatrix;
					
					CVector3 translation = new CVector3();
					CVector3 scale = new CVector3();
					CQuaternion rot = new CQuaternion();
					
					transform.DecomposeTransforms(translation, scale, rot);
					
					frame.mNodeTranslations.add(translation);
					frame.mNodeOrienations.add(rot.Normalise());
					frame.mNodeScalings.add(scale);
				}
				
				inAnim.mFrames.add(frame);
			}
		}
		else
		{
			SCLogger.LogFatalError("Animation and skeleton node counts do not match!");
		}
	}
}
















