package com.chillisource.colladatocsanim;

import com.chillisource.colladatocsanim.csanim.*;
import com.chillisource.toolutils.Quaternion;
import com.chillisource.toolutils.Vector3;

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
				Vector3 translation = frame.mNodeTranslations.get(j);
				Quaternion orientation = frame.mNodeOrienations.get(j);
				Vector3 scale = frame.mNodeScalings.get(j);
				
				float temp = translation.y;
				translation.y = translation.z;
				translation.z = temp;
				
				Vector3 vAxis = orientation.getAxis();
				float fAngle = orientation.getAngle();
				temp = vAxis.y;
				vAxis.y = vAxis.z;
				vAxis.z = temp;
				fAngle = -fAngle;
				Quaternion newOrientation = Quaternion.createFromAxisAngle(vAxis, fAngle);
				orientation.set(newOrientation);
				
				temp = scale.y;
				scale.y = scale.z;
				scale.z = temp;
			}
		}
	}
}
