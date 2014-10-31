/**
 * TGAToMoImage.java
 * Chilli Source
 * Created by Scott Downie on 26/10/2012
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
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

package com.chilliworks.chillisource.tgatocsimage;
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.zip.CRC32;
import java.util.zip.Deflater;

import javax.imageio.ImageIO;
import javax.imageio.spi.IIORegistry;

import com.chilliworks.chillisource.coreutils.Logging;

//============================================================================
/// TGA To MoImage
///
/// Handles all tga conversions to the various csimage formats.
///
/// Version History
///
/// 2 - First release version, no comporession @auther SDownie
/// 3 - Added default zlib comporession @author RHenning
//============================================================================
public class TGAToCSImage 
{
	//------------------------------------------------------------------------
	/// Constants
	//------------------------------------------------------------------------
	private final static int kdwVersion = 3;
	
	private static long mTGAFileSize = 0;
	
	//------------------------------------------------------------------------
	/// Run
	///
	/// Converts a TGA to MoImage format based upon the given options.
	//------------------------------------------------------------------------
	public static void Run(TGAToCSImageOptions inOptions) throws IOException
	{
		IIORegistry registry = IIORegistry.getDefaultInstance();
		registry.registerServiceProvider(new com.realityinteractive.imageio.tga.TGAImageReaderSpi());
		
		long ddwCRC = 0;
		int dwOriginalDataSize = 0;
		
		//Open the tga
		ImageContainer image = null;
		try 
		{
			Logging.logVerbose("Loading TGA...");
			String strInputFile = inOptions.strInputFilename;
			Logging.logVerbose("Input File: " + strInputFile);
			image = LoadTGA(strInputFile);
			Logging.logVerbose("Loading TGA Complete");
		} 
		catch (IOException e) 
		{
			Logging.logVerbose("Cannot load TGA file");
			e.printStackTrace();
			return;
		}
		
		//pre multiply data if required
		boolean bPreMultiply = inOptions.bPremultiply;
		if(image.bHasAlpha && bPreMultiply == true)
		{
			Logging.logVerbose("Premultiplying Alpha...");
			PreMultiplyImage(image);
			Logging.logVerbose("Premultiplying Alpha Complete");
		}
		
		//build the image data in the output format
		Logging.logVerbose("Building Image Data...");
		TGAToCSImageOptions.OUTPUT_FORMAT imageFormat = GetOutputFormat(inOptions, image);
		boolean bDithering = inOptions.bDither;
		byte[] outImageData = ConvertImageToFormat(image, imageFormat, bDithering);
		if(outImageData == null)
		{
			return;
		}
		dwOriginalDataSize = outImageData.length;
		Logging.logVerbose("Building Image Data Complete");

		//Perform compression if required
		if (inOptions.eCompressionType != TGAToCSImageOptions.COMPRESSION_FORMAT.NONE)
		{
			CRC32 checksum = new CRC32();
			checksum.update(outImageData);
			ddwCRC = checksum.getValue();
			
			Logging.logVerbose("Compressing Image...");
			outImageData = CompressImage(inOptions, outImageData);
			Logging.logVerbose("Compressing Image Complete");
		}
		
		//Create header and output
		try 
		{
			Logging.logVerbose("Outputting MoImage...");
			String strOutputFile = inOptions.strOutputFilename;
			Logging.logVerbose("Output File: " + strOutputFile);
			OutputMoImage(outImageData, imageFormat, inOptions.eCompressionType, ddwCRC, dwOriginalDataSize, image.dwWidth, image.dwHeight, strOutputFile);
			Logging.logVerbose("Outputting MoImage Complete");
		} 
		catch (IOException e) 
		{
			Logging.logVerbose("Cannot output moimage file");
			e.printStackTrace();
			return;
		}
	}
	//------------------------------------------------------------------------
	/// Load TGA
	///
	/// Loads the given TGA.
	//------------------------------------------------------------------------
	private static ImageContainer LoadTGA(String instrPath) throws IOException
	{
		if(instrPath == null)
		{
			throw new IOException();
		}

		File imageFile = new File(instrPath);
		mTGAFileSize = imageFile.length();
		
		BufferedImage image = ImageIO.read(imageFile);
		ImageContainer imageContainer = new ImageContainer();
		imageContainer.dwWidth = image.getWidth();
		imageContainer.dwHeight = image.getHeight();
		imageContainer.dwType = image.getType();
		imageContainer.bHasAlpha = image.getColorModel().hasAlpha();
		imageContainer.adwImageData = new int[imageContainer.dwWidth * imageContainer.dwHeight];
		
		switch (imageContainer.dwType)
		{
		case BufferedImage.TYPE_BYTE_GRAY:
			if (imageContainer.bHasAlpha == false)
				BuildImageL8(imageContainer, image);
			else
				BuildImageLA88(imageContainer, image);
			break;
		default:
			BuildImageDefault(imageContainer, image);	
		}
		
		return imageContainer;
	}
	//------------------------------------------------------------------------
	/// Build Image Default
	///
	/// The default image builder. Simply uses getRGB() to get the image data
	//------------------------------------------------------------------------
	private static void BuildImageDefault(ImageContainer inImageContainer, BufferedImage inImage)
	{
		for (int x = 0; x < inImageContainer.dwWidth; ++x)
		{
			for (int y = 0; y < inImageContainer.dwHeight; ++y)
			{
				int dwIndex = x + y * inImageContainer.dwWidth;
				inImageContainer.adwImageData[dwIndex] = inImage.getRGB(x, y);
			}
		}
	}
	//------------------------------------------------------------------------
	/// Build Image L8
	///
	/// The default image builder. Simply uses getRGB() to get the image data
	//------------------------------------------------------------------------
	private static void BuildImageL8(ImageContainer inImageContainer, BufferedImage inImage)
	{
		for (int x = 0; x < inImageContainer.dwWidth; ++x)
		{
			for (int y = 0; y < inImageContainer.dwHeight; ++y)
			{
				int dwIndex = x + y * inImageContainer.dwWidth;
				int dwL = inImage.getRaster().getSample(x, y, 0);
				int dwCombined 	 = (dwL << 0);
				dwCombined 		|= (dwL << 8);
				dwCombined 		|= (dwL << 16);
				dwCombined 		|= (0xFF << 24);
				inImageContainer.adwImageData[dwIndex] = dwCombined;
			}
		}
	}
	//------------------------------------------------------------------------
	/// Build Image LA88
	///
	/// The default image builder. Simply uses getRGB() to get the image data
	//------------------------------------------------------------------------
	private static void BuildImageLA88(ImageContainer inImageContainer, BufferedImage inImage)
	{
		for (int x = 0; x < inImageContainer.dwWidth; ++x)
		{
			for (int y = 0; y < inImageContainer.dwHeight; ++y)
			{
				int dwIndex = x + y * inImageContainer.dwWidth;
				int dwL = inImage.getRaster().getSample(x, y, 0);
				int dwA = inImage.getRaster().getSample(x, y, 1);
				int dwCombined 	 = (dwL << 0);
				dwCombined 		|= (dwL << 8);
				dwCombined 		|= (dwL << 16);
				dwCombined 		|= (dwA << 24);
				inImageContainer.adwImageData[dwIndex] = dwCombined;
			}
		}
	}
	//------------------------------------------------------------------------
	/// Get Output Format
	///
	/// @return the output format, based upon the given params and the type
	/// of the given image.
	//------------------------------------------------------------------------
	private static TGAToCSImageOptions.OUTPUT_FORMAT GetOutputFormat(TGAToCSImageOptions inOptions, ImageContainer inImage)
	{
		//if it has conversion type format, force output to this.
		if (inOptions.eConversionType != TGAToCSImageOptions.OUTPUT_FORMAT.NONE)
			return inOptions.eConversionType;
		
		//if it has alpha and a alpha image conversion type then use this type.
		if (inImage.bHasAlpha == true && inOptions.eConversionAlphaType != TGAToCSImageOptions.OUTPUT_FORMAT.NONE)
			return inOptions.eConversionAlphaType;
					
		//if it has no alpha and a no alpha image conversion type then use this type.
		if (inImage.bHasAlpha == false && inOptions.eConversionNoAlphaType != TGAToCSImageOptions.OUTPUT_FORMAT.NONE)
			return inOptions.eConversionNoAlphaType;

		//otherwise fall back on the type the image already is.
		switch (inImage.dwType)
		{
		case BufferedImage.TYPE_BYTE_GRAY:
			if (inImage.bHasAlpha == false)
				return TGAToCSImageOptions.OUTPUT_FORMAT.L8;
			else
				return TGAToCSImageOptions.OUTPUT_FORMAT.LA88;
		case BufferedImage.TYPE_BYTE_BINARY:
		case BufferedImage.TYPE_3BYTE_BGR:
		case BufferedImage.TYPE_4BYTE_ABGR:
		case BufferedImage.TYPE_BYTE_INDEXED:
		case BufferedImage.TYPE_INT_ARGB:
		case BufferedImage.TYPE_INT_ARGB_PRE:
		case BufferedImage.TYPE_INT_BGR:
		case BufferedImage.TYPE_INT_RGB:
		case BufferedImage.TYPE_CUSTOM:
			if (inImage.bHasAlpha == false)
				return TGAToCSImageOptions.OUTPUT_FORMAT.RGB888;
			else
				return TGAToCSImageOptions.OUTPUT_FORMAT.RGBA8888;
		default:
			return TGAToCSImageOptions.OUTPUT_FORMAT.NONE;	
		}
	}
	//------------------------------------------------------------------------
	/// PreMultiply Image
	///
	/// Pre multiplies the alpha of the given image.
	//------------------------------------------------------------------------
	private static void PreMultiplyImage(ImageContainer inImage)
	{
		int numPixels = inImage.dwWidth * inImage.dwHeight; 

		for(int pixel=0; pixel<numPixels; ++pixel)
		{
			int rgba = inImage.adwImageData[pixel];
			
			final int A = (rgba >>> 24 	& 0xFF); 
			final int R = CalculatePremultipliedAlpha(rgba >>> 16 & 0xFF, A); 
			final int G = CalculatePremultipliedAlpha(rgba >>>  8 & 0xFF, A); 
			final int B = CalculatePremultipliedAlpha(rgba >>>  0 & 0xFF, A);
			
			rgba = 	(A & 0xFF) << 24 |
					(R & 0xFF) << 16 |
					(G & 0xFF) << 8  | 
					(B & 0xFF);
			
			inImage.adwImageData[pixel] = rgba;
		}
	}
	//------------------------------------------------------------------------
	/// Convert Image To Format
	///
	/// Converts the image to the  requested output format.
	//------------------------------------------------------------------------
	private static byte[] ConvertImageToFormat(ImageContainer inImage, TGAToCSImageOptions.OUTPUT_FORMAT inFormatFlag, boolean inbDither)
	{
		switch (inFormatFlag)
		{
		case L8:
			return ConvertToL8(inImage);
		case LA88:
			return ConvertToLA88(inImage);
		case RGB565:
			if(inImage.dwWidth % 2 > 0)
			{
				Logging.logError("Cannot convert an image that is not divisible by 2 to RGB565 Format.");
				return null;
			}
			return ConvertToRGB565(inImage, inbDither);
		case RGBA4444:
			if(inImage.dwWidth % 2 > 0)
			{
				Logging.logError("Cannot convert an image that is not divisible by 2 to RGBA4444 Format.");
				return null;
			}
			return ConvertToRGBA4444(inImage, inbDither);
		case RGB888:
			return ConvertToRGB888(inImage); 
		case RGBA8888:
			return ConvertToRGBA8888(inImage);
		case NONE:
		default:
			Logging.logError("Invalid output format.");
			return null;
		}
	}
	//------------------------------------------------------------------------
	/// Convert To RGBA8888
	///
	/// Converts to format RGBA8888.
	//------------------------------------------------------------------------
	private static byte[] ConvertToRGBA8888(ImageContainer inImage)
	{
		Logging.logVerbose("Converting to RGBA8888");

		int area = inImage.dwHeight * inImage.dwWidth;
		byte[] outByteData = new byte[area * 4];

		int i = 0;
		for(int pixel=0; pixel<area; i+=4, ++pixel)
		{
			final int rgba = inImage.adwImageData[pixel];
	
			outByteData[i+0] = (byte)(rgba >>> 16 & 0xFF); 
			outByteData[i+1] = (byte)(rgba >>>  8 & 0xFF); 
			outByteData[i+2] = (byte)(rgba >>>  0 & 0xFF);
			outByteData[i+3] = (byte)(rgba >>> 24 & 0xFF); 
		}

		return outByteData;
	}
	//------------------------------------------------------------------------
	/// Convert To RGB888
	///
	/// Converts to format RGB888.
	//------------------------------------------------------------------------
	private static byte[] ConvertToRGB888(ImageContainer inImage)
	{
		Logging.logVerbose("Converting to RGB888");

		int area = inImage.dwHeight * inImage.dwWidth;
		byte[] outByteData = new byte[area * 3];

		int i = 0;
		for(int pixel=0; pixel<area; i+=3, ++pixel)
		{
			final int rgba = inImage.adwImageData[pixel];

			outByteData[i]   = (byte)(rgba >>> 16	& 0xFF);
			outByteData[i+1] = (byte)(rgba >>> 8  	& 0xFF);
			outByteData[i+2] = (byte)(rgba >>> 0 	& 0xFF);
		}

		return outByteData;
	}
	//------------------------------------------------------------------------
	/// Convert To L8
	///
	/// Converts to format L8.
	//------------------------------------------------------------------------
	private static byte[] ConvertToL8(ImageContainer inImage)
	{
		Logging.logVerbose("Converting to L8");
		int area = inImage.dwHeight * inImage.dwWidth;
		byte[] outByteData = new byte[area];

		int i = 0;
		for(int pixel=0; pixel<area; i++, ++pixel)
		{
			final int rgba = inImage.adwImageData[pixel];
			int byR = (int)(rgba >>> 16	& 0xFF);
			int byG = (int)(rgba >>> 8  & 0xFF);
			int byB = (int)(rgba >>> 0 	& 0xFF);
			int dwSum = byR + byG + byB;
			outByteData[i] = (byte)(dwSum / 3);
		}

		return outByteData;
	}
	//------------------------------------------------------------------------
	/// Convert To LA88
	///
	/// Converts to format LA88.
	//------------------------------------------------------------------------
	private static byte[] ConvertToLA88(ImageContainer inImage)
	{
		Logging.logVerbose("Converting to LA88");

		int area = inImage.dwHeight * inImage.dwWidth;
		byte[] outByteData = new byte[area * 2];

		int i = 0;
		for(int pixel=0; pixel<area; i+=2, ++pixel)
		{
			final int rgba = inImage.adwImageData[pixel];
	
			int byR = (int)(rgba >>> 16 & 0xFF); 
			int byG = (int)(rgba >>>  8 & 0xFF); 
			int byB = (int)(rgba >>>  0 & 0xFF);
			int byA = (int)(rgba >>> 24 & 0xFF); 
			int dwSum =  ((int)byR) + ((int)byG) + ((int)byB);
			outByteData[i + 0] = (byte)(dwSum / 3);
			outByteData[i + 1] = (byte)byA;
		}

		return outByteData;
	}
	//------------------------------------------------------------------------
	/// Convert To RGB565
	///
	/// Converts to format RGB565.
	//------------------------------------------------------------------------
	private static byte[] ConvertToRGB565(ImageContainer inImage, boolean inDither)
	{
		Logging.logVerbose("Converting to RGB565");
		
		if(inDither)
		{
			Logging.logVerbose("Dithering...");
		}

		int area = inImage.dwHeight * inImage.dwWidth;
		byte[] outByteData = new byte[area * 2];

		int i = 0;
		for(int pixel=0; pixel<area; i+=2, ++pixel)
		{
			final int rgba =inImage.adwImageData[pixel];
			
			int oR = (rgba >>> 16 & 0xFF);
			int oG = (rgba >>>  8 & 0xFF);
			int oB = (rgba >>>  0 & 0xFF);
			
			int R = oR >>> 3;
			int G = oG >>> 2;
			int B = oB >>> 3;
			
			if(inDither)
			{
				DitherSierraLite(inImage, pixel % inImage.dwWidth, pixel / inImage.dwWidth, oR - (R<<3), oG - (G<<2), oB - (B<<3), 0);
				//DitherSierra3(inImage, pixel % inImage.getWidth(), pixel / inImage.getWidth(), oR - (R<<3), oG - (G<<2), oB - (B<<3), 0);
			}

			short combined = (short) 
					(
						(R << 11) | 
						(G << 5)  | 
						(B << 0)
					);

			outByteData[i]	 = (byte)(combined >>> 0  & 0xFF);
			outByteData[i+1] = (byte)(combined >>> 8  & 0xFF);
		}

		return outByteData;
	}
	//------------------------------------------------------------------------
	/// Convert To RGBA4444
	///
	/// Converts to format RGBA4444.
	//------------------------------------------------------------------------
	private static byte[] ConvertToRGBA4444(ImageContainer inImage, boolean inDither)
	{
		Logging.logVerbose("Converting to RGBA4444");
		
		if(inDither)
		{
			Logging.logVerbose("Dithering...");
		}

		final int area = inImage.dwHeight * inImage.dwWidth;
		byte[] outByteData = new byte[area * 2];

		int i = 0;
		for(int pixel=0; pixel<area; i+=2, ++pixel)
		{
			int rgba = inImage.adwImageData[pixel];

			int oR = (rgba >>> 16 & 0xFF);
			int oG = (rgba >>>  8 & 0xFF);
			int oB = (rgba >>>  0 & 0xFF);
			int oA = (rgba >>> 24 & 0xFF);

			int R = oR >>> 4;
			int G = oG >>> 4;
			int B = oB >>> 4;
			int A = oA >>> 4;
			
			if(inDither)
			{
				DitherSierraLite(inImage, pixel % inImage.dwWidth, pixel / inImage.dwWidth, oR - (R<<4), oG - (G<<4), oB - (B<<4), oA - (A<<4));
				//DitherSierra3(inImage, pixel % inImage.getWidth(), pixel / inImage.getWidth(), oR - (R<<4), oG - (G<<4), oB - (B<<4), oA - (A<<4));
			}

			short combined = 
					(short) 
					(
						(R << 12)| 
						(G << 8) | 
						(B << 4) | 
						(A << 0)
					); 

			outByteData[i]	 = (byte)(combined >>> 0  & 0xFF);
			outByteData[i+1] = (byte)(combined >>> 8  & 0xFF);
		}

		return outByteData;
	}
	//------------------------------------------------------------------------
	/// Apply Error
	//------------------------------------------------------------------------
	private static int ApplyError(int rgba, float factor, int errorR, int errorG, int errorB, int errorA)
	{
		int a = (rgba >>> 24 & 0xFF);
		int r = (rgba >>> 16 & 0xFF); 
		int g = (rgba >>> 8 & 0xFF); 
		int b = (rgba 		& 0xFF);
		

		r+= (errorR * factor);
		r = r < 0 ? 0 : r;
		r = r > 255? 255 : r;

		g+= (errorG * factor);
		g = g < 0 ? 0 : g;
		g = g > 255? 255 : g;

		b+= (errorB * factor); 
		b = b < 0 ? 0 : b;
		b = b > 255? 255 : b;

		a+= (errorA * factor);
		a = a < 0 ? 0 : a;
		a = a > 255? 255 : a;

		return 	(a & 0xFF) << 24 |
				(r & 0xFF) << 16 |
				(g & 0xFF) << 8  | 
				(b & 0xFF);

	}
	//------------------------------------------------------------------------
	/// Dither Sierra Lite
	//------------------------------------------------------------------------
	private static void DitherSierraLite(ImageContainer inImage, int x, int y, int errorR, int errorG, int errorB, int errorA )
	{
		//        *   2                          The Sierra-2-4A filter
		//    1   1               (1/4)
		
		if(x < inImage.dwWidth - 1)
		{
			int dwIndex = (x+1) + (y+0) * inImage.dwWidth;
			int rgba = inImage.adwImageData[dwIndex];
			rgba = ApplyError(rgba, 2.0f/ 4.0f, errorR, errorG, errorB, errorA);
			inImage.adwImageData[dwIndex] = rgba;
		}
		if(y < inImage.dwHeight - 1)
		{
			int dwIndex = (x+0) + (y+1) * inImage.dwWidth;
			int rgba = inImage.adwImageData[dwIndex];
			rgba = ApplyError(rgba, 1.0f/ 4.0f, errorR, errorG, errorB, errorA);
			inImage.adwImageData[dwIndex] = rgba;
			
			if(x > 0)
			{
				dwIndex = (x-1) + (y+1) * inImage.dwWidth;
				rgba = inImage.adwImageData[dwIndex];
				rgba = ApplyError(rgba, 1.0f/ 4.0f, errorR, errorG, errorB, errorA);
				inImage.adwImageData[dwIndex] = rgba;
			}
		}
	}
//	//------------------------------------------------------------------------
//	/// Dither Sierra 3
//	//------------------------------------------------------------------------
//	private static void DitherSierra3(BufferedImage inImage, int x, int y, int errorR, int errorG, int errorB, int errorA)
//	{
//		//        *   5   3                  The Sierra3 filter
//		//2   4   5   4   2
//		//    2   3   2       (1/32)
//		
//		if(x < inImage.getWidth() - 1)
//		{
//			int rgba = inImage.getRGB(x+1, y);
//			rgba = ApplyError(rgba, 5.0f/ 32.0f, errorR, errorG, errorB, errorA);
//			inImage.setRGB(x+1, y, rgba);
//			
//			if(x < inImage.getWidth() - 2)
//			{
//				rgba = inImage.getRGB(x+2, y);
//				rgba = ApplyError(rgba, 3.0f/ 32.0f, errorR, errorG, errorB, errorA);
//				inImage.setRGB(x+2, y, rgba);
//			}
//		}
//		
//		if(y < inImage.getHeight() - 1)
//		{
//			int rgba = inImage.getRGB(x, y+1);
//			rgba = ApplyError(rgba, 5.0f/ 32.0f, errorR, errorG, errorB, errorA);
//			inImage.setRGB(x, y+1, rgba);
//			
//			if(y < inImage.getHeight() - 2)
//			{
//				rgba = inImage.getRGB(x, y+2);
//				rgba = ApplyError(rgba, 3.0f/ 32.0f, errorR, errorG, errorB, errorA);
//				inImage.setRGB(x, y+2, rgba);
//				
//				if(x > 0)
//				{
//					rgba = inImage.getRGB(x-1, y+2);
//					rgba = ApplyError(rgba, 2.0f/ 32.0f, errorR, errorG, errorB, errorA);
//					inImage.setRGB(x-1, y+2, rgba);
//				}
//				
//				if(x < inImage.getWidth() - 1)
//				{
//					rgba = inImage.getRGB(x+1, y+2);
//					rgba = ApplyError(rgba, 2.0f/ 32.0f, errorR, errorG, errorB, errorA);
//					inImage.setRGB(x+1, y+2, rgba);
//				}
//			}
//			
//			if(x > 0)
//			{
//				rgba = inImage.getRGB(x-1, y+1);
//				rgba = ApplyError(rgba, 4.0f/ 32.0f, errorR, errorG, errorB, errorA);
//				inImage.setRGB(x-1, y+1, rgba);
//			}
//			
//			if(x > 1)
//			{
//				rgba = inImage.getRGB(x-2, y+1);
//				rgba = ApplyError(rgba, 2.0f/ 32.0f, errorR, errorG, errorB, errorA);
//				inImage.setRGB(x-2, y+1, rgba);
//			}
//			
//			if(x < inImage.getWidth() - 1)
//			{
//				rgba = inImage.getRGB(x+1, y+1);
//				rgba = ApplyError(rgba, 4.0f/ 32.0f, errorR, errorG, errorB, errorA);
//				inImage.setRGB(x+1, y+1, rgba);
//			}
//			
//			if(x < inImage.getWidth() - 2)
//			{
//				rgba = inImage.getRGB(x+2, y+1);
//				rgba = ApplyError(rgba, 2.0f/ 32.0f, errorR, errorG, errorB, errorA);
//				inImage.setRGB(x+2, y+1, rgba);
//			}
//		}
//	}
	//------------------------------------------------------------------------
	/// Calculate Premultiplied Alpha
	///
	/// @return the premultiplied alpha value for the given pixel.
	//------------------------------------------------------------------------
	private static int CalculatePremultipliedAlpha(int inCol, int inAlpha)
	{
		return (inCol*inAlpha)/255;
	}
	//------------------------------------------------------------------------
	/// Compress Image
	///
	/// @return compresses the image with the requested compression algorithm.
	//------------------------------------------------------------------------
	private static byte[] CompressImage(TGAToCSImageOptions inOptions, byte[] inImageData) throws IOException
	{
		if(inOptions.eCompressionType != TGAToCSImageOptions.COMPRESSION_FORMAT.DEFAULT_ZLIB)
		{
			Logging.logFatal("Unsupported compression. Image will not be compressed");
			return inImageData;
		}
		
		// Compression using zlib
		Deflater compresser = new Deflater();
		compresser.setInput(inImageData);
		compresser.finish();

		ByteArrayOutputStream out = new ByteArrayOutputStream(inImageData.length);
		byte[] byBuffer = new byte[8192]; // Write in 8K chunks
		while(!compresser.finished())
		{
			int dwCompressedBytes = compresser.deflate(byBuffer);
			out.write(byBuffer, 0, dwCompressedBytes);
		}
		out.close();

		byte[] abyCompressedImge = out.toByteArray();

		return abyCompressedImge;
	}
	//------------------------------------------------------------------------
	/// Output MoImage
	///
	/// Outputs the generated csimage data to file.
	//------------------------------------------------------------------------
	private static void OutputMoImage(byte[] inImageData,
									  TGAToCSImageOptions.OUTPUT_FORMAT inFormat,
									  TGAToCSImageOptions.COMPRESSION_FORMAT inCompression,
									  long inddwChecksum, int indwOriginalSize,
									  int inWidth, int inHeight,
									  String instrOutputFile) throws IOException
	{
		if(instrOutputFile == null)
		{
			throw new IOException();
		}

		///Apply the header
		// Byte order check - int
		// Version - int
		// Width - int
		// Height - int
		// Format - int
		// Compression - int
		// Checksum (CRC32)
		// Uncompressed data size
		// Final data size - int
		int byteOrderCheck = 123456;
		int version = kdwVersion;
		int compression = 0;
		if(inCompression == TGAToCSImageOptions.COMPRESSION_FORMAT.DEFAULT_ZLIB)
			compression = 1;
		
		//Write out the data
		FileOutputStream fileStream = new FileOutputStream(instrOutputFile);
		WriteInt(fileStream, byteOrderCheck);
		WriteInt(fileStream, version);
		WriteInt(fileStream, inWidth);
		WriteInt(fileStream, inHeight);
		WriteInt(fileStream, inFormat.ordinal());
		WriteInt(fileStream, compression);
		WriteLong(fileStream, inddwChecksum);
		WriteInt(fileStream, indwOriginalSize);
		WriteInt(fileStream, inImageData.length);
		
		Logging.logVerbose("Completed writting file with header:\n\tOrderCheck:"+byteOrderCheck
																+"\n\tVersion:"+version
																+"\n\tWidth:"+inWidth
																+"\n\tHeight:"+inHeight
																+"\n\tFormat:"+inFormat.ordinal()
																+"\n\tCompression:"+compression
																+"\n\tChecksum:"+inddwChecksum
																+"\n\tOriginalSize:"+indwOriginalSize
																+"\n\tFinalSize:"+inImageData.length);
		
		fileStream.write(inImageData);
		fileStream.close();
		
		File imageFile = new File(instrOutputFile);
		Logging.logVerbose("MoImage: " + instrOutputFile);
		Logging.logVerbose("TGA File Size: " + Math.ceil((float)mTGAFileSize / (float)1024) + " KB");
		Logging.logVerbose("MoImage Uncompressed Size: " + Math.ceil((float)indwOriginalSize / (float)1024) + " KB");
		Logging.logVerbose("MoImage File Size: " + Math.ceil((float)imageFile.length() / (float)1024) + " KB");
		Logging.logVerbose("");
	}
	//-------------------------------------------------------
	/// Write Integer
	///
	/// Writes a single Integer to the output stream.
	///
	/// @param the integer.
	//-------------------------------------------------------
	private static void WriteInt(FileOutputStream inStream, int indwData) throws IOException
	{
		inStream.write((byte)(indwData 			& 0xFF));
		inStream.write((byte)(indwData >>> 8 	& 0xFF));
		inStream.write((byte)(indwData >>> 16 	& 0xFF));
		inStream.write((byte)(indwData >>> 24	& 0xFF));
	}
	//-------------------------------------------------------
	/// Write Long Integer
	///
	/// Writes a single Integer to the output stream.
	///
	/// @param the integer.
	//-------------------------------------------------------
	private static void WriteLong(FileOutputStream inStream, long indwData) throws IOException
	{
		inStream.write((byte)(indwData 			& 0xFF));
		inStream.write((byte)(indwData >>> 8 	& 0xFF));
		inStream.write((byte)(indwData >>> 16 	& 0xFF));
		inStream.write((byte)(indwData >>> 24	& 0xFF));
		inStream.write((byte)(indwData >>> 32	& 0xFF));
		inStream.write((byte)(indwData >>> 40	& 0xFF));
		inStream.write((byte)(indwData >>> 48 	& 0xFF));
		inStream.write((byte)(indwData >>> 56	& 0xFF));
	}
}
