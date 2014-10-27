/**
 * LittleEndianWriterUtils.java
 * Chilli Source
 * Created by Ian Copland on 24/10/2014
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Tag Games Limited
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

package com.chilliworks.chillisource.coreutils;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * A collection of methods for writing little endian binary files. All values
 * written using this will first be converted to little endian byte order.
 * 
 * @author Ian Copland
 */
public final class LittleEndianWriterUtils
{
	private static final long MAX_UNSIGNED_INT = 4294967295l;
	private static final int MAX_UNSIGNED_SHORT = 65535;
	
	/**
	 * Writes a signed 32-bit integer value to the given output stream. The value 
	 * will be converted to little endian before being written.
	 * 
	 * @param in_stream - The stream to write the value to.
	 * @param in_value - The value to write to the stream.
	 * 
	 * @throws IOException - Any IO Exception thrown by the given stream.
	 */
	public static void writeInt32(OutputStream in_stream, int in_value) throws IOException
	{
		byte[] bytes = new byte[4];
		
		bytes[0] = (byte)(in_value & 0xFF);
		bytes[1] = (byte)(in_value >>> 8 & 0xFF);
		bytes[2] = (byte)(in_value >>> 16 & 0xFF);
		bytes[3] = (byte)(in_value >>> 24 & 0xFF);
		
		in_stream.write(bytes);
	}
	/**
	 * Writes an unsigned 32-bit integer value to the given output stream. The value 
	 * will be converted to little endian before being written.
	 * 
	 * This is used to write unsigned values out with the max range of a signed int. 
	 * The value is provided as a long which should not be below zero or exceed 
	 * (2^32)-1. If a value is provided outside this range this will try to assert, 
	 * otherwise it will clamp to the valid range.
	 * 
	 * @param in_stream - The stream to write the value to.
	 * @param in_value - The value to write to the stream.
	 * 
	 * @throws IOException - Any IO Exception thrown by the given stream.
	 */
	public static void writeUInt32(OutputStream in_stream, long in_value) throws IOException
	{
		assert (in_value >= 0 && in_value <= MAX_UNSIGNED_INT) : "Value outside range of a unsigned 32-bit integer.";
		long value = Math.max(in_value, 0);
		value = Math.min(value, MAX_UNSIGNED_INT);
		
		byte[] bytes = new byte[4];
		
		bytes[0] = (byte)(value & 0xFF);
		bytes[1] = (byte)(value >>> 8 & 0xFF);
		bytes[2] = (byte)(value >>> 16 & 0xFF);
		bytes[3] = (byte)(value >>> 24 & 0xFF);
		
		in_stream.write(bytes);
	}
	/**
	 * Writes a signed 16-bit integer value to the given output stream. The value will 
	 * be converted to little endian before being written.
	 * 
	 * @param in_stream - The stream to write the value to.
	 * @param in_value - The value to write to the stream.
	 * 
	 * @throws IOException - Any IO Exception thrown by the given stream.
	 */
	public static void writeInt16(OutputStream in_stream, short in_value) throws IOException
	{
		byte[] bytes = new byte[2];
		
		bytes[0] = (byte)(in_value & 0xFF);
		bytes[1] = (byte)(in_value >>> 8 & 0xFF);
		
		in_stream.write(bytes);
	}
	/**
	 * Writes an unsigned 16-bit integer value to the given output stream. The value 
	 * will be converted to little endian before being written.
	 * 
	 * This is used to provide unsigned values out with the max range of a signed short. 
	 * The value is provided as an int which should not be below zero or exceed (2^16)-1.
	 * If a value is provided outside this range this will try to assert, otherwise it 
	 * will clamp to the valid range.
	 * 
	 * @param in_stream - The stream to write the value to.
	 * @param in_value - The value to write to the stream.
	 * 
	 * @throws IOException - Any IO Exception thrown by the given stream.
	 */
	public static void writeUInt16(OutputStream in_stream, int in_value) throws IOException
	{
		assert (in_value >= 0 && in_value <= MAX_UNSIGNED_SHORT) : "Value outside range of a unsigned 16-bit integer.";
		int value = Math.max(in_value, 0);
		value = Math.min(value, MAX_UNSIGNED_SHORT);

		byte[] bytes = new byte[2];
		
		bytes[0] = (byte)(value & 0xFF);
		bytes[1] = (byte)(value >>> 8 & 0xFF);
		
		in_stream.write(bytes);
	}
	/**
	 * Writes an float value to the given output stream. The value will be 
	 * converted to little endian before being written.
	 * 
	 * @param in_stream - The stream to write the value to.
	 * @param in_value - The value to write to the stream.
	 * 
	 * @throws IOException - Any IO Exception thrown by the given stream.
	 */
	public static void writeFloat32(DataOutputStream in_stream, float in_value) throws IOException
	{
		writeInt32(in_stream, Float.floatToRawIntBits(in_value));
	}
}
