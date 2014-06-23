//
// CAssetParser.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.colladaparser;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import com.taggames.colladaparser.colladadata.Collada;
import com.taggames.colladaparser.colladadata.ColladaAuthoringTool;
import com.taggames.colladaparser.colladadata.ColladaContributor;

public class AssetParser 
{
	//--------------------------------------------------------------
	/// Private Member Data
	//--------------------------------------------------------------
	private Collada mCollada;
	private ColladaContributor mCurrentContributor = null;
	private ColladaAuthoringTool mCurrentAuthoringTool = null;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public AssetParser(Collada inColladaData)
	{
		mCollada = inColladaData;
	}
	//--------------------------------------------------------------
	/// Start Element
	///
	/// Callback method used by SAX whenever a new element is found.
	//--------------------------------------------------------------
	public boolean StartElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException 
	{
		if (instrQName.equalsIgnoreCase("contributor"))
			return PushContributor(inAttributes);
		else if (instrQName.equalsIgnoreCase("authoring_tool"))
			return PushAuthoringTool(inAttributes);
		else
			return false;
	}
	//--------------------------------------------------------------
	/// End Element
	///
	/// Callback method used by SAX whenever a new element is found.
	//--------------------------------------------------------------
	public void EndElement(String instrUri, String instrLocalName, String instrQName, String inData) throws SAXException 
	{
		if (instrQName.equalsIgnoreCase("contributor"))
			PopContributor(inData);
		else if (instrQName.equalsIgnoreCase("authoring_tool"))
			PopAuthoringTool(inData);
	}
	//--------------------------------------------------------------
	/// Push Contributor
	///
	/// Pushes the contributor element type.
	//--------------------------------------------------------------
	private boolean PushContributor(Attributes inAttributes)
	{
		if (mCurrentContributor == null)
		{
			mCurrentContributor = mCollada.mAsset.mContributor;
		}
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Contributor
	///
	/// Pops the contributor element type.
	//--------------------------------------------------------------
	private void PopContributor(String inData)
	{
		mCurrentContributor = null;
	}
	//--------------------------------------------------------------
	/// Push Authoring Tool
	///
	/// Pushes the Authoring Tool element type.
	//--------------------------------------------------------------
	private boolean PushAuthoringTool(Attributes inAttributes)
	{
		if (mCurrentContributor != null)
		{
			mCurrentAuthoringTool = mCurrentContributor.mAuthoringTool;
		}
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop Authoring Tool
	///
	/// Pops the Authoring Tool element type.
	//--------------------------------------------------------------
	private void PopAuthoringTool(String inData)
	{
		if (mCurrentAuthoringTool != null)
		{
			mCurrentAuthoringTool.mstrName = inData;
			mCurrentAuthoringTool = null;
		}
	}
}
