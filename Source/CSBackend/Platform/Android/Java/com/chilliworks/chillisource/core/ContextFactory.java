/**
 * ContextFactory.java
 * Chilli Source
 * Created by Ian Copland on 14/08/2012.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
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
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import android.opengl.GLSurfaceView;

class ContextFactory implements GLSurfaceView.EGLContextFactory
{
	private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
	
	public EGLContext createContext(EGL10 inEgl, EGLDisplay inDisplay, EGLConfig inEglConfig) 
    {
        int[] adwAttribArray = 
        {
    		EGL_CONTEXT_CLIENT_VERSION, 
    		2, 
    		EGL10.EGL_NONE 
        };      
        EGLContext context = inEgl.eglCreateContext(inDisplay, inEglConfig, EGL10.EGL_NO_CONTEXT, adwAttribArray);
        return context;
    }

    public void destroyContext(EGL10 inEgl, EGLDisplay inDisplay, EGLContext inContext) 
    {
        inEgl.eglDestroyContext(inDisplay, inContext);
    }
}