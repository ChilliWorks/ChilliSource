/**
 * NativeLibraryLoader.java
 * ChilliSource
 * Created by Scott Downie on 28/05/2015.
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
import android.content.pm.PackageManager;
import android.os.Bundle;

/**
 * A static class which provides a means to load the native libraries which are used by this
 * application.
 *
 * @author Ian Copland
 */
public final class NativeLibraryLoader
{
    private static final String ADDITIONAL_LIBRARIES_KEY = "AdditionalSharedLibraries";
    private static final String APPLICATION_LIBRARY = "Application";

    private static boolean s_librariesLoaded = false;

    /**
     * Load the shared Java libraries required by the application. If the libraries are already
     * loaded this will simply do nothing.
     *
     * By default this is just libApplication.so, however an application can add additional
     * libraries by specifying meta-data with the key AdditionalSharedLibraries in
     * AndroidManifest.xml. This is a space delimited list of libraries, and the 'lib' prefix and
     * '.so' suffix should be omitted. For example to load both libLibraryOne.so and
     * libLibraryTwo.so the following would be required:
     *
     * <meta-data android:name="AdditionalSharedLibraries" android:value="LibraryOne LibraryTwo" />
     *
     * All additional libraries will be loaded prior to libApplication.so.
     *
     * This can only be called on the UI thread.
     *
     * @author Ian Copland
     *
     * @param in_activity - Any Activity in the application. This is required to get the meta data.
     */
    public static void load(Activity in_activity)
    {
        assert in_activity != null : "Cannot load libraries with a null activity.";
        assert s_librariesLoaded == false : "Cannot initialise the libraries more than once.";

        s_librariesLoaded = true;

        try
        {
            Bundle bundle = in_activity.getPackageManager().getApplicationInfo(in_activity.getPackageName(), PackageManager.GET_META_DATA).metaData;
            String strAdditionalLibraries = bundle.getString(ADDITIONAL_LIBRARIES_KEY);
            if (strAdditionalLibraries != null)
            {
                String[] astrAdditionalLibraries = strAdditionalLibraries.split(" ");

                for (String strAdditionalLibrary : astrAdditionalLibraries)
                {
                    java.lang.System.loadLibrary(strAdditionalLibrary);
                }
            }
        }
        catch (Exception e)
        {
            Logging.logFatal("Could not load additional libraries!");
        }

        //load the default libraries
        java.lang.System.loadLibrary(APPLICATION_LIBRARY);
    }
}
