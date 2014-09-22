/**
*  FontTool.java
*  Chilli Source
*  Created by Robert Henning on 17/06/2014.
*
*  The MIT License (MIT)
*
*  Copyright (c) 2014 Tag Games Limited
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

package com.chillisource.csfontbuilder;

import java.io.*;

import javax.imageio.*;

import java.util.*;

import com.chillisource.pngtocsimage.PNGToCSImage;
import com.chillisource.pngtocsimage.PNGToCSImageOptions;
import com.chillisource.texturepackerutils.PackedTexture;
import com.chillisource.texturepackerutils.TexturePacker;
import com.chillisource.toolutils.FileUtils;
import com.chillisource.toolutils.LittleEndianOutputStream;
import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.StringUtils;
 
/**
 * Converts a series of character images to a bitmap font.
 * 
 * @author Robert Henning
 */
public class CSFontBuilder
{
	private static final short k_version = 1;
	private static final int k_numPixelsPadding = 2;
	private static final Character k_charLF = new Character((char)10);
	
	private StringBuffer m_fontAlphabet = null; // this will create the alphabet file in
	private StringBuffer m_fontCSV = null; // debug csv file for comparing font output
	private FontBuilderOptions m_options;
	private File m_rootDirectory; // Root directory from which files are read,set from
	
