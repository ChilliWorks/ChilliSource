/**
 * File: PNGHotspot.java
 * Date: 07 September 2006
 * Description: Generate a index .bin file for a given .PNG image for
 * 				use with an in-game font
 * Notes: Code adapted from source availabe at
 * 		  http://www.java2s.com/ExampleCode/2D-Graphics-GUI/PNGfileformatdecoder.htm
 */

/**
 *
 * @author  Robert Henning
 * @version 1.0
 * @copyright 2006 Tag Games Limited - All rights reserved
 */

package com.taggames.tools;

import java.io.*;
import java.awt.image.*;
import javax.imageio.*;
import java.util.*;
import javax.xml.parsers.*;
import javax.xml.transform.*;
import javax.xml.transform.dom.*;
import javax.xml.transform.stream.*;
import org.w3c.dom.*;
import java.security.MessageDigest;
import java.math.BigInteger;
 
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
  */
public class SpriteTool
{
	private static final String VERSION = "2.12";
	enum Mode
	{
		FONT, SPRITE,
	}

	public static void main(String[] args) throws Exception
	{
		SpriteTool tool = new SpriteTool();
		tool.Run(args);
	}

	public class Rectangle
	{
		public int x = 0;
		public int y = 0;
		public int width = 0;
		public int height = 0;
		
		public int left()
		{
			return x;
		}
		
		public int right()
		{
			return x + width;
		}
		
		public int top()
		{
			return y;
		}
		
		public int bottom()
		{
			return y + height;
		}
		
		public Rectangle(){}

		public Rectangle(int x, int y, int width, int height)
		{
			this.x = x;
			this.y = y;
			this.width = width;
			this.height = height;
		}
		
		public boolean intersects(Rectangle other)
		{
			return !((other.left() >= this.right()) ||
					 (other.right() <= this.left()) ||
					 (other.top() >= this.bottom()) ||
					 (other.bottom() <= this.top()));
		}
	}

	/**
		Simple class to implement a collision grid that uses spatial hashing
		to reduce collision checks
		Defaults to 64x64 grid sizes
	*/
	public class CollisionGrid
	{
		CollisionGrid(int inGridSize)
		{
			mGridSize = inGridSize;
		}

		public void AddRectangle(Rectangle inRect)
		{
			int xStart = inRect.left() / mGridSize;
			int xEnd = (inRect.right() + mGridSize-1)/mGridSize;

			int yStart = inRect.top() / mGridSize;
			int yEnd = (inRect.bottom() + mGridSize-1)/mGridSize;

			for(int y = yStart; y < yEnd; y++)
				for(int x = xStart; x < xEnd; x++)
				{
					int hash = y <<16 | x;
					ArrayList<Rectangle> list = null;
					if(mapGridSections.containsKey(hash ) == false)
					{
						list = new ArrayList<Rectangle>();
						mapGridSections.put(hash, list);
					}
					else
						list = mapGridSections.get(hash);

					list.add(inRect);
				}
		}

		public boolean GetIntersecting(Rectangle inRect, ArrayList<Rectangle> outCollisions)
		{
			int xStart = inRect.left() / mGridSize;
			int xEnd = (inRect.right() + mGridSize-1)/mGridSize;

			int yStart = inRect.top() / mGridSize;
			int yEnd = (inRect.bottom() + mGridSize-1)/mGridSize;

			for(int y = yStart; y < yEnd; y++)
				for(int x = xStart; x < xEnd; x++)
				{
					int hash = y <<16 | x;
					ArrayList<Rectangle> list = mapGridSections.get(hash);

					if(list == null)
						continue;

					for(Rectangle test : list)
						if(inRect.intersects(test))
							outCollisions.add(test);
				}

			return outCollisions.isEmpty() == false;
		} 

		public void Clear()
		{
			mapGridSections.clear();
		}

		HashMap<Integer, ArrayList<Rectangle> > mapGridSections = new HashMap<Integer, ArrayList<Rectangle> >() ;
		int mGridSize = 64;
	}
	
	/**
	Predicates for choosing best placement point for a sprite out of two possibles
	@param The first placement point
	@param The second placement point
	@returns True if first is better than second, false otherwise
	*/
	enum PlacementHeuristic
	{
		TOPLEFT {
			public boolean ChooseBest(Rectangle inA, Rectangle inB )
			{
				int aLen = inA.left() * inA.left() + inA.top() * inA.top();
				int bLen = inB.left() * inB.left() + inB.top() * inB.top();
				return aLen < bLen;
			}
		},
		BOTTOMRIGHT {
			public boolean ChooseBest(Rectangle inA, Rectangle inB )
			{
				int aLen = inA.right() * inA.right() + inA.bottom() * inA.bottom();
				int bLen = inB.right() * inB.right() + inB.bottom() * inB.bottom();
				return aLen < bLen;
			}
		};
		
		abstract boolean ChooseBest(Rectangle inA, Rectangle inB );
	}

	// Arranged sprites
	Rectangle placedSpriteRects[];
	int combinedImageHeight;
	int combinedImageWidth;
	
	boolean verboseOutput = false;
	int bin_version = 0;
	Mode mode = Mode.SPRITE;
	int numPixelsPadding = 0;

	Character CR = null;
	Character LF = null;
	StringBuffer fontAlphabet = null; // this will create the alphabet file in
	// FONT mode
	StringBuffer fontCSV = null; // debug csv file for comparing font output
	boolean 	 bEnableKerning = true;

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
	int innerPadding = 0;
	int extrude = 0;
	PlacementHeuristic eBestPlacement = PlacementHeuristic.BOTTOMRIGHT;

	File[] sourceImageFiles;
	BufferedImage[] imageFiles;
	int numSourceImages;
	int numPlacedImages=0;

	int originalImageHeights[];
	int originalImageWidths[];
	int croppedImageHeights[];
	int croppedImageWidths[];
	int croppedImageArea[];
	int imageOffsetXs[];
	int imageOffsetYs[];

	int sortedOrder[];
	String inputDirectoryName = null; // Directory relative to working directory
	// where (commandline -dir, -directory)
	File rootDirectory; // Root directory from which files are read,set from
	// the above
	String orderingFileName = null;
	String outputName = null; // This forms the root for any output files'
	// names. (outputName.h, outputName.png etc)

	int useGraphics[]; // Some indirections
	boolean matchFound[];
	int namePointers[];
	boolean cropImages = true;	// When true we crop transparencies in the image
	boolean useSimpleSpriteIds = false;
	
