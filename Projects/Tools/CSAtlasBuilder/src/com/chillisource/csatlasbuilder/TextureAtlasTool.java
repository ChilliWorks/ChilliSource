/**
*  TextureAtlasTool.java
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
import com.chillisource.toolutils.StringUtils;

import java.util.*;
 
/*
 * Revision history.
 * 1.00 First release.
 * 1.01 Added ability to force order of files.
 * 1.02 Added ability to have blank padding round each frame.
 * 1.03 Fixed offset output if padding used.
 * 1.04 Updated output to be uppercase for .java and .h files.
 * 1.05 Added java package optional parameter, UPPERCASE source file name
 * 2.00 Refactored heavily. Added kerning capability to font output. Stuart McGaw, 12/05/2011
 * 2.01 Corrected UTF-8 output of alphabet files and added usage output on error
 * 2.02 Refactored to use correct package 'com.taggames.tools'. Fixed duplicate image detection
 * 		Removed generating .h and .java file. Added -maxwidth, -maxheight and -divisibleby args.
 * 		Fixed cropped frames at bottom of final output bug. R Henning, 20/06/2012
 * 2.03 Fixed the horizontal expansion of the sheet and the cropped frames. N Tanda, 02/08/2012
 * 2.04 Refactored for clarity and added some doxygen comments.  A Glass, 17/08/2012
 * 2.05 Improved packing density after refactor A Glass, 06/09/2012
 * 2.06 Improved speed of tool with many sprites (added collision grid and earlier check for identical files) A Glass, 14/09/2012
 * 2.07 Added heuristic choices and actually made the tool use them A Glass, 18/09/2012
 * 2.08	Added inner padding options	A Glass, 02/10/2012
 * 2.09 Added an option -simplespriteids for simplifying the output sprite id's. Ian Copland, 16/10/2012
 * 2.10 Added an option for extruding the border pixels. This can be used to stop bleeding from surrounding images. Ian Copland, 16/10/2012
 * 2.11 Added output for original sprite sheet size
 * 2.12 Updated kerning.
 * 2.13 Removed font into another tool.
 * 		Changed to always use simple sprite IDs and no uppercasing explicitly
 * 		ALways uses the latest version 
 * 		Removed xml output as it was useless. S Downie 17/06/2014
 * 		Splitting packing into separate lib so it can be shared with font tool
 */
public class TextureAtlasTool
{
	private static final String VERSION = "2.13";
	private static short k_versionNum = 3;

	public static void main(String[] args) throws Exception
	{
		TextureAtlasTool tool = new TextureAtlasTool();
		tool.Run(args);
	}

	boolean verboseOutput = false;
	int numPixelsPadding = 0;

	// Output image sizes
	int fixedHeight = -1;
	int fixedWidth = -1;
	int validHeights[];
	int validWidths[];
	int maxWidth = -1;
	int maxHeight = -1;
	int divisibleBy = -1;
	int innerPadding = 0;
	int extrude = 0;
	boolean cropImages = true;	// When true we crop transparencies in the image
	TexturePacker.PlacementHeuristic eBestPlacement = TexturePacker.PlacementHeuristic.BOTTOMRIGHT;

	String inputDirectoryName = null; // Directory relative to working directory
	// where (commandline -dir, -directory)
	File rootDirectory; // Root directory from which files are read,set from
	// the above
	String orderingFileName = null;
	String outputName = null; // This forms the root for any output files'
	
	Hashtable<String, String> options = new Hashtable<String, String>();

	private void addImageFilesInDirectory(File dir, boolean recursiveDirectorySearch, ArrayList<File> files)
	{
		String[] dirContents = dir.list();

		for (int i = 0; i < dirContents.length; i++)
		{
			File contentFile = new File(dir, dirContents[i]);

			if (contentFile.isDirectory() && recursiveDirectorySearch)
			{
				addImageFilesInDirectory(contentFile, false, files); // only go one level deep
			}
			else
			{
				boolean IsPngFile = dirContents[i].endsWith(".png") || dirContents[i].endsWith(".PNG");
				if (IsPngFile)
					files.add(contentFile);
			}
		}
	}

