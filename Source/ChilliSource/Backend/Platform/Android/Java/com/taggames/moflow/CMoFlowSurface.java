/*
 *  CMoFlowSurface.java
 *  moFlow
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow;
import com.taggames.moflow.nativeinterface.CCoreNativeInterface;
import com.taggames.moflow.nativeinterface.CNativeInterfaceManager;
import com.taggames.moflow.nativeinterface.CTouchInputNativeInterface;

import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

public class CMoFlowSurface extends GLSurfaceView 
{
	//-----------------------------------------------------------------
	/// Constructor
	//-----------------------------------------------------------------
	public CMoFlowSurface(CMoFlowActivity inActivity, CMoFlowRenderer inRenderer) 
	{
		super(inActivity);
        
		int dwDepthBufferSize = 16;
		int dwStencilBufferSize = 0;
		
		//create the context factory
		setEGLContextFactory(new CMoFlowContextFactory());
		
		//set the config
		setEGLConfigChooser(new CMoFlowConfigChooser(5, 6, 5, 0, dwDepthBufferSize, dwStencilBufferSize));
		
		//create renderer
		setRenderer(inRenderer);
    }
	//-----------------------------------------------------------------
	/// On Pause
	///
	/// Pauses the surface, suspending moFlow.
	//-----------------------------------------------------------------
	@Override public void onPause() 
	{
		//create the task to be run on the rendering thread
		Runnable task = new Runnable()
		{
			@Override public void run() 
			{
				((CCoreNativeInterface)CNativeInterfaceManager.GetSingleton().GetNativeInterface(CCoreNativeInterface.InterfaceID)).Suspend();
				
				synchronized(this)
				{
					notifyAll();
				}
			}
		};
		
		//run the task.
		this.queueEvent(task);
		
		//wait for the task to finish before pausing the rendering thread.
		try
		{
			synchronized(task)
			{
				task.wait();
			}
		}
		catch (Exception e)
		{
			Log.e("moFlow", e.getMessage());
			e.printStackTrace();
		}
		
		super.onPause();
	};
	//-----------------------------------------------------------------
	/// On Touch Event 
	///
	/// This records all touch events that occur.
	///
	/// @param the motion event.
	/// @return whether or not this view wants to pass the touch event
	///			on to the next applicable view.
	//-----------------------------------------------------------------
	@Override public boolean onTouchEvent(final MotionEvent event) 
	{	
		//get the action data.
		int dwActionData = event.getAction();
		int dwActionEvent = dwActionData & MotionEvent.ACTION_MASK;

		//ACTION_POINTER_ID_MASK and ACTION_POINTER_ID_SHIFT are now depreciated and have been replaced with ACTION_POINTER_INDEX_SHIFT
		//and ACTION_POINTER_INDEX_MASK in order to match the data they are used to retrieve. They are only available in API lv8 however, 
		//so this will need to be updated when lv8 is our minimum target.
		int dwPointerIndex = (dwActionData & MotionEvent.ACTION_POINTER_ID_MASK) >> MotionEvent.ACTION_POINTER_ID_SHIFT;
		final int dwPointerID = event.getPointerId(dwPointerIndex);

		//get the x and y positions of the touch if possible
		float fX = -1.0f;
		float fY = -1.0f;
		if (dwPointerIndex != -1)
		{
			fX = event.getX(dwPointerIndex);
			fY = event.getY(dwPointerIndex);
		}

		final float fTouchX = fX;
		final float fTouchY = fY;


		Runnable task = null;
		//pass the touch events on to MoFlow
		switch (dwActionEvent)
		{
		case MotionEvent.ACTION_POINTER_DOWN:
		case MotionEvent.ACTION_DOWN:
			task = new Runnable()
			{
				@Override public void run() 
				{
					CTouchInputNativeInterface.TouchDown(dwPointerID, fTouchX, fTouchY);
				}
			};
			queueEvent(task);
			break;
		case MotionEvent.ACTION_POINTER_UP:
		case MotionEvent.ACTION_UP:
		case MotionEvent.ACTION_CANCEL:
		case MotionEvent.ACTION_OUTSIDE:
			task = new Runnable()
			{
				@Override public void run() 
				{
					CTouchInputNativeInterface.TouchUp(dwPointerID, fTouchX, fTouchY);
				}
			};
			queueEvent(task);
			break;
		case MotionEvent.ACTION_MOVE:
			//in the case of moved events all touches need to be updated.
			for (int i = 0; i < event.getPointerCount(); i++)
			{
				dwPointerIndex = i;
				final int dwMovedPointerID = event.getPointerId(dwPointerIndex);
				fX = event.getX(dwPointerIndex);
				fY = event.getY(dwPointerIndex);

				final float fMovedTouchX = fX;
				final float fMovedTouchY = fY;

				task = new Runnable()
				{
					@Override public void run() 
					{
						CTouchInputNativeInterface.TouchMoved(dwMovedPointerID, fMovedTouchX, fMovedTouchY);
					}
				};
				queueEvent(task);
				
			}
			break;
		}
        return true;
    }
}

