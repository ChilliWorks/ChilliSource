/*
 *  CMain.java
 *  MasterTextTool
 *
 *  Created by Robert Henning on 15/01/2013.
 *  Refactoring using new 'Tool Utils' base. Previous version
 *  is 1.2 using a single source file (TextExtractor.java). Use
 *  this for any older projects that require .h/.java output.
 *  
 *  Copyright 2013 Tag Games Ltd. All rights reserved.
 *
 */

package com.taggames.tools.text;

import java.util.Hashtable;

import com.taggames.toolutils.SCLogger;

public class CMain
{
	private static final String		TEXT_EXTENSION		= "mofloloca";
    
    public static void main(String inastrArgs[]) throws Exception 
	{
    	SCLogger.LogMessage("Text Extractor; Version 1.3.");
		SCLogger.Initialise(inastrArgs);
		
		try
		{
			ExtractText(inastrArgs);
		}
		catch (Exception e)
		{
			SCLogger.LogFatalError("An exception has occurred: \n" + e.getStackTrace());
		}
		
		SCLogger.PrintErrorsAndWarnings();
	}
    
	//------------------------------------------------------
	/// Extract Text
	///
	/// Extracts options from the given arguments and kicks
    /// off the text extraction process
	//------------------------------------------------------
    private static void ExtractText(String inastrArgs[])
    {
    	ExtractTextOptions Options = new ExtractTextOptions();
    	
    	// Assumes all arguments are key/value
    	Hashtable<String, String> Arguments = new Hashtable<String, String>();
        for(int i = 0; i < inastrArgs.length; i++)
        {
        	Arguments.put(inastrArgs[i], inastrArgs[i + 1]);
            i++;
        }
        
        if(Arguments.containsKey("--inputfile"))
        {
        	Options.strInputFile = Arguments.get("--inputfile");
        }
        
        if(Arguments.containsKey("--outputdirectory"))
        {
        	Options.strOutputDirectory = Arguments.get("--outputdirectory");
        }
        
        if(Arguments.containsKey("--outputfilename"))
        {
        	Options.strOutputFilename = Arguments.get("--outputfilename");
        }
        
        if(Arguments.containsKey("--outputfileextension"))
        {
        	Options.strOutputFileExtension = Arguments.get("--outputfileextension");
        }
        else
        {
        	Options.strOutputFileExtension = TEXT_EXTENSION;
        }
        
        if(Arguments.containsKey("--language"))
        {
        	Options.strLanguage = Arguments.get("--language");
        }
        
        if(Arguments.containsKey("--languages"))
        {
        	Options.strLanguages = Arguments.get("--languages");
        }
        
        if(Arguments.containsKey("--device"))
        {
        	Options.strDevice = Arguments.get("--device");
        }
        
        if(!Options.IsValid())
        {
        	SCLogger.LogError("Arguments are invalid. Cannot continue.");
        	PrintHelpText();
        	return;
        }
        
        SCTextExtractor.Start(Options);
    }
	//------------------------------------------------------
	/// Print Help Text
	///
	/// Prints out instructions on how to use this tool.
	//------------------------------------------------------
	private static void PrintHelpText()
	{
		SCLogger.SetLoggingLevel(SCLogger.LOGGING_LEVEL_VERBOSE);
		SCLogger.LogMessage("java -jar TextExtractor.jar" +
				"[--logginglevel or -l <level>]" +
				"[--errordisplay or -e <errordisplay>]" +
				"[--inputfile <.xls file>]" +
				"[--outputdirectory <path>]" +
				"[--language <language>]" +
				"[--languages <lang1,lang2,lang3]" +
				"[--outputfilename <filename>]" +
				"[--outputfileextension <extension>]");
		SCLogger.LogMessage("\t--logginglevel(-l) \t-> Sets the logging level. Possible values are: 'none', 'fatal', 'error', 'warning', and 'verbose'. The default is value is 'warning'");
		SCLogger.LogMessage("\t--errordisplay(-e) \t-> Sets when errors should be displayed. Possible values are: 'never', 'atend', and 'whenreceived'. The default value is 'atend'");
		SCLogger.LogMessage("\t--inputfile \t\t-> Source .xls file to extract text from");
		SCLogger.LogMessage("\t--outputdirectory \t-> Path when output will be written to");
		SCLogger.LogMessage("\t--language \t\t-> Name of language column in source .xls to extract text from (cannot use with --languages)");
		SCLogger.LogMessage("\t--languages \t\t-> Comma separated list of language columns in source .xls to extract text from  (cannot use with --language)");
		SCLogger.LogMessage("\t--outputfilename \t-> [optional] if you want to specify an filename for extracted text (defaults to language, cannot be used with --languages)");
		SCLogger.LogMessage("\t--outputfileextension \t-> [optional] if you want to specify a file extension (defaults to mofloloca)");
	}
}
