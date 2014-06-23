/*
 *  SCTextExtractor.java
 *  MasterTextTool
 *
 *  Created by Robert Henning on 15/01/2013.
 *  Main text extraction process is handled here
 *  
 *  Copyright 2013 Tag Games Ltd. All rights reserved.
 *
 */

package com.taggames.tools.text;

import java.io.File;

import com.chillisource.toolutils.LittleEndianOutputStream;
import com.chillisource.toolutils.FileUtils;
import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.StringUtils;

import jxl.Cell;
import jxl.CellType;
import jxl.Sheet;
import jxl.Workbook;
import jxl.WorkbookSettings;

public class SCTextExtractor
{
	private static final String	kstrConstantColumnHeading = "Constant";
	private static final String kstrExcludeColumnHeading = "Exclude";
	
	private static void OpenExcelFile(ExtractTextOptions inOptions) throws Exception
	{
		File f = new File(inOptions.strInputFile);
		Logging.logVerbose("Input file path is \""+f.getCanonicalPath()+"\"");
        
        if(!f.exists())
        {
        	Logging.logFatal("Input file \""+inOptions.strInputFile+"\" does not exist.");
        }

        if(!f.isFile())
        {
        	Logging.logFatal("Input file \""+inOptions.strInputFile+"\" is not a valid file.");
        }

        if(!f.canRead())
        {
        	Logging.logFatal("Input file \""+inOptions.strInputFile+"\" cannot be read.");
        }

        Logging.logVerbose("Opening workbook...");
        WorkbookSettings ws = new WorkbookSettings();
        if (Logging.getLoggingLevel() != Logging.LOGGING_LEVEL_VERBOSE && Logging.getLoggingLevel() != Logging.LOGGING_LEVEL_WARNING)
        {
        	ws.setSuppressWarnings(true);
        }
        ws.setEncoding("Cp1252");
        mWorkbook = Workbook.getWorkbook(f, ws);
	}
	
	private static void CloseExcelFile()
	{
		try
		{
			if(mWorkbook != null)
	        {
				Logging.logVerbose("Closing workbook...");
	            mWorkbook.close();
	            mWorkbook = null;
	        }
		}
		catch(Exception e)
		{
			Logging.logFatal("Exception closing file. "+e.getMessage());
		}
	}
	
	private static String[] GetLanguages(ExtractTextOptions inOptions)
	{
		String astrLanguages[] = null;
		
		if(inOptions.strLanguage.length() > 0)
		{
			astrLanguages = new String[1];
			astrLanguages[0] = inOptions.strLanguage;
		}
		else
		{
			String strLanguages = inOptions.strLanguages;
			String strNextLanguage = null;
			int dwIndex = 0, dwStartIndex = 0, dwEndIndex = 0;
			
			while(dwIndex != -1)
			{
				dwIndex = strLanguages.indexOf(',', dwStartIndex);
				if(dwIndex != -1)
					dwEndIndex = dwIndex;
				else
					dwEndIndex = (strLanguages.length());
				
				strNextLanguage = strLanguages.substring(dwStartIndex, dwEndIndex);
				Logging.logVerbose("Adding language \""+strNextLanguage+"\"");
				
				if(astrLanguages != null)
				{
					String astrTmp[] = new String[astrLanguages.length + 1];
					System.arraycopy(astrLanguages, 0, astrTmp, 0, astrLanguages.length);
					astrTmp[astrLanguages.length] = strNextLanguage;
					astrLanguages = astrTmp;
				}
				else
				{
					astrLanguages = new String[1];
					astrLanguages[0] = strNextLanguage;
				}
				
				dwStartIndex = dwEndIndex + 1;
			}
		}
		
		return astrLanguages;
	}
	
	private static Sheet OpenSheet(int indwSheet)
	{
		Sheet CurrentSheet = null;
		
		try
        {
            Logging.logVerbose("Opening sheet "+indwSheet);
            CurrentSheet = mWorkbook.getSheet(indwSheet);
        }
        catch(IndexOutOfBoundsException ioobe)
        {
            Logging.logFatal("Exception trying to open sheet "+indwSheet+". "+ioobe.getMessage());
        }
		
		return CurrentSheet;
	}
	
