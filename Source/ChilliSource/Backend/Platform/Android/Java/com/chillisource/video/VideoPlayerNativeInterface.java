/*
 *  CVideoPlayerNativeInterface.java
 *  moFlow
 *
 *  Created by Ian Copland on 10/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.video;

import android.content.Intent;

import com.chillisource.core.InterfaceIDType;
import com.chillisource.core.INativeInterface;
import com.chillisource.video.SubtitlesView;
import com.chillisource.video.VideoPlayerActivity;
import com.chillisource.video.VideoPlayerView;

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
		Intent mediaPlayerIntent = new Intent(mActivity, VideoPlayerActivity.class);
		mActivity.startActivity(mediaPlayerIntent); 
	}
	//--------------------------------------------------------------
	/// Is Playing
	///
	/// @return whether or not there is currently a video playing.
	//--------------------------------------------------------------
	public boolean IsPlaying() 
	{
		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			VideoPlayerView view = activity.GetVideoPlayerView();
			if (view != null)
			{
				return view.IsPlaying();
			}
		}
		return false;
	}
	//--------------------------------------------------------------
	/// Get Duration
	///
	/// @return the total length of the currently running video.
	//--------------------------------------------------------------
	public float GetDuration() 
	{
		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			VideoPlayerView view = activity.GetVideoPlayerView();
			if (view != null)
			{
				return ((float)view.GetDuration()) / 1000.0f;
			}
		}
		return 0.0f;
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
	/// Dismiss
	///
	/// Stops the current running video from playing.
	//--------------------------------------------------------------
	public void Dismiss() 
	{
		VideoPlayerActivity activity = VideoPlayerActivity.GetActivity();
		if (activity != null)
		{
			VideoPlayerView view = activity.GetVideoPlayerView();
			if (view != null)
			{
				view.Dismiss();
			}
		}
	}
	//--------------------------------------------------------------
	/// Dismissed
	///
	/// Called when the video is dismissed.
	//--------------------------------------------------------------
	public native void Dismissed();
	//--------------------------------------------------------------
	/// Stopped
	///
	/// Called when the video stops.
	//--------------------------------------------------------------
	public native void Stopped();
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
	//--------------------------------------------------------------
	/// Update Subtitles
	///
	/// Called every frame in a video that has subtitles.
	//--------------------------------------------------------------
	public native void UpdateSubtitles();
}
