/**
 * MediaPlayerActivity.java
 * Chilli Source
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
	//------------------------------------------------------------------------
	/// Private Static Member data
	//------------------------------------------------------------------------
	private static VideoPlayerActivity mActivityInstance = null;
	private static boolean mbInAPK = false;
	private static String mstrFilename = "";
	private static boolean mbCanDismissWithTap = false;
	private static boolean mbHasSubtitles = false;
	private static int mBackgroundColour = 0x00000000;
	//------------------------------------------------------------------------
	/// Private Member data
	//------------------------------------------------------------------------
	private RelativeLayout mViewContainer;
	public RelativeLayout mAspectViewContainer; //View that contains the video sized based on its aspect ratio
	private VideoPlayerView mVideoPlayerView;
	private SubtitlesView mSubtitlesView;
	private int mdwSeekPosition;
	private boolean mbHasFocusChangedEventOccurred = false;
	private boolean mbIsVideoResuming = false;
	
	//------------------------------------------------------------------------
	/// Get Activity
	///
	/// @return return the static activity instance.
	//------------------------------------------------------------------------
    public static synchronized VideoPlayerActivity GetActivity() 
    {
    	return mActivityInstance;
    }
	//------------------------------------------------------------------------
	/// Setup Video
	///
	/// @param whether or not the video is in the APK.
    /// @param the filename of the video.
    /// @param whether or not the video can be dismissed with a tap.
    /// @param whether or not the video has subtitles.
    /// @param Background colour RGBA
	//------------------------------------------------------------------------
    public static synchronized void SetupVideo(boolean inbInAPK, String instrFilename, boolean inbCanDimissWithTap, boolean inbHasSubtitles, 
    										   float fR, float fG, float fB, float fA)
    {
    	mbInAPK = inbInAPK;
    	mstrFilename = instrFilename;
    	mbCanDismissWithTap = inbCanDimissWithTap;
    	mbHasSubtitles = inbHasSubtitles;
    	
    	mBackgroundColour = Color.argb((int)(fA * 255), (int)(fR * 255), (int)(fG * 255), (int)(fB * 255));
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
    		mActivityInstance = this;
    	}
    	
    	//go full screen!
    	requestWindowFeature(Window.FEATURE_NO_TITLE);
    	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
    	
    	//create the view container
    	mViewContainer = new RelativeLayout(this);
    	setContentView(mViewContainer);
    	
    	mViewContainer.setBackgroundColor(mBackgroundColour);
    	mAspectViewContainer = new RelativeLayout(this);
    	mViewContainer.addView(mAspectViewContainer);
    	
    	if (mbHasSubtitles == true)
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
		VideoPlayerNativeInterface mediaPlayerNI = (VideoPlayerNativeInterface)CSApplication.get().getSystem(VideoPlayerNativeInterface.InterfaceID);
		if (mediaPlayerNI != null)
		{
			mediaPlayerNI.OnVideoComplete();
		}
		
    	synchronized(VideoPlayerActivity.class)
    	{
    		mActivityInstance = null;
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
    		mVideoPlayerView = new VideoPlayerView(this, mbInAPK, mstrFilename, mbCanDismissWithTap, mdwSeekPosition);
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
}