	private static int GetColumn(Sheet inSheet, String instrHeading) throws Exception
    {
    	Cell HeadingCell;
    	int dwColumn = -1;
    	
    	if((inSheet == null) || (instrHeading == null))
    	{
    		Logging.logFatal("Unable to get column with heading \""+instrHeading+"\"");
    	}

    	if((instrHeading != null) && (instrHeading != ""))
    	{
			HeadingCell = inSheet.findCell(instrHeading);
			if(null != HeadingCell)
				dwColumn = HeadingCell.getColumn();
    	}
    	
    	if(dwColumn != -1)
    		Logging.logVerbose("Found heading \""+instrHeading+"\" at column "+dwColumn);
    	else
    		Logging.logVerbose("Unable to find heading \""+instrHeading+"\"");
    	
    	return dwColumn;
    }
	
	private static boolean ValidateMandatoryColumn(int indwColumn, String strHeading, boolean inbLogError)
	{
		if(-1 == indwColumn)
		{
			if(inbLogError)
				Logging.logError("Missing mandatory column heading \""+strHeading+"\"");
			return false;
		}
		
		return true;
	}
	
	private static int GetTextColumnForLanguage(Sheet inSheet, String instrLanguage) throws Exception
	{
		int dwTextColumn = GetColumn(inSheet, instrLanguage);
		if(!ValidateMandatoryColumn(dwTextColumn, instrLanguage, false))
		{
			dwTextColumn = GetColumn(inSheet, instrLanguage.toLowerCase());
			if(!ValidateMandatoryColumn(dwTextColumn, instrLanguage, false))
			{
				dwTextColumn = GetColumn(inSheet, instrLanguage.toUpperCase());
				if(!ValidateMandatoryColumn(dwTextColumn, instrLanguage, false))
				{
					Logging.logFatal("Unable to get language column for \""+instrLanguage+"\"");
				}
			}
		}
		
		return dwTextColumn;
	}
	
	private static StringBuffer ExtractLanguage(Sheet inSheet, int indwTextColumn, int indwExcludeColumn, int indwDeviceColumn) throws Exception
	{
		StringBuffer BufferedText = new StringBuffer("");
		
		int dwRowCount = inSheet.getRows();
		for(int dwRow = 1; dwRow < dwRowCount; dwRow++)
		{
			Cell CurrentCell = inSheet.getCell(indwExcludeColumn, dwRow);
    		if(CurrentCell.getType() != CellType.EMPTY)
    			continue;
    		
    		if(indwDeviceColumn != -1)
    		{
        		CurrentCell = inSheet.getCell(indwDeviceColumn, dwRow);
        		if (CurrentCell.getType() != CellType.EMPTY)
        			continue;
    		}
    		
    		CurrentCell = inSheet.getCell(indwTextColumn, dwRow);
			String strTextString = CurrentCell.getContents();
			if((null == strTextString) || ("" == strTextString))
				strTextString = "EMPTY TEXT";
			
			BufferedText.append(strTextString);
			BufferedText.append('\r');
		}
		
		return BufferedText;
	}
	
	private static void WriteText(String instrLanguage, StringBuffer inStrings, ExtractTextOptions inOptions) throws Exception
	{
		String strFilename = instrLanguage;
		if(inOptions.strOutputFilename.length() > 0)
			strFilename = inOptions.strOutputFilename;
		
		Logging.logVerbose("Outputting UTF8...");
		String strOutputPath = StringUtils.standardiseFilepath(inOptions.strOutputDirectory+"/"+strFilename+"."+inOptions.strOutputFileExtension);
		FileUtils.deleteFile(strOutputPath);

		LittleEndianOutputStream stream = new LittleEndianOutputStream(strOutputPath);
		
		String strStringsLeft = inStrings.toString();
		int udwBuferLimit = 16 * 1024;
		while(!strStringsLeft.isEmpty())
		{
			// Write the strings by chunk of 16K
			int udwLength = Math.min(strStringsLeft.length(), udwBuferLimit);
			stream.writeUtf8String(strStringsLeft.substring(0, udwLength));
			String strTemp = strStringsLeft.substring(udwLength, strStringsLeft.length());
			strStringsLeft = strTemp;
		}
		
		stream.close();
		stream = null;
		
		Logging.logVerbose("Finished writing file: \""+strOutputPath+"\"");
	}
	
