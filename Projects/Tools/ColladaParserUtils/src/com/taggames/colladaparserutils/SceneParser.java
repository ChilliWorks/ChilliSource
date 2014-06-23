//
// CSceneParser.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.colladaparserutils;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import com.chillisource.toolutils.Logging;
import com.taggames.colladaparserutils.colladadata.*;

public class SceneParser 
{
	//--------------------------------------------------------------
	/// Private Member Data
	//--------------------------------------------------------------
	private Collada mCollada;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public SceneParser(Collada inColladaData)
	{
		mCollada = inColladaData;
	}
	//--------------------------------------------------------------
	/// Start Element
	///
	/// Callback method used by SAX whenever the start of an element 
	/// is found.
	//--------------------------------------------------------------
	public boolean StartElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException 
	{
		//push element type on to the imaginary element stack
		if (instrQName.equalsIgnoreCase("instance_visual_scene"))
			return PushInstanceVisualScene(inAttributes);
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
		if (instrQName.equalsIgnoreCase("instance_visual_scene"))
			PopInstanceVisualScene(inData);
	}
	//--------------------------------------------------------------
	/// Push Instance Visual Scene
	///
	/// Pushes an instance visual scene element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInstanceVisualScene(Attributes inAttributes)
	{
		ColladaInstanceVisualScene newIVS = new ColladaInstanceVisualScene();
		newIVS.mstrUrl = inAttributes.getValue("url");
		
		if (mCollada.mScene != null)
			mCollada.mScene.mInstanceVisualScene = newIVS;
		else
			Logging.logFatal("mScene is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Instance Visual Scene
	///
	/// Pops an instance visual scene element from the element stack.
	//--------------------------------------------------------------
	private void PopInstanceVisualScene(String inData)
	{
	}
}
