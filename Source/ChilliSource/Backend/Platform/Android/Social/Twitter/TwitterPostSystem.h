//
//  TwitterPostSystem.h
//  Chilli Source
//
//  Created by Robert Henning on 11/05/2012.
//  Copyright 2012 Tag Games. All rights reserved.
//

#ifndef _CHILLISOURCE_PLATFORM_ANDROID_SOCIAL_TWITTER_TWITTERPOSTSYSTEM_H_
#define _CHILLISOURCE_PLATFORM_ANDROID_SOCIAL_TWITTER_TWITTERPOSTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/Android/ForwardDeclarations.h>
#include <ChilliSource/Social/Twitter/TwitterPostSystem.h>

namespace ChilliSource
{
	namespace Android
	{
		//-------------------------------------------------------------------
		/// System that allows the user to post tweets to Twitter. User
		/// must first authenticate with the Twitter app key and secret before
		/// posting is allowed. Posts are restricted to standard twitter
		/// character limits. NOTE: currently only one post is allowed at a time.
		///
		/// This is the Android version which will posts using the Chilli Source
		/// implementation.
		///
		/// @author R Henning
		//-------------------------------------------------------------------
		class TwitterPostSystem final : public Social::TwitterPostSystem
		{
		public:
            //------------------------------------------------------------------------
            /// @author R Henning
            ///
            /// @param Interface ID
			///
            /// @return Is of interface ID type
            //------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //------------------------------------------------------------------------
            /// Run the OAuth process and, if successful, leave the system in state
            /// ready to communicate with Twitter
            ///
            /// @author S Downie
            ///
            /// @param Key from Twitter application
            /// @param Secret from Twitter application
            /// @param Result delegate
            //------------------------------------------------------------------------
            void Authenticate(const std::string& in_key, const std::string& in_secret, const AuthenticationResultDelegate& in_delegate) override;
			//------------------------------------------------------------------------
			/// @author R Henning
            ///
            /// @return Whether or not posting images using twitter is supported.
			//------------------------------------------------------------------------
			bool IsImagePostSupported() const override;
            //------------------------------------------------------------------------
			/// Attempt to post a tweet to Twitter
            ///
            /// @author R Henning
			///
			/// @param Twitter post description (i.e. image, text etc)
			/// @param Completion delegate
			//------------------------------------------------------------------------
			void Post(const Social::TwitterPostSystem::PostDesc& in_desc, const Social::TwitterPostSystem::PostResultDelegate& in_delegate) override;

		private:
			friend Social::TwitterPostSystemUPtr Social::TwitterPostSystem::Create(Networking::HttpRequestSystem*, Core::OAuthSystem*);
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            ///
            /// @param The http connection system.
            /// @param The OAuth system.
            //----------------------------------------------------
            TwitterPostSystem(Networking::HttpRequestSystem* in_httpConnectionSystem, Core::OAuthSystem* in_oauthSystem);
			//------------------------------------------------------------------------
			/// Delegate called when the user confirms entry of the PIN
			///
            /// @author R Henning
            ///
			/// @param PIN entered by user
			//------------------------------------------------------------------------
			void OnPINComplete(const Social::TwitterAuthenticationActivity::AuthenticationPINResult& in_result);
			//------------------------------------------------------------------------
			/// Delegate called with the authorisation view is dismissed.
			///
			/// @author R Henning
			///
			/// @param Pointer to Activity that has been dismissed
			//------------------------------------------------------------------------
			void OnAuthorisationDismissed(Core::Activity* in_activity);

		private:

			Core::ConnectionUPtr m_authorisationDismissedConnection;
		};
	}
}

#endif
