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

package com.chilliworks.chillisource.googleplay.networking;

import android.app.Activity;

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
    private static final String CACHE_VERSION_CODE_TAG = "VersionCode";
    private static final String CACHE_FILE_SIZE_TAG = "FileSize";

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
        ApkExpansionConfig expansion = readApkExpansionConfig(in_activity);
        if (expansion != null)
        {
            int versionCode = calcExpansionVersionCode(in_activity);
            if (expansion.m_versionCode != versionCode)
            {
                return true;
            }

            long fileSize = calcExpansionFileSize(in_activity, versionCode);
            if (expansion.m_fileSize != fileSize)
            {
                return true;
            }
        }

        return false;
    }
    /**
     * @author Ian Copland
     *
     * @param in_activity - Any of the applications activities.
     *
     * @return The directory which Apk Expansion files are stored in.
     */
    private static String getExpansionDirectoryPath(Activity in_activity)
    {
        String expansionDirectory = Helpers.getSaveFilePath(in_activity);
        return StringUtils.standardiseDirectoryPath(expansionDirectory);
    }
    /**
     * @author Ian Copland
     *
     * @param in_activity - Any of the applications activities.
     * @param in_versionCode - The version code of the expansion file.
     *
     * @return The file path to the main apk expansion file.
     */
    private static String getExpansionFilePath(Activity in_activity, int in_versionCode)
    {
        String expansionFileName = Helpers.getExpansionAPKFileName(in_activity, true, in_versionCode);
        return getExpansionDirectoryPath(in_activity) + expansionFileName;
    }
    /**
     * Reads the contents of the apk expansion config and returns the contained information. If the
     * cache couldn't be read or doesn't exist, null is returned.
     *
     * @author Ian Copland
     *
     * @param in_activity - Any of the applications activities.
     *
     * @return The information in the config, or null.
     */
    private static ApkExpansionConfig readApkExpansionConfig(Activity in_activity)
    {
        //TODO: Move into APK root!

        String filePath = "AppResources/ApkExpansion.config";
        if (FileUtils.doesFileExistAPK(in_activity, filePath) == true)
        {
            byte[] fileContentsBytes = FileUtils.readFileAPK(in_activity, filePath);
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
     * @param in_activity - Any of the applications activities.
     *
     * @return The version code of the current expansion file. If the file doesn't exist, then
     * this will return -1;
     */
    private static int calcExpansionVersionCode(Activity in_activity)
    {
        File directory = new File(getExpansionDirectoryPath(in_activity));

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
     * @param in_activity - Any of the applications activities.
     * @param in_versionCode - The version code of the expansion file.
     *
     * @return The size of the expansion file in bytes. Zero is returned if the file doesn't exist.
     */
    private static long calcExpansionFileSize(Activity in_activity, int in_versionCode)
    {
        String filePath = getExpansionFilePath(in_activity, in_versionCode);
        return FileUtils.getFileSize(FileUtils.StorageLocation.k_root, filePath);
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
