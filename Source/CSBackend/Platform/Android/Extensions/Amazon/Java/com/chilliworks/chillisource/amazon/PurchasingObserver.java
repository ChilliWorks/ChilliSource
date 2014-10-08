/**
 * PurchasingObserver.java
 * Chilli Source
 * Created by Ian Copland on 10/12/2013.
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

package com.chilliworks.chillisource.amazon;

import android.content.Context;

import com.amazon.inapp.purchasing.BasePurchasingObserver;
import com.amazon.inapp.purchasing.GetUserIdResponse;
import com.amazon.inapp.purchasing.ItemDataResponse;
import com.amazon.inapp.purchasing.PurchaseResponse;
import com.amazon.inapp.purchasing.PurchaseUpdatesResponse;

//====================================================
/// Purchasing Observer
///
/// An observer for receiving purchase events from the
/// Amazon IAP system.
//====================================================
public class PurchasingObserver extends BasePurchasingObserver
{
	//----------------------------------------------
	/// Member data
	//----------------------------------------------
	AmazonIAPNativeInterface mAmazonNI = null;
	//----------------------------------------------
	/// Constructor
	///
	/// @param The context.
	/// @param The Amazon IAP native interface.
	//----------------------------------------------
	public PurchasingObserver(Context inContext, AmazonIAPNativeInterface inAmazonNI) 
	{
		super(inContext);
		mAmazonNI = inAmazonNI;
	}
	//----------------------------------------------
	/// On Sdk Available
	///
	/// This is called when the observer is registered.
	/// It provides information on whether the 
	/// application is in sandbox mode or not. This
	/// passes the event on to the Amazon IAP Native
	/// Interface.
	///
	/// @param Whether or not this application is
	/// in sandbox mode.
	//----------------------------------------------
	@Override public void onSdkAvailable(final boolean inbIsSandboxMode)
	{
		if (mAmazonNI != null)
		{
			mAmazonNI.OnSDKAvailable(inbIsSandboxMode);
		}
	}
	//----------------------------------------------
	/// On Get User ID Response
	///
	/// Called in response to a request for the users
	/// amazon store id. This passes the response
	/// on to the Amazon IAP Native Interface.
	/// 
	/// @param The response.
	//----------------------------------------------
	@Override public void onGetUserIdResponse(final GetUserIdResponse inResponse) 
	{
		if (mAmazonNI != null)
		{
			mAmazonNI.OnGetUserIdResponse(inResponse);
		}
	}
	//----------------------------------------------
	/// On Item Data Response
	///
	/// Called in response to a request for item
	/// data. This passes the response on to the Amazon 
	/// IAP Native Interface.
	/// 
	/// @param The response.
	//----------------------------------------------
	@Override public void onItemDataResponse(final ItemDataResponse inResponse) 
	{
		if (mAmazonNI != null)
		{
			mAmazonNI.OnItemDataResponse(inResponse);
		}
	}
	//----------------------------------------------
	/// On Purchase Response
	///
	/// Called in response to a purchase request. 
	/// This passes the response on to the Amazon IAP 
	/// Native Interface.
	/// 
	/// @param The response.
	//----------------------------------------------
	@Override public void onPurchaseResponse(final PurchaseResponse inResponse) 
	{
		if (mAmazonNI != null)
		{
			mAmazonNI.OnPurchaseResponse(inResponse);
		}
	}
	//----------------------------------------------
	/// On Purchase Update Response
	///
	/// Called in response to a request for an update
	/// on purchases. This passes the response on to 
	/// the Amazon IAP Native Interface.
	/// 
	/// @param The response.
	//----------------------------------------------
	@Override public void onPurchaseUpdatesResponse(final PurchaseUpdatesResponse inResponse) 
	{
		if (mAmazonNI != null)
		{
			mAmazonNI.OnPurchaseUpdatesResponse(inResponse);
		}
	}
}
