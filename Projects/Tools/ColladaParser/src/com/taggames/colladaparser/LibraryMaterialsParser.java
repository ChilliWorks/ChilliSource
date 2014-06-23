//
// CLibraryMaterialsParser.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.colladaparser;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import com.chillisource.toolutils.Logging;
import com.taggames.colladaparser.colladadata.*;

public class LibraryMaterialsParser 
{
	//--------------------------------------------------------------
	/// Private Memeber Data
	//--------------------------------------------------------------
	private Collada mCollada;
	private ColladaMaterial mCurrentMaterial;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public LibraryMaterialsParser(Collada inColladaData)
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
		if (instrQName.equalsIgnoreCase("material"))
			return PushMaterial(inAttributes);
		else if (instrQName.equalsIgnoreCase("instance_effect"))
			return PushInstanceEffect(inAttributes);
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
		if (instrQName.equalsIgnoreCase("material"))
			PopMaterial(inData);
		else if (instrQName.equalsIgnoreCase("instance_effect"))
			PopInstanceEffect(inData);
	}
	//--------------------------------------------------------------
	/// Push Material
	///
	/// Pushes a material element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushMaterial(Attributes inAttributes)
	{
		mCurrentMaterial = new ColladaMaterial();
		mCurrentMaterial.mstrId = inAttributes.getValue("id");
		mCurrentMaterial.mstrName = inAttributes.getValue("name");
		
		if (mCollada.mLibraryMaterials != null)
		{
			mCollada.mLibraryMaterials.put(mCurrentMaterial.mstrId, mCurrentMaterial);
		}else
			Logging.logFatal("mLibraryMaterials is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Material
	///
	/// Pops a material element from the element stack.
	//--------------------------------------------------------------
	private void PopMaterial(String inData)
	{
		mCurrentMaterial = null;
	}
	//--------------------------------------------------------------
	/// Push Instance Effect
	///
	/// Pushes an instance effect element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInstanceEffect(Attributes inAttributes)
	{
		ColladaInstanceEffect instanceEffect = new ColladaInstanceEffect();
		instanceEffect.mstrUrl = inAttributes.getValue("url");
		
		if (mCurrentMaterial != null)
			mCurrentMaterial.mInstanceEffect = instanceEffect;
		else
			Logging.logFatal("mCurrentMaterial is null!");

		return false;
	}
	//--------------------------------------------------------------
	/// Pop Instance Effect
	///
	/// Pops an instance effect element from the element stack.
	//--------------------------------------------------------------
	private void PopInstanceEffect(String inData)
	{
	}
}
