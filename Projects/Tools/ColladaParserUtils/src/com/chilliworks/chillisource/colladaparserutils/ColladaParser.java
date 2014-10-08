/**
 * ColladaParser.java
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

public class ColladaParser extends org.xml.sax.helpers.DefaultHandler
{
	//--------------------------------------------------------------
	/// Parser State
	//--------------------------------------------------------------
	enum PARSER_STATE
	{
		PS_BASE,
		PS_ASSET,
		PS_GEOMETRIES,
		PS_MATERIALS,
		PS_IMAGES,
		PS_EFFECTS,
		PS_VISUAL_SCENES,
		PS_CONTROLLERS,
		PS_ANIMATIONS,
		PS_SCENE
	}
	//--------------------------------------------------------------
	/// Private member Data
	//--------------------------------------------------------------
	private boolean mbReadCharacters;
	private StringBuffer mstrCurrentCharacterBuffer;
	private Collada mCollada;
	private AssetParser mAssetParser;
	private LibraryEffectsParser mEffectsParser;
	private LibraryMaterialsParser mMaterialsParser;
	private LibraryGeometriesParser mGeometriesParser;
	private LibraryVisualScenesParser mVisualScenesParser;
	private LibraryImagesParser mImagesParser;
	private LibraryControllersParser mControllersParser;
	private LibraryAnimationsParser mAnimationsParser;
	private SceneParser mSceneParser;
	private PARSER_STATE meState;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public ColladaParser(Collada inColladaData)
	{
		mCollada = inColladaData;
		
		mAssetParser = new AssetParser(inColladaData);
		mEffectsParser = new LibraryEffectsParser(mCollada);
		mMaterialsParser = new LibraryMaterialsParser(mCollada);
		mGeometriesParser = new LibraryGeometriesParser(mCollada);
		mImagesParser = new LibraryImagesParser(mCollada);
		mVisualScenesParser = new LibraryVisualScenesParser(mCollada);
		mControllersParser = new LibraryControllersParser(mCollada);
		mAnimationsParser = new LibraryAnimationsParser(mCollada);
		mSceneParser = new SceneParser(mCollada);
		
		mstrCurrentCharacterBuffer = null;
		mbReadCharacters = false;
		meState = PARSER_STATE.PS_BASE;
	}
	//--------------------------------------------------------------
	/// Start Element
	///
	/// Callback method used by SAX whenever a new element is found.
	//--------------------------------------------------------------
	public void startElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException 
	{
		mstrCurrentCharacterBuffer = new StringBuffer();
		
		switch (meState)
		{
		case PS_BASE:
			mbReadCharacters = StartBaseElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_ASSET:
			mbReadCharacters = StartAssetElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_GEOMETRIES:
			mbReadCharacters = StartGeometriesElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_IMAGES:
			mbReadCharacters = StartImagesElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_MATERIALS:
			mbReadCharacters = StartMaterialsElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_EFFECTS:
			mbReadCharacters = StartEffectsElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_VISUAL_SCENES:
			mbReadCharacters = StartVisualScenesElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_CONTROLLERS:
			mbReadCharacters = StartControllersElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_ANIMATIONS:
			mbReadCharacters = StartAnimationsElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		case PS_SCENE:
			mbReadCharacters = StartSceneElement(instrUri, instrLocalName,instrQName, inAttributes);
			break;
		}
	}
	//--------------------------------------------------------------
	/// End Element
	///
	/// Callback method used by SAX whenever a new element is found.
	//--------------------------------------------------------------
	public void endElement(String instrUri, String instrLocalName,String instrQName) throws SAXException 
	{
		switch (meState)
		{
		case PS_BASE:
			EndBaseElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_ASSET:
			EndAssetElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_GEOMETRIES:
			EndGeometriesElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_IMAGES:
			EndImagesElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_MATERIALS:
			EndMaterialsElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_EFFECTS:
			EndEffectsElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_VISUAL_SCENES:
			EndVisualScenesElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_CONTROLLERS:
			EndControllersElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_ANIMATIONS:
			EndAnimationsElement(instrUri, instrLocalName,instrQName);
			break;
		case PS_SCENE:
			EndSceneElement(instrUri, instrLocalName,instrQName);
			break;
		}
		
		mbReadCharacters = false;
		mstrCurrentCharacterBuffer = null;
	}
	//--------------------------------------------------------------
	/// Characters
	///
	/// Callback method used by SAX whenever contents of a element 
	/// is found.
	//--------------------------------------------------------------
	public void characters(char inbyCharacters[], int indwStart, int indwLength) throws SAXException 
	{
		if (mbReadCharacters == false) return;
		
		mstrCurrentCharacterBuffer.append(inbyCharacters, indwStart, indwLength);
	}
	//--------------------------------------------------------------
	/// Start Base Element
	///
	/// Starts a base element, for example a library.
	//--------------------------------------------------------------
	private boolean StartBaseElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes)
	{
		if (instrQName.equalsIgnoreCase("COLLADA") == true)
		{
			//Do nothing
		}
		else if (instrQName.equalsIgnoreCase("asset") == true)
		{
			meState = PARSER_STATE.PS_ASSET;
		}
		else if (instrQName.equalsIgnoreCase("library_geometries") == true)
		{
			meState = PARSER_STATE.PS_GEOMETRIES;
		}
		else if (instrQName.equalsIgnoreCase("library_images") == true)
		{
			meState = PARSER_STATE.PS_IMAGES;
		}
		else if (instrQName.equalsIgnoreCase("library_materials") == true)
		{
			meState = PARSER_STATE.PS_MATERIALS;
		}
		else if (instrQName.equalsIgnoreCase("library_effects") == true)
		{
			meState = PARSER_STATE.PS_EFFECTS;
		}
		else if (instrQName.equalsIgnoreCase("library_visual_scenes") == true)
		{
			meState = PARSER_STATE.PS_VISUAL_SCENES;
		}
		else if (instrQName.equalsIgnoreCase("library_controllers") == true)
		{
			meState = PARSER_STATE.PS_CONTROLLERS;
		}
		else if (instrQName.equalsIgnoreCase("library_animations") == true)
		{
			meState = PARSER_STATE.PS_ANIMATIONS;
		}
		else if (instrQName.equalsIgnoreCase("scene") == true)
		{
			meState = PARSER_STATE.PS_SCENE;
		}
		else
		{
			//element is unknown and unimportant.
		}
		
		return false;
	}
	//--------------------------------------------------------------
	/// End Base Element
	///
	/// Ends a base element, for example a library.
	//--------------------------------------------------------------
	private void EndBaseElement(String instrUri, String instrLocalName,String instrQName) throws SAXException
	{
		
	}
	//--------------------------------------------------------------
	/// Start Asset Element
	///
	/// Starts an asset element.
	//--------------------------------------------------------------
	private boolean StartAssetElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mAssetParser.StartElement(instrUri, instrLocalName,instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// End Asset Element
	///
	/// Ends an asset element.
	//--------------------------------------------------------------
	private void EndAssetElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("asset"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mAssetParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mAssetParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
	//--------------------------------------------------------------
	/// Start Geometries Element
	///
	/// Starts a geometries element.
	//--------------------------------------------------------------
	private boolean StartGeometriesElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mGeometriesParser.StartElement(instrUri, instrLocalName, instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// End Geometries Element
	///
	/// Ends a geometries element.
	//--------------------------------------------------------------
	private void EndGeometriesElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("library_geometries"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mGeometriesParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mGeometriesParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
	//--------------------------------------------------------------
	/// Start Images Element
	///
	/// Starts an images element.
	//--------------------------------------------------------------
	private boolean StartImagesElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mImagesParser.StartElement(instrUri, instrLocalName,instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// End Images Element
	///
	/// Ends an images element.
	//--------------------------------------------------------------
	private void EndImagesElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("library_images"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mImagesParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mImagesParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
	//--------------------------------------------------------------
	/// Start Materials Element
	///
	/// Starts a materials element.
	//--------------------------------------------------------------
	private boolean StartMaterialsElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mMaterialsParser.StartElement(instrUri, instrLocalName,instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// Ends Materials Element
	///
	/// Ends a materials element.
	//--------------------------------------------------------------
	private void EndMaterialsElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("library_materials"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mMaterialsParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mMaterialsParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
	//--------------------------------------------------------------
	/// Start Effects Element
	///
	/// Starts an effects element.
	//--------------------------------------------------------------
	private boolean StartEffectsElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mEffectsParser.StartElement(instrUri, instrLocalName,instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// End Effects Element
	///
	/// End an effects element.
	//--------------------------------------------------------------
	private void EndEffectsElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("library_effects"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mEffectsParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mEffectsParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
	//--------------------------------------------------------------
	/// Start Visual Scenes Element
	///
	/// Starts a visual scenes element.
	//--------------------------------------------------------------
	private boolean StartVisualScenesElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mVisualScenesParser.StartElement(instrUri, instrLocalName,instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// End Visual Scenes Element
	///
	/// Ends a visual scenes element.
	//--------------------------------------------------------------
	private void EndVisualScenesElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("library_visual_scenes"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mVisualScenesParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mVisualScenesParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
	//--------------------------------------------------------------
	/// Start Controllers Element
	///
	/// Starts a controllers element.
	//--------------------------------------------------------------
	private boolean StartControllersElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mControllersParser.StartElement(instrUri, instrLocalName,instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// End Controllers Element
	///
	/// Ends a controllers element.
	//--------------------------------------------------------------
	private void EndControllersElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("library_controllers"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mControllersParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mControllersParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
	//--------------------------------------------------------------
	/// Start Animations Element
	///
	/// Starts an animations element.
	//--------------------------------------------------------------
	private boolean StartAnimationsElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mAnimationsParser.StartElement(instrUri, instrLocalName,instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// End Animations Element
	///
	/// End an animations element.
	//--------------------------------------------------------------
	private void EndAnimationsElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("library_animations"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mAnimationsParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mAnimationsParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
	//--------------------------------------------------------------
	/// Start Scene Element
	///
	/// Starts a scene element.
	//--------------------------------------------------------------
	private boolean StartSceneElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException
	{
		return mSceneParser.StartElement(instrUri, instrLocalName,instrQName, inAttributes);
	}
	//--------------------------------------------------------------
	/// End Scene Element
	///
	/// Ends a scene element.
	//--------------------------------------------------------------
	private void EndSceneElement(String instrUri, String instrLocalName, String instrQName) throws SAXException
	{
		if (instrQName.equalsIgnoreCase("scene"))
		{
			meState = PARSER_STATE.PS_BASE;
		}
		else
		{
			if (mstrCurrentCharacterBuffer != null && mstrCurrentCharacterBuffer.length() > 0)
				mSceneParser.EndElement(instrUri, instrLocalName, instrQName, mstrCurrentCharacterBuffer.toString());
			else
				mSceneParser.EndElement(instrUri, instrLocalName, instrQName, " ");
		}
	}
}
