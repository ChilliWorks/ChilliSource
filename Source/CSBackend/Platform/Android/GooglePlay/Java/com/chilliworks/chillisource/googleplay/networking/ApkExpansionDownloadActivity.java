/**
 * ApkExpansionDownloadActivity.java
 * ChilliSource
 * Created by Ian Copland on 27/05/2015.
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

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Messenger;
import android.view.Window;
import android.view.WindowManager;

import com.chilliworks.chillisource.core.CSActivity;
import com.chilliworks.chillisource.core.ExceptionUtils;
import com.chilliworks.chillisource.core.FileUtils;
import com.chilliworks.chillisource.core.Logging;
import com.chilliworks.chillisource.core.NativeLibraryLoader;
import com.chilliworks.chillisource.core.StringUtils;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;

/**
 * An activity which is presented prior to the CSActivity in Google Play builds which checks
 * if the Apk Expansion file is present. If it is, the CSActivity is pushed. If not, then this
 * will start the download.
 *
 * TODO: Explain rendering.
 *
 * @author Ian Copland
 */
public class ApkExpansionDownloadActivity extends Activity implements IDownloaderClient
{
    private static final String CACHE_VERSION_CODE_TAG = "VersionCode";
    private static final String CACHE_FILE_SIZE_TAG = "FileSize";

    private IStub m_downloaderClientStub = null;
    private IDownloaderService m_remoteService = null;
    private ApkExpansionDownloadView m_view = null;

