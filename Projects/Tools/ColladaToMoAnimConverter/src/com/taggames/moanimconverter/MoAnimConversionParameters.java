package com.taggames.moanimconverter;

public class MoAnimConversionParameters 
{
	public String mstrInputFilepath;
	public String mstrOutputFilepath;
	public boolean mbSwapYAndZ;
	public boolean mbFlipVerticalTexCoords;
	public boolean mbMirrorInXZPlane;
	
	public MoAnimConversionParameters()
	{
		mstrInputFilepath 		= new String();
		mstrOutputFilepath 		= new String();
		mbSwapYAndZ 		= false;
		mbFlipVerticalTexCoords = false;
		mbMirrorInXZPlane 		= false;
	}
}