	private String enumNameFromFile(File file) throws IOException
	{
		//choose the output format for the filenames.
		String srcFilePath = file.getCanonicalPath();
		String srcFilePathWithoutExtension = srcFilePath.substring(0, srcFilePath.lastIndexOf("."));
		String rootDir = rootDirectory.getCanonicalPath();
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

	private void loadFilesFromOrderingFile(ArrayList<File> files) throws Exception
	{
		File f = new File(inputDirectoryName + "/" + orderingFileName);
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
				files.add(new File(inputDirectoryName + "/" + line));
		}
	}

	private void processCommandLineArgs(String[] args) throws NumberFormatException, Exception
	{
		for (int i = 0; i < args.length; i++)
		{
			if (args[i].equalsIgnoreCase("--verbose"))
			{
				verboseOutput = true;
			}
			else
			if (args[i].equalsIgnoreCase("--help"))
			{
				Usage();
			}
			else
			{
				options.put(args[i], args[i + 1]);
				i++;
			}
		}

		if (options.containsKey("--crop"))
		{
			String crop = options.get("--crop");
			int tmp = Integer.parseInt(crop);
			if (tmp == 1)
			{
				cropImages = true;
				System.out.println("set crop to true");
			}	
			else
			{
				cropImages = false;
				System.out.println("set crop to false");
			}
		}
		
		if (options.containsKey("--input"))
		{
			inputDirectoryName = options.get("--input");
		}
		else
		{
			throw new Exception("Error: no source directory!");
		}

		if (options.containsKey("--output"))
		{
			outputName = options.get("--output");
			outputName = StringUtils.removeExtension(outputName);
		}
		else
		{
			throw new Exception("Error: no output filepath!");
		}

		if (options.containsKey("--padding"))
		{
			String pad = options.get("--padding");
			numPixelsPadding = Integer.parseInt(pad);
			if (verboseOutput)
			{
				System.out.println("Padding set to:" + numPixelsPadding);
			}
		}

		if (options.containsKey("--filelist"))
		{
			orderingFileName = options.get("--filelist");
		}

		if (options.containsKey("--width"))
		{
			fixedWidth = Integer.decode(options.get("--width"));
		}

		if (options.containsKey("--height"))
		{
			fixedHeight = Integer.decode(options.get("--height"));
		}

		if (options.containsKey("--validwidths"))
		{
			int tempValidWidths[] = new int[256];
			
			String s = options.get("--validwidths");
			int numValidW = 0;
			int index = s.indexOf(':');
			while (index > 0)
			{
				String p = s.substring(0, s.indexOf(':'));
				s = s.substring(s.indexOf(':') + 1, s.length());
				int w = Integer.decode(p);
				tempValidWidths[numValidW] = w;
				numValidW++;
				index = s.indexOf(':');
			}
			int w = Integer.decode(s);
			tempValidWidths[numValidW] = w;
			numValidW++;
			
			validWidths = new int[numValidW];
			for(int i=0; i<numValidW; ++i)
			{
				validWidths[i] = tempValidWidths[i];
			}
		}

		if (options.containsKey("--validheights"))
		{
			int tempValidHeights[] = new int[256];
			
			String s = options.get("--validheights");
			int numValidH = 0;
			int index = s.indexOf(':');
			while (index > 0)
			{
				String p = s.substring(0, s.indexOf(':'));
				s = s.substring(s.indexOf(':') + 1, s.length());
				int h = Integer.decode(p);
				tempValidHeights[numValidH] = h;
				numValidH++;
				index = s.indexOf(':');
			}
			int h = Integer.decode(s);
			tempValidHeights[numValidH] = h;
			numValidH++;
			
			validHeights = new int[numValidH];
			for(int i=0; i<numValidH; ++i)
			{
				validHeights[i] = tempValidHeights[i];
			}
		}
		
		if (options.containsKey("--maxwidth"))
		{
			maxWidth = Integer.decode(options.get("--maxwidth"));
		}
		
		if (options.containsKey("--maxheight"))
		{
			maxHeight = Integer.decode(options.get("--maxheight"));
		}
		
		if (options.containsKey("--divisibleby"))
		{
			divisibleBy = Integer.decode(options.get("--divisibleby"));
		}

		if(options.containsKey("--heuristic"))
		{
			try
			{
				eBestPlacement = TexturePacker.PlacementHeuristic.valueOf(options.get("--heuristic").toUpperCase());
			}
			catch(Exception e)
			{
				System.out.println(options.get("--heuristic").toUpperCase() + " is not a valid heuristic");
			}
		}
		
		if(options.containsKey("--innerPadding"))
		{
			innerPadding = Integer.decode(options.get("-innerPadding"));
		}
		
		if(options.containsKey("--extrude"))
		{
			extrude = Integer.decode(options.get("--extrude"));
		}
	}

