/*
 *  TwitterAuthenticationActivity.h
 *  moFlow
 *
 *  Created by Robert Henning on 14/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_PLATFORM_ANDROID_SOCIAL_TWITTER_TWITTER_AUTHENTICATION_ACTIVITY_H_
#define _MO_FLO_PLATFORM_ANDROID_SOCIAL_TWITTER_TWITTER_AUTHENTICATION_ACTIVITY_H_

#include <ChilliSource/Social/Twitter/TwitterAuthenticationActivity.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>

namespace ChilliSource
{
	namespace Android
	{
		class CTwitterAuthenticationActivity : public Social::TwitterAuthenticationActivity
		{
		public:
			CTwitterAuthenticationActivity();
			//-----------------------------------------------
			/// Present
			/// 
			/// Starts the activity by presenting a view with
			/// a 'busy, please wait' message
			//-----------------------------------------------
			void Present();
			//-----------------------------------------------
			/// Presents the authorise URL
			/// 
			/// Set the URL received from Twitter to visit
			/// in order to see PIN
			///
			/// @param URL
			//-----------------------------------------------
			void PresentURL(const std::string& instrURL);
			//-----------------------------------------------
			/// Save PIN
			/// 
			/// Saves the PIN entered by the user
			///
			/// @param PIN number entered by the user
			//-----------------------------------------------
			bool SavePIN(const std::string& instrPIN);
			//-----------------------------------------------
			/// Dismiss
			/// 
			/// Dismiss the web view interface
			//-----------------------------------------------
			void Dismiss();
			//-----------------------------------------------
			/// On PIN Complete
			///
			/// Called when the user has entered a valid PIN
			//-----------------------------------------------
			void OnPINComplete();
			
			Core::UnifiedVector2			mvUnifiedSize;
			Core::Vector2					mvAbsoluteSize;
			Core::Vector2					mvAbsolutePositionTopLeft;
			Core::Vector2					mvAbsolutePositionCentre;
		};
	}
}

#endif
