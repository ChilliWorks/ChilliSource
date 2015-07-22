/**
 * ApkExpansionDownloadUtils.java
 * ChilliSource
 * Created by Ian Copland on 29/05/2015.
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

import com.chilliworks.chillisource.core.ApkExpansionInfo;
import com.chilliworks.chillisource.core.ExceptionUtils;
import com.chilliworks.chillisource.core.FileUtils;
import com.chilliworks.chillisource.core.Logging;
import com.chilliworks.chillisource.core.StringUtils;
import com.google.android.vending.expansion.downloader.Helpers;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;

/**
 * A collection of convenience functions for the Apk Expansion Download functionality.
 *
 * @author Ian Copland
 */
public final class ApkExpansionDownloadValidator
{
    private static final String MAIN_EXPANSION_PREFIX = "main.";

    /**
     * Reads the Apk Expansion Config file and checks its contents versus the Expansion file on
     * disk. If they are the same, no download is required. No download is required if there is
     * no Apk Expansion config file.
     *
     * This can be called from any thread.
     *
     * @author Ian Copland
     *
     * @param in_activity - Any of the applications activities.
     *
     * @return Returns whether or not the expansion files need to be downloaded.
     */
    public static boolean isDownloadRequired(Activity in_activity)
    {
        int versionCode = calcExpansionVersionCode(in_activity);
        if (ApkExpansionInfo.getVersionCode() != versionCode)
        {
            return true;
        }

        long fileSize = calcExpansionFileSize(in_activity, versionCode);
        if (ApkExpansionInfo.getFileSize() != fileSize)
        {
            return true;
        }

        return false;
    }
    /**
     * @author Ian Copland
     *
     * @param in_activity - The active Activity.
     *
     * @return The version code of the current expansion file. If the file doesn't exist, then
     * this will return -1.
     */
    private static int calcExpansionVersionCode(Activity in_activity)
    {
        File directory = new File(ApkExpansionInfo.getDirectoryPath());

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
        if (isMainExpansionFileName(in_activity, fileName) == false)
        {
            Logging.logError("The Apk Expansion directory contains a file which is not the main expansion file.");
            return -1;
        }

        //this is safe becuase it wouldn't have passed the previous check otherwise.
        String versionCodeString = fileName.substring(MAIN_EXPANSION_PREFIX.length(), fileName.indexOf(".", MAIN_EXPANSION_PREFIX.length()));
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
     * @param in_activity - The active Activity.
     * @param in_fileName - The file name.
     *
     * @return Whether or not the given file name is a valid name for main expansion file.
     */
    private static boolean isMainExpansionFileName(Activity in_activity, String in_fileName)
    {
        if (in_fileName.startsWith(MAIN_EXPANSION_PREFIX) == false)
        {
            return false;
        }

        int nextPeriod = in_fileName.indexOf(".", MAIN_EXPANSION_PREFIX.length());
        if (nextPeriod == -1)
        {
            return false;
        }

        //check that the version code contains a valid integer.
        String versionCodeString = in_fileName.substring(MAIN_EXPANSION_PREFIX.length(), nextPeriod);
        if (versionCodeString.matches("^-?\\d+$") == false)
        {
            return false;
        }

        if (in_fileName.length() <= nextPeriod + 1)
        {
            return false;
        }

        String suffix = in_fileName.substring(nextPeriod + 1);
        String intendedSuffix = in_activity.getPackageName() + ".obb";
        if (suffix.equals(intendedSuffix) == false)
        {
            return false;
        }

        return true;
    }
    /**
     * @author Ian Copland
     *
     * @param in_activity - Any of the applications activities.
     * @param in_versionCode - The version code of the expansion file.
     *
     * @return The size of the expansion file in bytes. Zero is returned if the file doesn't exist.
     */
    private static long calcExpansionFileSize(Activity in_activity, int in_versionCode)
    {
        String filePath = ApkExpansionInfo.getFilePath();
        return FileUtils.getFileSize(filePath);
    }
}
