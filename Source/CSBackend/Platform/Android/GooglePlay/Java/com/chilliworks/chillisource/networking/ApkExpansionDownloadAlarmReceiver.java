/**
 * ApkExpansionDownloadAlarmReceiver.java
 * ChilliSource
 * Created by Ian Copland on 23/04/2015.
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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;

import com.chilliworks.chillisource.core.ExceptionUtils;
import com.chilliworks.chillisource.core.Logging;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;

/**
 * An alarm receiver for the Apk Expansion Downloader.
 *
 * @author Ian Copland
 */
public final class ApkExpansionDownloadAlarmReceiver extends BroadcastReceiver
{
    /**
     * @author Ian Copland
     *
     * @param in_context - The context
     * @param in_intent - The intent
     */
    @Override public void onReceive(Context in_context, Intent in_intent)
    {
        try
        {
            DownloaderClientMarshaller.startDownloadServiceIfRequired(in_context, in_intent, ApkExpansionDownloadService.class);
        }
        catch (PackageManager.NameNotFoundException e)
        {
            Logging.logFatal(ExceptionUtils.convertToString(e));
        }
    }
}
