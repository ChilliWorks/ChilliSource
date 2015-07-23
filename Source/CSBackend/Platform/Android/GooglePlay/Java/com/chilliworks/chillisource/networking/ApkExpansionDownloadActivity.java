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

package com.chilliworks.chillisource.networking;

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
import com.chilliworks.chillisource.core.StringUtils;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;

/**
 * An activity which is presented prior to the CSActivity in Google Play builds which checks
 * if the Apk Expansion file is present. If it is, the CSActivity is pushed. If not, then this
 * will start the download.
 *
 * An ApkExpansionDownloaderView is used to present the user download progress. By default
 * a basic view is provided, but this can be overridden with an app specific implementation.
 *
 * @author Ian Copland
 */
public final class ApkExpansionDownloadActivity extends Activity implements IDownloaderClient
{
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

        if (ApkExpansionDownloadValidator.isDownloadRequired(this) == true)
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
        //ensure state changes are only received when expected.
        if (m_remoteService == null)
        {
            return;
        }

        String stateString = getString(Helpers.getDownloaderStringResourceIDFromState(in_newState));

        switch (in_newState)
        {
            case IDownloaderClient.STATE_DOWNLOADING:
                m_view.onStateChanged(ApkExpansionDownloadState.DOWNLOADING, stateString);
                break;
            case IDownloaderClient.STATE_FAILED_CANCELED:
            case IDownloaderClient.STATE_FAILED:
            case IDownloaderClient.STATE_FAILED_FETCHING_URL:
            case IDownloaderClient.STATE_FAILED_UNLICENSED:
                m_view.onStateChanged(ApkExpansionDownloadState.FAILED, stateString);
                cleanupClientStub();
                break;
            case IDownloaderClient.STATE_PAUSED_BY_REQUEST:
                m_view.onStateChanged(ApkExpansionDownloadState.PAUSED, stateString);
                break;
            case IDownloaderClient.STATE_PAUSED_NEED_CELLULAR_PERMISSION:
            case IDownloaderClient.STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION:
            case IDownloaderClient.STATE_PAUSED_ROAMING:
                m_view.onStateChanged(ApkExpansionDownloadState.PAUSED_NO_WIFI, stateString);
                break;
            case IDownloaderClient.STATE_FAILED_SDCARD_FULL:
            case IDownloaderClient.STATE_PAUSED_SDCARD_UNAVAILABLE:
                m_view.onStateChanged(ApkExpansionDownloadState.FAILED_NO_STORAGE, stateString);
                cleanupClientStub();
                break;
            case IDownloaderClient.STATE_COMPLETED:
                m_view.onStateChanged(ApkExpansionDownloadState.COMPLETE, stateString);
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
     * Starts the expansion downloader on the main thread. Prior to calling this,
     * isDownloadRequired() should be called to check if the download is required.
     *
     * This can be called from any thread.
     *
     * @author Ian Copland
     */
    private void startDownload()
    {
        deleteExpansionDirectory();
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
            Logging.logFatal(ExceptionUtils.convertToString(e));
        }

        if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED)
        {
            m_downloaderClientStub = DownloaderClientMarshaller.CreateStub(this, ApkExpansionDownloadService.class);
        }
        else
        {
            String stateString = getString(Helpers.getDownloaderStringResourceIDFromState(IDownloaderClient.STATE_COMPLETED));
            m_view.onStateChanged(ApkExpansionDownloadState.COMPLETE, stateString);
            cleanupClientStub();
            destroyView();
            startCSActivity();
        }
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

        m_remoteService = null;
    }
    /**
     * Deletes the expansion directory and its contents if it exists, to be sure that there won't
     * end up being multiple expansion files after the download.
     *
     * @author Ian Copland
     */
    private void deleteExpansionDirectory()
    {
        String expansionDirectory = StringUtils.standardiseDirectoryPath(Helpers.getSaveFilePath(this));
        if (FileUtils.doesDirectoryExist(expansionDirectory) == true)
        {
            boolean success = FileUtils.removeDirectory(expansionDirectory);
            assert (success == true) : "Failed to delete expansion directory.";
        }
    }
    /**
     * Creates the view which will render download progress, and send it the onInit() event.
     *
     * @author Ian Copland
     */
    private void createView()
    {
        assert (m_view == null) : "View already exists!";

        m_view = ApkExpansionDownloadViewFactory.createView(this);
        m_view.onInit();
    }
    /**
     * Destroys the view, sending the onDestroy() event and setting to null.
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
        Intent csActivityIntent = new Intent(this, CSActivity.class);
        startActivity(csActivityIntent);
    }
}
