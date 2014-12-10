/**
 *  TextureAtlasUtils.java
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

package com.chilliworks.chillisource.texturepackerutils;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;

import javax.imageio.ImageIO;

import com.chilliworks.chillisource.coreutils.Logging;

/**
 * Class that holds utility methos for packing images onto a single image to use the minimum
 * amount of space
 * 
 * @author R Henning
 */
public class TexturePacker 
{
	/**
	Predicates for choosing best placement point for a sprite out of two possibles
	@param The first placement point
	@param The second placement point
	@returns True if first is better than second, false otherwise
	 */
	public enum PlacementHeuristic
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

	int originalImageHeights[];
	int originalImageWidths[];
	int croppedImageHeights[];
	int croppedImageWidths[];
	int croppedImageArea[];
	int imageOffsetXs[];
	int imageOffsetYs[];
	int useGraphics[]; // Some indirections
	boolean matchFound[];
	BufferedImage[] imageFiles;
	int numPlacedImages=0;
	Rectangle placedSpriteRects[];
	int combinedImageHeight;
	int combinedImageWidth;
	int numSourceImages = 0;
	File[] sourceImageFiles;
	int namePointers[];

	HashSet<Rectangle> setSeenColliders = new HashSet<Rectangle>();
	CollisionGrid	   gridCheck = new CollisionGrid(64);
	
	int fixedHeight = -1;
	int fixedWidth = -1;
	int validHeights[];
	int validWidths[];
	int numValidH = -1;
	int numValidW = -1;
	int maxWidth = -1;
	int maxHeight = -1;
	int minAreaWidth = -1;
	int minAreaHeight = -1;
	int divisibleBy = -1;
	int numPixelsPadding = 0;
	int innerPadding = 0;
	int extrude = 0;
	boolean cropImages = true;	// When true we crop transparencies in the image
	PlacementHeuristic eBestPlacement = PlacementHeuristic.BOTTOMRIGHT;
	
	boolean m_ignoreFatalLogs = false;
	
	public TexturePacker setOuterPadding(int in_padding)
	{
		numPixelsPadding = in_padding;
		return this;
	}
	
	public TexturePacker setInnerPadding(int in_padding)
	{
		innerPadding = in_padding;
		return this;
	}
	
	public TexturePacker setExtrusion(int in_extrusion)
	{
		extrude = in_extrusion;
		return this;
	}
	
	public TexturePacker enableCropping(boolean in_enable)
	{
		cropImages = in_enable;
		return this;
	}
	
	public TexturePacker setHeuristic(PlacementHeuristic in_heuristic)
	{
		eBestPlacement = in_heuristic;
		return this;
	}
	
	public TexturePacker setMaxWidth(int in_maxWidth)
	{
		maxWidth = in_maxWidth;
		return this;
	}
	
	public TexturePacker setMaxHeight(int in_maxHeight)
	{
		maxHeight = in_maxHeight;
		return this;
	}
	
	public TexturePacker setFixedWidth(int in_width)
	{
		fixedWidth = in_width;
		return this;
	}
	
	public TexturePacker setFixedHeight(int in_height)
	{
		fixedHeight = in_height;
		return this;
	}
	
	public TexturePacker setValidWidths(int[] in_widths)
	{
		if(in_widths != null)
		{
			validWidths = in_widths;
			numValidW = in_widths.length;
		}
		return this;
	}
	
	public TexturePacker setValidHeights(int[] in_heights)
	{
		if(in_heights != null)
		{
			validHeights = in_heights;
			numValidH = in_heights.length;
		}
		return this;
	}
	
	public TexturePacker setDivisibleBy(int in_divBy)
	{
		divisibleBy = in_divBy;
		return this;
	}
	/**
	 * Set to ignore cases that would usually lead to a fatal assert when packing, used to test if atlasOptions will result in a valid
	 * Spritesheet 
	 * 
	 * @author HMcLaughlin
	 * @param in_ignore - Whether to ignore or not
	 * @return this instance
	 */
	public TexturePacker disableFatalLogs()
	{
		m_ignoreFatalLogs = true;
		return this;
	}
	
