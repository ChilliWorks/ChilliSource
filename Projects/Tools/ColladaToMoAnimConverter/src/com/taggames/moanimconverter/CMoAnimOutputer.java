package com.taggames.moanimconverter;

import com.taggames.moanimconverter.moanim.*;
import com.taggames.toolutils.CLittleEndianOutputStream;
import com.taggames.toolutils.CQuaternion;
import com.taggames.toolutils.CVector3;
import com.taggames.toolutils.SCLogger;


public class CMoAnimOutputer 
{
	/**
	 * Constants
	 */
	private static final int ENDIANESS_CHECK_VALUE = 7777;
	private static final int VERSION_NUMBER = 4;
	private static final String FILE_EXTENSION = ".moanim";
	
	/**
	 * Private Data
	 */
	CLittleEndianOutputStream mStream;
	
	/**
	 * Constructor
	 */
	public CMoAnimOutputer()
	{
		
	}
	
	/**
	 * Output
	 */
	public boolean Output(MoAnimConversionParameters inConversionParams, MoAnim inMoModel) throws Exception
	{
		boolean bSuccess = true;
		
		//try and open a new file stream. if this fails, throw a fatal error.
		try
		{
			String strOutputFilepath = inConversionParams.mstrOutputFilepath;
			if (inConversionParams.mstrOutputFilepath.endsWith(FILE_EXTENSION) == false)
				strOutputFilepath += FILE_EXTENSION;
				
			mStream = new CLittleEndianOutputStream(strOutputFilepath);
		}
		catch (Exception e)
		{
			if(mStream != null)
			{
				mStream.Close();
			}
			SCLogger.LogFatalError("Failed to open output file: " + inConversionParams.mstrOutputFilepath);
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
			mStream.Close();
			SCLogger.LogFatalError("Failed to write to file: " + inConversionParams.mstrOutputFilepath);
		}
		
		//close the filestream
		mStream.Close();

		return bSuccess;
	}
	
	/**
	 * Writes the MoAnim header
	 * @param inConversionParams
	 * @param inAnim
	 * @return
	 */
	public boolean WriteHeader(MoAnimConversionParameters inConversionParams, MoAnim inAnim) throws Exception
	{
		//write endianness check and version number
		mStream.WriteUnsignedInt(ENDIANESS_CHECK_VALUE);
		mStream.WriteUnsignedInt(VERSION_NUMBER);
		
		//declare that there are no features
		mStream.WriteByte((byte)0);
		
		//Write the number of frames and joints
		mStream.WriteUnsignedShort(inAnim.mFrames.size());
		mStream.WriteShort((short)inAnim.mSkeleton.mNodeList.size());
		
		//Write the frame rate
		mStream.WriteFloat(inAnim.mfFrameRate);
		
		return true;
	}
	
	/**
	 * Writes the MoAnim body
	 * @param inConversionParams
	 * @param inAnim
	 * @return
	 */
	public boolean WriteBody(MoAnimConversionParameters inConversionParams, MoAnim inAnim) throws Exception
	{
		//loop through each joint in each frame
		for (int i = 0; i < inAnim.mFrames.size(); i++)
		{
			MoAnimFrame frame = inAnim.mFrames.get(i);
			
			for (int j = 0; j < frame.mNodeTranslations.size(); j++)
			{
				CVector3 translation = frame.mNodeTranslations.get(j);
				CQuaternion orientation = frame.mNodeOrienations.get(j);
				CVector3 scale = frame.mNodeScalings.get(j);
				
				mStream.WriteFloat(translation.x);
				mStream.WriteFloat(translation.y);
				mStream.WriteFloat(translation.z);
				
				mStream.WriteFloat(orientation.x);
				mStream.WriteFloat(orientation.y);
				mStream.WriteFloat(orientation.z);
				mStream.WriteFloat(orientation.w);
				
				mStream.WriteFloat(scale.x);
				mStream.WriteFloat(scale.y);
				mStream.WriteFloat(scale.z);
			}
		}

		return true;
	}
	
}
