/**
 * AccelerometerJavaInterface.java
 * Chilli Source
 * Created by Ian Copland on 12/06/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
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

package com.chilliworks.chillisource.input;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.WindowManager;

import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.INativeInterface;
import com.chilliworks.chillisource.core.InterfaceIDType;

//=============================================================
/// Video Player Native Interface
///
/// A native interface for interfacing with the Android 
/// accelerometer from native code.
//=============================================================
public class AccelerometerNativeInterface extends INativeInterface implements SensorEventListener
{
	//-----------------------------------------------------
	/// Axis Swap
	///
	/// Defines a container for information used when swapping
	/// from device accelerometer coordinates into screen
	/// coordiates.
	//-----------------------------------------------------
	class AxisSwap
	{
		final int m_negateX;
		final int m_negateY;
		final int m_sourceX;
		final int m_sourceY;
		//-----------------------------------------------------
		/// Constructor
		//-----------------------------------------------------
		AxisSwap(int in_negateX, int in_negateY, int in_sourceX, int in_sourceY)
		{
			m_negateX = in_negateX;
			m_negateY = in_negateY;
			m_sourceX = in_sourceX;
			m_sourceY = in_sourceY;
		}
	};
	//-----------------------------------------------------
	/// Member data
	//-----------------------------------------------------
	public static InterfaceIDType InterfaceID = new InterfaceIDType("CAccelerometerNativeInterface");
	boolean mbListening = false;
	boolean mbHasAccelerometer = false;
	AxisSwap[] m_axisSwapForRotation = null;
	//-----------------------------------------------------
	/// Constructor
	//-----------------------------------------------------
	public AccelerometerNativeInterface()
	{
		PackageManager manager = CSApplication.get().getActivityContext().getPackageManager();
		mbHasAccelerometer = manager.hasSystemFeature(PackageManager.FEATURE_SENSOR_ACCELEROMETER);
		
		m_axisSwapForRotation = new AxisSwap[]
		{
			new AxisSwap( 1,-1, 0, 1),		// ROTATION_0
			new AxisSwap(-1,-1, 1, 0), 		// ROTATION_90
			new AxisSwap(-1, 1, 0, 1), 		// ROTATION_180
			new AxisSwap( 1, 1, 1, 0) 		// ROTATION_270
		}; 
	}
	//-----------------------------------------------------
	/// Is A
	///
	/// @return whether or not this implements the given 
	/// interface.
	//-----------------------------------------------------
	@Override public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return (inInterfaceType.Equals(InterfaceID));
	}
	//---------------------------------------------------
	/// Is Available
	///
	/// @return whether or not the accelerometer is
	/// available on this device.
	//---------------------------------------------------
	public boolean IsAvailable()
	{
		return mbHasAccelerometer;
	}
	//---------------------------------------------------
	/// Start Listening
	///
	/// Starts listening for accelerometer events.
	//---------------------------------------------------
	public void StartListening()
	{
		if (true == mbHasAccelerometer && false == mbListening)
		{
			mbListening = true;
			
			final AccelerometerNativeInterface accelerometerNI = this;
			Runnable task = new Runnable()
			{ 
				@Override public void run() 
				{
					SensorManager sensorManager = (SensorManager)CSApplication.get().getActivityContext().getSystemService(Activity.SENSOR_SERVICE);
					sensorManager.registerListener(accelerometerNI, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_GAME);	
				}
			};
			CSApplication.get().scheduleUIThreadTask(task);
		};
	}
	//---------------------------------------------------
	/// On Accuracy Changed
	///
	/// Called when the accuracy of a sensor has changed.
	///
	/// @param The sensor that has had it's accuracy change.
	/// @param The new accuracy value.
	//---------------------------------------------------
	@Override public void onAccuracyChanged(Sensor sensor, int accuracy) 
	{
	}
	//---------------------------------------------------
	/// On Sensor Changed
	///
	/// Called when sensor values have changed. NOTE: The
	/// application doesn't own the event object passed as 
	/// a parameter and therefore cannot hold on to it. 
	/// The object may be part of an internal pool and 
	/// may be reused by the framework.
	///
	/// @param The sensor event.
	//---------------------------------------------------
	@Override public void onSensorChanged(SensorEvent event) 
	{
		if (Sensor.TYPE_ACCELEROMETER == event.sensor.getType())
		{
			//The values provided from the accelerometer are always relative to the default screen orientation which can change
			//from device to device. To alleviate this we are converting into "screen" coordinates. This has been taken from
			//the nvidia accelerometer white paper which can be found at : http://developer.download.nvidia.com/tegra/docs/tegra_android_accelerometer_v5f.pdf
			Activity activity = CSApplication.get().getActivity();
			WindowManager windowManager = (WindowManager)activity.getSystemService(Activity.WINDOW_SERVICE);
			int rotationIndex = windowManager.getDefaultDisplay().getRotation();
			AxisSwap axisSwap = m_axisSwapForRotation[rotationIndex];
			float fScreenX = ((float)axisSwap.m_negateX) * event.values[axisSwap.m_sourceX];
			float fScreenY = ((float)axisSwap.m_negateY) * event.values[axisSwap.m_sourceY];
			float fScreenZ = event.values[2];

			//the values provided by android are in ms^-2. Accelerometer values are more typically given in
			//terms of "g"'s so we are converting here. We are also converting from a x, y and z positive in the
			//left, up and backward directions respectively to right, up and forward directions to be more consistent with iOS.
			final float k_gravity = 9.80665f;
			final float k_accelerationX = -fScreenX / k_gravity;
			final float k_accelerationY = fScreenY / k_gravity;
			final float k_accelerationZ = -fScreenZ / k_gravity;
			
			//update acceleration on main thread.
			Runnable task = new Runnable()
			{ 
				@Override public void run() 
				{
					if (true == mbHasAccelerometer && true == mbListening)
					{
			            UpdateAcceleration(k_accelerationX, k_accelerationY, k_accelerationZ);
			        }
				}
			};
			CSApplication.get().scheduleMainThreadTask(task);
		}
	}
	//---------------------------------------------------
	/// Update Acceleration
	///
	/// Calls down to native to update the acceleration.
	///
	/// @param The x component of the acceleration.
	/// @param The y component of the acceleration.
	/// @param The z component of the acceleration.
	//---------------------------------------------------
	private native void UpdateAcceleration(float infAccelerationX, float infAccelerationY, float infAccelerationZ);
	//---------------------------------------------------
	/// Stop Listening
	///
	/// Stops listening for accelerometer events.
	//---------------------------------------------------
	public void StopListening()
	{
		if (true == mbHasAccelerometer && true == mbListening)
		{
			mbListening = false;
			
			final AccelerometerNativeInterface accelerometerNI = this;
			Runnable task = new Runnable()
			{ 
				@Override public void run() 
				{
					SensorManager sensorManager = (SensorManager)CSApplication.get().getActivityContext().getSystemService(Activity.SENSOR_SERVICE);
					sensorManager.unregisterListener(accelerometerNI);
				}
			};
			CSApplication.get().scheduleUIThreadTask(task);
		}
	}
}
