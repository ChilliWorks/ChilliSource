/**
 * ApkExpansionDownloadViewFactory.java
 * ChilliSource
 * Created by Ian Copland on 09/07/2015.
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

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;

import com.chilliworks.chillisource.core.ExceptionUtils;
import com.chilliworks.chillisource.core.Logging;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

/**
 * Provides a means to create new instances of the ApkExpansionDownloadView. By default the
 * DefaultApKExpansionDownloadView is created, but the user can override this to provide thier
 * own custom download view.
 *
 * @author Ian Copland
 */
final class ApkExpansionDownloadViewFactory
{
    private static final String CUSTOM_APK_EXPANSION_DOWNLOADER_VIEW_KEY = "apkExpansionDownloadView";

    /**
     * Creates a new instance of the ApkExpansionDownloadView. If the AndroidManifest contains
     * meta data for the key described by CUSTOM_APK_EXPANSION_DOWNLOADER_VIEW_KEY then an instance
     * of this class is created. Otherwise, a new instance of DefaultApkExpansionDownloadView is
     * created. Note that the class described in meta data must be fully qualified.
     *
     * @author Ian Copland
     *
     * @param in_activity - The current Apk Expansion download activity.
     *
     * @return The new instance of ApkExpansionDownloadView.
     */
    protected static ApkExpansionDownloadView createView(ApkExpansionDownloadActivity in_activity)
    {
        //Try to create a custom view.
        String customViewName = "";
        try
        {
            ApplicationInfo applicationInfo = in_activity.getPackageManager().getApplicationInfo(in_activity.getPackageName(), PackageManager.GET_META_DATA);
            customViewName = applicationInfo.metaData.getString(CUSTOM_APK_EXPANSION_DOWNLOADER_VIEW_KEY);
            if (customViewName != null && customViewName.isEmpty() == false)
            {
                return createView(customViewName, in_activity);
            }
        }
        catch (PackageManager.NameNotFoundException e)
        {
            //If this is thrown, we should just return the default view, so do nothing.
        }
        catch (ClassNotFoundException e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logFatal("Could not create custom Apk Expansion Download View '" + customViewName + "' because the class doesn't exist.");
        }
        catch (Exception e)
        {
            Logging.logVerbose(ExceptionUtils.convertToString(e));
            Logging.logFatal("Could not create custom Apk Expansion Download View '" + customViewName + "' becuase an exception occured during instantiation.");
        }

        //failing that, simply return the default view.
        return new DefaultApkExpansionDownloadView(in_activity);
    }
    /**
     * Creates an instance of the ApkExpansionDownloadView with the given name using reflection.
     * The created class must have a constructor which takes the ApkExpansionDownloadActivity as
     * a single parameter.
     *
     * @author Ian Copland
     *
     * @param in_customViewName - The name of the custom view.
     * @param in_activity - The current Apk Expansion download activity.
     *
     * @return The new download view.
     *
     * @throws ClassNotFoundException - Will be thrown if the class doesn't exist.
     * @throws IllegalAccessException - Thrown if the constructor cannot be accessed.
     * @throws InvocationTargetException - Thrown if the constructor throws and exception.
     * @throws InstantiationException - If the class cannot be instantiated because it's abstract
     * or there is no suitable constructor.
     */
    private static ApkExpansionDownloadView createView(String in_customViewName, ApkExpansionDownloadActivity in_activity) throws ClassNotFoundException, IllegalAccessException, InvocationTargetException, InstantiationException
    {
        Class<?> customViewClass = Class.forName(in_customViewName);

        //find an appropriate constructor.
        Constructor[] constructors = customViewClass.getDeclaredConstructors();
        for (int i = 0; i < constructors.length; i++)
        {
            if (constructors[i].getParameterTypes().length == 1 && constructors[i].getParameterTypes()[0].equals(ApkExpansionDownloadActivity.class))
            {
                return (ApkExpansionDownloadView)constructors[i].newInstance(in_activity);
            }
        }

        throw new InstantiationException("Couldn't create class '" + in_customViewName + "' as it doesn't have a constructor which takes ApkExpansionDownloadActivity as its only argument.");
    }
}
