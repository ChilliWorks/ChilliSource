/**
 * LibraryGeometriesParser.java
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

public class LibraryGeometriesParser 
{
	//--------------------------------------------------------------
	/// Private Member Data
	//--------------------------------------------------------------
	private int mdwIgnoreStack;
	private Collada mCollada;
	private ColladaGeometry mCurrentGeometry;
	private ColladaSource mCurrentSource;
	private ColladaTechniqueCommon mCurrentTechniqueCommon;
	private ColladaFloatArray mCurrentFloatArray;
	private ColladaVertices mCurrentVertices;
	private ColladaTriangles mCurrentTriangles;
	private ColladaP mCurrentP;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public LibraryGeometriesParser(Collada inColladaData)
	{
		mCollada = inColladaData;
		mdwIgnoreStack = 0;
	}
	//--------------------------------------------------------------
	/// Start Element
	///
	/// Callback method used by SAX whenever a new element is found.
	//--------------------------------------------------------------
	public boolean StartElement(String instrUri, String instrLocalName, String instrQName, Attributes inAttributes) throws SAXException 
	{
		//push element type on to the imaginary element stack
		if (instrQName.equalsIgnoreCase("geometry"))
			return PushGeometry(inAttributes);
		else if (instrQName.equalsIgnoreCase("mesh"))
			return PushMesh(inAttributes);
		else if (instrQName.equalsIgnoreCase("source"))
			return PushSource(inAttributes);
		else if (instrQName.equalsIgnoreCase("technique_common"))
			return PushTechniqueCommon(inAttributes);
		else if (instrQName.equalsIgnoreCase("accessor"))
			return PushAccessor(inAttributes);
		else if (instrQName.equalsIgnoreCase("float_array"))
			return PushFloatArray(inAttributes);
		else if (instrQName.equalsIgnoreCase("vertices"))
			return PushVertices(inAttributes);
		else if (instrQName.equalsIgnoreCase("triangles") || instrQName.equalsIgnoreCase("polylist"))
			return PushTriangles(inAttributes);
		else if (instrQName.equalsIgnoreCase("input"))
			return PushInput(inAttributes);
		else if (instrQName.equalsIgnoreCase("vcount"))
			return PushVCount(inAttributes);
		else if (instrQName.equalsIgnoreCase("p"))
			return PushP(inAttributes);
		else
		{
			mdwIgnoreStack++;
			return false;
		}
	}
	//--------------------------------------------------------------
	/// End Element
	///
	/// Callback method used by SAX whenever a new element is found.
	//--------------------------------------------------------------
	public void EndElement(String instrUri, String instrLocalName, String instrQName, String inData) throws SAXException 
	{
		//pop element type off to the imaginary element stack
		if (instrQName.equalsIgnoreCase("geometry"))
			PopGeometry(inData);
		else if (instrQName.equalsIgnoreCase("mesh"))
			PopMesh(inData);
		else if (instrQName.equalsIgnoreCase("source"))
			PopSource(inData);
		else if (instrQName.equalsIgnoreCase("technique_common"))
			PopTechniqueCommon(inData);
		else if (instrQName.equalsIgnoreCase("accessor"))
			PopAccessor(inData);
		else if (instrQName.equalsIgnoreCase("float_array"))
			PopFloatArray(inData);
		else if (instrQName.equalsIgnoreCase("vertices"))
			PopVertices(inData);
		else if (instrQName.equalsIgnoreCase("triangles") || instrQName.equalsIgnoreCase("polylist"))
			PopTriangles(inData);
		else if (instrQName.equalsIgnoreCase("input"))
			PopInput(inData);
		else if (instrQName.equalsIgnoreCase("vcount"))
			PopVCount(inData);
		else if (instrQName.equalsIgnoreCase("p"))
			PopP(inData);
		else
			mdwIgnoreStack--;
	}
	//--------------------------------------------------------------
	/// Push Geometry
	///
	/// Pushes a geometry element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushGeometry(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create a new library geometry
		mCurrentGeometry = new ColladaGeometry();
		mCurrentGeometry.mstrId = inAttributes.getValue("id");
		mCurrentGeometry.mstrName = inAttributes.getValue("name");
		
		//try and add to library
		if (mCollada.mLibraryGeometries != null)
			mCollada.mLibraryGeometries.put(mCurrentGeometry.mstrId, mCurrentGeometry);
		else
			Logging.logFatal("mLibraryGeometries is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Geometry
	///
	/// Pops a geometry element from the element stack.
	//--------------------------------------------------------------
	private void PopGeometry(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentGeometry = null;
	}
	//--------------------------------------------------------------
	/// Push VCount
	///
	/// Pushes a vertex count element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushVCount(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop VCount
	///
	/// Pops a vertex count element from the element stack.
	//--------------------------------------------------------------
	private void PopVCount(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		
		String[] values = inData.split(" ");
		for(String val : values)
		{
			if(val.equals("3") == false)
			{
				Logging.logFatal("Exporter only supports models with triangles");
			}
		}
	}
	//--------------------------------------------------------------
	/// Push Mesh
	///
	/// Pushes a mesh element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushMesh(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//check geometry exists
		if (mCurrentGeometry == null)
			Logging.logFatal("mCurrentGeometry is null!");
		
		mCurrentGeometry.meType = ColladaGeometry.COLLADA_GEOMETRY_TYPE.MESH;
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Mesh
	///
	/// Pops a mesh element from the element stack.
	//--------------------------------------------------------------
	private void PopMesh(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
	}
	//--------------------------------------------------------------
	/// Push Source
	///
	/// Pushes a source element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushSource(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create new source element
		mCurrentSource = new ColladaSource();
		mCurrentSource.mstrId = inAttributes.getValue("id");
		
		//try and add to parent
		if (mCurrentGeometry != null && mCurrentGeometry.mMesh != null)
			mCurrentGeometry.mMesh.mSources.put(mCurrentSource.mstrId, mCurrentSource);
		else
			Logging.logFatal("mCurrentGeometry.mMesh is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Source
	///
	/// Pops a source element from the element stack.
	//--------------------------------------------------------------
	private void PopSource(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentSource = null;
	}
	//--------------------------------------------------------------
	/// Push Technique Common
	///
	/// Pushes a technique common element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushTechniqueCommon(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//check geometry exists
		if (mCurrentSource != null)
		{
			mCurrentTechniqueCommon = mCurrentSource.mTechniqueCommon;
		}
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Technique Common
	///
	/// Pops a technique common element from the element stack.
	//--------------------------------------------------------------
	private void PopTechniqueCommon(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		mCurrentTechniqueCommon = null;
	}
	//--------------------------------------------------------------
	/// Push Accessor
	///
	/// Pushes an accessor element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushAccessor(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		if (mCurrentTechniqueCommon != null)
		{
			mCurrentTechniqueCommon.mAccessor.mstrSource = inAttributes.getValue("source");
			mCurrentTechniqueCommon.mAccessor.mdwCount = Integer.parseInt(inAttributes.getValue("count"));
			mCurrentTechniqueCommon.mAccessor.mdwStride = Integer.parseInt(inAttributes.getValue("stride"));
		}
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Accessor
	///
	/// Pops an accessor element on to the element stack.
	//--------------------------------------------------------------
	private void PopAccessor(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
	}
	//--------------------------------------------------------------
	/// Push Float Array
	///
	/// Pushes a float array element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushFloatArray(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create the float array
		mCurrentFloatArray = new ColladaFloatArray();
		mCurrentFloatArray.mdwCount = Integer.parseInt(inAttributes.getValue("count"));
		mCurrentFloatArray.mstrId = inAttributes.getValue("id");
		mCurrentFloatArray.mafData = new float[mCurrentFloatArray.mdwCount];
		
		//try and add to parent
		if (mCurrentSource != null)
			mCurrentSource.mFloatArray = mCurrentFloatArray;
		else
			Logging.logFatal("No available parent for float array!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop Float Array
	///
	/// Pops a float array element from the element stack.
	//--------------------------------------------------------------
	private void PopFloatArray(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		//convert to a list of float strings
		String[] astrSplitBuffer = ColladaUtils.SplitOnWhitespace(inData);
		
		if (astrSplitBuffer.length == mCurrentFloatArray.mdwCount)
		{
			//convert the float strings to a list of actual floats
			for (int i = 0; i < astrSplitBuffer.length; i++)
			{
				mCurrentFloatArray.mafData[i] = Float.parseFloat(astrSplitBuffer[i]);
			}
		}
		else
		{
			Logging.logFatal("Incorrect number of values for 'float array'!");
		}
		
		mCurrentFloatArray = null;
	}
	//--------------------------------------------------------------
	/// Push Vertices
	///
	/// Pushes a vertices array element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushVertices(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		mCurrentVertices = new ColladaVertices();
		mCurrentVertices.mstrId = inAttributes.getValue("id");
		
		if (mCurrentGeometry != null && mCurrentGeometry.mMesh != null)
			mCurrentGeometry.mMesh.mVertices = mCurrentVertices;
		else
			Logging.logFatal("No available parent for 'vertices'!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Vertices
	///
	/// Pops a vertices array element on to the element stack.
	//--------------------------------------------------------------
	private void PopVertices(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentVertices = null;
	}
	//--------------------------------------------------------------
	/// Push Triangles
	///
	/// Pushes a triangles element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushTriangles(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create the "traingles"
		mCurrentTriangles = new ColladaTriangles();
		mCurrentTriangles.mdwCount = Integer.parseInt(inAttributes.getValue("count"));
		mCurrentTriangles.mstrMaterial = inAttributes.getValue("material");
		
		//try and add to parent
		if (mCurrentGeometry != null && mCurrentGeometry.mMesh != null)
			mCurrentGeometry.mMesh.mTrianglesList.add(mCurrentTriangles);
		else
			Logging.logFatal("No available mesh for triangles!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Triangles
	///
	/// Pops a triangles element from the element stack.
	//--------------------------------------------------------------
	private void PopTriangles(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentTriangles = null;
	}
	//--------------------------------------------------------------
	/// Push Input
	///
	/// Pushes an input element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushInput(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create the "input"
		ColladaInput input = new ColladaInput();
		input.mstrSemantic = inAttributes.getValue("semantic");
		input.mstrSource = inAttributes.getValue("source");
		input.mdwOffset = ColladaUtils.GetIntAttributeOrZero(inAttributes, "offset");
		input.mdwSet = ColladaUtils.GetIntAttributeOrZero(inAttributes, "set");
		
		//try and add to parent
		if (mCurrentVertices != null)
			mCurrentVertices.mInput = input;
		else if (mCurrentTriangles != null)
			mCurrentTriangles.mInputList.add(input);
		else
			Logging.logFatal("No available parent for 'input'!");
		
		return false;

	}
	//--------------------------------------------------------------
	/// Pop Input
	///
	/// Pops an input element on to the element stack.
	//--------------------------------------------------------------
	private void PopInput(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
	}
	//--------------------------------------------------------------
	/// Push P
	///
	/// Pushes a p element on to the element stack.
	//--------------------------------------------------------------
	private boolean PushP(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create current p
		mCurrentP = new ColladaP();
		
		if (mCurrentTriangles != null)
		{
			mCurrentTriangles.mP = mCurrentP;
			mCurrentTriangles.mP.madwValues = new int[mCurrentTriangles.mdwCount * 3 * mCurrentTriangles.mInputList.size()];
		}else
			Logging.logFatal("No available parent for 'P'!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop P
	///
	/// Pops a p element on to the element stack.
	//--------------------------------------------------------------
	private void PopP(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		//convert to a list of int strings
		String[] astrSplitBuffer = ColladaUtils.SplitOnWhitespace(inData);
		
		if (astrSplitBuffer.length > 0)
		{
			if (mCurrentP.madwValues != null && astrSplitBuffer.length == mCurrentP.madwValues.length)
			{
				for (int i = 0; i < astrSplitBuffer.length; i++)
				{
					mCurrentP.madwValues[i] = Integer.parseInt(astrSplitBuffer[i]);
				}
			}
			else
			{
				Logging.logFatal("Incorrect number of values for 'P'");
			}
		}
		
		mCurrentP = null;
	}
}