	private void Usage()
	{
		System.out.println("Help:");
		System.out.println("Example TextureAtlas creation:");
		System.out.println("java -jar TextureAtlasTool.jar -mode 0 -padding PADDING -input INPUT_DIR -validwidths VALID_WIDTH_LIST -validheight VALID_HEIGHT_LIST -output OUTPUT_DIR");
		System.out.println("\tCommands:");
		System.out.println("\t\t--padding value \t\t\t<Spacing (in pixels) between items on spritesheet>");
		System.out.println("\t\t--input \t\t<Directory to process. By default will process all contained PNGs");
		System.out.println("\t\t--output filename \t\t<Output PNG filename>");
		System.out.println("\t\t--filelist listfile \t\t<listfile contains list of .PNG files to include>");
		System.out.println("\t\t--width value \t\t\t<Force output width to be value>");
		System.out.println("\t\t--height value \t\t\t<Force output height to be value>");
		System.out.println("\t\t--validwidths v1:v2:v3:... \t<Choose best match final width from list>");
		System.out.println("\t\t--validheights v1:v2:v3:... \t<Choose best match final height from list>");
		System.out.println("\t\t--maxwidth value \t\t\t<Set a max width value, final image may be <= this value>");
		System.out.println("\t\t--maxheight value \t\t\t<Set a max height value, final image may be <= this value>");
		System.out.println("\t\t--divisibleby value \t\t\t<Dimensions of image must be divisible by this value>");
		System.out.println("\t\t--verbose \t\t\t<Additional processing information>");
		System.out.println("\t\t--crop \t\t\tChoose from DO CROP (1) or DO NOT CROP (0). Default is DO CROP.");
		System.out.println("\t\t--heuristic \t\t\tPacking heuristic (current choice of TOPLEFT or BOTTOMRIGHT)");
		System.out.println("\t\t--innerPadding value \t\t\tOptional amount of pixels to leave untrimmed/cropped from the sprite");
		System.out.println("\t\t--extrude [true/false] \t\t\tExtrudes the border of the image. Default is 0.");
		System.out.println("\t\t--convert' \t\t\tThe type to convert to.");
		System.out.println("\t\t--convertalpha' \t\t\tThe type to convert images with alpha to.");
		System.out.println("\t\t--convertnoalpha' \t\t\tThe type to convert images without alpha to.");
		System.out.println("\t\t--compression' \t\t\tThe compression type.");
		System.out.println("\t\t--premultiply [true/false (1/0)]' \t\t\tWhether or not to premultiply the output csimage.");
		System.out.println("\t\t--dither [true/false (1/0)]' \t\t\tWhether or not to dither if converting to a smaller image format.");
		System.out.println("Conversion Types:");
		System.out.println("\t\t\tL8");
		System.out.println("\t\t\tLA88");
		System.out.println("\t\t\tRGB565");
		System.out.println("\t\t\tRGBA4444");
		System.out.println("\t\t\tRGB888");
		System.out.println("\t\t\tRGBA8888");
		System.out.println("Compression Types:");
		System.out.println("\t\t\tNone");
		System.out.println("\t\t\tDefault");
	}
	
