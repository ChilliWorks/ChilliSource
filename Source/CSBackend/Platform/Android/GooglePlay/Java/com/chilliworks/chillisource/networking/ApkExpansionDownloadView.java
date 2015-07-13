/**
 * ApkExpansionDownloadView.java
 * ChilliSource
 * Created by Ian Copland on 28/05/2015.
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

/**
 * A base class for the Apk Expansion Download View. This receives events from the download
 * activity which can be used to render a download screen.
 *
 * @author Ian Copland
 */
public abstract class ApkExpansionDownloadView
{
    private ApkExpansionDownloadActivity m_activity = null;

    /**
     * @author Ian Copland
     *
     * @param in_activity - The Apk expansion download activity.
     */
    protected ApkExpansionDownloadView(ApkExpansionDownloadActivity in_activity)
    {
        m_activity = in_activity;
        assert m_activity != null : "Must supply a non-null Apk Expansion Download Activity.";
    }
    /**
     * @author Ian Copland
     *
     * @return The Apk expansion download activity.
     */
    protected ApkExpansionDownloadActivity getActivity()
    {
        return m_activity;
    }
    /**
     * Called when the view is first presented. This should perform any initialisation of the
     * view.
     *
     * @author Ian Copland
     */
    protected abstract void onInit();
    /**
     * Called whenever the download state changes. The implementing class can use this to
     * update the view to reflect the state of the download, i.e display failure, or a pause
     * screen.
     *
     * @author Ian Copland
     *
     * @param in_state - The new state of the download.
     * @param in_stateDescription - A user presentable description of the current download state.
     * This is typically used to present useful error messages to users.
     */
    protected abstract void onStateChanged(ApkExpansionDownloadState in_state, String in_stateDescription);
    /**
     * Called as the download progress updates.
     *
     * @author Ian Copland
     *
     * @param in_progress - The fraction of the download that has been downloaded.
     */
    protected abstract void onProgressUpdated(double in_progress);
    /**
     * Called when the view is dismissed. This should perform any cleanup required by the view.
     *
     * @author Ian Copland
     */
    protected abstract void onDestroy();
}
