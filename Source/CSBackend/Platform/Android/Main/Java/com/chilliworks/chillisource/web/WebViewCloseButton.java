/**
 * WebViewCloseButton.java
 * Chilli Source
 * Created by Steven Hendrie on 16/12/2011.
 * 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2011 Tag Games Limited
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

package com.chilliworks.chillisource.web;

import com.chilliworks.chillisource.core.ResourceHelper;
import com.chilliworks.chillisource.web.WebViewNativeInterface;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RelativeLayout;

public class WebViewCloseButton extends Button
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
	public WebViewCloseButton(Context context, int inudwIndex, float infSize)
	{
		super(context);		
		
		mudwIndex = inudwIndex;
		
		if (ResourceHelper.DoesResourceExist(context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "com_chillisource_close_button") == true)
		{
			setBackgroundResource(ResourceHelper.GetDynamicResourceIDForField(context, ResourceHelper.RESOURCE_SUBCLASS.RESOURCE_DRAWABLE, "com_chillisource_close_button"));
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
				WebViewNativeInterface.Dismiss(mudwIndex);			
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