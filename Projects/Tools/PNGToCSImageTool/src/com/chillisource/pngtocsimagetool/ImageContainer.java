/*
 *  ImageContainer.java
 *  PNGUtilities
 *
 *  Created by Ian Copland on 30/10/2012
 *  
 *  Copyright 2012 Tag Games Ltd. All rights reserved.
 *
 */

package com.chillisource.pngtocsimagetool;

//========================================================================
/// ImageContainer
///
/// Holds the loaded image data. All data is converted to RGBA format.
//========================================================================
public class ImageContainer
{
	public int[] adwImageData = null;
	public int dwWidth = 0;
	public int dwHeight = 0;
	public int dwType = 0;
	public boolean bHasAlpha = false;
}