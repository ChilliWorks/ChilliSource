/**
 * InternalFileUtils.java
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

import android.content.Context;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * A collection of convenience methods for reading and writing files to internal storage.
 *
 * @author Ian Copland
 */
public class InternalFileUtils
{
    /**
     * @author Ian Copland
     *
     * @param in_context - The current context.
     * @param in_filePath - The file path.
     *
     * @return Whether or not the given file exists in internal storage.
     */
    public static boolean doesFileExist(Context in_context, String in_filePath)
    {
        File file = in_context.getFileStreamPath(in_filePath);
        if (file.exists() == true && file.isFile() == true)
        {
            return true;
        }
        return false;
    }
    /**
     * Reads the entire contents of a binary file from internal storage. The contents are
     * returned as a byte array.
     *
     * @author Ian Copland
     *
     * @param in_filePath - The path to the file.
     *
     * @return The file contents.
     */
    public static byte[] readBinaryFile(Context in_context, String in_filePath)
    {
        try
        {
            if (doesFileExist(in_context, in_filePath) == true)
            {
                final int k_bufferSize = 16 * 1024;
                DynamicByteBuffer dynamicByteBuffer = new DynamicByteBuffer(k_bufferSize);

                File file = in_context.getFileStreamPath(in_filePath);
                InputStream stream = new BufferedInputStream(in_context.openFileInput(in_filePath));
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
        catch (Exception e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logError("An error occurred while reading file '" + in_filePath + "': " + e.getMessage());
        }

        return null;
    }
    /**
     * Reads the entire contents of a text file from internal storage.
     *
     * @author Ian Copland
     *
     * @param in_context - The active context.
     * @param in_filePath - The path to the file.
     *
     * @return The file contents.
     */
    public static String readTextFile(Context in_context, String in_filePath)
    {
        byte[] contents = readBinaryFile(in_context, in_filePath);
        if (contents == null)
        {
            return "";
        }
        return StringUtils.utf8BytesToString(contents);
    }
    /**
     * Writes a byte array to a file on internal storage.
     *
     * @author Ian Copland
     *
     * @param in_context - The active context.
     * @param in_filePath - The filename.
     * @param in_fileContents - The data to be written to file.
     *
     * @return Whether or not the file was successfully written.
     */
    public static boolean writeBinaryFile(Context in_context, String in_filePath, byte[] in_fileContents)
    {
        boolean success = false;
        try
        {
            OutputStream stream = new BufferedOutputStream(in_context.openFileOutput(in_filePath, CSActivity.MODE_PRIVATE));
            try
            {
                stream.write(in_fileContents);
                success = true;
            }
            finally
            {
                stream.close();
            }
        }
        catch (IOException e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logError("An error occurred while writing file '" + in_filePath + "': " + e.getMessage());
        }
        return success;
    }
    /**
     * Writes text to file on internal storage.
     *
     * @author Ian Copland
     *
     * @param in_context - The active context.
     * @param in_filePath - The filename.
     * @param in_fileContents - The text to be written to file.
     *
     * @return Whether or not the file was successfully written.
     */
    public static boolean writeBinaryFile(Context in_context, String in_filePath, String in_fileContents)
    {
        byte[] bytes = StringUtils.stringToUTF8Bytes(in_fileContents);
        return writeBinaryFile(in_context, in_filePath, bytes);
    }
    /**
     * Deletes the given file from internal storage. If the file doesn't exist this will return
     * an error.
     *
     * @author Ian Copland
     *
     * @param in_context - The active context.
     * @param in_filePath - The file path
     *
     * @return Whether or not this was successful.
     */
    public static boolean removeFile(Context in_context, String in_filePath)
    {
        if (doesFileExist(in_context, in_filePath) == false)
        {
            return false;
        }

        return in_context.deleteFile(in_filePath);
    }
    /**
     * Calculates the size of the given file located in internal storage.
     *
     * @author Ian Copland
     *
     * @param in_context - The active context.
     * @param in_filePath - The path to the file which is to have its size calculated.
     *
     * @return The size of the file in bytes.
     */
    public static long getFileSize(Context in_context, String in_filePath)
    {
        File file = in_context.getFileStreamPath(in_filePath);
        return file.length();
    }
}