	private boolean Run(String[] args) throws Exception
	{
		System.out.println("TextureAtlasTool version "+VERSION);
		System.out.println("-----------------------");

		try
		{
			processCommandLineArgs(args);
		}
		catch(Exception e)
		{
			System.out.println(e.getMessage());
			Usage();
			return false;
		}

		System.out.println("input dir name is:\"" + inputDirectoryName + "\"");

		ArrayList<File> filesToProcess = new ArrayList<File>();

		rootDirectory = new File(inputDirectoryName);
		if (orderingFileName != null)
		{
			loadFilesFromOrderingFile(filesToProcess);
		}
		else
		{
			addImageFilesInDirectory(rootDirectory, true, filesToProcess);
		}
		
		TexturePacker packer = new TexturePacker();
		packer
		.setDivisibleBy(divisibleBy)
		.setExtrusion(extrude)
		.setFixedHeight(fixedHeight)
		.setFixedWidth(fixedWidth)
		.setMaxHeight(maxHeight)
		.setMaxWidth(maxWidth)
		.setValidHeights(validHeights)
		.setValidWidths(validWidths)
		.setHeuristic(eBestPlacement)
		.setInnerPadding(innerPadding)
		.setOuterPadding(numPixelsPadding)
		.enableCropping(cropImages)
		.enableVerboseOutput(verboseOutput);
		
		PackedTexture result = packer.pack(filesToProcess);
		if(result == null)
		{
			return false;
		}
		
		///////////////////////////////////////////////////////
		// It's output time!
		///////////////////////////////////////////////////////
		// Write our combined png file
		File F = new File(outputName + ".png");
		ImageIO.write(result.getPackedTexture(), "png", F);

		writeBinaryFile(result);
		writeStringIDs(result);
		convertToCSImage();

		System.out.printf("Goodbye!\n");

		return true;
	}
	
	private void convertToCSImage()
	{
		System.out.println("Converting to CSImage");
		
		LinkedList<String> commands = new LinkedList<String>();
		commands.add("java");
		commands.add("-Djava.awt.headless=true");
		commands.add("-jar");
		commands.add(FileUtils.getPathToHere() + "PNGToCSImage.jar");
		commands.add("--input");
		commands.add(outputName + ".png");
		commands.add("--output");
		commands.add(outputName + ".csimage");
		if(options.containsKey("--compression"))
		{
			commands.add("--compression");
			commands.add(options.get("--compression"));
		}
		if(options.containsKey("--convert"))
		{
			commands.add("--convert");
			commands.add(options.get("--convert"));
		}
		if(options.containsKey("--dither"))
		{
			if(options.get("--dither") == "1")
			{
				commands.add("--dither");
			}
		}
		if(options.containsKey("--premultiply"))
		{
			if(options.get("--premultiply") == "1")
			{
				commands.add("--premultiply");
			}
		}
		if(options.containsKey("--convertalpha"))
		{
			commands.add("--convertalpha");
			commands.add(options.get("--convertalpha"));
		}
		if(options.containsKey("--convertnoalpha"))
		{
			commands.add("--convertnoalpha");
			commands.add(options.get("--convertnoalpha"));
		}
		
		ExternalProcess csImageConversion = new ExternalProcess();
		csImageConversion.run(commands);
		
		FileUtils.deleteFile(outputName + ".png");
		
		if(verboseOutput)
			System.out.println(csImageConversion.getMessages());
		
		System.out.println(csImageConversion.getWarnings());
		System.out.println(csImageConversion.getErrors());
	}

	private void writeBinaryFile(PackedTexture in_packedTexture) throws FileNotFoundException, IOException, Exception
	{
		int numImages = in_packedTexture.getNumImages();
		LittleEndianOutputStream dosBinary = new LittleEndianOutputStream(outputName + ".csatlas");
		dosBinary.writeShort((short) numImages);
		dosBinary.writeShort(k_versionNum); // file format revision

		//Write out spritesheet size
		dosBinary.writeShort((short) in_packedTexture.getPackedWidth());
		dosBinary.writeShort((short) in_packedTexture.getPackedHeight());
		
		System.out.println("Output Image size::" + in_packedTexture.getPackedWidth() + " x " + in_packedTexture.getPackedHeight());
		
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
			
			if (verboseOutput)
			{
				System.out.println("Writing pixels from:" + in_packedTexture.getOriginalFile(i).getName());
				System.out.println("Image:" + i + " position:" + ox + "," + oy + ":" + width + "," + height);
			}

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
	
	private void writeStringIDs(PackedTexture in_packedTexture) throws IOException 
	{
		int numImages = in_packedTexture.getNumImages();
		
		FileOutputStream outC = new FileOutputStream(outputName + ".csatlasid");
		DataOutputStream dosC = new DataOutputStream(outC);

		for (int i = 0; i < numImages; i++)
		{
			String enumName = enumNameFromFile(in_packedTexture.getOriginalFile(i));
			dosC.writeBytes(enumName);
			dosC.writeByte('\n');
		}

		dosC.close();
	}
}
