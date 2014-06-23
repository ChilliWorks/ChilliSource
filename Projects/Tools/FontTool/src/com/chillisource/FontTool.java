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

package com.chillisource;

import java.io.*;

import javax.imageio.*;

import java.util.*;

import com.chillisource.TexturePacker.PlacementHeuristic;
import com.chillisource.toolutils.ExternalProcess;
import com.chillisource.toolutils.LittleEndianOutputStream;
import com.chillisource.toolutils.FileUtils;
import com.chillisource.toolutils.StringUtils;
 
public class FontTool
{
	private static final short k_version = 1;

	public static void main(String[] args) throws Exception
	{
		FontTool tool = new FontTool();
		tool.Run(args);
	}

	boolean verboseOutput = false;
	int numPixelsPadding = 2;

	Character CR = null;
	Character LF = null;
	StringBuffer fontAlphabet = null; // this will create the alphabet file in
	StringBuffer fontCSV = null; // debug csv file for comparing font output

	// Output image sizes
	int fixedHeight = -1;
	int fixedWidth = -1;
	int validHeights[] = new int[256];
	int validWidths[] = new int[256];
	int numValidH = -1;
	int numValidW = -1;
	int maxWidth = -1;
	int maxHeight = -1;
	int minAreaWidth = -1;
	int minAreaHeight = -1;
	int divisibleBy = -1;
	short lineHeight = 0;
	PlacementHeuristic eBestPlacement = PlacementHeuristic.BOTTOMRIGHT;
	
	Hashtable<String, String> options = new Hashtable<String, String>();

	String inputDirectoryName = null; // Directory relative to working directory
	File rootDirectory; // Root directory from which files are read,set from
	String outputName = null; // This forms the root for any output files'

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
				PlacementHeuristic eChoice = PlacementHeuristic.valueOf(options.get("--heuristic").toUpperCase());
				eBestPlacement = eChoice;
			}
			catch(Exception e)
			{
				System.out.println(options.get("--heuristic").toUpperCase() + " is not a valid heuristic");
			}
		}
		
		if(options.containsKey("--lineHeight"))
		{
			lineHeight = Short.decode(options.get("--lineHeight"));
		}
	}
	
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

	public void Usage()
	{
		System.out.println("Help:");
		System.out.println("Example Font creation:");
		System.out.println("java -jar FontTool.jar -mode 0 -padding PADDING -dir INPUT_DIR -validwidths VALID_WIDTH_LIST -validheight VALID_HEIGHT_LIST -output OUTPUT_DIR");
		System.out.println("\tCommands:");
		System.out.println("\t\t-input \t\t<Directory to process. By default will process all contained PNGs");
		System.out.println("\t\t-output filename \t\t<Output font filename>");
		System.out.println("\t\t-width value \t\t\t<Force output width to be value>");
		System.out.println("\t\t-height value \t\t\t<Force output height to be value>");
		System.out.println("\t\t-validwidths v1:v2:v3:... \t<Choose best match final width from list>");
		System.out.println("\t\t-validheights v1:v2:v3:... \t<Choose best match final height from list>");
		System.out.println("\t\t-maxwidth value \t\t\t<Set a max width value, final image may be <= this value>");
		System.out.println("\t\t-maxheight value \t\t\t<Set a max height value, final image may be <= this value>");
		System.out.println("\t\t-divisibleby value \t\t\t<Dimensions of image must be divisible by this value>");
		System.out.println("\t\t-verbose \t\t\t<Additional processing information>");
		System.out.println("\t\t-heuristic \t\t\tPacking heuristic (current choice of TOPLEFT or BOTTOMRIGHT)");
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
	
	public boolean Run(String[] args) throws Exception
	{
		System.out.println("FontTool version " + new Short(k_version).toString());
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
		
		ArrayList<File> filesToProcess = new ArrayList<File>();

		rootDirectory = new File(inputDirectoryName);
		addImageFilesInDirectory(rootDirectory, true, filesToProcess);

		TexturePacker packer = new TexturePacker();
		packer
		.setDivisibleBy(divisibleBy)
		.setExtrusion(0)
		.setFixedHeight(fixedHeight)
		.setFixedWidth(fixedWidth)
		.setMaxHeight(maxHeight)
		.setMaxWidth(maxWidth)
		.setValidHeights(validHeights)
		.setValidWidths(validWidths)
		.setHeuristic(eBestPlacement)
		.setInnerPadding(0)
		.setOuterPadding(numPixelsPadding)
		.enableCropping(true)
		.enableVerboseOutput(verboseOutput);
		
		PackedTexture result = packer.pack(filesToProcess);
		if(result == null)
		{
			return false;
		}

		// Write our combined png file
		File F = new File(outputName + ".png");
		ImageIO.write(result.getPackedTexture(), "png", F);

		buildAlphabet(result);
		writeBinaryFile(result);
		
		convertToCSImage();

		System.out.printf("Goodbye!\n");

		return true;
	}
	
	private void convertToCSImage()
	{
		LinkedList<String> commands = new LinkedList<String>();
		commands.add("java");
		commands.add("-Djava.awt.headless=true");
		commands.add("-jar");
		commands.add("PNGToCSImage.jar");
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
				
				if(verboseOutput)
					System.out.println("Unicode for file \"" + charFilename + "\" is \"" + unicode + "\"");

				if (LF == null)
				{
					char tmp = 10;
					LF = new Character(tmp);
				}
				if (CR == null)
				{
					char tmp = 13;
					CR = new Character(tmp);
				}

				if (fontAlphabet != null)
				{
					fontAlphabet.append(unicode);

					// check for special characters
					if (unicode != '"')
					{
						fontCSV.append("\"" + unicodeString + "\",\"" + unicode + "\"," + LF);
					}
				}
				else
				{
					fontAlphabet = new StringBuffer(1);
					fontAlphabet.append(unicode);

					fontCSV = new StringBuffer(1);
					fontCSV.append("\"Uncode\",\"Character\"," + LF + "\"" + unicodeString + "\",\"" + unicode + "\"," + LF);
				}
			}
			else
			{
				if (verboseOutput)
					System.out.println("ERROR - unicode filename \"" + charFilename + "\" is not valid!");
			}
		}
	}

	private void writeBinaryFile(PackedTexture in_packedTexture) throws FileNotFoundException, IOException, Exception
	{
		int numImages = in_packedTexture.getNumImages();
		LittleEndianOutputStream dosBinary = new LittleEndianOutputStream(outputName + ".csfont");
		dosBinary.writeShort((short) numImages);
		dosBinary.writeShort(k_version); // file format revision

		//Write out spritesheet size
		dosBinary.writeShort((short) in_packedTexture.getPackedWidth());
		dosBinary.writeShort((short) in_packedTexture.getPackedHeight());
		dosBinary.writeShort(lineHeight);
		
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
		
		dosBinary.writeNullTerminatedUtf8String(fontAlphabet.toString());

		dosBinary.close();
	}
}