	HashSet<Rectangle> setSeenColliders = new HashSet<Rectangle>();
	CollisionGrid	   gridCheck = new CollisionGrid(64);
	
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
		if (useSimpleSpriteIds == false)
		{
			String srcFilePath = file.getCanonicalPath();
			String srcFilePathWithoutExtension = srcFilePath.substring(0, srcFilePath.lastIndexOf("."));
			String rootDir = rootDirectory.getCanonicalPath();
			int rootDirLength = rootDir.length();
			String srcFilePathFromRoot = srcFilePathWithoutExtension.substring(rootDirLength, srcFilePathWithoutExtension.length());
			
			// replace slashes with underscores and make uppercase
			String srcFilePathWithoutSlashes = srcFilePathFromRoot.replace('\\', '_').replace('/', '_'); 
			srcFilePathWithoutSlashes = srcFilePathWithoutSlashes.replaceAll("@2x", "");
			String strOutPutName = outputName.replaceAll("@2x", "");
			return "TPAGE_" + strOutPutName.toUpperCase() + "_" + srcFilePathWithoutSlashes.toUpperCase();
		}
		else
		{
			String srcFilePath = file.getCanonicalPath();
			String srcFilePathWithoutExtension = srcFilePath.substring(0, srcFilePath.lastIndexOf("."));
			String rootDir = rootDirectory.getCanonicalPath();
			int rootDirLength = rootDir.length();
			String srcFilePathFromRoot = srcFilePathWithoutExtension.substring(rootDirLength, srcFilePathWithoutExtension.length());
			
			// replace slashes with underscores and make uppercase
			String srcFilePathWithoutSlashes = srcFilePathFromRoot.replace('\\', '_').replace('/', '_'); 
			srcFilePathWithoutSlashes = srcFilePathWithoutSlashes.replaceAll("@2x", "");
			
			//if the first char is an underscore, remove it.
			if (srcFilePathWithoutSlashes.length() > 0 && srcFilePathWithoutSlashes.getBytes()[0] == '_')
			{
				srcFilePathWithoutSlashes = srcFilePathWithoutSlashes.substring(1);
			}
			return srcFilePathWithoutSlashes.toUpperCase();
		}
	}

	private boolean imagesAreEqual(int imageIndex2, int imageIndex1) 
	{
		int image1 = namePointers[imageIndex1];
		int image2 = namePointers[imageIndex2];

		File f1 = sourceImageFiles[image1];
		File f2 = sourceImageFiles[image2];
		BufferedImage I1 = imageFiles[image1];
		BufferedImage I2 = imageFiles[image2];

		if(I1 == I2)
		{
			System.out.println("Hash match found:" + f1.getName() + " and " + f2.getName());
			return true;
		}
			

		int w = croppedImageWidths[imageIndex2];
		int h = croppedImageHeights[imageIndex2];
		int xoff1 = imageOffsetXs[imageIndex1];
		int yoff1 = imageOffsetYs[imageIndex1];
		int xoff2 = imageOffsetXs[imageIndex2];
		int yoff2 = imageOffsetYs[imageIndex2];
		int y = 0;

		while (y < h)
		{
			int x = 0;
			while (x < w)
			{
				if (!(I1.getRGB(xoff1 + x, yoff1 + y) == I2.getRGB(xoff2 + x,yoff2 + y)))
					return false;
				x++;
			}
			y++;
		}

		System.out.println("Match found:" + f1.getName() + " and " + f2.getName());

		return true;
	}

	boolean imagePriorityHeuristic(int indwLeft, int indwRight)
	{
		int LeftSide = croppedImageHeights[indwLeft] > croppedImageWidths[indwLeft] ? croppedImageHeights[indwLeft] : croppedImageWidths[indwLeft];
		int RightSide = croppedImageHeights[indwRight] > croppedImageWidths[indwRight] ? croppedImageHeights[indwRight] : croppedImageWidths[indwRight];
		return (croppedImageArea[indwLeft] + LeftSide * LeftSide) < (croppedImageArea[indwRight] + RightSide * RightSide);
	}
	
	int guessOutputHeight()
	{
		if(minAreaHeight == -1)
		{
			//Super simple heuristic, can't be shorter than our tallest image
			int maxHeight = 0;
	
			for(int y : croppedImageHeights)
			{
				maxHeight = (y > maxHeight) ? y : maxHeight;
			}
	
			return maxHeight;
		}
		
		return minAreaHeight;
	}

	int guessOutputWidth()
	{
		if(minAreaWidth == -1)
		{
			ArrayList<Integer> widths = new ArrayList<Integer>();
			
			for(int w: croppedImageWidths)
			{
				widths.add(new Integer(w + 2 * numPixelsPadding));
			}
			
			Collections.sort(widths);
			
	        // Extract the maximum and median widths.
	        int maxWidth = widths.get(widths.size() - 1);
	        int medianWidth = widths.get(widths.size() / 2);
	
	        // Heuristic assumes an NxN grid of median sized sprites.
	        int width = medianWidth * (int)Math.round(Math.sqrt(widths.size()));
	
	        // Make sure we never choose anything smaller than our largest sprite.
	        return Math.max(width, maxWidth);
		}
		
		return minAreaWidth;
	}

	int nextAllowedHeight(int h)
	{
		// Fixed height always return same size
		if (fixedHeight != -1)
		{
			return fixedHeight;
		}
		// Bounded only by max height
		else if (maxHeight != -1)
		{
			if(h < minAreaHeight)
				return minAreaHeight;
			else
			{
				int nextHeight = h;
				if(nextHeight > maxHeight)
					nextHeight = maxHeight;
				if(divisibleBy != -1)
				{
					if((nextHeight % divisibleBy) != 0)
						nextHeight = (((nextHeight / divisibleBy) + 1) * divisibleBy);
				}
				return nextHeight;
			}
		}
		// Bounded by a list of valid sizes
		else
		{
			int n = 0;
			while (validHeights[n] <= h && n < numValidH)
				n++;
			
			return validHeights[n];
		}
	}

	int nextAllowedWidth(int w)
	{
		// Fixed widths always return same size
		if (fixedWidth != -1)
		{
			return fixedWidth;
		}
		// Bounded only by max widths
		else if (maxWidth != -1)
		{
			if(w < minAreaWidth)
				return minAreaWidth;
			else
			{
				int nextWidth = w ;
				if(nextWidth > maxWidth)
					nextWidth = maxWidth;
				if(divisibleBy != -1)
				{
					if((nextWidth % divisibleBy) != 0)
						nextWidth = (((nextWidth / divisibleBy) + 1) * divisibleBy);
				}
				return nextWidth;
			}
		}
		// Bounded by a list of valid sizes
		else
		{
			int n = 0;
			while (validWidths[n] <= w && n < numValidW)
				n++;
			
			return validWidths[n];
		}
	}
	
	
	
	/**
	Checks for intersections with other placed sprites
	@param Sprite to check for intersections with
	@return The intersecting sprite or null if there was no intersection
	*/
	public Rectangle findIntersectingSprite(Rectangle inSprite)
	{
		ArrayList<Rectangle> Intersecting = new ArrayList<Rectangle>();

		// Use our collision grid to get collisions
		gridCheck.GetIntersecting(inSprite, Intersecting);

		// We might hit more than one placed sprite
		// so we should check for the best one to use (which is usually the most topleft)
		// as the basis for moving our new sprite
		Rectangle bestHit = null;
		for(Rectangle hit: Intersecting)
		{
			if(bestHit == null)
				bestHit = hit;
			else
			{
				if(PlacementHeuristic.TOPLEFT.ChooseBest(hit, bestHit))
					bestHit = hit;
			}
		}
		
		return bestHit;
	}
	
	/**
	Checks if a sprite fits within specified bounds
	@param Sprite to check against bounds
	@param Bounding width
	@param Bounding height
	@returns If sprite fits
	*/
	public boolean spriteFitsArea(Rectangle inSprite, int inWidth, int inHeight)
	{
		return (inSprite.x >=0 && inSprite.y >=0) && (inSprite.bottom() <= inHeight && inSprite.right() <= inWidth);
	}
	
	/**
	Checks if a sprite will fit into our spritesheet if the spritesheet can be resized
	@param Sprite to check 
	@returns If sprite fits (with resizing)
	*/
	public boolean spriteFitsWithResize(Rectangle inSprite)
	{
		int newWidth = nextAllowedWidth(inSprite.right());
		int newHeight = nextAllowedHeight(inSprite.bottom());
		
		boolean bFits = spriteFitsArea(inSprite, newWidth, newHeight);
		
		return bFits;
	}
	
	/**
	Resizes our sprite sheet to fit a passed in sprite
	@param The sprite to fit on to our sprite sheet
	@returns If it was possible to resize to fit the sprite
	*/
	public boolean resizeToFitSprite(Rectangle inSprite)
	{
		if(spriteFitsArea(inSprite, combinedImageWidth, combinedImageHeight))
			return true;
		if(spriteFitsWithResize(inSprite) == false)
			return false;
		
		int newWidth = nextAllowedWidth(inSprite.right());
		int newHeight = nextAllowedHeight(inSprite.bottom());
		
		if(combinedImageWidth < newWidth)
			combinedImageWidth = newWidth;
		if(combinedImageHeight < newHeight)
			combinedImageHeight = newHeight;
		
		System.out.println("Trying next image size of "+combinedImageWidth+"x"+combinedImageHeight);
		
		return true;
	}
	
	/**
	Attempts to place a sprite on our spritesheet, expanding when necessary
	@param The sprite to fit
	@returns The position of the fitted sprite or null if it was not possible to fit
	*/
	public Rectangle findSpaceForSprite(Rectangle inSprite)
	{
		if(!spriteFitsWithResize(inSprite))
			return null;
		
		Rectangle result = null;
		Rectangle collider = findIntersectingSprite(inSprite);
						
		// No collisions so see if it fits on the image
		if(collider == null)
		{
			result = inSprite;
		}
		else
		{
			// We dont want to go over a collider we've seen before
			if(setSeenColliders.contains(collider))
				return null;
			
			setSeenColliders.add(collider);
			
			// check down

			// Try to bump left our rect to be more compact
			int newX = inSprite.x;
			if(inSprite.x > collider.x)
				newX = collider.x;
			
			Rectangle down = new Rectangle(newX, collider.bottom(), inSprite.width, inSprite.height);
			down= findSpaceForSprite(down);
			
			// check right 
			
			// Try to bump up our rect to be more compact
			int newY =inSprite.y;
			if(inSprite.y > collider.y)
				newY = collider.y;
			
			Rectangle right = new Rectangle(collider.right(), newY, inSprite.width, inSprite.height);
			right = findSpaceForSprite(right);
			
			if(right!= null && down!= null)
			{
				// Prefer one that doesn't need resizing
				boolean bDownResize = spriteFitsArea(down, combinedImageWidth, combinedImageHeight) == false;
				boolean bRightResize = spriteFitsArea(right, combinedImageWidth, combinedImageHeight) == false;
				
				// Either so further checks
				if(bDownResize == bRightResize)
				{
					// prefer the most top left one
					if(eBestPlacement.ChooseBest(right, down))
						result = right;
					else
						result = down;					
				}
				else if(bRightResize)
					result = down;
				else
					result = right;
				
			}
			else if(right != null)
				result = right;
			else
				result = down;

		}	
		return result;
	}
	
	/**
	Places a sprite down on the spritesheet
	@param The index of the sprite to place
	@param The list of original sprite indices
	@returns If it was possible to place the sprite on the sheet
	*/	
	public boolean placeSprite(int sortedIndex, int[] sortedOrder)
	{
		int imageIndex = sortedOrder[sortedIndex];
		
		Rectangle toBePlaced = new Rectangle(0,0,croppedImageWidths[imageIndex] + (numPixelsPadding * 2), croppedImageHeights[imageIndex] + (numPixelsPadding * 2));
		
		setSeenColliders.clear();
		Rectangle foundSpace = findSpaceForSprite(toBePlaced);
		if(foundSpace != null)
		{
			resizeToFitSprite(foundSpace);
			placedSpriteRects[imageIndex] = foundSpace;
			// Add the placed sprite to our collision grid
			gridCheck.AddRectangle(foundSpace);
			return true;
		}
		else
			return false;
	}

	private boolean TryToLayoutSprite(int inSpriteNo, int[] inSprites)
	{
		int[] sortedOrder = inSprites;
		
		int nOriginalIndex = sortedOrder[inSpriteNo];
		
		//Because of the way the sprites have been sorted we will always have placed the original graphics version of this
		//before we place the 
		if (useGraphics[nOriginalIndex] != nOriginalIndex)
		{
			placedSpriteRects[nOriginalIndex] = placedSpriteRects[useGraphics[nOriginalIndex]];
		}
		else
		{
			if(!placeSprite(inSpriteNo, sortedOrder))
				return false;
		}
		return true;
	}
	
	/**
	Layout all sprites
	@param List of sprite indices in best order to try placement
	@returns if it was possible to fit all sprites
	*/
	private boolean layoutSpritesForSheet(int[] sortedOrder)
	{
		placedSpriteRects = new Rectangle[sortedOrder.length];
		
		combinedImageHeight = nextAllowedHeight(guessOutputHeight());
		combinedImageWidth = nextAllowedWidth(guessOutputWidth());
		
		// Clear our collision grid
		gridCheck.Clear();

		for (int nSprite = 0; nSprite < numSourceImages; nSprite++)
		{
			if(TryToLayoutSprite(nSprite, sortedOrder) ==false)
			{
				int originalID = sortedOrder[nSprite];
				System.err.println("Could not fit sprite:" + sourceImageFiles[originalID].getName() );
				return false;
			}
			numPlacedImages++;

		}
				
		return true;
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

	/**
	GetFileHash
	Generates an md5 hash for any given file

	@param File to be hashed
	@return the md5 hash (as a hex string) of the file's contents
	*/
	private String GetFileHash(File inFile)
	{
		String strHash = null;
		InputStream fIn = null;
		byte[] buffer = new byte[1024];
		int numRead;
		try
		{
			MessageDigest md = MessageDigest.getInstance("MD5");
			fIn = new FileInputStream(inFile);

			do 
			{
	           	numRead = fIn.read(buffer);
	           	if (numRead > 0) {
	               md.update(buffer, 0, numRead);
	           	}
       		} while (numRead != -1);
       		fIn.close();

       		strHash = new BigInteger(1,md.digest()).toString(16);
		}
		catch (Exception e)
		{

		}

		return strHash;
	}

	private void loadImageFileInformation() throws IOException
	{
		imageFiles = new BufferedImage[numSourceImages];

		// hold all hashes of files we load
		HashMap<String, Integer> mapUniques = new HashMap<String, Integer>();

		for (int i = 0; i < numSourceImages; i++)
		{
			useGraphics[i] = i;
			matchFound[i] = false;
			{
				System.out.print("Scanning File:" + sourceImageFiles[i].getName());

				File f = sourceImageFiles[i];
				if (!f.exists())
				{
					System.out.println("<<<<<<<< does not exist in chosen directory");
					continue;
				}

				// Check if we've loaded an identical file 
				String fileHash = GetFileHash(f);
 
				Integer intMatch = mapUniques.get(fileHash);
				// We've already loaded a file with the same contents 
				if(intMatch != null)
				{	
					// We don't need to figure out its size				
					originalImageWidths[i] = originalImageWidths[intMatch];
					originalImageHeights[i] =originalImageHeights[intMatch];

					// We can also just use the previously loaded image file
					// to save memory
					imageFiles[i] = imageFiles[intMatch];
					croppedImageWidths[i] = croppedImageWidths[intMatch];
					croppedImageHeights[i] = croppedImageHeights[intMatch];
						
					imageOffsetXs[i] = imageOffsetXs[intMatch];
					imageOffsetYs[i] = imageOffsetYs[intMatch];
					croppedImageArea[i] = croppedImageArea[intMatch];

					// Still need to keep the original name though
					namePointers[i] = i;

					// Move on to the next image
					continue;
				}
				else // Store for future checks
					mapUniques.put(fileHash, i);

				BufferedImage I = ImageIO.read(f);
				int width = I.getWidth();
				int height = I.getHeight();
				System.out.print(" width:" + width + " height:" + height);
				originalImageWidths[i] = width;
				originalImageHeights[i] = height;

				int maxx = 0, minx = width - 1;
				int maxy = 0, miny = height - 1;
				boolean blankImage = true;
				
				if (cropImages)
				{
					// Crop completely transparent pixels
					for (int x = 0; x < width; x++)
					{
						for (int y = 0; y < height; y++)
						{
							int a = I.getRGB(x, y);
	
							if ((a & 0xFF000000) != 0)
							{
								blankImage = false;
								if (x < minx)
									minx = x;
								if (x > maxx)
									maxx = x;
								if (y < miny)
									miny = y;
								if (y > maxy)
									maxy = y;
							}
						}
					}
				}
				else
				{
					outerloop:
					for (int x = 0; x < width; x++)
					{
						for (int y = 0; y < height; y++)
						{
							int a = I.getRGB(x, y);

							if ((a & 0xFF000000) != 0)
							{
								blankImage = false;
								break outerloop;
							}
						}
					}
					
					minx= 0;
					maxx = width - 1;
					miny = 0;
					maxy = height - 1;
				}
				
				if (blankImage)
				{
					// force output of whole image.
					minx = 0;
					maxx = width;
					miny = 0;
					maxy = height;
				}

				imageFiles[i] = I;
				namePointers[i] = i;
				// Try to retain innerPadding
				minx = Math.max(0, minx - innerPadding);
				miny = Math.max(0, miny - innerPadding);
				maxx = Math.min(width - 1, maxx + innerPadding);
				maxy = Math.min(height - 1, maxy + innerPadding);

				croppedImageWidths[i] = ((maxx - minx) + 1); // inclusive box
				croppedImageHeights[i] = ((maxy - miny) + 1); // inclusive box

				if (croppedImageWidths[i] > width)
					croppedImageWidths[i] = width;
				if (croppedImageHeights[i] > height)
					croppedImageHeights[i] = height;
				imageOffsetXs[i] = minx;
				imageOffsetYs[i] = miny;
				croppedImageArea[i] = croppedImageWidths[i] * croppedImageHeights[i];
				System.out.println(" clipped area:" + minx + "," + maxx + ":"
						+ miny + "," + maxy);
			}
		}
	}

	private void processCommandLineArgs(String[] args) throws NumberFormatException, Exception
	{
		Hashtable<String, String> options = new Hashtable<String, String>();

		for (int i = 0; i < args.length; i++)
		{
			if (args[i].equalsIgnoreCase("-verbose"))
			{
				verboseOutput = true;
			}
			else
			if (args[i].equalsIgnoreCase("-help"))
			{
				Usage();
			}
			else
			{
				options.put(args[i], args[i + 1]);
				i++;
			}
		}

		if (options.containsKey("-crop"))
		{
			String crop = options.get("-crop");
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
		
		if (options.containsKey("-mode"))
		{
			String newMode = options.get("-mode");
			int tmp = Integer.parseInt(newMode);

			if (tmp == 0)
			{
				mode = Mode.SPRITE;
				System.out.println("set Mode to SPRITE");
			}
			else
			if (tmp == 1)
			{
				mode = Mode.FONT;
				System.out.println("set Mode to FONT");
			}
			else
			{
				System.out.println("Error: Unsupported mode " + newMode + ". Please use mode 0 (SPRITE) or mode 1 (FONT).");
				return;
			}
		}

		if (options.containsKey("-dir"))
		{
			inputDirectoryName = options.get("-dir");
		}
		else
		if (options.containsKey("-directory"))
		{
			inputDirectoryName = options.get("-directory");
		}
		else
		{
			throw new Exception("Error: no source directory!");
		}

		if (options.containsKey("-output"))
		{
			outputName = options.get("-output");
			if (outputName != null)
			{
				System.out.println("outputFile:" + outputName);
			}
			else
			{
				outputName = "Default";
			}
		}
		else
		{
			outputName = "Default";
		}

		if (options.containsKey("-padding"))
		{
			String pad = options.get("-padding");
			numPixelsPadding = Integer.parseInt(pad);
			if (verboseOutput)
			{
				System.out.println("Padding set to:" + numPixelsPadding);
			}
		}

		if (options.containsKey("-filelist"))
		{
			orderingFileName = options.get("-filelist");
		}

		if (options.containsKey("-width"))
		{
			fixedWidth = Integer.decode(options.get("-width"));
		}

		if (options.containsKey("-height"))
		{
			fixedHeight = Integer.decode(options.get("-height"));
		}

		if (options.containsKey("-validwidths"))
		{
			String s = options.get("-validwidths");
			numValidW = 0;
			int index = s.indexOf(':');
			while (index > 0)
			{
				String p = s.substring(0, s.indexOf(':'));
				s = s.substring(s.indexOf(':') + 1, s.length());
				int w = Integer.decode(p);
				validWidths[numValidW] = w;
				numValidW++;
				index = s.indexOf(':');
			}
			int w = Integer.decode(s);
			validWidths[numValidW] = w;
			numValidW++;
		}

		if (options.containsKey("-validheights"))
		{
			String s = options.get("-validheights");
			numValidH = 0;
			int index = s.indexOf(':');
			while (index > 0)
			{
				String p = s.substring(0, s.indexOf(':'));
				s = s.substring(s.indexOf(':') + 1, s.length());
				int h = Integer.decode(p);
				validHeights[numValidH] = h;
				numValidH++;
				index = s.indexOf(':');
			}
			int h = Integer.decode(s);
			validHeights[numValidH] = h;
			numValidH++;
		}
		
		if (options.containsKey("-maxwidth"))
		{
			maxWidth = Integer.decode(options.get("-maxwidth"));
		}
		
		if (options.containsKey("-maxheight"))
		{
			maxHeight = Integer.decode(options.get("-maxheight"));
		}
		
		if (options.containsKey("-divisibleby"))
		{
			divisibleBy = Integer.decode(options.get("-divisibleby"));
		}

		if (options.containsKey("-binversion"))
		{
			bin_version = Integer.decode(options.get("-binversion"));
		}
		if(options.containsKey("-heuristic"))
		{
			try
			{
				PlacementHeuristic eChoice = PlacementHeuristic.valueOf(options.get("-heuristic").toUpperCase());
				eBestPlacement = eChoice;
			}
			catch(Exception e)
			{
				System.out.println(options.get("-heuristic").toUpperCase() + " is not a valid heuristic");
			}
		}
		
		if(options.containsKey("-innerPadding"))
		{
			innerPadding = Integer.decode(options.get("-innerPadding"));
		}
		
		if(options.containsKey("-simplespriteids"))
		{
			useSimpleSpriteIds = Boolean.parseBoolean(options.get("-simplespriteids"));
		}
		
		if(options.containsKey("-extrude"))
		{
			extrude = Integer.decode(options.get("-extrude"));
		}

		if(options.containsKey("-kerning"))
		{
			bEnableKerning = Boolean.parseBoolean(options.get("-kerning"));
		}
	}

	public void Usage()
	{
		System.out.println("Help:");
		System.out.println("Example SpriteSheet creation:");
		System.out.println("java -jar SpriteTool.jar -mode 0 -padding PADDING -dir INPUT_DIR -validwidths VALID_WIDTH_LIST -validheight VALID_HEIGHT_LIST -output OUTPUT_DIR");
		System.out.println("\tCommands:");
		System.out.println("\t\t-mode <mode> \t\t\t<Choose from SPRITE mode (0) or FONT mode (1)");
		System.out.println("\t\t-binversion value \t\t<Choose binray format revision (0=default 1=include original image sizes 2=also include spritesheet size)>");
		System.out.println("\t\t-padding value \t\t\t<Spacing (in pixels) between items on spritesheet>");
		System.out.println("\t\t-dir[ectory] directory \t\t<Directory to process. By default will process all contained PNGs");
		System.out.println("\t\t-output filename \t\t<Output PNG filename>");
		System.out.println("\t\t-filelist listfile \t\t<listfile contains list of .PNG files to include>");
		System.out.println("\t\t-width value \t\t\t<Force output width to be value>");
		System.out.println("\t\t-height value \t\t\t<Force output height to be value>");
		System.out.println("\t\t-validwidths v1:v2:v3:... \t<Choose best match final width from list>");
		System.out.println("\t\t-validheights v1:v2:v3:... \t<Choose best match final height from list>");
		System.out.println("\t\t-maxwidth value \t\t\t<Set a max width value, final image may be <= this value>");
		System.out.println("\t\t-maxheight value \t\t\t<Set a max height value, final image may be <= this value>");
		System.out.println("\t\t[Optional] -divisibleby value \t\t\t<Dimensions of image must be divisible by this value>");
		System.out.println("\t\t-verbose \t\t\t<Additional processing information>");
		System.out.println("\t\t-crop \t\t\tChoose from DO CROP (1) or DO NOT CROP (0). Default is DO CLIP.");
		System.out.println("\t\t-heuristic \t\t\tPacking heuristic (current choice of TOPLEFT or BOTTOMRIGHT)");
		System.out.println("\t\t-innerPadding value \t\t\tOptional amount of pixels to leave untrimmed/cropped from the sprite");
		System.out.println("\t\t-simplespriteids [true/false] \t\t\tWhether or not simple sprite id's should be used. Default is false.");
		System.out.println("\t\t-extrude [true/false] \t\t\tExtrudes the border of the image. Default is 0.");
		System.out.println("\t\t-kerning [true/false] \t\t\tEnable/Disable the generation of kerning data");
	}
	
	public boolean Run(String[] args) throws Exception
	{
		System.out.println("SpriteTool version "+VERSION);
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
		
		//as extruding, "extrudes" into the padding, we need to make sure we have at least enough padding for extruding.
		if (extrude > 0 && numPixelsPadding < extrude)
		{
			numPixelsPadding = extrude;
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

		numSourceImages = filesToProcess.size();
		System.out.println(numSourceImages + " frames to be processed.");

		originalImageWidths = new int[numSourceImages];
		originalImageHeights = new int[numSourceImages];
		croppedImageWidths = new int[numSourceImages];
		croppedImageHeights = new int[numSourceImages];
		croppedImageArea = new int[numSourceImages];
		imageOffsetXs = new int[numSourceImages];
		imageOffsetYs = new int[numSourceImages];
		namePointers = new int[numSourceImages];
		useGraphics = new int[numSourceImages];
		matchFound = new boolean[numSourceImages];

		sourceImageFiles = filesToProcess.toArray(new File[1]);

		// find the width, height and offsets for each image
		loadImageFileInformation();

		System.out.println("Processing...");
		sortedOrder = new int[numSourceImages];
		for (int i = 0; i < numSourceImages; i++)
		{
			sortedOrder[i] = i;
		}
		
		// Bubble sort! For N^2 fun. 
		for (int i = 0; i < numSourceImages; i++)
		{
			for (int j = i + 1; j < numSourceImages; j++)
			{
				if (imagePriorityHeuristic(sortedOrder[i], sortedOrder[j]))
				{
					int tempSwap = sortedOrder[j];
					sortedOrder[j] = sortedOrder[i];
					sortedOrder[i] = tempSwap;
				}
			}
		}
		
		// Check if any of our loaded images are duplicates of each other.
		for(int i = 0; i < numSourceImages; i++)
		{
			for(int j = i + 1; j < numSourceImages; j++)
			{
				int cur = sortedOrder[i];
				int comp = sortedOrder[j];
				
				if (croppedImageWidths[cur] == croppedImageWidths[comp] && croppedImageHeights[cur] == croppedImageHeights[comp])
				{
					boolean match = imagesAreEqual(cur, comp);
					if((match) && (!matchFound[comp]))
					{
						useGraphics[comp] = cur;
						matchFound[comp] = true;
					}
				}
			}
		}
		
		if((maxWidth != -1) && (maxHeight != -1))
		{
			int maxWidth = -1;
			int maxHeight = -1;
			for(int i = 0; i < numSourceImages; i++)
			{
				// TK: Determine the maximum width and height 
				if (croppedImageWidths[i] > maxWidth)
					maxWidth = croppedImageWidths[i];
				if (croppedImageHeights[i] > maxHeight)
					maxHeight = croppedImageHeights[i];
			}
			minAreaWidth = maxWidth;
			minAreaHeight = maxHeight;
			
			System.out.println("Minimum area required is "+minAreaWidth+"x"+minAreaHeight);
		}
		else
		{
			maxWidth = -1;
			maxHeight = -1;
		}

		if (!layoutSpritesForSheet(sortedOrder))
		{
			System.err.println("Failed to create sprite sheet:" + outputName);
			System.err.println("Images will not fit in any of the allowed output sizes.");
			return false;
		}

		// Ok re-sort the output so that the files are processed in
		// the order they came in...

		int outputOrder[] = new int[numSourceImages];
		for (int i = 0; i < numSourceImages; i++)
		{
			outputOrder[i] = i;
		}

		///////////////////////////////////////////////////////
		// It's output time!
		///////////////////////////////////////////////////////

		if (verboseOutput)
		{
			for (int i = 0; i < numSourceImages; i++)
			{
				System.out.println("Image:" + i + " Box:" + useGraphics[i]
				                                                         + " position:" + placedSpriteRects[i].x + "," + placedSpriteRects[i].y + ":"
				                                                         + croppedImageWidths[i] + ","
				                                                         + croppedImageHeights[i]);
			}
		}
		System.out.println("Output Image size::" + combinedImageWidth + " x " + combinedImageHeight);

		BufferedImage outputImage = buildCombinedImage(outputOrder);

		//if we have reason to, then add extruded pixels to the output image.
		if (extrude > 0)
		{
			extrudeCombinedImage(outputOrder, outputImage);
		}
		
		if (verboseOutput)
			System.out.println("writing output files....");

		// Write our combined png file
		File F = new File(outputName + ".png");
		ImageIO.write(outputImage, "png", F);

		// Always
		writeBinaryFile(outputOrder);

		// Write mode appropriate files
		if (mode == Mode.FONT)
		{
			buildAlphabet();
			if(bEnableKerning)
			{
				writeKerningInfo(buildKerningInfo());
			}
			writeAlphabetFile();
		}
		else
		{
			writeXML(outputOrder);
			writeStringIDs();
		}

		System.out.printf("Goodbye!\n");

		return true;
	}

	private BufferedImage buildCombinedImage(int[] outputOrder) throws IOException
	{
		BufferedImage combinedImage = new BufferedImage(combinedImageWidth, combinedImageHeight, BufferedImage.TYPE_INT_ARGB);
		for (int y = 0; y < combinedImageHeight; y++)
		{
			for (int x = 0; x < combinedImageWidth; x++)
			{
				combinedImage.setRGB(x, y, 0);
			}
		}

		for (int i = 0; i < numSourceImages; i++)
		{
			BufferedImage I = imageFiles[i];
			int ix = imageOffsetXs[i];
			int iy = imageOffsetYs[i];

			int ox = placedSpriteRects[outputOrder[i]].x;
			int oy = placedSpriteRects[outputOrder[i]].y;
			int width = croppedImageWidths[i];
			int height = croppedImageHeights[i];
			int iw = originalImageWidths[i];
			int ih = originalImageHeights[i];

//			if (verboseOutput)
				System.out.println("Writing pixels from:"+sourceImageFiles[namePointers[i]].getName()+" at x:"+ox+", y:"+oy+", width:"+width+", height:"+height);

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					int pixel = 0;

					if ((ix + x) < iw && (iy + y) < ih)
					{
						pixel = I.getRGB(ix + x, iy + y);
					}
					else
					{
						if (verboseOutput)
							System.out.println("input out of range:" + (ix + x) + "," + (iy + y));
					}

					if ((ox + x) < combinedImageWidth && (oy + y) < combinedImageHeight)
					{
						combinedImage.setRGB(ox + x + numPixelsPadding, oy + y + numPixelsPadding, pixel);
					}
					else
					{
						if (verboseOutput)
							System.out.println("Pixel out of range:" + (ox + x) + "," + (oy + y));
					}
				}
			}
		}
		
		return combinedImage;
	}
	
	private void extrudeCombinedImage(int[] outputOrder, BufferedImage combinedImage) throws IOException
	{
		for (int i = 0; i < numSourceImages; i++)
		{
			BufferedImage I = imageFiles[i];
			int ix = imageOffsetXs[i];
			int iy = imageOffsetYs[i];
			int ox = placedSpriteRects[outputOrder[i]].x;
			int oy = placedSpriteRects[outputOrder[i]].y;
			int width = croppedImageWidths[i];
			int height = croppedImageHeights[i];
			System.out.println("Extruding pixels from:"+sourceImageFiles[namePointers[i]].getName()+" at x:"+ox+", y:"+oy+", width:"+width+", height:"+height);

			//For each of the sides, iterate along it and add the extruded pixel colours. The existing padding should mean that the pixels are free to be coloured as we wish.
			//top
			for (int j = 0; j < width; ++j)
			{
				int x = j;
				int y = 0;
				int pixelColour = I.getRGB(ix + x, iy + y);
				for (int k = 0; k < extrude; ++k)
				{
					int extrudeDistance = -(k + 1);
					combinedImage.setRGB(ox + x + numPixelsPadding, oy + y + numPixelsPadding + extrudeDistance, pixelColour);
				}
			}
			
			//bottom
			for (int j = 0; j < width; ++j)
			{
				int x = j;
				int y = height - 1;
				int pixelColour = I.getRGB(ix + x, iy + y);
				for (int k = 0; k < extrude; ++k)
				{
					int extrudeDistance = (k + 1);
					combinedImage.setRGB(ox + x + numPixelsPadding, oy + y + numPixelsPadding + extrudeDistance, pixelColour);
				}
			}
			
			//left
			for (int j = 0; j < height; ++j)
			{
				int x = 0;
				int y = j;
				int pixelColour = I.getRGB(ix + x, iy + y);
				for (int k = 0; k < extrude; ++k)
				{
					int extrudeDistance = -(k + 1);
					combinedImage.setRGB(ox + x + numPixelsPadding + extrudeDistance, oy + y + numPixelsPadding, pixelColour);
				}
			}
			
			//right
			for (int j = 0; j < height; ++j)
			{
				int x = width - 1;
				int y = j;
				int pixelColour = I.getRGB(ix + x, iy + y);
				for (int k = 0; k < extrude; ++k)
				{
					int extrudeDistance = (k + 1);
					combinedImage.setRGB(ox + x + numPixelsPadding + extrudeDistance, oy + y + numPixelsPadding, pixelColour);
				}
			}
			
			//Now handle the corners
			//top left
			for (int j = 0; j < extrude; ++j)
			{
				for (int k = 0; k < extrude; ++k)
				{
					int pixelColour = I.getRGB(ix + 0, iy + 0);
					int x = -(j + 1);
					int y = -(k + 1);
					combinedImage.setRGB(ox + x + numPixelsPadding, oy + y + numPixelsPadding, pixelColour);
				}
			}
			
			//top right
			for (int j = 0; j < extrude; ++j)
			{
				for (int k = 0; k < extrude; ++k)
				{
					int pixelColour = I.getRGB(ix + width - 1, iy + 0);
					int x = width + j;
					int y = -(k + 1);
					combinedImage.setRGB(ox + x + numPixelsPadding, oy + y + numPixelsPadding, pixelColour);
				}
			}
			
			//bottom left
			for (int j = 0; j < extrude; ++j)
			{
				for (int k = 0; k < extrude; ++k)
				{
					int pixelColour = I.getRGB(ix + 0, iy + height - 1);
					int x = -(j + 1);
					int y = height + k;
					combinedImage.setRGB(ox + x + numPixelsPadding, oy + y + numPixelsPadding, pixelColour);
				}
			}
			
			//bottom right
			for (int j = 0; j < extrude; ++j)
			{
				for (int k = 0; k < extrude; ++k)
				{
					int pixelColour = I.getRGB(ix + width - 1, iy + height - 1);
					int x = width + j;
					int y = height + k;
					combinedImage.setRGB(ox + x + numPixelsPadding, oy + y + numPixelsPadding, pixelColour);
				}
			}
		}
	}

	private void buildAlphabet()
	{
		for (int character = 0; character < numSourceImages; character++)
		{
			// This is a font so check the filename for unicode
			// value of character
			// and write into alphabet file.

			int index = character;

			// check for valid unicode
			String charFilename = sourceImageFiles[namePointers[index]].getName();

			if ((charFilename.length() == 8) && (charFilename.indexOf('.') == 4)) // eg 0123.png
			{
				String unicodeString = charFilename.substring(0, 4);
				char unicode = (char) Integer.parseInt(unicodeString, 16);
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
					else
					{
						if (unicode == '"')
						{
							// fontCSV.append("\"" + unicodeString + "\",\"" + '�' + "\"," + LF);
						}
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

	private void writeAlphabetFile() throws FileNotFoundException, IOException
	{
		BufferedWriter out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(outputName+ ".alphabet"),"UTF-8"));
		out.write(fontAlphabet.toString());
		out.close();

	}

	private class KerningInfo
	{
		public char C1;
		public char C2;
		public short Offset;
		public KerningInfo(char c1, char c2, short offset)
		{
			C1 = c1;
			C2 = c2;
			Offset = offset;
		}
	}

	private  ArrayList<KerningInfo> buildKerningInfo() throws IOException
	{
		ArrayList<KerningInfo> result = new ArrayList<KerningInfo>();

		char[] chars = fontAlphabet.toString().toCharArray();
		int averageWidth = 0;
		for(int i = 0; i < chars.length; i++)
		{
			averageWidth += croppedImageWidths[i];
		}
		averageWidth /= chars.length;
		if(verboseOutput)
		{
			System.out.println("Average character width is:"+averageWidth);
		}

		//Test every character's image against every character's image to see how many pixels kerning we need.
		for (int nChar1 = 0; nChar1 < chars.length; nChar1++)
		{
			for (int nChar2 = 0; nChar2 < chars.length; nChar2++)
			{
				int allowedPenetration = calculateKerningForCharacterPair(nChar1, nChar2, averageWidth);

				if (verboseOutput)
				{
					System.out.println("Kerning for character pair:" + chars[nChar1] + " & " + chars[nChar2] + " = " + allowedPenetration);
				}
				
				//Don't care about 0 kerning, don't bother reporting it to save space
				if (allowedPenetration != 0)
					result.add(new KerningInfo(chars[nChar1],chars[nChar2],(short)allowedPenetration));
			}
		}

		return result;
	}

	private int calculateKerningForCharacterPair(int nChar1, int nChar2, int inAverageWidth)
	throws IOException
	{
		int allowedPenetration = 0;

		// First, check our characters will overlap
		int lowerY1 = imageOffsetYs[nChar1];
		int upperY1 = imageOffsetYs[nChar1] + croppedImageHeights[nChar1];
		int lowerY2 = imageOffsetYs[nChar2];
		int upperY2 = imageOffsetYs[nChar2] + croppedImageHeights[nChar2];
		
		if(!checkBounds(lowerY1, lowerY2, upperY2) && !checkBounds(upperY1, lowerY2, upperY2)
		 &&!checkBounds(lowerY2, lowerY1, upperY1) && !checkBounds(upperY2, lowerY1, upperY1))
		{
			if(verboseOutput)
			{
				char[] chars = fontAlphabet.toString().toCharArray();
				System.out.println("No Y overlapped with:" + chars[nChar1] + " & " + chars[nChar2]);
			}
			return allowedPenetration;
		}
		
		BufferedImage image1 = imageFiles[nChar1];
		BufferedImage image2 = imageFiles[nChar2];

		int offsetX1 = imageOffsetXs[nChar1];
		int offsetX2 = imageOffsetXs[nChar2];
		int croppedWidth1 = croppedImageWidths[nChar1];
		int croppedWidth2 = croppedImageWidths[nChar2];

		boolean noOverlap = true;

		int minHeight = Math.min(image1.getHeight(), image2.getHeight());
		int maxPenetration = Math.min(croppedWidth1, croppedWidth2);
		while (noOverlap && allowedPenetration < maxPenetration)
		{				
			allowedPenetration++;

			for (int nOverlap = 0; nOverlap < allowedPenetration  && noOverlap; nOverlap++)
			{
				int image1X = offsetX1 + croppedWidth1 - (allowedPenetration - nOverlap);
				int image2X = nOverlap + offsetX2;

				for (int nY = 0; nY < minHeight && noOverlap; nY++)
				{
					boolean image1Transparent = (image1.getRGB(image1X, nY) & 0xFF000000) == 0;
					boolean image2Transparent = (image2.getRGB(image2X, nY) & 0xFF000000) == 0;

					//As long as both or either pixels are transparent there is no overlap
					noOverlap &= (image1Transparent || image2Transparent);
				}
			}
		}

		allowedPenetration--; //the above loop finds the first penetration with any overlap
		
		// Now we want to check if we should add negative spacing, for example
		// and 'l' character could be a solid horizontal block in which case
		// it should be spaced further.
		if(0 == allowedPenetration)
		{
			// First check char 1
			int cropCheckX = (croppedWidth1 + offsetX1 - 1);
			int cropCheckStartY = imageOffsetYs[nChar1];
			int chopCheckEndY = (imageOffsetYs[nChar1] + croppedImageHeights[nChar1]);
			if(checkForNegativeKerning(nChar1, cropCheckX, cropCheckStartY, chopCheckEndY))
			{
				cropCheckX = (croppedWidth2 - 1);
				cropCheckStartY = imageOffsetYs[nChar2];
				chopCheckEndY = (imageOffsetYs[nChar2] + croppedImageHeights[nChar2]);
				if(checkForNegativeKerning(nChar2, cropCheckX, cropCheckStartY, chopCheckEndY))
				{
					allowedPenetration -= (inAverageWidth * 0.15);
					{
						char[] chars = fontAlphabet.toString().toCharArray();
						if(verboseOutput)
						{
							System.out.println("Increasing space between:" + chars[nChar1] + " & " + chars[nChar2]);
						}
					}
				}
			}
		}
		
		return allowedPenetration;
	}
	
	private boolean checkBounds(int inPoint, int inLower, int inUpper)
	{
		if(inPoint >= inLower && inPoint <= inUpper)
		{
			return true;
		}
		
		return false;
	}

	private boolean checkForNegativeKerning(int inChar, int inX, int inStartY, int inEndY)
	{
		BufferedImage image = imageFiles[inChar];
		int solidColourCount = inEndY - inStartY;
		
		for(int nY = inStartY; nY < inEndY; nY++)
		{
			if((image.getRGB(inX, nY) & 0xFF000000) == 0)
			{
				solidColourCount--;
			}				
		}
		
		if(solidColourCount >= croppedImageHeights[inChar] / 2)
		{
			return true;
		}
		
		return false;
	}
	
	private void writeKerningInfo(ArrayList<KerningInfo> infoList) throws IOException
	{
		FileOutputStream fileStream = new FileOutputStream(outputName + ".kerninginfo");
		DataOutputStream dataStream = new DataOutputStream(fileStream);

		dataStream.writeInt(infoList.size());
		for (KerningInfo info : infoList)
		{
			dataStream.writeChar(info.C1);
			dataStream.writeChar(info.C2);
			dataStream.writeShort(info.Offset);
		}

		dataStream.close();
	}

	private void writeBinaryFile(int[] outputOrder) throws FileNotFoundException, IOException
	{
		FileOutputStream outBinary = new FileOutputStream(outputName + ".bin");
		DataOutputStream dosBinary = new DataOutputStream(outBinary);
		dosBinary.writeShort(numSourceImages);
		dosBinary.writeShort(bin_version); // file format revision

		//Write out spritesheet size
		if(bin_version >=2)
		{
			dosBinary.writeShort(combinedImageWidth);
			dosBinary.writeShort(combinedImageHeight);
		}
		
		for (int i = 0; i < numSourceImages; i++)
		{
			if (verboseOutput)
				System.out.println("Writing pixels from:" + sourceImageFiles[namePointers[i]].getName());

			int ix = imageOffsetXs[i];
			int iy = imageOffsetYs[i];

			int ox = placedSpriteRects[outputOrder[i]].x + numPixelsPadding;
			int oy = placedSpriteRects[outputOrder[i]].y + numPixelsPadding;
			int width = croppedImageWidths[i];
			int height = croppedImageHeights[i];
			int orig_width = originalImageWidths[i];
			int orig_height = originalImageHeights[i];

			dosBinary.writeShort(ox);
			dosBinary.writeShort(oy);
			dosBinary.writeShort(width);
			dosBinary.writeShort(height);
			dosBinary.writeShort(ix);
			dosBinary.writeShort(iy);

			if (bin_version >= 1)
			{
				dosBinary.writeShort(orig_width);
				dosBinary.writeShort(orig_height);
			}
		}

		dosBinary.close();
	}
	
	private void writeStringIDs() throws IOException 
	{
		FileOutputStream outC = new FileOutputStream(outputName + ".mospriteid");
		DataOutputStream dosC = new DataOutputStream(outC);

		for (int i = 0; i < numSourceImages; i++)
		{
			String enumName = enumNameFromFile(sourceImageFiles[namePointers[i]]);
			dosC.writeBytes(enumName);
			dosC.writeByte('\n');
		}

		dosC.close();
	}

	private void writeXML(int[] outputOrder) throws IOException, Exception 
	{
		DocumentBuilderFactory documentBuilderFactory =DocumentBuilderFactory.newInstance();

		DocumentBuilder documentBuilder =documentBuilderFactory.newDocumentBuilder();

		Document document = documentBuilder.newDocument();

		Element rootElement = document.createElement("SpriteSheet");
		document.appendChild(rootElement);

		Element framesEl = document.createElement("Frames");

		HashMap<String,ArrayList<Integer>> h = new HashMap<String, ArrayList<Integer>>(); 

		for (int nSpriteFrame = 0; nSpriteFrame < numSourceImages; nSpriteFrame++)
		{
			//Work out what folder and thus animation we're in
			File f = sourceImageFiles[nSpriteFrame];
			String folderName = f.getParent();
			//If this frame comes from the root directory it isn't part of an animation
			if (folderName.equals(inputDirectoryName))
				folderName = null;

			if (folderName != null)
			{
				//trim root parent name;
				folderName = folderName.substring(Math.max(folderName.lastIndexOf('\\'),folderName.lastIndexOf('/')) + 1);
				if (!h.containsKey(folderName))
				{
					h.put(folderName, new ArrayList<Integer>());
				}

				//Add the index of this frame to our animations's frame indices
				h.get(folderName).add(Integer.valueOf(nSpriteFrame));
			}

			Element frameEl = document.createElement("Frame");

			int x = placedSpriteRects[outputOrder[nSpriteFrame]].x + numPixelsPadding;
			int y = placedSpriteRects[outputOrder[nSpriteFrame]].y + numPixelsPadding;
			int ox = imageOffsetXs[nSpriteFrame];
			int oy = imageOffsetYs[nSpriteFrame];
			int width = croppedImageWidths[nSpriteFrame];
			int height = croppedImageHeights[nSpriteFrame];

			String fileName = f.getName(); 
			String trimmedFileName = fileName.substring(0,fileName.lastIndexOf('.'));

			frameEl.setAttribute("src-image", trimmedFileName);
			frameEl.setAttribute("x", Integer.toString(x));
			frameEl.setAttribute("y", Integer.toString(y));
			frameEl.setAttribute("ox", Integer.toString(ox));
			frameEl.setAttribute("oy", Integer.toString(oy));
			frameEl.setAttribute("w", Integer.toString(width));
			frameEl.setAttribute("h", Integer.toString(height));
			framesEl.appendChild(frameEl);
		}
		rootElement.appendChild(framesEl);

		Element animsEl = document.createElement("Animations");

		for (String animName : h.keySet())
		{
			Element animEl = document.createElement("Animation");
			animEl.setAttribute("name", animName);
			String rawFrames = h.get(animName).toString();
			String formattedFrames = rawFrames.substring(1, rawFrames.length()-1).replace(',', ' ');
			animEl.setAttribute("frames", formattedFrames);

			animsEl.appendChild(animEl);
		}

		rootElement.appendChild(animsEl);

		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer = transformerFactory.newTransformer();
		transformer.setOutputProperty(OutputKeys.ENCODING, "ASCII");
		transformer.setOutputProperty(OutputKeys.INDENT, "yes");
		transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");
		DOMSource source = new DOMSource(document);
		StreamResult result =  new StreamResult(new File(outputName+".mosprites"));
		transformer.transform(source, result);
	}
}
