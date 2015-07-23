/**
 * ApkFileUtils.java
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
import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.List;

/**
 * A collection of convenience methods for reading files from the APK.
 *
 * @author Ian Copland
 */
public final class ApkFileUtils
{
    /**
     * @author Ian Copland
     *
     * @param in_activity - The current activity.
     * @param in_filePath - The file path inside the APK.
     *
     * @return Whether or not the given file exists in the APK.
     */
    public static boolean doesFileExist(Activity in_activity, String in_filePath)
    {
        try
        {
            File file = new File(in_filePath);

            AssetManager assets = in_activity.getAssets();

            String parentDirectoryPath = file.getParent();
            if (parentDirectoryPath == null)
            {
                parentDirectoryPath = "";
            }

            String[] filesArray = assets.list(parentDirectoryPath);
            if (filesArray != null && filesArray.length > 0)
            {
                List<String> filesList = Arrays.asList(filesArray);
                return filesList.contains(file.getName());
            }
        }
        catch (IOException e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logError("An error occurred while checking if file '" + in_filePath + "' exists: " + e.getMessage());
        }

        return false;
    }
    /**
     * Reads the entire contents of a binary file from the APK. The contents are returned as a
     * byte array.
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
        try
        {
            if (doesFileExist(in_activity, in_filePath) == true)
            {
                final int k_bufferSize = 16 * 1024;
                DynamicByteBuffer dynamicByteBuffer = new DynamicByteBuffer(k_bufferSize);

                InputStream stream = new BufferedInputStream(in_activity.getAssets().open(in_filePath));
                try
                {
                    byte[] buffer = new byte[k_bufferSize];
                    int numRead = 0;
                    while(numRead != -1)
                    {
                        numRead = stream.read(buffer, 0, k_bufferSize);
                        if (numRead > 0)
                        {
                            dynamicByteBuffer.appendBytes(buffer, numRead);
                        }
                    }
                }
                finally
                {
                    stream.close();
                }

                return dynamicByteBuffer.toByteArray();
            }
        }
        catch (IOException e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logError("An error occurred while reading file '" + in_filePath + "': " + e.getMessage());
        }

        return null;
    }
    /**
     * Reads the entire contents of a text file from the APK.
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
        byte[] contents = readBinaryFile(in_activity, in_filePath);
        if (contents == null)
        {
            return "";
        }
        return StringUtils.utf8BytesToString(contents);
    }
    /**
     * Calculates the size of the given file located in the APK.
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
        try
        {
            AssetFileDescriptor fileDesc = in_activity.getAssets().openFd(in_filePath);
            if (fileDesc != null)
            {
                return fileDesc.getLength();
            }
        }
        catch (IOException e)
        {
            Logging.logError(ExceptionUtils.convertToString(e));
        }

        return 0;
    }
    /**
     * Copies the requested file from the Apk to external storage.
     *
     * @author Ian Copland
     *
     * @param in_activity - The activity which is currently active.
     * @param in_sourceFilePath - The source file path inside the Apk.
     * @param in_destFilePath - The destination file path in external storage.
     *
     * @return Whether or not the copy was successful.
     */
    public static boolean copyFile(Activity in_activity, String in_sourceFilePath, String in_destFilePath)
    {
        if (doesFileExist(in_activity, in_sourceFilePath) == false)
        {
            return false;
        }

        byte[] fileContents = readBinaryFile(in_activity, in_sourceFilePath);
        if (fileContents == null)
        {
            return false;
        }

        return FileUtils.writeBinaryFile(in_destFilePath, fileContents);
    }
}
