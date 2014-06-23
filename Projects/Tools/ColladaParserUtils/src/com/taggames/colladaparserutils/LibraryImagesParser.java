//
// CLibraryImagesParser.h
// Collada Parser
//
// Created by Ian Copland on 16/01/2013.
// Copyright 2012 Tag Games. All rights reserved.
//

package com.taggames.colladaparserutils;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import com.chillisource.toolutils.Logging;
import com.taggames.colladaparserutils.colladadata.Collada;
import com.taggames.colladaparserutils.colladadata.ColladaImage;
import com.taggames.colladaparserutils.colladadata.ColladaInitFrom;

public class LibraryImagesParser 
{
	//-------------------------------------------------------------
	/// Private Member Data
	//-------------------------------------------------------------
	private Collada mCollada;
	private ColladaImage mCurrentImage;
	private ColladaInitFrom mCurrentInitFrom;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public LibraryImagesParser(Collada inColladaData)
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
		//push element type on to the imaginary element stack
		if (instrQName.equalsIgnoreCase("image"))
			return PushImage(inAttributes);
		else if (instrQName.equalsIgnoreCase("init_from"))
			return PushInitFrom(inAttributes);
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
		//pop element type off to the imaginary element stack
		if (instrQName.equalsIgnoreCase("image"))
			PopImage(inData);
		else if (instrQName.equalsIgnoreCase("init_from"))
			PopInitFrom(inData);
	}
	//--------------------------------------------------------------
	/// Push Image
	///
	/// Pushes an image element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushImage(Attributes inAttributes)
	{
		mCurrentImage = new ColladaImage();
		mCurrentImage.mstrId = inAttributes.getValue("id");
		
		if (mCollada.mLibraryImages != null)
			mCollada.mLibraryImages.put(mCurrentImage.mstrId, mCurrentImage);
		else
			Logging.logFatal("mLibraryImages is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Image
	///
	/// Pops an image element from the element stack.
	//--------------------------------------------------------------
	private void PopImage(String inData)
	{
		mCurrentImage = null;
	}
	//--------------------------------------------------------------
	/// Push Init From
	///
	/// Pushes an init from element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInitFrom(Attributes inAttributes)
	{
		mCurrentInitFrom = new ColladaInitFrom();
		
		if (mCurrentImage != null)
			mCurrentImage.mInitFrom = mCurrentInitFrom;
		else
			Logging.logFatal("no parent for 'init from'!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop Init From
	///
	/// Pops an init from element from the element stack.
	//--------------------------------------------------------------
	private void PopInitFrom(String inData)
	{
		mCurrentInitFrom.mstrValue = inData;
		
		mCurrentInitFrom = null;
	}
}
