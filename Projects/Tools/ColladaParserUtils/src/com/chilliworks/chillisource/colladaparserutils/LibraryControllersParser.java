/**
 * LibraryAnimationsParser.java
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

public class LibraryControllersParser 
{
	//--------------------------------------------------------------
	/// Private Member Data
	//--------------------------------------------------------------
	private int mdwIgnoreStack;
	private Collada mCollada;
	private ColladaController mCurrentController;
	private ColladaBindShapeMatrix mCurrentBindShapeMatrix;
	private ColladaSource mCurrentSource;
	private ColladaFloatArray mCurrentFloatArray;
	private ColladaNameArray mCurrentNameArray;
	private ColladaJoints mCurrentJoints;
	private ColladaVertexWeights mCurrentVertexWeights;
	private ColladaVCount mCurrentVCount;
	private ColladaV mCurrentV;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public LibraryControllersParser(Collada inColladaData)
	{
		mCollada = inColladaData;
		mdwIgnoreStack = 0;
	}
	//--------------------------------------------------------------
	/// Start Element
	///
	/// Callback method used by SAX whenever a new element is found.
	//--------------------------------------------------------------
	public boolean StartElement(String instrUri, String instrLocalName,String instrQName, Attributes inAttributes) throws SAXException 
	{
		//push element type on to the imaginary element stack
		if (instrQName.equalsIgnoreCase("controller"))
			return PushController(inAttributes);
		else if (instrQName.equalsIgnoreCase("skin"))
			return PushSkin(inAttributes);
		else if (instrQName.equalsIgnoreCase("bind_shape_matrix"))
			return PushBindShapeMatrix(inAttributes);
		else if (instrQName.equalsIgnoreCase("source"))
			return PushSource(inAttributes);
		else if (instrQName.equalsIgnoreCase("float_array"))
			return PushFloatArray(inAttributes);
		else if (instrQName.equalsIgnoreCase("name_array"))
			return PushNameArray(inAttributes);
		else if (instrQName.equalsIgnoreCase("joints"))
			return PushJoints(inAttributes);
		else if (instrQName.equalsIgnoreCase("input"))
			return PushInput(inAttributes);
		else if (instrQName.equalsIgnoreCase("vertex_weights"))
			return PushVertexWeights(inAttributes);
		else if (instrQName.equalsIgnoreCase("vcount"))
			return PushVCount(inAttributes);
		else if (instrQName.equalsIgnoreCase("v"))
			return PushV(inAttributes);
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
		if (instrQName.equalsIgnoreCase("controller"))
			PopController(inData);
		else if (instrQName.equalsIgnoreCase("skin"))
			PopSkin(inData);
		else if (instrQName.equalsIgnoreCase("bind_shape_matrix"))
			PopBindShapeMatrix(inData);
		else if (instrQName.equalsIgnoreCase("source"))
			PopSource(inData);
		else if (instrQName.equalsIgnoreCase("float_array"))
			PopFloatArray(inData);
		else if (instrQName.equalsIgnoreCase("name_array"))
			PopNameArray(inData);
		else if (instrQName.equalsIgnoreCase("joints"))
			PopJoints(inData);
		else if (instrQName.equalsIgnoreCase("input"))
			PopInput(inData);
		else if (instrQName.equalsIgnoreCase("vertex_weights"))
			PopVertexWeights(inData);
		else if (instrQName.equalsIgnoreCase("vcount"))
			PopVCount(inData);
		else if (instrQName.equalsIgnoreCase("v"))
			PopV(inData);
		else
			mdwIgnoreStack--;
			
	}
	//--------------------------------------------------------------
	/// Push Controller
	///
	/// Pushes a controller element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushController(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		mCurrentController = new ColladaController();
		mCurrentController.mstrId = inAttributes.getValue("id");
		
		if (mCollada.mLibraryControllers != null)
			mCollada.mLibraryControllers.put(mCurrentController.mstrId, mCurrentController);
		else
			Logging.logFatal("mLibraryControllers is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Controller
	///
	/// Pops a controller element type from the element stack.
	//--------------------------------------------------------------
	private void PopController(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentController = null;
	}
	//--------------------------------------------------------------
	/// Push Skin
	///
	/// Pushes a skin element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushSkin(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		ColladaSkin newSkin = new ColladaSkin();
		newSkin.mstrSource = inAttributes.getValue("source");
		
		if (mCurrentController != null)
			mCurrentController.mSkin = newSkin;
		else
			Logging.logFatal("mCurrentController is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Skin
	///
	/// Pops a skin element type from the element stack.
	//--------------------------------------------------------------
	private void PopSkin(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
	}
	//--------------------------------------------------------------
	/// Push Bind Shape Matrix
	///
	/// Pushes a bind shape matrix element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushBindShapeMatrix(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		mCurrentBindShapeMatrix = new ColladaBindShapeMatrix();
		
		if (mCurrentController != null && mCurrentController.mSkin != null)
			mCurrentController.mSkin.mBindShapeMatrix = mCurrentBindShapeMatrix;
		else
			Logging.logFatal("mBindShapeMatrix is null!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop Bind Shape Matrix
	///
	/// Pops a bind shape matrix element type from the element stack.
	//--------------------------------------------------------------
	private void PopBindShapeMatrix(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		String[] astrSplitBuffer = ColladaUtils.SplitOnWhitespace(inData);
		
		mCurrentBindShapeMatrix.maafValues = new float[4][4];
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				mCurrentBindShapeMatrix.maafValues[i][j] = Float.parseFloat(astrSplitBuffer[i * 4 + j]);
			}
		}
		
		mCurrentBindShapeMatrix = null;
	}
	//--------------------------------------------------------------
	/// Push Source
	///
	/// Pushes a source element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushSource(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create new source element
		mCurrentSource = new ColladaSource();
		mCurrentSource.mstrId = inAttributes.getValue("id");
		
		//try and add to parent
		if (mCurrentController != null && mCurrentController.mSkin != null)
			mCurrentController.mSkin.mSourceTable.put(mCurrentSource.mstrId, mCurrentSource);
		else
			Logging.logFatal("mCurrentController.mSkin is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Source
	///
	/// Pops a source element type from the element stack.
	//--------------------------------------------------------------
	private void PopSource(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentSource = null;
	}
	//--------------------------------------------------------------
	/// Push Float Array
	///
	/// Pushes a float array element type onto the element stack.
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
	/// Pops a float array element type from the element stack.
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
	/// Push Name Array
	///
	/// Pushes a name array element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushNameArray(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create the float array
		mCurrentNameArray = new ColladaNameArray();
		mCurrentNameArray.mdwCount = Integer.parseInt(inAttributes.getValue("count"));
		mCurrentNameArray.mstrId = inAttributes.getValue("id");
		mCurrentNameArray.mstrData = new String[mCurrentNameArray.mdwCount];
		
		//try and add to parent
		if (mCurrentSource != null)
			mCurrentSource.mNameArray = mCurrentNameArray;
		else
			Logging.logFatal("No available parent for name array!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop Name Array
	///
	/// Pops a name array element type from the element stack.
	//--------------------------------------------------------------
	private void PopNameArray(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		//convert to a list of float strings
		String[] astrSplitBuffer = ColladaUtils.SplitOnWhitespace(inData);
		
		if (astrSplitBuffer.length == mCurrentNameArray.mdwCount)
		{
			//convert the float strings to a list of actual floats
			for (int i = 0; i < astrSplitBuffer.length; i++)
			{
				mCurrentNameArray.mstrData[i] = astrSplitBuffer[i];
			}
		}
		else
		{
			Logging.logFatal("Incorrect number of values for 'name array'!");
		}
		
		mCurrentNameArray = null;
	}
	//--------------------------------------------------------------
	/// Push Joints
	///
	/// Pushes a joints element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushJoints(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		mCurrentJoints = new ColladaJoints();
		
		if (mCurrentController != null && mCurrentController.mSkin != null)
			mCurrentController.mSkin.mJoints = mCurrentJoints;
		else
			Logging.logFatal("mCurrentController.mSkin is null!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop Joints
	///
	/// Pops a joints element type from the element stack.
	//--------------------------------------------------------------
	private void PopJoints(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentJoints = null;
	}
	//--------------------------------------------------------------
	/// Push Input
	///
	/// Pushes an input element type on to the element stack.
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
		if (mCurrentJoints != null)
			mCurrentJoints.mInputs.add(input);
		else if (mCurrentVertexWeights != null)
			mCurrentVertexWeights.mInputs.add(input);
		else
			Logging.logFatal("No available parent for 'input'!");
		
		return false;

	}
	//--------------------------------------------------------------
	/// Pop Input
	///
	/// Pops an input element type from the element stack.
	//--------------------------------------------------------------
	private void PopInput(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
	}
	//--------------------------------------------------------------
	/// Push Vertex Weights
	///
	/// Pushes a vertex weights element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushVertexWeights(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create the "input"
		mCurrentVertexWeights = new ColladaVertexWeights();
		mCurrentVertexWeights.mdwCount = Integer.parseInt(inAttributes.getValue("count"));
		
		//try and add to parent
		if (mCurrentController != null && mCurrentController.mSkin != null)
			mCurrentController.mSkin.mVertexWeights = mCurrentVertexWeights;
		else
			Logging.logFatal("mCurrentController.mSkin is null!");
		
		return false;

	}
	//--------------------------------------------------------------
	/// Pops Vertex Weights
	///
	/// Pops a vertex weights element type from the element stack.
	//--------------------------------------------------------------
	private void PopVertexWeights(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentVertexWeights = null;
	}
	//--------------------------------------------------------------
	/// Push VCount
	///
	/// Pushes a vcount element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushVCount(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create current p
		mCurrentVCount = new ColladaVCount();
		
		if (mCurrentVertexWeights != null)
		{
			mCurrentVertexWeights.mVCount = mCurrentVCount;
		}else
			Logging.logFatal("No available parent for 'vcount'!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop VCount
	///
	/// Pops a vcount element type from the element stack.
	//--------------------------------------------------------------
	private void PopVCount(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		//convert to a list of int strings
		String[] astrSplitBuffer = ColladaUtils.SplitOnWhitespace(inData);
		
		if (astrSplitBuffer.length > 0)
		{
			mCurrentVCount.madwData = new int[astrSplitBuffer.length];
			
			for (int i = 0; i < astrSplitBuffer.length; i++)
			{
				mCurrentVCount.madwData[i] = Integer.parseInt(astrSplitBuffer[i]);
			}
		}
		
		mCurrentVCount = null;
	}
	//--------------------------------------------------------------
	/// Push V
	///
	/// Pushes a v element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushV(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create current p
		mCurrentV = new ColladaV();
		
		if (mCurrentVertexWeights != null)
		{
			mCurrentVertexWeights.mV = mCurrentV;
		}else
			Logging.logFatal("No available parent for 'v'!");
		
		return true;
	}
	//--------------------------------------------------------------
	/// Pop V
	///
	/// Pops a v element type from the element stack.
	//--------------------------------------------------------------
	private void PopV(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		//convert to a list of int strings
		String[] astrSplitBuffer = ColladaUtils.SplitOnWhitespace(inData);
		
		if (astrSplitBuffer.length > 0)
		{
			mCurrentV.madwData = new int[astrSplitBuffer.length];

			for (int i = 0; i < astrSplitBuffer.length; i++)
			{
				mCurrentV.madwData[i] = Integer.parseInt(astrSplitBuffer[i]);
			}
		}
		
		mCurrentVCount = null;
	}
}
