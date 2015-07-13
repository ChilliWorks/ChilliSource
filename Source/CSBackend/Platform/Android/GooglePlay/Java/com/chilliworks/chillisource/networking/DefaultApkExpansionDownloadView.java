/**
 * DefaultApkExpansionDownloadView.java
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

import android.view.Gravity;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

/**
 * A default, basic implementation of the Apk Expansion Download View. This simply presents a black
 * screen with a download progress bar. If an error occurs, the bar is replaced with error text.
 *
 * If something more sophisticated is required this can be overridden with a custom implementation
 * of ApkExpansionDownloadView.
 *
 * @author Ian Copland
 */
final class DefaultApkExpansionDownloadView extends ApkExpansionDownloadView
{
    private RelativeLayout m_layout = null;
    private ProgressBar m_progressBar = null;
    private TextView m_textView = null;
    private boolean m_hasFailed = false;

    /**
     * Constructor. Declared protected so only related classes can instantiate it.
     *
     * @author Ian Copland
     *
     * @param in_activity - The Apk expansion download activity.
     */
    protected DefaultApkExpansionDownloadView(ApkExpansionDownloadActivity in_activity)
    {
        super(in_activity);
    }
    /**
     * Called when the view is first presented. Initialises the view.
     *
     * @author Ian Copland
     */
    @Override protected void onInit()
    {
        final float PROGRESS_BAR_MARGIN_PERCENTAGE = 0.25f;

        //The following uses the deprecated method: getWidth(). This should be changed to getSize() when the minimum SDK target is changed to API Level 13 or higher.
        @SuppressWarnings("deprecation") float displayWidth = (float)getActivity().getWindowManager().getDefaultDisplay().getWidth();

        int progressBarMargin = (int)Math.round(displayWidth * PROGRESS_BAR_MARGIN_PERCENTAGE);
        RelativeLayout.LayoutParams progressBarLayoutParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
        progressBarLayoutParams.addRule(RelativeLayout.CENTER_IN_PARENT);
        progressBarLayoutParams.setMargins(progressBarMargin, 0, progressBarMargin, 0);

        m_progressBar = new ProgressBar(getActivity(), null, android.R.attr.progressBarStyleHorizontal);
        m_progressBar.setLayoutParams(progressBarLayoutParams);

        final float TEXT_VIEW_MARGIN_PERCENTAGE = 0.1f;
        int textViewMargin = (int)Math.round(displayWidth * TEXT_VIEW_MARGIN_PERCENTAGE);
        RelativeLayout.LayoutParams textViewLayoutParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.MATCH_PARENT, RelativeLayout.LayoutParams.MATCH_PARENT);
        textViewLayoutParams.addRule(RelativeLayout.CENTER_IN_PARENT);
        textViewLayoutParams.setMargins(textViewMargin, 0, textViewMargin, 0);

        m_textView = new TextView(getActivity());
        m_textView.setGravity(Gravity.CENTER);
        m_textView.setLayoutParams(textViewLayoutParams);

        m_layout = new RelativeLayout(getActivity());
        m_layout.addView(m_progressBar);
        getActivity().setContentView(m_layout);

        m_hasFailed = false;
    }
    /**
     * Called whenever the download state changes.
     *
     * @author Ian Copland
     *
     * @param in_state - The new state of the download.
     * @param in_stateDescription - A user presentable description of the current download state.
     * This is typically used to present useful error messages to users.
     */
    @Override protected void onStateChanged(ApkExpansionDownloadState in_state, String in_stateDescription)
    {
        if (m_hasFailed == false && (in_state == ApkExpansionDownloadState.FAILED || in_state == ApkExpansionDownloadState.FAILED_NO_STORAGE))
        {
            m_textView.setText(in_stateDescription);

            m_layout.removeView(m_progressBar);
            m_layout.addView(m_textView);

            m_hasFailed = true;
        }
    }
    /**
     * Called as the download progress updates.
     *
     * @author Ian Copland
     *
     * @param in_progress - The fraction of the download that has been downloaded.
     */
    @Override protected void onProgressUpdated(double in_progress)
    {
        m_progressBar.setProgress((int)(in_progress * 100.0f));
    }
    /**
     * Called when the view is dismissed. Cleans up the view.
     *
     * @author Ian Copland
     */
    @Override protected void onDestroy()
    {
        if (m_hasFailed == false)
        {
            m_layout.removeView(m_progressBar);
        }
        else
        {
            m_layout.removeView(m_textView);
        }

        m_textView = null;
        m_layout = null;
        m_progressBar = null;
    }
}
