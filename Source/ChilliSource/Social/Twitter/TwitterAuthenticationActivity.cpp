/*
 *  TwitterAuthenticationActivity.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 09/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Social/Twitter/TwitterAuthenticationActivity.h>

#ifdef TARGET_OS_IPHONE
#include <ChilliSource/Backend/Platform/iOS/Social/Twitter/TwitterAuthenticationActivity.h>
#elif TARGET_ANDROID
#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivity.h>
#endif

namespace ChilliSource
{
	namespace Social
	{
		DEFINE_NAMED_INTERFACE(TwitterAuthenticationActivity);
        
        Core::IActivity* TwitterAuthenticationActivity::CreateTwitterAuthenticationActivity()
        {
#ifdef TARGET_OS_IPHONE
            return new ChilliSource::iOS::CTwitterAuthenticationActivity();
#elif TARGET_ANDROID
            return new ChilliSource::Android::CTwitterAuthenticationActivity();
#endif
			return nullptr;
        }
        
		//--------------------------------------------------------------
		/// Is A
		///
		/// @param Interface ID
		/// @param Whether activity is of given type
		//--------------------------------------------------------------
		bool TwitterAuthenticationActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == TwitterAuthenticationActivity::InterfaceID;
		}
		//-----------------------------------------------
		/// Get Dismissed Event 
		///
		/// @return Event triggered when activity is
		/// dismissed
		//-----------------------------------------------
		Core::IEvent<Core::ActivityDismissedEvent>& TwitterAuthenticationActivity::GetDismissedEvent()
		{
			return mOnDismissedEvent;
		}
		//-----------------------------------------------
		/// Set Authentication PIN Result Delegate
		//-----------------------------------------------
		void TwitterAuthenticationActivity::SetAuthenticationPINResultDelegate(const ChilliSource::Social::TwitterAuthenticationActivity::AuthenticationPINResultDelegate inCallback)
		{
			mOnPINResultDelegate = inCallback;
		}
	}
}

