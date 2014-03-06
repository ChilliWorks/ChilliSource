/*
 *  TwitterPostSystem.h
 *  moFlow
 *
 *  Created by Robert Henning on 11/05/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_ANDROIDPLATFORM_SOCIAL_TWITTER_TWITTER_POST_SYSTEM_H_
#define _MO_FLO_ANDROIDPLATFORM_SOCIAL_TWITTER_TWITTER_POST_SYSTEM_H_

#include <ChilliSource/Backend/Platform/Android/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Core/Cryptographic/OAuthSystem.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Social/Twitter/TwitterAuthenticationActivity.h>
#include <ChilliSource/Social/Twitter/TwitterPostSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		class TwitterPostSystem: public Social::TwitterPostSystem
		{
		public:
			TwitterPostSystem(Android::HttpConnectionSystem* inpHttpConnectionSystem, Core::OAuthSystem* inpOAuthSystem);
			~TwitterPostSystem();
            //------------------------------------------------------------------------
            /// Is A
            ///
            /// @param Interface ID
            /// @return Is of interface ID type
            //------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType inInterfaceID) const override;
            //------------------------------------------------------------------------
            /// Run the OAuth process and, if successful, leave the system in state
            /// ready to communicate with Twitter
            ///
            /// @return Returns true if successful, otherwise false
            //------------------------------------------------------------------------
            bool Authenticate() override;
			//------------------------------------------------------------------------
			/// Is Image Post Supported
            ///
            /// @return Whether or not posting images using twitter is supported.
			//------------------------------------------------------------------------
			bool IsImagePostSupported() const override;
			//------------------------------------------------------------------------
			/// Try Post
			///
			/// Attempt to post a tweet to Twitter
			///
			/// @param Twitter post description (i.e. image, text etc)
			/// @param Completion delegate
			/// @param Post handle identifier
            ///
            /// @return If the tweet exceeds the character limit imposed by Twitter
			//------------------------------------------------------------------------
			bool TryPost(const Social::TwitterPostDesc & insDesc, const Social::TwitterPostSystem::PostResultDelegate & inResultCallback) override;

		private:
			//------------------------------------------------------------------------
			/// Delegate called when the user confirms entry of the PIN
			///
			/// @param PIN entered by user
			//------------------------------------------------------------------------
			void OnPINComplete(const Social::TwitterAuthenticationActivity::AuthenticationPINResult &inResult);
			//------------------------------------------------------------------------
			/// Delegate called with the authorisation view is dismissed.
			///
			/// @param Pointer to IActivity that has been dismissed
			//------------------------------------------------------------------------
			void OnAuthorisationDismissed(Core::Activity* inpActivity);

		private:

			Core::ConnectionUPtr m_authorisationDismissedConnection;
		};
	}
}

#endif
