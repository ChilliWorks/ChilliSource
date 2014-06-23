/*
 *  CExtractTextOptions.java
 *  MasterTextTool
 *
 *  Created by Robert Henning on 15/01/2013.
 *  
 *  Copyright 2013 Tag Games Ltd. All rights reserved.
 *
 */

package com.taggames.tools.text;

import com.chillisource.toolutils.Logging;

public class ExtractTextOptions
{
	public String strInputFile;
	public String strOutputDirectory;
	public String strOutputFilename;
	public String strOutputFileExtension;
	public String strLanguage;				// Only one language
	public String strLanguages;				// Comma separated languages
	public String strDevice;
	
	//---------------------------------------------
	/// Constructor
	//---------------------------------------------
	public ExtractTextOptions()
	{
		strInputFile = new String();
		strOutputDirectory = new String();
		strOutputFilename = new String();
		strOutputFileExtension = new String();
		strLanguage = new String();
		strLanguages = new String();
		strDevice = new String();
	}
	//---------------------------------------------
	/// Check options are valid
	//---------------------------------------------
	public boolean IsValid()
	{
		if((strInputFile.length() > 0)
		 &&(strOutputDirectory.length() > 0)
		 &&(strOutputFileExtension.length() > 0))
		{
			if(((strLanguage.length()  > 0) && (strLanguages.length()  > 0))
			 ||((strLanguage.length() == 0) && (strLanguages.length() == 0)))
			{
				Logging.logError("Cannot provide --language and --languages options. You must pick one.");
				return false;
			}
			
			if((strLanguages.length() > 0) && (strOutputFilename.length() > 0))
			{
				Logging.logError("Cannot provide --languages and --outputfilename options. Use --language instead.");
				return false;
			}
			
			return true;
		}
		
		Logging.logError("Missing params. You must provide --inputfile and --outputdirectory");
		return false;
	}
}