	/**
	 * Packs the given images into a single image
	 * 
	 * @author S Downie
	 * 
	 * @param List of files to pack
	 * @param out_errorInfo - Info object to be populated if an error occurs, can be null
	 * 
	 * @return Packed image data
	 * 
	 * @throws IOException 
	 */
	public PackedTexture pack(ArrayList<File> in_imageFiles, PackerInfo out_errorInfo) throws IOException
	{
		//as extruding, "extrudes" into the padding, we need to make sure we have at least enough padding for extruding.
		if (extrude > 0 && numPixelsPadding < extrude)
		{
			numPixelsPadding = extrude;
		}
		
		numSourceImages = in_imageFiles.size();

		originalImageWidths = new int[numSourceImages];
		originalImageHeights = new int[numSourceImages];
		croppedImageWidths = new int[numSourceImages];
		croppedImageHeights = new int[numSourceImages];
		croppedImageArea = new int[numSourceImages];
		imageOffsetXs = new int[numSourceImages];
		imageOffsetYs = new int[numSourceImages];
		useGraphics = new int[numSourceImages];
		matchFound = new boolean[numSourceImages];
		namePointers = new int[numSourceImages];
		sourceImageFiles = in_imageFiles.toArray(new File[1]);

		// find the width, height and offsets for each image
		loadImageFileInformation();

		Logging.logVerbose("Processing...");
		int[] sortedOrder = new int[numSourceImages];
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

			if(out_errorInfo != null)
			{
				out_errorInfo.m_requiredWidth = minAreaWidth;
				out_errorInfo.m_requiredHeight = minAreaHeight;
			}
			
			Logging.logVerbose("Minimum area required is "+minAreaWidth+"x"+minAreaHeight);
		}
		else
		{
			maxWidth = -1;
			maxHeight = -1;
		}

		if (!layoutSpritesForSheet(sortedOrder, out_errorInfo))
		{
			logFatalError("Images will not fit in any of the allowed output sizes.");
			return null;
		}

		// Ok re-sort the output so that the files are processed in
		// the order they came in...

		int outputOrder[] = new int[numSourceImages];
		for (int i = 0; i < numSourceImages; i++)
		{
			outputOrder[i] = i;
		}

		BufferedImage outputImage = buildCombinedImage(outputOrder);

		//if we have reason to, then add extruded pixels to the output image.
		if (extrude > 0)
		{
			extrudeCombinedImage(outputOrder, outputImage);
		}
		
		for(int i=0; i<numSourceImages; ++i)
		{
			placedSpriteRects[outputOrder[i]].x = placedSpriteRects[outputOrder[i]].x + numPixelsPadding;
			placedSpriteRects[outputOrder[i]].y = placedSpriteRects[outputOrder[i]].y + numPixelsPadding;
		};
		
		PackedTexture pt = new PackedTexture(outputImage, numSourceImages, combinedImageWidth, combinedImageHeight, 
				originalImageWidths, originalImageHeights, croppedImageWidths, croppedImageHeights,
				imageOffsetXs, imageOffsetYs,
				placedSpriteRects,
				sourceImageFiles,
				outputOrder, namePointers);
		return pt;
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

