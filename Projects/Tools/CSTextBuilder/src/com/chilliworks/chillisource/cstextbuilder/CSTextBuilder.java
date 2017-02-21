/**
 * CSTextBuilder.java
 * ChilliSource
 * Created by Robert Henning on 15/01/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

package com.chilliworks.chillisource.cstextbuilder;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.json.JSONObject;

import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.Logging.LoggingLevel;

import jxl.Cell;
import jxl.CellType;
import jxl.Sheet;
import jxl.Workbook;
import jxl.WorkbookSettings;

/**
 * Builds localised text from the given excel sheet.
 * 
 * @author R Henning
 */
public class CSTextBuilder
{
	private static final int k_versionNumber = 1;
	private static final String	k_constantColumnHeading = "Constant";
	private static final String k_excludeColumnHeading = "Exclude";
	private static Workbook m_workbook = null;
	
	/**
	 * Run the text extractor.
	 * 
	 * @author R Henning
	 * 
	 * @param The options.
	 */
	public static void run(ExtractTextOptions in_options)
	{
		try
		{
			openExcelFile(in_options);
			
			int dwSheetIndex = 0;
			Sheet CurrentSheet = openSheet(dwSheetIndex);
			int dwConstantColumn = getColumn(CurrentSheet, k_constantColumnHeading);
			if(!validateMandatoryColumn(dwConstantColumn, k_constantColumnHeading, true))
				return;
			
			int dwExcludeColumn = getColumn(CurrentSheet, k_excludeColumnHeading);
			if(!validateMandatoryColumn(dwExcludeColumn, k_excludeColumnHeading, true))
				return;
			
			int dwTextColumn = getTextColumnForLanguage(CurrentSheet, in_options.m_language);
			List<String> textIds = extractTextIds(CurrentSheet, dwConstantColumn, dwExcludeColumn);
			List<String> texts = extractText(CurrentSheet, dwTextColumn, dwExcludeColumn);
			
			writeJson(in_options.m_outputFilePath, textIds, texts);
		}
		catch(Exception e)
		{
			Logging.logFatal("Exception occured. "+e.getMessage());
		}
		finally
		{
			closeExcelFile();
			Logging.logVerbose("\\o/ Text extraction complete. Have a nice day. \\o/");
		}
	}
	/**
	 * @author R Henning
	 * 
	 * @param The tool options.
	 */
	private static void openExcelFile(ExtractTextOptions in_options) throws Exception
	{
		File f = new File(in_options.m_inputFilePath);
		Logging.logVerbose("Input file path is \""+f.getCanonicalPath()+"\"");
        
        if(!f.exists())
        {
        	Logging.logFatal("Input file \""+in_options.m_inputFilePath+"\" does not exist.");
        }

        if(!f.isFile())
        {
        	Logging.logFatal("Input file \""+in_options.m_inputFilePath+"\" is not a valid file.");
        }

        if(!f.canRead())
        {
        	Logging.logFatal("Input file \""+in_options.m_inputFilePath+"\" cannot be read.");
        }

        Logging.logVerbose("Opening workbook...");
        WorkbookSettings ws = new WorkbookSettings();
        if (Logging.getLoggingLevel() == LoggingLevel.ERROR || Logging.getLoggingLevel() == LoggingLevel.FATAL || Logging.getLoggingLevel() == LoggingLevel.NONE)
        {
        	ws.setSuppressWarnings(true);
        }
        ws.setEncoding("Cp1252");
        m_workbook = Workbook.getWorkbook(f, ws);
	}
	/**
	 * @author R Henning
	 * 
	 * @param The sheet to open.
	 * 
	 * @return The output sheet.
	 */
	private static Sheet openSheet(int in_sheet)
	{
		Sheet CurrentSheet = null;
		
		try
        {
            Logging.logVerbose("Opening sheet "+in_sheet);
            CurrentSheet = m_workbook.getSheet(in_sheet);
        }
        catch(IndexOutOfBoundsException ioobe)
        {
            Logging.logFatal("Exception trying to open sheet "+in_sheet+". "+ioobe.getMessage());
        }
		
		return CurrentSheet;
	}
	/**
	 * @author R Henning
	 * 
	 * @param The sheet.
	 * @param The heading.
	 * 
	 * @return The column.
	 */
	private static int getColumn(Sheet in_sheet, String in_heading) throws Exception
    {
    	Cell HeadingCell;
    	int dwColumn = -1;
    	
    	if((in_sheet == null) || (in_heading == null))
    	{
    		Logging.logFatal("Unable to get column with heading \""+in_heading+"\"");
    	}

    	if((in_heading != null) && (in_heading != ""))
    	{
			HeadingCell = in_sheet.findCell(in_heading);
			if(null != HeadingCell)
				dwColumn = HeadingCell.getColumn();
    	}
    	
    	if(dwColumn != -1)
    		Logging.logVerbose("Found heading \""+in_heading+"\" at column "+dwColumn);
    	else
    		Logging.logVerbose("Unable to find heading \""+in_heading+"\"");
    	
    	return dwColumn;
    }
	/**
	 * @author R Henning
	 * 
	 * @param The column.
	 * @param The heading.
	 * @param Whether or not to log errors.
	 * 
	 * @return Whether or not the heading is valid.
	 */
	private static boolean validateMandatoryColumn(int in_column, String in_heading, boolean in_logError)
	{
		if(-1 == in_column)
		{
			if(in_logError)
				Logging.logError("Missing mandatory column heading \""+in_heading+"\"");
			return false;
		}
		
		return true;
	}
	/**
	 * @author R Henning
	 * 
	 * @param The sheet.
	 * @param The Language.
	 * 
	 * @return The text column.
	 */
	private static int getTextColumnForLanguage(Sheet in_sheet, String in_language) throws Exception
	{
		int dwTextColumn = getColumn(in_sheet, in_language);
		if(!validateMandatoryColumn(dwTextColumn, in_language, false))
		{
			dwTextColumn = getColumn(in_sheet, in_language.toLowerCase());
			if(!validateMandatoryColumn(dwTextColumn, in_language, false))
			{
				dwTextColumn = getColumn(in_sheet, in_language.toUpperCase());
				if(!validateMandatoryColumn(dwTextColumn, in_language, false))
				{
					Logging.logFatal("Unable to get language column for \""+in_language+"\"");
				}
			}
		}
		
		return dwTextColumn;
	}
	/**
	 * @author R Henning
	 * 
	 * @param The sheet.
	 * @param The text column.
	 * @param The exclude column.
	 * 
	 * @return The output buffer of ids.
	 */
	private static List<String> extractTextIds(Sheet in_sheet, int in_constantColumn, int in_excludeColumn)
	{
		List<String> output = new ArrayList<String>();
		
		int dwRowCount = in_sheet.getRows();
		for(int dwRow = 1; dwRow < dwRowCount; dwRow++)
		{
			Cell CurrentCell = in_sheet.getCell(in_excludeColumn, dwRow);
    		if(CurrentCell.getType() != CellType.EMPTY)
    			continue;
    		
    		CurrentCell = in_sheet.getCell(in_constantColumn, dwRow);
			String strTextIdString = CurrentCell.getContents().trim();
			if((null == strTextIdString) || ("" == strTextIdString))
				strTextIdString = "EMPTY_TEXT_ID";
			
			output.add(strTextIdString);
		}
		
		return output;
	}
	/**
	 * @author R Henning
	 * 
	 * @param The sheet.
	 * @param The text column.
	 * @param The exclude column.
	 * 
	 * @return The output buffer of strings.
	 */
	private static List<String> extractText(Sheet in_sheet, int in_textColumn, int in_excludeColumn) throws Exception
	{
		List<String> output = new ArrayList<String>();
		
		int dwRowCount = in_sheet.getRows();
		for(int dwRow = 1; dwRow < dwRowCount; dwRow++)
		{
			Cell CurrentCell = in_sheet.getCell(in_excludeColumn, dwRow);
    		if(CurrentCell.getType() != CellType.EMPTY)
    			continue;
    		
    		CurrentCell = in_sheet.getCell(in_textColumn, dwRow);
			String strTextString = CurrentCell.getContents();
			if((null == strTextString) || ("" == strTextString))
				strTextString = "EMPTY TEXT";
			
			output.add(strTextString);
		}
		
		return output;
	}
	/**
	 * Closes the excel file if it is open.
	 * 
	 * @author R Henning
	 */
	private static void closeExcelFile()
	{
		try
		{
			if(m_workbook != null)
	        {
				Logging.logVerbose("Closing workbook...");
	            m_workbook.close();
	            m_workbook = null;
	        }
		}
		catch(Exception e)
		{
			Logging.logFatal("Exception closing file. "+e.getMessage());
		}
	}
	/**
	 * Outputs the Id's and Strings as JSON key-value pairs.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The output file path.
	 * @param The list of text Ids.
	 * @param The list of text.
	 */
	private static void writeJson(String in_filePath, List<String> in_textIds, List<String> in_texts)
	{
		if (in_textIds.size() != in_texts.size())
		{
			Logging.logFatal("Text and Id count's are not the same!");
		}
		
		JSONObject jsonRoot = new JSONObject();
		jsonRoot.put("Version", k_versionNumber);
		
		JSONObject jsonText = new JSONObject();
		for (int i = 0; i < in_textIds.size(); ++i)
		{
			jsonText.put(in_textIds.get(i), in_texts.get(i));
		}
		jsonRoot.put("Text", jsonText);
		
		String output = jsonRoot.toString(2);
		if (FileUtils.writeFile(in_filePath, output) == false)
		{
			Logging.logFatal("Failed to write file: " + in_filePath);
		}
	}
}
