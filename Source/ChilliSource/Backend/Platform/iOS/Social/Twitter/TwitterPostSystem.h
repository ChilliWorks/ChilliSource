/*
 *  TwitterPostSystem.h
 *  moFlow
 *
 *  Created by Robert Henning on 08/05/2012.
 *  Copyright 2012 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_IOSPLATFORM_SOCIAL_TWITTER_TWITTER_POST_SYSTEM_H_
#define _MO_FLO_IOSPLATFORM_SOCIAL_TWITTER_TWITTER_POST_SYSTEM_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Backend/Platform/iOS/ForwardDeclarations.h>
#include <ChilliSource/Backend/Platform/iOS/Networking/Http/HttpConnectionSystem.h>
#include <ChilliSource/Core/Cryptographic/OAuthSystem.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/System/System.h>
#include <ChilliSource/Social/Twitter/TwitterPostSystem.h>

namespace ChilliSource
{
    namespace iOS
    {
        class TwitterPostSystem : public ChilliSource::Social::TwitterPostSystem
        {
        public:
			
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
            /// Supported By Device
            ///
            // @return true if authentication has already occured, false otherwise
            //------------------------------------------------------------------------
            static bool SupportedByDevice();
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
			//------------------------------------------------------------------------
			/// Delegate called when the user confirms entry of the PIN
			///
			/// @param PIN entered by user
			//------------------------------------------------------------------------
			void OnPINComplete(const ChilliSource::Social::TwitterAuthenticationActivity::AuthenticationPINResult &inResult) override;
			//------------------------------------------------------------------------
			/// Delegate called with the authorisation view is dismissed.
			///
			/// @param Pointer to Activity that has been dismissed
			//------------------------------------------------------------------------
			void OnAuthorisationDismissed(Core::Activity* inpActivity) override;
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
            TwitterPostSystem(Networking::HttpConnectionSystem* inpHttpConnectionSystem, Core::OAuthSystem* inpOAuthSystem);
			//------------------------------------------------------------------------
			/// Try Post Using iOS
			///
			/// Attempt to post a tweet to Twitter using the iOS Twitter framework
			///
			/// @param Twitter post description (i.e. image, text etc)
			/// @param Completion delegate
			/// @param Post handle identifier
            ///
            /// @return If the tweet exceeds the character limit imposed by Twitter
			//------------------------------------------------------------------------
			bool TryPostUsingiOS(const Social::TwitterPostDesc & insDesc, const Social::TwitterPostSystem::PostResultDelegate & inResultCallback);
			//------------------------------------------------------------------------
			/// Try Post Using moFlow
			///
			/// Attempt to post a tweet to Twitter using the moFlow implementation
			///
			/// @param Twitter post description (i.e. image, text etc)
			/// @param Completion delegate
			/// @param Post handle identifier
            ///
            /// @return If the tweet exceeds the character limit imposed by Twitter
			//------------------------------------------------------------------------
			bool TryPostUsingMoFlow(const Social::TwitterPostDesc & insDesc, const Social::TwitterPostSystem::PostResultDelegate & inResultCallback);
            
            friend Social::TwitterPostSystemUPtr Social::TwitterPostSystem::Create(Networking::HttpConnectionSystem* inpHttpConnectionSystem, Core::OAuthSystem* inpOAuthSystem);
        private:
            
            Core::ConnectionUPtr m_authorisationDismissedConnection;
        };
    }
}


#endif
