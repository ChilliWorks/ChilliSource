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

public class LibraryAnimationsParser 
{
	//--------------------------------------------------------------
	/// Private Member Data
	//--------------------------------------------------------------
	private int mdwIgnoreStack;
	private Collada mCollada;
	private ColladaAnimation mCurrentAnimation;
	private ColladaSource mCurrentSource;
	private ColladaFloatArray mCurrentFloatArray;
	private ColladaNameArray mCurrentNameArray;
	private ColladaSampler mCurrentSampler;
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public LibraryAnimationsParser(Collada inColladaData)
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
		if (instrQName.equalsIgnoreCase("animation"))
			return PushAnimation(inAttributes);
		else if (instrQName.equalsIgnoreCase("source"))
			return PushSource(inAttributes);
		else if (instrQName.equalsIgnoreCase("float_array"))
			return PushFloatArray(inAttributes);
		else if (instrQName.equalsIgnoreCase("name_array"))
			return PushNameArray(inAttributes);
		else if (instrQName.equalsIgnoreCase("sampler"))
			return PushSampler(inAttributes);
		else if (instrQName.equalsIgnoreCase("input"))
			return PushInput(inAttributes);
		else if (instrQName.equalsIgnoreCase("channel"))
			return PushChannel(inAttributes);
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
		if (instrQName.equalsIgnoreCase("animation"))
			PopAnimation(inData);
		else if (instrQName.equalsIgnoreCase("source"))
			PopSource(inData);
		else if (instrQName.equalsIgnoreCase("float_array"))
			PopFloatArray(inData);
		else if (instrQName.equalsIgnoreCase("name_array"))
			PopNameArray(inData);
		else if (instrQName.equalsIgnoreCase("sampler"))
			PopSampler(inData);
		else if (instrQName.equalsIgnoreCase("input"))
			PopInput(inData);
		else if (instrQName.equalsIgnoreCase("channel"))
			PopChannel(inData);
		else
			mdwIgnoreStack--;
	}
	//--------------------------------------------------------------
	/// Push Animation
	///
	/// Pushes the animation element type onto the element stack.
	//--------------------------------------------------------------
	private boolean PushAnimation(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		mCurrentAnimation = new ColladaAnimation();
		
		if (mCollada.mAnimation != null)
			mCollada.mAnimation = mCurrentAnimation;
		else
			Logging.logFatal("mCurrentAnimation is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Animation
	///
	/// Pops the animation element type from the element stack.
	//--------------------------------------------------------------
	private void PopAnimation(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentAnimation = null;
	}
	//--------------------------------------------------------------
	/// Push Source
	///
	/// Pushes the source element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushSource(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create new source element
		mCurrentSource = new ColladaSource();
		mCurrentSource.mstrId = inAttributes.getValue("id");
		
		//try and add to parent
		if (mCurrentAnimation != null)
			mCurrentAnimation.mSourceTable.put(mCurrentSource.mstrId, mCurrentSource);
		else
			Logging.logFatal("mCurrentController.mSkin is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Source
	///
	/// Pops the source element type from the element stack.
	//--------------------------------------------------------------
	private void PopSource(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentSource = null;
	}
	//--------------------------------------------------------------
	/// Push Float Array
	///
	/// Pushes the float array element type on to the element stack.
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
	/// Pops the float array element type from the element stack.
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
	/// Pushes the name array element type on to the element stack.
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
	/// Pops the name array element type from the element stack.
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
	/// Push Sampler
	///
	/// Pushes the sampler element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushSampler(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create new source element
		mCurrentSampler = new ColladaSampler();
		mCurrentSampler.mstrId = inAttributes.getValue("id");
		
		//try and add to parent
		if (mCurrentAnimation != null)
			mCurrentAnimation.mSamplerTable.put(mCurrentSampler.mstrId, mCurrentSampler);
		else
			Logging.logFatal("mCurrentAnimation is null!");
		
		return false;
	}
	//--------------------------------------------------------------
	/// Pop Sampler
	///
	/// Pops the sampler element type from the element stack.
	//--------------------------------------------------------------
	private void PopSampler(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
		
		mCurrentSampler = null;
	}
	//--------------------------------------------------------------
	/// Push Input
	///
	/// Push the input element type on to the element stack.
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
		if (mCurrentSampler != null)
			mCurrentSampler.mInputTable.put(input.mstrSemantic, input);
		else
			Logging.logFatal("No available parent for 'input'!");
		
		return false;

	}
	//--------------------------------------------------------------
	/// Pop Input
	///
	/// Pops the input element type on to the element stack.
	//--------------------------------------------------------------
	private void PopInput(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
	}
	//--------------------------------------------------------------
	/// Push Channel
	///
	/// Pushes the channel element type on to the element stack.
	//--------------------------------------------------------------
	private boolean PushChannel(Attributes inAttributes)
	{
		if (mdwIgnoreStack > 0 ) return false;
		
		//create the "input"
		ColladaChannel channel = new ColladaChannel();
		channel.mstrTarget = inAttributes.getValue("target");
		channel.mstrSource = inAttributes.getValue("source");
		
		//try and add to parent
		if (mCurrentAnimation != null)
			mCurrentAnimation.mChannelList.add(channel);
		else
			Logging.logFatal("mCurrentAnimation is null!");
		
		return false;

	}
	//--------------------------------------------------------------
	/// Pop Channel
	///
	/// Pops the channel element type from the element stack.
	//--------------------------------------------------------------
	private void PopChannel(String inData)
	{
		if (mdwIgnoreStack > 0 ) return;
	}
}
