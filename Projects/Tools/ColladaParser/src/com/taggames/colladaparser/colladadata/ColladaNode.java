//
// ColladaNode.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//


package com.taggames.colladaparser.colladadata;

import java.util.Hashtable;

public class ColladaNode 
{
	public enum COLLADA_NODE_TYPE
	{
		BASE,
		JOINT,
		LIGHT,
		GEOMETRY,
		CONTROLLER,
		CAMERA
	}
	
	public Hashtable<String, ColladaNode> mChildNodes;
	public String mstrId;
	public String mstrName;
	public String mstrType;
	public String mstrSId;
	public ColladaMatrix mMatrix;
	public COLLADA_NODE_TYPE meType;
	public ColladaInstanceLight mInstanceLight;
	public ColladaInstanceGeometry mInstanceGeometry;
	public ColladaInstanceController mInstanceController;
	public ColladaInstanceCamera mInstanceCamera;
	public ColladaExtra mExtra;
	
	public ColladaNode()
	{
		mChildNodes = new Hashtable<String, ColladaNode>();
		mstrId = new String();
		mstrName = new String();
		mMatrix = new ColladaMatrix();
		meType = COLLADA_NODE_TYPE.BASE;
		mInstanceLight = new ColladaInstanceLight();
		mInstanceGeometry = new ColladaInstanceGeometry();
		mInstanceController = new ColladaInstanceController();
		mInstanceCamera = new ColladaInstanceCamera();
		mExtra = new ColladaExtra();
	}
}
