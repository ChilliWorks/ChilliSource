/**
 * ApkExpansionContentProvider.java
 * ChilliSource
 * Created by Ian Copland on 23/06/2015.
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

import android.content.ContentProvider;
import android.content.ContentValues;
import android.content.res.AssetFileDescriptor;
import android.database.Cursor;
import android.net.Uri;
import android.os.ParcelFileDescriptor;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Random;

/**
 * //TODO: !?
 *
 * @author Ian Copland
 */
public final class ApkExpansionContentProvider extends ContentProvider
{
    private static final String k_tempDirectory = "_ApkExpansionContentProvider/";

    /**
     * //TODO: !?
     *
     * @return
     */
    @Override public boolean onCreate()
    {
        //TODO: !?

        Logging.logError("[ApkExpansionContentProvider] onCreate()");

        return false;
    }
    /**
     * //TODO: !?
     *
     * @param in_uri
     * @return
     */
    @Override public String getType(Uri in_uri)
    {
        //TODO: !?

        Logging.logError("[ApkExpansionContentProvider] getType('" + in_uri + "')");

        return null;
    }
    /**
     * //TODO: !?
     *
     * @param in_uri
     * @param in_mode
     * @return
     */
    @Override public ParcelFileDescriptor openFile(Uri in_uri, String in_mode) throws FileNotFoundException
    {
        assert (CSApplication.get() != null && CSApplication.get().getActivity() != null) : "Cannot open asset file before the application has been created.";

        if (in_mode != "r")
        {
            throw new FileNotFoundException("This content provider only supports read operations.");
        }

        final String contentPrefix = "content://" + CSApplication.get().getActivity().getPackageName() + ".apkexpansion/";
        final String uriString = in_uri.toString();
        if (uriString.startsWith(contentPrefix) == false)
        {
            throw new FileNotFoundException("The content prefix is incorrect for this content provider.");
        }

        String filePath = uriString.substring(contentPrefix.length());
        String fileName = StringUtils.getFileName(filePath);

        String tempDirectoryPath = FileUtils.getExternalCacheDirectory(CSApplication.get().getActivity().getPackageName()) + k_tempDirectory;
        FileUtils.createDirectory(FileUtils.StorageLocation.k_externalStorage, tempDirectoryPath);

        Random random = new Random();
        String tempFileName = StringUtils.removeExtension(fileName) + "-" + random.nextLong() + "." + StringUtils.getExtension(fileName);
        String absTempFilePath = FileUtils.getExternalStorageDirectory() + tempDirectoryPath + tempFileName;

        if (extractApkExpansionFile(filePath, absTempFilePath) == false)
        {
            throw new FileNotFoundException("Failed to extract temp file '" + tempFileName + "'.");
        }

        //TODO: Handle removal of files after descriptor is closed.
        try
        {
            return ParcelFileDescriptor.open(new File(absTempFilePath), ParcelFileDescriptor.MODE_READ_ONLY);
        }
        catch (FileNotFoundException e)
        {
            throw new FileNotFoundException("Could not read the extracted temp file '" + tempFileName + "'.");
        }
    }
    /**
     * //TODO: !?
     *
     * @param in_uri
     * @param in_projection
     * @param in_selection
     * @param in_selectionArgs
     * @param in_sortOrder
     * @return
     */
    @Override public Cursor query(Uri in_uri, String[] in_projection, String in_selection, String[] in_selectionArgs, String in_sortOrder)
    {
        throw new UnsupportedOperationException("This content provider only supports reading files from the Apk Expansion.");
    }
    /**
     * //TODO: !?
     *
     * @param in_uri
     * @param in_values
     * @return
     */
    @Override public Uri insert(Uri in_uri, ContentValues in_values)
    {
        throw new UnsupportedOperationException("This content provider only supports reading files from the Apk Expansion.");
    }
    /**
     * //TODO: !?
     *
     * @param in_uri
     * @param in_selection
     * @param in_selectionArgs
     * @return
     */
    @Override public int delete(Uri in_uri, String in_selection, String[] in_selectionArgs)
    {
        throw new UnsupportedOperationException("This content provider only supports reading files from the Apk Expansion.");
    }
    /**
     * //TODO: !?
     *
     * @param in_uri
     * @param in_values
     * @param in_selection
     * @param in_selectionArgs
     * @return
     */
    @Override public int update(Uri in_uri, ContentValues in_values, String in_selection, String[] in_selectionArgs)
    {
        throw new UnsupportedOperationException("This content provider only supports reading files from the Apk Expansion.");
    }
    /**
     * //TODO: !?
     *
     * @param in_sourceFilePath
     * @param in_destFilePath
     * @return
     */
    private native boolean extractApkExpansionFile(String in_sourceFilePath, String in_destFilePath);
}
