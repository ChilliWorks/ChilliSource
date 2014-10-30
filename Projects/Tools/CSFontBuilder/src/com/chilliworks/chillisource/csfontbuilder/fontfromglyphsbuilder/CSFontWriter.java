/**
 * CSFontWriter.java
 * Chilli Source
 * Created by Ian Copland on 21/10/2014.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
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

package com.chilliworks.chillisource.csfontbuilder.fontfromglyphsbuilder;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Random;

import javax.imageio.ImageIO;

import com.chilliworks.chillisource.coreutils.CSException;
import com.chilliworks.chillisource.coreutils.FileUtils;
import com.chilliworks.chillisource.coreutils.LittleEndianWriterUtils;
import com.chilliworks.chillisource.coreutils.Logging;
import com.chilliworks.chillisource.coreutils.StringUtils;
import com.chilliworks.chillisource.coreutils.StringWriterUtils;
import com.chilliworks.chillisource.pngtocsimage.PNGToCSImage;
import com.chilliworks.chillisource.pngtocsimage.PNGToCSImageOptions;
import com.chilliworks.chillisource.texturepackerutils.PackedTexture;

/**
 * A container for a series of methods used to write a CSFont to disk. This
 * includes writing the CSFont binary file and the packed glyph image as
 * a CSImage file.
 * 
 * @author Ian Copland
 */
public final class CSFontWriter
{
	private static final String TEMP_DIRECTORY_PATH_PREFIX = "_temp-csfont-";
	private static final String EXTENSION_CSFONT = ".csfont";
	private static final String EXTENSION_CSIMAGE = ".csimage";
	private static final int CSFONT_FILE_VERSION_NUMBER = 2;
	
