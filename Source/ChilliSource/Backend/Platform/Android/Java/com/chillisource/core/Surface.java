/**
 *  Surface.java
 *  ChilliSource
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;
import com.chillisource.input.TouchInputNativeInterface;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

/**
 * A surface is composited into the android view hierarchy and manages
 * Open GL and input events.
 *
 * @author I Copland
 */
public class Surface extends GLSurfaceView 
{
	/**
	 * Constructor
	 *
	 * @author I Copland
	 * 
	 * @param Activity
	 * @param GL renderer
	 */
	public Surface(Activity in_activity, Renderer in_renderer) 
	{
		super(in_activity);
        
		int depthBufferSize = 16;
		int stencilBufferSize = 0;
		
		//create the context factory
		setEGLContextFactory(new ContextFactory());
		
		//set the config
		setEGLConfigChooser(new ConfigChooser(5, 6, 5, 0, depthBufferSize, stencilBufferSize));
		
		//create renderer
		setRenderer(in_renderer);
    }
	/**
	 * Triggered when the surface is paused i.e. when it is no
	 * longer visible. This suspends the application
	 *
	 * @author I Copland
	 */
	@Override public void onPause() 
	{
		super.onPause();
	};
	/**
	 * Triggered when the surface receives a touch input event
	 *
	 * @author I Copland
	 * 
	 * @param Input event
	 * 
	 * @return Whether or not the surface consumes the event or
	 * allows it to filter through to the next surface
	 */
	@Override public boolean onTouchEvent(final MotionEvent event) 
	{	
		//get the action data.
		int actionData = event.getAction();
		int actionEvent = actionData & MotionEvent.ACTION_MASK;

		//ACTION_POINTER_ID_MASK and ACTION_POINTER_ID_SHIFT are now depreciated and have been replaced with ACTION_POINTER_INDEX_SHIFT
		//and ACTION_POINTER_INDEX_MASK in order to match the data they are used to retrieve. They are only available in API lv8 however, 
		//so this will need to be updated when lv8 is our minimum target.
		int pointerIndex = (actionData & MotionEvent.ACTION_POINTER_ID_MASK) >> MotionEvent.ACTION_POINTER_ID_SHIFT;
		final int pointerID = event.getPointerId(pointerIndex);

		//get the x and y positions of the touch if possible
		float xPos = -1.0f;
		float yPos = -1.0f;
		if (pointerIndex != -1)
		{
			xPos = event.getX(pointerIndex);
			yPos = event.getY(pointerIndex);
		}

		final float touchX = xPos;
		final float touchY = yPos;

		Runnable task = null;
		//pass the touch events on to MoFlow
		switch (actionEvent)
		{
		case MotionEvent.ACTION_POINTER_DOWN:
		case MotionEvent.ACTION_DOWN:
			task = new Runnable()
			{
				@Override public void run() 
				{
					TouchInputNativeInterface.TouchDown(pointerID, touchX, touchY);
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
					TouchInputNativeInterface.TouchUp(pointerID, touchX, touchY);
				}
			};
			queueEvent(task);
			break;
		case MotionEvent.ACTION_MOVE:
			//in the case of moved events all touches need to be updated.
			for (int i = 0; i < event.getPointerCount(); i++)
			{
				pointerIndex = i;
				final int movedPointerID = event.getPointerId(pointerIndex);
				xPos = event.getX(pointerIndex);
				yPos = event.getY(pointerIndex);

				final float movedTouchX = xPos;
				final float movedTouchY = yPos;

				task = new Runnable()
				{
					@Override public void run() 
					{
						TouchInputNativeInterface.TouchMoved(movedPointerID, movedTouchX, movedTouchY);
					}
				};
				queueEvent(task);
				
			}
			break;
		}
        return true;
    }
}