			Logging.logVerbose("Writing pixels from:"+sourceImageFiles[namePointers[i]].getName()+" at x:"+ox+", y:"+oy+", width:"+width+", height:"+height);

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
						logFatalError("input out of range:" + (ix + x) + "," + (iy + y));
					}

					if ((ox + x) < combinedImageWidth && (oy + y) < combinedImageHeight)
					{
						combinedImage.setRGB(ox + x + numPixelsPadding, oy + y + numPixelsPadding, pixel);
					}
					else
					{
						logFatalError("Pixel out of range:" + (ox + x) + "," + (oy + y));
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
			Logging.logVerbose("Extruding pixels from:"+sourceImageFiles[namePointers[i]].getName()+" at x:"+ox+", y:"+oy+", width:"+width+", height:"+height);

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
			Logging.logVerbose("Hash match found:" + f1.getName() + " and " + f2.getName());
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

		Logging.logVerbose("Match found:" + f1.getName() + " and " + f2.getName());

		return true;
	}

	private boolean imagePriorityHeuristic(int indwLeft, int indwRight)
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
	private Rectangle findIntersectingSprite(Rectangle inSprite)
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
	private boolean spriteFitsArea(Rectangle inSprite, int inWidth, int inHeight)
	{
		return (inSprite.x >=0 && inSprite.y >=0) && (inSprite.bottom() <= inHeight && inSprite.right() <= inWidth);
	}

	/**
	Checks if a sprite will fit into our spritesheet if the spritesheet can be resized
	@param Sprite to check 
	@returns If sprite fits (with resizing)
	 */
	private boolean spriteFitsWithResize(Rectangle inSprite)
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
	private boolean resizeToFitSprite(Rectangle inSprite)
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

		Logging.logVerbose("Trying next image size of "+combinedImageWidth+"x"+combinedImageHeight);

		return true;
	}

	/**
	Attempts to place a sprite on our spritesheet, expanding when necessary
	@param The sprite to fit
	@returns The position of the fitted sprite or null if it was not possible to fit
	 */
	private Rectangle findSpaceForSprite(Rectangle inSprite)
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
	private boolean placeSprite(int sortedIndex, int[] sortedOrder)
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
			Rectangle copy = new Rectangle(placedSpriteRects[useGraphics[nOriginalIndex]]);
			placedSpriteRects[nOriginalIndex] = copy;
		}
		else
		{
			if(!placeSprite(inSpriteNo, sortedOrder))
				return false;
		}
		return true;
	}

	/**
	* Layout all sprites
	* 
	* @param in_sortedOrder - List of sprite indices in best order to try placement
	* @param out_errorInfo - error struct to store any error information, can be null
	* @returns if it was possible to fit all sprites
	*/
	private boolean layoutSpritesForSheet(int[] in_sortedOrder, PackerInfo out_errorInfo)
	{
		placedSpriteRects = new Rectangle[in_sortedOrder.length];

		combinedImageHeight = nextAllowedHeight(guessOutputHeight());
		combinedImageWidth = nextAllowedWidth(guessOutputWidth());

		// Clear our collision grid
		gridCheck.Clear();

		boolean success = true;
		
		for (int nSprite = 0; nSprite < numSourceImages; nSprite++)
		{
			if(TryToLayoutSprite(nSprite, in_sortedOrder) ==false)
			{
				int originalID = in_sortedOrder[nSprite];
				
				if(out_errorInfo != null)
				{
					//If were populating the error info, we want to know how many of all the images wouldn't fit on this spritesheet
					out_errorInfo.m_unfittingImages.add(sourceImageFiles[originalID].getAbsolutePath());
					success = false;
				}
				else 
				{
					logFatalError("Could not fit sprite:" + sourceImageFiles[originalID].getAbsolutePath());
					return false;
				}
			}
			numPlacedImages++;

		}

		return success;
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
				Logging.logVerbose("Scanning File:" + sourceImageFiles[i].getName());

				File f = sourceImageFiles[i];
				if (!f.exists())
				{
					Logging.logError(f.getPath() + "does not exist in chosen directory");
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
				Logging.logVerbose(" width:" + width + " height:" + height);
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
				Logging.logVerbose(" clipped area:" + minx + "," + maxx + ":" + miny + "," + maxy);
			}
		}
	}
	/**
	 * Raises a fatal error if enabled
	 * 
	 * @author HMcLaughlin
	 * @param in_message - Message to show
	 */
	private void logFatalError(String in_message)
	{
		if(!m_ignoreFatalLogs)
		{
			Logging.logError(in_message);
		}
	}
}
