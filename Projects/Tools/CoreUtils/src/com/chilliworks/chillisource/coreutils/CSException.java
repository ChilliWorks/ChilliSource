/**
 * Result.java
 * Chilli Source
 * Created by Ian Copland on 24/10/2014.
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
 * A general Exception class that provides an error string. This is used for
 * cases where the exception has arisen due to a fatal error in the throwing
 * method and there can be no recovery other than displaying an error to the
 * user.
 * 
 * @author Ian Copland
 */
public final class CSException extends Exception
{
	private static final long serialVersionUID = -8853364100633327738L;
	
	/**
	 * Constructor. Creates the exception with the given error message.
	 * 
	 * @param in_errorMessage - A description of the error that has arisen. This 
	 * should be in a form displayable to the user.
	 */
	public CSException(String in_errorMessage)
	{
		super(in_errorMessage);
		
		assert (in_errorMessage != null) : "Must provide an error message.";
	}
	/**
	 * Constructor. Creates the exception with the given error message.
	 * 
	 * @param in_errorMessage - A description of the error that has arisen. This 
	 * should be in a form displayable to the user.
	 * @param in_throwable - The throwable that caused this exception.
	 */
	public CSException(String in_errorMessage, Throwable in_throwable)
	{
		super(in_errorMessage, in_throwable);
		
		assert (in_errorMessage != null) : "Must provide an error message.";
	}
}
