/**
 * LoadingView.java
 * Chilli Source
 * Created by Scott Downie on 09/09/2013.
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

package com.chilliworks.chillisource.core;

import com.chilliworks.chillisource.core.ResourceHelper;

import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageView;

public class LoadingView 
{
	private static final String kstrDefaultImage = "com_chillisource_default";
	private volatile ImageView mLoadingView = null;
	private volatile boolean mbIsPresented = false;

	//---------------------------------------------------
	/// Present
	///
	/// Displays the loading view over other views.
	///
	/// @param The name of the resource to be presented
	/// if the resource does not exist the default
	/// com_chillisource_default will be displayed instead.
	//---------------------------------------------------
	public void Present(final String instrResource)
	{
		if(IsPresented())
			return;
		
		mbIsPresented = true;
		
		CSApplication.get().scheduleUIThreadTask(new Runnable()
		{
			@Override 
			public void run() 
			{
				mLoadingView = new ImageView(CSApplication.get().getActivityContext());
				mLoadingView.setLayoutParams(new ViewGroup.LayoutParams(WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.MATCH_PARENT));
				if (ResourceHelper.DoesResourceExist(CSApplication.get().getActivityContext(), ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, instrResource) == true)
				{
					mLoadingView.setImageResource(ResourceHelper.GetDynamicResourceIDForField(CSApplication.get().getActivityContext(), ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, instrResource));
				}
				else
				{
					mLoadingView.setImageResource(ResourceHelper.GetDynamicResourceIDForField(CSApplication.get().getActivityContext(), ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, kstrDefaultImage));
				}
				mLoadingView.setScaleType(ImageView.ScaleType.CENTER_CROP);
				CSApplication.get().addView(mLoadingView);
				mLoadingView.bringToFront();
			}
		});
	}
	//---------------------------------------------------------------------
	/// Is Presented
	///
	/// @return Whether the view is presented or scheduled to be presented
	//---------------------------------------------------------------------
	public boolean IsPresented()
	{
		return mbIsPresented;
	}
	//---------------------------------------------------
	/// Dismiss
	///
	/// Dismisses the loading view if it is currently
	/// displayed.
	//---------------------------------------------------
	public void Dismiss()
	{
		if(mLoadingView == null)
			return;
		
		CSApplication.get().scheduleUIThreadTask(new Runnable()
		{
			@Override 
			public void run() 
			{
				CSApplication.get().removeView(mLoadingView);
				mLoadingView = null;
				mbIsPresented = false;
			}
		});
	}
}
