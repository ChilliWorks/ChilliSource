/**
 * ColladaToCSModelOptions.java
 * Chilli Source
 * Created by Ian Copland on 15/01/2013.
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

package com.chilliworks.chillisource.colladatocsmodel;

/**
 * A container for the input options for the Collada To CSModel tool.
 * 
 * @author Ian Copland
 */
public final class ColladaToCSModelOptions 
{
	public String m_inputFilePath = "";
	public String m_outputFilePath = "";
	public boolean m_animated = false;
	public boolean m_swapHandedness = false;
	public boolean m_swapYAndZ = false;
	public boolean m_flipVerticalTexCoords = true;
	public boolean m_combineMeshes = true;
	
	public boolean m_vertexHasPosition = false;
	public boolean m_vertexHasTexCoords = false;
	public boolean m_vertexHasNormal = false;
	public boolean m_vertexHasColour = false;
	public boolean m_vertexHasWeights = false;
	public boolean m_vertexHasJointIndices = false;
}
