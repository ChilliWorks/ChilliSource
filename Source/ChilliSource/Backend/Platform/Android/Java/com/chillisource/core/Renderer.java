/*
 *  CMoFlowRenderer.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Intent;
import android.opengl.GLSurfaceView;

import com.chillisource.core.CoreNativeInterface;
import com.chillisource.core.NativeInterfaceManager;

public class Renderer implements GLSurfaceView.Renderer 
{
	//------------------------------------------------------------------
	/// Renderer State
	///
	/// An enum describing the different states of the renderer.
	//------------------------------------------------------------------
	enum RendererState
	{
		INITIALISING,
		RESUMING,
		RENDERING
	}
	//------------------------------------------------------------------
	/// Member data
	//------------------------------------------------------------------
	private long mMillisecondsPerFrame = 33;
	private long mddwLastTime = 0;
	private long mddwTotalRunningTime = 0;
	private RendererState meRendererState = RendererState.INITIALISING;
	private LoadingView mLoadingView = null;
	private boolean mbNewLaunchIntent = false;
	private boolean mbResetTime = false;
	//------------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------------
	public Renderer(LoadingView inLoadingView)
	{
		mLoadingView = inLoadingView;
	}
	//------------------------------------------------------------------
	/// Set Max FPS
	///
	/// @param Max FPS to clamp to
	//------------------------------------------------------------------
	public void SetMaxFPS(int inMaxFPS)
	{
		mMillisecondsPerFrame = (long) (1000.0f/(float)inMaxFPS);
	}
	//------------------------------------------------------------------
	/// New Launch Intent Received
	///
	/// Informs the renderer that a new launch intent event needs to be
	/// sent during the next update.
	//------------------------------------------------------------------
	public void NewLaunchIntentReceived()
	{
		mbNewLaunchIntent = true;
	}
	//------------------------------------------------------------------
	/// On Draw Frame
	///
	/// Called every frame on on rendering thread. This will initialise,
	//// resume and update moFlow.
	///
	/// @param The OpenGL context.
	//------------------------------------------------------------------
    @Override public void onDrawFrame(GL10 inGlContext) 
    {	
    	if (ChilliSourceActivity.GetActivity().IsActive() == true)
    	{
	    	switch (meRendererState)
	    	{
	    	case INITIALISING:
	    		Initialise(inGlContext);
	    		break;
	    	case RESUMING:
	    		Resume(inGlContext);
	    		break;
	    	case RENDERING:
	    		Render(inGlContext);
	    		break;
	    	default:
	    		android.util.Log.e("MoFlow", "Invalid Renderer state.");
	    		break;
	    	}
    	}
    	else
    	{
    		Inactive(inGlContext);
    	}
    }
	//------------------------------------------------------------------
	/// On Surface Changed
	///
	/// Called whenever the surface changes - for example, is resized.
	///
	/// @param The OpenGL context.
    /// @param the new width.
    /// @param the new height.
	//------------------------------------------------------------------
    @Override public void onSurfaceChanged(GL10 inGlContext, int indwWidth, int indwHeight) 
    {
    	//ensure the viewport remains correct.
        inGlContext.glViewport(0, 0, indwWidth, indwHeight);
    }
	//------------------------------------------------------------------
	/// On Surface Created
	///
	/// Called whenever the surface is created. This will set the
    /// renderer into initialising or preparing state depending on
    /// whether the application has been run before or not.
	///
	/// @param The OpenGL context.
    /// @param The config.
	//------------------------------------------------------------------
    @Override public void onSurfaceCreated(GL10 inGlContext, EGLConfig inConfig) 
    {	
    	if (meRendererState != RendererState.INITIALISING)
    	{
    		meRendererState = RendererState.RESUMING;
    	}
    }
	//------------------------------------------------------------------
	/// Inactive
    ///
    /// Render nothing while inactive. This will occur whenever the application
    /// has been resumed, but its not yet forgrounded. This should never
    /// be visible to the user as the default splash should always be
    /// displayed while this is occuring.
    ///
    /// @param The opengl context.
	//------------------------------------------------------------------
    private void Inactive(GL10 inGlContext)
    {
    	inGlContext.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		inGlContext.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
    }
	//------------------------------------------------------------------
	/// Initialise
    ///
    /// Initalises the renderer. This calls into the moFlow initialise.
    ///
    /// @param The opengl context.
	//------------------------------------------------------------------
    private void Initialise(GL10 inGlContext)
    {
    	inGlContext.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		inGlContext.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		
    	((CoreNativeInterface)NativeInterfaceManager.GetSingleton().GetNativeInterface(CoreNativeInterface.InterfaceID)).Initialise();
    	meRendererState = RendererState.RENDERING;
    	
    	mbResetTime = true;
    }
	//------------------------------------------------------------------
	/// Resume
    ///
    /// Resumes moFlow.
    ///
    /// @param The opengl context.
	//------------------------------------------------------------------
    private void Resume(GL10 inGlContext)
    {
    	inGlContext.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		inGlContext.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		
    	((CoreNativeInterface)NativeInterfaceManager.GetSingleton().GetNativeInterface(CoreNativeInterface.InterfaceID)).Resume();
    	meRendererState = RendererState.RENDERING;
    	
    	mbResetTime = true;
    }
	//------------------------------------------------------------------
	/// Render
    ///
    /// Renders moFlow. This called the moFlow BeginFrame event.
    ///
    /// @param The opengl context.
	//------------------------------------------------------------------
    private void Render(GL10 inGlContext)
    {

       	//Ensure we are rendering at the required frame rate. This is achieved by checking
    	//how long a frame has taken and sleeping the app until the intended frame time 
    	//has elapsed.
    	try
    	{
    		long ddwEndTime = System.currentTimeMillis();
    		long ddwTimeTaken = ddwEndTime - mddwLastTime;
    		if (ddwTimeTaken < mMillisecondsPerFrame)
    			Thread.sleep(mMillisecondsPerFrame - ddwTimeTaken);
    	}
    	catch(Exception e)
    	{
    		e.printStackTrace();
    	}
        
    	//if we are resuming, or this is the first frame then ensure the delta time will be 0.
      	if (mbResetTime == true)
        {
      		mddwLastTime = System.currentTimeMillis();
      		mbResetTime = false;
        }
    	
        //calculate delta time. Hopefully this will be kMillisecondsPerFrame in most 
    	//circumstances.
        long ddwTimeNow = System.currentTimeMillis();
    	float fDeltaTime = 0.0f;
    	if (mddwLastTime != 0)
    	{
    		long ddwDeltaTime = ddwTimeNow - mddwLastTime;
    		fDeltaTime = ((float)ddwDeltaTime) * 0.001f;
    		mddwTotalRunningTime += ddwDeltaTime;
    	}
    	mddwLastTime = ddwTimeNow;
    	
    	//if a new launch intent has been received sent the event
    	if (mbNewLaunchIntent == true)
    	{
    		mbNewLaunchIntent = false;
    		Intent newIntent = ChilliSourceActivity.GetActivity().getIntent();
    		if (newIntent != null)
    		{
    			NativeInterfaceManager.GetSingleton().OnLaunchIntentReceived(newIntent);
    		}
    	}
    	
    	//start the frame.
    	((CoreNativeInterface)NativeInterfaceManager.GetSingleton().GetNativeInterface(CoreNativeInterface.InterfaceID)).FrameBegin(fDeltaTime, mddwTotalRunningTime);
    	
    	mLoadingView.Dismiss();
    }
}
