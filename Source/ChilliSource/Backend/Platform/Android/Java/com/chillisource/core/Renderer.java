/**
 *  Renderer.java
 *  ChilliSource
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

/**
 *  Implementation of the Android GL renderer.
 *  This actually pumps the main application update loop
 *  as the ChilliSource render thread is the main thread.
 *  
 *  @author I Copland
 */
public class Renderer implements GLSurfaceView.Renderer 
{
	private LoadingView m_loadingView = null;
	private int m_prepareFrameCount = 0;
	
	/**
	 * Constructor
	 * 
	 * @author S Downie
	 */
	public Renderer()
	{
		m_loadingView = new LoadingView();
	}
	/**
	 * Triggered every frame by the surface in order to render the GL context.
	 * This actually runs the application update and render loop
	 * 
	 * @author I Copland
	 * 
	 * @param GL Context
	 */
    @Override public void onDrawFrame(GL10 in_GlContext) 
    {	
		//Hacky state to allow the loading image to actually appear. Android seems to require that you draw
		//a few frames in order for this to appear
		if(m_prepareFrameCount > 0)
		{
    		//The surface must issue at least one draw command per draw frame call. Even if 
    		//the application isn't active
    		in_GlContext.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    		in_GlContext.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
    		
			m_prepareFrameCount--;
		}
		else if (CSApplication.get() != null && CSApplication.get().isActive() == true)
    	{
    		CSApplication.get().update();
    		m_loadingView.Dismiss();
    	}
    }
	/**
	 * Triggered when the surface changes (i.e. on resize, etc)
	 * 
	 * @author I Copland
	 * 
	 * @param GL Context
	 * @param Surface width
	 * @param Surface height
	 */
    @Override public void onSurfaceChanged(GL10 in_GlContext, int in_width, int in_height) 
    {
    	//ensure the viewport remains correct.
    	in_GlContext.glViewport(0, 0, in_width, in_height);
    }
	/**
	 * Triggered when the surface is created and ready for rendering
	 * 
	 * @author I Copland
	 * 
	 * @param GL Context
	 * @param Configuration
	 */
    @Override public void onSurfaceCreated(GL10 in_GlContext, EGLConfig in_config) 
    {	
    	m_prepareFrameCount = 5;
    	
    	boolean isInitialised = CSApplication.get().hasReceivedInit();
    	
    	if(isInitialised == false)
    	{
    		m_loadingView.Present("com_chillisource_default");
    		//App cannot be initialised prior to this point as OpenGL is not ready til now
    		CSApplication.get().init();
    	}
    	else
    	{
    		m_loadingView.Present("com_chillisource_resume");
    	}
    }
}
