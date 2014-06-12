/**
 *  Surface.java
 *  Chilli Source
 *
 *  Created by Ian Copland on 14/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;
import org.json.JSONObject;

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
	 */
	public Surface(Activity in_activity) 
	{
		super(in_activity);
        
		//create the context factory
		setEGLContextFactory(new ContextFactory());
		
		//set the config
		setEGLConfigChooser(createConfigChooser(in_activity));
		
		//create renderer
		setRenderer(new com.chillisource.core.Renderer());
    }
	/**
	 * Creates a new config chooser with the params as defined in
	 * the App.config file.
	 *
	 * @author I Copland
	 * 
	 * @param The activity.
	 * 
	 * @return The config chooser.
	 */
	private ConfigChooser createConfigChooser(Activity in_activity)
	{
		String surfaceFormat = readSurfaceFormat(in_activity);
		
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
	 * Reads the surface format from the App.config file.
	 *
	 * @author I Copland
	 * 
	 * @param The activity
	 * 
	 * @return The config chooser.
	 */
	private String readSurfaceFormat(Activity in_activity)
	{
		final String k_configFilePath = "AppResources/Shared/App.config";
		
		String output = "rgb565_depth24";
		if (FileUtils.doesFileExistAPK(in_activity, k_configFilePath))
		{
			byte[] byteContents = FileUtils.readFileAPK(in_activity, k_configFilePath);
			String stringContents = StringUtils.UTF8ByteArrayToString(byteContents);
			
			try
			{
				JSONObject jObject = new JSONObject(stringContents);
				if (jObject.has("PreferredSurfaceFormat") == true)
				{
					output = jObject.getString("PreferredSurfaceFormat");
				}
			}
			catch (Exception e)
			{
				Logging.logFatal("Could not load App.config!");
			}
		}
		else
		{
			Logging.logFatal("App.config does not exist!");
		}
		
		return output;
	}
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

