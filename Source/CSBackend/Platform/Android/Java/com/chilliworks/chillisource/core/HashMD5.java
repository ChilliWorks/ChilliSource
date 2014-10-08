/**
 * HashMD5.java
 * Chilli Source
 * Created by Scott Downie on 23/09/2014.
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

package com.chilliworks.chillisource.core;

import java.security.MessageDigest;

/**
 * Static class for calculating an MD5 hash for a given data blob
 * 
 * @author S Downie
 */
public class HashMD5
{
	/**
	 * Generate an MD5 hash for the given byte array
	 * 
	 * @author S Downie
	 * 
	 * @param Data to hash
	 * 
	 * @return Hashed data
	 */
	public static byte[] generate(byte[] in_data)
	{
		byte[] output = null;
	    try
	    {
	        MessageDigest digest = MessageDigest.getInstance("MD5");
	        digest.update(in_data, 0, in_data.length);
	        output = digest.digest();
	    }
	    catch (Exception e)
	    {
	    	Logging.logError(ExceptionUtils.ConvertToString(e));
	    }
	    
	    return output;
	}
	/**
	 * Generate an MD5 hash as a hexadecimal string
	 * 
	 * @author S Downie
	 * 
	 * @param Data to hash
	 * 
	 * @return Hashed data as hex
	 */
	public static String generateHex(byte[] in_data)
	{
		byte[] preHex = generate(in_data);

		//converting byte array to Hexadecimal String
		StringBuilder sb = new StringBuilder(2 * preHex.length);
		for(byte b : preHex)
		{
			sb.append(String.format("%02x", b&0xff));
		}

		return sb.toString();
	}
}
