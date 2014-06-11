package com.chillisource.csanimconverter;

public class CSAnimConversionParameters 
{
	public String mstrInputFilepath;
	public String mstrOutputFilepath;
	public boolean mbSwapYAndZ;
	public boolean mbFlipVerticalTexCoords;
	public boolean mbMirrorInXZPlane;
	
	public CSAnimConversionParameters()
	{
		mstrInputFilepath 		= new String();
		mstrOutputFilepath 		= new String();
		mbSwapYAndZ 		= false;
		mbFlipVerticalTexCoords = false;
		mbMirrorInXZPlane 		= false;
	}
}
