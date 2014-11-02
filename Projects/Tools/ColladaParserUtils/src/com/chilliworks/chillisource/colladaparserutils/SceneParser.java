/**
 * SceneParser.java
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
