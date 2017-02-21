/**
 * SubtitlesView.java
 * ChilliSource
 * Created by Ian Copland on 25/02/2013.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2013 Tag Games Limited
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

import java.util.HashMap;
import java.util.Map;

import com.chilliworks.chillisource.core.CSApplication;
import com.chilliworks.chillisource.core.Logging;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

//=============================================================
/// Subtitles View
///
/// A view for displaying subtitles. This is typically displayed
/// over the video player view.
//=============================================================
public class SubtitlesView extends ViewGroup
{	
	//----------------------------------------------------------
	/// Subtitle
	///
	/// All information required to display a single subtitle.
	//----------------------------------------------------------
	class BoundsInfo
	{
		float fCentreX;
		float fCentreY;
		float fWidth;
		float fHeight;
	}
	//----------------------------------------------------------
	/// Member data
	//----------------------------------------------------------
	private VideoPlayer mVideoPlayer = null;
	private HashMap<Long, TextView> mTextViewMap = new HashMap<Long, TextView>();
	private HashMap<Long, BoundsInfo> mSubtitleMap = new HashMap<Long, BoundsInfo>();
	private long mlwNextID = 0;
	//----------------------------------------------------------
	/// Constructor
	//----------------------------------------------------------
	public SubtitlesView(Context context) 
	{
		super(context);
		mVideoPlayer = (VideoPlayer)CSApplication.get().getSystem(VideoPlayer.INTERFACE_ID);
		if (mVideoPlayer == null)
		{
			Logging.logError("Could not get the video player native interface!");
		}
		
		setWillNotDraw(false);
	}
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
	/// @output the ID of the new subtitle.
	//--------------------------------------------------------------
	public long CreateSubtitle(String instrText, String instrFontName, int indwFontSize, String instrAlignment, float infCentreX, float infCentreY, float infWidth, float infHeight)
	{
		long lwID = mlwNextID++;
		
		//create new subtitle info
		BoundsInfo info = new BoundsInfo();
		info.fCentreX = infCentreX;
		info.fCentreY = infCentreY;
		info.fWidth = infWidth;
		info.fHeight = infHeight;
		mSubtitleMap.put(lwID, info);
		
		//create a new text view
		DisplayMetrics metrics = new DisplayMetrics();
		VideoPlayerActivity.GetActivity().getWindowManager().getDefaultDisplay().getMetrics(metrics);
		TextView textView = new TextView(getContext());
		textView.setText(instrText);
		textView.setTextSize(indwFontSize);
		textView.setTextColor(Color.TRANSPARENT);
		SetTextAlignment(textView, instrAlignment);
		addView(textView);
		mTextViewMap.put(lwID, textView);
		return lwID;
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
		TextView textView = mTextViewMap.get(inlwSubtitleID);
		if (textView != null)
		{
			textView.setTextColor(Color.argb((int)(infAlpha * 255.0), (int)(infRed * 255.0), (int)(infGreen * 255.0), (int)(infBlue * 255.0)));
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
		TextView textView = mTextViewMap.get(inlwSubtitleID);
		if (textView != null)
		{
			removeView(textView);
			mTextViewMap.remove(inlwSubtitleID);
		}
	}
	//--------------------------------------------------------------
	/// Draw
	///
	/// This is called every time the view is invalidated. To insure
	/// This happens every frame, this calls postInvalidate.
	//--------------------------------------------------------------
	@Override public void onDraw(Canvas inCanvas)
	{
		mVideoPlayer.updateSubtitles();
		postInvalidate();
	}
	//--------------------------------------------------------------
	/// On Layout
	///
	/// Positions all subviews.
	//--------------------------------------------------------------
	@Override protected void onLayout(boolean inbChanged, int indwLeft, int indwTop, int indwRight, int indwBottom)
	{
		int dwWidth = indwRight - indwLeft;
		int dwHeight = indwBottom - indwTop;
		for (Map.Entry<Long, TextView> textView : mTextViewMap.entrySet())
		{
			BoundsInfo boundsInfo = mSubtitleMap.get(textView.getKey());

            // Converting from origin at bottom-left to top-left
            float fRelativeLeft = boundsInfo.fCentreX - boundsInfo.fWidth * 0.5f;
            float fRelativeTop = 1.0f - boundsInfo.fCentreY - boundsInfo.fHeight * 0.5f;
            float fRelativeRight = boundsInfo.fCentreX + boundsInfo.fWidth * 0.5f;
            float fRelativeBottom = 1.0f - boundsInfo.fCentreY + boundsInfo.fHeight * 0.5f;

            int dwAbsoluteLeft = (int)(dwWidth * fRelativeLeft);
			int dwAbsoluteTop = (int)(dwHeight * fRelativeTop);
            int dwAbsoluteRight = (int)(dwWidth * fRelativeRight);
            int dwAbsoluteBottom = (int)(dwHeight * fRelativeBottom);

			textView.getValue().measure(MeasureSpec.makeMeasureSpec(dwAbsoluteRight - dwAbsoluteLeft, MeasureSpec.EXACTLY), MeasureSpec.makeMeasureSpec(dwAbsoluteBottom - dwAbsoluteTop, MeasureSpec.EXACTLY));
			textView.getValue().layout(dwAbsoluteLeft, dwAbsoluteTop, dwAbsoluteRight, dwAbsoluteBottom);
		}
	}
	//--------------------------------------------------------------
	/// Set Text Alignment
	///
	/// Tests the alignment in the given text view.
	///
	/// @param The text view
	/// @param the alignment in string form.
	//--------------------------------------------------------------
	private void SetTextAlignment(TextView inTextView, String instrAlignment) 
	{
		int dwGravity = Gravity.NO_GRAVITY;
		if (instrAlignment.equalsIgnoreCase("TopLeft") == true)
		{
			dwGravity = Gravity.TOP | Gravity.LEFT;
		}
		else if (instrAlignment.equalsIgnoreCase("TopCentre") == true)
		{
			dwGravity = Gravity.TOP | Gravity.CENTER_HORIZONTAL;
		}
		else if (instrAlignment.equalsIgnoreCase("TopRight") == true)
		{
			dwGravity = Gravity.TOP | Gravity.RIGHT;
		}
		else if (instrAlignment.equalsIgnoreCase("MiddleLeft") == true)
		{
			dwGravity = Gravity.CENTER_VERTICAL | Gravity.LEFT;
		}
		else if (instrAlignment.equalsIgnoreCase("MiddleCentre") == true)
		{
			dwGravity = Gravity.CENTER;
		}
		else if (instrAlignment.equalsIgnoreCase("MiddleRight") == true)
		{
			dwGravity = Gravity.CENTER_VERTICAL | Gravity.RIGHT;
		}
		else if (instrAlignment.equalsIgnoreCase("BottomLeft") == true)
		{
			dwGravity = Gravity.BOTTOM | Gravity.LEFT;
		}
		else if (instrAlignment.equalsIgnoreCase("BottomCentre") == true)
		{
			dwGravity = Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL;
		}
		else if (instrAlignment.equalsIgnoreCase("BottomRight") == true)
		{
			dwGravity = Gravity.BOTTOM | Gravity.RIGHT;
		}
		inTextView.setGravity(dwGravity);
        LinearLayout.LayoutParams newParameters = new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        newParameters.gravity = dwGravity;
        inTextView.setLayoutParams(newParameters);
	}
}
