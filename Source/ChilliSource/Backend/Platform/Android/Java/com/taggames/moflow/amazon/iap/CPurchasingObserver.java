//  
//  CPurchasingObserver.java
//  moFlow
//  
//  Created by Ian Copland on 10/12/2013
//  Copyright (c)2013 Tag Games Limited - All rights reserved
//  

package com.taggames.moflow.amazon.iap;

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
public class CPurchasingObserver extends BasePurchasingObserver
{
	//----------------------------------------------
	/// Member data
	//----------------------------------------------
	CAmazonIAPNativeInterface mAmazonNI = null;
	//----------------------------------------------
	/// Constructor
	///
	/// @param The context.
	/// @param The Amazon IAP native interface.
	//----------------------------------------------
	public CPurchasingObserver(Context inContext, CAmazonIAPNativeInterface inAmazonNI) 
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
