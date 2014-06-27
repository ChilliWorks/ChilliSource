/**
*  CSAtlasBuilder.java
*  Chilli Source
*  Created by Robert Henning on 07/09/2006.
*
*  The MIT License (MIT)
*
*  Copyright (c) 2006 Tag Games Limited
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

package com.chillisource.csatlasbuilder;

import java.io.*;

import javax.imageio.*;

import com.chillisource.texturepackerutils.PackedTexture;
import com.chillisource.texturepackerutils.TexturePacker;
import com.chillisource.toolutils.ExternalProcess;
import com.chillisource.toolutils.LittleEndianOutputStream;
import com.chillisource.toolutils.FileUtils;
import com.chillisource.toolutils.Logging;
import com.chillisource.toolutils.StringUtils;

import java.util.*;

/**
 * Packs images into a single "texture atlas".
 * 
 * @author R Henning
 */
public class CSAtlasBuilder
{
	private static final String k_versionString = "2.13";
	private static final short k_versionNum = 3;

	AtlasBuilderOptions m_options = null;
	File m_rootDirectory = null; // Root directory from which files are read,set from

	/**
	 * @author R Henning
	 * 
	 * @param The options for the tool.
	 * 
	 * @return Whether or not the run was successful.
	 */
	public boolean run(AtlasBuilderOptions in_options) throws Exception
	{
		Logging.logVerbose("TextureAtlasTool version "+k_versionString);
		Logging.logVerbose("-----------------------");

		m_options = in_options;

		Logging.logVerbose("input dir name is:\"" + m_options.m_inputDirectoryPath + "\"");

		ArrayList<File> filesToProcess = new ArrayList<File>();

		m_rootDirectory = new File(m_options.m_inputDirectoryPath);
		if (m_options.m_fileList.length() > 0)
		{
			loadFilesFromOrderingFile(filesToProcess);
		}
		else
		{
			addImageFilesInDirectory(m_rootDirectory, true, filesToProcess);
		}
		
		TexturePacker packer = new TexturePacker();
		packer
		.setDivisibleBy(m_options.m_divisibleBy)
		.setExtrusion(m_options.m_extrude)
		.setFixedHeight(m_options.m_fixedHeight)
		.setFixedWidth(m_options.m_fixedWidth)
		.setMaxHeight(m_options.m_maxHeight)
		.setMaxWidth(m_options.m_maxWidth)
		.setValidHeights(m_options.m_validHeights)
		.setValidWidths(m_options.m_validWidths)
		.setHeuristic(m_options.m_packingHeuristic)
		.setInnerPadding(m_options.m_innerPadding)
		.setOuterPadding(m_options.m_padding)
		.enableCropping(m_options.m_crop);
		
		PackedTexture result = packer.pack(filesToProcess);
		if(result == null)
		{
			return false;
		}
		
		///////////////////////////////////////////////////////
		// It's output time!
		///////////////////////////////////////////////////////
		// Write our combined png file
		File F = new File(StringUtils.removeExtension(m_options.m_outputFilePath) + ".png");
		ImageIO.write(result.getPackedTexture(), "png", F);

		writeBinaryFile(result);
		writeStringIDs(result);
		convertToCSImage();

		Logging.logVerbose("Goodbye!\n");

		return true;
	}
	/**
	 * @author R Henning
	 * 
	 * @param The input directory.
	 * @param Whether or not to recurse. Will only recurse one level deep.
	 * @param [Out] The output list of files.
	 */
	private void addImageFilesInDirectory(File in_directory, boolean in_recursiveDirectorySearch, ArrayList<File> out_files)
	{
		String[] dirContents = in_directory.list();

		for (int i = 0; i < dirContents.length; i++)
		{
			File contentFile = new File(in_directory, dirContents[i]);

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
	 * @author R Henning
	 * 
	 * @param The input file.
	 * 
	 * @return The output sprite name.
	 */
	private String generateSpriteNameFromFile(File in_file) throws IOException
	{
		//choose the output format for the filenames.
		String srcFilePath = in_file.getCanonicalPath();
		String srcFilePathWithoutExtension = srcFilePath.substring(0, srcFilePath.lastIndexOf("."));
		String rootDir = m_rootDirectory.getCanonicalPath();
		int rootDirLength = rootDir.length();
		String srcFilePathFromRoot = srcFilePathWithoutExtension.substring(rootDirLength, srcFilePathWithoutExtension.length());

		// replace slashes with underscores
		String srcFilePathWithoutSlashes = srcFilePathFromRoot.replace('\\', '_').replace('/', '_'); 
		
		//if the first char is an underscore, remove it.
		if (srcFilePathWithoutSlashes.length() > 0 && srcFilePathWithoutSlashes.getBytes()[0] == '_')
		{
			srcFilePathWithoutSlashes = srcFilePathWithoutSlashes.substring(1);
		}
		return srcFilePathWithoutSlashes;
	}
	/**
	 * @author R Henning
	 * 
	 * @param [Out] The list of files.
	 */
	private void loadFilesFromOrderingFile(ArrayList<File> files) throws Exception
	{
		File f = new File(m_options.m_fileList);
		FileInputStream in = new FileInputStream(f);

		DataInputStream I = new DataInputStream(in);
		int length = (int) f.length();
		byte tmp[] = new byte[length];
		I.readFully(tmp);
		I.close();
		
		String fileAsString = new String(tmp);

		String[] lines = fileAsString.replace('\r','\n').split("\n");

		for(String line : lines)
		{
			if ((line != null) && (line.length()> 0))
				files.add(new File(StringUtils.standardiseFilePath(m_options.m_inputDirectoryPath + "/" + line)));
		}
	}
	/**
	 * Converts the temporary png file to CSImage.
	 * 
	 * @author S Downie
	 */
	private void convertToCSImage()
	{
		Logging.logVerbose("Converting to CSImage");
		
		LinkedList<String> commands = new LinkedList<String>();
		commands.add("java");
		commands.add("-Djava.awt.headless=true");
		commands.add("-jar");
		commands.add(FileUtils.getPathToHere() + "PNGToCSImage.jar");
		
		commands.add("--input");
		commands.add(StringUtils.removeExtension(m_options.m_outputFilePath) + ".png");
		
		commands.add("--output");
		commands.add(StringUtils.removeExtension(m_options.m_outputFilePath) + ".csimage");
		
		if (m_options.m_imageCompression.length() > 0)
		{
			commands.add("--compression");
			commands.add(m_options.m_imageCompression);
		}
		
		if (m_options.m_imageFormat.length() > 0)
		{
			commands.add("--convert");
			commands.add(m_options.m_imageFormat);
		}
		
		if (m_options.m_imageDither == true)
		{
			commands.add("--dither");
		}
		
		if (m_options.m_imagePremultiplyAlpha == true)
		{
			commands.add("--disablepremultipliedalpha");
		}
		
		ExternalProcess csImageConversion = new ExternalProcess();
		csImageConversion.run(commands);
		
		for (String message : csImageConversion.getMessages())
		{
			Logging.logVerbose(message);
		}
		
		for (String message : csImageConversion.getWarnings())
		{
			Logging.logWarning(message);
		}
		
		for (String message : csImageConversion.getErrors())
		{
			Logging.logError(message);
		}
		
		FileUtils.deleteFile(StringUtils.removeExtension(m_options.m_outputFilePath) + ".png");
	}
	/**
	 * @author R Henning
	 * 
	 * @param The packed texure.
	 */
	private void writeBinaryFile(PackedTexture in_packedTexture) throws FileNotFoundException, IOException, Exception
	{
		int numImages = in_packedTexture.getNumImages();
		LittleEndianOutputStream dosBinary = new LittleEndianOutputStream(m_options.m_outputFilePath);
		dosBinary.writeShort((short) numImages);
		dosBinary.writeShort(k_versionNum); // file format revision

		//Write out spritesheet size
		dosBinary.writeShort((short) in_packedTexture.getPackedWidth());
		dosBinary.writeShort((short) in_packedTexture.getPackedHeight());
		
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

		dosBinary.close();
	}
	/**
	 * @author R Henning
	 * 
	 * @param The packaged texture.
	 */
	private void writeStringIDs(PackedTexture in_packedTexture) throws IOException 
	{
		int numImages = in_packedTexture.getNumImages();
		
		FileOutputStream outC = new FileOutputStream(m_options.m_outputFilePath + "id");
		DataOutputStream dosC = new DataOutputStream(outC);

		for (int i = 0; i < numImages; i++)
		{
			String enumName = generateSpriteNameFromFile(in_packedTexture.getOriginalFile(i));
			dosC.writeBytes(enumName);
			dosC.writeByte('\n');
		}

		dosC.close();
	}
}
