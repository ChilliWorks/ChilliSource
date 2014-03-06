/*
 *  TwitterAuthenticationActivity.cpp
 *  moFlow
 *
 *  Created by Robert Henning on 09/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Social/Twitter/TwitterAuthenticationActivity.h>

#include <ChilliSource/Core/Base/MakeDelegate.h>

#ifdef CS_TARGETPLATFORM_IOS
#include <ChilliSource/Backend/Platform/iOS/Social/Twitter/TwitterAuthenticationActivity.h>
#elif CS_TARGETPLATFORM_ANDROID
#include <ChilliSource/Backend/Platform/Android/Social/Twitter/TwitterAuthenticationActivity.h>
#endif

namespace ChilliSource
{
	namespace Social
	{
		CS_DEFINE_NAMEDTYPE(TwitterAuthenticationActivity);
        
        //-----------------------------------------------
        //-----------------------------------------------
        TwitterAuthenticationActivityUPtr TwitterAuthenticationActivity::Create()
        {
#ifdef CS_TARGETPLATFORM_IOS
            return TwitterAuthenticationActivityUPtr(new ChilliSource::iOS::TwitterAuthenticationActivity());
#elif CS_TARGETPLATFORM_ANDROID
            return TwitterAuthenticationActivityUPtr(new ChilliSource::Android::TwitterAuthenticationActivity());
#endif
			return TwitterAuthenticationActivityUPtr();
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
		Core::IConnectableEvent<Core::ActivityDismissedEvent>& TwitterAuthenticationActivity::GetDismissedEvent()
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