	private static StringBuffer ExtractTextIds(Sheet inSheet, int indwConstantColumn, int indwExcludeColumn, int indwDeviceColumn)
	{
		StringBuffer BufferedTextIds = new StringBuffer("");
		
		int dwRowCount = inSheet.getRows();
		for(int dwRow = 1; dwRow < dwRowCount; dwRow++)
		{
			Cell CurrentCell = inSheet.getCell(indwExcludeColumn, dwRow);
    		if(CurrentCell.getType() != CellType.EMPTY)
    			continue;
    		
    		if(indwDeviceColumn != -1)
    		{
        		CurrentCell = inSheet.getCell(indwDeviceColumn, dwRow);
        		if (CurrentCell.getType() != CellType.EMPTY)
        			continue;
    		}
    		
    		CurrentCell = inSheet.getCell(indwConstantColumn, dwRow);
			String strTextIdString = CurrentCell.getContents().trim();
			if((null == strTextIdString) || ("" == strTextIdString))
				strTextIdString = "EMPTY_TEXT_ID";
			
			BufferedTextIds.append(strTextIdString);
			BufferedTextIds.append('\r');
			BufferedTextIds.append('\n');
		}
		
		return BufferedTextIds;
	}
	
	private static void WriteTextIds(StringBuffer inStringIds, ExtractTextOptions inOptions) throws Exception
	{
		Logging.logVerbose("Writing text ids...");
		
		String strFilename = "TagText";
		String strOutputPath = StringUtils.standardiseFilepath(inOptions.strOutputDirectory+"/"+strFilename+".id");
		
		FileUtils.deleteFile(strOutputPath);
		FileUtils.writeFile(strOutputPath, StringUtils.stringToUTF8Bytes(inStringIds.toString()));
		
		Logging.logVerbose("Finished writing file: \""+strOutputPath+"\"");
	}
	
	public static void Start(ExtractTextOptions inOptions)
	{
		try
		{
			OpenExcelFile(inOptions);
			
			int dwSheetIndex = 0;
			Sheet CurrentSheet = OpenSheet(dwSheetIndex);
			int dwConstantColumn = GetColumn(CurrentSheet, kstrConstantColumnHeading);
			if(!ValidateMandatoryColumn(dwConstantColumn, kstrConstantColumnHeading, true))
				return;
			
			int dwExcludeColumn = GetColumn(CurrentSheet, kstrExcludeColumnHeading);
			if(!ValidateMandatoryColumn(dwExcludeColumn, kstrExcludeColumnHeading, true))
				return;
			
			int dwDeviceColumn = -1;
			if(inOptions.strDevice.length() > 0)
				dwDeviceColumn = GetColumn(CurrentSheet, inOptions.strDevice);
			
			String astrLanguages[] = GetLanguages(inOptions);
			for(String strLanguage : astrLanguages)
			{
				int dwTextColumn = GetTextColumnForLanguage(CurrentSheet, strLanguage);
				StringBuffer LanguageStrings = ExtractLanguage(CurrentSheet, dwTextColumn, dwExcludeColumn, dwDeviceColumn);
				WriteText(strLanguage, LanguageStrings, inOptions);
			}
			
			StringBuffer TextIdStrings = ExtractTextIds(CurrentSheet, dwConstantColumn, dwExcludeColumn, dwDeviceColumn);
			WriteTextIds(TextIdStrings, inOptions);
		}
		catch(Exception e)
		{
			Logging.logFatal("Exception occured. "+e.getMessage());
		}
		finally
		{
			CloseExcelFile();
			if(!Logging.IsError())
				Logging.logVerbose("\\o/ Text extraction complete. Have a nice day. \\o/");
		}
	}
	
	private static Workbook mWorkbook = null;
}
