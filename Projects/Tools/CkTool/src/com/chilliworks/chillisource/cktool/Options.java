/**
 * Options.java
 * Chilli Source
 * Created by Ian Copland on 20/01/2015.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Tag Games Limited
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

package com.chilliworks.chillisource.cktool;

/**
 * A container for options for the CkTool.
 * 
 * @author Ian Copland
 */
public final class Options 
{
	/**
	 * The action to perform using the cktool.
	 * 
	 * @author Ian Copland
	 */
	public enum Action
	{
		NONE,
		BUILD_BANK,
		BUILD_BANK_SINGLE,
		BUILD_STREAM
	}
	
	public String m_inputFilePath = "";
	public String m_outputFilePath = "";
	public Action m_action = Action.NONE;
	public String m_format = "";
	public String m_volume = "";
	public String m_pan = "";
	public String m_loopStart = "";
	public String m_loopEnd = "";
	public String m_loopCount = "";
}