    /**
     * Sets whether or not the download should be paused.
     *
     * This can only be called from the UI thread.
     *
     * @author Ian Copland
     *
     * @param in_pause - Whether to pause or un-pause the download.
     */
    public void setDownloadPaused(final boolean in_pause)
    {
        if (m_remoteService != null)
        {
            if (in_pause == true)
            {
                m_remoteService.requestPauseDownload();
            }
            else
            {
                m_remoteService.requestContinueDownload();
            }
        }
    }
    /**
     * Called when the activity is first created.
     *
     * @author Ian Copland
     *
     * @param in_savedInstanceState - The saved instance state.
     */
    @Override protected void onCreate(Bundle in_savedInstanceState)
    {
        super.onCreate(in_savedInstanceState);

        //enable full screen and no title.
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        //Set black background.
        getWindow().getDecorView().setBackgroundColor(Color.BLACK);

        NativeLibraryLoader.load(this);

        if (isDownloadRequired() == true)
        {
            startDownload();
        }
        else
        {
            startCSActivity();
        }
    }
    /**
     * Called when the application is resumed. The downloader client stub will be reconnected
     * if appropriate.
     *
     * @author Ian Copland
     */
    @Override protected void onResume()
    {
        if (null != m_downloaderClientStub)
        {
            m_downloaderClientStub.connect(this);
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
                m_view.onStateChanged(ApkExpansionDownloadState.k_downloading);
                break;
            case IDownloaderClient.STATE_FAILED_CANCELED:
            case IDownloaderClient.STATE_FAILED:
            case IDownloaderClient.STATE_FAILED_FETCHING_URL:
            case IDownloaderClient.STATE_FAILED_UNLICENSED:
                m_view.onStateChanged(ApkExpansionDownloadState.k_failed);
                cleanupClientStub();
                break;
            case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
                m_view.onStateChanged(ApkExpansionDownloadState.k_paused);
                break;
            case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
            case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
            case IDownloaderClient.STATE_PAUSED_ROAMING:
                m_view.onStateChanged(ApkExpansionDownloadState.k_pausedNoWifi);
                break;
            case IDownloaderClient.STATE_FAILED_SDCARD_FULL:
            case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
                m_view.onStateChanged(ApkExpansionDownloadState.k_failedNoStorage);
                cleanupClientStub();
                break;
            case IDownloaderClient.STATE_COMPLETED:
                m_view.onStateChanged(ApkExpansionDownloadState.k_complete);
                cleanupClientStub();
                destroyView();
                startCSActivity();
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
        double progress = ((double)in_progress.mOverallProgress / (double)in_progress.mOverallTotal);
        m_view.onProgressUpdated(progress);
    }
    /**
     * Called when the application resumes. Disconnects the downloader client stub where
     * appropriate.
     *
     * @author Ian Copland
     */
    @Override public void onStop()
    {
        if (m_downloaderClientStub != null)
        {
            m_downloaderClientStub.disconnect(this);
        }

        super.onStop();
    }
    /**
     * Reads the Apk Expansion Config file and checks its contents versus the Expansion file on
     * disk. If they are the same, no download is required. No download is required if there is
     * no Apk Expansion config file.
     *
     * This can be called from any thread.
     *
     * @author Ian Copland
     *
     * @return Returns whether or not the expansion files need to be downloaded.
     */
    private boolean isDownloadRequired()
    {
        ApkExpansionConfig expansion = readApkExpansionConfig();
        if (expansion != null)
        {
            int versionCode = calcExpansionVersionCode();
            if (expansion.m_versionCode != versionCode)
            {
                return true;
            }

            long fileSize = calcExpansionFileSize(versionCode);
            if (expansion.m_fileSize != fileSize)
            {
                return true;
            }
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
    private void startDownload()
    {
        Logging.logError(" >> Start Download << ");

        createView();

        Intent notifierIntent = new Intent(this, getClass());
        notifierIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, notifierIntent, PendingIntent.FLAG_UPDATE_CURRENT);

        int startResult = 0;
        try
        {
            startResult = DownloaderClientMarshaller.startDownloadServiceIfRequired(this, pendingIntent, ApkExpansionDownloadService.class);
        }
        catch (PackageManager.NameNotFoundException e)
        {
            Logging.logFatal(ExceptionUtils.ConvertToString(e));
        }

        if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED)
        {
            m_downloaderClientStub = DownloaderClientMarshaller.CreateStub(this, ApkExpansionDownloadService.class);
        }
        else
        {
            m_view.onStateChanged(ApkExpansionDownloadState.k_complete);
            cleanupClientStub();
            destroyView();
            startCSActivity();
        }
    }
    /**
     * @author Ian Copland
     *
     * @return The directory which Apk Expansion files are stored in.
     */
    private String getExpansionDirectoryPath()
    {
        String expansionDirectory = Helpers.getSaveFilePath(this);
        return StringUtils.standardiseDirectoryPath(expansionDirectory);
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
        String expansionFileName = Helpers.getExpansionAPKFileName(this, true, in_versionCode);
        return getExpansionDirectoryPath() + expansionFileName;
    }
    /**
     * Reads the contents of the apk expansion config and returns the contained information. If the
     * cache couldn't be read or doesn't exist, null is returned.
     *
     * @author Ian Copland
     *
     * @return The information in the config, or null.
     */
    private ApkExpansionConfig readApkExpansionConfig()
    {
        //TODO: Move into APK root!

        String filePath = "AppResources/ApkExpansion.config";
        if (FileUtils.doesFileExistAPK(this, filePath) == true)
        {
            byte[] fileContentsBytes = FileUtils.readFileAPK(this, filePath);
            String fileContents = StringUtils.utf8BytesToString(fileContentsBytes);
            if (fileContents.length() > 0)
            {
                try
                {
                    JSONObject jsonRoot = new JSONObject(fileContents);
                    ApkExpansionConfig config = new ApkExpansionConfig();
                    config.m_versionCode = jsonRoot.getInt(CACHE_VERSION_CODE_TAG);
                    config.m_fileSize = jsonRoot.getLong(CACHE_FILE_SIZE_TAG);
                    return config;
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
     * @author Ian Copland
     *
     * @return The version code of the current expansion file. If the file doesn't exist, then
     * this will return -1;
     */
    private int calcExpansionVersionCode()
    {
        File directory = new File(getExpansionDirectoryPath());

        if (directory.exists() == false)
        {
            return -1;
        }

        File[] contents = directory.listFiles();
        if (contents.length != 1)
        {
            if (contents.length > 1)
            {
                Logging.logError("The Apk Expansion directory contains more than 1 file!");
            }
            return -1;
        }

        if (contents[0].isFile() == false)
        {
            Logging.logError("The Apk Expansion directory contains a directory.");
            return -1;
        }
        String fileName = contents[0].getName();

        if (fileName.startsWith("main.") == false)
        {
            Logging.logError("The Apk Expansion directory contains a file which is not the main expansion file.");
            return -1;
        }

        int nextPeriod = fileName.indexOf(".", 5);
        if (nextPeriod == -1)
        {
            Logging.logError("The Apk Expansion file has an invalid file name.");
            return -1;
        }

        String versionCodeString = fileName.substring(5, nextPeriod);
        int versionCode = 0;

        try
        {
            versionCode = Integer.parseInt(versionCodeString);
        }
        catch (NumberFormatException e)
        {
            Logging.logError("Could not parse Apk Expansion file version code.");
            return -1;
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
     * Disconnects the client stub if appropriate and the sets to null.
     *
     * @author Ian Copland
     */
    private void cleanupClientStub()
    {
        if (m_downloaderClientStub != null)
        {
            m_downloaderClientStub.disconnect(this);
            m_downloaderClientStub = null;
        }

        m_remoteService.onClientUpdated(null);
        m_remoteService = null;
    }
    /**
     * Creates the view which will render download progress, and send it the onInit() event.
     *
     * @author Ian Copland
     */
    private void createView()
    {
        assert (m_view == null) : "View already exists!";

        m_view = new DefaultApkExpansionDownloadView(this);
        m_view.onInit();
    }
    /**
     * Destroies the view, sending the onDestroy() event and setting to null.
     *
     * @author Ian Copland
     */
    private void destroyView()
    {
        assert (m_view != null) : "View already null!";

        m_view.onDestroy();
        m_view = null;
    }
    /**
     * Starts the CSActivity, which proceeds into the rest of the application.
     *
     * @author Ian Copland
     */
    public void startCSActivity()
    {
        Logging.logError(" >> Start CS Activity << ");

        Intent csActivityIntent = new Intent(this, CSActivity.class);
        startActivity(csActivityIntent);
    }
    /**
     * A container for information stored in the Apk Expansion Config file.
     *
     * @author Ian Copland
     */
    private static class ApkExpansionConfig
    {
        public int m_versionCode = 0;
        public long m_fileSize = 0;
    }
}
