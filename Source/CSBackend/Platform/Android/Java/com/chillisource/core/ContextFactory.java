/*
 *  CMoFlowContextFactory.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;
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