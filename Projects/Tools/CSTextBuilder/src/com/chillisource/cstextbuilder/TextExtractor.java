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

package com.chillisource.cstextbuilder;

import java.io.File;

import com.chillisource.toolutils.LittleEndianOutputStream;
import com.chillisource.toolutils.FileUtils;
import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.Logging.LoggingLevel;
import com.chillisource.toolutils.StringUtils;

import jxl.Cell;
import jxl.CellType;
import jxl.Sheet;
import jxl.Workbook;
import jxl.WorkbookSettings;

public class TextExtractor
{
	private static final String	kstrConstantColumnHeading = "Constant";
	private static final String kstrExcludeColumnHeading = "Exclude";
	
	private static void OpenExcelFile(ExtractTextOptions inOptions) throws Exception
	{
		File f = new File(inOptions.m_inputFilePath);
		Logging.logVerbose("Input file path is \""+f.getCanonicalPath()+"\"");
        
        if(!f.exists())
        {
        	Logging.logFatal("Input file \""+inOptions.m_inputFilePath+"\" does not exist.");
        }

        if(!f.isFile())
        {
        	Logging.logFatal("Input file \""+inOptions.m_inputFilePath+"\" is not a valid file.");
        }

        if(!f.canRead())
        {
        	Logging.logFatal("Input file \""+inOptions.m_inputFilePath+"\" cannot be read.");
        }

        Logging.logVerbose("Opening workbook...");
        WorkbookSettings ws = new WorkbookSettings();
        if (Logging.getLoggingLevel() != LoggingLevel.k_warning)
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
	
	private static StringBuffer ExtractLanguage(Sheet inSheet, int indwTextColumn, int indwExcludeColumn) throws Exception
	{
		StringBuffer BufferedText = new StringBuffer("");
		
		int dwRowCount = inSheet.getRows();
		for(int dwRow = 1; dwRow < dwRowCount; dwRow++)
		{
			Cell CurrentCell = inSheet.getCell(indwExcludeColumn, dwRow);
    		if(CurrentCell.getType() != CellType.EMPTY)
    			continue;
    		
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
		Logging.logVerbose("Outputting UTF8...");
		String strOutputPath = StringUtils.standardiseFilePath(inOptions.m_outputFilePath);
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
	
	private static StringBuffer ExtractTextIds(Sheet inSheet, int indwConstantColumn, int indwExcludeColumn)
	{
		StringBuffer BufferedTextIds = new StringBuffer("");
		
		int dwRowCount = inSheet.getRows();
		for(int dwRow = 1; dwRow < dwRowCount; dwRow++)
		{
			Cell CurrentCell = inSheet.getCell(indwExcludeColumn, dwRow);
    		if(CurrentCell.getType() != CellType.EMPTY)
    			continue;
    		
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
		
		String strOutputPath = StringUtils.standardiseFilePath(inOptions.m_outputFilePath + "id");
		
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
			
			int dwTextColumn = GetTextColumnForLanguage(CurrentSheet, inOptions.m_language);
			StringBuffer LanguageStrings = ExtractLanguage(CurrentSheet, dwTextColumn, dwExcludeColumn);
			WriteText(inOptions.m_language, LanguageStrings, inOptions);
			
			StringBuffer TextIdStrings = ExtractTextIds(CurrentSheet, dwConstantColumn, dwExcludeColumn);
			WriteTextIds(TextIdStrings, inOptions);
		}
		catch(Exception e)
		{
			Logging.logFatal("Exception occured. "+e.getMessage());
		}
		finally
		{
			CloseExcelFile();
			Logging.logVerbose("\\o/ Text extraction complete. Have a nice day. \\o/");
		}
	}
	
	private static Workbook mWorkbook = null;
}
