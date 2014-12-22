/**
 * Renderer.java
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

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

/**
 *  Implementation of the Android GL renderer.
 *  This actually pumps the main application update loop
 *  as the ChilliSource render thread is the main thread.
 *  
 *  @author Ian Copland
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
	 * @author Ian Copland
	 * 
	 * @param GL Context
	 */
    @Override public void onDrawFrame(GL10 in_GlContext) 
    {
		if (m_prepareFrameCount <= 0 && CSApplication.get() != null && CSApplication.get().isActive() == true)
    	{
    		CSApplication.get().update();
    		m_loadingView.Dismiss();
    	}
		else
		{
			//The surface must issue at least one draw command per draw frame call. Even if 
			//the application isn't active
			in_GlContext.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			in_GlContext.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
			
			//Hacky state to allow the loading image to actually appear. Android seems to require that you draw
			//a few frames in order for this to appear
			if(m_prepareFrameCount > 0)
			{
				m_prepareFrameCount--;
			}
		}
    }
	/**
	 * Triggered when the surface changes (i.e. on resize, etc)
	 * 
	 * @author Ian Copland
	 * 
	 * @param GL Context
	 * @param Surface width
	 * @param Surface height
	 */
    @Override public void onSurfaceChanged(GL10 in_GlContext, int in_width, int in_height) 
    {
    	CoreNativeInterface coreNI = (CoreNativeInterface)CSApplication.get().getSystem(CoreNativeInterface.InterfaceID);
    	if (coreNI != null)
    	{
    		coreNI.onResolutionChanged(in_width, in_height);
    	}
    }
	/**
	 * Triggered when the surface is created and ready for rendering
	 * 
	 * @author Ian Copland
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
