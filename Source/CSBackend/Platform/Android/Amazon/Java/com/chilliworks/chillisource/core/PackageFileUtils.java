/**
 * PackageFileUtils.java
 * ChilliSource
 * Created by Ian Copland on 22/07/2015
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

/**
 * A collection of convenience methods for reading files from the either the Apk or the Apk
 * expansion file, depending on the current product flavour. Google Play builds will refer to
 * the Apk expansion file, while Amazon builds will use the Apk.
 *
 * Note that package doesn't quite mean the same thing here as on the native side of the engine.
 * In the native side, package refers specifically to the AppResources/ directory inside either the
 * Apk Expansion file or Apk; this refers simply to root.
 *
 * @author Ian Copland
 */
public final class PackageFileUtils
{
    /**
     * @author Ian Copland
     *
     * @param in_activity - The current activity.
     * @param in_filePath - The file path inside either the Apk or the Apk Expansion file depending
     * on the current product flavour.
     *
     * @return Whether or not the given file exists in either the Apk or the Apk Expansion file
     * depending on the current product flavour.
     */
    public static boolean doesFileExist(Activity in_activity, String in_filePath)
    {
        return ApkFileUtils.doesFileExist(in_activity, in_filePath);
    }
    /**
     * Reads the entire contents of a binary file from either the Apk or the Apk Expansion file
     * depending on the current product flavour. The contents are returned as a byte array.
     *
     * @author Ian Copland
     *
     * @param in_activity - The activity
     * @param in_filePath - The path to the file.
     *
     * @return The file contents.
     */
    public static byte[] readBinaryFile(Activity in_activity, String in_filePath)
    {
        return ApkFileUtils.readBinaryFile(in_activity, in_filePath);
    }
    /**
     * Reads the entire contents of a text file from either the Apk or the Apk Expansion file
     * depending on the current product flavour
     *
     * @author Ian Copland
     *
     * @param in_activity - The activity
     * @param in_filePath - The path to the file.
     *
     * @return The file contents.
     */
    public static String readTextFile(Activity in_activity, String in_filePath)
    {
        return ApkFileUtils.readTextFile(in_activity, in_filePath);
    }
    /**
     * Calculates the size of the given file located in either the Apk or the Apk Expansion file
     * depending on the current product flavour.
     *
     * @author Ian Copland
     *
     * @param in_activity - The activity which is currently active.
     * @param in_filePath - The path to the file which is to have its size calculated.
     *
     * @return The size of the file in bytes.
     */
    public static long getFileSize(Activity in_activity, String in_filePath)
    {
        return ApkFileUtils.getFileSize(in_activity, in_filePath);
    }
    /**
     * Copies the requested file from either the Apk or the Apk Expansion file - depending on the c
     * current product flavour - to external storage.
     *
     * @author Ian Copland
     *
     * @param in_activity - The activity which is currently active.
     * @param in_sourceFilePath - The source file path inside either the Apk or the Apk Expansion
     * file depending on the current product flavour.
     * @param in_destFilePath - The destination file path in external storage.
     *
     * @return Whether or not the copy was successful.
     */
    public static boolean copyFile(Activity in_activity, String in_sourceFilePath, String in_destFilePath)
    {
        return ApkFileUtils.copyFile(in_activity, in_sourceFilePath, in_destFilePath);
    }
}
