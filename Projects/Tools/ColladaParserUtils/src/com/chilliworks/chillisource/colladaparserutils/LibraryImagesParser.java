/**
 * LibraryImagesParser.java
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

package com.chilliworks.chillisource.colladaparserutils;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;

import com.chilliworks.chillisource.colladaparserutils.colladadata.Collada;
import com.chilliworks.chillisource.colladaparserutils.colladadata.ColladaImage;
import com.chilliworks.chillisource.colladaparserutils.colladadata.ColladaInitFrom;
import com.chilliworks.chillisource.coreutils.Logging;

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
