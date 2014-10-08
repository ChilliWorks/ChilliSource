/**
 *  PackedTexture.java
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

/**
 * Holds the result of the texture packing process
 * 
 * @author R Henning
 */
public class PackedTexture 
{
	private int m_numImages = 0;
	private int m_packedWidth = 0;
	private int m_packedHeight = 0;
	private BufferedImage m_image = null;
	
	private int m_originalImageHeights[];
	private int m_originalImageWidths[];
	private int m_croppedImageHeights[];
	private int m_croppedImageWidths[];
	private int m_imageOffsetXs[];
	private int m_imageOffsetYs[];
	private Rectangle m_imageRects[];
	private File[] m_files;
	private int m_dataOrderKeys[];
	private int m_nameOrderKeys[];
	
	/**
	 * Constructor
	 * 
	 * @author S Downie
	 * 
	 * @param Image
	 * @param Num images
	 * @param Packed width
	 * @param Packed height
	 * @param Widths
	 * @param Heights
	 * @param Cropped widths
	 * @param Cropped heights
	 * @param X offsets
	 * @param Y offsets
	 * @param Rects
	 * @param Files
	 * @param Data order keys
	 * @param File order keys
	 */
	public PackedTexture(BufferedImage in_image, int in_numImages, int in_packedWidth, int in_packedHeight, 
			int[] in_originalWidths, int[] in_originalHeights, int[] in_cropWidths, int[] in_cropHeights, 
			int[] in_offsetsX, int[] in_offsetsY,
			Rectangle[] in_imageRects,
			File[] in_files, 
			int[] in_dataOrderKeys, int[] in_nameOrderKeys)
	{
		m_image = in_image;
		m_numImages = in_numImages;
		m_packedWidth = in_packedWidth;
		m_packedHeight = in_packedHeight;
		m_originalImageWidths = in_originalWidths;
		m_originalImageHeights = in_originalHeights;
		m_croppedImageWidths = in_cropWidths;
		m_croppedImageHeights = in_cropHeights;
		m_imageOffsetXs = in_offsetsX;
		m_imageOffsetYs = in_offsetsY;
		m_imageRects = in_imageRects;
		m_files = in_files;
		m_dataOrderKeys = in_dataOrderKeys;
		m_nameOrderKeys = in_nameOrderKeys;
	}
	/**
	* @author S Downie
	*
	* @return Num packed images
	*/
	public int getNumImages()
	{
		return m_numImages;
	}
	/**
	* @author S Downie
	*
	* @return Width of packed image
	*/
	public int getPackedWidth()
	{
		return m_packedWidth;
	}
	/**
	* @author S Downie
	*
	* @return Height of packed image
	*/
	public int getPackedHeight()
	{
		return m_packedHeight;
	}
	/**
	* @author S Downie
	*
	* @return Packed image
	*/
	public BufferedImage getPackedTexture()
	{
		return m_image;
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return X offset for image at index
	*/
	public int getOffsetX(int in_index)
	{
		return m_imageOffsetXs[m_dataOrderKeys[in_index]];
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return Y offset for image at index
	*/
	public int getOffsetY(int in_index)
	{
		return m_imageOffsetYs[m_dataOrderKeys[in_index]];
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return Width of cropped image at index
	*/
	public int getCroppedWidth(int in_index)
	{
		return m_croppedImageWidths[m_dataOrderKeys[in_index]];
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return Height of cropped image at index
	*/
	public int getCroppedHeight(int in_index)
	{
		return m_croppedImageHeights[m_dataOrderKeys[in_index]];
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return Width of original image at index
	*/
	public int getOriginalWidth(int in_index)
	{
		return m_originalImageWidths[m_dataOrderKeys[in_index]];
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return Height of original image at index
	*/
	public int getOriginalHeight(int in_index)
	{
		return m_originalImageHeights[m_dataOrderKeys[in_index]];
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return X origin (left) of image at index
	*/
	public int getOriginX(int in_index)
	{
		return m_imageRects[m_dataOrderKeys[in_index]].x;
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return Y origin (top) of image at index
	*/
	public int getOriginY(int in_index)
	{
		return m_imageRects[m_dataOrderKeys[in_index]].y;
	}
	/**
	* @author S Downie
	*
	* @param Image index
	* 
	* @return Original file for image at index
	*/
	public File getOriginalFile(int in_index)
	{
		return m_files[m_nameOrderKeys[in_index]];
	}
}
