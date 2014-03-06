/*
 *  TwitterAuthenticationActivity.h
 *  moFlow
 *
 *  Created by Robert Henning on 09/05/2012.
 *  Copyright (c) 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_SOCIAL_TWITTER_TWITTER_AUTHENTICATION_ACTIVITY_H_
#define _MO_FLO_SOCIAL_TWITTER_TWITTER_AUTHENTICATION_ACTIVITY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Activity.h>
#include <ChilliSource/Core/Event/Event.h>

#include <functional>

namespace ChilliSource
{
	namespace Social
	{
		class TwitterAuthenticationActivity : public Core::Activity
		{
		public:
			CS_DECLARE_NAMEDTYPE(TwitterAuthenticationActivity);
            
			struct AuthenticationPINResult
			{
				std::string strPIN;	// PIN entered by user
			};
			
			typedef std::function<void(const AuthenticationPINResult&)> AuthenticationPINResultDelegate;
            
            //-----------------------------------------------
			/// Creates a new instance of the Twitter Authetication
            /// activity.
            ///
            /// @author I Copland
            ///
            /// @return The new instance.
			//-----------------------------------------------
			static TwitterAuthenticationActivityUPtr Create();
			//--------------------------------------------------------------
			/// Is A
			///
			/// @param Interface ID
			/// @param Whether activity is of given type
			//--------------------------------------------------------------
			bool IsA(Core::InterfaceIDType inID) const override;
			//-----------------------------------------------
			/// Present
			/// 
			/// Starts the activity by presenting a view with
			/// a 'busy, please wait' message
			//-----------------------------------------------
			virtual void Present() = 0;
			//-----------------------------------------------
			/// Presents the authorise URL
			/// 
			/// Set the URL received from Twitter to visit
			/// in order to see PIN
			///
			/// @param URL
			//-----------------------------------------------
			virtual void PresentURL(const std::string& instrURL) = 0;
			//-----------------------------------------------
			/// Save PIN
			/// 
			/// Saves the PIN entered by the user
			///
			/// @param PIN entered by user
			//-----------------------------------------------
			virtual bool SavePIN(const std::string& instrPIN) = 0;
			//-----------------------------------------------
			/// Dismiss
			/// 
			/// Dismiss the web view interface
			//-----------------------------------------------
			virtual void Dismiss() = 0;
			//-----------------------------------------------
			/// Get Dismissed Event 
			///
			/// @return Event triggered when activity is
			/// dismissed
			//-----------------------------------------------
			Core::IConnectableEvent<Core::ActivityDismissedEvent>& GetDismissedEvent();
			//-----------------------------------------------
			/// Set Authentication PIN Result Delegate
			///
			/// Sets the delegate to trigger when the user
			/// confirms PIN entry
			//-----------------------------------------------
			void SetAuthenticationPINResultDelegate(const ChilliSource::Social::TwitterAuthenticationActivity::AuthenticationPINResultDelegate inCallback);
            //-----------------------------------------------
			/// Destructor
            ///
            /// @author I Copland
			//-----------------------------------------------
            virtual ~TwitterAuthenticationActivity(){};
		protected:
            //-----------------------------------------------
			/// Protected constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
			//-----------------------------------------------
            TwitterAuthenticationActivity(){};
            
			Core::Event<Core::ActivityDismissedEvent> mOnDismissedEvent;
			TwitterAuthenticationActivity::AuthenticationPINResultDelegate mOnPINResultDelegate;
			std::string	mstrURL;
			std::string	mstrPIN;
		};
	}
}

#endif
