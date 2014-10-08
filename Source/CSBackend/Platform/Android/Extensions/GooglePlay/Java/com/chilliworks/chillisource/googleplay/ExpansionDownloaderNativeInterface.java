/**
 * ExpansionDownloader.java
 * Chilli Source
 * Created by Hugh McLaughlin on 25/06/2012.
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

package com.chilliworks.chillisource.googleplay;

import android.app.PendingIntent;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Environment;
import android.os.Messenger;
import android.os.StatFs;

import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;
import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.CSPowerManager;
import com.chilliworks.chillisource.core.INativeInterface;
import com.chilliworks.chillisource.core.InterfaceIDType;
import com.chilliworks.chillisource.core.Logging;
import com.chilliworks.chillisource.core.ResourceHelper;
import com.chilliworks.chillisource.googleplay.GooglePlayDownloaderService;

public class ExpansionDownloaderNativeInterface extends INativeInterface implements IDownloaderClient 
{
	private static final InterfaceIDType InterfaceID = new InterfaceIDType("CExpansionDownloaderNativeInterface");
	
	private IDownloaderService mRemoteService;
    private IStub mDownloaderClientStub;
    
    private int mCurrentState = -1;	
    private float mProgress = 0.0f;
    
    private ExpansionPackageDesc[] mExpansions = {new ExpansionPackageDesc(ExpansionPackageDesc.MAIN_FILE)};
    
    //==============================================================
    /// Native Methods
    //==============================================================
    public static native void OnDownloadStateChangedInProgress();
    public static native void OnDownloadStateChangedComplete();
    public static native void OnDownloadStateChangedPausedNoWiFi();
    public static native void OnDownloadStateChangedPaused();
    public static native void OnDownloadStateChangedFailedNoStorage();
    public static native void OnDownloadStateChangedFailed();
    
	//--------------------------------------------------------------
	/// Is A
	///
	/// @param Comparison type
    /// @return Whether this class is of the same type
	//--------------------------------------------------------------
	@Override public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return inInterfaceType == ExpansionDownloaderNativeInterface.InterfaceID;
	}
	//--------------------------------------------------------------
	/// Init
	///
	/// Initialises the Google Play Expansion downloader.
	///
	/// @param Public key as found on the Google Play Store
	//--------------------------------------------------------------
	public void Init()
	{	
		//Construct this projects resource file package name and set it in the downloader library 
		String strPackage = CSApplication.get().getActivityContext().getPackageName() + ".R";
		Helpers.REMOTE_PACKAGE_NAME = strPackage;
		
		int keyStringID = ResourceHelper.GetDynamicResourceIDForField(CSApplication.get().getActivityContext(), ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_STRING, "GooglePlayPublicKey");
		if(keyStringID > 0)
		{
			String strPublicKey = CSApplication.get().getActivityContext().getString(keyStringID);
			GooglePlayDownloaderService.SetMarketPublicKey(strPublicKey);
		}
		else
		{
			Logging.logError("CExpansionDownloaderNativeInterface: Failed to get 'GooglePlayPublicKey' from the Values resource");
			return;
		}
		
		for(ExpansionPackageDesc expansion : mExpansions)
		{
			int versionCode = 0;
			try 
			{
				versionCode = CSApplication.get().getActivityContext().getPackageManager().getPackageInfo(CSApplication.get().getActivityContext().getPackageName(), 0).versionCode;
			} 
			catch (NameNotFoundException e) 
			{
				e.printStackTrace();
			}
			
			expansion.SetVersionCode(versionCode);
		}
	}
	//--------------------------------------------------------------
	/// Launch Download Intent
	///
	/// Launch the intent that manages the download notification
	/// in the notification bar
	//--------------------------------------------------------------
	private void LaunchDownloadIntent()
	{
		try 
		{
			Intent launchIntent = CSApplication.get().getActivity().getIntent();
			Intent intentToLaunchThisActivityFromNotification = new Intent(CSApplication.get().getActivity(), CSApplication.get().getActivity().getClass());
			intentToLaunchThisActivityFromNotification.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
			intentToLaunchThisActivityFromNotification.setAction(launchIntent.getAction());

			if(launchIntent.getCategories() != null) 
			{
				for(String category : launchIntent.getCategories()) 
				{
					intentToLaunchThisActivityFromNotification.addCategory(category);
				}
			}

			PendingIntent pendingIntent = PendingIntent.getActivity(CSApplication.get().getActivityContext(), 0, intentToLaunchThisActivityFromNotification, PendingIntent.FLAG_UPDATE_CURRENT);
			int Result = DownloaderClientMarshaller.startDownloadServiceIfRequired(CSApplication.get().getActivityContext(), pendingIntent, GooglePlayDownloaderService.class);
			if(Result == DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED)
			{
				OnDownloadStateChangedComplete();
			}
		} 
		catch (NameNotFoundException e) 
		{
			e.printStackTrace();
		} 
	}
	//--------------------------------------------------------------
	/// Get External Free Storage In Bytes
	///
	/// @return SD card free storage in bytes
	//--------------------------------------------------------------
	public long GetExternalFreeStorageInBytes()
	{
		StatFs stat = new StatFs(Environment.getExternalStorageDirectory().getPath());
		@SuppressWarnings("deprecation")
		long sdAvailSize = (long) ((long)stat.getAvailableBlocks() * (long)stat.getBlockSize());
		return sdAvailSize;
	}
	//--------------------------------------------------------------
	/// Get Path To Expansion
	///
	/// @param Index of expansion in list
	/// @returns the path to the expansion file.
	//--------------------------------------------------------------
	public String GetExpansionPath(int inIndex)
	{
		ExpansionPackageDesc expansion = mExpansions[inIndex];
			   
		if(expansion == null)
		{
			Logging.logError("CExpansionDownloader::GetPathToExpansion() - No Expansion file could be found!");
			return "";
		}
			   
		String SavePath = Helpers.getSaveFilePath(CSApplication.get().getActivityContext());
		String FileName = Helpers.getExpansionAPKFileName(CSApplication.get().getActivityContext(), true, expansion.GetVersionCode());

		return SavePath + "/" + FileName; 
	}
	//--------------------------------------------------------------
	/// Get Expansion Version Code
	///
	/// @param Index of expansion in list
	/// @return the version number of the expansion file.
	//--------------------------------------------------------------
	public int GetExpansionVersionCode(int inIndex)
	{
		ExpansionPackageDesc expansion = mExpansions[inIndex];
		return expansion.GetVersionCode();
	}
	//--------------------------------------------------------------
	/// Get Expansion File Size
	///
	/// @param Index of expansion in list
	/// @return the file size of the expansion file.
	//--------------------------------------------------------------
	public long GetExpansionFileSize(int inIndex)
	{
		ExpansionPackageDesc expansion = mExpansions[inIndex];
		return expansion.GetFileSize();
	}
	//--------------------------------------------------------------
	/// Get Num Expansions
	///
	/// @return number of expansion packages
	//--------------------------------------------------------------
	public int GetNumExpansions()
	{
		return mExpansions.length;
	}
	//--------------------------------------------------------------
	/// Download
	///
	/// Begin downloading both main and patch if they do not
	/// already exist
	//--------------------------------------------------------------
    public void Download()
    {
    	final ExpansionDownloaderNativeInterface actualThis = this;
    	CSApplication.get().scheduleUIThreadTask(new Runnable()
    	{
    		@Override public void run() 
    		{
    			if(mDownloaderClientStub == null)
    			{
    				mDownloaderClientStub = DownloaderClientMarshaller.CreateStub(actualThis, GooglePlayDownloaderService.class);
    			}

		        mDownloaderClientStub.connect(CSApplication.get().getActivityContext());
		        
		        LaunchDownloadIntent();
    		}
    	});
    }
	//--------------------------------------------------------------
	/// Set Download Progress
    ///
    /// @param the progress percentage.
	//--------------------------------------------------------------
    public void SetDownloadProgress(float inProgress)
    {
    	mProgress = inProgress;
    }
	//--------------------------------------------------------------
	/// Get Download Progress
    ///
    /// @return the progress percentage.
	//--------------------------------------------------------------
    public float GetDownloadProgress()
    {
    	return mProgress;
    }
	//--------------------------------------------------------------
	/// Keep App Awake
	///
    /// Request a partial wake lock, this will keep the cpu active 
    /// for this app even if the user presses the power button
	//--------------------------------------------------------------
    public void KeepAppAwake()
    {
    	CSPowerManager.RequestWakeLock(CSPowerManager.LOCK_TYPE.PARTIAL_LOCK);
    }
	//--------------------------------------------------------------
	/// Allow App To Sleep
	///
	/// Allow the app to go back to sleep
	//--------------------------------------------------------------
    public void AllowAppToSleep()
    {
    	//Request a partial wake lock, this will keep the cpu active for this app even if the user presses the power button
    	CSPowerManager.ReleaseLock(CSPowerManager.LOCK_TYPE.PARTIAL_LOCK);
    }
    //--------------------------------------------------------------
    /// onServiceConnected
    ///
    /// Critical implementation detail. In onServiceConnected we create the
    /// remote service and marshaler. This is how we pass the client information
    /// back to the service so the client can be properly notified of changes. We
    /// must do this every time we reconnect to the service.
    //--------------------------------------------------------------
    @Override public void onServiceConnected(Messenger m) 
    {
        mRemoteService = DownloaderServiceMarshaller.CreateProxy(m);
        mRemoteService.onClientUpdated(mDownloaderClientStub.getMessenger());
    }
    //--------------------------------------------------------------
    /// onDownloadProgress
    ///
    /// Triggered when progress is made on the download
    //--------------------------------------------------------------
    @Override public void onDownloadProgress(DownloadProgressInfo progress) 
    {
    	//TODO_FIX: Need a way of retrieving the size per expansion
		for(ExpansionPackageDesc expansion : mExpansions)
		{
			expansion.SetFileSize(progress.mOverallTotal);
		}
		
        //Let the native interface know about the progress
        SetDownloadProgress((float)progress.mOverallProgress / (float)progress.mOverallTotal);
    }
    //--------------------------------------------------------------
    /// onDownloadStateChanged
    ///
    /// Sends to the native code the status of the download
    //--------------------------------------------------------------
    @Override public void onDownloadStateChanged(int newState) 
    {
    	if(newState == mCurrentState)
        	return;
        
        mCurrentState = newState;
        
        switch (newState) 
        {
            case IDownloaderClient.STATE_DOWNLOADING:
                OnDownloadStateChangedInProgress();
                break;
                
            case IDownloaderClient.STATE_FAILED_CANCELED:
            case IDownloaderClient.STATE_FAILED:
            case IDownloaderClient.STATE_FAILED_FETCHING_URL:
            case IDownloaderClient.STATE_FAILED_UNLICENSED:
            	Logging.logError("Expansion download failed with error "+newState);
            	OnDownloadStateChangedFailed();
                break;
                
            case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
            	OnDownloadStateChangedPaused();
                break;
                
            case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
            case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
            case IDownloaderClient.STATE_PAUSED_ROAMING:
            	OnDownloadStateChangedPausedNoWiFi();
            	break;
            	
            case IDownloaderClient.STATE_FAILED_SDCARD_FULL:
            case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
            	OnDownloadStateChangedFailedNoStorage();
                break;
                
            case IDownloaderClient.STATE_COMPLETED:
                OnDownloadStateChangedComplete();
                break;
                
            default:
            	break;
        }
    }
	//--------------------------------------------------------------
	/// Resume Download
	///
	/// Starts the download.
	//--------------------------------------------------------------
    public void ResumeDownload() 
    {
    	CSApplication.get().scheduleUIThreadTask(new Runnable()
    	{
			@Override public void run() 
			{
		        if (null != mDownloaderClientStub) 
		        {
		            mDownloaderClientStub.connect(CSApplication.get().getActivityContext());
		        }
			}
    	});
    }
	//--------------------------------------------------------------
	/// Pause Download
	///
	/// ends the download 
	//--------------------------------------------------------------
    public void PauseDownload() 
    {
    	CSApplication.get().scheduleUIThreadTask(new Runnable()
    	{
			@Override public void run() 
			{
		        if (null != mDownloaderClientStub) 
		        {
		            mDownloaderClientStub.disconnect(CSApplication.get().getActivityContext());
		        }
			}
    	});
    }
    
    class ExpansionPackageDesc
    {
    	public static final int MAIN_FILE = 0;
    	public static final int PATCH_FILE = 1;
    	
    	private int mType = 0;
    	private int mVersionCode = 0;
    	private long mFileSize = 0;
    
    	ExpansionPackageDesc(int inType)
    	{
    		mType = inType;
    	}
    	
    	public int GetType()
    	{
    		return mType;
    	}
    	
    	public void SetVersionCode(int inCode)
    	{
    		mVersionCode = inCode;
    	}
    	
    	public void SetFileSize(long inSize)
    	{
    		mFileSize = inSize;
    	}
    	
    	public int GetVersionCode()
    	{
    		return mVersionCode;
    	}
    	
    	public long GetFileSize()
    	{
    		return mFileSize;
    	}
    }
}
