package com.chillisource.core;

import com.chillisource.core.ResourceHelper;
import com.chillisource.core.CSActivity;

import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageView;

public class LoadingView 
{
	private static final String kstrDefaultImage = "com_chillisource_default";
	private CSActivity mActivity = null;
	private volatile ImageView mLoadingView = null;
	private volatile boolean mbIsPresented = false;
	//---------------------------------------------------
	/// Constructor
	///
	/// @param The moFlow activity.
	//---------------------------------------------------
	public LoadingView(CSActivity inActivity)
	{
		mActivity = inActivity;
	}
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
				mLoadingView = new ImageView(mActivity);
				mLoadingView.setLayoutParams(new ViewGroup.LayoutParams(WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.MATCH_PARENT));
				if (ResourceHelper.DoesResourceExist(mActivity, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, instrResource) == true)
				{
					mLoadingView.setImageResource(ResourceHelper.GetDynamicResourceIDForField(mActivity, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, instrResource));
				}
				else
				{
					mLoadingView.setImageResource(ResourceHelper.GetDynamicResourceIDForField(mActivity, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, kstrDefaultImage));
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
