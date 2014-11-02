/**
 * LibraryMaterialsParser.java
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

import com.chilliworks.chillisource.colladaparserutils.colladadata.*;
import com.chilliworks.chillisource.coreutils.Logging;

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
