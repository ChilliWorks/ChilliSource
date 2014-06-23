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
