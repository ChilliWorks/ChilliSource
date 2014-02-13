/*
 *  CWebViewCloseBUtton.java
 *  Android Webview Close Button
 *
 *  Created by Steven Hendrie on 10/12/11.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

package com.taggames.moflow.view.webview;

import com.taggames.moflow.core.CMoFlowResourceHelper;
import com.taggames.moflow.nativeinterface.CWebViewNativeInterface;
import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RelativeLayout;

public class CWebViewCloseButton extends Button
{
	final private int mudwIndex;
	
	//---------------------------------------------------------------------
	/// CCloseButton
	///
	/// Constructor for CCloseButton - sets layout and size of button.
	/// Override on click listener to remove views
	///
	/// @param Context
	/// @param index of webview that this will be associated with
	///
	//---------------------------------------------------------------------
	public CWebViewCloseButton(Context context, int inudwIndex, float infSize)
	{
		super(context);		
		
		mudwIndex = inudwIndex;
		
		if (CMoFlowResourceHelper.DoesResourceExist(context, CMoFlowResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "com_taggames_close_button") == true)
		{
			setBackgroundResource(CMoFlowResourceHelper.GetDynamicResourceIDForField(context, CMoFlowResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "com_taggames_close_button"));
		}
		else
		{
			setText("X");
		}
		
		RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams((int)infSize, (int)infSize);
		params.addRule(RelativeLayout.ALIGN_RIGHT, mudwIndex);
		params.addRule(RelativeLayout.ALIGN_TOP, mudwIndex);
		setLayoutParams(params);
		
		setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v) 
			{
				ViewGroup vg = (ViewGroup)(v.getParent());
				vg.removeView(v);
				CWebViewNativeInterface.Dismiss(mudwIndex);			
			}
		});
	}
	//---------------------------------------------------------------------
	/// Get Index
	///
	/// Get Index of webview associated with button
	///
	///	@returns int of index
	///
	//---------------------------------------------------------------------
	public int GetIndex()
	{
		return mudwIndex;
	}
}