/**
 * ApkExpansionDownloader.java
 * ChilliSource
 * Created by Ian Copland on 21/04/2015.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Tag Games Limited
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

package com.chilliworks.chillisource.googleplay.networking;

import android.app.PendingIntent;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Messenger;

import com.chilliworks.chillisource.core.*;
import com.chilliworks.chillisource.core.System;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * A system for managing the downloading of Apk Expansion files. The provides a means to check
 * if the Apk Expansion files already exist and are up to date, and download it if not.
 *
 * Only main expansion files are supported.
 *
 * @author Ian Copland
 */
public final class ApkExpansionDownloader extends System implements IDownloaderClient
{
    public static InterfaceID INTERFACE_ID = new InterfaceID();

    private static final String CACHE_VERSION_CODE_TAG = "VersionCode";
    private static final String CACHE_FILE_SIZE_TAG = "FileSize";

    private IStub m_downloaderClientStub = null;
    private IDownloaderService m_remoteService = null;
    private volatile float m_downloadProgress = 0.0f;

    /**
     * Constructor. Sets the google play LVL public key. Note that, as the public key can only be
     * set once, there can only ever be one instance of this class.
     *
     * @author Ian Copland
     *
     * @param in_lvlPublicKey - The Google Play LVL public key
     */
    public ApkExpansionDownloader(String in_lvlPublicKey)
    {
        ApkExpansionDownloaderService.setPublicKey(in_lvlPublicKey);
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
    @Override public boolean IsA(InterfaceID in_interfaceId)
    {
        return (in_interfaceId == INTERFACE_ID);
    }
    /**
     * This can be called from any thread.
     *
     * @author Ian Copland
     *
     * @return Returns whether or not the expansion files need to be downloaded.
     */
    public boolean isDownloadRequired()
    {
        ApkExpansion expansion = readExpansionCache();
        if (expansion == null)
        {
            return true;
        }

        int versionCode = calcExpansionVersionCodeFromActivity();
        if (expansion.m_versionCode != versionCode)
        {
            return true;
        }

        long fileSize = calcExpansionFileSize(versionCode);
        if (expansion.m_fileSize != fileSize)
        {
            return true;
        }

        return false;
    }
    /**
     * Starts the expansion downloader on the main thread. Prior to calling this,
     * isDownloadRequired() should be called to check if the download is required.
     *
     * This can be called from any thread.
     *
     * @author Ian Copland
     */
    public void startDownload()
    {
        final ApkExpansionDownloader apkExpansionDownloader = this;
        CSApplication.get().scheduleUIThreadTask(new Runnable() {
            @Override
            public void run() {
                Intent notifierIntent = new Intent(CSApplication.get().getActivity(), CSApplication.get().getActivity().getClass());
                notifierIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
                PendingIntent pendingIntent = PendingIntent.getActivity(CSApplication.get().getActivity(), 0, notifierIntent, PendingIntent.FLAG_UPDATE_CURRENT);

                int startResult = 0;
                try {
                    startResult = DownloaderClientMarshaller.startDownloadServiceIfRequired(CSApplication.get().getActivity(), pendingIntent, ApkExpansionDownloaderService.class);
                } catch (PackageManager.NameNotFoundException e) {
                    Logging.logFatal(ExceptionUtils.ConvertToString(e));
                }

                if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED) {
                    m_downloaderClientStub = DownloaderClientMarshaller.CreateStub(apkExpansionDownloader, ApkExpansionDownloaderService.class);
                    if (CSApplication.get().isActive() == true) {
                        m_downloaderClientStub.connect(CSApplication.get().getActivity());
                    }
                } else {
                    downloadComplete();
                }
            }
        });
    }
    /**
     * Sets whether or not the download should be paused.
     *
     * This can be called from any thread.
     *
     * @author Ian Copland
     *
     * @param in_pause - Whether to pause or un-pause the download.
     */
    public void setDownloadPaused(boolean in_pause)
    {
        //TODO: pause download from UI Thread.
    }
    /**
     * This can be called from any thread.
     *
     * @author Ian Copland
     *
     * @return The fractional progress through the download.
     */
    public float getDownloadProgress()
    {
        return m_downloadProgress;
    }
    /**
     * Called when the application is resumed. The downloader client stub will be reconnected
     * if appropriate.
     *
     * @author Ian Copland
     */
    @Override public void onResume()
    {
        if (null != m_downloaderClientStub)
        {
            m_downloaderClientStub.connect(CSApplication.get().getActivity());
        }

        super.onResume();
    }
    /**
     * Called when the service is connected.
     *
     * @author Ian Copland
     *
     * @param in_messenger - The class which forwards on messages.
     */
    @Override public void onServiceConnected(Messenger in_messenger)
    {
        m_remoteService = DownloaderServiceMarshaller.CreateProxy(in_messenger);
        m_remoteService.onClientUpdated(m_downloaderClientStub.getMessenger());
    }
    /**
     * Called when the download state changes.
     *
     * @author Ian Copland
     *
     * @param in_newState - The state which the system has changed to.
     */
    @Override public void onDownloadStateChanged(int in_newState)
    {
        switch (in_newState)
        {
        case IDownloaderClient.STATE_DOWNLOADING:
            onStateChangedDownloading();
            break;
        case IDownloaderClient.STATE_FAILED_CANCELED:
        case IDownloaderClient.STATE_FAILED:
        case IDownloaderClient.STATE_FAILED_FETCHING_URL:
        case IDownloaderClient.STATE_FAILED_UNLICENSED:
            downloadFailed();
            break;
        case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
            onStateChangedPaused();
            break;
        case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
        case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
        case IDownloaderClient.STATE_PAUSED_ROAMING:
            onStateChangedPausedNoWifi();
            break;
        case IDownloaderClient.STATE_FAILED_SDCARD_FULL:
        case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
            downloadFailedNoStorage();
            break;
        case IDownloaderClient.STATE_COMPLETED:
            downloadComplete();
            break;
        }
    }
    /**
     * Called when download progress updates.
     *
     * @author Ian Copland
     *
     * @param in_progress - A container for information on the progress of the download.
     */
    @Override public void onDownloadProgress(DownloadProgressInfo in_progress)
    {
        m_downloadProgress = (float)in_progress.mOverallProgress / (float)in_progress.mOverallTotal;
    }
    /**
     * Called when the application resumes. Disconnects the downloader client stub where
     * appropriate.
     *
     * @author Ian Copland
     */
    @Override public void onSuspend()
    {
        if (null != m_downloaderClientStub)
        {
            m_downloaderClientStub.disconnect(CSApplication.get().getActivity());
        }

        super.onSuspend();
    }
    /**
     * @author Ian Copland
     *
     * @return The path to the expansion cache file.
     */
    private String getExpansionCacheFilePath()
    {
        final String MANIFEST_FILE_NAME = "_ApkExpansion.cache";
        return FileUtils.getSaveDataDirectory(CSApplication.get().getActivity().getPackageName()) + MANIFEST_FILE_NAME;
    }
    /**
     * @author Ian Copland
     *
     * @param in_versionCode - The version code of the expansion file.
     *
     * @return The file path to the main apk expansion file.
     */
    private String getExpansionFilePath(int in_versionCode)
    {
        String expansionDirectory = Helpers.getSaveFilePath(CSApplication.get().getActivityContext());
        String expansionFileName = Helpers.getExpansionAPKFileName(CSApplication.get().getActivityContext(), true, in_versionCode);
        return StringUtils.standardiseDirectoryPath(expansionDirectory) + expansionFileName;
    }
    /**
     * Reads the contents of the expansion cache and returns the contained information. If the
     * cache couldn't be read or doesn't exist, null is returned.
     *
     * @author Ian Copland
     *
     * @return The information in the cache, or null.
     */
    private ApkExpansion readExpansionCache()
    {
        String filePath = getExpansionCacheFilePath();
        if (FileUtils.doesFileExist(FileUtils.StorageLocation.k_externalStorage, filePath) == true)
        {
            String fileContents = FileUtils.readTextFile(FileUtils.StorageLocation.k_apk, filePath);
            if (fileContents.length() > 0)
            {
                try
                {
                    JSONObject jsonRoot = new JSONObject(fileContents);
                    ApkExpansion expansion = new ApkExpansion();
                    expansion.m_versionCode = jsonRoot.getInt(CACHE_VERSION_CODE_TAG);
                    expansion.m_fileSize = jsonRoot.getLong(CACHE_FILE_SIZE_TAG);
                    return expansion;
                }
                catch (JSONException e)
                {
                    Logging.logFatal(ExceptionUtils.ConvertToString(e));
                }
            }
        }

        return null;
    }
    /**
     * Writes the given information to the expansion cache.
     *
     * @author Ian Copland
     *
     * @param in_expansion - The expansion cache.
     */
    private void writeExpansionCache(ApkExpansion in_expansion)
    {
        String fileContents = "";
        try
        {
            JSONObject jsonRoot = new JSONObject();
            jsonRoot.put(CACHE_VERSION_CODE_TAG, in_expansion.m_versionCode);
            jsonRoot.put(CACHE_FILE_SIZE_TAG, in_expansion.m_fileSize);
            fileContents = jsonRoot.toString();
        }
        catch (JSONException e)
        {
            Logging.logFatal(ExceptionUtils.ConvertToString(e));
        }

        String filePath = getExpansionCacheFilePath();
        FileUtils.removeFile(FileUtils.StorageLocation.k_externalStorage, filePath);
        FileUtils.writeTextFile(FileUtils.StorageLocation.k_externalStorage, filePath, fileContents);
    }
    /**
     * @author Ian Copland
     *
     * @return The intended version code, as described by the activity. This information originates
     * in the AndroidManifest.xml.
     */
    private int calcExpansionVersionCodeFromActivity()
    {
        //TODO: Check android manifest.

        int versionCode = 0;
        try
        {
            versionCode = CSApplication.get().getActivityContext().getPackageManager().getPackageInfo(CSApplication.get().getActivityContext().getPackageName(), 0).versionCode;
        }
        catch (PackageManager.NameNotFoundException e)
        {
            Logging.logFatal(ExceptionUtils.ConvertToString(e));
        }

        return versionCode;
    }
    /**
     * @author Ian Copland
     *
     * @return The current version code, i.e the version in the file name of the current Apk
     * Expansion file.
     */
    private int calcExpansionVersionCodeFromFile()
    {
        //TODO: !?

        int versionCode = 0;
        try
        {
            versionCode = CSApplication.get().getActivityContext().getPackageManager().getPackageInfo(CSApplication.get().getActivityContext().getPackageName(), 0).versionCode;
        }
        catch (PackageManager.NameNotFoundException e)
        {
            Logging.logFatal(ExceptionUtils.ConvertToString(e));
        }

        return versionCode;
    }
    /**
     * @author Ian Copland
     *
     * @param in_versionCode - The version code of the expansion file.
     *
     * @return The size of the expansion file in bytes. Zero is returned if the file doesn't exist.
     */
    private long calcExpansionFileSize(int in_versionCode)
    {
        String filePath = getExpansionFilePath(in_versionCode);
        return FileUtils.getFileSize(FileUtils.StorageLocation.k_root, filePath);
    }
    /**
     * Finishes the download, writing the Apk Expansion info to cache and calls the completion
     * delegate.
     *
     * @author Ian Copland
     */
    private void downloadComplete()
    {
        m_downloadProgress = 1.0f;

        ApkExpansion apkExpansion = new ApkExpansion();
        apkExpansion.m_versionCode = calcExpansionVersionCodeFromFile();
        apkExpansion.m_fileSize = calcExpansionFileSize(apkExpansion.m_versionCode);
        writeExpansionCache(apkExpansion);

        cleanupClientStub();

        onStateChangedComplete();
    }
    /**
     * Flags the download as failed and cleans up the downloader.
     *
     * @author Ian Copland
     */
    private void downloadFailed()
    {
        m_downloadProgress = 0.0f;

        String filePath = getExpansionCacheFilePath();
        FileUtils.removeFile(FileUtils.StorageLocation.k_externalStorage, filePath);

        cleanupClientStub();

        onStateChangedFailed();
    }
    /**
     * Flags the download as failed and cleans up the downloader.
     *
     * @author Ian Copland
     */
    private void downloadFailedNoStorage()
    {
        m_downloadProgress = 0.0f;

        String filePath = getExpansionCacheFilePath();
        FileUtils.removeFile(FileUtils.StorageLocation.k_externalStorage, filePath);

        cleanupClientStub();

        onStateChangedFailedNoStorage();
    }
    /**
     * Disconnects the client stub if appropriate and the sets to null.
     *
     * @author Ian Copland
     */
    private void cleanupClientStub()
    {
        if (m_downloaderClientStub != null)
        {
            if (CSApplication.get().isActive() == true)
            {
                m_downloaderClientStub.disconnect(CSApplication.get().getActivity());
            }

            m_downloaderClientStub = null;
        }
    }
    /**
     * Notifies the native side of the app that the state has changed to Downloading.
     *
     * @author Ian Copland
     */
    private native void onStateChangedDownloading();
    /**
     * Notifies the native side of the app that the state has changed to Complete.
     *
     * @author Ian Copland
     */
    private native void onStateChangedComplete();
    /**
     * Notifies the native side of the app that the state has changed to Paused.
     *
     * @author Ian Copland
     */
    private native void onStateChangedPaused();
    /**
     * Notifies the native side of the app that the state has changed to Paused No Wifi.
     *
     * @author Ian Copland
     */
    private native void onStateChangedPausedNoWifi();
    /**
     * Notifies the native side of the app that the state has changed to Failed.
     *
     * @author Ian Copland
     */
    private native void onStateChangedFailed();
    /**
     * Notifies the native side of the app that the state has changed to Failed No Storage.
     *
     * @author Ian Copland
     */
    private native void onStateChangedFailedNoStorage();
    /**
     * A container for information on a single Apk Expansion file.
     *
     * @author Ian Copland
     */
    private static class ApkExpansion
    {
        public int m_versionCode = 0;
        public long m_fileSize = 0;
    }
}
