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
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IStub;

/**
 * TODO
 *
 * @author Ian Copland
 */
public final class ApkExpansionDownloader extends System implements IDownloaderClient
{
    public static InterfaceID INTERFACE_ID = new InterfaceID();

    private IStub m_downloaderClientStub = null;

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
     * This can be called on any thread.
     *
     * @author Ian Copland
     *
     * @return Whether or not a download is required.
     */
    public boolean isDownloadRequired()
    {
        int fileVersion = 0; //TODO: Do properly!
        int fileSize = 0; //TODO: Do propertly!
        String fileName = Helpers.getExpansionAPKFileName(CSApplication.get().getActivity(), true, fileVersion);
        return Helpers.doesFileExist(CSApplication.get().getActivity(), fileName, fileSize, false);
    }
    /**
     * Tries to download the APK expansion file.
     *
     * @author Ian Copland
     */
    public void startDownload()
    {
        Logging.logError(">>> Starting Download <<<");

        Intent notifierIntent = new Intent(CSApplication.get().getActivity(), CSApplication.get().getActivity().getClass());
        notifierIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);
        PendingIntent pendingIntent = PendingIntent.getActivity(CSApplication.get().getActivity(), 0, notifierIntent, PendingIntent.FLAG_UPDATE_CURRENT);

        int startResult = 0;
        try
        {
            startResult = DownloaderClientMarshaller.startDownloadServiceIfRequired(CSApplication.get().getActivity(), pendingIntent, ApkExpansionDownloaderService.class);
        }
        catch (PackageManager.NameNotFoundException e)
        {
            Logging.logFatal(ExceptionUtils.ConvertToString(e));
        }

        if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED)
        {
            m_downloaderClientStub = DownloaderClientMarshaller.CreateStub(this, ApkExpansionDownloaderService.class);
        }
        else
        {
            //TODO: ERROR? :S
        }
    }

    @Override public void onResume()
    {
        if (null != m_downloaderClientStub)
        {
            m_downloaderClientStub.connect(CSApplication.get().getActivity());
        }

        super.onResume();
    }

    @Override public void onServiceConnected(Messenger in_messenger)
    {
        //TODO: Maybe send down current state and progress?
    }

    @Override public void onDownloadStateChanged(int in_newState)
    {
        //TODO: call down to native on state change
    }

    @Override public void onDownloadProgress(DownloadProgressInfo in_progress)
    {
        //TODO: call down to native on download progress
    }

    @Override public void onSuspend()
    {
        if (null != m_downloaderClientStub)
        {
            m_downloaderClientStub.disconnect(CSApplication.get().getActivity());
        }

        super.onSuspend();
    }

    private native void onStateChanged(int in_newState);
    private native void onStateChanged(int in_newState);
}
