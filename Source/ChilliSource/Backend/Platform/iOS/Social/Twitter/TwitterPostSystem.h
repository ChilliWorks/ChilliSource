/*
 *  TwitterPostSystem.h
 *  Chilli Source
 *
 *  Created by Robert Henning on 08/05/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _CHILLISOURCE_PLATFORM_IOS_SOCIAL_TWITTER_TWITTERPOSTSYSTEM_H_
#define _CHILLISOURCE_PLATFORM_IOS_SOCIAL_TWITTER_TWITTERPOSTSYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Social/Twitter/TwitterPostSystem.h>

namespace ChilliSource
{
    namespace iOS
    {
        class TwitterPostSystem : public ChilliSource::Social::TwitterPostSystem
        {
        public:
			
			//------------------------------------------------------------------------
            /// @author R Henning
            ///
            /// @param Interface ID
            ///
            /// @return Is of interface ID type
            //------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceID) const override;
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
            /// @author S Downie
            ///
            /// @return true if authentication has already occured, false otherwise
            //------------------------------------------------------------------------
            static bool SupportedByDevice();
			//------------------------------------------------------------------------
			/// @author I Copland
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
			//------------------------------------------------------------------------
			/// Delegate called when the user confirms entry of the PIN
			///
            /// @author R Henning
            ///
			/// @param PIN entered by user
			//------------------------------------------------------------------------
			void OnPINComplete(const ChilliSource::Social::TwitterAuthenticationActivity::AuthenticationPINResult& in_result);
			//------------------------------------------------------------------------
			/// Delegate called with the authorisation view is dismissed.
            ///
            /// @author R Henning
            ///
			/// @param Pointer to Activity that has been dismissed
			//------------------------------------------------------------------------
			void OnAuthorisationDismissed(Core::Activity* in_activity);
		private:
            //----------------------------------------------------
            /// Private constructor to force the use of the
            /// factory method.
            ///
            /// @author I Copland
            ///
            /// @param The http connection system.
            /// @param The OAuth system.
            //----------------------------------------------------
            TwitterPostSystem(Networking::HttpConnectionSystem* in_httpConnectionSystem, Core::OAuthSystem* in_oauthSystem);
			//------------------------------------------------------------------------
			/// Attempt to post a tweet to Twitter using the iOS Twitter framework
            ///
            /// @author R Henning
            ///
			/// @param Twitter post description (i.e. image, text etc)
			/// @param Completion delegate
			//------------------------------------------------------------------------
			void PostUsingiOS(const Social::TwitterPostSystem::PostDesc& in_desc, const Social::TwitterPostSystem::PostResultDelegate& in_delegate);
			//------------------------------------------------------------------------
			/// Attempt to post a tweet to Twitter using the Chilli Source implementation
            ///
            /// @author R Henning
            ///
			/// @param Twitter post description (i.e. image, text etc)
			/// @param Completion delegate
			//------------------------------------------------------------------------
			void PostUsingChilliSource(const Social::TwitterPostSystem::PostDesc& in_desc, const Social::TwitterPostSystem::PostResultDelegate& in_delegate);
            
            friend Social::TwitterPostSystemUPtr Social::TwitterPostSystem::Create(Networking::HttpConnectionSystem* inpHttpConnectionSystem, Core::OAuthSystem* inpOAuthSystem);
        private:
            
            Core::ConnectionUPtr m_authorisationDismissedConnection;
        };
    }
}


#endif