	/**
	 * Writes the CSFont binary file and the pack bitmap glyph CSImage file
	 * to disk using the given options.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_outputFilePath - The file path of the output CSFont binary
	 * file. The CSImage file will have the same path but with the csimage
	 * extension.
	 * @param in_options - The output options.
	 * @param in_glyphs - The glyphs which are to be outputted.
	 * @param in_packedBitmapFont - The packed bitmap font data.
	 * 
	 * @throws CSException - An exception which provides a message describing 
	 * the error which has occurred.
	 */
	public static void write(String in_outputFilePath, FontFromGlyphsBuilderOptions in_options, Glyphs in_glyphs, PackedTexture in_packedBitmapFont) throws CSException
	{
		if ((in_outputFilePath.toLowerCase().endsWith(EXTENSION_CSFONT) || in_outputFilePath.toLowerCase().endsWith(EXTENSION_CSFONT)) == false)
		{
			throw new CSException(in_outputFilePath + " has the wrong extension. Extension must be " + EXTENSION_CSFONT + " or " + EXTENSION_CSIMAGE + ".");
		}
		
		String randomHex = Long.toHexString(new Random().nextLong());
		String outputFileRoot = StringUtils.removeExtension(StringUtils.getFileName(in_outputFilePath));
		String outputDirectoryPath = StringUtils.getDirectory(in_outputFilePath);
		String tempDirectoryPath = outputDirectoryPath + TEMP_DIRECTORY_PATH_PREFIX + randomHex + "/";
		String tempPNGImageFilePath = tempDirectoryPath + outputFileRoot + ".png";
		String tempCSImageFilePath = tempDirectoryPath + outputFileRoot + EXTENSION_CSIMAGE;
		String tempCSFontFilePath = tempDirectoryPath + outputFileRoot + EXTENSION_CSFONT;
		String outputCSImageFilePath = outputDirectoryPath + outputFileRoot + EXTENSION_CSIMAGE;
		String outputCSFontFilePath = outputDirectoryPath + outputFileRoot + EXTENSION_CSFONT;
		
		createTempDirectory(tempDirectoryPath);
		
		try
		{
			createBitmapFontPNG(tempPNGImageFilePath, in_packedBitmapFont);
			convertBitmapFontToCSImage(tempPNGImageFilePath, tempCSImageFilePath, in_options);
			createCSFont(tempCSFontFilePath, in_glyphs, in_packedBitmapFont);
			copyToOutput(tempCSImageFilePath, tempCSFontFilePath, outputCSImageFilePath, outputCSFontFilePath);
		}
		catch (CSException e)
		{
			throw new CSException(e.getMessage(), e);
		}
		finally
		{
			deleteTempDirectory(tempDirectoryPath);
		}
	}	
	/**
	 * Creates the temporary directory to create the output files in.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_tempDirectoryPath - The temporary directory path.
	 * 
	 * @throws CSException - An exception which provides a message 
	 * describing the error which has occurred.
	 */
	private static void createTempDirectory(String in_tempDirectoryPath) throws CSException
	{
		if (new File(in_tempDirectoryPath).exists() == true)
		{
			throw new CSException("Could not create temporary directory as it already exists: " + in_tempDirectoryPath);
		}
		
		if (FileUtils.createDirectory(in_tempDirectoryPath) == false)
		{
			throw new CSException("Could not create temporary directory: " + in_tempDirectoryPath);
		}
	}
	/**
	 * Writes the packed bitmap font image to disk as a PNG. This can later be 
	 * converted to CS Image.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_outputFilePath - The output file path for the PNG bitmap font image.
	 * @param in_packedBitmapFont - The packed bitmap font data.
	 * 
	 * @throws CSException - An exception which provides a message describing the 
	 * error which has occurred.
	 */
	private static void createBitmapFontPNG(String in_outputFilePath, PackedTexture in_packedBitmapFont) throws CSException
	{
		try
		{
			File imageFile = new File(in_outputFilePath);
			ImageIO.write(in_packedBitmapFont.getPackedTexture(), "png", imageFile);
		}
		catch (Exception e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			throw new CSException("Could not create the temporary bitmap font PNG file: " + in_outputFilePath, e);
		}
	}
	/**
	 * Converts the packed bitmap font image from PNG format to CSImage. 
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_inputFilePath - The PNG image file path.
	 * @param in_outputFilePath - The CSImage file path.
	 * @param in_options - The options with which to create the CSImage file.
	 * 
	 * @throws CSException - An exception which provides a message describing 
	 * the error which has occurred.
	 */
	private static void convertBitmapFontToCSImage(String in_inputFilePath, String in_outputFilePath, FontFromGlyphsBuilderOptions in_options) throws CSException
	{
		try
		{
			PNGToCSImageOptions pngToCSImageOptions = new PNGToCSImageOptions();
			pngToCSImageOptions.strInputFilename = in_inputFilePath;
			pngToCSImageOptions.strOutputFilename = in_outputFilePath;
			
			if (in_options.m_imageCompression.length() > 0)
			{
				pngToCSImageOptions.eCompressionType = PNGToCSImage.convertStringToCompressionFormat(in_options.m_imageCompression);
			}
			
			if (in_options.m_imageFormat.length() > 0)
			{
				pngToCSImageOptions.eConversionType = PNGToCSImage.convertStringToConversionFormat(in_options.m_imageFormat);
			}
			
			if (in_options.m_imageDither == true)
			{
				pngToCSImageOptions.bDither = true;
			}
			
			if (in_options.m_imagePremultiplyAlpha == false)
			{
				pngToCSImageOptions.bPremultiply = false;
			}
			
			PNGToCSImage.run(pngToCSImageOptions);
		}
		catch (Exception e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			throw new CSException("Could not convert the temp PNG to CSImage: " + in_outputFilePath, e);
		}
	}
	/**
	 * Creates the CSFont binary file from the glyphs data and the packed image
	 * data.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_outputFilePath - The output file path of the CSFont file.
	 * @param in_glyphs - The input glyphs data.
	 * @param in_packedBitmapFont - The input packed bitmap font data.
	 * 
	 * @throws CSException - An exception which provides a message describing
	 * the error which has occurred.
	 */
	private static void createCSFont(String in_outputFilePath, Glyphs in_glyphs, PackedTexture in_packedBitmapFont) throws CSException
	{
		try (DataOutputStream stream = new DataOutputStream(new FileOutputStream(in_outputFilePath)))
		{
			writeStandardHeader(stream, in_glyphs, in_packedBitmapFont);
			writeINFOChunk(stream, in_glyphs, in_packedBitmapFont);
			writeCHARChunk(stream, in_glyphs);
			writeGLPHChunk(stream, in_glyphs, in_packedBitmapFont);
		}
		catch (IOException e)
		{
			Logging.logVerbose(StringUtils.convertExceptionToString(e));
			throw new CSException("Could not create the CSFont file: " + in_outputFilePath, e);
		}
	}
	/**
	 * Writes the Chilli Source standard file header to the given stream. This
	 * contains information such as the file Id, version number and the file chunk
	 * table.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_stream - The stream to write the header to.
	 * @param in_glyphs - The input glyphs data.
	 * @param in_packedBitmapFont - The input packed bitmap font data.
	 * 
	 * @throws IOException - Any exception thrown by the output stream.
	 */
	private static void writeStandardHeader(DataOutputStream in_stream, Glyphs in_glyphs, PackedTexture in_packedBitmapFont) throws IOException
	{
		//write the chilli source Id
		StringWriterUtils.writeUTF8String(in_stream, "CSCS");
		
		//write the endianness check flag
		LittleEndianWriterUtils.writeInt32(in_stream, 9999);
				
		//write the file type identifier. This is 1 for CSFont files.
		LittleEndianWriterUtils.writeInt32(in_stream, 1);
		
		//write the CSFont version number.
		LittleEndianWriterUtils.writeInt32(in_stream, CSFONT_FILE_VERSION_NUMBER);
		
		//write the number of chunk table entries. This will be constant for now as there are always 3 entries: Font Info, Valid Characters and Glyph Data.
		final int NUM_ENTRIES = 3;
		LittleEndianWriterUtils.writeInt32(in_stream, NUM_ENTRIES);
		
		//calculate the sizes of the chunks
		final int GLOBAL_HEADER_SIZE = 5 * 4; //5x 4byte values
		final int CHUNK_TABLE_ENTRY_SIZE = 3 * 4; //3x 4byte values
		final int CHUNK_TABLE_SIZE = NUM_ENTRIES * CHUNK_TABLE_ENTRY_SIZE;
		final int INFO_CHUNK_OFFSET = GLOBAL_HEADER_SIZE + CHUNK_TABLE_SIZE;
		final int INFO_CHUNK_SIZE = 7 * 4; //7x 4byte values
		final int CHAR_CHUNK_OFFSET = GLOBAL_HEADER_SIZE + CHUNK_TABLE_SIZE + INFO_CHUNK_SIZE;
		
		int charChunkSize = calculateCHARChunkSize(in_glyphs);
		int glphChunkOffset = GLOBAL_HEADER_SIZE + CHUNK_TABLE_SIZE + INFO_CHUNK_SIZE + charChunkSize;
		int glphChunkSize = calculateGLPHChunkSize(in_packedBitmapFont);
		
		//write the INFO chunk entry.
		StringWriterUtils.writeUTF8String(in_stream, "INFO");
		LittleEndianWriterUtils.writeInt32(in_stream, INFO_CHUNK_OFFSET);
		LittleEndianWriterUtils.writeInt32(in_stream, INFO_CHUNK_SIZE);
		
		//write the CHAR chunk entry.
		StringWriterUtils.writeUTF8String(in_stream, "CHAR");
		LittleEndianWriterUtils.writeInt32(in_stream, CHAR_CHUNK_OFFSET);
		LittleEndianWriterUtils.writeInt32(in_stream, charChunkSize);
		
		//write the GLPH chunk entry.
		StringWriterUtils.writeUTF8String(in_stream, "GLPH");
		LittleEndianWriterUtils.writeInt32(in_stream, glphChunkOffset);
		LittleEndianWriterUtils.writeInt32(in_stream, glphChunkSize);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @param in_glyphs- The glyphs.
	 * 
	 * @return The size of the CHAR chunk.
	 */
	private static int calculateCHARChunkSize(Glyphs in_glyphs)
	{
		String charString = new String();
		
		for (int i = 0; i < in_glyphs.getNumGlyphs(); ++i)
		{
			charString += "" + in_glyphs.getGlyph(i).getCharacter();
		}
		
		byte[] charBytes = StringUtils.stringToUTF8Bytes(charString);
		return charBytes.length;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The size of the GLPH chunk.
	 */
	private static int calculateGLPHChunkSize(PackedTexture in_packedBitmapFont)
	{
		final int GLPH_ENTRY_SIZE = 10 * 2; //10x 2byte values.
		return in_packedBitmapFont.getNumImages() * GLPH_ENTRY_SIZE;
	}
	/**
	 * Writes the INFO chunk to the output stream. The INFO chunk contains information
	 * relating all glyphs such as the vertical padding, and data relating to the 
	 * original font including the point size and line height.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_stream - The stream to write the header to.
	 * @param in_glyphs - The input glyphs data.
	 * @param in_packedBitmapFont - The input packed bitmap font data.
	 * 
	 * @throws IOException - Any exception thrown by the output stream.
	 */
	private static void writeINFOChunk(DataOutputStream in_stream, Glyphs in_glyphs, PackedTexture in_packedBitmapFont) throws IOException
	{
		//write the original font information.
		LittleEndianWriterUtils.writeInt32(in_stream, in_glyphs.getFontSize());
		LittleEndianWriterUtils.writeInt32(in_stream, in_glyphs.getLineHeight());
		LittleEndianWriterUtils.writeInt32(in_stream, in_glyphs.getDescent());
		LittleEndianWriterUtils.writeInt32(in_stream, in_glyphs.getSpaceAdvance());
		
		//write info on the bitmap font image
		LittleEndianWriterUtils.writeInt32(in_stream, in_packedBitmapFont.getPackedWidth());
		LittleEndianWriterUtils.writeInt32(in_stream, in_packedBitmapFont.getPackedHeight());
		LittleEndianWriterUtils.writeInt32(in_stream, in_glyphs.getVerticalPadding());
	}
	/**
	 * Writes the CHAR chunk to the output stream. The CHAR chunk contains the list
	 * of valid characters in the font. Characters are written as a UTF8 string.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_stream - The stream to write the header to.
	 * @param in_glyphs - The glyphs.
	 * 
	 * @throws IOException - Any exception thrown by the output stream.
	 */
	private static void writeCHARChunk(DataOutputStream in_stream, Glyphs in_glyphs) throws IOException
	{
		String charString = new String();
		
		for (int i = 0; i < in_glyphs.getNumGlyphs(); ++i)
		{
			charString += "" + in_glyphs.getGlyph(i).getCharacter();
		}
		
		StringWriterUtils.writeUTF8String(in_stream, charString);
	}
	/**
	 * Writes the GLPH chunk to the output stream. The GLPH chunk contains all of the
	 * data for each glyph.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_stream - The stream to write the header to.
	 * @param in_glyphs - The glyphs.
	 * @param in_packedBitmapFont - The input packed bitmap font data.
	 * 
	 * @throws IOException - Any exception thrown by the output stream.
	 */
	private static void writeGLPHChunk(DataOutputStream in_stream, Glyphs in_glyphs, PackedTexture in_packedBitmapFont) throws IOException
	{
		assert (in_glyphs.getNumGlyphs() == in_packedBitmapFont.getNumImages()) : "Cannot write GLPH chunk with different amount of glyphs to packed images.";
		
		for (int i = 0; i < in_packedBitmapFont.getNumImages(); ++i)
		{
			short imageOriginX = (short)in_packedBitmapFont.getOriginX(i);
			short imageOriginY = (short)in_packedBitmapFont.getOriginY(i);
			short imagePackedWidth =  (short)in_packedBitmapFont.getCroppedWidth(i);
			short imagePackedHeight = (short)in_packedBitmapFont.getCroppedHeight(i);
			short imagePackedOffsetX = (short)in_packedBitmapFont.getOffsetX(i);
			short imagePackedOffsetY = (short)in_packedBitmapFont.getOffsetY(i);
			short imageOriginalWidth = (short)in_packedBitmapFont.getOriginalWidth(i);
			short imageOriginalHeight = (short)in_packedBitmapFont.getOriginalHeight(i);
			
			Glyph glyph = in_glyphs.getGlyph(i);
			short glyphOrigin = (short)glyph.getOrigin();
			short glyphAdvance = (short)glyph.getAdvance();
			
			LittleEndianWriterUtils.writeInt16(in_stream, imageOriginX);
			LittleEndianWriterUtils.writeInt16(in_stream, imageOriginY);
			LittleEndianWriterUtils.writeInt16(in_stream, imagePackedWidth);
			LittleEndianWriterUtils.writeInt16(in_stream, imagePackedHeight);
			LittleEndianWriterUtils.writeInt16(in_stream, imagePackedOffsetX);
			LittleEndianWriterUtils.writeInt16(in_stream, imagePackedOffsetY);
			LittleEndianWriterUtils.writeInt16(in_stream, imageOriginalWidth);
			LittleEndianWriterUtils.writeInt16(in_stream, imageOriginalHeight);
			LittleEndianWriterUtils.writeInt16(in_stream, glyphOrigin);
			LittleEndianWriterUtils.writeInt16(in_stream, glyphAdvance);
		}
	}
	/**
	 * Copies the output files from the temp directory to the output.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_sourceCSImageFilePath - The source CSImage file path.
	 * @param in_sourceCSFontFilePath - The source CSFont file path.
	 * @param in_destCSImageFilePath - The destination CSImage file path.
	 * @param in_destCSFontFilePath - The destination CSFont file path.
	 * 
	 * @throws CSException - An exception which provides a message 
	 * describing the error which has occurred.
	 */
	private static void copyToOutput(String in_sourceCSImageFilePath, String in_sourceCSFontFilePath, String in_destCSImageFilePath, String in_destCSFontFilePath) throws CSException
	{
		if (FileUtils.copyFile(in_sourceCSImageFilePath, in_destCSImageFilePath) == false)
		{
			throw new CSException("Could not copy to output directory: " + in_destCSImageFilePath);
		}
		
		if (FileUtils.copyFile(in_sourceCSFontFilePath, in_destCSFontFilePath) == false)
		{
			throw new CSException("Could not copy to output directory: " + in_destCSFontFilePath);
		}
	}
	/**
	 * Deletes the temporary directory and its contents.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_tempDirectoryPath - The temporary directory path.
	 * 
	 * @throws CSException - An exception which provides a message 
	 * describing the error which has occurred.
	 */
	private static void deleteTempDirectory(String in_tempDirectoryPath) throws CSException
	{
		if (FileUtils.deleteDirectory(in_tempDirectoryPath) == false)
		{
			throw new CSException("Could not delete temporary directory: " + in_tempDirectoryPath);
		}
	}
}
