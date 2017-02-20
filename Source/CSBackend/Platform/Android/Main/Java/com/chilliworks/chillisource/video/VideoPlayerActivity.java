/**
 * MediaPlayerActivity.java
 * ChilliSource
 * Created by Ian Copland on 15/08/2012.
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

import com.chilliworks.chillisource.core.CSApplication;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;

//========================================================
/// Media Player Activity
///
/// An Activity for displaying the media player.
//========================================================
public class VideoPlayerActivity extends Activity
{
	private static VideoPlayerActivity s_activityInstance = null;
	private static VideoInfo s_videoInfo = null;

	private RelativeLayout mViewContainer;
	public RelativeLayout mAspectViewContainer; //View that contains the video sized based on its aspect ratio
	private VideoPlayerView mVideoPlayerView;
	private SubtitlesView mSubtitlesView;
	private int mdwSeekPosition;
	private boolean mbHasFocusChangedEventOccurred = false;
	private boolean mbIsVideoResuming = false;

	/**
	 * This is thread safe.
	 *
	 * @author Ian Copland
	 *
	 * @return The active video player activity. Will be null if no activity is active.
	 */
    public static synchronized VideoPlayerActivity GetActivity() 
    {
    	return s_activityInstance;
    }

	/**
	 * Sets the info which will be used to play the video when this activty is started.
	 *
	 * This is thread safe.
	 *
	 * @author Ian Copland
	 *
	 * @param in_inApk - Whether or not the file is inside the Apk.
	 * @param in_filePath - The path to the video file or file which contains the video data.
	 * @param in_fileOffset - The offset into file where the video data starts. Should be -1 if the
	 * file is a video file.
	 * @param in_fileLength - The length of the video data. Should be -1 if the file is a video
	 * file.
	 * @param in_dismissedWithTap - Whether or not the video can be dismissed with a tap.
	 * @param in_hasSubtitles - Whether or not the video has subtitles.
	 * @param in_bgColourR - The red component of the background colour.
	 * @param in_bgColourG - The green component of the background colour.
	 * @param in_bgColourB - The blue component of the background colour.
	 * @param in_bgColourA - The alpha component of the background colour.
	 */
    public static synchronized void setVideoInfo(boolean in_inApk, String in_filePath, int in_fileOffset, int in_fileLength, boolean in_dismissedWithTap, boolean in_hasSubtitles,
    										   float in_bgColourR, float in_bgColourG, float in_bgColourB, float in_bgColourA)
    {
    	assert (s_videoInfo == null) : "Video info is already set!";

		int backgroundColour = Color.argb((int)(in_bgColourR * 255), (int)(in_bgColourG * 255), (int)(in_bgColourB * 255), (int)(in_bgColourA * 255));
		s_videoInfo = new VideoInfo(in_inApk, in_filePath, in_fileOffset, in_fileLength, in_dismissedWithTap, in_hasSubtitles, backgroundColour);
    }
	//------------------------------------------------------------------------
	/// On Create
	///
	/// Creates the media player
	///
	/// @param the saved instance state. This is of no use to us.
	//------------------------------------------------------------------------
    @Override public synchronized void onCreate(Bundle savedInstanceState) 
    {
    	super.onCreate(savedInstanceState);
    	synchronized(VideoPlayerActivity.class)
    	{
    		s_activityInstance = this;
    	}
    	
    	//go full screen!
    	requestWindowFeature(Window.FEATURE_NO_TITLE);
    	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
    	
    	//create the view container
    	mViewContainer = new RelativeLayout(this);
    	setContentView(mViewContainer);
    	
    	mViewContainer.setBackgroundColor(s_videoInfo.m_backgroundColour);
    	mAspectViewContainer = new RelativeLayout(this);
    	mViewContainer.addView(mAspectViewContainer);
    	
    	if (s_videoInfo.m_hasSubtitles == true)
    	{
    		mSubtitlesView = new SubtitlesView(this);
    		mAspectViewContainer.addView(mSubtitlesView);
    	}
    	
    	mdwSeekPosition = 0;
    	mbHasFocusChangedEventOccurred = false;
    	mbIsVideoResuming = false;
    }
    //------------------------------------------------------------------------
  	/// On Start
  	///
  	/// This is called whenever the activity is made visible.
  	//------------------------------------------------------------------------
    @Override public synchronized void onStart()
    {
	    super.onStart();
    }
    //------------------------------------------------------------------------
  	/// On Resume
  	///
  	/// This is called whenever the activity becomes the foreground activity.
  	//------------------------------------------------------------------------
    @Override protected synchronized void onResume() 
    {
    	//If false then this is the part of the initialization of the video-activity and is not resuming from screen lock(or other) so create the player immediately 
    	if(!mbHasFocusChangedEventOccurred)
    	{
	    	//create and add the media player view.
    		ShowVideoPlayer();
    	}
    	else //We are resuming the app from an unlock or other, we should only resume the video once we gain window focus (This is to prevent audio playing on lock-screen)
    	{
    		mbIsVideoResuming = true;
    	}
    	
    	super.onResume();
    }
  	//------------------------------------------------------------------------
  	/// On Stop
  	///
  	/// This is called whenever the activity is no longer in the foreground
  	//------------------------------------------------------------------------
    @Override protected synchronized void onPause() 
    {
    	if (mVideoPlayerView != null)
    	{
    		mdwSeekPosition = mVideoPlayerView.GetTime();
    		mVideoPlayerView.Cleanup();
    		mAspectViewContainer.removeView(mVideoPlayerView);
    		mVideoPlayerView = null;
    	}
    	
        super.onPause();
    }
    //------------------------------------------------------------------------
  	/// On Stop
  	///
  	/// This is called whenever the activity is no longer visible.
  	//------------------------------------------------------------------------
    @Override public synchronized void onStop()
    {
	    super.onStop();
    }
  	//------------------------------------------------------------------------
  	/// On Destroy
  	///
  	/// This is called when the lifecycle of the activty ends.
  	//------------------------------------------------------------------------
    @Override protected synchronized void onDestroy()
    {
		s_videoInfo = null;

		VideoPlayer mediaPlayerNI = (VideoPlayer)CSApplication.get().getSystem(VideoPlayer.INTERFACE_ID);
		if (mediaPlayerNI != null)
		{
			mediaPlayerNI.completeVideo();
		}
		
    	synchronized(VideoPlayerActivity.class)
    	{
    		s_activityInstance = null;
    	}
	    super.onDestroy();
    }
  	//------------------------------------------------------------------------
  	/// onWindowFocusChanged
  	///
  	/// This is called when any activity gains focus.
  	//------------------------------------------------------------------------
    @Override public void onWindowFocusChanged(boolean inbHasFocus)
    {
    	//We want to resume the video here if we are resuming the app and the app has focus
    	if(inbHasFocus && mbIsVideoResuming)
    	{
    		mbIsVideoResuming = false;
    		
    		ShowVideoPlayer();
    	}
    	
    	mbHasFocusChangedEventOccurred = true;
    	
    	super.onWindowFocusChanged(inbHasFocus);
    }
  	//------------------------------------------------------------------------
  	/// ShowVideoPlayer
  	//------------------------------------------------------------------------
    private synchronized void ShowVideoPlayer()
    {
    	//create and add the media player view.
    	if (mVideoPlayerView == null)
    	{
    		mVideoPlayerView = new VideoPlayerView(this, s_videoInfo.m_inApk, s_videoInfo.m_filePath, s_videoInfo.m_fileOffset, s_videoInfo.m_fileLength, s_videoInfo.m_dismissedWithTap, mdwSeekPosition);
    		mAspectViewContainer.addView(mVideoPlayerView);
    		if (mSubtitlesView != null)
    		{
    			mAspectViewContainer.bringChildToFront(mSubtitlesView);
    		}
    	}
    }
  	//------------------------------------------------------------------------
  	/// Get Video Player View
  	///
  	/// @return the video player view.
  	//------------------------------------------------------------------------
    public synchronized VideoPlayerView GetVideoPlayerView()
    {
    	return mVideoPlayerView;
    }
  	//------------------------------------------------------------------------
  	/// Get Subtitles View
  	///
  	/// @return the subtitles view.
  	//------------------------------------------------------------------------
    public synchronized SubtitlesView GetSubtitlesView()
    {
    	return mSubtitlesView;
    }
	//---------------------------------------------------------------
	/// On Back Pressed
    ///
    /// Called whenever the back button is pressed while this activity
    /// is active.
	//---------------------------------------------------------------
	@Override public void onBackPressed()
	{
		mVideoPlayerView.OnBackPressed();
	}
	/**
	 * A container for information on the video which should be played by the activity.
	 *
	 * @author Ian Copland
	 */
	private static class VideoInfo
	{
		public final boolean m_inApk;
		public final String m_filePath;
		public final int m_fileOffset;
		public final int m_fileLength;
		public final boolean m_dismissedWithTap;
		public final boolean m_hasSubtitles;
		public final int m_backgroundColour;

		public VideoInfo(boolean in_inApk, String in_filePath, int in_fileOffset, int in_fileLength, boolean in_dismissedWithTap, boolean in_hasSubtitles, int in_backgroundColour)
		{
			m_inApk = in_inApk;
			m_filePath = in_filePath;
			m_fileOffset = in_fileOffset;
			m_fileLength = in_fileLength;
			m_dismissedWithTap = in_dismissedWithTap;
			m_hasSubtitles = in_hasSubtitles;
			m_backgroundColour = in_backgroundColour;
		}
	}
}
