package com.taggames.premultipliedalphapngtool;

public class PremultiplicationOptions 
{
	public String strInputFilename;
	public String strOutputFilename;
	public boolean bShouldCache;
	
	//---------------------------------------------
	/// Constructor
	//---------------------------------------------
	public PremultiplicationOptions()
	{
		strInputFilename = new String();
		strOutputFilename = new String();
		bShouldCache = false;
	}
}
