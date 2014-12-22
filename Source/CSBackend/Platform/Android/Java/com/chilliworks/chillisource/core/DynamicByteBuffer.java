/**
 * DynamicByteBuffer.java
 * Chilli Source
 * Created by Ian Copland on 01/01/2013
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
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

package com.chilliworks.chillisource.core;

import java.util.Arrays;

/**
 * A byte array buffer that will resize when it no longer has space to add
 * more data.
 * 
 * @author Ian Copland
 *
 */
public class DynamicByteBuffer 
{
	private byte[] m_data;
	private int m_size;
	private int m_capacity;
	/**
	 * Constructor
	 * 
	 * @author Ian Copland
	 */
	public DynamicByteBuffer()
	{
		m_capacity = 1;
		m_data = new byte[m_capacity];
		m_size = 0;
	}
	/**
	 * Constructor
	 * 
	 * @author Ian Copland
	 * 
	 * @param the default capacity for the buffer.
	 */
	public DynamicByteBuffer(int in_defaultCapacity)
	{
		m_capacity = in_defaultCapacity;
		m_data = new byte[m_capacity];
		m_size = 0;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The internal byte array buffer. Note that this will contain
	 * additional data beyond the end of the added bytes. Use getSize() to
	 * get the number of bytes added.
	 */
	public byte[] toByteArray()
	{
		return Arrays.copyOfRange(m_data, 0, m_size);
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The internal byte array buffer. Note that this will contain
	 * additional data beyond the end of the added bytes. Use getSize() to
	 * get the number of bytes added.
	 */
	public byte[] getInternalBuffer()
	{
		return m_data;
	}
	/**
	 * @author Ian Copland
	 * 
	 * @return The number of bytes added to the buffer.
	 */
	public int getByteCount()
	{
		return m_size;
	}
	/**
	 * Add new bytes to the the buffer. If this has gone over the internal buffer
	 * size it will be resized to fit the new data.
	 * 
	 * @author Ian Copland
	 * 
	 * @param The data.
	 * @param The data count.
	 */
	public void appendBytes(byte[] in_data, int in_count)
	{
		if (in_data != null)
		{
			checkCapacity(in_count);
		
			for (int i = 0; i < in_count; i++)
			{
				m_data[m_size + i] = in_data[i];
			}
			
			m_size += in_count;
		}
	}
	/**
	 * Clears the current buffer so that its size is zero
	 * 
	 * @author S Downie
	 */
	public void clear()
	{
		m_size = 0;
		m_capacity = 0;
	}
	/**
	 * This makes sure the capacity is always big enough to fit any new data 
	 * being added. Any time the size will not be enough this will simply 
	 * double the size.
	 * 
	 * @param The additional data size.
	 */
	private void checkCapacity(int in_additionalCount)
	{
		//if its not big enough, then make sure it is.
		if (m_size + in_additionalCount > m_capacity)
		{
			//create the new buffer
			int dwNewCapacity = m_capacity * 2;
			byte[] abyNewData = new byte[dwNewCapacity];
			
			//put the old data in the new buffer
			for (int i = 0; i < m_size; i++)
				abyNewData[i] = m_data[i];
			
			//scap the old buffer
			m_data = abyNewData;
			
			//update the capacity
			int dwAmountIncreased = dwNewCapacity - m_capacity;
			m_capacity = dwNewCapacity;
			
			//recurse until it is indeed big enough! (hopefully this doesn't happen often...)
			checkCapacity(in_additionalCount - dwAmountIncreased);
		}
	}
	
}
