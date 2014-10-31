/**
 * HashCodeUtils.java
 * Chilli Source
 * Created by Ian Copland on 31/10/2014
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

/**
 * A utilities class for calculating object hash codes returned from hashCode().
 * To generate a hash for each member of a class the previous hash needs to be
 * passed into the next. For the initial member INITIAL_VALUE should be passed
 * in as the previous hash.
 * 
 * @author Ian Copland
 *
 */
public final class HashCodeUtils
{
	public static int INITIAL_VALUE = 131;
	
	private static int ARITRARY_PRIME_NUMBER = 79;

	/**
	 * Adds a value into the hash.
	 * 
	 * @author Ian Copland
	 * 
	 * @param in_currentHash - The current hash value before adding the new value.
	 * @param in_value - The new value to add to the hash.
	 * 
	 * @return The new hash code.
	 */
	public static <TType> int add(int in_currentHash, TType in_value)
	{
		return in_currentHash * ARITRARY_PRIME_NUMBER + in_value.hashCode();
	}
}
