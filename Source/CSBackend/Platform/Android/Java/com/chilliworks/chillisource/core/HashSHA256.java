/**
 * HashSHA256.java
 * Chilli Source
 * Created by Ian Copland on 16/12/2014.
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

//===============================================================
/// CHashSHA256
///
/// Static methods for hashing using SHA256.
//===============================================================
public class HashSHA256 
{
	//-------------------------------------------------------
	/// Generate
	///
	/// The calculates the SHA-256 hash of the given byte
	/// array and returns it as a 32 byte array.
	///
	/// @param The data to hash.
	/// @return The 32 byte array.
	//-------------------------------------------------------
	public static byte[] Generate(final byte[] inabyData)
	{
		byte[] abyOutput = null;
	    try
	    {
	        MessageDigest digest = MessageDigest.getInstance("SHA-256");
	        digest.update(inabyData, 0, inabyData.length);
	        abyOutput = digest.digest();
	    }
	    catch (Exception e)
	    {
	    	Logging.logError(ExceptionUtils.ConvertToString(e));
	    }
	    
	    return abyOutput;
	}
}
