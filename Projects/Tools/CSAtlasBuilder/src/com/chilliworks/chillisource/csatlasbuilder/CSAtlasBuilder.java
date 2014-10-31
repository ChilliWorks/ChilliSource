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

package com.chilliworks.chillisource.csatlasbuilder;

import java.io.*;

import javax.imageio.*;

import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.LittleEndianWriterUtils;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.pngtocsimage.PNGToCSImage;
import com.chilliworks.chillisource.pngtocsimage.PNGToCSImageOptions;
import com.chilliworks.chillisource.texturepackerutils.PackedTexture;
import com.chilliworks.chillisource.texturepackerutils.PackerInfo;
import com.chilliworks.chillisource.texturepackerutils.TexturePacker;

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
	private static final String k_atlasExtension = ".csatlas";
	private static final String k_atlasIdExtension = ".csatlasid";
	private static final String k_csImageExtension = ".csimage";
	
	private AtlasBuilderOptions m_options = null;
	private File m_rootDirectory = null; // Root directory from which files are read,set from
	private String m_outputFilePathWithoutExtension;
	
	/**
	 * @author R Henning
	 * 
	 * @param The options for the tool.
	 * 
	 * @return Whether or not the run was successful.
	 */
	public boolean buildAtlas(AtlasBuilderOptions in_options) throws Exception
	{
		Logging.logVerbose("TextureAtlasTool version " + k_versionString);
		Logging.logVerbose("-----------------------");

		m_options = in_options;
		
		String extension = StringUtils.getExtension(m_options.m_outputFilePath);
		
		//Check for a valid extension, i.e. either csatlas/csatlasid/csimage
		if(!(extension.equals(k_atlasExtension.substring(1)) || extension.equals(k_atlasIdExtension.substring(1)) || extension.equals(k_csImageExtension.substring(1))))
		{
			Logging.logFatal("Output filepath (" + m_options.m_outputFilePath +  ") has wrong extension type, valid output extensions are " + k_atlasExtension + ", " + k_atlasIdExtension + " and " + k_csImageExtension);
		}
		
		//Remove the extension from the outputpath
		m_outputFilePathWithoutExtension = StringUtils.removeExtension(m_options.m_outputFilePath);
		
		Logging.logVerbose("input dir name is:\"" + m_options.m_inputDirectoryPath + "\"");

		m_rootDirectory = new File(m_options.m_inputDirectoryPath);
		
		List<AtlasImage> atlasImagesToProcess;
		if (m_options.m_atlasImages.size() > 0)
		{
			atlasImagesToProcess = m_options.m_atlasImages;
		}
		else if (m_options.m_fileList.length() > 0)
		{
			atlasImagesToProcess = new ArrayList<AtlasImage>();
			loadFilesFromOrderingFile(atlasImagesToProcess);
		}
		else
		{
			atlasImagesToProcess = new ArrayList<AtlasImage>();
			addImageFilesInDirectory(m_rootDirectory, true, atlasImagesToProcess);
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
		
		ArrayList<File> filesToProcess = new ArrayList<File>();
		for (AtlasImage atlasImage : atlasImagesToProcess)
		{
			filesToProcess.add(atlasImage.getSourceImageFile());
		}
		
		PackedTexture result = packer.pack(filesToProcess, null);
		if(result == null)
		{
			return false;
		}
		
		///////////////////////////////////////////////////////
		// It's output time!
		///////////////////////////////////////////////////////
		// Write our combined png file
		File F = new File(m_outputFilePathWithoutExtension + ".png");
		ImageIO.write(result.getPackedTexture(), "png", F);

		writeBinaryFile(result);
		writeStringIDs(result, atlasImagesToProcess);
		convertToCSImage();

		Logging.logVerbose("Goodbye!\n");

		return true;
	}
	/**
	 * Checks if images specified with options will fit in the sprite sheet dimensions, will populate the error params if not
	 * @param in_options - Atlas Options
	 * @param out_errorInfo - Error Info
	 * @return If the options can fit
	 * @throws Exception
	 */
	public boolean checkCanFit(AtlasBuilderOptions in_options, PackerInfo out_errorInfo) throws Exception
	{
		Logging.logVerbose("TextureAtlasTool version " + k_versionString);
		Logging.logVerbose("-----------------------");

		m_options = in_options;
		m_rootDirectory = new File(m_options.m_inputDirectoryPath);
		
		List<AtlasImage> atlasImagesToProcess;
		if (m_options.m_atlasImages.size() > 0)
		{
			atlasImagesToProcess = m_options.m_atlasImages;
		}
		else if (m_options.m_fileList.length() > 0)
		{
			atlasImagesToProcess = new ArrayList<AtlasImage>();
			loadFilesFromOrderingFile(atlasImagesToProcess);
		}
		else
		{
			atlasImagesToProcess = new ArrayList<AtlasImage>();
			addImageFilesInDirectory(m_rootDirectory, true, atlasImagesToProcess);
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
		.enableCropping(m_options.m_crop)
		.disableFatalLogs();
		
		ArrayList<File> filesToProcess = new ArrayList<File>();
		for (AtlasImage atlasImage : atlasImagesToProcess)
		{
			filesToProcess.add(atlasImage.getSourceImageFile());
		}
		
		PackedTexture result = packer.pack(filesToProcess, out_errorInfo);
		if(result == null)
		{
			return false;
		}

		return true;
	}
	/**
	 * @author R Henning
	 * 
	 * @param The input directory.
	 * @param Whether or not to recurse. Will only recurse one level deep.
	 * @param [Out] The output list of files.
	 */
	private void addImageFilesInDirectory(File in_directory, boolean in_recursiveDirectorySearch, List<AtlasImage> out_atlasImages)
	{
		String[] dirContents = in_directory.list();

		for (int i = 0; i < dirContents.length; i++)
		{
			File contentFile = new File(in_directory, dirContents[i]);

			if (contentFile.isDirectory() && in_recursiveDirectorySearch)
			{
				addImageFilesInDirectory(contentFile, false, out_atlasImages); // only go one level deep
			}
			else
			{
				boolean IsPngFile = dirContents[i].endsWith(".png") || dirContents[i].endsWith(".PNG");
				if (IsPngFile == true)
				{
					String atlasImageId = generateSpriteNameFromFile(contentFile);
					out_atlasImages.add(new AtlasImage(contentFile, atlasImageId));
				}
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
	private String generateSpriteNameFromFile(File in_file)
	{
		//choose the output format for the filenames.
		String srcFilePath = null;
		String rootDir = null;
		try
		{
			srcFilePath = in_file.getCanonicalPath();
			rootDir = m_rootDirectory.getCanonicalPath();
		}
		catch (Exception e)
		{
			Logging.logFatal(StringUtils.convertExceptionToString(e));
		}
		
		String srcFilePathWithoutExtension = srcFilePath.substring(0, srcFilePath.lastIndexOf("."));
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
	private void loadFilesFromOrderingFile(List<AtlasImage> out_atlasImages) throws Exception
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
			{
				File imageFile = new File(StringUtils.standardiseFilePath(m_options.m_inputDirectoryPath + "/" + line));
				String atlasImageId = generateSpriteNameFromFile(imageFile);
				out_atlasImages.add(new AtlasImage(imageFile, atlasImageId));
			}
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
		
		PNGToCSImageOptions options = new PNGToCSImageOptions();
		options.strInputFilename = m_outputFilePathWithoutExtension + ".png";
		options.strOutputFilename = m_outputFilePathWithoutExtension + k_csImageExtension;
		
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
		
		FileUtils.deleteFile(m_outputFilePathWithoutExtension + ".png");
	}
	/**
	 * @author R Henning
	 * 
	 * @param The packed texure.
	 */
	private void writeBinaryFile(PackedTexture in_packedTexture) throws FileNotFoundException, IOException, Exception
	{
		int numImages = in_packedTexture.getNumImages();
		DataOutputStream dosBinary = new DataOutputStream(new FileOutputStream(m_outputFilePathWithoutExtension + k_atlasExtension));
		
		LittleEndianWriterUtils.writeInt16(dosBinary, (short) numImages);
		LittleEndianWriterUtils.writeInt16(dosBinary, k_versionNum); // file format revision

		//Write out spritesheet size
		LittleEndianWriterUtils.writeInt16(dosBinary, (short) in_packedTexture.getPackedWidth());
		LittleEndianWriterUtils.writeInt16(dosBinary, (short) in_packedTexture.getPackedHeight());
		
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

			LittleEndianWriterUtils.writeInt16(dosBinary, (short) ox);
			LittleEndianWriterUtils.writeInt16(dosBinary, (short) oy);
			LittleEndianWriterUtils.writeInt16(dosBinary, (short) width);
			LittleEndianWriterUtils.writeInt16(dosBinary, (short) height);
			LittleEndianWriterUtils.writeInt16(dosBinary, (short) ix);
			LittleEndianWriterUtils.writeInt16(dosBinary, (short) iy);
			LittleEndianWriterUtils.writeInt16(dosBinary, (short) orig_width);
			LittleEndianWriterUtils.writeInt16(dosBinary, (short) orig_height);
		}

		dosBinary.close();
	}
	/**
	 * @author R Henning
	 * 
	 * @param in_packedTexture - The packaged texture.
	 * @param in_textureToSpriteNameMapper - Optional file to spriteId name map 
	 */
	private void writeStringIDs(PackedTexture in_packedTexture, List<AtlasImage> in_atlasImages) throws IOException 
	{
		int numImages = in_packedTexture.getNumImages();
		
		FileOutputStream outC = new FileOutputStream(m_outputFilePathWithoutExtension + k_atlasIdExtension);
		DataOutputStream dosC = new DataOutputStream(outC);

		for (int i = 0; i < numImages; i++)
		{
			String atlasImageId = "";
			
			for (AtlasImage atlasImage : in_atlasImages)
			{
				if (atlasImage.getSourceImageFile().equals(in_packedTexture.getOriginalFile(i)) == true)
				{
					atlasImageId = atlasImage.getAtlasImageId();
					break;
				}
			}
			
			if (atlasImageId.length() == 0)
			{
				Logging.logFatal("Couldn't file atlas image Id for image: " + in_packedTexture.getOriginalFile(i).getName());
			}
			
			dosC.writeBytes(atlasImageId);
			dosC.writeByte('\n');
		}

		dosC.close();
	}
}
