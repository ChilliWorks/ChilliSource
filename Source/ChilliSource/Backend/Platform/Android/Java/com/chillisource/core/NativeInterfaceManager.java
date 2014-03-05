/*
 *  CNativeInterfaceManager.java
 *  moFlow
 *
 *  Created by Ian Copland on 09/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.core;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;

import com.chillisource.core.InterfaceIDType;
import com.chillisource.core.IActivityResults;
import com.chillisource.core.IDestroyable;
import com.chillisource.core.ILaunchIntentReceiver;
import com.chillisource.core.IPauseable;
import com.chillisource.core.IResumeable;
import com.chillisource.core.IStartable;
import com.chillisource.core.IStoppable;

//=============================================================
/// Native Interface Manager
///
/// Container class that provides an interface to all Native
/// Interfaces.
//=============================================================
public class NativeInterfaceManager implements IStartable, IResumeable, IPauseable, IStoppable, IDestroyable, IActivityResults, ILaunchIntentReceiver 
{
	//---------------------------------------------------------
	/// Static Member Data
	//---------------------------------------------------------
	static private NativeInterfaceManager mpSingletonInstance = null;
	//---------------------------------------------------------
	/// Member Data
	//---------------------------------------------------------
	private ArrayList<INativeInterface> mNativeInterfaces;
	//---------------------------------------------------------
	/// Get Singleton
	///
	/// @returns the singleton instance of this object.
	//---------------------------------------------------------
	static public NativeInterfaceManager GetSingleton()
	{
		if (mpSingletonInstance == null)
		{
			mpSingletonInstance = new NativeInterfaceManager();
		}
		
		return mpSingletonInstance;
	}
	//---------------------------------------------------------
	/// Constructor
	//---------------------------------------------------------
	public NativeInterfaceManager()
	{
		mNativeInterfaces = new ArrayList<INativeInterface>();
	}
	//---------------------------------------------------------
	/// Setup
	///
	/// Sets up the native/java interface
	///
	/// @param The current activity.
	//---------------------------------------------------------
	public void Setup(Activity inActivity)
	{
		LoadSharedLibraries(inActivity);
		
		//setup the application and the java/native interface
		CoreNativeInterface.CreateApplication();
		CoreNativeInterface.SetupCoreJavaNativeInterface();
	}
	//---------------------------------------------------------
	/// On Start
	///
	/// Resumes any native interfaces that implement IResumeable
	//---------------------------------------------------------
	@Override public void OnStart()
	{
		for (INativeInterface nativeInterface : mNativeInterfaces)
		{
			if (nativeInterface instanceof IStartable)
			{
				((IStartable)nativeInterface).OnStart();
			}
		}
	}
	//---------------------------------------------------------
	/// On Resume
	///
	/// Resumes any native interfaces that implement IResumeable
	//---------------------------------------------------------
	@Override public void OnResume()
	{
		for (INativeInterface nativeInterface : mNativeInterfaces)
		{
			if (nativeInterface instanceof IResumeable)
			{
				((IResumeable)nativeInterface).OnResume();
			}
		}
	}
	//---------------------------------------------------------
	/// On Pause
	///
	/// Pauses any native interfaces that implement IPauseable
	//---------------------------------------------------------
	@Override public void OnPause()
	{
		for (INativeInterface nativeInterface : mNativeInterfaces)
		{
			if (nativeInterface instanceof IPauseable)
			{
				((IPauseable)nativeInterface).OnPause();
			}
		}
	}
	//---------------------------------------------------------
	/// On Stop
	///
	/// Resumes any native interfaces that implement IResumeable
	//---------------------------------------------------------
	@Override public void OnStop()
	{
		for (INativeInterface nativeInterface : mNativeInterfaces)
		{
			if (nativeInterface instanceof IStoppable)
			{
				((IStoppable)nativeInterface).OnStop();
			}
		}
	}
	//---------------------------------------------------------
	/// On Destroy
	///
	/// Destroy any native interfaces that implement IDestroyable
	//---------------------------------------------------------
	@Override public void OnDestroy()
	{
		for (INativeInterface nativeInterface : mNativeInterfaces)
		{
			if (nativeInterface instanceof IDestroyable)
			{
				((IDestroyable)nativeInterface).OnDestroy();
			}
		}
		
		((CoreNativeInterface)GetNativeInterface(CoreNativeInterface.InterfaceID)).DestroyApplication();
	}
	//------------------------------------------------------
	/// On Activity Result
	///
	/// Called from the activity upon receiving
	/// an intent result
	///
	/// @param Request Code
	/// @param Result Code
	/// @param Intent
	//------------------------------------------------------
	@Override public void OnActivityResult(int requestCode, int resultCode, Intent data)
	{
		for (INativeInterface nativeInterface : mNativeInterfaces)
		{
			if (nativeInterface instanceof IActivityResults)
			{
				((IActivityResults)nativeInterface).OnActivityResult(requestCode, resultCode, data);
			}
		}
	}
	//------------------------------------------------------
	/// On Activity Result
	///
	/// Called when the application receives a launch
	/// intent.
	///
	/// @param The new launch intent.
	//------------------------------------------------------
	@Override public void OnLaunchIntentReceived(Intent inIntent) 
	{
		for (INativeInterface nativeInterface : mNativeInterfaces)
		{
			if (nativeInterface instanceof ILaunchIntentReceiver)
			{
				((ILaunchIntentReceiver)nativeInterface).OnLaunchIntentReceived(inIntent);
			}
		}
	}
	//---------------------------------------------------------
	/// Add Native Interface
	///
	/// @param the new native interface.
	//---------------------------------------------------------
	public void AddNativeInterface(INativeInterface inNativeInterface)
	{
		mNativeInterfaces.add(inNativeInterface);
	}
	//---------------------------------------------------------
	/// Get Native Interface
	///
	/// @returns the first native interface with the given
	/// interface.
	//---------------------------------------------------------
	public INativeInterface GetNativeInterface(InterfaceIDType inInterfaceType)
	{
		for (INativeInterface nativeInterface : mNativeInterfaces)
		{
			if (nativeInterface.IsA(inInterfaceType))
			{
				return nativeInterface;
			}
		}
		
		Log.e("MoFlow", "Could not find native interface.");
		return null;
	}
	//---------------------------------------------------------
	/// Load Shared Libraries
	///
	/// Loads all shared libraries required by the appliation.
	/// This includes the main application library, the two
	/// fmod libraries and any additional libraries provided
	/// in the AdditionalSharedLibraries application meta-data.
	///
	/// @param The current activity.
	//---------------------------------------------------------
	private void LoadSharedLibraries(Activity inActivity)
	{
		//load additional shared libraries
		try
		{
			Bundle bundle = inActivity.getPackageManager().getApplicationInfo(inActivity.getPackageName(), PackageManager.GET_META_DATA).metaData;
			String strAdditionalLibraries = bundle.getString("AdditionalSharedLibraries");
			if (strAdditionalLibraries != null)
			{
				String[] astrAdditionalLibraries = strAdditionalLibraries.split(" ");
				
				for (String strAdditionalLibrary : astrAdditionalLibraries)
				{
					System.loadLibrary(strAdditionalLibrary);
				}
			}
		}
		catch (Exception e)
		{
			Log.e("moFlow", "Could not load additional libraries!");
		}
		
		//load the default libraries
		System.loadLibrary("fmodex");
		System.loadLibrary("fmodevent");
		System.loadLibrary("Application");
	}
}
