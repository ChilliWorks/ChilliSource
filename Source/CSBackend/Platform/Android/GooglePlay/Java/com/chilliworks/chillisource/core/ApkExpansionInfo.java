/**
 * ApkExpansionInfo.java
 * ChilliSource
 * Created by Ian Copland on 08/06/2015.
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

import android.app.Activity;

import com.google.android.vending.expansion.downloader.Helpers;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * A static class which provides a series of methods which provide information on the Apk
 * Expansion file.
 *
 * These methods are thread safe.
 *
 * @author Ian Copland
 */
public final class ApkExpansionInfo
{
    private static final String CACHE_VERSION_CODE_TAG = "VersionCode";
    private static final String CACHE_FILE_SIZE_TAG = "FileSize";

    private static boolean m_calculated = false;
    private static String m_apkExpansionDirectoryPath = "";
    private static String m_apkExpansionFilePath = "";
    private static int m_versionCode = 0;
    private static long m_fileSize = 0;

    /**
     * Reads the Apk Expansion config file, describing the intended version code and file size
     * of the main Apk Expansion file.
     *
     * @author Ian Copland
     *
     * @param in_activity - Any of the applications activities.
     */
    public static synchronized void init(Activity in_activity)
    {
        assert (m_calculated == false) : "Cannot re-calculate the expansion info.";

        String filePath = "ApkExpansion.config";
        if (ApkFileUtils.doesFileExist(in_activity, filePath) == false)
        {
            Logging.logFatal("Apk Expansion config file '" + filePath + "' does not exist.");
        }

        String fileContents = ApkFileUtils.readTextFile(in_activity, filePath);

        try
        {
            JSONObject jsonRoot = new JSONObject(fileContents);
            m_versionCode = jsonRoot.getInt(CACHE_VERSION_CODE_TAG);
            m_fileSize = jsonRoot.getLong(CACHE_FILE_SIZE_TAG);
        }
        catch (JSONException e)
        {
            Logging.logFatal(ExceptionUtils.convertToString(e));
        }

        String expansionDirectory = Helpers.getSaveFilePath(in_activity);
        m_apkExpansionDirectoryPath = StringUtils.standardiseDirectoryPath(expansionDirectory);

        String expansionFileName = Helpers.getExpansionAPKFileName(in_activity, true, m_versionCode);
        m_apkExpansionFilePath = m_apkExpansionDirectoryPath + expansionFileName;
    }
    /**
     * Returns the path to the Apk Expansion directory. The Apk Expansion file doesn't necessarily
     * exist yet.
     *
     * init() must have been called prior to this.
     *
     * This is thread safe.
     *
     * @author Ian Copland
     *
     * @return The directory which Apk Expansion files are stored in.
     */
    public static synchronized String getDirectoryPath()
    {
        assert (m_calculated == true) : "Must calculate Apk Expansion info before it can be retrieved.";

        return m_apkExpansionDirectoryPath;
    }
    /**
     * Returns the path to the main Apk Expansion file. The Apk Expansion file doesn't necessarily
     * exist yet.
     *
     * init() must have been called prior to this.
     *
     * This is thread safe.
     *
     * @author Ian Copland
     *
     * @return The file path to the main apk expansion file.
     */
    public static synchronized String getFilePath()
    {
        assert (m_calculated == true) : "Must calculate Apk Expansion info before it can be retrieved.";

        return m_apkExpansionFilePath;
    }
    /**
     * Returns the versionCode of the main Apk Expansion file. TThe Apk Expansion file doesn't
     * necessarily exist yet.
     *
     * init() must have been called prior to this.
     *
     * This is thread safe.
     *
     * @author Ian Copland
     *
     * @return The version code of the Apk Expansion file.
     */
    public static synchronized int getVersionCode()
    {
        assert (m_calculated == true) : "Must calculate Apk Expansion info before it can be retrieved.";

        return m_versionCode;
    }
    /**
     * Returns the file size of the main Apk Expansion file. TThe Apk Expansion file doesn't
     * necessarily exist yet.
     *
     * init() must have been called prior to this.
     *
     * This is thread safe.
     *
     * @author Ian Copland
     *
     * @return The file size of the main Apk Expansion file.
     */
    public static synchronized long getFileSize()
    {
        assert (m_calculated == true) : "Must calculate Apk Expansion info before it can be retrieved.";

        return m_fileSize;
    }
}
