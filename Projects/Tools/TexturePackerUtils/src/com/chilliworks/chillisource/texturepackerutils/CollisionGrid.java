/**
 *  CollisionGrid.java
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

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Simple class to implement a collision grid that uses spatial hashing
 * to reduce collision checks
 * Defaults to 64x64 grid sizes
 * 
 * @author S McGaw
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