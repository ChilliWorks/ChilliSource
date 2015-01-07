/**
 * UserManifestData.java
 * Chilli Source
 * Created by Ian Copland on 20/06/2014.
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

package com.chilliworks.chillisource.androidmanifestbuilder;

/**
 * A container for the data in the user manifest.
 * 
 * @author Ian Copland
 */
public final class UserManifestData
{
	public String m_package = "";
	public String m_versionCode = "1";
	public String m_versionName = "1.0";
	public String m_minSdkVersion = "9";
	public String m_targetSdkVersion = "19";
	public String m_orientation = "sensor";
	public String m_debuggable = "false";
	public String m_facebookAppId = "";
	public String m_googlePlayProjectId = "";
	public String m_manifestExtra = "";
	public String m_applicationExtra = "";
	public String m_permissions = "";
}