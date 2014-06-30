/**
 * CSAnimOutputter.java
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

package com.chillisource.colladatocsanim;

import com.chillisource.colladatocsanim.csanim.*;
import com.chillisource.toolutils.LittleEndianOutputStream;
import com.chillisource.toolutils.Quaternion;
import com.chillisource.toolutils.Vector3;
import com.chillisource.toolutils.Logging;

public class CSAnimOutputer 
{
	/**
	 * Constants
	 */
	private static final int ENDIANESS_CHECK_VALUE = 7777;
	private static final int VERSION_NUMBER = 4;
	
	/**
	 * Private Data
	 */
	LittleEndianOutputStream mStream;
	
	/**
	 * Constructor
	 */
	public CSAnimOutputer()
	{
		
	}
	
	/**
	 * Output
	 */
	public boolean Output(CSAnimConversionParameters inConversionParams, CSAnim inMoModel) throws Exception
	{
		boolean bSuccess = true;
		
		//try and open a new file stream. if this fails, throw a fatal error.
		try
		{	
			mStream = new LittleEndianOutputStream(inConversionParams.mstrOutputFilepath);
		}
		catch (Exception e)
		{
			if(mStream != null)
			{
				mStream.close();
			}
			Logging.logFatal("Failed to open output file: " + inConversionParams.mstrOutputFilepath);
		}
		
		//output the file
		try
		{
			if (mStream != null)
			{
				bSuccess = WriteHeader(inConversionParams, inMoModel);
				if (bSuccess == true)
					bSuccess = WriteBody(inConversionParams, inMoModel);
			}
		}
		catch (Exception e)
		{
			mStream.close();
			Logging.logFatal("Failed to write to file: " + inConversionParams.mstrOutputFilepath);
		}
		
		//close the filestream
		mStream.close();

		return bSuccess;
	}
	
	/**
	 * Writes the MoAnim header
	 * @param inConversionParams
	 * @param inAnim
	 * @return
	 */
	public boolean WriteHeader(CSAnimConversionParameters inConversionParams, CSAnim inAnim) throws Exception
	{
		//write endianness check and version number
		mStream.writeUnsignedInt(ENDIANESS_CHECK_VALUE);
		mStream.writeUnsignedInt(VERSION_NUMBER);
		
		//declare that there are no features
		mStream.writeByte((byte)0);
		
		//Write the number of frames and joints
		mStream.writeUnsignedShort(inAnim.mFrames.size());
		mStream.writeShort((short)inAnim.mSkeleton.mNodeList.size());
		
		//Write the frame rate
		mStream.writeFloat(inAnim.mfFrameRate);
		
		return true;
	}
	
	/**
	 * Writes the MoAnim body
	 * @param inConversionParams
	 * @param inAnim
	 * @return
	 */
	public boolean WriteBody(CSAnimConversionParameters inConversionParams, CSAnim inAnim) throws Exception
	{
		//loop through each joint in each frame
		for (int i = 0; i < inAnim.mFrames.size(); i++)
		{
			CSAnimFrame frame = inAnim.mFrames.get(i);
			
			for (int j = 0; j < frame.mNodeTranslations.size(); j++)
			{
				Vector3 translation = frame.mNodeTranslations.get(j);
				Quaternion orientation = frame.mNodeOrienations.get(j);
				Vector3 scale = frame.mNodeScalings.get(j);
				
				mStream.writeFloat(translation.x);
				mStream.writeFloat(translation.y);
				mStream.writeFloat(translation.z);
				
				mStream.writeFloat(orientation.x);
				mStream.writeFloat(orientation.y);
				mStream.writeFloat(orientation.z);
				mStream.writeFloat(orientation.w);
				
				mStream.writeFloat(scale.x);
				mStream.writeFloat(scale.y);
				mStream.writeFloat(scale.z);
			}
		}

		return true;
	}
	
}
