package com.chillisource.csanimconverter;

import com.chillisource.csanimconverter.csanim.*;
import com.taggames.toolutils.CQuaternion;
import com.taggames.toolutils.CVector3;

public class CSAnimTransformer 
{
	
	/**
	 * Constructor
	 */
	public CSAnimTransformer()
	{
		
	}
	
	/**
	 * This modifies the data in the MoModel to the desired output. for example, changing from Z as up to Y as up.
	 * @param inConversionParams
	 * @param inMoModel
	 */
	public void Modify(CSAnimConversionParameters inConversionParams, CSAnim inMoModel)
	{
		if (inConversionParams.mbSwapYAndZ == true)
		{
			SwapYAndZ(inMoModel);
		}
	}
	
	/**
	 * Switches the x and y in the transform, such that Y is now the up axis.
	 * @param inMoModel
	 */
	private void SwapYAndZ(CSAnim inAnim)
	{
		for (int i = 0; i < inAnim.mFrames.size(); i++)
		{
			CSAnimFrame frame = inAnim.mFrames.get(i);
			
			for (int j = 0; j < frame.mNodeTranslations.size(); j++)
			{
				CVector3 translation = frame.mNodeTranslations.get(j);
				CQuaternion orientation = frame.mNodeOrienations.get(j);
				CVector3 scale = frame.mNodeScalings.get(j);
				
				float temp = translation.y;
				translation.y = translation.z;
				translation.z = temp;
				
				CVector3 vAxis = orientation.GetAxis();
				float fAngle = orientation.GetAngle();
				temp = vAxis.y;
				vAxis.y = vAxis.z;
				vAxis.z = temp;
				fAngle = -fAngle;
				CQuaternion newOrientation = CQuaternion.CreateFromAxisAngle(vAxis, fAngle);
				orientation.Set(newOrientation);
				
				temp = scale.y;
				scale.y = scale.z;
				scale.z = temp;
			}
		}
	}
}
