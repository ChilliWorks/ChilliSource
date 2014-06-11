//
// CLibraryVisualScenesParser.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.colladaparser;

import java.util.Stack;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import com.taggames.colladaparser.colladadata.*;
import com.taggames.toolutils.SCLogger;

public class CLibraryVisualScenesParser 
{
	//--------------------------------------------------------------
	/// Private Member Data
	//--------------------------------------------------------------
	private Collada mCollada;
	private ColladaVisualScene mCurrentVisualScene;
	private ColladaMatrix mCurrentMatrix;
	private ColladaInstanceController mCurrentInstanceController;
	private ColladaSkeleton mCurrentSkeleton;
	private ColladaBindMaterial mCurrentBindMaterial;
	private ColladaTechniqueCommon mCurrentTechniqueCommon;
	private ColladaInstanceMaterial mCurrentInstanceMaterial;
	private ColladaInstanceGeometry mCurrentInstanceGeometry;
	private Stack<ColladaNode> mNodeStack;
	private int mdwNextNodeIndex;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public CLibraryVisualScenesParser(Collada inColladaData)
	{
		mCollada = inColladaData;
		mNodeStack = new Stack<ColladaNode>();
		mdwNextNodeIndex = 0;
	}
	//--------------------------------------------------------------
	/// Start Element
	///
	/// Callback method used by SAX whenever a new element is found.
	//--------------------------------------------------------------
	public boolean StartElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException 
	{
		//push element type on to the imaginary element stack
		if (instrQName.equalsIgnoreCase("visual_scene"))
			return PushVisualScene(inAttributes);
		else if (instrQName.equalsIgnoreCase("node"))
			return PushNode(inAttributes);
		else if (instrQName.equalsIgnoreCase("instance_light"))
			return PushInstanceLight(inAttributes);
		else if (instrQName.equalsIgnoreCase("matrix"))
			return PushMatrix(inAttributes);
		else if (instrQName.equalsIgnoreCase("instance_controller"))
			return PushInstanceController(inAttributes);
		else if (instrQName.equalsIgnoreCase("skeleton"))
			return PushSkeleton(inAttributes);
		else if (instrQName.equalsIgnoreCase("bind_material"))
			return PushBindMaterial(inAttributes);
		else if (instrQName.equalsIgnoreCase("technique_common"))
			return PushTechniqueCommon(inAttributes);
		else if (instrQName.equalsIgnoreCase("instance_material"))
			return PushInstanceMaterial(inAttributes);
		else if (instrQName.equalsIgnoreCase("bind_vertex_input"))
			return PushBindVertexInput(inAttributes);
		else if (instrQName.equalsIgnoreCase("instance_geometry"))
			return PushInstanceGeometry(inAttributes);
		else if (instrQName.equalsIgnoreCase("instance_camera"))
			return PushInstanceCamera(inAttributes);
		else if (instrQName.equalsIgnoreCase("extra"))
			return PushExtra(inAttributes);
		else if (instrQName.equalsIgnoreCase("technique"))
			return PushTechnique(inAttributes);
		else if (instrQName.equalsIgnoreCase("user_properties"))
			return PushUserProperties(inAttributes);
		else
			return false;
	}
	//--------------------------------------------------------------
	/// End Element
	///
	/// Callback method used by SAX whenever the end of an element 
	/// is found.
	//--------------------------------------------------------------
	public void EndElement(String instrUri, String instrLocalName, String instrQName, String inData) throws SAXException 
	{
		//pop element type off to the imaginary element stack
		if (instrQName.equalsIgnoreCase("visual_scene"))
			PopVisualScene(inData);
		else if (instrQName.equalsIgnoreCase("node"))
			PopNode(inData);
		else if (instrQName.equalsIgnoreCase("instance_light"))
			PopInstanceLight(inData);
		else if (instrQName.equalsIgnoreCase("matrix"))
			PopMatrix(inData);
		else if (instrQName.equalsIgnoreCase("instance_controller"))
			PopInstanceController(inData);
		else if (instrQName.equalsIgnoreCase("skeleton"))
			PopSkeleton(inData);
		else if (instrQName.equalsIgnoreCase("bind_material"))
			PopBindMaterial(inData);
		else if (instrQName.equalsIgnoreCase("technique_common"))
			PopTechniqueCommon(inData);
		else if (instrQName.equalsIgnoreCase("instance_material"))
			PopInstanceMaterial(inData);
		else if (instrQName.equalsIgnoreCase("bind_vertex_input"))
			PopBindVertexInput(inData);
		else if (instrQName.equalsIgnoreCase("instance_geometry"))
			PopInstanceGeometry(inData);
		else if (instrQName.equalsIgnoreCase("instance_camera"))
			PopInstanceCamera(inData);
		else if (instrQName.equalsIgnoreCase("extra"))
			PopExtra(inData);
		else if (instrQName.equalsIgnoreCase("technique"))
			PopTechnique(inData);
		else if (instrQName.equalsIgnoreCase("user_properties"))
			PopUserProperties(inData);
	}
	//--------------------------------------------------------------
	/// Push Visual Scene
	///
	/// Pushes a visual scene element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushVisualScene(Attributes inAttributes)
	{
		mCurrentVisualScene = new ColladaVisualScene();
		mCurrentVisualScene.mstrId = inAttributes.getValue("id");
		
		if (mCollada.mLibraryVisualScenes != null)
			mCollada.mLibraryVisualScenes.put(mCurrentVisualScene.mstrId, mCurrentVisualScene);
		else
			SCLogger.LogFatalError("mLibraryVisualScenes is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Visual Scene
	///
	/// Pops a visual scene element from the element stack.
	//--------------------------------------------------------------
	private void PopVisualScene(String inData)
	{
		mCurrentVisualScene = null;
	}
	//--------------------------------------------------------------
	/// Push Node
	///
	/// Pushes a node element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushNode(Attributes inAttributes)
	{
		ColladaNode newNode = new ColladaNode();
		newNode.mstrName = inAttributes.getValue("name");
		newNode.mstrId = CColladaUtils.GetStringAttributeOrEmpty(inAttributes, "id");
		newNode.mstrSId = CColladaUtils.GetStringAttributeOrEmpty(inAttributes, "sid");
		newNode.mstrType = CColladaUtils.GetStringAttributeOrEmpty(inAttributes, "type");
		
		//if there is no id, set the id to the name so the id can still be used as the key.
		if (newNode.mstrId.equalsIgnoreCase("") == true)
		{
			newNode.mstrId = "node" + mdwNextNodeIndex++;
		}
		
		//if this node is a joint, flag it as such
		if (newNode.mstrType.equalsIgnoreCase("JOINT") == true)
		{
			newNode.meType = ColladaNode.COLLADA_NODE_TYPE.JOINT;
		}
		
		if (mCurrentVisualScene != null && mNodeStack.size() == 0)
			mCurrentVisualScene.mRootNodes.put(newNode.mstrId, newNode);
		else if (mNodeStack.size() > 0)
			mNodeStack.peek().mChildNodes.put(newNode.mstrId, newNode);
		else
			SCLogger.LogFatalError("mCurrentVisualScene is null!");
		
		//add to node stack
		mNodeStack.push(newNode);
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Node
	///
	/// Pops a node element from the element stack.
	//--------------------------------------------------------------
	private void PopNode(String inData)
	{
		mNodeStack.pop();
	}
	//--------------------------------------------------------------
	/// Push Instance Light
	///
	/// Pushes an instance light element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInstanceLight(Attributes inAttributes)
	{
		ColladaInstanceLight newLight = new ColladaInstanceLight();
		newLight.mstrUrl = inAttributes.getValue("url");
		
		if (mNodeStack.size() > 0)
		{
			if (mNodeStack.peek().meType == ColladaNode.COLLADA_NODE_TYPE.BASE)
			{
				mNodeStack.peek().mInstanceLight = newLight;
				mNodeStack.peek().meType = ColladaNode.COLLADA_NODE_TYPE.LIGHT;
			}
		}
		else
			SCLogger.LogFatalError("mCurrentVisualScene is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Instance Light
	///
	/// Pops an instance light element from the element stack.
	//--------------------------------------------------------------
	private void PopInstanceLight(String inData)
	{
		
	}
	//--------------------------------------------------------------
	/// Push Matrix
	///
	/// Pushes a matrix element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushMatrix(Attributes inAttributes)
	{
		mCurrentMatrix= new ColladaMatrix();
		mCurrentMatrix.mstrSId = CColladaUtils.GetStringAttributeOrEmpty(inAttributes, "sid");
		
		if (mNodeStack.size() > 0)
			mNodeStack.peek().mMatrix = mCurrentMatrix;
		else
			SCLogger.LogFatalError("mNodeStack is empty!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop Matrix
	///
	/// Pops a matrix element from the element stack.
	//--------------------------------------------------------------
	private void PopMatrix(String inData)
	{
		String[] astrSplitBuffer = CColladaUtils.SplitOnWhitespace(inData);
		
		mCurrentMatrix.maafValues = new float[4][4];
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				mCurrentMatrix.maafValues[i][j] = Float.parseFloat(astrSplitBuffer[i * 4 + j]);
			}
		}
		
		mCurrentMatrix = null;
	}
	//--------------------------------------------------------------
	/// Push Instance Controller
	///
	/// Pushes an instance controller element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInstanceController(Attributes inAttributes)
	{
		mCurrentInstanceController = new ColladaInstanceController();
		mCurrentInstanceController.mstrUrl = CColladaUtils.GetStringAttributeOrEmpty(inAttributes, "url");
		
		if (mNodeStack.size() > 0)
		{
			if (mNodeStack.peek().meType == ColladaNode.COLLADA_NODE_TYPE.BASE)
			{
				mNodeStack.peek().mInstanceController = mCurrentInstanceController;
				mNodeStack.peek().meType = ColladaNode.COLLADA_NODE_TYPE.CONTROLLER;
			}
		}
		else
			SCLogger.LogFatalError("mNodeStack is empty!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Instance Controller
	///
	/// Pops an instance controller element from the element stack.
	//--------------------------------------------------------------
	private void PopInstanceController(String inData)
	{
		mCurrentInstanceController = null;
	}
	//--------------------------------------------------------------
	/// Push Skeleton
	///
	/// Pushes a skeleton element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushSkeleton(Attributes inAttributes)
	{
		mCurrentSkeleton = new ColladaSkeleton();
		
		if (mCurrentInstanceController != null)
			mCurrentInstanceController.mSkeleton = mCurrentSkeleton;
		else
			SCLogger.LogFatalError("mCurrentInstanceController is null!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop Skeleton
	///
	/// Pops a skeleton element from the element stack.
	//--------------------------------------------------------------
	private void PopSkeleton(String inData)
	{
		mCurrentSkeleton.mstrValue = inData;
		mCurrentSkeleton = null;
	}
	//--------------------------------------------------------------
	/// Push Bind Material
	///
	/// Pushes a bind material element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushBindMaterial(Attributes inAttributes)
	{
		mCurrentBindMaterial = new ColladaBindMaterial();
		
		if (mCurrentInstanceController != null)
			mCurrentInstanceController.mBindMaterial = mCurrentBindMaterial;
		else if (mCurrentInstanceGeometry != null)
			mCurrentInstanceGeometry.mBindMaterial = mCurrentBindMaterial;
		else
			SCLogger.LogFatalError("mCurrentInstanceController is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Bind Material
	///
	/// Pops a bind material element on to the element stack.
	//--------------------------------------------------------------
	private void PopBindMaterial(String inData)
	{
		mCurrentBindMaterial = null;
	}
	//--------------------------------------------------------------
	/// Push Technique Common
	///
	/// Pushes a technique common element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushTechniqueCommon(Attributes inAttributes)
	{
		mCurrentTechniqueCommon = new ColladaTechniqueCommon();
		
		if (mCurrentBindMaterial != null)
			mCurrentBindMaterial.mTechniqueCommon = mCurrentTechniqueCommon;
		else
			SCLogger.LogFatalError("mCurrentBindMaterial is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Technique Common
	///
	/// Pops a technique common element from the element stack.
	//--------------------------------------------------------------
	private void PopTechniqueCommon(String inData)
	{
		mCurrentTechniqueCommon = null;
	}
	//--------------------------------------------------------------
	/// Push Instance Material
	///
	/// Pushes an instance material element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInstanceMaterial(Attributes inAttributes)
	{
		mCurrentInstanceMaterial = new ColladaInstanceMaterial();
		mCurrentInstanceMaterial.mstrSymbol = inAttributes.getValue("symbol");
		mCurrentInstanceMaterial.mstrTarget = inAttributes.getValue("target");
		
		if (mCurrentTechniqueCommon != null)
			mCurrentTechniqueCommon.mInstanceMaterialList.add(mCurrentInstanceMaterial);
		else
			SCLogger.LogFatalError("mCurrentTechniqueCommon is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Instance Material
	///
	/// Pops an instance material element from the element stack.
	//--------------------------------------------------------------
	private void PopInstanceMaterial(String inData)
	{
		mCurrentInstanceMaterial = null;
	}
	//--------------------------------------------------------------
	/// Push Bind Vertex Input
	///
	/// Pushes a Bind Vertex Input element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushBindVertexInput(Attributes inAttributes)
	{
		ColladaBindVertexInput newBindvBindVertexInput = new ColladaBindVertexInput();
		newBindvBindVertexInput.mstrInputSemantic = inAttributes.getValue("input_semantic");
		newBindvBindVertexInput.mstrInputSet = inAttributes.getValue("input_set");
		newBindvBindVertexInput.mstrSemantic = inAttributes.getValue("semantic");
		
		if (mCurrentInstanceMaterial != null)
			mCurrentInstanceMaterial.mBindVertexInput = newBindvBindVertexInput;
		else
			SCLogger.LogFatalError("mCurrentInstanceMaterial is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Bind Vertex Input
	///
	/// Pops a Bind Vertex Input element from the element stack.
	//--------------------------------------------------------------
	private void PopBindVertexInput(String inData)
	{
	}
	//--------------------------------------------------------------
	/// Push Instance Geometry
	///
	/// Pushes an instance geometry element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInstanceGeometry(Attributes inAttributes)
	{
		mCurrentInstanceGeometry = new ColladaInstanceGeometry();
		mCurrentInstanceGeometry.mstrUrl = CColladaUtils.GetStringAttributeOrEmpty(inAttributes, "url");
		
		if (mNodeStack.size() > 0)
		{
			if (mNodeStack.peek().meType == ColladaNode.COLLADA_NODE_TYPE.BASE)
			{
				mNodeStack.peek().mInstanceGeometry = mCurrentInstanceGeometry;
				mNodeStack.peek().meType = ColladaNode.COLLADA_NODE_TYPE.GEOMETRY;
			}
		}
		else
			SCLogger.LogFatalError("mNodeStack is empty!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Instance Geometry
	///
	/// Pops an instance geometry element from the element stack.
	//--------------------------------------------------------------
	private void PopInstanceGeometry(String inData)
	{
		mCurrentInstanceController = null;
	}
	//--------------------------------------------------------------
	/// Push Instance Camera
	///
	/// Pushes an instance camera element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInstanceCamera(Attributes inAttributes)
	{
		ColladaInstanceCamera newCamera = new ColladaInstanceCamera();
		newCamera.mstrUrl = inAttributes.getValue("url");
		
		if (mNodeStack.size() > 0)
		{
			if (mNodeStack.peek().meType == ColladaNode.COLLADA_NODE_TYPE.BASE)
			{
				mNodeStack.peek().mInstanceCamera = newCamera;
				mNodeStack.peek().meType = ColladaNode.COLLADA_NODE_TYPE.CAMERA;
			}
		}
		else
			SCLogger.LogFatalError("mNodeStack is empty!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Instance Camera
	///
	/// Pops an instance camera element on to the element stack.
	//--------------------------------------------------------------
	private void PopInstanceCamera(String inData)
	{
	}
	//--------------------------------------------------------------
	/// Push Extra
	///
	/// Pushes an extra element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushExtra(Attributes inAttributes)
	{
		ColladaExtra extra = new ColladaExtra();
		
		if (mNodeStack.size() > 0)
		{
			mNodeStack.peek().mExtra = extra;
		}
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Extra
	///
	/// Pops an extra element from the element stack.
	//--------------------------------------------------------------
	private void PopExtra(String inData)
	{
	}
	//--------------------------------------------------------------
	/// Push Technique
	///
	/// Pushes a technique on to the element stack.
	//--------------------------------------------------------------
	private boolean PushTechnique(Attributes inAttributes)
	{
		ColladaTechnique technique = new ColladaTechnique();
		technique.mstrProfile = inAttributes.getValue("profile");
		
		if (mNodeStack.size() > 0 && mNodeStack.peek().mExtra != null)
		{
			mNodeStack.peek().mExtra.mTechnique = technique;
		}
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Technique
	///
	/// Pops a Technique element from the element stack.
	//--------------------------------------------------------------
	private void PopTechnique(String inData)
	{
	}
	//--------------------------------------------------------------
	/// Push User Properties
	///
	/// Pushes a user properties element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushUserProperties(Attributes inAttributes)
	{
		ColladaUserProperties userProperties = new ColladaUserProperties();
		
		if (mNodeStack.size() > 0 && mNodeStack.peek().mExtra != null && mNodeStack.peek().mExtra.mTechnique != null)
		{
			mNodeStack.peek().mExtra.mTechnique.mUserProperties = userProperties;
			return true;
		}
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop User Properties
	///
	/// Pops a user properties from the element stack.
	//--------------------------------------------------------------
	private void PopUserProperties(String inData)
	{
		if (mNodeStack.size() > 0 && mNodeStack.peek().mExtra != null && mNodeStack.peek().mExtra.mTechnique != null && mNodeStack.peek().mExtra.mTechnique.mUserProperties != null)
		{
			mNodeStack.peek().mExtra.mTechnique.mUserProperties.mstrData = inData;
		}
	}
}
