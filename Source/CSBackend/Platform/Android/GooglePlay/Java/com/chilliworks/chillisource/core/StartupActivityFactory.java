/**
 * StartupActivityFactory.java
 * ChilliSource
 * Created by Ian Copland on 01/06/2015.
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

package com.chilliworks.chillisource.core;

import android.content.Intent;

import com.chilliworks.chillisource.networking.ApkExpansionDownloadActivity;
import com.chilliworks.chillisource.networking.ApkExpansionDownloadValidator;

/**
 * A factory class for creating a flavour specific Activity which is presented prior to the
 * CSActivity. In this case the Google Play Apk expansion download activity will be presented
 * if the Apk expansion files don't exist.
 *
 * @author Ian Copland
 */
public final class StartupActivityFactory
{
    /**
     * Creates the Apk Expansion Download Activity if the Apk Expansion files don't exist.
     * Otherwise does nothing and returns false.
     *
     * @author Ian Copland
     *
     * @param in_currentActivity - The currently active activity, typically the CSActivity.
     *
     * @return Whether or not the activity was started.
     */
    public static boolean tryStartActivity(CSActivity in_currentActivity)
    {
        ApkExpansionInfo.init(in_currentActivity);

        if (ApkExpansionDownloadValidator.isDownloadRequired(in_currentActivity) == true)
        {
            Intent in = new Intent(in_currentActivity, ApkExpansionDownloadActivity.class);
            in_currentActivity.startActivity(in);
            return true;
        }

        return false;
    }
}
