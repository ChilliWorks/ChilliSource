/**
 * GyroscopeJavaInterface.java
 * ChilliSource
 * Created by Jordan Brown on 20/01/2017.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Tag Games Limited
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

import com.chilliworks.chillisource.core.*;
import com.chilliworks.chillisource.core.System;

//=============================================================
/// Gyroscope Native Interface
///
/// A native interface for interfacing with the Android
/// gyroscope from native code.
//=============================================================
public class GyroscopeNativeInterface extends System implements SensorEventListener
{

    //-----------------------------------------------------
    /// Member data
    //-----------------------------------------------------
    public static InterfaceId INTERFACE_ID = new InterfaceId();

    boolean mbListening = false;
    boolean mbHasGyroscope = false;

    Runnable mRunTask;
    float[] mQuaternion = new float[4];
    //-----------------------------------------------------
    /// Constructor
    //-----------------------------------------------------
    public GyroscopeNativeInterface()
    {
        init();

        PackageManager manager = CSApplication.get().getActivityContext().getPackageManager();
        mbHasGyroscope = manager.hasSystemFeature(PackageManager.FEATURE_SENSOR_GYROSCOPE);

        mRunTask = new Runnable()
        {
            @Override public void run()
            {
                if(true == mbHasGyroscope && true == mbListening)
                {
                    // Android stores quaternions with the w-term first
                    //                     X                Y               Z               W
                    UpdateOrientation(mQuaternion[1], mQuaternion[2], mQuaternion[3], mQuaternion[0]);
                }
            }
        };

    }
    /**
     * Allows querying of whether or not the system implements the interface described by the
     * given interface id.
     *
     * @author Ian Copland
     *
     * @param in_interfaceId - The interface id to check
     *
     * @return Whether the system implements the given interface
     */
    @Override public boolean isA(InterfaceId in_interfaceId)
    {
        return (in_interfaceId == INTERFACE_ID);
    }
    //---------------------------------------------------
    /// Is Available
    ///
    /// @return whether or not the gyroscope is
    /// available on this device.
    //---------------------------------------------------
    public boolean IsAvailable()
    {
        return mbHasGyroscope;
    }
    //---------------------------------------------------
    /// Start Listening
    ///
    /// Starts listening for gyroscope events.
    //---------------------------------------------------
    public void StartListening()
    {
        if (true == mbHasGyroscope && false == mbListening)
        {
            mbListening = true;

            final GyroscopeNativeInterface gyroscopeNI = this;
            Runnable task = new Runnable()
            {
                @Override public void run()
                {
                    SensorManager sensorManager = (SensorManager)CSApplication.get().getActivityContext().getSystemService(Activity.SENSOR_SERVICE);
                    sensorManager.registerListener(gyroscopeNI, sensorManager.getDefaultSensor(Sensor.TYPE_GAME_ROTATION_VECTOR), SensorManager.SENSOR_DELAY_FASTEST);
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
        if (Sensor.TYPE_GAME_ROTATION_VECTOR == event.sensor.getType())
        {
            SensorManager.getQuaternionFromVector(mQuaternion, event.values);

            // Run orientation update task
            CSApplication.get().scheduleMainThreadTask(mRunTask);
        }
    }
    //---------------------------------------------------
    /// Update Orientation
    ///
    /// Calls down to native to update the orientation.
    ///
    /// @param The x component of the orientation.
    /// @param The y component of the orientation.
    /// @param The z component of the orientation.
    //---------------------------------------------------
    private native void UpdateOrientation(float infOrientationX, float infOrientationY, float infOrientationZ, float infOrientationW);
    //---------------------------------------------------
    /// Stop Listening
    ///
    /// Stops listening for gyroscope events.
    //---------------------------------------------------
    public void StopListening()
    {
        if (true == mbHasGyroscope && true == mbListening)
        {
            mbListening = false;

            final GyroscopeNativeInterface gyroscopeNI = this;
            Runnable task = new Runnable()
            {
                @Override public void run()
                {
                    SensorManager sensorManager = (SensorManager)CSApplication.get().getActivityContext().getSystemService(Activity.SENSOR_SERVICE);
                    sensorManager.unregisterListener(gyroscopeNI);
                }
            };
            CSApplication.get().scheduleUIThreadTask(task);
        }
    }
}
