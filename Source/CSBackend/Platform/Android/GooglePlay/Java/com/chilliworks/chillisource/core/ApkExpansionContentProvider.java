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
import android.database.Cursor;
import android.net.Uri;
import android.os.Handler;
import android.os.ParcelFileDescriptor;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Random;

/**
 * A content provider for accessing assets within the main Apk Expansion OBB file. The file will
 * first be extracted to a temporary location then deleted again when the relevant file descriptor
 * is closed. This is pretty inefficient, so it should only be used where absolutely necessary.
 *
 * @author Ian Copland
 */
public final class ApkExpansionContentProvider extends ContentProvider
{
    private static final String TEMP_DIRECTORY = "_ApkExpansionContentProvider/";

    /**
     * @author Ian Copland
     *
     * @return The prefix which all paths that this content provider relates to should have.
     */
    public static String getContentPathPrefix()
    {
        assert (CSApplication.get() != null && CSApplication.get().getActivity() != null) : "Cannot get content path prefix before the application has been created.";

        return "content://" + CSApplication.get().getApplicationId() + ".apkexpansioncontentprovider/";
    }
    /**
     * Called when the provider is first created.
     *
     * @author Ian Copland
     *
     * @return Whether the provider was successfully loaded. Always true in this case.
     */
    @Override public boolean onCreate()
    {
        return true;
    }
    /**
     * Returns the MIME type for the given URL.
     *
     * @author Ian Copland
     *
     * @param in_uri - The URI which should have its MIME type checked.
     *
     * @return The MIME Type. Currently this always returns null.
     */
    @Override public String getType(Uri in_uri)
    {
        return null;
    }
    /**
     * Opens a descriptor for the file with the given URI. As this provides access to the contents
     * of a zip file, the file is first extracted to a temporary location and it is this file that
     * the descriptor describes. As the file is extracted, this is pretty inefficient and other
     * methods should be preferred.
     *
     * @author Ian Copland
     *
     * @param in_uri - The URI to the requested file.
     * @param in_mode - The file mode. Must be 'r'.
     *
     * @return The file descriptor. Will never be null.
     *
     * @throws FileNotFoundException - This is thrown if the file doesn't exist, the content prefix
     * is incorrect or the read mode is anything other than 'r'.
     */
    @Override public ParcelFileDescriptor openFile(Uri in_uri, String in_mode) throws FileNotFoundException
    {
        assert (CSApplication.get() != null && CSApplication.get().getActivity() != null) : "Cannot open asset file before the application has been created.";

        if (in_mode.equals("r") == false)
        {
            throw new FileNotFoundException("This content provider only supports read operations.");
        }

        String contentPathPrefix = getContentPathPrefix();

        String uriString = in_uri.toString();
        if (uriString.startsWith(contentPathPrefix) == false)
        {
            throw new FileNotFoundException("The content prefix is incorrect for this content provider.");
        }

        String filePath = uriString.substring(contentPathPrefix.length());
        String fileName = StringUtils.getFileName(filePath);

        String tempDirectoryPath = FileUtils.getExternalCacheDirectory(CSApplication.get().getActivity().getPackageName()) + TEMP_DIRECTORY;
        FileUtils.createDirectory(tempDirectoryPath);

        Random random = new Random();
        String tempFileName = StringUtils.removeExtension(fileName) + "-" + random.nextLong() + "." + StringUtils.getExtension(fileName);
        final String tempFilePath = tempDirectoryPath + tempFileName;

        if (ApkExpansionFileUtils.copyFile(filePath, tempFilePath) == false)
        {
            throw new FileNotFoundException("Failed to extract temp file '" + tempFileName + "'.");
        }

        try
        {
            Handler uiThreadHandler = new Handler(CSApplication.get().getActivity().getMainLooper());
            return new ListenableParcelFileDescriptor(new File(tempFilePath), ParcelFileDescriptor.MODE_READ_ONLY, new ListenableParcelFileDescriptor.OnClose()
            {
                @Override public void onClosed()
                {
                    FileUtils.removeFile(tempFilePath);
                }
            });
        }
        catch (Exception e)
        {
            FileUtils.removeFile(tempDirectoryPath);
            throw new FileNotFoundException("Could not read the extracted temp file '" + tempFileName + "'.");
        }
    }
    /**
     * Unsupported by this provider. Will throw an UnsupportedOperationException if called.
     *
     * @author Ian Copland
     *
     * @param in_uri - The URI
     * @param in_projection - The project
     * @param in_selection - The selection
     * @param in_selectionArgs - The selection arguments
     * @param in_sortOrder - The sort order
     *
     * @return Nothing will be returned as this always throws
     */
    @Override public Cursor query(Uri in_uri, String[] in_projection, String in_selection, String[] in_selectionArgs, String in_sortOrder)
    {
        throw new UnsupportedOperationException("This content provider only supports reading files from the Apk Expansion.");
    }
    /**
     * Unsupported by this provider. Will throw an UnsupportedOperationException if called.
     *
     * @author Ian Copland
     *
     * @param in_uri - The URI
     * @param in_values - The values
     *
     * @return Nothing will be returned as this always throws.
     */
    @Override public Uri insert(Uri in_uri, ContentValues in_values)
    {
        throw new UnsupportedOperationException("This content provider only supports reading files from the Apk Expansion.");
    }
    /**
     * Unsupported by this provider. Will throw an UnsupportedOperationException if called.
     *
     * @author Ian Copland
     *
     * @param in_uri - The URI
     * @param in_selection - The selection
     * @param in_selectionArgs - The selection arguments
     *
     * @return Nothing will be returned as this always throws.
     */
    @Override public int delete(Uri in_uri, String in_selection, String[] in_selectionArgs)
    {
        throw new UnsupportedOperationException("This content provider only supports reading files from the Apk Expansion.");
    }
    /**
     * Unsupported by this provider. Will throw an UnsupportedOperationException if called.
     *
     * @author Ian Copland
     *
     * @param in_uri - The URI
     * @param in_values - The values
     * @param in_selection - The selection
     * @param in_selectionArgs - The selection arguments
     *
     * @return Nothing will be returned as this always throws.
     */
    @Override public int update(Uri in_uri, ContentValues in_values, String in_selection, String[] in_selectionArgs)
    {
        throw new UnsupportedOperationException("This content provider only supports reading files from the Apk Expansion.");
    }
}
