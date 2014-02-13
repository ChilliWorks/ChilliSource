/*
 *  WebView.cpp
 *  iOSTemplate
 *
 *  Created by Scott Downie on 25/07/2011.
 *  Copyright 2011 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/WebViewActivity.h>

namespace moFlo
{
	DEFINE_NAMED_INTERFACE(IWebViewActivity);
	//--------------------------------------------------------------
	/// Is A
	///
	/// @param Interface ID
	/// @param Whether activity is of given type
	//--------------------------------------------------------------
	bool IWebViewActivity::IsA(Core::InterfaceIDType inID) const
	{
		return inID == IWebViewActivity::InterfaceID;
	}
	//-----------------------------------------------
	/// Get Dismissed Event 
	///
	/// @return Event triggered when activity is
	/// dismissed
	//-----------------------------------------------
	IEvent<ActivityDismissedEvent>& IWebViewActivity::GetDismissedEvent()
	{
		return mOnDismissedEvent;
	}
}

