/**
 * Surface.java
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

import com.chilliworks.chillisource.input.TouchInputNativeInterface;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

/**
 * A surface is composited into the android view hierarchy and manages
 * Open GL and input events.
 *
 * @author Ian Copland
 */
public class Surface extends GLSurfaceView 
{
	/**
	 * Constructor
	 *
	 * @author Ian Copland
	 * 
	 * @param App config
	 * @param Activity
	 */
	public Surface(AppConfig in_appConfig, Activity in_activity) 
	{
		super(in_activity);
		
		//create the context factory
		setEGLContextFactory(new ContextFactory());
		
		//set the config
		setEGLConfigChooser(createConfigChooser(in_appConfig));
		
		//create renderer
		setRenderer(new com.chilliworks.chillisource.core.Renderer());
    }
	/**
	 * Creates a new config chooser with the params as defined in
	 * the App.config file.
	 *
	 * @author Ian Copland
	 * 
	 * @param The app config.
	 * 
	 * @return The config chooser.
	 */
	private ConfigChooser createConfigChooser(AppConfig in_config)
	{
		String surfaceFormat = in_config.getSurfaceFormat();
		
		if (surfaceFormat.equalsIgnoreCase("rgb565_depth24") == true)
		{
			return new ConfigChooser(5, 6, 5, 0, 16, 24, 0);
		}
		else if (surfaceFormat.equalsIgnoreCase("rgb565_depth32") == true)
		{
			return new ConfigChooser(5, 6, 5, 0, 16, 32, 0);
		}
		else if (surfaceFormat.equalsIgnoreCase("rgb888_depth24") == true)
		{
			return new ConfigChooser(8, 8, 8, 0, 16, 24, 0);
		}
		else if (surfaceFormat.equalsIgnoreCase("rgb888_depth32") == true)
		{
			return new ConfigChooser(8, 8, 8, 0, 16, 32, 0);
		}
		else
		{
			Logging.logFatal("Surface: Invalid surface format.");
			return null;
		}
	}
	/**
	 * Triggered when the surface receives a touch input event
	 *
	 * @author Ian Copland
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
		@SuppressWarnings("deprecation")
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
		//pass the touch events on to the native side of the engine
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

