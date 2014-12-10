/**
 *  Rectangle.java
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

/**
 * Container that represents a rectangle with origin
 * at top left
 * 
 * @author R Henning
 */
public class Rectangle
{
	public int x = 0;
	public int y = 0;
	public int width = 0;
	public int height = 0;

	/**
	 * Constructor
	 * 
	 * @author R Henning
	 */
	public Rectangle()
	{

	}
	/**
	 * Constructor
	 * 
	 * @author R Henning
	 * 
	 * @param Left edge
	 * @param Top edge
	 * @param Width
	 * @param Height
	 */
	public Rectangle(int in_x, int in_y, int in_width, int in_height)
	{
		this.x = in_x;
		this.y = in_y;
		this.width = in_width;
		this.height = in_height;
	}
	/**
	 * Copy constructor
	 * 
	 * @author Ian Copland.
	 */
	public Rectangle(Rectangle in_rectangle)
	{
		this.x = in_rectangle.x;
		this.y = in_rectangle.y;
		this.width = in_rectangle.width;
		this.height = in_rectangle.height;
	}
	/**
	 * @author R Henning
	 * 
	 * @return Left edge of the rectangle
	 */
	public int left()
	{
		return x;
	}
	/**
	 * @author R Henning
	 * 
	 * @return Right edge of the rectangle
	 */
	public int right()
	{
		return x + width;
	}
	/**
	 * @author R Henning
	 * 
	 * @return Top edge of the rectangle
	 */
	public int top()
	{
		return y;
	}
	/**
	 * @author R Henning
	 * 
	 * @return Bottom edge of the rectangle
	 */
	public int bottom()
	{
		return y + height;
	}
	/**
	 * Tests if one rectangle intersects another
	 * 
	 * @param Rectangle to test against
	 * 
	 * @return Whether given rect intersects this one
	 */
	public boolean intersects(Rectangle in_other)
	{
		return !((in_other.left() >= this.right()) ||
				(in_other.right() <= this.left()) ||
				(in_other.top() >= this.bottom()) ||
				(in_other.bottom() <= this.top()));
	}
}

