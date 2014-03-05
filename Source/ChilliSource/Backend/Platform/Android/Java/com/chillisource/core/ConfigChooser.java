/*
 *  CMoFlowConfigChooser.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;

import android.opengl.GLSurfaceView;

public class ConfigChooser implements GLSurfaceView.EGLConfigChooser 
{
    private static int 		EGL_OPENGL_ES2_BIT 		= 4;
    private static int[] 	s_configAttribs2 =
    {
        EGL10.EGL_RED_SIZE, 4,
        EGL10.EGL_GREEN_SIZE, 4,
        EGL10.EGL_BLUE_SIZE, 4,
        EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL10.EGL_NONE
    };
	
    protected int mRedSize;
    protected int mGreenSize;
    protected int mBlueSize;
    protected int mAlphaSize;
    protected int mDepthSize;
    protected int mStencilSize;
    private int[] mValue = new int[1];
    
    public ConfigChooser(int r, int g, int b, int a, int depth, int stencil) 
    {
        mRedSize = r;
        mGreenSize = g;
        mBlueSize = b;
        mAlphaSize = a;
        mDepthSize = depth;
        mStencilSize = stencil;
    }
    
    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display) 
    {

        /* Get the number of minimally matching EGL configurations
         */
        int[] num_config = new int[1];
        egl.eglChooseConfig(display, s_configAttribs2, null, 0, num_config);

        int numConfigs = num_config[0];

        if (numConfigs <= 0) {
            throw new IllegalArgumentException("No configs match configSpec");
        }

        /* Allocate then read the array of minimally matching EGL configs
         */
        EGLConfig[] configs = new EGLConfig[numConfigs];
        egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, num_config);

        /* Now return the "best" one
         */
        return chooseConfig(egl, display, configs);
    }

    public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display, EGLConfig[] configs) 
    {
        for(EGLConfig config : configs) 
        {
            int d = findConfigAttrib(egl, display, config,
                    EGL10.EGL_DEPTH_SIZE, 0);
            int s = findConfigAttrib(egl, display, config,
                    EGL10.EGL_STENCIL_SIZE, 0);

            // We need at least mDepthSize and mStencilSize bits
            if (d < mDepthSize || s < mStencilSize)
                continue;

            // We want an *exact* match for red/green/blue/alpha
            int r = findConfigAttrib(egl, display, config,
                    EGL10.EGL_RED_SIZE, 0);
            int g = findConfigAttrib(egl, display, config,
                        EGL10.EGL_GREEN_SIZE, 0);
            int b = findConfigAttrib(egl, display, config,
                        EGL10.EGL_BLUE_SIZE, 0);
            int a = findConfigAttrib(egl, display, config,
                    EGL10.EGL_ALPHA_SIZE, 0);

            if (r == mRedSize && g == mGreenSize && b == mBlueSize && a == mAlphaSize)
                return config;
        }
        return null;
    }

    private int findConfigAttrib(EGL10 egl, EGLDisplay display, EGLConfig config, int attribute, int defaultValue) 
    {
        if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) 
        {
            return mValue[0];
        }
        return defaultValue;
    }
}
