/**
 * VideoPlayerNativeInterface.java
 * Chilli Source
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
import com.chilliworks.chillisource.core.INativeInterface;
import com.chilliworks.chillisource.core.InterfaceIDType;
import com.chilliworks.chillisource.video.SubtitlesView;
import com.chilliworks.chillisource.video.VideoPlayerActivity;
import com.chilliworks.chillisource.video.VideoPlayerView;

//=============================================================
/// Video Player Native Interface
///
/// Native interface for playing videos.
//=============================================================
public class VideoPlayerNativeInterface extends INativeInterface
{
	//--------------------------------------------------------------
	/// Static Member Data
	//--------------------------------------------------------------
	public static InterfaceIDType InterfaceID = new InterfaceIDType("CVideoPlayerNativeInterface");
	//--------------------------------------------------------------
	/// Constructor
	//--------------------------------------------------------------
	public VideoPlayerNativeInterface()
	{
	}
	//--------------------------------------------------------------
	/// Is A
	///
	/// @return whether or not this implements the given interface.
	//--------------------------------------------------------------
	@Override public boolean IsA(InterfaceIDType inInterfaceType) 
	{
		return (inInterfaceType.Equals(InterfaceID));
	}
	//--------------------------------------------------------------
	/// Present
	///
	/// Presents the video
	/// 
	/// @param whether or not the video is in the APK.
	/// @param the filename of the video.
	//--------------------------------------------------------------
	public void Present(boolean inbInAPK, String instrFilename, boolean inbCanDimissWithTap, boolean inbHasSubtitles, float fR, float fB, float fG, float fA) 
	{
		VideoPlayerActivity.SetupVideo(inbInAPK, instrFilename, inbCanDimissWithTap, inbHasSubtitles, fR, fG, fB, fA);
		Intent mediaPlayerIntent = new Intent(CSApplication.get().getActivityContext(), VideoPlayerActivity.class);
		CSApplication.get().getActivityContext().startActivity(mediaPlayerIntent); 
	}
	//--------------------------------------------------------------
	/// Get Time
	///
	/// @return the current position through the video.
	//--------------------------------------------------------------
	public float GetTime() 
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
	//--------------------------------------------------------------
	/// Called when the video is complete.
	///
	/// @author Ian Copland
	//--------------------------------------------------------------
	public native void OnVideoComplete();
	//--------------------------------------------------------------
	/// Called every frame in a video that has subtitles.
	///
	/// @author Ian Copland
	//--------------------------------------------------------------
	public native void OnUpdateSubtitles();
	//--------------------------------------------------------------
	/// Create Subtitle
	///
	/// Creates a new subtitle to be displayed over the video.
	///
	/// @param The subtitle text.
	/// @param The name of the font to be used.
	/// @param The font size.
	/// @param The text alignment.
	/// @param The X position.
	/// @param the Y position.
	/// @param The width.
	/// @param The height.
	//--------------------------------------------------------------
	public long CreateSubtitle(String instrText, String instrFontName, int indwFontSize, String instrAlignment, float infX, float infY, float infWidth, float infHeight) 
	{
		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			SubtitlesView view = activity.GetSubtitlesView();
			if (view != null)
			{
				return view.CreateSubtitle(instrText, instrFontName, indwFontSize, instrAlignment, infX, infY, infWidth, infHeight);
			}
		}
		return -1;
	}
	//--------------------------------------------------------------
	/// Set Subtitle Colour
	///
	/// Changes the colour of a currently active subtitle.
	///
	/// @param The subtitle object.
	/// @param Red.
	/// @param Green.
	/// @param Blue.
	/// @param Alpha.
	//--------------------------------------------------------------
	public void SetSubtitleColour(long inlwSubtitleID, float infRed, float infGreen, float infBlue, float infAlpha) 
	{
		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			SubtitlesView view = activity.GetSubtitlesView();
			if (view != null)
			{
				view.SetSubtitleColour(inlwSubtitleID, infRed, infGreen, infBlue, infAlpha);
			}
		}
	}
	//--------------------------------------------------------------
	/// Remove Subtitle
	///
	/// Removes the given subtitle from the video.
	///
	/// @param The subtitle object.
	//--------------------------------------------------------------
	public void RemoveSubtitle(long inlwSubtitleID) 
	{
		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			SubtitlesView view = activity.GetSubtitlesView();
			if (view != null)
			{
				view.RemoveSubtitle(inlwSubtitleID);
			}
		}
	}
}
