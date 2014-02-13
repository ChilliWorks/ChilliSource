/*
 *  CScreen.java
 *  Android Screen Utilities
 *
 *  Created by Robert Henning on 11/05/12.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.util;

import android.util.DisplayMetrics;

public class CScreen
{
	public static int GetPixelsFromDips(float infDips)
	{
		// Converting dips to pixels
		return Math.round(infDips * metrics.density);
	}

	public static float GetDipsFromPixels(int indwPixels)
	{
		// Converting pixels to dips
		return (indwPixels / metrics.density);
	}
	
	// Members
	private static DisplayMetrics metrics = new DisplayMetrics();
}
