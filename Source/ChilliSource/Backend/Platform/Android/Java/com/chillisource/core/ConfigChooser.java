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

/**
 * A custom config builder for generating configs with a preferred depth size, but
 * able to fall back on a minimum.
 * 
 * @author I Copland
 */
public final class ConfigChooser implements GLSurfaceView.EGLConfigChooser 
{
    private static int EGL_OPENGL_ES2_BIT = 4;
    private static int[] s_configAttribs2 =
    {
        EGL10.EGL_RED_SIZE, 4,
        EGL10.EGL_GREEN_SIZE, 4,
        EGL10.EGL_BLUE_SIZE, 4,
        EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL10.EGL_NONE
    };
	
    private int m_redSize;
    private int m_greenSize;
    private int m_blueSize;
    private int m_alphaSize;
    private int m_minDepthSize;
    private int m_preferredDepthSize;
    private int m_minStencilSize;
    
    /**
     * Constructor.
     * 
     * @author I Copland
     * 
     * @param Size of the red channel.
     * @param Size of the green channel.
     * @param Size of the blue channel.
     * @param Size of the alpha channel.
     * @param Minimum depth buffer size.
     * @param Preferred depth buffer size.
     * @param The minimum stencil buffer size.
     */
    public ConfigChooser(int in_r, int in_g, int in_b, int in_a, int in_minDepth, int in_preferredDepth, int in_minStencil) 
    {
        m_redSize = in_r;
        m_greenSize = in_g;
        m_blueSize = in_b;
        m_alphaSize = in_a;
        m_minDepthSize = in_minDepth;
        m_preferredDepthSize = in_preferredDepth;
        m_minStencilSize = in_minStencil;
    }
    /**
     * Chooses the best config for the given parameters.
     * 
     * @author I Copland
     * 
     * @param The gl context.
     * @param The display.
     * 
     * @return The config.
     */
    public EGLConfig chooseConfig(EGL10 in_egl, EGLDisplay in_display) 
    {
        //Get the number of minimally matching EGL configurations
        int[] num_config = new int[1];
        in_egl.eglChooseConfig(in_display, s_configAttribs2, null, 0, num_config);

        int numConfigs = num_config[0];
        if (numConfigs <= 0) 
        {
            throw new IllegalArgumentException("No configs match configSpec");
        }

        //Allocate then read the array of minimally matching EGL configs
        EGLConfig[] configs = new EGLConfig[numConfigs];
        in_egl.eglChooseConfig(in_display, s_configAttribs2, configs, numConfigs, num_config);

        //Now return the "best" one
        EGLConfig output = chooseConfig(in_egl, in_display, configs, true);
        if (output == null)
        {
        	output = chooseConfig(in_egl, in_display, configs, false);
        }
        return output;
    }
    /**
     * Chooses the best config for the given parameters.
     * 
     * @author I Copland
     * 
     * @param The gl context
     * @param The display
     * @param The list of poddible configs.
     * @param Whether or not to only look for preferred depth sizes.
     * 
     * @return The config.
     */
    private EGLConfig chooseConfig(EGL10 in_egl, EGLDisplay in_display, EGLConfig[] in_configs, boolean in_preferred) 
    {
        for(EGLConfig config : in_configs) 
        {
            int d = findConfigAttrib(in_egl, in_display, config, EGL10.EGL_DEPTH_SIZE, 0);
            int s = findConfigAttrib(in_egl, in_display, config, EGL10.EGL_STENCIL_SIZE, 0);

            // We need at least mDepthSize and mStencilSize bits
            if ((in_preferred == true && d < m_preferredDepthSize) || (in_preferred == false && d < m_minDepthSize) || s < m_minStencilSize)
            {
                continue;
            }

            // We want an *exact* match for red/green/blue/alpha
            int r = findConfigAttrib(in_egl, in_display, config, EGL10.EGL_RED_SIZE, 0);
            int g = findConfigAttrib(in_egl, in_display, config, EGL10.EGL_GREEN_SIZE, 0);
            int b = findConfigAttrib(in_egl, in_display, config, EGL10.EGL_BLUE_SIZE, 0);
            int a = findConfigAttrib(in_egl, in_display, config, EGL10.EGL_ALPHA_SIZE, 0);

            if (r == m_redSize && g == m_greenSize && b == m_blueSize && a == m_alphaSize)
            {
                return config;
            }
        }
        return null;
    }
    /**
     * Get the value for a single attribute within the config.
     * 
     * @author I Copland
     * 
     * @param The gl context
     * @param The display
     * @param The config.
     * @param The attribute.
     * @param The default value.
     * 
     * @return The value.
     */
    private int findConfigAttrib(EGL10 in_egl, EGLDisplay in_display, EGLConfig in_config, int in_attribute, int in_defaultValue) 
    {
    	int[] value = new int[1];
        if (in_egl.eglGetConfigAttrib(in_display, in_config, in_attribute, value) == true) 
        {
            return value[0];
        }
        
        return in_defaultValue;
    }
}
