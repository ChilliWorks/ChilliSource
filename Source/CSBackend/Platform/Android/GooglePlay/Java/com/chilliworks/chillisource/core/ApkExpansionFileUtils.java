/**
 * ApkExpansion.java
 * ChilliSource
 * Created by Ian Copland on 23/07/2015
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
import java.util.Enumeration;
import java.util.List;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * A collection of convenience methods for reading files from the the Apk expansion.
 *
 * @author Ian Copland
 */
public final class ApkExpansionFileUtils
{
    /**
     * @author Ian Copland
     *
     * @param in_filePath - The path to the file inside the Apk Expansion.
     *
     * @return Whether or not the given file exists in the Apk Expansion.
     */
    public static boolean doesFileExist(String in_filePath)
    {
        try
        {
            ZipFile zipFile = new ZipFile(ApkExpansionInfo.getFilePath());
            try
            {
                Enumeration<? extends ZipEntry> entries = zipFile.entries();
                while (entries.hasMoreElements())
                {
                    ZipEntry zipEntry = entries.nextElement();
                    if (zipEntry.isDirectory() == false)
                    {
                        String zipEntryFilePath = StringUtils.standardiseFilePath(zipEntry.getName());
                        if (in_filePath.equals(zipEntryFilePath) == true)
                        {
                            return true;
                        }
                    }
                }
            }
            finally
            {
                zipFile.close();
            }
        }
        catch (IOException e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logError("An error occurred while trying to check if file '" + in_filePath + "' exists inside the Apk expansion file.");
        }

        return false;
    }
    /**
     * Reads the entire contents of a binary file from the Apk Expansion. The contents are returned
     * as a byte array.
     *
     * @author Ian Copland
     *
     * @param in_filePath - The path to the file inside the Apk Expansion.
     *
     * @return The file contents.
     */
    public static byte[] readBinaryFile(String in_filePath)
    {
        try
        {
            ZipFile zipFile = new ZipFile(ApkExpansionInfo.getFilePath());
            try
            {
                Enumeration<? extends ZipEntry> entries = zipFile.entries();
                while (entries.hasMoreElements())
                {
                    ZipEntry zipEntry = entries.nextElement();
                    if (zipEntry.isDirectory() == false)
                    {
                        String zipEntryFilePath = StringUtils.standardiseFilePath(zipEntry.getName());
                        if (in_filePath.equals(zipEntryFilePath) == true)
                        {
                            return readZipEntry(zipFile, zipEntry);
                        }
                    }
                }
            }
            finally
            {
                zipFile.close();
            }
        }
        catch (IOException e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logError("An error occurred while trying to check if file '" + in_filePath + "' exists inside the Apk expansion file.");
        }

        return null;
    }
    /**
     * Reads the entire contents of a text file from the Apk expansion.
     *
     * @author Ian Copland
     *
     * @param in_filePath - The path to the file inside the Apk Expansion.
     *
     * @return The file contents.
     */
    public static String readTextFile(String in_filePath)
    {
        byte[] contents = readBinaryFile(in_filePath);
        if (contents == null)
        {
            return "";
        }
        return StringUtils.utf8BytesToString(contents);
    }
    /**
     * Calculates the size of the given file located in the Apk Expansion
     *
     * @author Ian Copland
     *
     * @param in_filePath - The path to the file inside the Apk Expansion.
     *
     * @return The size of the file in bytes.
     */
    public static long getFileSize(String in_filePath)
    {
        try
        {
            ZipFile zipFile = new ZipFile(ApkExpansionInfo.getFilePath());
            try
            {
                Enumeration<? extends ZipEntry> entries = zipFile.entries();
                while (entries.hasMoreElements())
                {
                    ZipEntry zipEntry = entries.nextElement();
                    if (zipEntry.isDirectory() == false)
                    {
                        return zipEntry.getSize();
                    }
                }
            }
            finally
            {
                zipFile.close();
            }
        }
        catch (IOException e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logError("An error occurred while trying to check if file '" + in_filePath + "' exists inside the Apk expansion file.");
        }

        return 0;
    }
    /**
     * Copies the requested file from the Apk Expansion file to external storage.
     *
     * @author Ian Copland
     *
     * @param in_sourceFilePath - The source file path inside the Apk Expansion
     * @param in_destFilePath - The destination file path in external storage.
     *
     * @return Whether or not the copy was successful.
     */
    public static boolean copyFile(String in_sourceFilePath, String in_destFilePath)
    {
        if (doesFileExist(in_sourceFilePath) == false)
        {
            return false;
        }

        byte[] fileContents = readBinaryFile(in_sourceFilePath);
        if (fileContents == null)
        {
            return false;
        }

        return FileUtils.writeBinaryFile(in_destFilePath, fileContents);
    }
    /**
     * Reads the contents of a the file described by the given zip entry from the given zip file.
     *
     * @author Ian Copland
     *
     * @param in_zipFile - The zip file.
     * @param in_zipEntry - The zip entry.
     *
     * @throws IOException - This is thrown if the operation fails for any reason.
     */
    private static byte[] readZipEntry(ZipFile in_zipFile, ZipEntry in_zipEntry) throws IOException
    {
        final int k_bufferSize = 16 * 1024;
        DynamicByteBuffer dynamicByteBuffer = new DynamicByteBuffer(k_bufferSize);

        BufferedInputStream stream = new BufferedInputStream(in_zipFile.getInputStream(in_zipEntry));
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
