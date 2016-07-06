/**
 * VideoPlayer.java
 * ChilliSource
 * Created by Ian Copland on 10/08/2012.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2012 Tag Games Limited
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

package com.chilliworks.chillisource.video;

import android.content.Intent;

import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.InterfaceId;
import com.chilliworks.chillisource.core.System;

/**
 * A system which is used to present full screen video from native. This launches a new activity
 * over the OpenGL activity and uses this to play the video in a new view.
 *
 * @author Ian Copland
 */
public class VideoPlayer extends System
{
    public static InterfaceId INTERFACE_ID = new InterfaceId();

	private boolean m_updateSubtitlesEvent = false;

    /**
     * Allows querying of whether or not the system implements the interface described by the
     * given interface id.
     *
     * @author Ian Copland
     *
     * @param in_interfaceId - The interface id to check
     *
     * @return Whether the system implements the given interface
     */
    @Override public boolean isA(InterfaceId in_interfaceId)
    {
        return (in_interfaceId == INTERFACE_ID);
    }
	/**
	 * Presents a video. Videos can be played from file in external storage, from inside the APK
	 * or from inside another file. To play a file from within another (i.e from within a zip other
	 * than the APK) the file offset and length should be set. If playing from a zip then the video
	 * file must not be compressed.
	 *
	 * This is thread-safe.
	 *
	 * @param in_inApk - Whether or not the file is inside the APK.
	 * @param in_filePath - The path to the video file or the file which contains the video file.
	 * @param in_fileOffset - The offset into the file that the video file is located at. This should
	 * be -1 if the file is a video file.
	 * @param in_fileLength - The length of the video file. This should be -1 if the file is a video
	 * file.
	 * @param in_dismissWhenTapped - Whether or not the video activity can be dismissed by a tap.
	 * @param in_hasSubtitles - Whether or not the video has subtitles.
	 * @param in_bgColourR - the red component of the background colour.
	 * @param in_bgColourG - the green component of the background colour.
	 * @param in_bgColourB - the blue component of the background colour.
	 * @param in_bgColourA - the alpha component of the background colour.
	 */
	public void present(boolean in_inApk, String in_filePath, int in_fileOffset, int in_fileLength, boolean in_dismissWhenTapped, boolean in_hasSubtitles,
						float in_bgColourR, float in_bgColourG, float in_bgColourB, float in_bgColourA)
	{
		VideoPlayerActivity.setVideoInfo(in_inApk, in_filePath, in_fileOffset, in_fileLength, in_dismissWhenTapped, in_hasSubtitles, in_bgColourR, in_bgColourG, in_bgColourB, in_bgColourA);
		Intent mediaPlayerIntent = new Intent(CSApplication.get().getActivityContext(), VideoPlayerActivity.class);
		CSApplication.get().getActivityContext().startActivity(mediaPlayerIntent);
	}
	/**
	 * This is thread-safe.
	 *
	 * @author Ian Copland
	 *
	 * @return The current playback position through the video, in seconds.
	 */
	public float getPlaybackPosition()
	{
		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			VideoPlayerView view = activity.GetVideoPlayerView();
			if (view != null)
			{
				return ((float)view.GetTime()) / 1000.0f;
			}
		}
		return 0.0f;
	}
	/**
	 * Creates a new subtitle which will be displayed over the playing video.
	 *
	 * This must be called during the "Update Subtitles" event.
	 *
	 * @author Ian Copland
	 *
	 * @param in_text - The subtitle text.
	 * @param in_fontName - The name of the font to be used.
	 * @param in_fontSize - The font size.
	 * @param in_alignment - The text alignment.
	 * @param in_x - The X position of the rect the text will be located in.
	 * @param in_y - The Y position of the rect the text will be located in.
	 * @param in_width - The width of the rect the text will be located in.
	 * @param in_height - The height of the rect the text will be located in.
	 *
	 * @return The Id of the created subtitle.
	 */
	public long createSubtitle(String in_text, String in_fontName, int in_fontSize, String in_alignment, float in_x, float in_y, float in_width, float in_height)
	{
		assert (m_updateSubtitlesEvent == true) : "Cannot only be called during the Update Subtitles event.";

		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			SubtitlesView view = activity.GetSubtitlesView();
			if (view != null)
			{
				return view.CreateSubtitle(in_text, in_fontName, in_fontSize, in_alignment, in_x, in_y, in_width, in_height);
			}
		}
		return -1;
	}
	/**
	 * Sets the colour of the subtitle with the given Id.
	 *
	 * This must be called during the "Update Subtitles" event.
	 *
	 * @author Ian Copland
	 *
	 * @param in_subtitleId - The Id of the subtitle object.
	 * @param in_red - The red component of the subtitle colour.
	 * @param in_green - The red component of the subtitle colour.
	 * @param in_blue - The red component of the subtitle colour.
	 * @param in_alpha - The red component of the subtitle colour.
	 */
	public void setSubtitleColour(long in_subtitleId, float in_red, float in_green, float in_blue, float in_alpha)
	{
		assert (m_updateSubtitlesEvent == true) : "Cannot only be called during the Update Subtitles event.";

		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			SubtitlesView view = activity.GetSubtitlesView();
			if (view != null)
			{
				view.SetSubtitleColour(in_subtitleId, in_red, in_green, in_blue, in_alpha);
			}
		}
	}
	/**
	 * Removes the subtitle with the given Id.
	 *
	 * This must be called during the "Update Subtitles" event.
	 *
	 * @param in_subtitleId - The Id of the subtitle object which should be removed.
	 */
	public void removeSubtitle(long in_subtitleId)
	{
		assert (m_updateSubtitlesEvent == true) : "Cannot only be called during the Update Subtitles event.";

		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			SubtitlesView view = activity.GetSubtitlesView();
			if (view != null)
			{
				view.RemoveSubtitle(in_subtitleId);
			}
		}
	}
	/**
	 * Called by the Video Player Activity to send the update subtitles event to the native side of
	 * the engine.
	 *
	 * This must be called on the UI thread.
	 *
	 * @author Ian Copland
	 */
	protected void updateSubtitles()
	{
		m_updateSubtitlesEvent = true;
		onUpdateSubtitles();
		m_updateSubtitlesEvent = false;
	}
	/**
	 * Called by the Video Player Activity on completion to signal the native side of the engine.
	 *
	 * This must be called on the UI thread.
	 *
	 * @author Ian Copland
	 */
	protected void completeVideo()
	{
		onVideoComplete();
	}
	/**
	 * Notifies the native side of the engine that it should update subtitles.
	 *
	 * @author Ian Copland
	 */
	private native void onUpdateSubtitles();
	/**
	 * Notifies the native side of the engine that video playback has completed.
	 *
	 * @author Ian Copland
	 */
	private native void onVideoComplete();
}
