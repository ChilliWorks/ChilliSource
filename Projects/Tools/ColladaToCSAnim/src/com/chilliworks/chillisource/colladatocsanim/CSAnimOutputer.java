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

package com.chilliworks.chillisource.colladatocsanim;

import java.io.DataOutputStream;
import java.io.FileOutputStream;

import com.chilliworks.chillisource.colladatocsanim.csanim.*;
import com.chilliworks.chillisource.coreutils.LittleEndianWriterUtils;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.Quaternion;
import com.chilliworks.chillisource.coreutils.Vector3;

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
	DataOutputStream mStream;
	
	/**
	 * Constructor
	 */
	public CSAnimOutputer()
	{
		
	}
	
	/**
	 * Output
	 */
	public boolean Output(ColladaToCSAnimOptions inConversionParams, CSAnim inMoModel) throws Exception
	{
		boolean bSuccess = true;
		
		//try and open a new file stream. if this fails, throw a fatal error.
		try
		{	
			mStream = new DataOutputStream(new FileOutputStream(inConversionParams.m_outputFilePath));
		}
		catch (Exception e)
		{
			if(mStream != null)
			{
				mStream.close();
			}
			Logging.logFatal("Failed to open output file: " + inConversionParams.m_outputFilePath);
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
			Logging.logFatal("Failed to write to file: " + inConversionParams.m_outputFilePath);
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
	public boolean WriteHeader(ColladaToCSAnimOptions inConversionParams, CSAnim inAnim) throws Exception
	{
		//write endianness check and version number
		LittleEndianWriterUtils.writeUInt32(mStream, ENDIANESS_CHECK_VALUE);
		LittleEndianWriterUtils.writeUInt32(mStream, VERSION_NUMBER);
		
		//declare that there are no features
		mStream.writeByte((byte)0);
		
		//Write the number of frames and joints
		LittleEndianWriterUtils.writeUInt16(mStream, inAnim.mFrames.size());
		LittleEndianWriterUtils.writeInt16(mStream, (short)inAnim.mSkeleton.mNodeList.size());
		
		//Write the frame rate
		LittleEndianWriterUtils.writeFloat32(mStream, inAnim.mfFrameRate);
		
		return true;
	}
	
	/**
	 * Writes the MoAnim body
	 * @param inConversionParams
	 * @param inAnim
	 * @return
	 */
	public boolean WriteBody(ColladaToCSAnimOptions inConversionParams, CSAnim inAnim) throws Exception
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
				
				LittleEndianWriterUtils.writeFloat32(mStream, (float)translation.getX());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)translation.getY());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)translation.getZ());
				
				LittleEndianWriterUtils.writeFloat32(mStream, (float)orientation.getX());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)orientation.getY());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)orientation.getZ());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)orientation.getW());

				LittleEndianWriterUtils.writeFloat32(mStream, (float)scale.getX());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)scale.getY());
				LittleEndianWriterUtils.writeFloat32(mStream, (float)scale.getZ());
			}
		}

		return true;
	}
	
}
