/**
 * AssetParser.java
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
import com.chilliworks.chillisource.colladaparserutils.colladadata.ColladaAuthoringTool;
import com.chilliworks.chillisource.colladaparserutils.colladadata.ColladaContributor;

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
