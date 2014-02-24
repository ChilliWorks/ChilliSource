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
		DEFINE_NAMED_INTERFACE(ITwitterAuthenticationActivity);
        
        IActivity* ITwitterAuthenticationActivity::CreateTwitterAuthenticationActivity()
        {
#ifdef TARGET_OS_IPHONE
            return new moFlo::iOSPlatform::CTwitterAuthenticationActivity();
#elif TARGET_ANDROID
            return new moFlo::AndroidPlatform::CTwitterAuthenticationActivity();
#endif
			return NULL;
        }
        
		//--------------------------------------------------------------
		/// Is A
		///
		/// @param Interface ID
		/// @param Whether activity is of given type
		//--------------------------------------------------------------
		bool ITwitterAuthenticationActivity::IsA(Core::InterfaceIDType inID) const
		{
			return inID == ITwitterAuthenticationActivity::InterfaceID;
		}
		//-----------------------------------------------
		/// Get Dismissed Event 
		///
		/// @return Event triggered when activity is
		/// dismissed
		//-----------------------------------------------
		IEvent<ActivityDismissedEvent>& ITwitterAuthenticationActivity::GetDismissedEvent()
		{
			return mOnDismissedEvent;
		}
		//-----------------------------------------------
		/// Set Authentication PIN Result Delegate
		//-----------------------------------------------
		void ITwitterAuthenticationActivity::SetAuthenticationPINResultDelegate(const moFlo::Social::ITwitterAuthenticationActivity::AuthenticationPINResultDelegate inCallback)
		{
			mOnPINResultDelegate = inCallback;
		}
	}
}

