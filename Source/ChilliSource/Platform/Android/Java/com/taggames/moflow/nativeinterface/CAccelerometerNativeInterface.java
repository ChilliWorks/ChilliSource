//
//  CAccelerometerJavaInterface.java
//  moFlow
//
//  Created by Ian Copland on 12/06/2013.
//  Copyright 2013 Tag Games. All rights reserved.
//

package com.taggames.moflow.nativeinterface;

import android.app.Activity;
import android.content.pm.PackageManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.view.WindowManager;

import com.taggames.moflow.core.CInterfaceIDType;

//=============================================================
/// Video Player Native Interface
///
/// A native interface for interfacing with the Android 
/// accelerometer from native code.
//=============================================================
public class CAccelerometerNativeInterface extends INativeInterface implements SensorEventListener
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
		int mdwNegateX;
		int mdwNegateY;
		int mdwSourceX;
		int mdwSourceY;
		//-----------------------------------------------------
		/// Constructor
		//-----------------------------------------------------
		AxisSwap(int indwNegateX, int indwNegateY, int indwSourceX, int indwSourceY)
		{
			mdwNegateX = indwNegateX;
			mdwNegateY = indwNegateY;
			mdwSourceX = indwSourceX;
			mdwSourceY = indwSourceY;
		}
	};
	//-----------------------------------------------------
	/// Member data
	//-----------------------------------------------------
	public static CInterfaceIDType InterfaceID = new CInterfaceIDType("CAccelerometerNativeInterface");
	final AxisSwap mAxisSwap;
	boolean mbListening = false;
	boolean mbHasAccelerometer = false;
	//-----------------------------------------------------
	/// Constructor
	//-----------------------------------------------------
	public CAccelerometerNativeInterface()
	{
		PackageManager manager = mActivity.getPackageManager();
		mbHasAccelerometer = manager.hasSystemFeature(PackageManager.FEATURE_SENSOR_ACCELEROMETER);
		
		//calculate the axis swap
		final AxisSwap[] adwAxisSwapPossibilities = 
		{
			new AxisSwap( 1,-1, 0, 1),		// ROTATION_0
			new AxisSwap(-1,-1, 1, 0), 		// ROTATION_90
			new AxisSwap(-1, 1, 0, 1), 		// ROTATION_180
			new AxisSwap( 1, 1, 1, 0) 		// ROTATION_270
		}; 
		
		WindowManager windowManager = (WindowManager)mActivity.getSystemService(Activity.WINDOW_SERVICE);
		int rotationIndex = windowManager.getDefaultDisplay().getRotation();
		mAxisSwap = adwAxisSwapPossibilities[rotationIndex];
	}
	//-----------------------------------------------------
	/// Is A
	///
	/// @return whether or not this implements the given 
	/// interface.
	//-----------------------------------------------------
	@Override public boolean IsA(CInterfaceIDType inInterfaceType) 
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
			
			final CAccelerometerNativeInterface accelerometerNI = this;
			Runnable task = new Runnable()
			{ 
				@Override public void run() 
				{
					SensorManager sensorManager = (SensorManager)mActivity.getSystemService(Activity.SENSOR_SERVICE);
					sensorManager.registerListener(accelerometerNI, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), SensorManager.SENSOR_DELAY_GAME);	
				}
			};
			mActivity.runOnUiThread(task);
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
			float fScreenX = ((float)mAxisSwap.mdwNegateX) * event.values[mAxisSwap.mdwSourceX];
			float fScreenY = ((float)mAxisSwap.mdwNegateY) * event.values[mAxisSwap.mdwSourceY];
			float fScreenZ = event.values[2];

			//the values provided by android are in ms^-2. Accelerometer values are more typically given in
			//terms of "g"'s so we are converting here. We are also converting from a x, y and z positive in the
			//left, up and backward directions respectively to right, up and forward directions to be more consistent with iOS.
			final float kGravity = 9.80665f;
			final float kX = -fScreenX / kGravity;
			final float kY = fScreenY / kGravity;
			final float kZ = -fScreenZ / kGravity;
			Runnable task = new Runnable()
			{ 
				@Override public void run() 
				{
					if (true == mbHasAccelerometer && true == mbListening)
					{
			            UpdateAcceleration(kX, kY, kZ);
			        }
				}
			};
			
			mActivity.GetSurface().queueEvent(task);
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
			
			final CAccelerometerNativeInterface accelerometerNI = this;
			Runnable task = new Runnable()
			{ 
				@Override public void run() 
				{
					SensorManager sensorManager = (SensorManager)mActivity.getSystemService(Activity.SENSOR_SERVICE);
					sensorManager.unregisterListener(accelerometerNI);
				}
			};
			mActivity.runOnUiThread(task);
		}
	}
}
