/*
 *  CMediaPlayerView.java
 *  moFlow
 *
 *  Created by Ian Copland on 15/08/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

package com.chillisource.video;
import com.chillisource.video.VideoPlayerNativeInterface;
import com.chillisource.core.CSApplication;
import com.chillisource.core.Logging;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.graphics.Point;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.view.Display;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

//========================================================
/// Media Player View
///
/// A view for displaying the media player.
//========================================================
public class VideoPlayerView extends SurfaceView implements OnPreparedListener, OnErrorListener, OnCompletionListener, SurfaceHolder.Callback
{	
	//------------------------------------------------------------------------
	/// Constants
	//------------------------------------------------------------------------
	private long kqwTapLengthInMS = 150;
	//------------------------------------------------------------------------
	/// Private Member Data
	//------------------------------------------------------------------------
	private boolean mbInAPK;
	private String mstrFilename;
	private boolean mbCanDismissWithTap;
	private MediaPlayer mMediaPlayer;
	private int mdwSeekPosition;
	private VideoPlayerActivity mActivity;
	private long mqwTapTime;
	//------------------------------------------------------------------------
	/// Constructor
	//------------------------------------------------------------------------
	public VideoPlayerView(Context inContext, boolean inbInAPK, String instrFilename, boolean inbCanDismissWithTap, int indwSeekPosition) 
	{
		super(inContext);
		mActivity = VideoPlayerActivity.GetActivity();

		mbInAPK = inbInAPK;
		mstrFilename = instrFilename;
		mbCanDismissWithTap = inbCanDismissWithTap;
		mdwSeekPosition = indwSeekPosition;
		mqwTapTime = 0;
		
    	//setup the holder
		getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		getHolder().addCallback(this);
	}
	//--------------------------------------------------------------
	/// Is Playing
	///
	/// @return whether or not there is currently a video playing.
	//--------------------------------------------------------------
	public synchronized boolean IsPlaying() 
	{
		if (mMediaPlayer != null)
		{
			return mMediaPlayer.isPlaying();
		}
		return false;
	}
	//--------------------------------------------------------------
	/// Get Duration
	///
	/// @return the total length of the currently running video.
	//--------------------------------------------------------------
	public synchronized int GetDuration() 
	{
		if (mMediaPlayer != null)
		{
			return mMediaPlayer.getDuration();
		}
		return 0;
	}
	//--------------------------------------------------------------
	/// Get Time
	///
	/// @return the current position through the video.
	//--------------------------------------------------------------
	public synchronized int GetTime() 
	{
		if (mMediaPlayer != null)
		{
			return mMediaPlayer.getCurrentPosition();
		}
		return 0;
	}
	//--------------------------------------------------------------
	/// Dismiss
	///
	/// Stops the current running video from playing.
	//--------------------------------------------------------------
	public void Dismiss() 
	{
		//create a task for dismissing this on the UI thread.
		Runnable task = new Runnable()
		{
			@Override public void run() 
			{
				VideoPlayerNativeInterface mediaPlayerNI = (VideoPlayerNativeInterface)CSApplication.get().getSystem(VideoPlayerNativeInterface.InterfaceID);
				if (mediaPlayerNI != null)
				{
					mediaPlayerNI.Dismissed();
				}
				onCompletion(mMediaPlayer);
				
				synchronized(this)
				{
					notifyAll();
				}
			}
		};
		
		CSApplication.get().scheduleUIThreadTask(task);
		
		//wait for this task to finish
		try
		{
			synchronized(task)
			{
				task.wait();
			}
		}
		catch(Exception inE)
		{
			
		}
	}
	//--------------------------------------------------------------
	/// Prepare Media Player
	///
	/// Prepares the media player for use.
	//--------------------------------------------------------------
	private synchronized void PrepareVideoPlayer() 
	{
		if (mMediaPlayer == null)
		{
			try
			{	
				//setup the media player
				mMediaPlayer = new MediaPlayer();
				mMediaPlayer.setOnPreparedListener(this);
				mMediaPlayer.setOnErrorListener(this);
				mMediaPlayer.setOnCompletionListener(this);
				mMediaPlayer.setDisplay(getHolder());
				mMediaPlayer.setScreenOnWhilePlaying(true);
				mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
				
				//set the data source
				if (mbInAPK == true)
				{
					AssetFileDescriptor fileDesc = mActivity.getAssets().openFd(mstrFilename);
					mMediaPlayer.setDataSource(fileDesc.getFileDescriptor(), fileDesc.getStartOffset(), fileDesc.getLength());
				}
				else
				{
					mMediaPlayer.setDataSource(mstrFilename);
				}
				
				//prepare the media player asynchronously
				mMediaPlayer.prepareAsync();
			}
			catch (Exception e)
			{
				Logging.logError("Error trying to open video file: " + mstrFilename);
				onError(mMediaPlayer, 0, 0);
			}
		}
	}	
	//--------------------------------------------------------------
	/// Cleanup Video
	///
	/// Cleans up the video, releasing everything and setting back
	/// to initial state.
	//--------------------------------------------------------------
	public synchronized void Cleanup() 
	{
		//clean up the video
		if (mMediaPlayer != null)
		{
			if (mMediaPlayer.isPlaying() == true)
			{
				mMediaPlayer.stop();
			}
			mMediaPlayer.release();
			mMediaPlayer = null;
		}
	}
	//--------------------------------------------------------------
	/// surface Created
	///
	/// Called when the surface is created. Prepares the video for 
	/// displaying in a background thread. Once the video has finished 
	/// loading it will present the video and calls the OnVideoPrepared.
	/// 
	/// @param the created holder.
	//--------------------------------------------------------------
	@Override public synchronized void surfaceCreated(SurfaceHolder inHolder) 
	{
		if (getHolder() == inHolder)
		{
			PrepareVideoPlayer();
		}
	}
	//--------------------------------------------------------------
	/// surface Changed
	///
	/// called whenever the surface changes.
	/// 
	/// @param the holder.
	/// @param the format.
	/// @param the width.
	/// @param the height.
	//--------------------------------------------------------------
	@Override public synchronized void surfaceChanged(SurfaceHolder inHolder, int indwFormat, int indwWidth, int indwHeight) 
	{
	}
	//--------------------------------------------------------------
	/// surface Destroyed
	///
	/// called whenever the surface is destroyed.
	/// 
	/// @param the holder.
	//--------------------------------------------------------------
	@Override public synchronized void surfaceDestroyed(SurfaceHolder inHolder) 
	{
	}
	//--------------------------------------------------------------
	/// on Prepared
	///
	/// Called once the video is prepared. This will start the video
	/// playing and call OnVideoStarted.
	//--------------------------------------------------------------
	@Override public synchronized void onPrepared(MediaPlayer inMediaPlayer) 
	{
		if (mMediaPlayer == inMediaPlayer)
		{	
			//Since the view were placing the video onto is normally full screen, we need to fit the 
			//video by width aspect, i.e video is full width of screen but maintains height aspect
			int dwVideoHeight = mMediaPlayer.getVideoHeight();
			int dwVideoWidth = mMediaPlayer.getVideoWidth();
			
			int dwScreenWidth = 0;
			int dwScreenHeight = 0;
			Display display = mActivity.getWindowManager().getDefaultDisplay();
			if (android.os.Build.VERSION.SDK_INT < 13)
			{
				dwScreenWidth = display.getWidth();
				dwScreenHeight = display.getHeight();
			}
			else
			{
				Point size = new Point();
				display.getSize(size);
				dwScreenWidth = size.x;
				dwScreenHeight = size.y;
			}
			
		
			float fVideoAspectRatio = (float)dwVideoHeight / (float)dwVideoWidth;
			
			int dwDiffWidth = Math.abs(dwScreenWidth - dwVideoWidth);
			int dwDiffHeight = Math.abs(dwScreenHeight - dwVideoHeight);
			
			float fVideoViewWidth = 0.0f;
			float fVideoViewHeight = 0.0f;
			
			if(dwDiffWidth > dwDiffHeight)
			{
				float fWidthMultiplier = (float)dwScreenWidth / (float)dwVideoWidth;
				float fHeightMultiplier = fWidthMultiplier * fVideoAspectRatio;
				fVideoViewWidth = dwVideoWidth * fWidthMultiplier;
				fVideoViewHeight = dwVideoWidth * fHeightMultiplier;
			}
			else
			{
				float fHeightMultiplier = (float)dwScreenHeight / (float)dwVideoHeight;
				float fWidthMultiplier = fHeightMultiplier * fVideoAspectRatio;
				fVideoViewHeight = dwVideoHeight * fHeightMultiplier;
				fVideoViewWidth = dwVideoHeight * fWidthMultiplier;
			}
			
			float fYPadding = dwScreenHeight - fVideoViewHeight; 
			if(fYPadding < 0)
				fYPadding = 0;
			
			float fXPadding = dwScreenWidth - fVideoViewWidth; 
			if(fXPadding < 0)
				fXPadding = 0;
			
			//We set the top and bottom padding on the viewContainer to difference in video / screen height * 0.5
			mActivity.mAspectViewContainer.setPadding(0, (int)(fYPadding * 0.5f), 0, (int)(fYPadding * 0.5f));
			
			//start the video
			mMediaPlayer.seekTo(mdwSeekPosition);
			mMediaPlayer.start();
		}
	}
	//--------------------------------------------------------------
	/// on Error
	///
	/// Called when the video preparation step fails. This clears up
	/// the video and calls onCompletion.
	//--------------------------------------------------------------
	@Override public synchronized boolean onError(MediaPlayer inMediaPlayer, int indwWhat, int indwExtra) 
	{
		if (mMediaPlayer == inMediaPlayer)
		{
			Logging.logError("Media player has encountered an error while preparing.");
			onCompletion(inMediaPlayer);
			return true;
		}
		return false;
	}
	//--------------------------------------------------------------
	/// on Completion
	///
	/// Called when the video has finished playing. This tidies up 
	/// the video player and calls OnVideoStopped.
	//--------------------------------------------------------------
	@Override public synchronized void onCompletion(MediaPlayer inMediaPlayer) 
	{
		if (mMediaPlayer == inMediaPlayer)
		{
			Cleanup();
			mActivity.finish();
		}
	}
	//-----------------------------------------------------------------
	/// On Touch Event 
	///
	/// This records all touch events that occur.
	///
	/// @param the motion event.
	/// @return whether or not this view wants to pass the touch event
	///			on to the next applicable view.
	//-----------------------------------------------------------------
	@Override public synchronized boolean onTouchEvent(final MotionEvent event) 
	{	
		if (mbCanDismissWithTap == true)
		{
			int dwActionData = event.getAction();
			int dwActionEvent = dwActionData & MotionEvent.ACTION_MASK;
			if (dwActionEvent == MotionEvent.ACTION_DOWN)
			{
				mqwTapTime = System.currentTimeMillis();
			}
			else if (dwActionEvent == MotionEvent.ACTION_UP)
			{
				if (System.currentTimeMillis() - mqwTapTime <= kqwTapLengthInMS)
				{
					VideoPlayerNativeInterface mediaPlayerNI = (VideoPlayerNativeInterface)CSApplication.get().getSystem(VideoPlayerNativeInterface.InterfaceID);
					if (mediaPlayerNI != null)
					{
						mediaPlayerNI.Dismissed();
					}
					onCompletion(mMediaPlayer);
				}
			}
		}
		
        return true;
    }
	//---------------------------------------------------------------
	/// On Back Pressed
	///
	/// Called when the back button is pressed. This will dismiss
	/// the video if tap dismissal is enabled.
	//---------------------------------------------------------------
	public void OnBackPressed()
	{
		if (mbCanDismissWithTap == true)
		{
			VideoPlayerNativeInterface videoPlayerNI = (VideoPlayerNativeInterface)CSApplication.get().getSystem(VideoPlayerNativeInterface.InterfaceID);
			if (videoPlayerNI != null)
			{
				videoPlayerNI.Dismissed();
			}
			onCompletion(mMediaPlayer);
		}
	}
}