	/**
	 * @author Robert Henning
	 * 
	 * @param Directory file.
	 * @param Whether or not to recurse.
	 * @param The output list of files
	 */
	private void addImageFilesInDirectory(File in_dir, boolean in_recursiveDirectorySearch, ArrayList<File> out_files)
	{
		String[] dirContents = in_dir.list();

		for (int i = 0; i < dirContents.length; i++)
		{
			File contentFile = new File(in_dir, dirContents[i]);

			if (contentFile.isDirectory() && in_recursiveDirectorySearch)
			{
				addImageFilesInDirectory(contentFile, false, out_files); // only go one level deep
			}
			else
			{
				boolean IsPngFile = dirContents[i].endsWith(".png") || dirContents[i].endsWith(".PNG");
				if (IsPngFile)
					out_files.add(contentFile);
			}
		}
	}
	/**
	 * @author Robert Henning
	 * 
	 * @param The options for running the tool.
	 */
	public boolean Run(FontBuilderOptions in_options) throws Exception
	{
		Logging.logVerbose("FontTool version " + new Short(k_version).toString());
		Logging.logVerbose("-----------------------");

		m_options = in_options;
		
		ArrayList<File> filesToProcess = new ArrayList<File>();

		m_rootDirectory = new File(m_options.m_inputDirectoryPath);
		addImageFilesInDirectory(m_rootDirectory, true, filesToProcess);

		TexturePacker packer = new TexturePacker();
		packer
		.setDivisibleBy(m_options.m_divisibleBy)
		.setExtrusion(0)
		.setFixedHeight(m_options.m_fixedHeight)
		.setFixedWidth(m_options.m_fixedWidth)
		.setMaxHeight(m_options.m_maxHeight)
		.setMaxWidth(m_options.m_maxWidth)
		.setValidHeights(m_options.m_validHeights)
		.setValidWidths(m_options.m_validWidths)
		.setHeuristic(m_options.m_packingHeuristic)
		.setInnerPadding(0)
		.setOuterPadding(k_numPixelsPadding)
		.enableCropping(true);
		
		PackedTexture result = packer.pack(filesToProcess, null);
		if(result == null)
		{
			return false;
		}

		// Write our combined png file
		File F = new File(StringUtils.removeExtension(m_options.m_outputFilePath) + ".png");
		ImageIO.write(result.getPackedTexture(), "png", F);

		buildAlphabet(result);
		writeBinaryFile(result);
		
		convertToCSImage();

		Logging.logVerbose("Goodbye!\n");

		return true;
	}
	/**
	 * Converts the created PNG to a CSImage.
	 * 
	 * @author S Downie
	 */
	private void convertToCSImage()
	{
		Logging.logVerbose("Converting to CSImage");
		
		PNGToCSImageOptions options = new PNGToCSImageOptions();
		options.strInputFilename = StringUtils.removeExtension(m_options.m_outputFilePath) + ".png";
		options.strOutputFilename = StringUtils.removeExtension(m_options.m_outputFilePath) + ".csimage";
		
		if (m_options.m_imageCompression.length() > 0)
		{
			options.eCompressionType = PNGToCSImage.convertStringToCompressionFormat(m_options.m_imageCompression);
		}
		
		if (m_options.m_imageFormat.length() > 0)
		{
			options.eConversionType = PNGToCSImage.convertStringToConversionFormat(m_options.m_imageFormat);
		}
		
		if (m_options.m_imageDither == true)
		{
			options.bDither = true;
		}
		
		if (m_options.m_imagePremultiplyAlpha == false)
		{
			options.bPremultiply = false;
		}
		
		try
		{
			PNGToCSImage.run(options);
		}
		catch (Exception e)
		{
			Logging.logFatal("An exception occurred while converting to CSImage:\n" + StringUtils.convertExceptionToString(e));
		}
		
		FileUtils.deleteFile(StringUtils.removeExtension(m_options.m_outputFilePath) + ".png");
	}
	/**
	 * @author Robert Henning
	 * 
	 * @param The packaged texture.
	 */
	private void buildAlphabet(PackedTexture in_packedTexture)
	{
		int numSourceImages = in_packedTexture.getNumImages();
		for (int character = 0; character < numSourceImages; character++)
		{
			// This is a font so check the filename for unicode
			// value of character
			// and write into alphabet file.

			int index = character;

			// check for valid unicode
			String charFilename = in_packedTexture.getOriginalFile(index).getName();

			if ((charFilename.length() == 8) && (charFilename.indexOf('.') == 4)) // eg 0123.png
			{
				String unicodeString = charFilename.substring(0, 4);
				char unicode = (char) Integer.parseInt(unicodeString, 16);
				
				Logging.logVerbose("Unicode for file \"" + charFilename + "\" is \"" + unicode + "\"");

				if (m_fontAlphabet != null)
				{
					m_fontAlphabet.append(unicode);

					// check for special characters
					if (unicode != '"')
					{
						m_fontCSV.append("\"" + unicodeString + "\",\"" + unicode + "\"," + k_charLF);
					}
				}
				else
				{
					m_fontAlphabet = new StringBuffer(1);
					m_fontAlphabet.append(unicode);

					m_fontCSV = new StringBuffer(1);
					m_fontCSV.append("\"Uncode\",\"Character\"," + k_charLF + "\"" + unicodeString + "\",\"" + unicode + "\"," + k_charLF);
				}
			}
			else
			{
				Logging.logError("unicode filename \"" + charFilename + "\" is not valid!");
			}
		}
	}
	/**
	 * @author Robert Henning
	 * 
	 * @param The packaged texture.
	 */
	private void writeBinaryFile(PackedTexture in_packedTexture) throws FileNotFoundException, IOException, Exception
	{
		int numImages = in_packedTexture.getNumImages();
		LittleEndianOutputStream dosBinary = new LittleEndianOutputStream(m_options.m_outputFilePath);
		dosBinary.writeShort((short) numImages);
		dosBinary.writeShort(k_version); // file format revision

		//Write out spritesheet size
		dosBinary.writeShort((short) in_packedTexture.getPackedWidth());
		dosBinary.writeShort((short) in_packedTexture.getPackedHeight());
		dosBinary.writeShort((short) m_options.m_lineHeight);
		
		Logging.logVerbose("Output Image size::" + in_packedTexture.getPackedWidth() + " x " + in_packedTexture.getPackedHeight());
		
		for (int i = 0; i < numImages; i++)
		{
			int ix = in_packedTexture.getOffsetX(i);
			int iy = in_packedTexture.getOffsetY(i);

			int ox = in_packedTexture.getOriginX(i);
			int oy = in_packedTexture.getOriginY(i);
			int width =  in_packedTexture.getCroppedWidth(i);
			int height = in_packedTexture.getCroppedHeight(i);
			int orig_width = in_packedTexture.getOriginalWidth(i);
			int orig_height = in_packedTexture.getOriginalHeight(i);
			
			Logging.logVerbose("Writing pixels from:" + in_packedTexture.getOriginalFile(i).getName());
			Logging.logVerbose("Image:" + i + " position:" + ox + "," + oy + ":" + width + "," + height);

			dosBinary.writeShort((short) ox);
			dosBinary.writeShort((short) oy);
			dosBinary.writeShort((short) width);
			dosBinary.writeShort((short) height);
			dosBinary.writeShort((short) ix);
			dosBinary.writeShort((short) iy);
			dosBinary.writeShort((short) orig_width);
			dosBinary.writeShort((short) orig_height);
		}
		
		dosBinary.writeNullTerminatedUtf8String(m_fontAlphabet.toString());

		dosBinary.close();
	}
}
