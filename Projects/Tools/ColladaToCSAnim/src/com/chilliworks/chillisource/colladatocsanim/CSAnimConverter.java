/**
 * CSAnimConverter.java
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

import java.util.LinkedList;

import com.chilliworks.chillisource.colladaparserutils.colladadata.*;
import com.chilliworks.chillisource.colladatocsanim.csanim.*;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.Matrix4;
import com.chilliworks.chillisource.coreutils.Quaternion;
import com.chilliworks.chillisource.coreutils.Tuple3;
import com.chilliworks.chillisource.coreutils.Vector3;

public class CSAnimConverter 
{
	/**
	 * private data
	 */
	Animation[] mAnimations;
	
	/**
	 * Constructor
	 */
	public CSAnimConverter()
	{
		
	}
	
	/**
	 * Converts the input collections of data from the collada file into something useable for outputing in MoAnim format.
	 * @param inMeshList
	 * @param inTextureList
	 */
	public void ConvertToMoAnimFormat(LinkedList<String> inNodesToExport, Collada inCollada, CSAnim inMoAnim)
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
	private void TryAddChannel(Collada inCollada, CSAnim inMoAnim, LinkedList<String> inAllowedNodes, ColladaChannel inChannel)
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
	public void AddAnimation(CSAnim inMoAnim, ColladaSource inTimelineSource, ColladaSource inTransformSource, ColladaSource inInterpolationSource, String instrSkeletonNodeId)
	{
		Animation anim = new Animation();
		
		//confirm that all interpolations are linear, throw an error if they are not
		for (String interpType: inInterpolationSource.mNameArray.mstrData)
		{
			if (interpType.equals("LINEAR") == false)
			{
				Logging.logFatal("Unsupported Interpolation type: " + interpType);
			}
		}
		
		//get the frame rate
		if (inTimelineSource.mFloatArray.mdwCount > 1)
		{
			anim.mfFrameRate = inTimelineSource.mFloatArray.mafData[1] - inTimelineSource.mFloatArray.mafData[0];
		}
		
		//get the index of the skeleton node this is paired with.
		int index = SkeletonBuilder.GetIndexOfSkeletonNodeById(inMoAnim, instrSkeletonNodeId);
		
		if(index == -1)
			return;
		
		//get all the matrix data
		float[] m = inTransformSource.mFloatArray.mafData;
		for (int i = 0; i < inTransformSource.mFloatArray.mdwCount / 16; i++)
		{
			//create new matrix from the float array
			Matrix4 mat = new Matrix4(m[i*16 + 0], m[i*16 + 4], m[i*16 + 8], m[i*16 + 12],
				m[i*16 + 1], m[i*16 + 5], m[i*16 + 9], m[i*16 + 13],
				m[i*16 + 2], m[i*16 + 6], m[i*16 + 10], m[i*16 + 14],
				m[i*16 + 3], m[i*16 + 7], m[i*16 + 11], m[i*16 + 15]);

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
	private void ConvertAnimationsToFrames(CSAnim inAnim)
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
				CSAnimFrame frame = new CSAnimFrame();
				
				for (int j = 0; j < mAnimations.length; j++)
				{
					Matrix4 transform;
					if (mAnimations[j].mTransformList.size() > 0)
						transform = mAnimations[j].mTransformList.get(i);
					else
						transform = inAnim.mSkeleton.mNodeList.get(j).mInitialPoseMatrix;
					
					Tuple3<Vector3, Vector3, Quaternion> decomposedTransform = transform.decomposeTransforms();
					
					frame.mNodeTranslations.add(decomposedTransform.getFirst());
					frame.mNodeOrienations.add(Quaternion.normalise(decomposedTransform.getThird()));
					frame.mNodeScalings.add(decomposedTransform.getSecond());
				}
				
				inAnim.mFrames.add(frame);
			}
		}
		else
		{
			Logging.logFatal("Animation and skeleton node counts do not match!");
		}
	}
}
















